#include "bit_vector.h"
#include "bit_matrix.h"
#include "hamming_code.h"
#include <array>
#include <iostream>

int main()
{
    BitMatrix<3, 4> myMat(std::array<BitVector<4>, 3>{
    BitVector<4>(0b0000),
    BitVector<4>(0b0110),
    BitVector<4>(0b0101)
});
    std::cout << HammingCode<4>().getCheckMat() << std::endl;
    std::cout << HammingCode<4>().getGenMat() << std::endl;
    
    return 0;
}
