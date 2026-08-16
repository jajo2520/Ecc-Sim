#include <complex>
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

    const BitMatrix<r, n>& getCheckMat() const;
    const BitMatrix<n, k>& getGenMat() const;
    BitVector<n> encode(const BitVector<k>& message);
    BitVector<r> decode(const BitVector<n>& received);
private:
    BitMatrix<r, n> checkMat;
    BitMatrix<n, k> genMat;
    void buildMatrices();
};

// definitions

template <std::size_t r>
HammingCode<r>::HammingCode()
{
    buildMatrices();
}


template <std::size_t r>
const BitMatrix<r, HammingCode<r>::n>& HammingCode<r>::getCheckMat() const
{
    return checkMat;
}

template <std::size_t r>
const BitMatrix<HammingCode<r>::n, HammingCode<r>::k>& HammingCode<r>::getGenMat() const
{
    return genMat;
}

template <std::size_t r>
void HammingCode<r>::buildMatrices()
{
    // generating checkMat
    std::size_t i {0};
    std::size_t bits {1};
    while (i<k) 
    {
        if ((bits & (bits-1)) != 0)
        {
            checkMat.setCol(i, BitVector<r>(bits));
            i++;
        }
        bits++;
    }
    std::size_t j {k};
    bits = 1;
    while (j<n) 
    {
        if ((bits & (bits-1)) == 0)
        {
            checkMat.setCol(j, BitVector<r>(bits));
            j++;
        }
        bits++;
    }   
    
    // generating genMat 
    for (i = 0; i < k; ++i)
        genMat.set(i,i,1); 

    for (i = k; i < n; ++i)
    {
        for (j = 0; j < k; ++j)
            genMat.set(i,j,checkMat.get(i-k,j));
    }
}
