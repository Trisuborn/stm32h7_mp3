// #include "ov2640.h"
// #include "ov2640_regs.h"

// #define OV2640_XCLK_FREQUENCY       (20000000)
// #define OV2640_NUM_ALLOWED_SIZES    (19)
// #define NUM_BRIGHTNESS_LEVELS       (5)
// #define NUM_CONTRAST_LEVELS         (5)
// #define NUM_SATURATION_LEVELS       (5)
// #define NUM_EFFECTS                 (9)
// #define REGLENGTH 8

// #define ov2640_delay HAL_Delay

// static const uint8_t allowed_sizes[OV2640_NUM_ALLOWED_SIZES] ={
//     FRAMESIZE_CIF,      // 352x288
//     FRAMESIZE_SIF,      // 352x240
//     FRAMESIZE_QQVGA,    // 160x120
//     FRAMESIZE_128X64,   // 128x64
//     FRAMESIZE_QVGA,     // 320x240
//     FRAMESIZE_VGA,      // 640x480
//     FRAMESIZE_HVGA,     // 480 * 320
//     FRAMESIZE_WVGA2,    // 752x480
//     FRAMESIZE_SVGA,     // 800x600
//     FRAMESIZE_XGA,      // 1024x768
//     FRAMESIZE_SXGA,     // 1280x1024
//     FRAMESIZE_UXGA,     // 1600x1200
// };

// //---------------------------------------
// static const uint8_t rgb565_regs[][2] ={
//     { BANK_SEL, BANK_SEL_DSP },
//     { REG_RESET, REG_RESET_DVP },
//     { IMAGE_MODE, IMAGE_MODE_RGB565 },
//     { 0xD7, 0x03 },
//     { 0xE1, 0x77 },
//     { REG_RESET, 0x00 },
//     { 0, 0 },
// };

// //-------------------------------------
// static const uint8_t jpeg_regs[][2] ={
//     { BANK_SEL, BANK_SEL_DSP },
//     { REG_RESET, REG_RESET_DVP },
//     { IMAGE_MODE, IMAGE_MODE_JPEG_EN | IMAGE_MODE_RGB565 }, // | IMAGE_MODE_HREF_VSYNC
//     { 0xd7, 0x03 },
//     { 0xe1, 0x77 },
//     //{QS,            0x0c},
//     { REG_RESET, 0x00 },
//     { 0, 0 },
// };

// static const uint8_t yuyv_regs[][2] ={
//     { BANK_SEL, BANK_SEL_DSP },
//     { REG_RESET, REG_RESET_DVP },
//     { IMAGE_MODE, IMAGE_MODE_YUV422 },
//     { 0xd7, 0x03 },
//     { 0x33, 0xa0 },
//     { 0xe5, 0x1f },
//     { 0xe1, 0x67 },
//     { REG_RESET, 0x00 },
//     { 0, 0 },
// };

// //--------------------------------------------------------------------
// static const uint8_t brightness_regs[NUM_BRIGHTNESS_LEVELS + 1][5] ={
//     { BPADDR, BPDATA, BPADDR, BPDATA, BPDATA },
//     { 0x00, 0x04, 0x09, 0x00, 0x00 }, /* -2 */
//     { 0x00, 0x04, 0x09, 0x10, 0x00 }, /* -1 */
//     { 0x00, 0x04, 0x09, 0x20, 0x00 }, /*  0 */
//     { 0x00, 0x04, 0x09, 0x30, 0x00 }, /* +1 */
//     { 0x00, 0x04, 0x09, 0x40, 0x00 }, /* +2 */
// };

// //----------------------------------------------------------------
// static const uint8_t contrast_regs[NUM_CONTRAST_LEVELS + 1][7] ={
//     { BPADDR, BPDATA, BPADDR, BPDATA, BPDATA, BPDATA, BPDATA },
//     { 0x00, 0x04, 0x07, 0x20, 0x18, 0x34, 0x06 }, /* -2 */
//     { 0x00, 0x04, 0x07, 0x20, 0x1c, 0x2a, 0x06 }, /* -1 */
//     { 0x00, 0x04, 0x07, 0x20, 0x20, 0x20, 0x06 }, /*  0 */
//     { 0x00, 0x04, 0x07, 0x20, 0x24, 0x16, 0x06 }, /* +1 */
//     { 0x00, 0x04, 0x07, 0x20, 0x28, 0x0c, 0x06 }, /* +2 */
// };

// //--------------------------------------------------------------------
// static const uint8_t saturation_regs[NUM_SATURATION_LEVELS + 1][5] ={
//     { BPADDR, BPDATA, BPADDR, BPDATA, BPDATA },
//     { 0x00, 0x02, 0x03, 0x28, 0x28 }, /* -2 */
//     { 0x00, 0x02, 0x03, 0x38, 0x38 }, /* -1 */
//     { 0x00, 0x02, 0x03, 0x48, 0x48 }, /*  0 */
//     { 0x00, 0x02, 0x03, 0x58, 0x58 }, /* +1 */
//     { 0x00, 0x02, 0x03, 0x68, 0x68 }, /* +2 */
// };

// //--------------------------------------------------------
// static const uint8_t OV2640_EFFECTS_TBL[NUM_EFFECTS][3]={
//     //00-0 05-0  05-1
//     { 0x00, 0x80, 0x80 }, // Normal
//     { 0x18, 0xA0, 0x40 }, // Blueish (cool light)
//     { 0x18, 0x40, 0xC0 }, // Redish (warm)
//     { 0x18, 0x80, 0x80 }, // Black and white
//     { 0x18, 0x40, 0xA6 }, // Sepia
//     { 0x40, 0x80, 0x80 }, // Negative
//     { 0x18, 0x50, 0x50 }, // Greenish
//     { 0x58, 0x80, 0x80 }, // Black and white negative
//     { 0x00, 0x80, 0x80 }, // Normal
// };

// //-----------------------------------------------------
// static uint8_t OV2640_WR_Reg(uint8_t reg, uint8_t data)
// {
//     Camera_WriteReg(&hcamera, reg, &data);
//     return 0;
// }

// //---------------------------------------
// static uint8_t OV2640_RD_Reg(uint8_t reg)
// {
//     uint8_t data;
//     Camera_ReadReg(&hcamera, reg, &data);
//     return data;
// }

// //------------------------------------------------
// static void wrSensorRegs(const uint8_t(*regs)[2])
// {
//     for ( int i = 0; regs[i][0]; i++ ) {
//         Camera_WriteReg(&hcamera, regs[i][0], &regs[i][1]);
//     }
// }

