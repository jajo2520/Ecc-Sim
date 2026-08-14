#include <iostream>
#include "bit_matrix.h"
#include "bit_vector.h"

template <std::size_t r>
class HammingCode
{
public:
    HammingCode();
    static constexpr std::size_t n {(1u << r) - 1}; // 0...001 bitshifted r = 2^r - thank you kevin.
    static constexpr std::size_t k {n-r};

    BitVector<n> encode(const BitVector<k>& message);
    BitVector<r> decode(const BitVector<n>& received);
private:
    BitMatrix<n, k> genMat;
    BitMatrix<r, n> checkMat;
    void buildMatrices();
};

// definitions

template <std::size_t r>
HammingCode<r>::HammingCode()
{
    buildMatrices();
}


