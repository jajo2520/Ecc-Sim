#pragma once

#include "bit_vector.h"
#include "rng.h"
#include "hamming_code.h"
#include "channel.h"
#include "neural_decoder.h"
#include "neural_training.h"
#include "ber_eval.h" 
#include <array>
#include <vector>

// structured nearly identically to ber_eval.h

namespace neural_eval
{
    template <std::size_t r>
    std::size_t runTrial(const HammingCode<r>& code, NeuralDecoder<r>& net, RNG& rng, double p)
    {
        constexpr std::size_t k = HammingCode<r>::k;
        constexpr std::size_t n = HammingCode<r>::n;

        BitVector<k> trial = rng.randomBitVector<k>(0.5);
        auto encoded = code.encode(trial);
        auto corrupted = channel::corrupt(rng, encoded, p);

        BitVector<r> syndrome = code.getCheckMat() * corrupted;
        auto predictedErrorD = net.forward(neural_training::toDoubleArray(syndrome));
        BitVector<n> predictedError = neural_training::toBitVec(predictedErrorD);

        BitVector<n> corrected = corrupted;
        for (std::size_t i {}; i < n; ++i)
            if (predictedError[i])
                corrected.set(i, corrected[i] ^ 1);

        BitVector<k> decoded (0);
        for (std::size_t i {}; i < k; ++i)
            decoded.set(i, corrected[i]);

        std::size_t errorNum {trial.hammingDistance(decoded)};
        return errorNum;
    }

    template <std::size_t r>
    ber_eval::BerResult runPTest(const HammingCode<r>& code, NeuralDecoder<r>& net, RNG& rng, double p)
    {
        std::size_t bitErrors {};
        std::size_t blockErrors {};

        for (std::size_t i {}; i < ber_eval::numTrials; ++i)
        {
            std::size_t trialErrors {runTrial(code, net, rng, p)};
            if (trialErrors != 0)
            {
                bitErrors += trialErrors;
                blockErrors += 1;
            }
        }

        return ber_eval::BerResult {
            p,
            static_cast<double>(bitErrors) / HammingCode<r>::k / ber_eval::numTrials,
            static_cast<double>(blockErrors) / ber_eval::numTrials
        };
    }

    template <std::size_t r>
    std::vector<ber_eval::BerResult> runSweep(const HammingCode<r>& code, NeuralDecoder<r>& net, RNG& rng)
    {
        std::vector<ber_eval::BerResult> results;
        results.reserve(ber_eval::pValues.size());
        for (auto p : ber_eval::pValues)
        {
            results.push_back(runPTest(code, net, rng, p));
        }
        return results;
    }
};