// //----------------
// static int reset()
// {
//     ov2640_delay(100);
//   // Reset all registers
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
//     OV2640_WR_Reg(COM7, COM7_SRST);
//     // Delay 5 ms
//     ov2640_delay(5);
//     wrSensorRegs(ov2640_Slow_regs);
//     // 30 ms
//     ov2640_delay(30);

//     return 0;
// }

// //----------------------------------------
// static int set_special_effect(uint8_t sde)
// {
//     if ( sde >= NUM_EFFECTS ) return -1;
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     OV2640_WR_Reg(BPADDR, 0x00);
//     OV2640_WR_Reg(BPDATA, OV2640_EFFECTS_TBL[sde][0]);
//     OV2640_WR_Reg(BPADDR, 0x05);
//     OV2640_WR_Reg(BPDATA, OV2640_EFFECTS_TBL[sde][1]);
//     OV2640_WR_Reg(BPDATA, OV2640_EFFECTS_TBL[sde][2]);

//     return 0;
// }

// //-----------------------------------
// static int set_exposure(int exposure)
// {
//     int ret = 0;
//     // Disable DSP
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_BYPAS);

//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
//     if ( exposure == -1 ) {
//         // get exposure
//         uint16_t exp = (uint16_t)(OV2640_RD_Reg(REG45) & 0x3f) << 10;
//         exp |= (uint16_t)OV2640_RD_Reg(AEC) << 2;
//         exp |= (uint16_t)OV2640_RD_Reg(REG04) & 0x03;
//         ret = (int)exp;
//     } else if ( exposure == -2 ) {
//         // disable auto exposure and gain
//         OV2640_WR_Reg(COM8, COM8_SET(0));
//     } else if ( exposure > 0 ) {
//         // set manual exposure
//         int max_exp = (dvp_cam_resolution[hcamera.framesize][0] <= 800) ? 672 : 1248;
//         if ( exposure > max_exp ) exposure = max_exp;
//         OV2640_WR_Reg(COM8, COM8_SET(0));
//         OV2640_WR_Reg(REG45, (uint8_t)((exposure >> 10) & 0x3f));
//         OV2640_WR_Reg(AEC, (uint8_t)((exposure >> 2) & 0xff));
//         OV2640_WR_Reg(REG04, (uint8_t)(exposure & 3));
//     } else {
//         // enable auto exposure and gain
//         OV2640_WR_Reg(COM8, COM8_SET(COM8_BNDF_EN | COM8_AGC_EN | COM8_AEC_EN));
//     }

//     // Enable DSP
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_EN);
//     return ret;
// }

// //-------------------------------------------
// static void _set_framesize(uint8_t framesize)
// {
//     uint8_t cbar, qsreg, com7;

//     // save color bar status and qs register
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
//     cbar = OV2640_RD_Reg(COM7) & COM7_COLOR_BAR;
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     qsreg = OV2640_RD_Reg(QS);

//     uint16_t w = dvp_cam_resolution[framesize][0];
//     uint16_t h = dvp_cam_resolution[framesize][1];
//     const uint8_t(*regs)[2];

//     if ( w <= dvp_cam_resolution[FRAMESIZE_SVGA][0] ) regs = OV2640_svga_regs;
//     else regs = OV2640_uxga_regs;

//     // Disable DSP
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_BYPAS);

//     // Write output width
//     OV2640_WR_Reg(ZMOW, (w >> 2) & 0xFF); // OUTW[7:0] (real/4)
//     OV2640_WR_Reg(ZMOH, (h >> 2) & 0xFF); // OUTH[7:0] (real/4)
//     OV2640_WR_Reg(ZMHH, ((h >> 8) & 0x04) | ((w >> 10) & 0x03)); // OUTH[8]/OUTW[9:8]

//     // Set CLKRC
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
//     //OV2640_WR_Reg(CLKRC, ov2640_sensor->night_mode);

//     // Write DSP input regsiters
//     wrSensorRegs(regs);

//     // restore color bar status
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
//     com7 = OV2640_RD_Reg(COM7) | cbar;
//     OV2640_WR_Reg(COM7, com7);

//     // restore qs register
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     OV2640_WR_Reg(QS, qsreg);

//     // Enable DSP
//     OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_EN);
// }

// //---------------------------------------------
// static int set_pixformat(pixformat_t pixformat)
// {
//     // Disable DSP
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_BYPAS);

//     switch ( pixformat ) {
//     case PIXFORMAT_RGB565:
//         //OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
//         //OV2640_WR_Reg(COM8, COM8_SET(COM8_BNDF_EN | COM8_AGC_EN | COM8_AEC_EN));
//         wrSensorRegs(rgb565_regs);
//         break;
//     case PIXFORMAT_JPEG:
//         //OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
//         //OV2640_WR_Reg(COM8, COM8_SET(COM8_BNDF_EN));
//         wrSensorRegs(jpeg_regs);
//         break;
//     case PIXFORMAT_YUV422:
//         wrSensorRegs(yuyv_regs);
//         break;
//     default:
//         // Enable DSP
//         OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//         OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_EN);
//         return -1;
//     }
//     _set_framesize(hcamera.framesize);
//     // Enable DSP (enabled in '_set_framesize'
//     //OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     //OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_EN);
//     // Delay 30 ms
//     //ov2640_delay(30);

//     return 0;
// }

// //-----------------------------------------
// static int set_framesize(framesize_t framesize)
// {
//     int i = OV2640_NUM_ALLOWED_SIZES;
//     for ( i=0; i < OV2640_NUM_ALLOWED_SIZES; i++ ) {
//         if ( allowed_sizes[i] == framesize ) break;
//     }
//     if ( i >= OV2640_NUM_ALLOWED_SIZES ) {
//         //LOGW(TAG, "Frame size %d not allowed", framesize);
//         return -1;
//     }

//     hcamera.framesize = framesize;
//     _set_framesize(framesize);

//     //delay 30 ms
//     ov2640_delay(30);

//     return 0;
// }

// //-------------------------------------------
// int ov2640_check_framesize(uint8_t framesize)
// {
//     int i = OV2640_NUM_ALLOWED_SIZES;
//     for ( i=0; i < OV2640_NUM_ALLOWED_SIZES; i++ ) {
//         if ( allowed_sizes[i] == framesize ) break;
//     }
//     if ( i >= OV2640_NUM_ALLOWED_SIZES ) return -1;
//     return 0;
// }

// //--------------------------------
// static int set_contrast(int level)
// {
//     level += (NUM_CONTRAST_LEVELS / 2) + 1;
//     if ( level < 0 || level > NUM_CONTRAST_LEVELS ) {
//         return -1;
//     }

