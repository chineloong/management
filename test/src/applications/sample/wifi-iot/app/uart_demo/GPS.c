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

GPS_Data GPSdata;
// char Serial_RxPacket1[100] = "$GPGGA,123519,4807.038,N,11131.000,E,1,08,0.9,545.4,M,46.9,M,,*47";
// char Serial_RxPacket[] = "$GPRMC,225446,A,4916.45,N,12311.12,W,000.5,054.7,191194,020.3,E*68";
// char Serial_RxPacket1[] = "$GPGLL,4916.45,N,12311.12,W,225444,A*31";
// GPS����֡����

extern char Serial_RxPacket[100];

GPS_Data parse_GPGGA(char *gps_string) {
	char *token = strtok(gps_string, ",");//分解字符串为一组字符串
	int field_num = 0;
	
	//经纬度计算用
	float coordinate = 0;
	int degree = 0;
	float minute = 0;

	uint8_t handleflag;
	handleflag = 0;

	while (token != NULL) {
		if(field_num == 1) {
			handleflag = 1;
			strncpy(GPSdata.time, token, 10);//把 src 所指向的字符串复制到 dest，最多复制 n 个字符
		}
		if(field_num == 2){						//纬度转换
			coordinate = atof(token);
			degree = coordinate / 100;	// 取整数部分作为度数
			float minute = coordinate - degree * 100;	// 取小数部分作为分钟数
			GPSdata.latitude = degree + minute / 60;	// 将分钟数转换为度数
			printf("纬度转换成功\n");
		}
		if(field_num == 3){
				if (strcmp(token, "S") == 0) {			//南北纬设定，南纬为-
					GPSdata.latitude *= -1;
				}
		}
		if(field_num == 4){

					float coordinate = atof(token);
					int degree = coordinate / 100;	// 取整数部分作为度数
					float minute = coordinate - degree * 100;	// 取小数部分作为分钟数
					GPSdata.longitude = degree + minute / 60;	// 将分钟数转换为度数
					printf("经度转换成功\n");
		}
		if(field_num == 5){
				if (strcmp(token, "W") == 0) {
					GPSdata.longitude *= -1;
				}
		}
		if(field_num == 6){
			GPSdata.fix_quality = atoi(token);
		}
		if(field_num == 7){
				GPSdata.num_satellites = atoi(token);
		}
		if(field_num == 8){
				GPSdata.hdop = atof(token);
		}
		if(field_num == 9){
				GPSdata.altitude = atof(token);
		}
		token = strtok(NULL, ",");
		field_num++;
		if(field_num >= 10) break;
	}
	printf("数据处理完成\n");
	return GPSdata;
}

GPS_Data parse_GPRMC(char *gps_string) {
	char *token = strtok(gps_string, ",");//分解字符串为一组字符串
	int field_num = 0;
	while (token != NULL) {
		if(field_num == 1) {
			strncpy(GPSdata.time, token, 10);//把 src 所指向的字符串复制到 dest，最多复制 n 个字符
		}
		if(field_num == 3) {							//纬度转换
			float coordinate = atof(token);
			int degree = coordinate / 100;	// 取整数部分作为度数
			float minute = coordinate - degree * 100;	// 取小数部分作为分钟数
			GPSdata.latitude = degree + minute / 60;	// 将分钟数转换为度数
			printf("纬度转换成功\n");
		}
		if(field_num == 4) {
			if (strcmp(token, "S") == 0) {			//南北纬设定，南纬为-
				GPSdata.latitude *= -1;
			}
		}
		if(field_num == 5) {
			float coordinate = atof(token);
			int degree = coordinate / 100;	// 取整数部分作为度数
			float minute = coordinate - degree * 100;	// 取小数部分作为分钟数
			GPSdata.longitude = degree + minute / 60;	// 将分钟数转换为度数
			printf("经度转换成功\n");
		}
		if(field_num == 6) {
			if (strcmp(token, "W") == 0) {
				GPSdata.longitude *= -1;
			}
		}
		// if(field_num == 7) {
		// 	GPSdata.speed = atoi(token);
		// }
		// if(field_num == 8) {
		// 	//GPSdata.num_satellites = atoi(token);
		// 	}
		// if(field_num == 9) {
		// 	//GPSdata.hdop = atof(token);
		// 	}
		// if(field_num == 10) {
		// 	//GPSdata.altitude = atof(token);
		// }
		token = strtok(NULL, ",");
		field_num++;
		if(field_num >= 10) break;
	}
	printf("数据处理完成\n");
	return GPSdata;
}

GPS_Data parse_GPGLL(char *gps_string){
	char *token = strtok(gps_string, ",");//分解字符串为一组字符串
	int field_num = 0;

	//经纬度计算用
	float coordinate = 0;
	int degree = 0;
	float minute = 0;

	uint8_t handleflag;
	handleflag = 0;

	while (token != NULL) {
		if(field_num == 1){
			coordinate = atof(token);
			degree = coordinate / 100;	// 取整数部分作为度数
			minute = coordinate - degree * 100;	// 取小数部分作为分钟数
			GPSdata.latitude = degree + minute / 60;	// 将分钟数转换为度数
			printf("纬度转换成功\n");
		}
		else if(field_num == 2){
			if (strcmp(token, "S") == 0) {			//南北纬设定，南纬为-
				GPSdata.latitude *= -1;
			}
		}
		else if(field_num == 3){
			coordinate = atof(token);
			degree = coordinate / 100;	// 取整数部分作为度数
			minute = coordinate - degree * 100;	// 取小数部分作为分钟数
			GPSdata.longitude = degree + minute / 60;	// 将分钟数转换为度数
			printf("经度转换成功\n");
		}
		else if(field_num == 4){
			if (strcmp(token, "W") == 0) {
				GPSdata.longitude *= -1;
			}
		}
		token = strtok(NULL, ",");
		field_num++;
		if(field_num >= 10) break;
	}
	printf("数据处理完成\n");
	return GPSdata;
}