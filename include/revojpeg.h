#ifndef __REVOJPEG_H__
#define __REVOJPEG_H__

#define _DEBUG_MIN_ 1
#define _DEBUG_STAT_ 2
#define _DEBUG_VERBOSE_ 4
#define _DEBUG_UNUSED_ 8

#ifndef _NO_DEBUG_
  #define _DEBUG_ 3
#endif

#define _MSE_HVS_ 1
#define _MSE_HVS_M_ 2
#define _MSE_METRIC_ (1)
#define _TRELLIS_ 0

/// Should be template, temporary use typedef
enum color_channel_t { Y=0, U=1, V=2 };
typedef unsigned int QUANT_TPL;
typedef short PIXEL_TPL;
typedef short DCT_TPL;
typedef short PIXEL_TPL;
enum run_mode_t {GOOD=0, FAST=1, PSNR=2};

void initialize_REVOJPEG(color_channel_t channel, int width, int height, float target_mse_common_block, float target_mse_worst_block_, run_mode_t mode);
void input_qtable(QUANT_TPL qtable[64]);
void input_by_pixels(PIXEL_TPL tpl_input_pixels[64], int x8y8);
void input_by_dct(DCT_TPL tpl_input_dct[64], int x8y8);
void run_revojpeg(QUANT_TPL tpl_output_qtable[64]);
void process_dct(DCT_TPL tpl_input_quantized_dct[64], DCT_TPL tpl_output_quantized_dct[64], int x8y8);

#endif
