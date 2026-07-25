/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GPU.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldesboui <ldesboui@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 20:26:55 by ldesboui          #+#    #+#             */
/*   Updated: 2026/07/25 01:07:37 by ldesboui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/GPU.hpp"
#include "../includes/Matrix.hpp"
#include <GLFW/glfw3.h>
#include <utility>

GPU::GPU(){}


GPU::~GPU(){}

GPU::GPU(Obj &Object)
{
	std::vector<std::vector<t_facePoint> >& faces = Object.getFaces();

	for (size_t i = 0; i < faces.size(); ++i)
	{
		for (size_t j = 0; j < faces[i].size(); ++j)
		{
			t_facePoint aPoint = faces[i][j];
			t_glvertex vertex;
			vertex.x = aPoint.v.x;
			vertex.y = aPoint.v.y;
			vertex.z = aPoint.v.z;
			vertex.r = aPoint.v.r / 255.0f;
			vertex.g = aPoint.v.g / 255.0f;
			vertex.b = aPoint.v.b / 255.0f;

			vertex.nx = aPoint.vn.x;
			vertex.ny = aPoint.vn.y;
			vertex.nz = aPoint.vn.z;

			vertex.tx = aPoint.vt.x;
			vertex.ty = aPoint.vt.y;
			vertex.ka = aPoint.mtl.ka;
			vertex.kd = aPoint.mtl.kd;
			vertex.ks = aPoint.mtl.ks;
			vertex.d = aPoint.mtl.d;
			vertex.ns = aPoint.mtl.ns;
			this->vboBuffer.push_back(vertex);
		}
	}
	mySettings.cameraPos = {0.0f, 0.0f, 3.0f};
	mySettings.up = {0.0f, 1.0f, 0.0f};
	mySettings.cameraFront = {0.0f, 0.0f, -1.0f};
	mySettings.scale = 1.0f;
	mySettings.translate = {0.0f, 0.0f, 0.0f};
	mySettings.rx = 0.0f;
	mySettings.ry = -90.0f;
	mySettings.ObjRx = 0;
	mySettings.ObjRy = 0;
	mySettings.ObjRz = 0;
	if (!glfwInit())
		throw GPU::TheException("initialization did not work");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	this->window = glfwCreateWindow(1920, 1080, "scop", glfwGetPrimaryMonitor(), NULL);
	if (!window)
	{
		glfwTerminate();
		throw GPU::TheException("window creation did not work");
	}
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
		throw GPU::TheException("glew initalization did not work");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GPU::process(Obj &myObj)
{
	const char *strVertexShader = this->vertexShader.c_str();
	const char *strFragmentShader = this->fragmentShader.c_str();
	//compiling both shader program
	GLuint vertexShader =glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &(strVertexShader), NULL);
	glCompileShader(vertexShader);
	GLuint fragmentShader =glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &(strFragmentShader), NULL);
	glCompileShader(fragmentShader);

	char infoLog[512];
	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		throw GPU::TheException(std::string("Vertex shader: ") + infoLog);
	}
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		throw GPU::TheException(std::string("fragment shader: ") + infoLog);
	}


	//create and link both shader program
	GLuint shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	//cleaning
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//preparation of vao to link vbo
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//creating an id
	glGenBuffers(1, &vbo);
	//modifying the vbo
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//allocating, statiic draw means we won't change the buffer as movement will be managed by vertex shader with our mvp matrix
	glBufferData(GL_ARRAY_BUFFER, vboBuffer.size() * sizeof(t_glvertex), vboBuffer.data(), GL_STATIC_DRAW);

	GLsizei vertexSize = sizeof(t_glvertex);
	//configuring where is what
	/*
	 * 0 is the GLSL canal 
	 * 3 is the size (3 position x y z)
	 * GL_FLOAT is the type of the args
	 * GL_FALSE	is to not normalize values (lose diistance information);
	 * the size of the whole data
	 * where to start
	 */
	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (void *)0);
	glEnableVertexAttribArray(0);


	//colors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, (void *)offsetof(t_glvertex, r));
	glEnableVertexAttribArray(1);

	//normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vertexSize, (void *)offsetof(t_glvertex, nx));
	glEnableVertexAttribArray(2);

	//texture
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, vertexSize, (void *)offsetof(t_glvertex, tx));
	glEnableVertexAttribArray(3);

	GLint mvpLoc = glGetUniformLocation(shaderProgram, "mvp");
	GLint RotLoc = glGetUniformLocation(shaderProgram, "rotation");
	GLint kaLoc = glGetUniformLocation(shaderProgram, "ka");
	GLint kdLoc = glGetUniformLocation(shaderProgram, "kd");
	GLint ksLoc = glGetUniformLocation(shaderProgram, "ks");
	GLint nsLoc = glGetUniformLocation(shaderProgram, "ns");
	GLint dLoc = glGetUniformLocation(shaderProgram, "d");
	//end of config
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{ 	 
		glfwPollEvents();
		processInput();

		std::array<float, 16> scale = Matrix::getScale(mySettings.scale);
		std::array<float, 16> trans = Matrix::getTranslate(mySettings.translate[0], mySettings.translate[1], mySettings.translate[2]);
	
		std::array<float, 3> camRight = Matrix::normalize(Matrix::cross(mySettings.cameraFront, mySettings.up));
		std::array<float, 3> camUp = Matrix::normalize(Matrix::cross(camRight, mySettings.cameraFront));
		std::array<float, 16> view = Matrix::getLookAt(camUp, mySettings.cameraFront, camRight, mySettings.cameraPos);
		std::array<float, 16> proj = Matrix::getProjection(M_PI/4.0, 1920.0/1080.0 , 0.01f, 100.0f);


		std::array<float, 16> xRot = Matrix::getXRotation(mySettings.ObjRx * M_PI/180.0f);
		std::array<float, 16> yRot = Matrix::getYRotation(mySettings.ObjRy * M_PI/180.0f);
		std::array<float, 16> zRot = Matrix::getZRotation(mySettings.ObjRz * M_PI/180.0f);
		std::array<float, 16> rot = Matrix::multiply(Matrix::multiply(zRot, yRot), xRot);
		std::array<float, 16> model = Matrix::multiply(trans,Matrix::multiply(rot, scale));
		std::array<float, 16> mvp = Matrix::multiply(proj,Matrix::multiply(view, model));
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaderProgram);
		
		// "send" mvp matrix to the vertex shader
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, mvp.data());
		glUniformMatrix4fv(RotLoc, 1, GL_FALSE, rot.data());
		glBindVertexArray(vao);
		//we do "grouped" mtl draw so we dont send every single vertice with the mtl in "in" in vertexShader 
		//but all in group directly into fragmentShader, for optimisation
		size_t previousIndexes = 0;
		for (std::vector<size_t>::iterator it = myObj.indexesOfMtlSwicth.begin(); it != myObj.indexesOfMtlSwicth.end(); ++it)
		{	
			glUniform3fv(kdLoc, 1, vboBuffer[previousIndexes].kd.data());
			glUniform3fv(ksLoc, 1, vboBuffer[previousIndexes].ks.data());
			glUniform3fv(kaLoc, 1, vboBuffer[previousIndexes].ka.data());
			glUniform1f(nsLoc, vboBuffer[previousIndexes].ns);
			glUniform1f(dLoc, vboBuffer[previousIndexes].d);
			glDrawArrays(GL_TRIANGLES, previousIndexes, *it);
			previousIndexes += *it;
		}
		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glUseProgram(0);
	}

}
void	GPU::processInput()
{
	const float camSpeed = 0.05f;
	const float sens	 = 0.5f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		mySettings.rx += sens;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		mySettings.rx -= sens;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		mySettings.ry -= sens;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		mySettings.ry += sens;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		mySettings.scale += 0.1f;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		mySettings.scale -= 0.1f;

	if (mySettings.rx > 89.0f)
		mySettings.rx = 89.0f;
	if (mySettings.rx < -89.0f)
		mySettings.rx = -89.0f;
	if (mySettings.scale < 0.1f)
		mySettings.scale = 0.1f;
	if (mySettings.scale > 2000.0f)
		mySettings.scale = 2000.0f;
	float rx = mySettings.rx * M_PI / 180.0f;
	float ry = mySettings.ry * M_PI / 180.0f;
	mySettings.cameraFront[0] = std::cos(rx) * std::cos(ry);
	mySettings.cameraFront[1] = std::sin(rx);
	mySettings.cameraFront[2] = std::sin(ry) * std::cos(rx);
	mySettings.cameraFront = Matrix::normalize(mySettings.cameraFront);

	std::array<float, 3> mvForward = Matrix::multiply(mySettings.cameraFront, camSpeed);
	std::array<float, 3> camRight = Matrix::normalize(Matrix::cross(mySettings.cameraFront, mySettings.up));
	std::array<float, 3> mvRight = Matrix::multiply(camRight, camSpeed);
	std::array<float, 3> mvUp = Matrix::multiply(mySettings.up, camSpeed);
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		mySettings.cameraPos = Matrix::add(mySettings.cameraPos, mvForward);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		mySettings.cameraPos = Matrix::sub(mySettings.cameraPos, mvForward);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		mySettings.cameraPos = Matrix::sub(mySettings.cameraPos, mvRight);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		mySettings.cameraPos = Matrix::add(mySettings.cameraPos, mvRight);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		mySettings.cameraPos = Matrix::add(mySettings.cameraPos, mvUp);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		mySettings.cameraPos = Matrix::sub(mySettings.cameraPos, mvUp);
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		mySettings.ObjRx -= sens;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		mySettings.ObjRx += sens;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		mySettings.ObjRy -= sens;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		mySettings.ObjRy += sens;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		mySettings.ObjRz += sens;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		mySettings.ObjRz -= sens;

}

void GPU::loadShaderCodes() 
{
	std::ifstream fileFs("srcs/shaders/fragmentShader.frag");
	if (!fileFs.is_open())
		throw GPU::TheException("Couldn't load fragment shader file");
	std::stringstream bufferFs;
	bufferFs << fileFs.rdbuf();
	this->fragmentShader = bufferFs.str();
	std::ifstream fileVs("srcs/shaders/vertexShader.vert");
	if (!fileVs.is_open())
		throw GPU::TheException("Couldn't load vertex shader file");
	std::stringstream bufferVs;
	bufferVs << fileVs.rdbuf();
	this->vertexShader = bufferVs.str();
}

GPU::TheException::~TheException() throw(){}

GPU::TheException::TheException(std::string msg)
{
	this->message = msg;
}
const char *GPU::TheException::what() const throw()
{
	return this->message.c_str();
}
