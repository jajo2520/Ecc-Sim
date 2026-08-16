#pragma once
#include <array>
#include <bitset>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include "bit_vector.h"

template <std::size_t row, std::size_t col>
class BitMatrix
{
public:
    BitMatrix() = default; 
    BitMatrix(const std::array<BitVector<col>, row>& rows);
    void set(std::size_t i, std::size_t j, bool state);
    void setRow(std::size_t i, const BitVector<col>& stateVec);
    void setCol(std::size_t j, const BitVector<row>& stateVec);
    bool get(std::size_t i, std::size_t j) const;
    BitVector<col> getRow(std::size_t i) const;
    BitVector<row> getCol(std::size_t j) const;

    BitMatrix<col, row> transpose() const;

    template <std::size_t row2>
    BitMatrix<row+row2,col> augment(BitMatrix<row2, col>& outMat) const;

    BitMatrix<row, col>& operator+=(const BitMatrix<row, col>& mat);

    template<std::size_t dim>
    static BitMatrix<dim, dim> identity();

private:
    std::bitset<row*col> mData;
    std::size_t getPos(std::size_t i, std::size_t j) const;
};

template <std::size_t row, std::size_t col>
BitMatrix<row, col> operator+(BitMatrix<row, col> mat1, const BitMatrix<row, col>& mat2);

template <std::size_t matRow, std::size_t matCol>
BitVector<matRow> operator*(BitMatrix<matRow, matCol> mat, BitVector<matRow> vec);

template <std::size_t row1, std::size_t col1, std::size_t row2, std::size_t col2>
BitMatrix<row1, col2> operator*(BitMatrix<row1, col1> mat1, BitMatrix<row2, col2> mat2);


template <std::size_t row, std::size_t col>
std::ostream& operator<<(std::ostream& out, BitMatrix<row, col> mat);


// definitions

template <std::size_t row, std::size_t col>
BitMatrix<row, col>::BitMatrix(const std::array<BitVector<col>, row>& rows)
{ 
    for (size_t i {}; i<row; i++)
        setRow(i,rows[i]);
}


template <std::size_t row, std::size_t col>
std::size_t BitMatrix<row, col>::getPos(std::size_t i, std::size_t j) const
{
    std::size_t outIndex {i*col + j};
    if (outIndex >= row*col)
        throw std::out_of_range("Index out of range");

    return outIndex;
}

template <std::size_t row, std::size_t col>
void BitMatrix<row, col>::set(std::size_t i, std::size_t j, bool state)
{
    mData[getPos(i,j)] = state;
}

template <std::size_t row, std::size_t col>
void BitMatrix<row, col>::setRow(std::size_t i, const BitVector<col>& stateVec)
{
    for (int k {}; k < col; ++k)
    {
        mData[getPos(i,k)] = stateVec[k];
    }
}

template <std::size_t row, std::size_t col>
void BitMatrix<row, col>::setCol(std::size_t j, const BitVector<row>& stateVec)
{
    for (int k {}; k < row; ++k)
    {
        mData[getPos(k,j)] = stateVec[k];
    }
}

template <std::size_t row, std::size_t col>
bool BitMatrix<row, col>::get(std::size_t i, std::size_t j) const
{
    return mData[getPos(i,j)];
}

template <std::size_t row, std::size_t col>
BitVector<col> BitMatrix<row, col>::getRow(std::size_t i) const 
{
    BitVector<col> outVec(0);
    for (size_t k {}; k < col; ++k)
    {
        outVec.set(k, mData[getPos(i,k)]);
    }
    return outVec;
}

template <std::size_t row, std::size_t col>
BitVector<row> BitMatrix<row, col>::getCol(std::size_t j) const 
{
    BitVector<row> outVec(0);
    for (int k {}; k < row; ++k)
    {
        outVec.set(k, mData[getPos(k,j)]);
    }
    return outVec;
}

template <std::size_t row, std::size_t col>
BitMatrix<col, row> BitMatrix<row, col>::transpose() const 
{
    BitMatrix<col, row> outMat {};
    for (int i {}; i < row; ++i)
    {
        for (int j {}; j < col; ++j)
        {
            outMat.set(j,i,mData[getPos(i,j)]);
        }

    }
    return outMat;
}

template <std::size_t row, std::size_t col>
template <std::size_t row2>
BitMatrix<row+row2,col> BitMatrix<row, col>::augment(BitMatrix<row2, col>& mat2) const
{
    BitMatrix<row+row2,col> outMat {};
    for (std::size_t i {}; i < row+row2; ++i)
    {
        for (std::size_t j {}; j < col; ++j)
        {
            if (i < row)
                outMat.set(i,j,getPos(i,j));
            else 
                outMat.set(i,j,mat2.getPos(i,j));
        }
    }
    return outMat;
}

template <std::size_t row, std::size_t col>
BitMatrix<row, col>& BitMatrix<row ,col>::operator+=(const BitMatrix<row, col>& mat)
{
    mData ^= mat.mData;
    return *this;
}


template <std::size_t row, std::size_t col>
BitMatrix<row, col> operator+(BitMatrix<row, col> mat1, const BitMatrix<row, col>& mat2)
{
    return mat1 += mat2;
}

template <std::size_t row, std::size_t col>
std::ostream& operator<<(std::ostream& out, BitMatrix<row, col> mat)
{
    for (int i {}; i < row; ++i)
    {
        for (int j {}; j < col; ++j)
        {
            out << mat.get(i,j) << " ";
        }
        out << "\n";
    }
    return out;
}

template <std::size_t matRow, std::size_t matCol>
BitVector<matRow> operator*(BitMatrix<matRow, matCol> mat, BitVector<matCol> vec)
{
    BitVector<matRow> outVec(0);
    for (int i {}; i < matRow; ++i)
    {
        outVec.set(i, mat.getRow(i).dot(vec));
    }
    return outVec;
}

template <std::size_t row1, std::size_t col1, std::size_t row2, std::size_t col2>
BitMatrix<row1, col2> operator*(BitMatrix<row1, col1> mat1, BitMatrix<row2, col2> mat2)
{
    static_assert(col1 == row2, "mat dims are incorrect");
    BitMatrix<row1, col2> outMat {};
    for (std::size_t i {}; i < row1; ++i)
    {
        for (std::size_t j {}; j < col2; ++j)
        {
            outMat.set(i,j,mat1.getRow(i).dot(mat2.getCol(j)));
        }
    }
    return outMat;
}

template <std::size_t row, std::size_t col>
template <std::size_t dim>
BitMatrix<dim, dim> BitMatrix<row, col>::identity()
{
    BitMatrix<dim, dim> outMat {};
    for (std::size_t i; i<dim; ++i)
        outMat.set(i,i, 1);

    return outMat;
}
