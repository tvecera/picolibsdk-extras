#ifndef HPCALC_ROM_H
#define HPCALC_ROM_H

#ifdef __cplusplus
extern "C" {
#endif

bool rom_read_object_file(const sim_t *sim, const char *file_name, rom_word_t *data);

#ifdef __cplusplus
	}
#endif


#endif //HPCALC_ROM_H