//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_BYPAS);
//     // Write contrast registers
//     for ( int i=0; i < sizeof(contrast_regs[0]) / sizeof(contrast_regs[0][0]); i++ ) {
//         OV2640_WR_Reg(contrast_regs[0][i], contrast_regs[level][i]);
//     }
//     OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_EN);

//     return 0;
// }

// //----------------------------------
// static int set_brightness(int level)
// {
//     level += (NUM_BRIGHTNESS_LEVELS / 2) + 1;
//     if ( (level < 0) || (level > NUM_BRIGHTNESS_LEVELS) ) return -1;

//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_BYPAS);
//     // Write brightness registers
//     for ( int i=0; i < sizeof(brightness_regs[0]) / sizeof(brightness_regs[0][0]); i++ ) {
//         OV2640_WR_Reg(brightness_regs[0][i], brightness_regs[level][i]);
//     }
//     OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_EN);

//     return 0;
// }

// //----------------------------------
// static int set_saturation(int level)
// {
//     level += (NUM_SATURATION_LEVELS / 2) + 1;
//     if ( (level < 0) || (level > NUM_SATURATION_LEVELS) ) return -1;

//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_BYPAS);
//     // Write saturation registers
//     for ( int i=0; i < sizeof(saturation_regs[0]) / sizeof(saturation_regs[0][0]); i++ ) {
//         OV2640_WR_Reg(saturation_regs[0][i], saturation_regs[level][i]);
//     }
//     OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_EN);

//     return 0;
// }

// //----------------------------
// static int set_quality(int qs)
// {
//     if ( (qs < 2) || (qs > 60) ) return -1;

//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     // Write QS register
//     OV2640_WR_Reg(QS, qs);

//     return 0;
// }

// //---------------------------------
// static int set_colorbar(int enable)
// {
//     uint8_t reg;
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
//     reg = OV2640_RD_Reg(COM7);

//     if ( enable ) reg |= COM7_COLOR_BAR;
//     else reg &= ~COM7_COLOR_BAR;

//     return OV2640_WR_Reg(COM7, reg);
// }

// //--------------------------------
// static int set_hmirror(int enable)
// {
//     uint8_t reg;
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
//     reg = OV2640_RD_Reg(REG04);

//     if ( !enable ) { // Already mirrored.
//         reg |= REG04_HFLIP_IMG;
//     } else {
//         reg &= ~REG04_HFLIP_IMG;
//     }

//     return OV2640_WR_Reg(REG04, reg);
// }

// //------------------------------
// static int set_vflip(int enable)
// {
//     uint8_t reg;
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
//     reg = OV2640_RD_Reg(REG04);

//     if ( !enable ) { // Already flipped.
//         reg |= REG04_VFLIP_IMG | REG04_VREF_EN;
//     } else {
//         reg &= ~(REG04_VFLIP_IMG | REG04_VREF_EN);
//     }

//     return OV2640_WR_Reg(REG04, reg);
// }

// //---------------------------------------------------------
// static int read_id(uint16_t* manuf_id, uint16_t* device_id)
// {
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
//     *manuf_id = ((uint16_t)OV2640_RD_Reg(0x1C) << 8) | OV2640_RD_Reg(0x1D);
//     *device_id = ((uint16_t)OV2640_RD_Reg(0x0A) << 8) | OV2640_RD_Reg(0x0B);
//     return 0;
// }

// //------------------------------------
// static int read_reg(uint16_t reg_addr)
// {
//     return (int)OV2640_RD_Reg((uint8_t)reg_addr);
// }

// //--------------------------------------------------------
// static int write_reg(uint16_t reg_addr, uint16_t reg_data)
// {
//     return (int)OV2640_WR_Reg((uint8_t)reg_addr, (uint8_t)reg_data);
// }

// static const uint8_t OV2640_LIGHTMODE_TBL[5][3]=
// {
//     { 0x5e, 0x41, 0x54 }, //Auto, NOT used
//     { 0x5e, 0x41, 0x54 }, //Sunny
//     { 0x52, 0x41, 0x66 }, //Office
//     { 0x65, 0x41, 0x4f }, //Cloudy
//     { 0x42, 0x3f, 0x71 }, //Home
// };


// //-------------------------------------
// static int set_light_mode(uint8_t mode)
// {
//     if ( mode > 4 ) return -1;

//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     if ( mode == 0 ) {
//         OV2640_WR_Reg(0xc7, 0x00); // AWB on
//     } else {
//         OV2640_WR_Reg(0xc7, 0x40); // AWB off
//         OV2640_WR_Reg(0xcc, OV2640_LIGHTMODE_TBL[mode][0]);
//         OV2640_WR_Reg(0xcd, OV2640_LIGHTMODE_TBL[mode][1]);
//         OV2640_WR_Reg(0xce, OV2640_LIGHTMODE_TBL[mode][2]);
//     }
//     return 0;
// }

// //-----------------------------------
// static int set_night_mode(int enable)
// {
//     // Disable DSP
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_BYPAS);
//     // Set CLKRC
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
//     OV2640_WR_Reg(CLKRC, (enable) ? 0 : CLKRC_DOUBLE);
//     // Enable DSP
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_EN);
//     //delay 30 ms
//     ov2640_delay(30);
//     return 0;
// }

// //===============================
// int ov2640_init(framesize_t framesize)
// {
//     reset();
//     hcamera.framesize = framesize;
//     hcamera.pixformat = PIXFORMAT_RGB565;
//     //set_framesize(FRAMESIZE_QQVGA);
//     set_pixformat(hcamera.pixformat);
//     set_hmirror(0);
//     set_vflip(0);
//     return 0;
// }

/************************************************
 * @file ov2640.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 1.0
 * @date 2021-01-21
 *
 * @copyright Copyright (c) 2021
 *
 *************************************************/
#include "pro_conf.h"

#include "ov2640.h"
#include "ov2640_regs.h"
#include "camera_i2c.h"

#if USER_USE_RTTHREAD == 1
#define LOG_TAG       "CAM_CORE"
#include <drv_log.h>

#else
// #define LOG_I     rt_kprintf
// #define LOG_D     rt_kprintf
// #define LOG_E     rt_kprintf
#endif


