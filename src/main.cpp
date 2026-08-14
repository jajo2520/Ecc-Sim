#include "bit_vector.h"
#include "bit_matrix.h"
#include <array>
#include <iostream>

int main()
{
    std::cout << BitVector<5>(0b10110) << std::endl;
    std::cout << BitMatrix<3, 4>(std::array<BitVector<4>, 3>{
    BitVector<4>(0b1011),
    BitVector<4>(0b0110),
    BitVector<4>(0b1101)
});
    return 0;
}
