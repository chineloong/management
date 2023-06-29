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
extern uint32_t Serial_RxFlag;

extern char Serial_RxPacket[100];

GPS_Data parse_GPGGA(char *gps_string) {
	char *token = strtok(gps_string, ",");//分解字符串为一组字符串
	int field_num = 0;
	while (token != NULL) {
		switch (field_num) {
			case 1:
				strncpy(data.time, token, 10);//把 src 所指向的字符串复制到 dest，最多复制 n 个字符
				break;
			case 2:								//纬度转换
				if (1) {
					float coordinate = atof(token);
					int degree = coordinate / 100;	// 取整数部分作为度数
					float minute = coordinate - degree * 100;	// 取小数部分作为分钟数
					data.latitude = degree + minute / 60;	// 将分钟数转换为度数
					printf("纬度转换成功\n");
				}
				break;
			case 3:
				if (strcmp(token, "S") == 0) {			//南北纬设定，南纬为-
					data.latitude *= -1;
				}
				break;
			case 4:
				if (1) {
					float coordinate = atof(token);
					int degree = coordinate / 100;	// 取整数部分作为度数
					float minute = coordinate - degree * 100;	// 取小数部分作为分钟数
					data.longitude = degree + minute / 60;	// 将分钟数转换为度数
					printf("经度转换成功\n");
				}
				break;
			case 5:
				if (strcmp(token, "W") == 0) {
					data.longitude *= -1;
				}
				break;
			case 6:
				data.fix_quality = atoi(token);
				break;
			case 7:
				data.num_satellites = atoi(token);
				break;
			case 8:
				data.hdop = atof(token);
				break;
			case 9:
				data.altitude = atof(token);
				break;
			default:
				break;
		}
		token = strtok(NULL, ",");
		field_num++;
		if(field_num >= 10) break;
	}
	printf("数据处理完成\n");
	return data;
}

GPS_Data parse_GPRMC(char *gps_string) {
	char *token = strtok(gps_string, ",");//分解字符串为一组字符串
	int field_num = 0;
	while (token != NULL) {
		switch (field_num) {
			case 1:
				strncpy(data.time, token, 10);//把 src 所指向的字符串复制到 dest，最多复制 n 个字符
				break;
			case 3:								//纬度转换
				if (1) {
					float coordinate = atof(token);
					int degree = coordinate / 100;	// 取整数部分作为度数
					float minute = coordinate - degree * 100;	// 取小数部分作为分钟数
					data.latitude = degree + minute / 60;	// 将分钟数转换为度数
					printf("纬度转换成功\n");
				}
				break;
			case 4:
				if (strcmp(token, "S") == 0) {			//南北纬设定，南纬为-
					data.latitude *= -1;
				}
				break;
			case 5:
				if (1) {
					float coordinate = atof(token);
					int degree = coordinate / 100;	// 取整数部分作为度数
					float minute = coordinate - degree * 100;	// 取小数部分作为分钟数
					data.longitude = degree + minute / 60;	// 将分钟数转换为度数
					printf("经度转换成功\n");
				}
				break;
			case 6:
				if (strcmp(token, "W") == 0) {
					data.longitude *= -1;
				}
				break;
			case 7:
				data.speed = atoi(token);
				break;
			case 8:
				//data.num_satellites = atoi(token);
				break;
			case 9:
				//data.hdop = atof(token);
				break;
			case 10:
				//data.altitude = atof(token);
				break;
			default:
				break;
		}
		token = strtok(NULL, ",");
		field_num++;
		if(field_num >= 10) break;
	}
	printf("数据处理完成\n");
	return data;
}

GPS_Data parse_GPGLL(char *gps_string) {
	char *token = strtok(gps_string, ",");//分解字符串为一组字符串
	int field_num = 0;
	while (token != NULL) {
		switch (field_num) { 
			case 1:								//纬度转换
				if (1) {
					float coordinate = atof(token);
					int degree = coordinate / 100;	// 取整数部分作为度数
					float minute = coordinate - degree * 100;	// 取小数部分作为分钟数
					data.latitude = degree + minute / 60;	// 将分钟数转换为度数
					printf("纬度转换成功\n");
				}
				break;
			case 2:
				if (strcmp(token, "S") == 0) {			//南北纬设定，南纬为-
					data.latitude *= -1;
				}
				break;
			case 3:
				if (1) {
					float coordinate = atof(token);
					int degree = coordinate / 100;	// 取整数部分作为度数
					float minute = coordinate - degree * 100;	// 取小数部分作为分钟数
					data.longitude = degree + minute / 60;	// 将分钟数转换为度数
					printf("经度转换成功\n");
				}
				break;
			case 4:
				if (strcmp(token, "W") == 0) {
					data.longitude *= -1;
				}
				break;
		}
		token = strtok(NULL, ",");
		field_num++;
		if(field_num >= 10) break;
	}
	printf("数据处理完成\n");
	return data;
}

void GPS_Compare(void)
{
	while(1){
	// printf("%s",Serial_RxFlag);
	if (Serial_RxFlag != -1)
	// if (1)
	{
		// printf("%s/n",Serial_RxPacket);
		if (strncmp(Serial_RxPacket, "$GPGGA", 6) == 0)
		{
			printf("帧格式GPGGA\n");
			parse_GPGGA(Serial_RxPacket);
		}
		else if(strncmp(Serial_RxPacket,"$GPGLL",6) == 0)
			{
				printf("帧格式GPGLL\n");
				parse_GPGLL(Serial_RxPacket);
			}
		else if(strncmp(Serial_RxPacket,"$GPRMC",6) == 0)//时间，纬度，半球，精度，半球，速度，航向
			{
				printf("帧格式GPRMC\n");
				parse_GPRMC(Serial_RxPacket);
		    }
		else printf("error\n");
	}
	}
}

/*
biref:线程配置
*/
void GPSTask(void)
{
    osThreadAttr_t attr;
    IoTWatchDogDisable();
	attr.name = "GPS";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 7 * 1024; // 任务栈大小*1024 stack size 5*1024
    attr.priority = osPriorityNormal;

    if (osThreadNew((osThreadFunc_t)GPS_Compare, NULL, &attr) == NULL) {
        printf("[UartTask] Failed to create UartTask!\n");
    }
}

APP_FEATURE_INIT(GPSTask);



// int main(){
// 	GPS_Compare();
// 	printf("纬度%f\n",data.latitude);
// 	printf("经度%f\n",data.longitude);
// 	printf("done");
// }