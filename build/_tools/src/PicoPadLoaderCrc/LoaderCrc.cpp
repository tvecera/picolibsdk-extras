// PicoLibSDK - Alternative SDK library for Raspberry Pico, RP2040 and RP2350
// Copyright (c) 2023 Miroslav Nemecek, Panda38@seznam.cz, hardyplotter2@gmail.com
// 	https://github.com/Panda381/PicoLibSDK
//	https://www.breatharian.eu/hw/picolibsdk/index_en.html
//	https://github.com/pajenicko/picopad
//	https://picopad.eu/en/
// License:
//	This source code is freely available for any purpose, including commercial.
//	It is possible to take and modify the code or parts of it, without restriction.

#include <cstdio>
#include <cstdlib>

#pragma warning(disable : 4996) // function not safe

typedef unsigned char       u8;
typedef unsigned int        u32;

#define LOADER_SIZE		0x8000		// RP2040 loader
#define LOADER2_SIZE	0x10000		// RP2350 loader
u8* Buf = nullptr;
int BufNum;

u32 rev(u32 n)
{
	return ((n & 0xff) << 24) | ((n & 0xff00) << 8) | ((n & 0xff0000) >> 8) | ((n & 0xff000000) >> 24);
}

// simulate function from bootrom_misc.S
u32 crc32(const u8* r0, u32 r1)
{
	u32 r2 = 0xffffffff;
	u32 r3;
	u32 r4;
	u32 r5 = 0x04c11db7;
	for (; r1 > 0; r1--)
	{
		r4 = *r0;
		r3 = rev(r2);
		r4 ^= r3;
		r4 <<= 24;

		for (r3 = 8; r3 > 0; r3--)
		{
			if ((r4 & 0x80000000) != 0)
			{
				r4 <<= 1;
				r4 ^= r5;
			}
			else
				r4 <<= 1;
		}

		r2 <<= 8;
		r2 ^= r4;
		r0++;
	}

	return r2;
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("Syntax: LoaderCrc program.bin program.uf2\n");
		return 1;
	}

	// open input file
	FILE* f = fopen(argv[1], "rb");
	if (f == nullptr)
	{
		printf("File %s not found\n", argv[1]);
		return 1;
	}

	// get file size
	fseek(f, 0, SEEK_END);
	u32 size = (u32)ftell(f);
	fseek(f, 0, SEEK_SET);
	if (size <= LOADER_SIZE+51*4)
	{
		printf("Incorrect %s file size\n", argv[1]);
		fclose(f);
		return 1;
	}

	// create file buffer
	Buf = (u8*)malloc(size);
	if (Buf == nullptr)
	{
		printf("Memory error\n");
		fclose(f);
		return 1;
	}

	// load file
	u32 n = (u32)fread(Buf, 1, size, f);
	if (n != size)
	{
		printf("Cannot load file %s\n", argv[1]);
		return 1;
	}
	fclose(f);

	// check header of RP2040 (new modification)
	u32* h = (u32*)&Buf[LOADER_SIZE + 48*4]; // end of vector table
	int loadersize = LOADER_SIZE;
	int off = 51*4;
	if ((h[0] != 0x44415050) // magic "PPAD"
		|| (h[1] != 0x01234567) // application length
		|| (h[2] != 0x89ABCDEF) // application CRC
		|| (h[5] != 0x64617070)) // magic "ppad"
	{
		// check header of RP2350-ARM
		h = (u32*)&Buf[LOADER2_SIZE + 68*4]; // end of vector table
		loadersize = LOADER2_SIZE;
		off = 71*4;
		if ((h[0] != 0x44415050) // magic "PPAD"
			|| (h[1] != 0x01234567) // application length
			|| (h[2] != 0x89ABCDEF) // application CRC
			|| (h[5] != 0x64617070)) // magic "ppad"
		{
			// check header of RP2350-RISCV
			h = (u32*)&Buf[LOADER2_SIZE + 192*4]; // end of vector table
			loadersize = LOADER2_SIZE;
			off = 195*4;
			if ((h[0] != 0x44415050) // magic "PPAD"
				|| (h[1] != 0x01234567) // application length
				|| (h[2] != 0x89ABCDEF) // application CRC
				|| (h[5] != 0x64617070)) // magic "ppad"
			{
				printf("Incorrect format of %s\n", argv[1]);
				return 1;
			}
		}
	}

	// calculate CRC
	size -= loadersize + off; // rest of application
	h[1] = size;	// set size of rest of application
	u32 crc = crc32(&Buf[loadersize + off], size); // calculate CRC of rest of application
	h[2] = crc;

	// open for write
	f = fopen(argv[1], "r+b");
	if (f == nullptr)
	{
		printf("Cannot open %s for write\n", argv[1]);
		return 1;
	}

	// write new header
	fseek(f, loadersize + off - 2*4, SEEK_SET);
	n = (u32)fwrite(&Buf[loadersize + off - 2*4], 1, 2*4, f);
	if (n != 2*4)
	{
		printf("Cannot write to %s\n", argv[1]);
		return 1;
	}
	fclose(f);

	// open 2nd file
	f = fopen(argv[2], "r+b");
	if (f == nullptr)
	{
		printf("Cannot open %s for write\n", argv[2]);
		return 1;
	}

	// prepare file offset
	// - For now, it is assumed that the header will
	//   not lie on the 256 byte sector boundary
	//		RP2040: 0xC0, offset 192-215, OK
	//		RP235ARM: 0x110 - 0x100, offset 16-39, OK
	//		RP2350RISCV: 0x300 - 0x300, offset 0-23, OK
	off = off - 3*4;
	n = 2*loadersize + 32;
	while (off >= 256)
	{
		off -= 256;
		n += 512;
	}
	off += (int)n;

	// load header
	fseek(f, off, SEEK_SET);
	u32 h2[6];
	n = (u32)fread(h2, 1, 6*4, f);
	if (n != 6*4)
	{
		printf("Cannot read from %s\n", argv[2]);
		return 1;
	}

	// check header
	if ((h2[0] != 0x44415050) // magic "PPAD"
		|| (h2[1] != 0x01234567) // application length
		|| (h2[2] != 0x89ABCDEF) // application CRC
		|| (h2[5] != 0x64617070)) // magic "ppad"
	{
		printf("Incorrect format of %s\n", argv[2]);
		return 1;
	}

	// write new header
	fseek(f, off + 4, SEEK_SET);
	n = (u32)fwrite(&h[1], 1, 2*4, f);
	if (n != 2*4)
	{
		printf("Cannot write to %s\n", argv[2]);
		return 1;
	}
	fclose(f);

	return 0;
}
