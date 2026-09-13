#pragma once

#include "ber_eval.h"

#include <fstream>
#include <vector>

namespace csv_export 
{
    void writeCSV (const std::vector<ber_eval::BerResult>& results, std::string name)
    {
        std::ofstream outFile(name);
        if (!outFile) 
        {
            std::cerr << "Failed to open file for writing.\n";
        }

        outFile << "p, bit_error_rate, block_error_rate\n";

        for (auto& result : results)
        {
            outFile << result.p << "," << result.bitErrorRate << "," << result.blockErrorRate << "\n";
        }
        outFile.close();
    }
}


