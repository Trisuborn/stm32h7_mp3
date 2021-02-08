/************************************************
 * @file camera_i2c.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief package i2c's API for OS or No OS
 * @version 0.1
 * @date 2021-01-16
 *
 * @copyright Copyright (c) 2021
 *
*************************************************/
#include "pro_conf.h"
#include "camera_i2c.h"

#if USER_USE_RTTHREAD == 1
#include <rtthread.h>
#include <rtdevice.h>
#include <rthw.h>
#include "i2c.h"

#define LOG_TAG       "CAM_I2C"
#include <drv_log.h>

#define CAM_USING_I2C   "i2c1"
static struct rt_i2c_bus_device* i2c_dev ={ 0 };

#else
// #define LOG_I     rt_kprintf
// #define LOG_D     rt_kprintf
// #define LOG_E     rt_kprintf

I2C_HandleTypeDef hi2c1;
#endif


/* camera's default function when user unspecified any obj of camera */
static uint8_t camera_i2c_read_reg(uint8_t dev_addr, uint8_t reg_addr, uint8_t* recv_buf, size_t r_size);
static uint8_t camera_i2c_writ_reg(uint8_t dev_addr, uint8_t reg_addr, uint8_t* send_buf, size_t w_size);
static int camera_i2c_init(void);

struct camera_i2c_ops camera_i2c ={
    .init = camera_i2c_init,
};

/************************************************
 * @brief camera i2c read x byte
 *
 * @param dev_addr device address
 * @param reg_addr register of device's address
 * @param recv_buf buffer that want to receive
 * @param r_size size want to read
 * @return uint8_t  result.
 *                  0 : successfully
 *                  1 : err
*************************************************/
static uint8_t camera_i2c_read_reg(uint8_t dev_addr, uint8_t reg_addr, uint8_t* recv_buf, size_t r_size)
{
    /* Send the reg address that want to read to real i2c device specified  */
#if USER_USE_RTTHREAD == 1
    size_t len = 0;
    int ret = 0;
    len = rt_i2c_master_send(i2c_dev, (rt_uint16_t)dev_addr, RT_I2C_WR, &reg_addr, r_size);
    if ( len != r_size ) {
        LOG_E("Camera I2C send register's addr error. (%d)", len);
        ret = -RT_ERROR;
        goto __out;
    }
    len = rt_i2c_master_recv(i2c_dev, (rt_uint16_t)dev_addr, RT_I2C_RD, recv_buf, r_size);
    if ( len != r_size ) {
        LOG_E("Camera I2C receive error.");
        ret = -RT_ERROR;
        goto __out;
    }
    ret = RT_EOK;
#else
    uint8_t ret = 1;
    ret = HAL_I2C_Master_Transmit(&hi2c1, dev_addr, &reg_addr, 1, 0xFFFF);
    if ( ret != HAL_OK ) {
        LOG_E("Camera I2C send register's addr error. (%d)", ret);
        ret = 1;
        goto __out;
    }
    ret = HAL_I2C_Master_Receive(&hi2c1, dev_addr, recv_buf, 1, 0xFFFF);
    if ( ret != HAL_OK ) {
        LOG_E("Camera I2C receive error. (%d)", ret);
        ret = 1;
        goto __out;
    }
#endif

__out:
    return ret;
}

/************************************************
 * @brief camera i2c write x byte
 *
 * @param dev_addr device address
 * @param reg_addr register of device's address
 * @param send_buf buffer that want to write
 * @param r_size size want to write
 * @return uint8_t  result.
 *                  0 : successfully
 *                  1 : err
*************************************************/
static uint8_t camera_i2c_writ_reg(uint8_t dev_addr, uint8_t reg_addr, uint8_t* send_buf, size_t w_size)
{
    uint8_t* buf = NULL;
    buf = malloc(sizeof(uint8_t) * (w_size + 1));
    buf[0] = reg_addr;
    buf[1] = *send_buf;
#if USER_USE_RTTHREAD == 1
    size_t len = 0;
    int ret = 0;
    len = rt_i2c_master_send(i2c_dev, dev_addr, RT_I2C_WR, buf, (w_size + 1));
    if ( len != (w_size + 1) ) {
        LOG_E("Camera I2C send error. (%d)", len);
        ret = -RT_ERROR;
        goto __out;
    }
#else
    uint8_t ret = 1;
    ret = HAL_I2C_Master_Transmit(&hi2c1, dev_addr, buf, w_size + 1);
    if ( ret != HAL_OK ) {
        LOG_E("Camera I2C send error. (%d)", ret);
        ret = 1;
        goto __out;
    }
#endif
__out:
    if ( buf )
        free(buf);
    return ret;
}


#if USER_USE_RTTHREAD == 0
static void MX_I2C1_Init(void);

/* I2C1 init function */
void MX_I2C1_Init(void)
{

    hi2c1.Instance = I2C1;
    hi2c1.Init.Timing = 0x307075B1;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0x94;
    hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if ( HAL_I2C_Init(&hi2c1) != HAL_OK ) {

    }
    /** Configure Analogue filter
    */
    if ( HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK ) {

    }
    /** Configure Digital filter
    */
    if ( HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK ) {

    }

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct ={ 0 };
    if ( i2cHandle->Instance == I2C1 ) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**I2C1 GPIO Configuration
        PB8     ------> I2C1_SCL
        PB9     ------> I2C1_SDA
        */
        GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* I2C1 clock enable */
        __HAL_RCC_I2C1_CLK_ENABLE();
    }
}
#endif


static int camera_i2c_init(void)
{
    int ret = 0;

    camera_i2c.read_reg = camera_i2c_read_reg;
    camera_i2c.writ_reg = camera_i2c_writ_reg;

#if USER_USE_RTTHREAD == 1
    if ( !i2c_dev ) {
        i2c_dev = (struct rt_i2c_bus_device*)rt_device_find(CAM_USING_I2C);
        if ( i2c_dev == RT_NULL ) {
            LOG_E("Camera's I2C get error.");
            ret = -RT_ERROR;
            goto __out;
        }
        LOG_I("Camera I2C get successfully.");
    }
    ret = RT_EOK;
#else
    MX_I2C1_Init();
    ret = 0;
#endif

__out:
    return ret;
}