static void ov2640_init(void);
static uint8_t read_reg(uint8_t reg, uint8_t len, uint8_t* buf);
static uint8_t write_reg(uint8_t reg, uint8_t data);
static void ov2640_jpeg_mode(void);
static void ov2640_rgb565_mode(void);
static void ov2640_set_auto_exposure(uint8_t level);
static void ov2640_set_light_mode(uint8_t mode);
static void ov2640_set_color_saturation(uint8_t sat);
static void ov2640_set_brightness(uint8_t bright);
static void ov2640_set_contrast(uint8_t contrast);
static void ov2640_set_special_effects(uint8_t eft);
static void ov2640_set_window_size(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height);
static uint8_t ov2640_set_image_out_size(uint16_t width, uint16_t height);
static uint8_t ov2640_set_image_window_size(uint16_t offx, uint16_t offy, uint16_t width, uint16_t height);
static uint8_t ov2640_set_image_size(uint16_t width, uint16_t height);


struct camera_ov_t cam_ov2640 ={ 0 };
struct camera_ops cam_ov2640_ops ={
    .init = ov2640_init
};

static uint8_t read_reg(uint8_t reg, uint8_t len, uint8_t* buf)
{
    return cam_ov2640.i2c_ops->read_reg(cam_ov2640.dev_addr, reg, buf, len);
}

/* i2c write reg */
static uint8_t write_reg(uint8_t reg, uint8_t data)
{
    return cam_ov2640.i2c_ops->writ_reg(cam_ov2640.dev_addr, reg, &data, 1);
}

/* change ov2640 to jpeg mode */
static void ov2640_jpeg_mode(void)
{
    uint16_t i = 0;
    /* set yun422 mode */
    // for ( i = 0; i < (sizeof(ov2640_yuv422_reg_tbl) / 2); i++ ) {
    //     write_reg(ov2640_yuv422_reg_tbl[i][0], ov2640_yuv422_reg_tbl[i][1]);
    // }
    for ( i = 0; ov2640_yuv422_reg_tbl[i][0]; i++ ) {
        write_reg(ov2640_yuv422_reg_tbl[i][0], ov2640_yuv422_reg_tbl[i][1]);
    }

    /* set jpeg mode */
    // for ( i=0;i < (sizeof(ov2640_jpeg_reg_tbl) / 2);i++ ) {
    //     write_reg(ov2640_jpeg_reg_tbl[i][0], ov2640_jpeg_reg_tbl[i][1]);
    // }
    for ( i = 0; ov2640_jpeg_reg_tbl[i][0]; i++ ) {
        write_reg(ov2640_jpeg_reg_tbl[i][0], ov2640_jpeg_reg_tbl[i][1]);
    }
}

/* change ov2640 to rgb565 mode */
static void ov2640_rgb565_mode(void)
{
    uint16_t i=0;
    // for ( i = 0; i < (sizeof(ov2640_rgb565_reg_tbl) / 2); i++ ) {
    //     write_reg(ov2640_rgb565_reg_tbl[i][0], ov2640_rgb565_reg_tbl[i][1]);
    // }
    for ( i = 0; i < ov2640_rgb565_reg_tbl[i][0]; i++ ) {
        write_reg(ov2640_rgb565_reg_tbl[i][0], ov2640_rgb565_reg_tbl[i][1]);
    }
}

/* set auto exposure */
static void ov2640_set_auto_exposure(uint8_t level)
{
    uint8_t i = 0;
    uint8_t* p = (uint8_t*)OV2640_AUTOEXPOSURE_LEVEL[level];
    for ( i = 0; i < 4; i++ ) {
        write_reg(p[i * 2], p[i * 2 + 1]);
    }
}

/* set light mode
 * 0: auto
 * 1: sunny
 * 2: cloudy
 * 3: office
 * 4: home
 * */
static void ov2640_set_light_mode(uint8_t mode)
{
    uint8_t regccval, regcdval, regceval;

    switch ( mode ) {
    case 0:
        write_reg(0xFF, 0x00);
        write_reg(0xC7, 0x10);
        return;

    case 2:
        regccval = 0x65;
        regcdval = 0x41;
        regceval = 0x4F;
        break;

    case 3:
        regccval = 0x52;
        regcdval = 0x41;
        regceval = 0x66;
        break;

    case 4:
        regccval = 0x42;
        regcdval = 0x3F;
        regceval = 0x71;
        break;

    default:
        regccval = 0x5E;
        regcdval = 0x41;
        regceval = 0x54;
        break;
    }

    write_reg(0xFF, 0x00);
    write_reg(0xC7, 0x40);
    write_reg(0xCC, regccval);
    write_reg(0xCD, regcdval);
    write_reg(0xCE, regceval);
}

/* set color saturation
 * 0: -2
 * 1: -1
 * 2: 0
 * 3: +1
 * 4: +2
 * */
static void ov2640_set_color_saturation(uint8_t sat)
{
    uint8_t reg7dval = ((sat + 2) << 4) | 0x08;
    write_reg(0xFF, 0X00);
    write_reg(0x7C, 0X00);
    write_reg(0x7D, 0X02);
    write_reg(0x7C, 0X03);
    write_reg(0x7D, reg7dval);
    write_reg(0x7D, reg7dval);
}

/* set brightness
 * 0: -2
 * 1: -1
 * 2: 0
 * 3: 1
 * 4: 2
 * */
static void ov2640_set_brightness(uint8_t bright)
{
    write_reg(0xff, 0x00);
    write_reg(0x7c, 0x00);
    write_reg(0x7d, 0x04);
    write_reg(0x7c, 0x09);
    write_reg(0x7d, bright << 4);
    write_reg(0x7d, 0x00);
}

/* set contrast
 * 0: -2
 * 1: -1
 * 2: 0
 * 3: 1
 * 4: 2
 * */
static void ov2640_set_contrast(uint8_t contrast)
{
    uint8_t reg7d0val, reg7d1val;

    switch ( contrast ) {
    case 0:
        reg7d0val = 0x18;
        reg7d1val = 0x34;
        break;

    case 1:
        reg7d0val = 0x1C;
        reg7d1val = 0x2A;
        break;

    case 3:
        reg7d0val = 0x24;
        reg7d1val = 0x16;
        break;

    case 4:
        reg7d0val = 0x28;
        reg7d1val = 0x0C;
        break;

    default:
        reg7d0val = 0x20;
        reg7d1val = 0x20;
        break;
    }
    write_reg(0xff, 0x00);
    write_reg(0x7c, 0x00);
    write_reg(0x7d, 0x04);
    write_reg(0x7c, 0x07);
    write_reg(0x7d, 0x20);
    write_reg(0x7d, reg7d0val);
    write_reg(0x7d, reg7d1val);
    write_reg(0x7d, 0x06);
}

