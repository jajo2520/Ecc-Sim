#pragma once

#include "rng.h"
#include "bit_vector.h"

namespace channel 
{
    template <std::size_t n>
    BitVector<n> corrupt (RNG& rng, const BitVector<n>& codeword, double p)
    {
        BitVector<n> output {0};
        for (std::size_t i {}; i < n; ++i)
        {
            output.set(i, codeword[i] ^ rng.flipBit(p));
        }
        return output;
    }
}

