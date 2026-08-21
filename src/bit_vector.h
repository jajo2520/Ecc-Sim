#pragma once

#include <bitset>
#include <cstddef>
#include <iostream>
#include <stdexcept>

template <std::size_t dim>
class BitVector
{
public:
    BitVector(unsigned long long binaryInt);
    void set(std::size_t pos, bool state);
    void toggle(std::size_t pos);
    bool get(std::size_t pos) const; // checked access - has protection. 
    bool operator[](std::size_t pos) const; // can and should only be read only. Unchecked access.
    bool dot(const BitVector<dim>& vec) const;
    BitVector<dim> operator+=(const BitVector<dim>& vec);
    BitVector<dim> reverse() const;

    friend bool operator==(const BitVector<dim>& vec1, const BitVector<dim>& vec2) // have to define in class
    {
        return (vec1.mData==vec2.mData);
    }

    friend bool operator!=(const BitVector<dim>& vec1, const BitVector<dim>& vec2)
    {
        return !(vec1.mData==vec2.mData);
    }
private:
    std::bitset<dim> mData;
};

template <std::size_t dim>
std::ostream& operator<<(std::ostream& out, BitVector<dim> vec);

template <std::size_t dim>
BitVector<dim> operator+(BitVector<dim> vec1, BitVector<dim> vec2);


// definitions 

template <std::size_t dim>
BitVector<dim>::BitVector(unsigned long long binaryInt) : mData(binaryInt) {}

template <std::size_t dim>
void BitVector<dim>::set(std::size_t pos, bool state) {mData[pos]=state;}

template <std::size_t dim>
void BitVector<dim>::toggle(std::size_t pos) {mData.flip(pos);}

template <std::size_t dim>
bool BitVector<dim>::get(std::size_t pos) const 
{
    if (pos >= dim) throw std::out_of_range("BitVector::get");
    return mData[pos];
}

template <std::size_t dim>
bool BitVector<dim>::dot(const BitVector<dim>& vec) const
{
    bool acc {0};
    for (int i {}; i < dim; ++i)
    {
        acc ^= mData[i] * vec[i];
    }
    return acc;
}

template <std::size_t dim>
bool BitVector<dim>::operator[](std::size_t pos) const // only readable
{
    return mData[pos];
}

template <std::size_t dim>
BitVector<dim> BitVector<dim>::operator+=(const BitVector<dim>& vec)
{
    BitVector<dim> outVec(0);
    for (int i {}; i < dim; ++i)
    {
        outVec.set(i, mData[i] ^ vec[i]);
    }
    return outVec;
}

template <std::size_t dim>
BitVector<dim> BitVector<dim>::reverse() const
{
    BitVector<dim> outVec {0};
    for (std::size_t i {}; i < dim; ++i)
    {
        outVec.set(i, mData[dim-i-1]);
    }
    return outVec;
}

template <std::size_t dim>
BitVector<dim> operator==(const BitVector<dim>& vec1, const BitVector<dim>& vec2)
{
    return (vec1.mData==vec2.mData);
}

template <std::size_t dim>
BitVector<dim> operator!=(const BitVector<dim>& vec1, const BitVector<dim>& vec2)
{
    return !(vec1.mData==vec2.mData);
}
template <std::size_t dim>
BitVector<dim> operator+(BitVector<dim> vec1, BitVector<dim> vec2)
{
    return vec1 += vec2;    
}

template <std::size_t dim>
std::ostream& operator<<(std::ostream& out, BitVector<dim> vec) // worth noting this prints LSB first - for continuity with BitMatrix.
{
    for (size_t i {}; i < dim; ++i)
    {
        out << vec[i] << " ";
    }
    return out;
}
