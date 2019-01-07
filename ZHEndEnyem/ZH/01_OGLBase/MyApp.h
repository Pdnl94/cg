#pragma once

// C++ includes
#include <memory>

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "gCamera.h"

#include "ProgramObject.h"
#include "BufferObject.h"
#include "VertexArrayObject.h"
#include "TextureObject.h"
#include "Mesh_OGL3.h"

class CMyApp
{
public:
	CMyApp(void);
	~CMyApp(void);

	bool Init();
	void Clean();

	void Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	void Resize(int, int);
protected:
	// segédeljárások
	void TextureFromFileAttach(const char* filename, GLuint role) const;


	//Parametric generator functions
	glm::vec3 GetSpherePos(float u, float v);
	glm::vec3 GetSphereNorm(float u, float v);
	glm::vec3 GetCylinderPos(float u, float v);
	glm::vec3 GetCylinderNorm(float u, float v);
	glm::vec2 GetTex(float u, float v);

	//Init functions
	void GetSphereData(
		std::vector<glm::vec3> &pos,
		std::vector<glm::vec3> &norm,
		std::vector<glm::vec2> &tex,
		std::vector<int> & ind);

	void GetCylinderData(
		std::vector<glm::vec3> &pos,
		std::vector<glm::vec3> &norm,
		std::vector<glm::vec2> &tex,
		std::vector<int> & ind);

	void GetCubeData(std::vector<glm::vec3> &pos, std::vector<int> & ind);
	

	//Draw functions

	void drawSkyBox();

	void drawTrunk(  const glm::mat4 & transform);
	void drawLeaves(const glm::mat4 & transform);
	void drawTree(const glm::mat4 & transform);

	std::vector<glm::vec3> tree_positions;

	std::unique_ptr<Mesh> cat;
	glm::vec3 cat_pos = { 0, 0, 0 }, cat_dir = { 0, 0, 1 };
	float cat_speed = 1, cat_rotation = 0, cat_size = 1;


	// shaderekhez szükséges változók
	ProgramObject		m_program;			// shaderek programja
	ProgramObject		m_programSkybox;	// skybox shaderek

	VertexArrayObject	m_vao;				// VAO objektum
	IndexBuffer			m_gpuBufferIndices;	// indexek
	ArrayBuffer			m_gpuBufferPos;		// pozíciók tömbje

	VertexArrayObject	square_vao;
	ArrayBuffer			square_vbo;

	VertexArrayObject	sphere_vao;
	ArrayBuffer			sphere_vbo_pos;
	ArrayBuffer			sphere_vbo_norm;
	ArrayBuffer			sphere_vbo_tex;
	IndexBuffer			sphere_ind;

	VertexArrayObject	cylinder_vao;
	ArrayBuffer			cylinder_vbo_pos;
	ArrayBuffer			cylinder_vbo_norm;
	ArrayBuffer			cylinder_vbo_tex;
	IndexBuffer			cylinder_ind;

	Texture2D treeTex;
	Texture2D leafTex;

	gCamera				m_camera;

	int N = 40;
	int M = 40;

	// nyers OGL azonosítók
	GLuint				m_skyboxTexture;
};

