#include "hamming_code.h"
#include "rng.h"
#include "ber_eval.h"
#include "csv_export.h"

#include <vector>
#include <random>

int main()
{
    HammingCode<3> code {};
    RNG rng(std::random_device{}());

    std::vector<ber_eval::BerResult> results3 {ber_eval::runSweep(HammingCode<3>(), rng)};
    std::vector<ber_eval::BerResult> results4 {ber_eval::runSweep(HammingCode<4>(), rng)};
    std::vector<ber_eval::BerResult> results5 {ber_eval::runSweep(HammingCode<5>(), rng)};

    csv_export::writeCSV(results3, "../results/results3.csv");
    csv_export::writeCSV(results4, "../results/results4.csv");
    csv_export::writeCSV(results5, "../results/results5.csv");
    return 0;
    
}
