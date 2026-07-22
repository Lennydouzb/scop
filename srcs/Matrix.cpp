/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Matrix.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldesboui <ldesboui@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 01:30:20 by ldesboui          #+#    #+#             */
/*   Updated: 2026/07/21 19:16:49 by ldesboui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Matrix.hpp"
std::array<float, 16> Matrix::identity;

std::array<float, 16> Matrix::getIdentity()
{
	return (Matrix::identity);
}

std::array<float, 16> Matrix::getTranslate(float x, float y, float z)
{
	std::array<float, 16> matrix = Matrix::getIdentity();
	matrix[12] = x;
	matrix[13] = y;
	matrix[14] = z;
	return (matrix);
}

std::array<float, 16> Matrix::getScale(float x)
{
	std::array<float, 16> matrix = Matrix::getIdentity();
	matrix[0] = x;
	matrix[5] = x;
	matrix[10] = x;
	return (matrix);
}
void	Matrix::init()
{
	for (int i = 0; i < 16 ; ++i)
		Matrix::identity[i] = 0;
	Matrix::identity[0] = 1;
	Matrix::identity[5] = 1;
	Matrix::identity[10] = 1;
	Matrix::identity[15] = 1;
}
std::array<float, 16> Matrix::getXRotation(float angle)
{
	std::array<float, 16> matrix = Matrix::getIdentity();
	matrix[5] = std::cos(angle);
	matrix[9] = -(std::sin(angle));
	matrix[6] = std::sin(angle);
	matrix[10] = std::cos(angle);
	return (matrix);

}
std::array<float, 16> Matrix::getYRotation(float angle)
{
	std::array<float, 16> matrix = Matrix::getIdentity();
	matrix[0] = std::cos(angle);
	matrix[2] = -(std::sin(angle));
	matrix[8] = std::sin(angle);
	matrix[10] = std::cos(angle);
	return (matrix);

}
std::array<float, 16> Matrix::getZRotation(float angle)
{
	std::array<float, 16> matrix = Matrix::getIdentity();
	matrix[0] = std::cos(angle);
	matrix[4] = -(std::sin(angle));
	matrix[1] = std::sin(angle);
	matrix[5] = std::cos(angle);
	return (matrix);

}
std::array<float, 16> Matrix::getProjection(float fov, float screenRatio, float near, float far)
{
	std::array<float, 16> matrix = Matrix::getIdentity();
	float f = 1/(std::tan(fov / 2));
	matrix[0] = f/screenRatio;
	matrix[5] = f;
	matrix[10] = - ((far + near) / (far - near));
	matrix[11] = -1;
	matrix[14] = -((2 * far * near) / (far - near));
	return matrix;

}

std::array<float, 16> Matrix::getLookAt(std::array<float, 3> up, 
		std::array<float, 3> front, std::array<float, 3> right, std::array<float, 3> pos)
{
	std::array<float, 16> matrix = Matrix::getIdentity();
	matrix[0] = right[0];
	matrix[4] = right[1];
	matrix[8] = right[2];
	matrix[1] = up[0];
	matrix[5] = up[1];
	matrix[9] = up[2];
	matrix[2] = -front[0];
	matrix[6] = -front[1];
	matrix[10] = -front[2];

	matrix[12] = -(right[0]*pos[0] + right[1]*pos[1] + right[2]*pos[2]);
    matrix[13] = -(up[0]*pos[0]    + up[1]*pos[1]    + up[2]*pos[2]);
    matrix[14] =  (front[0]*pos[0] + front[1]*pos[1] + front[2]*pos[2]);
	return matrix;
}

std::array<float, 16> Matrix::multiply(std::array<float, 16> m1, std::array<float, 16> m2)
{
	std::array<float, 16> matrix = Matrix::getIdentity();
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j< 4; ++j)
		{
			matrix[i * 4 + j] =
				m1[0 * 4 + j] * m2[i * 4 + 0] +
				m1[1 * 4 + j] * m2[i * 4 + 1] +
				m1[2 * 4 + j] * m2[i * 4 + 2] +
				m1[3 * 4 + j] * m2[i * 4 + 3];
		}
	}
	return matrix;
}

std::array<float, 4> Matrix::multiply(std::array<float, 16> m1, std::array<float, 4> m2)
{
	std::array<float, 4> matrix;
	matrix[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
	matrix[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
	matrix[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
	matrix[3] = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];
	return matrix;
}

std::array<float, 3>  Matrix::normalize(std::array<float, 3> m)
{
	float len = std::sqrt(m[0] *m[0] + m[1] * m[1] + m[2] * m[2]);
	if (len == 0.0f)
		return {0.0f, 0.0f, 0.0f};
	return {m[0] / len, m[1] / len, m[2] / len};


}

std::array<float, 3>  Matrix::cross(std::array<float, 3> m1, std::array<float, 3> m2)
{
	return {m1[1] * m2[2] - m1[2] * m2[1], m1[2] * m2[0] - m1[0] * m2[2], m1[0] * m2[1] - m1[1] * m2[0]};
}

std::array<float, 3> Matrix::add(std::array<float, 3> m1, std::array<float, 3> m2)
{
    return {m1[0] + m2[0], m1[1] + m2[1], m1[2] + m2[2]};
}
std::array<float, 3> Matrix::sub(std::array<float, 3> m1, std::array<float, 3> m2)
{
    return {m1[0] - m2[0], m1[1] - m2[1], m1[2] - m2[2]};
}
std::array<float, 3>  Matrix::multiply(std::array<float, 3>m , float number)
{
    return {m[0] * number, m[1] * number, m[2] * number};
}
///scalar product
float dot(std::array<float, 3> m1, std::array<float, 3> m2)
{
	return (m1[0] * m2[0] + m1[1] * m2[1] + m1[2] * m2[2]);
}