/* set special effects
 * 0: noraml
 * 1: negative film
 * 2: black-and-white
 * 3: the red
 * 4: the green
 * 5: the blue
 * 6: Retro
*/
static void ov2640_set_special_effects(uint8_t eft)
{
    uint8_t reg7d0val, reg7d1val, reg7d2val;

    switch ( eft ) {
    case 1:
        reg7d0val = 0x40;
        break;
    case 2:
        reg7d0val = 0x18;
        break;
    case 3:
        reg7d0val = 0x18;
        reg7d1val = 0x40;
        reg7d2val = 0xC0;
        break;
    case 4:
        reg7d0val = 0x18;
        reg7d1val = 0x40;
        reg7d2val = 0x40;
        break;
    case 5:
        reg7d0val = 0x18;
        reg7d1val = 0xA0;
        reg7d2val = 0x40;
        break;
    case 6:
        reg7d0val = 0x18;
        reg7d1val = 0x40;
        reg7d2val = 0xA6;
        break;
    default:
        reg7d0val = 0x00;
        reg7d1val = 0x80;
        reg7d2val = 0x80;
        break;
    }
    write_reg(0xff, 0x00);
    write_reg(0x7c, 0x00);
    write_reg(0x7d, reg7d0val);
    write_reg(0x7c, 0x05);
    write_reg(0x7d, reg7d1val);
    write_reg(0x7d, reg7d2val);
}

/* set the image output window */
static void ov2640_set_window_size(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height)
{
    uint16_t endx;
    uint16_t endy;
    uint8_t temp;
    endx = sx + width / 2;
    endy = sy + height / 2;

    write_reg(0xFF, 0x01);
    read_reg(0x03, 1, &temp);
    temp &= 0xF0;
    temp |= ((endy & 0x03) << 2) | (sy & 0x03);
    write_reg(0x03, temp);
    write_reg(0x19, sy >> 2);
    write_reg(0x1A, endy >> 2);

    read_reg(0x32, 1, &temp);
    temp &= 0xC0;
    temp |= ((endx & 0x07) << 3) | (sx & 0x07);
    write_reg(0x32, temp);
    write_reg(0x17, sx >> 3);
    write_reg(0x18, endx >> 3);
}

/* set the image output size */
static uint8_t ov2640_set_image_out_size(uint16_t width, uint16_t height)
{
    uint16_t outh, outw;
    uint8_t temp;

    if ( width % 4 )return 1;
    if ( height % 4 )return 2;
    outw = width / 4;
    outh = height / 4;
    write_reg(0xFF, 0x00);
    write_reg(0xE0, 0x04);
    write_reg(0x5A, outw & 0XFF);
    write_reg(0x5B, outh & 0XFF);
    temp = (outw >> 8) & 0x03;
    temp |= (outh >> 6) & 0x04;
    write_reg(0x5C, temp);
    write_reg(0xE0, 0X00);

    return RT_EOK;
}

/* set the image window size */
static uint8_t ov2640_set_image_window_size(uint16_t offx, uint16_t offy, uint16_t width, uint16_t height)
{
    uint16_t hsize, vsize;
    uint8_t temp;
    if ( (width % 4) || (height % 4) ) {
        return RT_ERROR;
    }
    hsize = width / 4;
    vsize = height / 4;
    write_reg(0XFF, 0X00);
    write_reg(0XE0, 0X04);
    write_reg(0X51, hsize & 0XFF);
    write_reg(0X52, vsize & 0XFF);
    write_reg(0X53, offx & 0XFF);
    write_reg(0X54, offy & 0XFF);
    temp=(vsize >> 1) & 0X80;
    temp|=(offy >> 4) & 0X70;
    temp|=(hsize >> 5) & 0X08;
    temp|=(offx >> 8) & 0X07;
    write_reg(0X55, temp);
    write_reg(0X57, (hsize >> 2) & 0X80);
    write_reg(0XE0, 0X00);
    return 0;
}

/* set output resolution */
static uint8_t ov2640_set_image_size(uint16_t width, uint16_t height)
{
    uint8_t temp;
    write_reg(0xFF, 0x00);
    write_reg(0xE0, 0x04);
    write_reg(0xC0, (width >> 3) & 0xFF);
    write_reg(0xC1, (height >> 3) & 0xFF);
    temp  = (width & 0x07) << 3;
    temp |= height & 0x07;
    temp |= (width >> 4) & 0x80;
    write_reg(0x8C, temp);
    write_reg(0xE0, 0x00);

    return RT_EOK;
}


#define QS                      0x44
#define HSIZE                   0x51
#define VSIZE                   0x52
#define XOFFL                   0x53
#define YOFFL                   0x54
#define VHYX                    0x55
#define DPRP                    0x56
#define TEST                    0x57
#define ZMOW                    0x5A
#define ZMOH                    0x5B
#define ZMHH                    0x5C
#define BPADDR                  0x7C
#define BPDATA                  0x7D
#define SIZEL                   0x8C
#define HSIZE8                  0xC0
#define VSIZE8                  0xC1
#define CTRL1                   0xC3
#define MS_SP                   0xF0
#define SS_ID                   0xF7
#define SS_CTRL                 0xF7
#define MC_AL                   0xFA
#define MC_AH                   0xFB
#define MC_D                    0xFC
#define P_CMD                   0xFD
#define P_STATUS                0xFE

#define CTRLI                   0x50
#define CTRLI_LP_DP             0x80
#define CTRLI_ROUND             0x40

#define CTRL0                   0xC2
#define CTRL0_AEC_EN            0x80
#define CTRL0_AEC_SEL           0x40
#define CTRL0_STAT_SEL          0x20
#define CTRL0_VFIRST            0x10
#define CTRL0_YUV422            0x08
#define CTRL0_YUV_EN            0x04
#define CTRL0_RGB_EN            0x02
#define CTRL0_RAW_EN            0x01

#define CTRL2                   0x86
#define CTRL2_DCW_EN            0x20
#define CTRL2_SDE_EN            0x10
#define CTRL2_UV_ADJ_EN         0x08
#define CTRL2_UV_AVG_EN         0x04
#define CTRL2_CMX_EN            0x01

#define CTRL3                   0x87
#define CTRL3_BPC_EN            0x80
#define CTRL3_WPC_EN            0x40
#define R_DVP_SP                0xD3
#define R_DVP_SP_AUTO_MODE      0x80

#define R_BYPASS                0x05
#define R_BYPASS_DSP_EN         0x00
#define R_BYPASS_DSP_BYPAS      0x01

#define IMAGE_MODE              0xDA
#define IMAGE_MODE_Y8_DVP_EN    0x40
#define IMAGE_MODE_JPEG_EN      0x10
#define IMAGE_MODE_YUV422       0x00
#define IMAGE_MODE_RAW10        0x04
#define IMAGE_MODE_RGB565       0x08
#define IMAGE_MODE_HREF_VSYNC   0x02
#define IMAGE_MODE_LBYTE_FIRST  0x01
#define IMAGE_MODE_GET_FMT(x)   ((x)&0xC)

