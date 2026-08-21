#include "test_utils.h"
#include "test_hamming.h"

int main()
{
    testParityIdentity<3>();
    testParityIdentity<4>();
    testParityIdentity<5>();

    testEncodeDecode<3>();
    testEncodeDecode<4>();
    testEncodeDecode<5>();

    testOneBitFlip<3>();
    testOneBitFlip<4>();
    testOneBitFlip<5>();
    return test::testsFailed == 0 ? 0 : 1; // return 0 if all tests passed
}
