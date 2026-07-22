/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Obj.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldesboui <ldesboui@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 11:41:05 by ldesboui          #+#    #+#             */
/*   Updated: 2026/07/22 18:41:45 by ldesboui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Obj.hpp"
#include <sstream>
#include <stdio.h>
Obj::Obj()
{
	defaultVn = (t_vn){.x =0.0f, .y= 0.0f, .z=0.0f};
	defaultVt = (t_vt){.x =0.0f, .y= 0.0f};
	mtl.ka = {0.2f, 0.2f, 0.2f};
	mtl.kd = {0.8f, 0.8f, 0.8f};
	mtl.ks = {0.0f, 0.0f, 0.0f};
	mtl.ni = 1.0f;
	mtl.d = 1.0f;
	mtl.ns = 0.0f;
}

Obj::~Obj()
{

}

std::vector<t_v>& Obj::getVertices()
{
	return (this->vertices);
}

std::vector<t_vn>& Obj::getVerticesNormal()
{
	return (this->verticesNormal);
}

std::vector<t_vt>& Obj::getVerticesTexture()
{
	return (this->verticesTexture);
}

std::vector<std::vector<t_facePoint> >& Obj::getFaces()
{
	return (this->faces);
}

static int cmpType(std::string type)
{
	std::string types[] = {"v", "vt", "vn", "f", "mtllib"};

	size_t	i = 0;
	while (i < 5)
	{
		if (type == types[i])
			return i;
		++i;
	}
	return SKIP;
}

void	Obj::parser(std::ifstream &file)
{
	std::string			line;
	std::string			type;

	while (std::getline(file, line))
	{
		std::istringstream	iss(line);
		//empty line we skip
		if (!(iss >> type))
			continue;
		switch (cmpType(type))
		{
			case (V):
				this->parseVLine(line);
				break;
			case (VT):
				this->parseVtLine(line);
				break;
			case (VN):
				this->parseVnLine(line);
				break;
			case (F):
				this->parseFLine(line);
				break;
			case (MTLLIB):
				this->parseLibLine(line);
			case (SKIP):
				break;
		}
	}
	computeNormals();
}

bool Obj::apply(std::string iV, std::string iVt, std::string iVn, size_t flag)
{
	size_t i = 0;
	t_facePoint aPoint;
	bool first = true;
	while (i < iV.size())
	{
		if (!std::isdigit(iV[i]))
		{
			if (!first)
				return false;
			else if (iV[i] == '+' || iV[i] == '-')
				first = false;
			else
				return false;
		}
		++i;
		first = false;
	}
	i = 0;
	first = true;
	while (i < iVt.size())
	{
		if (!std::isdigit(iVt[i]))
		{
			if (!first)
				return false;
			else if (iVt[i] == '+' || iVt[i] == '-')
				first = false;
			else
				return false;
		}
		++i;
		first = false;
	}
	i = 0;
	first = true;
	while (i < iVn.size())
	{
		if (!std::isdigit(iVn[i]))
		{
			if (!first)
				return false;
			else if (iVn[i] == '+' || iVn[i] == '-')
				first = false;
			else
				return false;
		}
		++i;
		first = false;
	}
	//-1 to respect openGl
	int indexV = std::atoi(iV.c_str()) - 1;
	int indexVt = std::atoi(iVt.c_str()) - 1;
	int indexVn = std::atoi(iVn.c_str()) - 1;
	if (indexV < 0 || static_cast<size_t>(indexV) >= this->vertices.size())
		return false;

	if (static_cast<ssize_t>(indexVt) >= static_cast<ssize_t>(this->verticesTexture.size()))
		return false;
	if (indexVt < 0)
		aPoint.vt = this->defaultVt;
	else
		aPoint.vt = this->verticesTexture[indexVt];

	if (static_cast<ssize_t>(indexVn) >= static_cast<ssize_t>(this->verticesNormal.size()))
		return false;
	if (indexVn < 0)
		aPoint.vn = this->defaultVn;
	else
		aPoint.vn = this->verticesNormal[indexVn];
	aPoint.v = this->vertices[indexV];
	if (flag == 0)
	{
		//new line so new vector
		std::vector<t_facePoint> tmp;
		this->faces.push_back(tmp);
		this->faces.back().push_back(aPoint);
	}
	else if (flag > 0 && flag < 3)
	{
		//not a new line but its still a triangle
		this->faces.back().push_back(aPoint);
	}
	else
	{
		//for f that has more than 3 points
		//retrieve the correct angles
		t_facePoint firstPoint = this->faces.back()[0];
		t_facePoint lastPoint = this->faces.back().back();	
		std::vector<t_facePoint> tmp;
		this->faces.push_back(tmp);
		this->faces.back().push_back(firstPoint);
		this->faces.back().push_back(lastPoint);
		this->faces.back().push_back(aPoint);

	}
	return true;
}

void	Obj::parseFLine(std::string line)
{
	std::istringstream	iss(line);
	std::string type;
	std::string tmpPointString;
	std::string indexV = "";
	std::string indexVt = "";
	std::string indexVn = "";
	size_t		flag = 0;
	//skip
	if (!(iss >> type))
		throw Obj::TheException("Bad f line format");
	while (iss >> tmpPointString)
	{
		std::istringstream tmpIss(tmpPointString);
		std::getline(tmpIss, indexV, '/');
		//these parameteres arent mandatory
		if (std::getline(tmpIss, indexVt, '/'))
		{
			std::getline(tmpIss, indexVn, '/');
		}
		if (indexV.empty())
			throw Obj::TheException("f line parsing error");
		if (indexVt.empty())
			indexVt = "0";
		if (indexVn.empty())
			indexVn = "0";
		if (!apply(indexV, indexVt, indexVn, flag))
			throw Obj::TheException("f line incorrect format");
		tmpIss >> std::ws;
		if (!tmpIss.eof())
			throw Obj::TheException("f line bad input");
		++flag;
	}
	if (flag < 2)
		throw Obj::TheException("f line incomplete");

}
void	Obj::parseVLine(std::string line)
{
	std::istringstream	iss(line);
	std::string type;
	t_v v;
	float x, y, z, r = 255.0f ,g = 255.0f, b = 255.0f;
	if  (iss >> type >> x >> y >> z)
	{
		v.x = x;
		v.y = y;
		v.z = z;
		v.r = r;
		v.g = g;
		v.b = b;
		//getting rid of whitespace (trim like)
		iss >> std::ws;
		//nothing more no rgb
		if (iss.eof())
			this->vertices.push_back(v);
		else
		{
			//if its not a float then it's garbage
			if  (iss >> r >> g >> b)
			{
				iss >> std::ws;
				if (!iss.eof())
					throw Obj::TheException("Bad v line RGB");
				v.r = r;
				v.g = g;
				v.b = b;
				if (r > 255 || r < 0 || g < 0 || g > 255 || b < 0 || b > 255)
					throw Obj::TheException("Bad v line RGB");
				this->vertices.push_back(v);
			}
			else
				throw Obj::TheException("Bad v line RGB");
		}
	}
	else
		throw Obj::TheException("Bad v line coordinates");
}


void	Obj::parseVtLine(std::string line)
{
	std::istringstream	iss(line);
	std::string type;
	t_vt vt;
	float x, y;
	if  (iss >> type >> x >> y)
	{
		vt.x = x;
		vt.y = y;
		//getting rid of whitespace (trim like)
		iss >> std::ws;
		//nothing more
		if (iss.eof())
			this->verticesTexture.push_back(vt);
		//there is garbage
		else
			throw Obj::TheException("Bad vt line");

	}
	else
		throw Obj::TheException("Bad vt line coordinates");
}

void	Obj::parseVnLine(std::string line)
{
	std::istringstream	iss(line);
	std::string type;
	t_vn vn;
	float x, y, z;
	if  (iss >> type >> x >> y >> z)
	{
		vn.x = x;
		vn.y = y;
		vn.z = z;
		//getting rid of whitespace (trim like)
		iss >> std::ws;
		//nothing more
		if (iss.eof())
			this->verticesNormal.push_back(vn);
		//there is garbage
		else
			throw Obj::TheException("Bad vn line");

	}
	else
		throw Obj::TheException("Bad vn line directions");


}

void	Obj::parseLibLine(std::string line)
{
	std::istringstream	iss(line);
	std::string type;
	std::string	fileName;
	if  (iss >> type >> fileName)
	{
		if (fileName.empty())
			throw Obj::TheException("mtlliib line filename is empty");
		std::ifstream mtlFile("resources/" + fileName);
		if (!mtlFile.is_open())
			throw Obj::TheException("mtllib line file could not be opened");
		Mtl Mtl;
		mtl = Mtl.parse(mtlFile);
	}
	else
		throw Obj::TheException("Bad mtllib line");
}

Obj::TheException::~TheException() throw(){}

Obj::TheException::TheException(std::string msg)
{
	this->message = msg;
}
const char *Obj::TheException::what() const throw()
{
	return this->message.c_str();
}

void	Obj::computeNormals()
{
	for (size_t i = 0; i < faces.size(); ++i)
	{
		if (faces[i].size() < 3)
			continue;
		t_v a = faces[i][0].v;
		t_v b = faces[i][1].v;
		t_v c = faces[i][2].v;

		float ux = b.x - a.x;
		float uy = b.y - a.y;
		float uz = b.z - a.z;
		float vx = c.x - a.x;
		float vy = c.y - a.y;
		float vz = c.z - a.z; 


		float nx = uy * vz - uz * vy;
		float ny = uz * vx - ux * vz;
		float nz = ux * vy - uy * vx;

		float absolute = std::sqrt(nx *nx + ny*ny + nz*nz);
		if (absolute > 0.0f)
		{
			nx /= absolute;
			ny /= absolute;
			nz /= absolute;
		}
		for (size_t j = 0; j < faces[i].size(); ++j)
		{
			t_facePoint &p = faces[i][j];
			if (p.vn.x == 0.0f && p.vn.y == 0.0f && p.vn.z == 0.0f)
			{
				p.vn.x = nx; p.vn.y = ny; p.vn.z = nz;
			}
		}
	}
}
