#ifndef HPCALC_SIM_H
#define HPCALC_SIM_H

#ifdef __cplusplus
extern "C" {
#endif

struct sim_thread_vars_t {
	uint64_t *ram_data;
	uint64_t last_run_time_us;
};

typedef struct chip_mem_t {
	char *addr_space;
	addr_t base_addr;
	addr_t size;
	bank_mask_t bank_mask;
} chip_mem_t;

typedef struct calculator_chip_t {
	char *id;
	chip_type_t type;
	int index;
	int32_t flags;
	int memory_count;
	chip_mem_t memory[5];
} calculator_chip_t;

typedef struct calculator_t {
	char *ncd_copyright;
	char *ncd_license;
	char *model_name;
	int platform;
	int arch;
	int arch_variant;
	int ram_size;
	double clock_frequency; // in Hz
	bool key_scanner_as_flags;
	int lcd_digits;
	int lcd_segments;
	int chip_count;
	uint64_t ram_data[8];
	calculator_chip_t chip[];
} calculator_t;

void sim_update_display(sim_t *sim);

void sim_term(sim_t *sim);

void sim_run(sim_t *sim);

void sim_display_init(sim_t *sim);

#ifdef __cplusplus
}
#endif


#endif //HPCALC_SIM_H
