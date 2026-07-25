/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GPU.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldesboui <ldesboui@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 20:01:41 by ldesboui          #+#    #+#             */
/*   Updated: 2026/07/25 15:05:07 by ldesboui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <array>
#include "Obj.hpp"
typedef struct s_glvertex
{
	float	x,y,z;
	float	r,g,b;
	float	tx,ty;
	float	nx,ny,nz;
	std::array<float, 3>	ka;	
	std::array<float, 3>	kd;	
	std::array<float, 3>	ks;
	float					d;
	float					ns;
	GLuint					texId;
}	t_glvertex;

typedef struct	s_renderSettings
{
	std::array<float, 3>	cameraPos;
	std::array<float, 3>	cameraFront;
	std::array<float, 3>	up;
	float					scale;
	std::array<float, 3>	translate;
	float					rx;
	float					ry;
	float					ObjRx;
	float					ObjRy;
	float					ObjRz;
	int						texturedView;
	bool					key1Released;
}	t_renderSettings;

class GPU
{
	private:
		GLuint					vao;
		GLuint					vbo;
		std::string				fragmentShader;
		std::string				vertexShader;
		std::vector<t_glvertex>	vboBuffer;
		GPU();
		GLFWwindow					*window;
		t_renderSettings			mySettings;
	public:
		~GPU();
		GPU(Obj &);
		void		process(Obj &myObj);
		void		loadShaderCodes();
		void		processInput();
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
