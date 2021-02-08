/************************************************
 * @file main.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 0.1
 * @date 2021-01-16
 *
 * @copyright Copyright (c) 2021
 *
*************************************************/
#include "main.h"
#include "sys_conf.h"
#include "pro_conf.h"


#if USER_USE_RTTHREAD == (1u)
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_gpio.h"

#define LED0_PIN    GET_PIN(I, 8)
#define LED1_PIN    GET_PIN(E, 3)
#define LCD_BLK     GET_PIN(D, 4)
#endif

int main()
{

#if USER_USE_RTTHREAD == (1u)
    uint32_t ts, te, t;
    uint32_t i = 0;

    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);

    while ( 1 ) {
        rt_thread_mdelay(500);
        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED0_PIN, PIN_LOW);
    }

#else
    HAL_Init();
    SystemClock_Config();

    while ( 1 ) {

    }

#endif

}

// #define LCD_WIDTH           320u
// #define LCD_HEIGHT          480u
// #define LCD_BITS_PER_PIXEL  24u
// #define LCD_BYTES_PER_PIXEL  (LCD_BITS_PER_PIXEL / 8)
// #define LCD_BUF_SIZE        (460800) // (LCD_WIDTH * LCD_HEIGHT * LCD_BYTES_PER_PIXEL)
// extern struct rt_memheap system_heap0;
// static void sdram(void)
// {
//     uint32_t size = LCD_BUF_SIZE;
//     rt_kprintf("allocate : %d b.\n", size);
//     uint8_t *buf = rt_memheap_alloc(&system_heap0, size);
// }
// MSH_CMD_EXPORT(sdram, sdram);
// INIT_APP_EXPORT(sdram);
