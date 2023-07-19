/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
//#include "stm32f10x.h"
#include "string.h"
//#include "Serial.h"
#include <stdint.h>
#include "GPS.h"
#include <stdlib.h>
#include <stdio.h>
#include "stdarg.h"
#include "math.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "iot_gpio_ex.h"
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_watchdog.h"

GPS_Data data;
//  char Serial_RxPacket[] = "$GPGGA,123519,4807.038,N,11131.000,E,1,08,0.9,545.4,M,46.9,M,,*47";
// char Serial_RxPacket[] = "$GPRMC,225446,A,4916.45,N,12311.12,W,000.5,054.7,191194,020.3,E*68";
// char Serial_RxPacket1[] = "$GPGLL,4916.45,N,12311.12,W,225444,A*31";
// GPS����֡����

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "iot_gpio_ex.h"
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_gpio.h"
#include "iot_uart.h"
#include "hi_uart.h"
#include "iot_watchdog.h"
#include "iot_errno.h"
#include "GPS.h"

#define UART_BUFF_SIZE 100
#define U_SLEEP_TIME   100000

//定义接收信息字符串（首地址）
 unsigned char uartReadBuff[UART_BUFF_SIZE] = {0};
char Serial_RxPacket[100];

uint32_t Serial_RxFlag = 0;

/*
biref:设置GPIO0.GPIO1口分别为tx，rx引脚
 */
void Uart1GpioInit(void)
{
    IoTGpioInit(IOT_IO_NAME_GPIO_0);
    // 设置GPIO0的管脚复用关系为UART1_TX Set the pin reuse relationship of GPIO0 to UART1_ TX
    IoSetFunc(IOT_IO_NAME_GPIO_0, IOT_IO_FUNC_GPIO_0_UART1_TXD);
    IoTGpioInit(IOT_IO_NAME_GPIO_1);
    // 设置GPIO1的管脚复用关系为UART1_RX Set the pin reuse relationship of GPIO1 to UART1_ RX
    IoSetFunc(IOT_IO_NAME_GPIO_1, IOT_IO_FUNC_GPIO_1_UART1_RXD);
}

/*
biref:对uart1初始化，配置波特率，停止位等
*/
void Uart1Config(void)
{
    uint32_t ret;
    /* 初始化UART配置，波特率 9600，数据bit为8,停止位1，奇偶校验为NONE */
    /* Initialize UART configuration, baud rate is 9600, data bit is 8, stop bit is 1, parity is NONE */
    IotUartAttribute uart_attr = {
        .baudRate = 9600,
        .dataBits = 8,
        .stopBits = 1,
        .parity = 0,
    };
    ret = IoTUartInit(HI_UART_IDX_1, &uart_attr);
    //失败配置
    if (ret != IOT_SUCCESS) {
        printf("Init Uart1 Falied Error No : %d\n", ret);
        return;
    }
}

/*
biref:uart1接收任务
*/
static void UartTask(void)
{
    uint32_t count = 0;
    uint32_t in = 0;
    static int8_t rx_flag = 0;
    uint8_t num = 0;
    static uint8_t uart_done_flag=0;
    // 对UART1的一些初始化 Some initialization of UART1
    Uart1GpioInit();
    // 对UART1参数的一些配置 Some configurations of UART1 parameters
    Uart1Config();

    while (1) {
        // 通过UART1 接收数据 Receive data through UART1
        printf("2");
        Serial_RxFlag = IoTUartRead(HI_UART_IDX_1,in, UART_BUFF_SIZE);
        if (Serial_RxFlag != -1) {
            // 把接收到的数据打印出来 Print the received data
            // printf("Uart Read Data is: [ %d ] %s \r\n", count, uartReadBuff);
            printf("2");
            // if(in == "$" && rx_flag == 0){
            //     num = 0;
            //     rx_flag = 1;
            // }
            // if(rx_flag == 1){
            //     if(in == "/r"){
            //         rx_flag = 2;
            //     }
            //     else {
            //         Serial_RxPacket[num] = in;
            //         num++;
            //          }
            // }
            // if(rx_flag == 2 && in == '\n'){
			// 	rx_flag = 0;
            //     Serial_RxPacket[num] = '\0';
            //     uart_done_flag=1;
            // }
            // if(uart_done_flag){
            //     if (strncmp(Serial_RxPacket, "$GPGGA", 6) == 0)
            //     {
            //         printf("帧格式GPGGA\n");
            //         parse_GPGGA(Serial_RxPacket);
            //     }
            //     else if(strncmp(Serial_RxPacket,"$GPGLL",6) == 0)
            //     {
            //         printf("帧格式GPGLL\n");
            //         parse_GPGLL(Serial_RxPacket);
            //     }
		    //     else if(strncmp(Serial_RxPacket,"$GPRMC",6) == 0)//时间，纬度，半球，精度，半球，速度，航向
			//     {
            //         printf("帧格式GPRMC\n");
            //         parse_GPRMC(Serial_RxPacket);
		    //      }
		    //     else printf("error\n");
            // }
            // uart_done_flag=0;

        }
    }
}


/*
biref:线程配置
*/
void UartExampleEntry(void)
{
    osThreadAttr_t attr;
    IoTWatchDogDisable();

    attr.name = "UartTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 5 * 1024; // 任务栈大小*1024 stack size 5*1024
    attr.priority = osPriorityNormal;

    if (osThreadNew((osThreadFunc_t)UartTask, NULL, &attr) == NULL) {
        printf("[UartTask] Failed to create UartTask!\n");
    }
}

APP_FEATURE_INIT(UartExampleEntry);
