/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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

#include <gtest/gtest.h>

#include "hks_exist_test.h"

#include "hks_api.h"
#include "hks_param.h"
#include "hks_test_api_performance.h"
#include "hks_test_common.h"
#include "hks_test_log.h"
#include "hks_type.h"

using namespace testing::ext;
namespace {
#ifndef _CUT_AUTHENTICATE_
class HksExistTest : public testing::Test {
public:
    static void SetUpTestCase(void);

    static void TearDownTestCase(void);

    void SetUp();

    void TearDown();
};

void HksExistTest::SetUpTestCase(void)
{
}

void HksExistTest::TearDownTestCase(void)
{
}

void HksExistTest::SetUp()
{
    EXPECT_EQ(HksInitialize(), 0);
}

void HksExistTest::TearDown()
{
}

const struct HksTestKeyExistParams g_testKeyExistParams[] = {
    /* normal case */
    { 0, HKS_SUCCESS, true, { true, DEFAULT_KEY_ALIAS_SIZE, true, DEFAULT_KEY_ALIAS_SIZE } },
};

/**
 * @tc.name: HksExistTest.HksExistTest001
 * @tc.desc: The static function will return true;
 * @tc.type: FUNC
 */
HWTEST_F(HksExistTest, HksExistTest001, TestSize.Level1)
{
    int32_t ret;
    struct HksBlob *keyAlias = NULL;
    if (g_testKeyExistParams[0].isGenKey) {
        HKS_TEST_ASSERT(TestGenDefaultKeyAndGetAlias(&keyAlias) == 0);
        ret = HksKeyExistRun(keyAlias, 1);
        HKS_TEST_ASSERT(ret == g_testKeyExistParams[0].expectResult);
        HKS_TEST_ASSERT(HksDeleteKey(keyAlias, NULL) == HKS_SUCCESS);
    } else {
        ret = TestConstuctBlob(&keyAlias,
                               g_testKeyExistParams[0].keyAliasParams.blobExist,
                               g_testKeyExistParams[0].keyAliasParams.blobSize,
                               g_testKeyExistParams[0].keyAliasParams.blobDataExist,
                               g_testKeyExistParams[0].keyAliasParams.blobDataSize);
        HKS_TEST_ASSERT(ret == 0);
        ret = HksKeyExistRun(keyAlias, 1);
        if (ret != g_testKeyExistParams[0].expectResult) {
            HKS_TEST_LOG_I("HksKeyExistRun 2 failed, ret[%u] = %d", g_testKeyExistParams[0].testId, ret);
        }
        HKS_TEST_ASSERT(ret == g_testKeyExistParams[0].expectResult);
    }
    TestFreeBlob(&keyAlias);
    ASSERT_TRUE(ret == 0);
}
#endif /* _CUT_AUTHENTICATE_ */
}