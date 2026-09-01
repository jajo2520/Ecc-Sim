#include "hamming_code.h"
#include "rng.h"
#include "ber_eval.h"

#include <vector>
#include <random>

int main()
{
    HammingCode<3> code {};
    RNG rng(std::random_device{}());

    std::vector<ber_eval::BerResult> results {ber_eval::runSweep(code, rng)};
    for (auto& result : results)
    {
        std::cout << "p: " << result.p << std::endl <<
                  "Bit Error Rate: " << result.bitErrorRate << std::endl << 
                  "Block Error Rate: " << result.blockErrorRate << std::endl << std::endl;
    }
    return 0;
    
}