#define REG_RESET                   0xE0
#define REG_RESET_MICROC            0x40
#define REG_RESET_SCCB              0x20
#define REG_RESET_JPEG              0x10
#define REG_RESET_DVP               0x04
#define REG_RESET_IPU               0x02
#define REG_RESET_CIF               0x01

#define MC_BIST                 0xF9
#define MC_BIST_RESET           0x80
#define MC_BIST_BOOT_ROM_SEL    0x40
#define MC_BIST_12KB_SEL        0x20
#define MC_BIST_12KB_MASK       0x30
#define MC_BIST_512KB_SEL       0x08
#define MC_BIST_512KB_MASK      0x0C
#define MC_BIST_BUSY_BIT_R      0x02
#define MC_BIST_MC_RES_ONE_SH_W 0x02
#define MC_BIST_LAUNCH          0x01

#define BANK_SEL                0xFF
#define BANK_SEL_DSP            0x00
#define BANK_SEL_SENSOR         0x01

/* Sensor register bank FF=0x01*/

#define GAIN                0x00
#define COM1                0x03
#define REG_PID             0x0A
#define REG_VER             0x0B
#define COM4                0x0D
#define AEC                 0x10

#define CLKRC               0x11
#define CLKRC_DOUBLE        0x80
#define CLKRC_2X_UXGA       (0x01 | CLKRC_DOUBLE)
#define CLKRC_2X_SVGA       CLKRC_DOUBLE
#define CLKRC_2X_CIF        CLKRC_DOUBLE
#define CLKRC_DIVIDER_MASK  0x3F

#define COM10               0x15
#define HSTART              0x17
#define HSTOP               0x18
#define VSTART              0x19
#define VSTOP               0x1A
#define MIDH                0x1C
#define MIDL                0x1D
#define AEW                 0x24
#define AEB                 0x25
#define REG2A               0x2A
#define FRARL               0x2B
#define ADDVSL              0x2D
#define ADDVSH              0x2E
#define YAVG                0x2F
#define HSDY                0x30
#define HEDY                0x31
#define ARCOM2              0x34
#define REG45               0x45
#define FLL                 0x46
#define FLH                 0x47
#define COM19               0x48
#define ZOOMS               0x49
#define COM22               0x4B
#define COM25               0x4E
#define BD50                0x4F
#define BD60                0x50
#define REG5D               0x5D
#define REG5E               0x5E
#define REG5F               0x5F
#define REG60               0x60
#define HISTO_LOW           0x61
#define HISTO_HIGH          0x62

#define REG04               0x04
#define REG04_DEFAULT       0x28
#define REG04_HFLIP_IMG     0x80
#define REG04_VFLIP_IMG     0x40
#define REG04_VREF_EN       0x10
#define REG04_HREF_EN       0x08
#define REG04_SET(x)        (REG04_DEFAULT|x)

#define REG08               0x08
#define COM2                0x09
#define COM2_STDBY          0x10
#define COM2_OUT_DRIVE_1x   0x00
#define COM2_OUT_DRIVE_2x   0x01
#define COM2_OUT_DRIVE_3x   0x02
#define COM2_OUT_DRIVE_4x   0x03

#define COM3                0x0C
#define COM3_DEFAULT        0x38
#define COM3_BAND_50Hz      0x04
#define COM3_BAND_60Hz      0x00
#define COM3_BAND_AUTO      0x02
#define COM3_BAND_SET(x)    (COM3_DEFAULT|x)

#define COM7                0x12
#define COM7_SRST           0x80
#define COM7_RES_UXGA       0x00 /* UXGA */
#define COM7_RES_SVGA       0x40 /* SVGA */
#define COM7_RES_CIF        0x20 /* CIF  */
#define COM7_ZOOM_EN        0x04 /* Enable Zoom */
#define COM7_COLOR_BAR      0x02 /* Enable Color Bar Test */
#define COM7_GET_RES(x)     ((x)&0x70)

#define COM8                0x13
#define COM8_DEFAULT        0xC0
#define COM8_BNDF_EN        0x20 /* Enable Banding filter */
#define COM8_AGC_EN         0x04 /* AGC Auto/Manual control selection */
#define COM8_AEC_EN         0x01 /* Auto/Manual Exposure control */
#define COM8_SET(x)         (COM8_DEFAULT|x)
#define COM8_SET_AEC(r,x)   (((r)&0xFE)|((x)&1))

#define COM9                0x14 /* AGC gain ceiling */
#define COM9_DEFAULT        0x08
#define COM9_AGC_GAIN_2x    0x00 /* AGC:    2x */
#define COM9_AGC_GAIN_4x    0x01 /* AGC:    4x */
#define COM9_AGC_GAIN_8x    0x02 /* AGC:    8x */
#define COM9_AGC_GAIN_16x   0x03 /* AGC:   16x */
#define COM9_AGC_GAIN_32x   0x04 /* AGC:   32x */
#define COM9_AGC_GAIN_64x   0x05 /* AGC:   64x */
#define COM9_AGC_GAIN_128x  0x06 /* AGC:  128x */
#define COM9_AGC_SET(x)     (COM9_DEFAULT|(x<<5))

#define CTRL1_AWB           0x08 /* Enable AWB */

#define VV                  0x26
#define VV_AGC_TH_SET(h,l)  ((h<<4)|(l&0x0F))

#define REG32               0x32
#define REG32_UXGA          0x36
#define REG32_SVGA          0x09
#define REG32_CIF           0x00

#define VAL_SET(x, mask, rshift, lshift) ((((x) >> rshift) & mask) << lshift)

#define CTRLI_V_DIV_SET(x)      VAL_SET(x, 0x3, 0, 3)
#define CTRLI_H_DIV_SET(x)      VAL_SET(x, 0x3, 0, 0)

#define SIZEL_HSIZE8_11_SET(x)  VAL_SET(x, 0x1, 11, 6)
#define SIZEL_HSIZE8_SET(x)     VAL_SET(x, 0x7, 0, 3)
#define SIZEL_VSIZE8_SET(x)     VAL_SET(x, 0x7, 0, 0)

#define HSIZE8_SET(x)           VAL_SET(x, 0xFF, 3, 0)
#define VSIZE8_SET(x)           VAL_SET(x, 0xFF, 3, 0)

#define HSIZE_SET(x)            VAL_SET(x, 0xFF, 2, 0)
#define VSIZE_SET(x)            VAL_SET(x, 0xFF, 2, 0)

