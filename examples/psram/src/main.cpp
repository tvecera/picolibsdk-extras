/*
 * 256 KB Operations
 * +-------------+-------------------+-------------------+
 * | Operation   | Average Time (ms)	| Speed (Bytes/ms)  |
 * +-------------+-------------------+-------------------+
 * | PSRAM Write | ~48               | ~5461             |
 * | PSRAM Read  | ~39               | ~6722             |
 * | RAM Write   | ~12               | ~21845            |
 * | RAM Read    | ~13               | ~20165            |
 * +-------------+-------------------+-------------------+
 *
 * 480 KB Operations
 * +-------------+-------------------+-------------------+
 * | Operation   | Average Time (ms)	| Speed (Bytes/ms)  |
 * +-------------+-------------------+-------------------+
 * | PSRAM Write | ~91               | ~5401             |
 * | PSRAM Read  | ~73               | ~6733             |
 * | RAM Write   | ~23               | ~21370            |
 * | RAM Read    | ~24               | ~20480            |
 * +-------------+-------------------+-------------------+
 *
 * +---------------------+---------------+-------------+----------------------+-----------------------+
 * | Platform            | CPU           | Clock Speed | Theoretical Bytes/ms | Practical Bytes/ms    |
 * +---------------------+---------------+-------------+----------------------+-----------------------+
 * | Commodore 64        | MOS 6510      | 1 MHz       | ~1,000 bytes/ms      | ~500-800 bytes/ms     |
 * | Apple II            | MOS 6502      | 1 MHz       | ~1,000 bytes/ms      | ~400-700 bytes/ms     |
 * | Atari 800           | MOS 6502      | 1.79 MHz    | ~1,790 bytes/ms      | ~800-1,300 bytes/ms   |
 * | ZX Spectrum         | Zilog Z80     | 3.5 MHz     | ~3,500 bytes/ms      | ~1,500-2,500 bytes/ms |
 * | Nintendo NES        | Ricoh 2A03    | 1.79 MHz    | ~1,790 bytes/ms      | ~800-1,200 bytes/ms   |
 * | Commodore VIC-20    | MOS 6502      | 1 MHz       | ~1,000 bytes/ms      | ~400-700 bytes/ms     |
 * | BBC Micro           | MOS 6502      | 2 MHz       | ~2,000 bytes/ms      | ~900-1,500 bytes/ms   |
 * +---------------------+---------------+-------------+----------------------+-----------------------+
 *
 * Write Speed: RAM is approximately 4 times faster than PSRAM.
 * Read Speed: RAM is approximately 3 times faster than PSRAM.
 */

#include "psram.h"

volatile u8 mems_psram[480 * 1024 * 1] PSRAM;
volatile u8 mems_ram[480 * 1024 * 1];

int __no_inline_not_in_flash_func(main)() {
	psram_init();

	size_t m;
	u32 count = 1;
	u8 expected;
	u64 start_time = 0;
	u64 end_time = 0;
	u64 write_time = 0;
	size_t total_size = sizeof(mems_ram);

	WaitMs(5000);
	size_t psram_total_size = psram_size();
	printf("Total PSRAM size: %d\n", psram_total_size);

	while (True) {
		WaitMs(2000);

		printf("%lu:\n", count);

		start_time = Time64Fast();
		for (m = 0; m < total_size; m++) {
			mems_psram[m] = 0x5E;
		}
		end_time = Time64Fast();
		write_time = end_time - start_time;
		printf("PSRAM write %d in %llu us\n", total_size, write_time);

		start_time = Time64Fast();
		for (m = 0; m < total_size; m++) {
			expected = mems_psram[m];
		}
		end_time = Time64Fast();
		write_time = end_time - start_time;
		printf("PSRAM read %d in %llu us\n", total_size, write_time);
		printf("Expected: %d\n", expected);

		start_time = Time64Fast();
		for (m = 0; m < total_size; m++) {
			mems_ram[m] = 0x5E;
		}
		end_time = Time64Fast();
		write_time = end_time - start_time;
		printf("RAM write %d in %llu us\n", total_size, write_time);

		start_time = Time64Fast();
		for (m = 0; m < total_size; m++) {
			expected = mems_ram[m];
		}
		end_time = Time64Fast();
		write_time = end_time - start_time;
		printf("RAM read %d in %llu us\n", total_size, write_time);
		printf("Expected: %d\n", expected);

		count++;
		printf("------------------------------------------\n");
	}
}