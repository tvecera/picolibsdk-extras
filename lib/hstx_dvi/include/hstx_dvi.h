#ifndef HSTX_DVI_H
#define HSTX_DVI_H

#if USE_HSTX_DVI && !RP2040

#ifdef __cplusplus
extern "C" {
#endif

u32 *hstx_dvi_init(uint width, uint height, uint color_depth);

Bool hstx_dvi_start();

void hstx_dvi_term();

#ifdef __cplusplus
}
#endif

#endif // USE_HSTX_DVI

#endif //HSTX_DVI_H
