/**********************************************************************************
* \file Graphics.h
* \brief File contains the function definitions for all functions that are used
		 to provide graphics.
* \author Willy Lau, 100% Code Contribution
*
* \copyright Copyright (C) 2022 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
//#include "../Window/Window.h"
#include "../Window/Window/Window.h"
//#include <GL/glew.h> // for access to OpenGL API declarations 
#include <GLFW/glfw3.h>
#include "VideoReader/video_reader.h"

#include "../GLshader/glslshader.h"
#include "../GLmodel/GLmodel.h"
#include "../texture.h"
#include "../Viewport.h"


#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <list>
#include <assert.h>

#include "Camera/CameraCom.h"
#include "../Graphics/TransformCom.h"
#include "../Graphics/LightCom.h"
#include "../Graphics/MaterialCom.h"
#include "Physics/RigidBodyComponent.h"
#include "../Graphics/SpriteCom.h"
#include "UserIntered/UICom.h"

#include <glm/glm.hpp>
#include<glm/common.hpp>
struct MaterialCom;
class Animator;

class Graphics
{
public:

	using model_iterator = std::map<std::string, GLModel>::iterator;
	using shader_iterator = std::map<std::string, GLSLShader>::iterator;
	using tex_iterator = std::map<std::string, Texture>::iterator;

	using render_pair = std::pair<std::shared_ptr<TransformCom>, std::shared_ptr<MaterialCom>>;
	using renderUI_pair = std::pair<TransformCom*, SpriteCom*>;


	std::map<std::string, Animator> _animationInfo{};

	//Insert Shaders/Tex/Model
	void Insert_boxModel();
	void Insert_fbx();
	void Insert_plane();
	void insert_wireframe_box_model();
	void insert_shdrpgms(std::string shdr_pgm_name, std::string vtx_shdr_name, std::string frg_shdr_name);
	void insert_shdrpgms(std::string shdr_pgm_name, std::string vtx_shdr_name, std::string frg_shdr_name, std::string geom_shdr_name);

	Graphics();
	Graphics(Viewport vp);
	Graphics(Viewport vp, GLFWwindow* windowptr);

	void resize_viewport(int w, int h);
	void resize_framebuffer(int w, int h);

	Viewport getViewport();

	virtual void init();
	virtual void exit();

	//RENDERING FUNCTIONS

	//Function is used for mesh wireframe drawing
	void update(float dt, const CameraCom&, const std::vector<render_pair>&);

	//Function used for rendering the AABB of every collision box within the scene
	void update_AABB(float dt, const CameraCom&, const std::vector<std::shared_ptr<iRigid>>&, unsigned);

	//The default draw function called for rendering everything in the scene
	void update_v4(float dt, const CameraCom&, const std::vector<render_pair>&,
		const std::vector<std::shared_ptr<LightCom>>&);

	void update_v5(float delta_time);

	void initVideoTexture();

	//Function for rendering the skybox
	void render_skybox(const CameraCom& cam);

	//Function used in update_AABB// used for collision box wireframe drawing
	void render_AABB(const std::shared_ptr<iRigid>& obj, GLSLShader& shdr, GLModel& mdl);

	//Function used in update// used for mesh wireframe drawing
	void render_wireframe(const render_pair& obj, const CameraCom& m_cam);

	void Init_framebuffer();
	GLuint getFrameBuffer() const;
	GLuint getColorBuffer() const;
	GLuint getRedBuffer() const;
	GLuint getVideoBuffer() const;
	bool RenderToFrameBuffer();
	void RenderToFrameBuffer(bool flag);
	void render_frameBuffer();
	//for debug to render the lights
	bool RenderLightbox() const;
	void RenderLightbox(bool flag);
	void RenderShadowMap(const std::vector<render_pair>& renderList, const CameraCom& cam);


	//parameter string is the name of the folder!
	//make sure the filenames are the same as the folder
	void ChangeSkybox(const std::string foldername);

	bool _render_lightbox;
	bool _pauseCutScene{ false };

	//Sprite setups
	model_iterator GetModelRefByKey(std::string key);
	shader_iterator GetShaderRefByKey(std::string key);
	tex_iterator GetTexRefByKey(std::string key);

	void SetWindow_Handle(GLFWwindow*);

	//change bg_clr
	void set_bgClr(const glm::vec3&);
	glm::vec3 get_bgClr();

	//add a alrdy loaded texture to the system
	void add_texture(const std::string& name, const GLuint& texID);
	//given a list of paths to the 6 cube map textures
	unsigned int load_cubeMap(const std::vector<std::string>& pathList);

	std::vector<std::string> GetModels();
	std::vector<std::string> GetShaders();
	std::vector<std::string> GetTextures();

	int readPixel(uint32_t attachmentIndex, int x, int y);

	//shadow relateds
	GLuint depthTex;
	GLuint shadowFBO;
	glm::vec3 lightLookAt = glm::vec3(0.0f, 0.0f, 1.0f);   // In world space.
	glm::vec3 lightUpVector = glm::vec3(0.0f, 1.0f, 0.0f); // In world space.
	float cameraNearPlane = 0.1f;
	float cameraFarPlane = 300.0f;
	const unsigned int depthMapResolution = 4096;
	std::vector<glm::mat4> lightMatrices;
	std::vector<float> shadowCascadeLevels{ cameraFarPlane / 50.0f, cameraFarPlane / 25.0f, cameraFarPlane / 10.0f, cameraFarPlane / 2.0f };
	std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& projview);
	std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view);
	glm::mat4 getLightSpaceMatrix(const CameraCom& cam,const float nearPlane, const float farPlane);
	std::vector<glm::mat4> getLightSpaceMatrices(const CameraCom& cam);

	//sprite related
	//Render all sprites in 2D space on the screen
	void draw_all_sprite(const std::vector<renderUI_pair>& render, const std::vector<std::pair<SpriteCom*, EntityID>> spriteList);
	//Render all the sprites
	void draw_sprite(const renderUI_pair& rPair, GLSLShader& shader);
	//Function for rendering the sprite in 3D space(using XYZ coords)
	void render_sprite3D(const CameraCom& cam, const std::vector<renderUI_pair>& renderList);
	//drawing sprites in 3D like a billboard
	void draw_sprite3D(const renderUI_pair& rPair, GLSLShader& shader);


	GLSLShader& get_shader(std::string name)
	{
		return _shaders[name];
	}

	//Uniform Buffer Objects
	enum BUFFERLOC
	{
		Matrices = 0,
		DirectionLight,
		PointLight,
		SpotLight,
		LightMatrices,
		NUMCOUNT
	};
	std::array<GLuint, BUFFERLOC::NUMCOUNT> _UBOs;
	unsigned int _uboMatrice;
	unsigned int _mat_block_id;
	void init_uniformBlockData();
	void sendBufferData(CameraCom& cam);
	void AddGammaValue(float val);
	float getGammaValue();
	void setGammaVal(float val); // for gamma white fade ins

	//set the bool to render minimap
	void toggleMinimap(bool val);
	bool toggleMinimap();

	//this is to set the toggle to not render text during cutscene (for now)
	void toggleCutscene(bool val);
	bool toggleCutscene();

private:
	float _defaultGamma = 0;

	//FrameBuffer
	const GLenum _buffers[2]
	{
		GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1
	};
	GLuint _redbuffer{};
	GLuint _framebuffer{};
	GLuint _colorbuffer{};
	unsigned int _rbo;
	//unsigned int rbo{};
	//Viewport currently set to window W and H.
	Viewport _viewport{};
	Viewport editor_viewport{};


	//MINIMAP STUFFS

	void RenderMiniMap(const std::vector<render_pair>& renderList, const CameraCom& cam);
	GLuint _minimapBuffer{};
	GLuint _minimapClr{};
	unsigned int _rbo2;
	bool _renderMinimap{ false };

	//playing cutscene boolean
	bool _playingCutscene{ false };

	//The position and scaling of the minimap on the screen
	glm::vec3 minimapPos{ -0.82f, 0.7f, 0.0f }; //the pos is from (-1,1) on both x,y dims
	glm::vec3 minimapScale{ 0.15f, 0.25f, 0.0f };


	//to change to mathlib vec3
	glm::vec3 _bgClr;

	//Container for Shaders
	std::map<std::string, GLSLShader> _shaders{};
	std::map<std::string, GLModel> _models{};
	std::map<std::string, Texture> _textures{};

	//Container for data
	//First is name, second is filename
	//std::vector< std::pair<std::string, std::string>> textureNameData;

	//handle for GLFWwindow
	GLFWwindow* _windowptr{};

	bool _renderToFrameBuffer{};
	void init_models();
	void init_shaders();
	void init_textures();
	void init_geom(const std::string& inPath);
	void init_skybox();
	void init_fboQuad();

	//function for rendering
	void update_Render(const render_pair& pair, GLSLShader& shdr); //currently the primary render function for 3Dobjects

	//this deletes all framebuffers
	void delete_framebuffers();

	Vector2 _viewportBounds[2];

	unsigned int _quadVAO;
	unsigned int _quadVBO;
	unsigned int _skyboxVAO;
	unsigned int _skyboxVBO;

	std::string startingSkybox = "cloud_1";
	std::string currSkybox;

	std::vector<std::string> skybox_prefix
	{
		"Resources/Textures/skybox_textures/",
		"Resources/Textures/skybox_textures/",
		"Resources/Textures/skybox_textures/",
		"Resources/Textures/skybox_textures/",
		"Resources/Textures/skybox_textures/",
		"Resources/Textures/skybox_textures/"
	};
	std::vector<std::string> skybox_postfix
	{
		"_right.dds",
		"_left.dds",
		"_top.dds",
		"_bottom.dds",
		"_front.dds",
		"_back.dds"
	};

#define NR_MAX_LIGHTS 8
#define MAX_DIR_LIGHTS 1

	void Send_dirLight(const int&, GLSLShader&, LightCom::DirLight&);
	void Send_pointLight(const int&, GLSLShader&, LightCom::PointLight&);
	void Send_spotLight(const int&, GLSLShader&, LightCom::SpotLight&);

// OPENGL DEBUG
	void gl_checkerror(const char* function, const char* file, int line);
//macro for opengl debug
#define GCE gl_checkerror(__FUNCTION__, __FILE__, __LINE__);

	// FOR VIDEO READER STUFF

	static int check_align(size_t align)
	{
		for (size_t i = sizeof(void*); i != 0; i *= 2)
			if (align == i)
				return 0;
		return EINVAL;
	}

	int posix_memalign(void** ptr, size_t align, size_t size)
	{
		if (check_align(align))
			return EINVAL;

		int saved_errno = errno;
		void* p = _aligned_malloc(size, align);
		if (p == NULL)
		{
			errno = saved_errno;
			return ENOMEM;
		}

		*ptr = p;
		return 0;
	}

public:
	void openVideo();
	bool videoplaying = false;
	bool videoStart = false;
	VideoReaderState vr_state{};
	GLuint video_handle;
	uint8_t* frame_data{ nullptr };
	double videoEndTime = 15.0;
	std::string videoName{};
	double staringTime{ 0.0 };

	bool skipOnce;
	float timer;

	bool showMinimapAfter = true;
};
