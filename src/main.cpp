#include "bit_vector.h"
#include "bit_matrix.h"
#include "hamming_code.h"
#include <array>
#include <iostream>

int main()
{
    BitMatrix<3, 4> myMat(std::array<BitVector<4>, 3>{
    BitVector<4>(0b0110).reverse(),
    BitVector<4>(0b1100).reverse(),
    BitVector<4>(0b1100).reverse()
});
    BitVector<11> test {0b010'0001'0110}; // 0110 1000 010
    HammingCode<4> myHam;
    //std::cout << myHam.encode(test) << std::endl;
    //std::cout << myHam.decode(BitVector<7>(0b0010111).reverse());
    std::cout << myHam.encode(test) << std::endl;
    std::cout << myHam.decode(myHam.encode(test) + BitVector<15>(0b0100'0000'000'0000));

    return 0;
}
