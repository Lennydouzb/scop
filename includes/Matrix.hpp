/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Matrix.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldesboui <ldesboui@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 00:54:00 by ldesboui          #+#    #+#             */
/*   Updated: 2026/07/21 18:17:54 by ldesboui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#pragma once
#include <cmath>
#include <array>
class Matrix
{
	private:
		static std::array<float, 16> identity;
		Matrix();
	public:
		static	void	init();
		static	std::array<float, 16> getLookAt(std::array<float, 3> up, std::array<float, 3> front
				, std::array<float, 3> right, std::array<float, 3> pos);
		static	std::array<float, 16> getIdentity();
		static	std::array<float, 16> getTranslate(float x, float y, float z);
		static	std::array<float, 16> getScale(float x);
		static	std::array<float, 16> getXRotation(float angle);
		static	std::array<float, 16> getYRotation(float angle);
		static	std::array<float, 16> getZRotation(float angle);
		static	std::array<float, 16> getProjection(float fov, float screenRatio, float near, float far);
		static	std::array<float, 16> getView(std::array<float, 3> up, std::array<float, 3> right, std::array<float, 3> direction);
		static	std::array<float, 16> multiply(std::array<float, 16> , std::array<float, 16> );
		static	std::array<float, 4>  multiply(std::array<float, 16> m1, std::array<float, 4> m2);
		static	std::array<float, 3>  multiply(std::array<float, 3>m , float number);
		static	std::array<float, 3>  normalize(std::array<float, 3> m);
		static	std::array<float, 3>  cross(std::array<float, 3> m, std::array<float, 3>m2);
		static	std::array<float, 3>  add(std::array<float, 3> m1, std::array<float, 3> m2);
		static	std::array<float, 3>  sub(std::array<float, 3> m1, std::array<float, 3> m2);
		static	float				  dot(std::array<float, 3> m1, std::array<float, 3> m2);
};
