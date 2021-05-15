#include "tst_basex.h"
#include "tst_constant.h"
#include "tst_equality.h"
#include "tst_sum.h"

#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
