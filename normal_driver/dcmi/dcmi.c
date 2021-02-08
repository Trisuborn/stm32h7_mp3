/************************************************
 * @file dcmi_noos.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 0.1
 * @date 2021-01-15
 *
 * @copyright Copyright (c) 2021
 *
*************************************************/
#include "pro_conf.h"
#include "dcmi.h"
#include "screen.h"

#if USER_USE_RTTHREAD == 1
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_gpio.h"

#define LOG_TAG       "DCMI"
#include <drv_log.h>

#else
// #define LOG_I     rt_kprintf
// #define LOG_D     rt_kprintf
// #define LOG_E     rt_kprintf
#endif

DCMI_HandleTypeDef hdcmi;
DMA_HandleTypeDef hdma_dcmi;


/* DCMI init function */
void MX_DCMI_Init(void)
{

    hdcmi.Instance = DCMI;
    hdcmi.Init.SynchroMode      = DCMI_SYNCHRO_HARDWARE;
    hdcmi.Init.PCKPolarity      = DCMI_PCKPOLARITY_RISING;
    hdcmi.Init.VSPolarity       = DCMI_VSPOLARITY_LOW;
    hdcmi.Init.HSPolarity       = DCMI_HSPOLARITY_LOW;
    hdcmi.Init.CaptureRate      = DCMI_CR_ALL_FRAME;
    hdcmi.Init.ExtendedDataMode = DCMI_EXTEND_DATA_8B;
    hdcmi.Init.JPEGMode         = DCMI_JPEG_DISABLE;
    hdcmi.Init.ByteSelectMode   = DCMI_BSM_ALL;
    hdcmi.Init.ByteSelectStart  = DCMI_OEBS_ODD;
    hdcmi.Init.LineSelectMode   = DCMI_LSM_ALL;
    hdcmi.Init.LineSelectStart  = DCMI_OELS_ODD;
    if ( HAL_DCMI_Init(&hdcmi) != HAL_OK ) {
        Error_Handler();
    }
}
INIT_BOARD_EXPORT(MX_DCMI_Init);

void HAL_DCMI_MspInit(DCMI_HandleTypeDef* dcmiHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct ={ 0 };
    if ( dcmiHandle->Instance == DCMI ) {
      /* DCMI clock enable */
        __HAL_RCC_DCMI_CLK_ENABLE();
        __HAL_RCC_DMA1_CLK_ENABLE();

        __HAL_RCC_GPIOE_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**DCMI GPIO Configuration
        PE4     ------> DCMI_D4
        PE5     ------> DCMI_D6
        PE6     ------> DCMI_D7
        PA4     ------> DCMI_HSYNC
        PA6     ------> DCMI_PIXCLK
        PC6     ------> DCMI_D0
        PC7     ------> DCMI_D1
        PD3     ------> DCMI_D5
        PB7     ------> DCMI_VSYNC
        PE0     ------> DCMI_D2
        PE1     ------> DCMI_D3
        */
        GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_0
            | GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_6;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* DCMI DMA Init */
        /* DCMI Init */
        hdma_dcmi.Instance = DMA1_Stream0;
        hdma_dcmi.Init.Request = DMA_REQUEST_DCMI;
        hdma_dcmi.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_dcmi.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_dcmi.Init.MemInc = DMA_MINC_ENABLE;
        hdma_dcmi.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
        hdma_dcmi.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
        hdma_dcmi.Init.Mode = DMA_CIRCULAR;
        hdma_dcmi.Init.Priority = DMA_PRIORITY_LOW;
        hdma_dcmi.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if ( HAL_DMA_Init(&hdma_dcmi) != HAL_OK ) {
            Error_Handler();
        }

        __HAL_LINKDMA(dcmiHandle, DMA_Handle, hdma_dcmi);

        /* DCMI interrupt Init */
        HAL_NVIC_SetPriority(DCMI_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(DCMI_IRQn);

        /* DMA1_Stream0_IRQn interrupt configuration */
        HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);

    }
}


static uint16_t *buf;
static uint8_t cnt = 0;
static uint8_t fflag = 0;

void HAL_DCMI_LineEventCallback(DCMI_HandleTypeDef* hdcmi)
{
    static uint32_t i = 0;
    rt_kprintf("%s %d success \n", __FUNCTION__, i++);
}

void HAL_DCMI_VsyncEventCallback(DCMI_HandleTypeDef* hdcmi)
{
    static uint32_t i = 0;
    rt_kprintf("%s %d success \n", __FUNCTION__, i++);
}

void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef* hdcmi)
{
    static uint32_t i = 0;
    rt_kprintf("%s %d success \n", __FUNCTION__, i++);
    fflag = 1;
}

void DMA1_Stream0_IRQHandler(void)
{
    OS_INT_ENTER();
    HAL_DMA_IRQHandler(&hdma_dcmi);
    OS_INT_EXIT();
}

void DCMI_IRQHandler(void)
{
    OS_INT_ENTER();
    HAL_DCMI_IRQHandler(&hdcmi);
    OS_INT_EXIT();
}

#include "board.h"
static void my_dcmi_test(void)
{
    buf = rt_malloc((200 * 200 * 2));
    rt_memset(buf, 0xFF, (200 * 200 * 2));
    HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_SNAPSHOT, (uint32_t)&buf, (160 * 120 * 2));
    // while ( 1 ) {
    //     if ( fflag ) {
    rt_thread_mdelay(500);
             scr_flush_with(0, 0, 160, 80, buf);
    //         fflag = 0;
    //     }
    // }
    //while (fflag == 0);

    // rt_thread_mdelay(100);
}
MSH_CMD_EXPORT(my_dcmi_test, my_dcmi_test);