#define XOFFL_SET(x)            VAL_SET(x, 0xFF, 0, 0)
#define YOFFL_SET(x)            VAL_SET(x, 0xFF, 0, 0)

#define VHYX_VSIZE_SET(x)       VAL_SET(x, 0x1, (8+2), 7)
#define VHYX_HSIZE_SET(x)       VAL_SET(x, 0x1, (8+2), 3)
#define VHYX_YOFF_SET(x)        VAL_SET(x, 0x3, 8, 4)
#define VHYX_XOFF_SET(x)        VAL_SET(x, 0x3, 8, 0)

#define TEST_HSIZE_SET(x)       VAL_SET(x, 0x1, (9+2), 7)

#define ZMOW_OUTW_SET(x)        VAL_SET(x, 0xFF, 2, 0)
#define ZMOH_OUTH_SET(x)        VAL_SET(x, 0xFF, 2, 0)

#define ZMHH_ZSPEED_SET(x)      VAL_SET(x, 0x0F, 0, 4)
#define ZMHH_OUTH_SET(x)        VAL_SET(x, 0x1, (8+2), 2)
#define ZMHH_OUTW_SET(x)        VAL_SET(x, 0x3, (8+2), 0)


#define CIF_WIDTH                   (400)
#define CIF_HEIGHT                  (296)
#define SVGA_WIDTH                  (800)
#define SVGA_HEIGHT                 (600)
#define UXGA_WIDTH                  (1600)
#define UXGA_HEIGHT                 (1200)
#define SVGA_HSIZE                  (800)
#define SVGA_VSIZE                  (600)
#define UXGA_HSIZE                  (1600)
#define UXGA_VSIZE                  (1200)

const uint8_t OV2640_svga_regs[][2] ={
    { BANK_SEL, BANK_SEL_SENSOR },
    /* DSP input image resoultion and window size control */
        { COM7, COM7_RES_SVGA },
        { COM1, 0x0F }, /* UXGA=0x0F, SVGA=0x0A, CIF=0x06 */
        { REG32, 0x09 }, /* UXGA=0x36, SVGA/CIF=0x09 */

        { HSTART, 0x11 }, /* UXGA=0x11, SVGA/CIF=0x11 */
        { HSTOP, 0x43 }, /* UXGA=0x75, SVGA/CIF=0x43 */

        { VSTART, 0x00 }, /* UXGA=0x01, SVGA/CIF=0x00 */
        { VSTOP, 0x4b }, /* UXGA=0x97, SVGA/CIF=0x4b */
        { 0x3d, 0x38 }, /* UXGA=0x34, SVGA/CIF=0x38 */

        { 0x35, 0xda },
        { 0x22, 0x1a },
        { 0x37, 0xc3 },
        { 0x34, 0xc0 },
        { 0x06, 0x88 },
        { 0x0d, 0x87 },
        { 0x0e, 0x41 },
        { 0x42, 0x03 },

        /* Set DSP input image size and offset.
           The sensor output image can be scaled with OUTW/OUTH */
        { BANK_SEL, BANK_SEL_DSP },
        { R_BYPASS, R_BYPASS_DSP_BYPAS },

        { REG_RESET, REG_RESET_DVP },
        { HSIZE8, (SVGA_HSIZE >> 3) }, /* Image Horizontal Size HSIZE[10:3] */
        { VSIZE8, (SVGA_VSIZE >> 3) }, /* Image Vertiacl Size VSIZE[10:3] */

        /* {HSIZE[11], HSIZE[2:0], VSIZE[2:0]} */
        { SIZEL, ((SVGA_HSIZE >> 6) & 0x40) | ((SVGA_HSIZE & 0x7) << 3) | (SVGA_VSIZE & 0x7) },

        { XOFFL, 0x00 }, /* OFFSET_X[7:0] */
        { YOFFL, 0x00 }, /* OFFSET_Y[7:0] */
        { HSIZE, ((SVGA_HSIZE >> 2) & 0xFF) }, /* H_SIZE[7:0]= HSIZE/4 */
        { VSIZE, ((SVGA_VSIZE >> 2) & 0xFF) }, /* V_SIZE[7:0]= VSIZE/4 */

        /* V_SIZE[8]/OFFSET_Y[10:8]/H_SIZE[8]/OFFSET_X[10:8] */
        { VHYX, ((SVGA_VSIZE >> 3) & 0x80) | ((SVGA_HSIZE >> 7) & 0x08) },
        { TEST, (SVGA_HSIZE >> 4) & 0x80 }, /* H_SIZE[9] */

        { CTRL2, CTRL2_DCW_EN | CTRL2_SDE_EN |
        CTRL2_UV_AVG_EN | CTRL2_CMX_EN | CTRL2_UV_ADJ_EN },

      /* H_DIVIDER/V_DIVIDER */
        { CTRLI, CTRLI_LP_DP | 0x00 },
        /* DVP prescalar */
        { R_DVP_SP, R_DVP_SP_AUTO_MODE },

        { R_BYPASS, R_BYPASS_DSP_EN },
        { REG_RESET, 0x00 },
        { 0, 0 },
};

