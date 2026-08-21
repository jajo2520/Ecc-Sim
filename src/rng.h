#pragma once
#include <random>
#include <chrono>

class RNG
{
private:
    std::mt19937 mt;

public:
    RNG(): mt{ static_cast<std::mt19937::result_type>(
		    std::chrono::steady_clock::now().time_since_epoch().count())}
    {};
    int randInt(int lower, int upper)
    {
        std::uniform_int_distribution dist{lower, upper};
        return dist(mt);
        // code shamelessly yanked from the internet
    }
    float randFloat(float lower, float upper)
    {
        std::uniform_real_distribution dist{lower, upper};
        return dist(mt);
    }
};
