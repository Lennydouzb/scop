/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldesboui <ldesboui@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 10:35:21 by ldesboui          #+#    #+#             */
/*   Updated: 2026/07/24 11:04:24 by ldesboui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include "includes/Obj.hpp"
#include "includes/GPU.hpp"
#include "includes/Matrix.hpp"

int	main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Usage is './scop <file.obj>'." << std::endl;
		return 1;
	}
	std::ifstream objFile(av[1]);
	if (!objFile.is_open())
	{
		std::string name = av[1];
		std::cerr << name + " : This file can't be opened or doesn't exist." << std::endl;
		return 1;
	}
	Obj myObj;
	Matrix::init();
	try
	{
		myObj.parser(objFile);
		GPU myGPU(myObj);
		myGPU.loadShaderCodes();
		myGPU.process();
	}catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
