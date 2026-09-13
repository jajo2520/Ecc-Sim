#pragma once

#include "bit_vector.h"
#include "hamming_code.h"
#include "rng.h"
#include "neural_decoder.h"
#include <array>
#include <sys/syslimits.h>


namespace neural_training 
{
    template <std::size_t dim>
    std::array<double, dim> toDoubleArray(const BitVector<dim>& bitVec)
    {
        std::array<double, dim> outVec {};
        for (std::size_t i {}; i < dim; ++i)
        {
            outVec[i] = bitVec[i];
        }
        return outVec;
    }

    template <std::size_t dim>
    BitVector<dim> toBitVec(const std::array<double, dim>& doubleArray)
    {
        BitVector<dim> outVec (0);
        for (std::size_t i {}; i < dim; ++i)
        {
            outVec.set(i, doubleArray[i] >= 0.5);
        }
    } 

    template <std::size_t r>
    struct TrainingExample
    {
        std::array<double, r> syndrome;
        std::array<double, HammingCode<r>::n> errorPattern;
    };

    template <std::size_t r>
    TrainingExample<r> generateExample(const HammingCode<r>& code, RNG& rng, double trainingP)
    {
        constexpr std::size_t n {HammingCode<r>::n};
        BitVector<n> error {};
        for (std::size_t i {}; i < n; ++i)
        {
            error.set(i, rng.flipBit(trainingP));
        }

        BitVector<r> syndrome = code.getCheckMat() * error;

        return TrainingExample<r> {toDoubleArray(syndrome), toDoubleArray(error)};
    }

    template <std::size_t r>
    void trainNetwork(NeuralDecoder<r>& net, const HammingCode<r>& code, RNG& rng,
                      double trainingP, std::size_t numIterations)
    {
        for (std::size_t i {}; i < numIterations; ++i)
        {
            TrainingExample<r> example = generateExample(code, rng, trainingP);
            net.forward(example.syndrome);
            net.backward(example.errorPattern);
            if (i % 10 == 0)
            {
                std::cout << net.computeLoss(example.errorPattern) << std::endl;
            }
        }
    }
}

