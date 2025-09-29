#include "unity.h"
#include "calc.h"

void setUp()
{

}

void tearDown()
{

}

void test_add()
{
    int output = add(10, 20);
    TEST_ASSERT_EQUAL(300, output);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_add); 

    UNITY_END();

    return 0;
}

