#include "MyApp.h"

#include <math.h>
#include <vector>

#include <array>
#include <list>
#include <tuple>
#include <imgui/imgui.h>

#include <random>

#include "ObjParser_OGL3.h"

CMyApp::CMyApp(void)
{
	m_camera.SetView(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}


CMyApp::~CMyApp(void)
{
	std::cout << "dtor!\n";
}

glm::vec3 CMyApp::GetSpherePos(float u, float v)
{
	u *= 2 * 3.1415f;
	v *= 3.1415f;
	float cu = cosf(u), su = sinf(u), cv = cosf(v), sv = sinf(v);
	float r = 1;

	return glm::vec3(r*cu*sv, r*cv, r*su*sv);
}
//
// egy parametrikus felület (u,v) paraméterértékekhez tartozó normálvektorának
// kiszámítását végzõ függvény
//
glm::vec3 CMyApp::GetSphereNorm(float u, float v)
{
	u *= 2 * 3.1415f;
	v *= 3.1415f;
	float cu = cosf(u), su = sinf(u), cv = cosf(v), sv = sinf(v);
	return glm::vec3(cu*sv, cv, su*sv);

	// Numerikusan (nem kell ismerni a képletet, elég a pozícióét)
	/*glm::vec3 uv = GetPos(u, v);
	glm::vec3 du = GetPos(u+0.01, v)-GetPos(u-0.01, v);
	glm::vec3 dv = GetPos(u, v+0.01)-GetPos(u, v-0.01);

	return (glm::cross(du, dv));*/
}

glm::vec3 CMyApp::GetCylinderPos(float u, float v)
{
	u *= -2 * 3.1415f;

	float cu = cosf(u), su = sinf(u), cv = cosf(v), sv = sinf(v);
	float r = 1;

	return glm::vec3(r*cu, v, r*su);
}

glm::vec3 CMyApp::GetCylinderNorm(float u, float v)
{
	u *= 2 * 3.1415f;
	float cu = cosf(u), su = sinf(u), cv = cosf(v), sv = sinf(v);
	return glm::vec3(cu, 0, su);
}

glm::vec2 CMyApp::GetTex(float u, float v)
{
	return glm::vec2(u, 1 - v);
}

void CMyApp::GetSphereData(
	std::vector<glm::vec3> &pos,
	std::vector<glm::vec3> &norm,
	std::vector<glm::vec2> &tex,
	std::vector<int> & ind)
{
	pos.resize((N + 1) * (M + 1));
	norm.resize((N + 1) * (M + 1));
	tex.resize((N + 1) * (M + 1));
	ind.resize(3 * 2 * N * M);

	for (int i = 0; i <= N; ++i)
		for (int j = 0; j <= M; ++j)
		{
			float u = i / (float)N;
			float v = j / (float)M;

			pos[i + j * (N + 1)] = GetSpherePos(u, v);
			norm[i + j * (N + 1)] = GetSphereNorm(u, v);
			tex[i + j * (N + 1)] = GetTex(u, v);
		}

	for (int i = 0; i < N; ++i)
		for (int j = 0; j < M; ++j)
		{
			ind[6 * i + j * 3 * 2 * (N)+0] = (i)+(j)*	(N + 1);
			ind[6 * i + j * 3 * 2 * (N)+1] = (i + 1) + (j)*	(N + 1);
			ind[6 * i + j * 3 * 2 * (N)+2] = (i)+(j + 1)*(N + 1);
			ind[6 * i + j * 3 * 2 * (N)+3] = (i + 1) + (j)*	(N + 1);
			ind[6 * i + j * 3 * 2 * (N)+4] = (i + 1) + (j + 1)*(N + 1);
			ind[6 * i + j * 3 * 2 * (N)+5] = (i)+(j + 1)*(N + 1);
		}
}

void CMyApp::GetCylinderData(
	std::vector<glm::vec3> &pos,
	std::vector<glm::vec3> &norm,
	std::vector<glm::vec2> &tex,
	std::vector<int> & ind)
{
	pos.resize((N + 1) * (M + 1));
	norm.resize((N + 1) * (M + 1));
	tex.resize((N + 1) * (M + 1));
	ind.resize(3 * 2 * N * M);

	for (int i = 0; i <= N; ++i)
		for (int j = 0; j <= M; ++j)
		{
			float u = i / (float)N;
			float v = j / (float)M;

			pos[i + j * (N + 1)] = GetCylinderPos(u, v);
			norm[i + j * (N + 1)] = GetCylinderNorm(u, v);
			tex[i + j * (N + 1)] = GetTex(u, v);
		}

	for (int i = 0; i < N; ++i)
		for (int j = 0; j < M; ++j)
		{
			ind[6 * i + j * 3 * 2 * (N)+0] = (i)+(j)*	(N + 1);
			ind[6 * i + j * 3 * 2 * (N)+1] = (i + 1) + (j)*	(N + 1);
			ind[6 * i + j * 3 * 2 * (N)+2] = (i)+(j + 1)*(N + 1);
			ind[6 * i + j * 3 * 2 * (N)+3] = (i + 1) + (j)*	(N + 1);
			ind[6 * i + j * 3 * 2 * (N)+4] = (i + 1) + (j + 1)*(N + 1);
			ind[6 * i + j * 3 * 2 * (N)+5] = (i)+(j + 1)*(N + 1);
		}
}

void CMyApp::GetCubeData(std::vector<glm::vec3>& pos, std::vector<int>& ind)
{
	pos = {
		glm::vec3(-1, -1, -1),
		glm::vec3(1, -1, -1),
		glm::vec3(1, 1, -1),
		glm::vec3(-1, 1, -1),
		// elülsõ lap
		glm::vec3(-1, -1, 1),
		glm::vec3(1, -1, 1),
		glm::vec3(1, 1, 1),
		glm::vec3(-1, 1, 1)
	};

	ind = {
		// hátsó lap
		0, 1, 2,
		2, 3, 0,
		// elülsõ lap
		4, 6, 5,
		6, 4, 7,
		// bal
		0, 3, 4,
		4, 3, 7,
		// jobb
		1, 5, 2,
		5, 6, 2,
		// alsó
		1, 0, 4,
		1, 4, 5,
		// felsõ
		3, 2, 6,
		3, 6, 7,
	};
}

bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)

	//
	// shaderek betöltése
	//

	// a shadereket tároló program létrehozása az OpenGL-hez hasonló módon:
	m_program.AttachShaders({
		{ GL_VERTEX_SHADER, "tex.vert"},
		{ GL_FRAGMENT_SHADER, "tex.frag"}
	});

	// attributomok osszerendelese a VAO es shader kozt
	m_program.BindAttribLocations({
		{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		{ 1, "vs_in_col" }				// VAO 1-es csatorna menjen a vs_in_col-ba
	});

	m_program.LinkProgram();

	// shader program rövid létrehozása, egyetlen függvényhívással a fenti három:
	m_programSkybox.Init(
	{
		{ GL_VERTEX_SHADER, "skybox.vert" },
		{ GL_FRAGMENT_SHADER, "skybox.frag" }
	},
	{
		{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
	}
	);


	std::vector<glm::vec3> cube_pos;
	std::vector<int>       cube_ind;

	GetCubeData(cube_pos, cube_ind);

	m_gpuBufferPos.BufferData(    cube_pos);
	m_gpuBufferIndices.BufferData(cube_ind);

	m_vao.Init(
	{
		{ CreateAttribute<0, glm::vec3>, m_gpuBufferPos },		
	},
	m_gpuBufferIndices
	);


	std::vector<glm::vec3> square_pos{
		glm::vec3(-1, -1, 0),
		glm::vec3(1, -1, 0),
		glm::vec3(1, 1, 0),
		glm::vec3(-1, 1, 0),
	};

	square_vbo.BufferData(square_pos);

	square_vao.Init(
		{
			{ CreateAttribute<0, glm::vec3>, square_vbo }
		}
	);


	std::vector<glm::vec3> sphere_pos;
	std::vector<glm::vec3> sphere_norm;
	std::vector<glm::vec2> sphere_tex;
	std::vector<int>       sphere_indices;

	GetSphereData(
		sphere_pos,
		sphere_norm,
		sphere_tex,
		sphere_indices);

	sphere_vbo_pos.BufferData(sphere_pos);
	sphere_vbo_norm.BufferData(sphere_norm);
	sphere_vbo_tex.BufferData(sphere_tex);
	sphere_ind.BufferData(sphere_indices);

	sphere_vao.Init(
		{
			{ CreateAttribute<0, glm::vec3>, sphere_vbo_pos },
			{ CreateAttribute<1, glm::vec3>, sphere_vbo_norm },
			{ CreateAttribute<2, glm::vec2>, sphere_vbo_tex }
		},
		sphere_ind
	);


	std::vector<glm::vec3> cylinder_pos;
	std::vector<glm::vec3> cylinder_norm;
	std::vector<glm::vec2> cylinder_tex;
	std::vector<int>       cylinder_indices;

	GetCylinderData(
		cylinder_pos,
		cylinder_norm,
		cylinder_tex,
		cylinder_indices);

	cylinder_vbo_pos.BufferData( cylinder_pos);
	cylinder_vbo_norm.BufferData(cylinder_norm);
	cylinder_vbo_tex.BufferData( cylinder_tex);
	cylinder_ind.BufferData(     cylinder_indices);

	cylinder_vao.Init(
		{
			{ CreateAttribute<0, glm::vec3>, cylinder_vbo_pos },
			{ CreateAttribute<1, glm::vec3>, cylinder_vbo_norm },
			{ CreateAttribute<2, glm::vec2>, cylinder_vbo_tex }
		},
		cylinder_ind
	);

	treeTex.FromFile("tree.jpg");
	leafTex.FromFile("leaves.jpg");


	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> rnd_double(0, 2 * M_PI);
	std::uniform_int_distribution<>  rnd_int(10, 50);

	// CPU oldali részecsketömbök feltöltése
	for (int i = 0; i < 200; ++i)
	{
		float tmpR = rnd_int(gen);
		float tmpAngle = rnd_double(gen);
		tree_positions.push_back(
			glm::vec3(tmpR * cosf(tmpAngle), 0, tmpR * sinf(tmpAngle)));
	}

	cat = ObjParser::parse("cat.obj");


	// skybox
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glGenTextures(1, &m_skyboxTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	TextureFromFileAttach("xpos.png", GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	TextureFromFileAttach("xneg.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	TextureFromFileAttach("ypos.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	TextureFromFileAttach("yneg.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	TextureFromFileAttach("zpos.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	TextureFromFileAttach("zneg.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	// kamera
	m_camera.SetProj(45.0f, 640.0f / 480.0f, 0.01f, 1000.0f);

	return true;
}

void CMyApp::Clean()
{
	glDeleteTextures(1, &m_skyboxTexture);
}

void CMyApp::Update()
{
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;

	cat_pos += delta_time * cat_dir * cat_speed;

	for (const auto & tree_pos : tree_positions)
	{
		if (glm::length(tree_pos - cat_pos) < 1)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> rnd_double(-M_PI / 2.0, M_PI / 2.0);

			float angle = rnd_double(gen);
			
			cat_dir *= -1;
			cat_pos += 2.0f * cat_dir;
			cat_rotation += M_PI + angle;

		}
	}

	m_camera.Update(delta_time);

	last_time = SDL_GetTicks();
}

void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (const auto & pos : tree_positions)
	{
		drawTree(glm::translate(pos));
	}

	glm::mat4 transform = 
		glm::translate(cat_pos) *
		glm::rotate(cat_rotation, glm::vec3(0, 1, 0));
	m_program.SetUniform("world", transform);
	m_program.SetUniform("worldIT", glm::inverse(glm::transpose(transform)));
	m_program.SetUniform("MVP", m_camera.GetViewProj() * transform);
	m_program.SetTexture("texImage", 0, treeTex);
	cat->draw();

	if (ImGui::Begin("Macska"))
	{
		ImGui::SliderFloat("Sebesség", &cat_speed, 1, 10);
		ImGui::SliderFloat("Méret", &cat_size, 1, 10);
	}
	ImGui::End();

	/********/
	// skybox
	/********/

	drawSkyBox();
	
	//ImGui Testwindow
	ImGui::ShowTestWindow();
}

void CMyApp::drawTrunk(const glm::mat4 & transform)
{
	m_program.Use();
	cylinder_vao.Bind();

	m_program.SetUniform("world", transform);
	m_program.SetUniform("worldIT", glm::inverse(glm::transpose(transform)));
	m_program.SetUniform("MVP", m_camera.GetViewProj() * transform);
	m_program.SetTexture("texImage", 0, treeTex);
	glDrawElements(GL_TRIANGLES, 3 * 2 * N*M, GL_UNSIGNED_INT, nullptr);
}

void CMyApp::drawLeaves(const glm::mat4 & transform)
{
	m_program.Use();
	sphere_vao.Bind();

	m_program.SetUniform("world", transform);
	m_program.SetUniform("worldIT", transform);
	m_program.SetUniform("MVP", m_camera.GetViewProj() * transform);
	m_program.SetTexture("texImage", 0, leafTex);
	glDrawElements(GL_TRIANGLES, 3 * 2 * N*M, GL_UNSIGNED_INT, nullptr);
}

void CMyApp::drawTree(const glm::mat4 & transform)
{
	drawTrunk(transform * glm::scale(glm::vec3(0.5, 5, 0.5)));

	drawLeaves(
		transform * 
		glm::translate(glm::vec3(0, 6.8, 0)) *
		glm::scale(glm::vec3(2))
	);
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardDown(key);
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardUp(key);
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	m_camera.MouseMove(mouse);
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

// a két paraméterbe az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h );

	m_camera.Resize(_w, _h);
}

void CMyApp::drawSkyBox()
{
	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);

	// most kisebb-egyenlõt használjunk, mert mindent kitolunk a távoli vágósíkokra
	glDepthFunc(GL_LEQUAL);

	m_vao.Bind();

	m_programSkybox.Use();
	m_programSkybox.SetUniform("MVP", m_camera.GetViewProj() * glm::translate(m_camera.GetEye()));

	// cube map textúra beállítása 0-ás mintavételezõre és annak a shaderre beállítása
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);
	glUniform1i(m_programSkybox.GetLocation("skyboxTexture"), 0);

	// az elõzõ három sor <=> m_programSkybox.SetCubeTexture("skyboxTexture", 0, m_skyboxTexture);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

	// végül állítsuk vissza
	glDepthFunc(prevDepthFnc);
}

void CMyApp::TextureFromFileAttach(const char* filename, GLuint role) const
{
	SDL_Surface* loaded_img = IMG_Load(filename);

	int img_mode = 0;

	if (loaded_img == 0)
	{
		std::cout << "[TextureFromFile] Hiba a kép betöltése közben: " << filename << std::endl;
		return;
	}

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	if (loaded_img->format->BytesPerPixel == 4)
		img_mode = GL_BGRA;
	else
		img_mode = GL_BGR;
#else
	if (loaded_img->format->BytesPerPixel == 4)
		img_mode = GL_RGBA;
	else
		img_mode = GL_RGB;
#endif

	glTexImage2D(role, 0, GL_RGBA, loaded_img->w, loaded_img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, loaded_img->pixels);

	SDL_FreeSurface(loaded_img);
}
