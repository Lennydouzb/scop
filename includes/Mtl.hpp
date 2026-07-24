/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mtl.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldesboui <ldesboui@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 11:57:16 by ldesboui          #+#    #+#             */
/*   Updated: 2026/07/24 14:07:06 by ldesboui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#pragma once
#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
//these are mtl information
typedef struct s_mtl
{
	//ambient color
	std::array<float, 3>	ka;
	//diffuse color (object base color)
	std::array<float, 3>	kd;
	//spcular (light reflect color)
	std::array<float, 3>	ks;
	//optical density
	float					ni;
	//transparency
	float					d;
	//shinyness
	float					ns;

	int						ppm_witdh;
	int						ppm_height;
	std::vector<u_int8_t>	map_kd;
	std::string				name;

}	t_mtl;
enum e_mtlType
{
	ka = 0,
	kd = 1,
	ks = 2,
	ni = 3,
	d = 4,
	ns = 5,
	map_kd = 6,
	newmtl = 7,
	SKIP = 8
};

class Mtl
{
	private:
		t_mtl	mtl;
		std::vector<std::string> existingName;
	public:
		Mtl();
		t_mtl	parse(std::ifstream &file);
		void	parsePpm(std::string fileName);
		void	parseKa(std::string line);
		void	parseKd(std::string line);
		void	parseKs(std::string line);
		void	parseNi(std::string line);
		void	parseD(std::string line);
		void	parseNs(std::string line);
		void	parseMapKd(std::string line);
		void	parseNewmtl(std::string line);
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
