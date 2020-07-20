#include <iostream>
#include "gtest/gtest.h"

extern "C" {
#include "../libs/log.h"
#include "../libs/utils.h"
#include "../libs/kustd.h"
#include "stub.h"
}


TEST(test1, test1_1) {
    ASSERT_EQ(1, 1);
}



