#pragma once

#include <array>
#include <vector>

#include "rng.h"

inline double sigmoid(double x)
{
    return 1.0 / (1.0 + std::exp(-x));
}

template <std::size_t r>
class NeuralDecoder
{
public:
    NeuralDecoder<r>(std::size_t hiddenSize, double learningRate, RNG& rng); // rng for weight initialisation
    static constexpr std::size_t n {(1u << r) - 1};
    static constexpr std::size_t k {n-r};

    std::array<double, n> forward(const std::array<double, r>& syndrome); // non-void return type as we want to return 
                                                                          // predictions after training.
    void backward(const std::array<double, n>& target);
    double computeLoss(const std::array<double, n>& target);
    std::vector<double> getA1() {return a1;};

private:
    std::size_t h; // everything that can be arrays are arrays (anything that isn't controlled by h parameter)
    double eta; 
    std::vector<std::vector<double>> W1; // h x r
    std::vector<double> b1;
    std::vector<std::vector<double>> W2; // n x h
    std::array<double, n> b2;
    std::array<double, r> x;
    std::vector<double> a1;
    std::array<double, n> yhat;

    void initWeights(RNG& rng);
};

// definitions

template <std::size_t r>
NeuralDecoder<r>::NeuralDecoder(std::size_t hiddenSize, double learningRate, RNG& rng) : h{hiddenSize}, 
                                                                                         eta{learningRate},
                                                                                         W1(h, std::vector<double>(r)),
                                                                                         b1(h),
                                                                                         W2(n, std::vector<double>(h)),
                                                                                         b2{},
                                                                                         x{}, 
                                                                                         a1(h),
                                                                                         yhat{}
{
    initWeights(rng);
}

template <std::size_t r>
void NeuralDecoder<r>::initWeights(RNG& rng)
{
    double range1 = 1.0 / std::sqrt(static_cast<double>(r));
    double range2 = 1.0 / std::sqrt(static_cast<double>(h));

    for (std::size_t j {}; j < h; ++j)
    {
        for (std::size_t i {}; i < r; ++i)
        {
            W1[j][i] = rng.randomDouble(-range1, range1); 
        }
        b1[j] = 0.0;
    }
    for (std::size_t j {}; j < n; ++j)
    {
        for (std::size_t i {}; i < h; ++i)
        {
            W2[j][i] = rng.randomDouble(-range2, range2);
        }
        b2[j] = 0.0;
    }
}

template <std::size_t r>
std::array<double, NeuralDecoder<r>::n> NeuralDecoder<r>::forward(const std::array<double, r>& syndrome)
{
    x = syndrome;

    // hidden layer
    for (std::size_t j {}; j < h; ++j)
    {
        double z1 {b1[j]};
        for (std::size_t i {}; i < r; ++i)
        {
            z1 += W1[j][i] * x[i];
        }
        a1[j] = sigmoid(z1);
    }
    // output layer
    for (std::size_t j {}; j < n; ++j)
    {
        double z2 {b2[j]};
        for (std::size_t i {}; i < h; ++i)
        {
            z2 += W2[j][i] * a1[i];
        }
        yhat[j] = sigmoid(z2);
    }
    return yhat;
}


template <std::size_t r>
void NeuralDecoder<r>::backward(const std::array<double, n>& target)
{
    std::array<double, n> delta2 {};
    for (std::size_t k {}; k < n; ++k)
    {
        delta2[k] = yhat[k] - target[k];
    }

    std::vector<double> delta1(h);
    for (std::size_t j {}; j < h; ++j)
    {
        double sum {};
        for (std::size_t k {}; k < n; ++k)
        {
            sum += W2[k][j] * delta2[k];
        }
        delta1[j] = sum * a1[j] * (1.0 - a1[j]);
    }

    for (std::size_t k {}; k < n; ++k)
    {
        for (std::size_t j {}; j < h; ++j)
        {
            W2[k][j] -= eta * delta2[k] * a1[j];
        }
        b2[k] -= eta * delta2[k];
    }
     
    for (std::size_t j {}; j < h; ++j)
    {
        for (std::size_t i {}; i < r; ++i)
            W1[j][i] -= eta * delta1[j] * x[i];
        b1[j] -= eta * delta1[j];
    }
}

template <std::size_t r>
double NeuralDecoder<r>::computeLoss(const std::array<double, n>& target)
{
    double loss {};
    for (std::size_t i {}; i < n; ++i)
    {
        loss -= target[i] * std::log(yhat[i]) + (1.0 - target[i]) * std::log(1.0 - yhat[i]); // binary cross-entropy
    }
    return loss;
}
