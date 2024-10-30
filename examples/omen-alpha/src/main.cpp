#if USE_PSRAM
#include "psram.h"
#endif
#include "hw/platform.h"
#include "hw/memory.h"
#include "hal/cpu.h"
#include "hw/ports.h"
#include "sw/alpha-mon-v4/alpha_mon_v4.c"
#include "sw/applications/tm1638.c"

// Read memory
unsigned char FASTCODE NOFLASH(emu_read_mem)(unsigned short addr) {
	return memory_read(addr);
}

// Write memory
void FASTCODE NOFLASH(emu_write_mem)(unsigned short addr, unsigned char data) {
	memory_write(addr, data);
}

// Read port
u8 FASTCODE NOFLASH(emu_read_port)(unsigned char addr) {
	addr &= 0xFF;

	if (port_input[addr]) {
		return (*port_input[addr])(addr);
	}
	return 0xFF;
}

// Write port
void FASTCODE NOFLASH(emu_write_port)(unsigned char addr, unsigned char data) {
	addr &= 0xFF;

	if (port_output[addr]) {
		(*port_output[addr])(addr, data);
	}
}

int main() {
	// Find system clock in Hz that sets the most accurate PWM clock frequency.
	u32 sysclk = PWM_FindSysClk(EMU_CLKSYS_MIN * MHZ, EMU_CLKSYS_MAX * MHZ, EMU_FREQ * cpu_clock_mul());

	// Setup system clock to get precise frequency of emulation
	if (sysclk > 160 * MHZ) VregSetVoltage(VREG_VOLTAGE_1_20);
	ClockPllSysFreq((sysclk + 500) / 1000);

	WaitMs(5000);

#if USE_PSRAM
	psram_init();
	DEBUG_PRINT("PSRAM initialized: %d\r\n", psram_size());
#endif

	cpu_config_t cpu_config = {
			.freq = EMU_FREQ,
			.pc = 0,
			.memory_read_cb = emu_read_mem,
			.memory_write_cb = emu_write_mem,
			.port_read_cb = emu_read_port,
			.port_write_cb = emu_write_port
	};

	memory_init(&omen_alpha_config);
	rom_load(0, alpha_mon_v4, alpha_mon_v4_len);
	app_load(0, tm1638, tm1638_len);
	ports_init();

	while (True) {
		// initialize device
		cpu_init(&cpu_config);

		// run program
		u32 freq = cpu_start();
		DEBUG_PRINT("CPU freq: %lu\r\n", freq);

		while (True) {
			ports_task(cpu_clock());
		}

		cpu_stop();
		ports_term();
		memory_term();
		ResetToBootLoader();
	}
}