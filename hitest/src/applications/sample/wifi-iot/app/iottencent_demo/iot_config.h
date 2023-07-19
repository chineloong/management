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

#ifndef IOT_CONFIG_H
#define IOT_CONFIG_H

// <CONFIG THE LOG
/* if you need the iot log for the development , please enable it, else please comment it */
#define CONFIG_LINKLOG_ENABLE   1

// < CONFIG THE WIFI
/* Please modify the ssid and pwd for the own */
#define CONFIG_AP_SSID  "Chineloong" // WIFI SSID
#define CONFIG_AP_PWD   "12345678" // WIFI PWD
/* Tencent iot Cloud user ID , password */
#define CONFIG_USER_ID    "CTBB6V388EHi3861_mqtt;12010126;c97a3;1697472000"
#define CONFIG_USER_PWD   "142038c9936595dd0127d26b1feeb6928d10539e195fe3eb1c6fccde535fa638;hmacsha256"
#define CN_CLIENTID     "CTBB6V388EHi3861_mqtt" // Tencent cloud ClientID format: Product ID + device name
#endif