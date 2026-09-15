#include "hamming_code.h"
#include "rng.h"
#include "neural_decoder.h"
#include "neural_training.h"
#include "neural_eval.h"
#include "csv_export.h"


int main()
{
    
    RNG rng{1};

    HammingCode<3> code3{};
    NeuralDecoder<3> net3{20, 0.01, rng};
    neural_training::trainNetwork(net3, code3, rng, 0.01, 50000);
    
    HammingCode<4> code4{};
    NeuralDecoder<4> net4{60, 0.01, rng};
    neural_training::trainNetwork(net4, code4, rng, 0.01, 100000);

    HammingCode<5> code5{};
    NeuralDecoder<5> net5{60, 0.01, rng};
    neural_training::trainNetwork(net5, code5, rng, 0.01, 100000);

    std::vector<ber_eval::BerResult> results3 {neural_eval::runSweep(HammingCode<3>(), net3, rng)};
    std::vector<ber_eval::BerResult> results4 {neural_eval::runSweep(HammingCode<4>(), net4, rng)};
    std::vector<ber_eval::BerResult> results5 {neural_eval::runSweep(HammingCode<5>(), net5, rng)};

    csv_export::writeCSV(results3, "../results/resultsNeural3.csv");
    csv_export::writeCSV(results4, "../results/resultsNeural4.csv");
    csv_export::writeCSV(results5, "../results/resultsNeural5.csv");

    



    return 0;

    
}