const uint8_t ov2640_Slow_regs[][2] ={
    { 0xff, 0x01 },
    { 0x12, 0x80 },
    { 0xff, 0x00 },
    { 0x2c, 0xff },
    { 0x2e, 0xdf },
    { 0xff, 0x01 },
    { 0x3c, 0x32 },
    { 0x11, 0x00 }, // clk div 0
    { 0x09, 0x02 },
    { 0x04, 0xD8 }, //0x88
    { 0x13, 0xe5 },
    { 0x14, 0x48 },
    { 0x2c, 0x0c },
    { 0x33, 0x78 },
    { 0x3a, 0x33 },
    { 0x3b, 0xfb },
    { 0x3e, 0x00 },
    { 0x43, 0x11 },
    { 0x16, 0x10 },
    { 0x39, 0x92 },
    { 0x35, 0xda },
    { 0x22, 0x1a },
    { 0x37, 0xc3 },
    { 0x23, 0x00 },
    { 0x34, 0xc0 },
    { 0x36, 0x1a },
    { 0x06, 0x88 },
    { 0x07, 0xc0 },
    { 0x0d, 0x87 },
    { 0x0e, 0x41 },
    { 0x4c, 0x00 },
    { 0x48, 0x00 },
    { 0x5b, 0x00 },
    { 0x42, 0x03 },
    { 0x4a, 0x81 },
    { 0x21, 0x99 },
    { 0x24, 0x40 },
    { 0x25, 0x38 },
    { 0x26, 0x82 },
    { 0x5c, 0x00 },
    { 0x63, 0x00 },
    { 0x46, 0x22 },
    { 0x0c, 0x3c },
    { 0x61, 0x70 },
    { 0x62, 0x80 },
    { 0x7c, 0x05 },
    { 0x20, 0x80 },
    { 0x28, 0x30 },
    { 0x6c, 0x00 },
    { 0x6d, 0x80 },
    { 0x6e, 0x00 },
    { 0x70, 0x02 },
    { 0x71, 0x94 },
    { 0x73, 0xc1 },
    { 0x3d, 0x34 },
    { 0x5a, 0x57 },
    { 0x12, 0x40 },
    { 0x17, 0x11 },
    { 0x18, 0x43 },
    { 0x19, 0x00 },
    { 0x1a, 0x4b },
    { 0x32, 0x09 },
    { 0x37, 0xc0 },
    { 0x4f, 0xca },
    { 0x50, 0xa8 },
    { 0x5a, 0x23 },
    { 0x6d, 0x00 },
    { 0x3d, 0x38 },
    { 0xff, 0x00 },
    { 0xe5, 0x7f },
    { 0xf9, 0xc0 },
    { 0x41, 0x24 },
    { 0xe0, 0x14 },
    { 0x76, 0xff },
    { 0x33, 0xa0 },
    { 0x42, 0x20 },
    { 0x43, 0x18 },
    { 0x4c, 0x00 },
    { 0x87, 0xd5 },
    { 0x88, 0x3f },
    { 0xd7, 0x03 },
    { 0xd9, 0x10 },
    { 0xd3, 0x82 },
    { 0xc8, 0x08 },
    { 0xc9, 0x80 },
    { 0x7c, 0x00 },
    { 0x7d, 0x00 },
    { 0x7c, 0x03 },
    { 0x7d, 0x48 },
    { 0x7d, 0x48 },
    { 0x7c, 0x08 },
    { 0x7d, 0x20 },
    { 0x7d, 0x10 },
    { 0x7d, 0x0e },
    { 0x90, 0x00 },
    { 0x91, 0x0e },
    { 0x91, 0x1a },
    { 0x91, 0x31 },
    { 0x91, 0x5a },
    { 0x91, 0x69 },
    { 0x91, 0x75 },
    { 0x91, 0x7e },
    { 0x91, 0x88 },
    { 0x91, 0x8f },
    { 0x91, 0x96 },
    { 0x91, 0xa3 },
    { 0x91, 0xaf },
    { 0x91, 0xc4 },
    { 0x91, 0xd7 },
    { 0x91, 0xe8 },
    { 0x91, 0x20 },
    { 0x92, 0x00 },
    { 0x93, 0x06 },
    { 0x93, 0xe3 },
    { 0x93, 0x05 },
    { 0x93, 0x05 },
    { 0x93, 0x00 },
    { 0x93, 0x04 },
    { 0x93, 0x00 },
    { 0x93, 0x00 },
    { 0x93, 0x00 },
    { 0x93, 0x00 },
    { 0x93, 0x00 },
    { 0x93, 0x00 },
    { 0x93, 0x00 },
    { 0x96, 0x00 },
    { 0x97, 0x08 },
    { 0x97, 0x19 },
    { 0x97, 0x02 },
    { 0x97, 0x0c },
    { 0x97, 0x24 },
    { 0x97, 0x30 },
    { 0x97, 0x28 },
    { 0x97, 0x26 },
    { 0x97, 0x02 },
    { 0x97, 0x98 },
    { 0x97, 0x80 },
    { 0x97, 0x00 },
    { 0x97, 0x00 },
    { 0xc3, 0xed },
    { 0xa4, 0x00 },
    { 0xa8, 0x00 },
    { 0xc5, 0x11 },
    { 0xc6, 0x51 },
    { 0xbf, 0x80 },
    { 0xc7, 0x10 },
    { 0xb6, 0x66 },
    { 0xb8, 0xa5 },
    { 0xb7, 0x64 },
    { 0xb9, 0x7c },
    { 0xb3, 0xaf },
    { 0xb4, 0x97 },
    { 0xb5, 0xff },
    { 0xb0, 0xc5 },
    { 0xb1, 0x94 },
    { 0xb2, 0x0f },
    { 0xc4, 0x5c },
    { 0xc0, 0x64 },
    { 0xc1, 0x4b },
    { 0x8c, 0x00 },
    { 0x86, 0x3d },
    { 0x50, 0x00 },
    { 0x51, 0xc8 },
    { 0x52, 0x96 },
    { 0x53, 0x00 },
    { 0x54, 0x00 },
    { 0x55, 0x00 },
    { 0x5a, 0xc8 },
    { 0x5b, 0x96 },
    { 0x5c, 0x00 },
    { 0xd3, 0x02 },
    { 0xc3, 0xed },
    { 0x7f, 0x00 },
    { 0xda, 0x08 },
    { 0xe5, 0x1f },
    { 0xe1, 0x67 },
    { 0xe0, 0x00 },
    { 0xdd, 0x7f },
    { 0x05, 0x00 },
    { 0xff, 0x00 },
    { 0xe0, 0x04 },
    { 0x5a, 0x50 },
    { 0x5b, 0x3c },
    { 0x5c, 0x00 },
    { 0xe0, 0x00 },
    { 0x00, 0x00 },
};

static void ov2640_init(void)
{
    // rt_thread_mdelay(100);
    // write_reg(0xFF, 0x01);
    // write_reg(COM7, COM7_SRST);
    // rt_thread_mdelay(5);

    /* make camera with svga */
    // for ( uint16_t i = 0; ov2640_svga_init_reg_tbl[i][0]; i++ ) {
    //     write_reg(ov2640_svga_init_reg_tbl[i][0], ov2640_svga_init_reg_tbl[i][1]);
    // }

    for ( uint16_t i = 0; ov2640_Slow_regs[i][0]; i++ ) {
        write_reg(ov2640_Slow_regs[i][0], ov2640_Slow_regs[i][1]);
    }

    rt_thread_mdelay(30);

    // for ( uint16_t i = 0; OV2640_svga_regs[i][0]; i++ ) {
    //     write_reg(OV2640_svga_regs[i][0], OV2640_svga_regs[i][1]);
    // }

    // ov2640_rgb565_mode();
    // ov2640_set_light_mode(0);
    // ov2640_set_color_saturation(3);
    // ov2640_set_brightness(4);
    // ov2640_set_contrast(3);
    // ov2640_jpeg_mode();
    // ov2640_set_image_window_size(0, 0, 160, 120);
    // ov2640_set_image_out_size(160, 120);

}

