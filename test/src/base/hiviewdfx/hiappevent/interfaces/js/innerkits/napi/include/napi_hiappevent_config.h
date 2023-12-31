/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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
#ifndef NATIVE_HIAPPEVENT_CONFIG_H
#define NATIVE_HIAPPEVENT_CONFIG_H

#include "napi/native_api.h"
#include "napi/native_node_api.h"

namespace OHOS {
namespace HiviewDFX {
bool ConfigureFromNapi(const napi_env env, const napi_value configObj);
void SetStorageDirFromNapi(const napi_env env, const napi_callback_info info);
} // HiviewDFX
} // OHOS
#endif // NATIVE_HIAPPEVENT_CONFIG_H