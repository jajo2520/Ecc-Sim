#pragma once

#include <random>
#include "bit_vector.h"

class RNG
{
private:
    std::mt19937 mt;

public:
    RNG(unsigned int seed): mt{seed} {};

    int randomInt(int lower, int upper); // inclusive
    float randomFloat(float lower, float upper);
    float randomDouble(double lower, double upper);
    bool flipBit(double p);
    template <std::size_t dim>
    BitVector<dim> randomBitVector(double p);

};

// definitions

int RNG::randomInt(int lower, int upper) // inclusive
{
    std::uniform_int_distribution dist{lower, upper};
    return dist(mt);
}

float RNG::randomFloat(float lower, float upper)
{
    std::uniform_real_distribution dist{lower, upper};
    return dist(mt);
}

float RNG::randomDouble(double lower, double upper)
{
    std::uniform_real_distribution dist{lower, upper};
    return dist(mt);
}
bool RNG::flipBit(double p)
{
    return std::bernoulli_distribution(p)(mt);
}

template <std::size_t dim>
BitVector<dim> RNG::randomBitVector(double p)
{
    BitVector<dim> result {0};
    for (std::size_t i {}; i < dim; ++i)
    {
        result.set(i,flipBit(p));
    }
    return result;
}

