/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Obj.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldesboui <ldesboui@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 10:49:24 by ldesboui          #+#    #+#             */
/*   Updated: 2026/07/25 00:34:46 by ldesboui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <sstream>
#include <cmath>
#include <array>
#include "Mtl.hpp"
//vertex position and color
enum e_typeLine {
	V = 0,
	VT = 1,
	VN = 2,
	F = 3,
	MTLLIB = 4,
	USEMTL = 5
};

typedef struct s_v
{
	float	x;
	float	y;
	float	z;
	float	r;
	float	g;
	float	b;
}	t_v;
//normal
typedef struct s_vn
{
	float	x;
	float	y;
	float	z;
}	t_vn;
//texture coordinates
typedef struct s_vt
{
	float	x;
	float	y;
}	t_vt;
//one spot of a face
typedef struct s_facePoint
{
	t_v		v;
	t_vt	vt;
	t_vn	vn;
	t_mtl	mtl;
}	t_facePoint;


class Obj 
{
	private:
		std::vector<t_v>								vertices;
		std::vector<t_vn>								verticesNormal;
		std::vector<t_vt>								verticesTexture;
		std::vector<std::vector<t_facePoint> >			faces;
		t_vn											defaultVn;
		t_vt											defaultVt;
		t_mtl											emptymtl;
		t_mtl											actualmtl;
		std::vector<t_mtl>								mtllib;
		float											xMax;
		float											xMin;
		float											yMax;
		float											yMin;
		float											zMax;
		float											zMin;

		void											parseFLine(std::string line);
		void											parseVLine(std::string line);
		void											parseVtLine(std::string line);
		void											parseVnLine(std::string line);
		void											parseLibLine(std::string line);
		void											parseUsemtlLine(std::string line);
		void											computeNormals();
		void											centerVertices();
		ssize_t											findMtlByName(std::string name);
	public:
		Obj();
		~Obj();
		bool											apply(std::string, std::string, std::string, size_t);
		std::vector<t_v>&								getVertices();
		std::vector<t_vn>&								getVerticesNormal();
		std::vector<t_vt>&								getVerticesTexture();
		std::vector<std::vector<t_facePoint> >&			getFaces();
		std::vector<size_t>								indexesOfMtlSwicth;
		size_t											sameMtlFaceCounter;
		void											parser(std::ifstream &file);
		class TheException: public std::exception
		{
		private:
			std::string message;
		public:
			~TheException() throw();
			TheException(std::string message);
			const char *what() const throw();
		};
};
