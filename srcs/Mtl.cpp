/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mtl.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldesboui <ldesboui@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 11:58:08 by ldesboui          #+#    #+#             */
/*   Updated: 2026/07/25 01:42:02 by ldesboui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/Mtl.hpp"
#include <filesystem>
#include <stdio.h>

Mtl::Mtl()
{
	mtl.ka = {0.2f, 0.2f, 0.2f};
	mtl.kd = {0.8f, 0.8f, 0.8f};
	mtl.ks = {0.0f, 0.0f, 0.0f};
	mtl.ni = 1.0f;
	mtl.d = 1.0f;
	mtl.ns = 0.0f;
	mtl.name = "";
}

static int cmpType(std::string type)
{
	std::string types[] = {"Ka", "Kd", "Ks", "Ni", "d", "Ns", "map_kd", "newmtl"};

	size_t	i = 0;
	while (i < 8)
	{
		if (type == types[i])
			return i;
		++i;
	}
	return SKIP;
}

std::vector<t_mtl> Mtl::parse(std::ifstream &file)
{
	std::string			line;
	std::string			type;
	bool				first = true;
	std::vector<t_mtl>	res;
	while (std::getline(file, line))
	{
		std::istringstream	iss(line);
		//empty line we skip
		if (!(iss >> type))
			continue;
		switch (cmpType(type))
		{
			case (ka):
				this->parseKa(line);
				break;
			case (kd):
				this->parseKd(line);
				break;
			case (ks):
				this->parseKs(line);
				break;
			case (ni):
				this->parseNi(line);
				break;
			case (d):
				this->parseD(line);
				break;
			case (ns):
				this->parseNs(line);
				break;
			case (map_kd):
				this->parseMapKd(line);
				break;
			case (newmtl):
				if (!first)
					res.push_back(this->mtl);
				first = false;
				this->mtl = (t_mtl){.ka={0.2f,0.2f,0.2f}, .kd={0.8f,0.8f,0.8f}, .ks={0.0f,0.0f,0.0f}, .ni=1.0f, .d=1.0f, .ns=0.0f, .ppm_witdh=0, .ppm_height=0, .map_kd={}, .name=""};
				this->parseNewmtl(line);
				break;
			case (SKIP):
				break;
		}
	}
	if (!first)
        res.push_back(this->mtl);
	else
		throw Mtl::TheException("No mtl defined");
	return res;
}

void	Mtl::parseNewmtl(std::string line)
{
	std::istringstream	iss(line);
	std::string type;
	std::string name;
	if  (iss >> type >> name)
	{
		//getting rid of whitespace (trim like)
		iss >> std::ws;
		//nothing more
		if (iss.eof())
		{
			if (std::find(existingName.begin(), existingName.end(), name) == existingName.end())
			{
				this->mtl.name = name;
				this->existingName.push_back(name);
			}
			else
				throw Mtl::TheException("Bad newmtl line, name already exists");
		}
		//there is garbage
		else
			throw Mtl::TheException("Bad newmtl line");
	}
	else
		throw Mtl::TheException("Bad newmtl line name");
}

void Mtl::parseKa(std::string line)
{
	std::istringstream	iss(line);
	std::string type;
	float a, b ,c ;
	if  (iss >> type >> a >> b >> c)
	{
		//getting rid of whitespace (trim like)
		iss >> std::ws;
		//nothing more
		if (iss.eof())
		{
			this->mtl.ka = {a, b, c};
		}
		//there is garbage
		else
			throw Mtl::TheException("Bad vt line");

	}
	else
		throw Mtl::TheException("Bad vt line coordinates");
}

void Mtl::parseKd(std::string line)
{
	std::istringstream	iss(line);
	std::string type;
	float a, b ,c ;
	if  (iss >> type >> a >> b >> c)
	{
		//getting rid of whitespace (trim like)
		iss >> std::ws;
		//nothing more
		if (iss.eof())
		{
			this->mtl.kd = {a, b, c};
		}
		//there is garbage
		else
			throw Mtl::TheException("Bad kd line");

	}
	else
		throw Mtl::TheException("Bad kd line values");
}

void Mtl::parseKs(std::string line)
{
	std::istringstream	iss(line);
	std::string type;
	float a, b ,c ;
	if  (iss >> type >> a >> b >> c)
	{
		//getting rid of whitespace (trim like)
		iss >> std::ws;
		//nothing more
		if (iss.eof())
		{
			this->mtl.ks = {a, b, c};
		}
		//there is garbage
		else
			throw Mtl::TheException("Bad ks line");

	}
	else
		throw Mtl::TheException("Bad ks line values");
}

void Mtl::parseNi(std::string line)
{
	std::istringstream	iss(line);
	std::string type;
	float a;
	if  (iss >> type >> a)
	{
		//getting rid of whitespace (trim like)
		iss >> std::ws;
		//nothing more
		if (iss.eof())
		{
			this->mtl.ni = a;
		}
		//there is garbage
		else
			throw Mtl::TheException("Bad ni line");

	}
	else
		throw Mtl::TheException("Bad ni liine value");
}

void Mtl::parseD(std::string line)
{
	std::istringstream	iss(line);
	std::string type;
	float a;
	if  (iss >> type >> a)
	{
		//getting rid of whitespace (trim like)
		iss >> std::ws;
		//nothing more
		if (iss.eof())
		{
			this->mtl.d = a;
		}
		//there is garbage
		else
			throw Mtl::TheException("Bad d line");

	}
	else
		throw Mtl::TheException("Bad d line values");

}

void Mtl::parseNs(std::string line)
{
	std::istringstream	iss(line);
	std::string type;
	float a;
	if  (iss >> type >> a)
	{
		//getting rid of whitespace (trim like)
		iss >> std::ws;
		//nothing more
		if (iss.eof())
		{
			this->mtl.ns = a;
		}
		//there is garbage
		else
			throw Mtl::TheException("Bad ns line");
	}
	else
		throw Mtl::TheException("Bad ns line value");

}

void Mtl::parseMapKd(std::string line)
{
	std::istringstream	iss(line);
	std::string type;
	std::string map_kdFileName;
	if  (iss >> type >> map_kdFileName)
	{
		//getting rid of whitespace (trim like)
		iss >> std::ws;
		//nothing more
		if (iss.eof())
		{
			parsePpm("resources/"+map_kdFileName);
		}
		//there is garbage
		else
			throw Mtl::TheException("Bad map_kd line");

	}
	else
		throw Mtl::TheException("Bad map_kd line filename");

}


static void	skipComment(std::ifstream &file)
{
	//skip whitespaces
	file >> std::ws;
	std::string garbageCollector;
	while (file.peek() == '#')
	{
		std::getline(file, garbageCollector);
		file >> std::ws;
	}
}
void Mtl::parsePpm(std::string fileName)
{
	//this is to manage p6 ppm version that is full binary (no text)
	std::ifstream file(fileName, std::ios::binary);
    if (!file.is_open())
	{
        throw std::runtime_error("could not open " + fileName);
    }
	std::string version;
	if (!(file >> version))
		Mtl::TheException("There was an error during ppm parsing");
	if (version != "P6")
		Mtl::TheException("PPM file incorrect version, only P6 is managed");
	skipComment(file);
	file >> mtl.ppm_witdh;
	skipComment(file);
	file >> mtl.ppm_height;
	skipComment(file);
	int	maxColor;
	file >> maxColor;
	if (maxColor < 0 || maxColor > 255)
		Mtl::TheException("PPM file incorrect max color");
	std::string garbageCollector;
	getline(file, garbageCollector);
	size_t size = mtl.ppm_height * mtl.ppm_witdh * 3;
	mtl.map_kd.resize(size);
	file.read(reinterpret_cast<char*>(mtl.map_kd.data()), size);
}
Mtl::TheException::~TheException() throw(){}

Mtl::TheException::TheException(std::string msg)
{
	this->message = msg;
}
const char *Mtl::TheException::what() const throw()
{
	return this->message.c_str();
}
