#include <complex>
#include <iostream>
#include <array>
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
    BitVector<k> decode(const BitVector<n>& message);
private:
    BitMatrix<r, n> checkMat;
    BitMatrix<n, k> genMat;
    std::array<std::size_t, n> bitMap {};
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
        if ((bits & (bits-1)) != 0) // not a multiple of 2
        {
            checkMat.setCol(i, BitVector<r>(bits));
            bitMap[bits-1] = i;
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
            bitMap[bits-1] = j;
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

template <std::size_t r>
BitVector<HammingCode<r>::n> HammingCode<r>::encode(const BitVector<k>& message)
{
    return genMat*message;
}

template <std::size_t r>
BitVector<HammingCode<r>::k> HammingCode<r>::decode(const BitVector<HammingCode<r>::n>& received)
{
    BitVector<r> syndrome = checkMat * received;
    std::size_t errorNum {};

    for (std::size_t i {}; i < r; ++i)
        errorNum += (std::size_t{1} << i) * syndrome[i];

    BitVector<HammingCode<r>::k> correctMessage {0};
    for (std::size_t i {}; i < k; ++i)
        correctMessage.set(i, received[i]);

    if (errorNum != 0)                          
    {
        std::size_t errorPos {bitMap[errorNum]-1};
        if (errorPos < k)
            correctMessage.set(errorPos, received[errorPos] ^ 1);
    }
    return correctMessage;
}

