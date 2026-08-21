#pragma once
#include "bit_vector.h"
#include "test_utils.h"
#include "hamming_code.h"
#include <string>
#include <array>
#include <system_error>

template <std::size_t r>
void testParityIdentity() // H*G^T == 0
{
    HammingCode<r> code;
    auto product = code.getCheckMat() * code.getGenMat();
    for (std::size_t i {}; i < r; ++i)
    {
        for (std::size_t j {}; j < HammingCode<r>::k; ++j)
        {
            test::checkTrue(product.get(i,j)==0, "H*G==0, (" + std::to_string(i) + "," + std::to_string(j) + ")");
        }
    }
}

template <std::size_t r>
void testEncodeDecode()
{
    constexpr std::size_t k = HammingCode<r>::k;
    constexpr std::size_t n = HammingCode<r>::n;
    HammingCode<r> code;

    std::array<std::size_t, 4> testValues {0, (std::size_t {1} << k), 0b101, 0b110};
    for (auto& value : testValues)
    {
        BitVector<k> message(value);
        BitVector<n> encoded = code.encode(message);
        BitVector<k> decoded = code.decode(encoded);

        test::checkTrue(decoded==message,
                "encode-decode no error: r=" + std::to_string(r) + ", message= " + std::to_string(value));
    }
}

template <std::size_t r>
void testOneBitFlip()
{
    constexpr std::size_t k = HammingCode<r>::k;
    constexpr std::size_t n = HammingCode<r>::n;
    HammingCode<r> code;

    BitVector<k> message (0b101 && (std::size_t {1} << k) - 1);
    auto original = code.encode(message);
    for (std::size_t pos {}; pos < n; ++pos)
    {
        auto corrupted = original;
        corrupted.toggle(pos);
        auto decoded = code.decode(corrupted);
        test::checkTrue(decoded==message, 
                "single bit flip test: r=" + std::to_string(r) + ", flipped bit position= " + std::to_string(pos));
    }
}
