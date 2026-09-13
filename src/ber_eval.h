#pragma once

#include "bit_vector.h"
#include "rng.h"
#include "hamming_code.h"
#include "channel.h"

#include <array>
#include <vector>


namespace ber_eval 
{

    inline constexpr std::size_t numTrials {10000}; 
    inline constexpr std::array<double, 10> pValues {0.01, 0.025, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.4, 0.5};

    struct BerResult
    {
        double p;
        double bitErrorRate;
        double blockErrorRate;
    };

    template <std::size_t r>
    std::size_t runTrial (const HammingCode<r>& code, RNG& rng, double p) // passing Hcode in to avoid non-trivial rebuilding
                                                                          // each matrix every time.
    {
        BitVector<HammingCode<r>::k> trial = rng.randomBitVector<HammingCode<r>::k>(0.5);
        auto encoded = code.encode(trial);
        auto corrupted = channel::corrupt(rng, encoded, p);
        auto decoded = code.decode(corrupted);
        
        std::size_t errorNum {trial.hammingDistance(decoded)}; // XOR then count # of 1s
        return errorNum;
    }

    template <std::size_t r>
    BerResult runPTest (const HammingCode<r>& code, RNG& rng, double p)
    {
        std::size_t bitErrors {};
        std::size_t blockErrors {};

        for (std::size_t i {}; i < numTrials; ++i)
        {
            std::size_t trialErrors {runTrial(code, rng, p)};
            if (trialErrors != 0)
            {
                bitErrors += trialErrors;
                blockErrors += 1;
            }
        }

        return BerResult {p, static_cast<double>(bitErrors)/HammingCode<r>::k/numTrials, static_cast<double>(blockErrors)/numTrials};
    }

    template <std::size_t r>
    std::vector<BerResult> runSweep (const HammingCode<r>& code, RNG& rng)
    {
        std::vector<BerResult> results;
        results.reserve(pValues.size());
        for (auto p : pValues)
        {
            results.push_back(runPTest(code, rng, p));
        }
        return results;
    }
};

