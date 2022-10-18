#pragma once

#include "../Data/Memory/Array.hpp"
#include "Vector.hpp"

template<Number T, size_t R, size_t C>
class Matrix
{
private:
	StackBlock<Vector<T, C>, R> m_rows;
public:
	Matrix() : m_rows() {}

	Matrix<T, R, C>& InitIdentity()
	{
		for(Size i = 0U; i < C; i++)
		{
			for(Size j = 0U; j < R; j++)
			{ 
				T value = j == i ? T::One : T::Zero;
				m_rows[j][i] = value;
			}
		}
	}

	friend Matrix<T, C, R> operator*(const Matrix<T, R, C>& left, const Matrix<T, C, R>& right)
	{
		Matrix<T, C, R> result;
        for(Size i = 0; i < C; i++)
        {
            for(Size j = 0; j < R; j++)
            {
				result.m_rows[i][j] = T::Zero;
                for(int k = 0; k < C; k++)
                    result.m_rows[i][j] = result.m_rows[i][j] + (left.m_rows[i][k] * right.m_rows[k][j]);
            }
        }
        return result;
	}

	String ToString() const
	{
		MutableString result;
		
		for(Size i = 0; i < C; i++)
        {
			result += "[";

			for(Size j = 0; j < R; j++)
			{
				result += m_rows[j][i].ToString();
				if(j < R - 1)
					result += ", ";
			}

			result += "]";
        }

		return result;
	}
};

template<Number T>
class Matrix4x4 : public Matrix<T, 4, 4>
{
public:

};