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

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#include "iot_gpio_ex.h"
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_gpio.h"
#include "iot_uart.h"
#include "hi_uart.h"
#include "iot_watchdog.h"
#include "iot_errno.h"
#include "GPS.h"
#include <stdlib.h>
#include "stdarg.h"
#include "math.h"
#include <unistd.h>

#include "iot_gpio_ex.h"
#include "ohos_init.h"
#include "cmsis_os2.h"

#define UART_BUFF_SIZE 1
#define U_SLEEP_TIME   100000

char uartReadBuff[1];
char Serial_RxPacket[100];

uint32_t Serial_RxFlag = 0;

int complete_flag = 0;

void Uart1GpioInit(void)
{
    IoTGpioInit(IOT_IO_NAME_GPIO_0);
    // 设置GPIO0的管脚复用关系为UART1_TX Set the pin reuse relationship of GPIO0 to UART1_ TX
    IoSetFunc(IOT_IO_NAME_GPIO_0, IOT_IO_FUNC_GPIO_0_UART1_TXD);
    IoTGpioInit(IOT_IO_NAME_GPIO_1);
    // 设置GPIO1的管脚复用关系为UART1_RX Set the pin reuse relationship of GPIO1 to UART1_ RX
    IoSetFunc(IOT_IO_NAME_GPIO_1, IOT_IO_FUNC_GPIO_1_UART1_RXD);
}

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
    if (ret != IOT_SUCCESS) {
        printf("Init Uart1 Falied Error No : %d\n", ret);
        return;
    }
}

static void UartTask(void)
{
    uint32_t count = 0;
    int8_t rx_flag = 0;
    uint8_t num = 0;
    static uint8_t uart_done_flag = 0;   

    // 对UART1的一些初始化 Some initialization of UART1
    Uart1GpioInit();
    // 对UART1参数的一些配置 Some configurations of UART1 parameters
    Uart1Config();

    while (1) {
        // 通过UART1 发送数据 Send data through UART1
        // IoTUartWrite(HI_UART_IDX_1, (unsigned char*)data, strlen(data));
        // 通过UART1 接收数据 Receive data through UART1
        Serial_RxFlag = IoTUartRead(HI_UART_IDX_1, uartReadBuff, UART_BUFF_SIZE);
        if (Serial_RxFlag > 0) {
            // 把接收到的数据打印出来 Print the received data
            // printf("Uart Read Data is: [ %d ] %s \r\n", count, uartReadBuff);
            if(*uartReadBuff == '$' && rx_flag == 0){
                printf("start\n");
                num = 0;
                rx_flag = 1;
                }
            if(rx_flag == 1){
                if(*uartReadBuff == '\r'){
                    rx_flag = 2;
                    }
                else{
                    Serial_RxPacket[num] = *uartReadBuff;
                    // printf("Uart Read Data is: [ %d ] %s \r\n", count,  uartReadBuff);
                    num++;
                    // count++;
                     }
                }
            else if(rx_flag == 2 && *uartReadBuff == '\n'){
				rx_flag = 0;
                Serial_RxPacket[num] = '\0';
                printf("done\n");
                uart_done_flag = 1;
            }
            if(uart_done_flag){
                if (strncmp(Serial_RxPacket, "$GPGGA", 6) == 0 || strncmp(Serial_RxPacket, "$BDGGA", 6) == 0 || strncmp(Serial_RxPacket, "$GNGGA", 6) == 0)
                {
                    printf("帧格式GPGGA\n");
                    parse_GPGGA(Serial_RxPacket);
                    uart_done_flag=0;
                    printf("纬度是%f\n",GPSdata.latitude);
					printf("经度是%f\n",GPSdata.longitude);
                }
                else if(strncmp(Serial_RxPacket,"$GPGLL",6) == 0 || strncmp(Serial_RxPacket,"$BDGLL",6) == 0 ||strncmp(Serial_RxPacket,"$GNGLL",6) == 0)
                {
                    printf("帧格式GPGLL\n");
                    parse_GPGLL(Serial_RxPacket);
                    uart_done_flag=0;
                    printf("纬度是%f\n",GPSdata.latitude);
					printf("经度是%f\n",GPSdata.longitude);
                }
		        else if(strncmp(Serial_RxPacket,"$GPRMC",6) == 0 || strncmp(Serial_RxPacket,"$BDRMC",6) == 0 || strncmp(Serial_RxPacket,"$GNRMC",6) == 0)//时间，纬度，半球，精度，半球，速度，航向
			    {
                    printf("帧格式GPRMC\n");
                    parse_GPRMC(Serial_RxPacket);
                    uart_done_flag=0;
                    printf("纬度是%f\n",GPSdata.latitude);
					printf("经度是%f\n",GPSdata.longitude);
		         }
		        else{
                    printf("error\n");
                    uart_done_flag=0;}
            }
            
        }
    }
        
    
}

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
    attr.priority = osPriorityNormal5;

    if (osThreadNew((osThreadFunc_t)UartTask, NULL, &attr) == NULL) {
        printf("[UartTask] Failed to create UartTask!\n");
    }
}

APP_FEATURE_INIT(UartExampleEntry);
