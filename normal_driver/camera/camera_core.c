/************************************************
 * @file camera_core.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 1.0
 * @date 2021-01-20
 *
 * @copyright Copyright (c) 2021
 *
 *************************************************/
#include "pro_conf.h"
#include "camera_i2c.h"
#include "camera_core.h"
#include "ov2640.h"

#if USER_USE_RTTHREAD == 1
#define LOG_TAG       "CAM_CORE"
#include <drv_log.h>

#else
// #define LOG_I     rt_kprintf
// #define LOG_D     rt_kprintf
// #define LOG_E     rt_kprintf
#endif

const uint16_t dvp_cam_resolution[][2] ={
    { 0, 0 },
    // C/SIF Resolutions
    { 88, 72 },	        /* QQCIF     */
    { 176, 144 },       /* QCIF      */
    { 352, 288 },       /* CIF       */
    { 88, 60 },	        /* QQSIF     */
    { 176, 120 },       /* QSIF      */
    { 352, 240 },       /* SIF       */
    // VGA Resolutions
    { 40, 30 },	        /* QQQQVGA   */
    { 80, 60 },	        /* QQQVGA    */
    { 160, 120 },       /* QQVGA     */
    { 320, 240 },       /* QVGA      */
    { 640, 480 },       /* VGA       */
    { 60, 40 },	        /* HQQQVGA   */
    { 120, 80 },	    /* HQQVGA    */
    { 240, 160 },       /* HQVGA     */
    { 480, 320 },       /* HVGA      */
    // FFT Resolutions
    { 64, 32 },	        /* 64x32     */
    { 64, 64 },	        /* 64x64     */
    { 128, 64 },	    /* 128x64    */
    { 128, 128 },       /* 128x128   */
    // Other
    { 128, 160 },	    /* LCD       */
    { 128, 160 },	    /* QQVGA2    */
    { 720, 480 },	    /* WVGA      */
    { 752, 480 },	    /* WVGA2     */
    { 800, 600 },	    /* SVGA      */
    { 1024, 768 },      /* XGA       */
    { 1280, 1024 },     /* SXGA      */
    { 1600, 1200 },     /* UXGA      */
    { 1280, 720 },      /* 720P      */
    { 1920, 1080 },     /* 1080P     */
    { 1280, 960 },      /* 960P      */
    { 2592, 1944 },     /* 5MP       */
};

extern struct camera_ov_t cam_ov2640;
extern struct camera_ops cam_ov2640_ops;

uint8_t camera_read_id(struct camera_ov_t* cam)
{
    uint8_t mid_reg[2] ={ 0 };
    uint8_t pid_reg[2] ={ 0 };

    if ( !cam ) {
        LOG_E("camera obj error.");
        return 1;
    }

    if ( cam->dev_addr == OV2640_ADDRESS ) {
        mid_reg[0] = 0x1C;
        mid_reg[1] = 0x1D;
        pid_reg[0] = 0x0A;
        pid_reg[1] = 0x0B;
    }

    cam->i2c_ops->read_reg(cam->dev_addr, mid_reg[0], &mid_reg[0], 1);
    cam->i2c_ops->read_reg(cam->dev_addr, mid_reg[1], &mid_reg[1], 1);
    cam->mid = (mid_reg[0] << 8) | mid_reg[1];
    if ( cam->mid != OV2640_MID ) {
        LOG_E("read camera [%s] mid error.", cam->name);
        return 1;
    } else
        LOG_I("Camera [%s] mid : 0x%X", cam->name, cam->mid);

    cam->i2c_ops->read_reg(cam->dev_addr, pid_reg[0], &pid_reg[0], 1);
    cam->i2c_ops->read_reg(cam->dev_addr, pid_reg[1], &pid_reg[1], 1);
    cam->pid = (pid_reg[0] << 8) | pid_reg[1];
    if ( cam->pid != OV2640_PID ) {
        LOG_E("read camera [%s] pid error.", cam->name);
        return 1;
    } else
        LOG_I("Camera [%s] pid : 0x%X", cam->name, cam->pid);

    return 0;
}

uint8_t camera_hw_init(struct camera_ov_t* cam)
{
    if ( cam->dev_addr == 0 ) {
        LOG_E("Error camera addr.");
        return 1;
    }

    if ( cam->ops == NULL ) {
        LOG_E("Error camera handler.");
        return 1;
    }

    if ( (cam->mid != 0) || (cam->pid != 0) ) {
        LOG_E("Camera already initialized.");
        return 1;
    }

    /* 初始化I2C总线 */
    if ( !cam->flag && (cam->i2c_ops->init != NULL) ) {
        cam->i2c_ops->init();
        cam->flag = 1;
    } else {
        LOG_E("Camera i2c error or already.");
        return 1;
    }

    return 0;
}

static void camera_xclk_config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct ={ 0 };
    /*Configure GPIO pin : PA8 */
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

static int camera_init(void)
{

    uint8_t ret = 0;

    cam_ov2640.name     = (uint8_t*)"OV2640";
    cam_ov2640.dev_addr = OV2640_ADDRESS;
    cam_ov2640.i2c_ops  = &camera_i2c;
    cam_ov2640.ops      = &cam_ov2640_ops;
    cam_ov2640.mid      = 0;
    cam_ov2640.pid      = 0;
    cam_ov2640.flag     = 0;

    camera_xclk_config();

    RT_DELAY(100);

    ret = camera_hw_init(&cam_ov2640);
    if ( ret ) {
        LOG_E("camera hw init failed");
        ret = 1;
        goto __out;
    }
    ret = camera_read_id(&cam_ov2640);
    if ( ret ) {
        LOG_E("camera read id failed");
        ret = 1;
        goto __out;
    }

    /* 初始化摄像头 */
    if ( cam_ov2640.ops->init != NULL )
        cam_ov2640.ops->init();
    else {
        LOG_E("camera init failed");
        return 1;
    }

__out:
    return ret;
}
#if USER_USE_RTTHREAD == 1
INIT_APP_EXPORT(camera_init);
#endif /* USER_USE_RTTHREAD */

