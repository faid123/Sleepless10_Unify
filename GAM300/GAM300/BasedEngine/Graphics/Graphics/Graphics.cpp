/**********************************************************************************
* \file Graphics.cpp
* \brief File contains the function definitions for all functions that are used
		 to provide graphics.
* \author Willy Lau, 100% Code Contribution
*
* \copyright Copyright (C) 2022 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Graphics.h"
#include "../Window/Window.h"
#include "Input.h"
//#include "Editor/EditorSystem/EditorUI.h"
//
////Need to define if you want to include stb_image.h
//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>

#include <iostream>
#include <array>
#include <random>
#include <filesystem>

#include <glm/gtx/string_cast.hpp>
#include<glm/gtx/quaternion.hpp>
#include<glm/gtc/quaternion.hpp>
#include <Graphics/DDSLoader/ModelLoader.h>
#include <Editor/EditorSystem/EditorUI.h>
#include "MathLib/Precompiled.h"
#include "Input.h"
#include "Animation/AnimationSystem.h"
#include "../FreeType/text.h"



//#include "renderdoc_app.h"
//RENDERDOC_API_1_1_2* rdoc_api = NULL;

namespace fs = std::filesystem;


Graphics::Graphics() :
	_viewport{ 1280, 720 }
{}
Graphics::Graphics(Viewport vp) :
	_viewport{ vp },
	_framebuffer{ 0 }
{
}

Graphics::Graphics(Viewport vp, GLFWwindow* windowptr) :
	_viewport{ vp },
	_framebuffer{ 0 }

{
	(void*)(windowptr);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glViewport(0, 0, _viewport.w, _viewport.h);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


#pragma region Models

void Graphics::Insert_plane()
{
	GLModel mdl;
	mdl.init_plane();
	_models.emplace("plane", mdl);

}
void Graphics::Insert_boxModel()
{
	GLModel mdl;

	mdl.init_cube();

	_models.emplace("default", mdl);
}

void Graphics::init_geom(const std::string& inPath)
{
	GLModel mdl;
	if (inPath.find("Bone") != std::string::npos || (inPath.find("bone") != std::string::npos))
	{
		mdl.init_fbxAnim(inPath);
		std::string path = "Resources/Models/" + mdl.model_name + ".fbx";
		Animator tmp(path, "none", mdl.model_name, 7.0f);
		_animationInfo[mdl.model_name] = tmp;
	}
	else
		mdl.init_geom(inPath);

	_models.emplace(mdl.model_name, mdl);
}

void Graphics::insert_wireframe_box_model()
{
	//doesnt work/render :( will come back to bug fix in future
	GLModel mdl;
	mdl.init_wireframe();

	_models.emplace("wireframe_box", mdl);
}

void Graphics::Insert_fbx()
{
	GLModel mdl;

	mdl.init_fbx();
	_models.emplace("unicorn", mdl);

	/*GLModel mdl2;
	mdl2.init_fbxAnim();
	_models.emplace("unicorn_bone", mdl2);*/
}

#pragma endregion  

#pragma region Init

void Graphics::init_models()
{
	//std::cout << "Current path" << fs::current_path() << std::endl;
	const std::string inDir{ "Resources/Geometry" };
	if (fs::exists(inDir))
		for (auto const& dir_entry : std::filesystem::directory_iterator{ inDir })
			init_geom(dir_entry.path().string());
	else
		std::cout << "MeshLoad::No geometry found!" << std::endl;

	//Insert_fbx();
	Insert_boxModel();
	Insert_plane();
	//insert_wireframe_box_model();
}

void Graphics::insert_shdrpgms(std::string shdr_pgm_name, std::string vtx_shdr_name, std::string frg_shdr_name
	, std::string geom_shdr_name)
{
	std::vector<std::pair<GLenum, std::string>> shdr_files{
	std::make_pair(GL_VERTEX_SHADER, vtx_shdr_name),
	std::make_pair(GL_FRAGMENT_SHADER, frg_shdr_name),
	std::make_pair(GL_GEOMETRY_SHADER, geom_shdr_name) };

	GLSLShader shdr_pgm;
	shdr_pgm.CompileLinkValidate(shdr_files);
	if (GL_FALSE == shdr_pgm.IsLinked())
	{
		//Logger::LoggerError("Unable to compile / link / validate shader programs\n" + vtx_shdr_name + " " + frg_shdr_name + shdr_pgm.GetLog());
		std::cerr << "Unable to compile/link/validate shader programs\n";
		std::cerr << vtx_shdr_name + " " + frg_shdr_name << std::endl;
		std::cerr << shdr_pgm.GetLog() << "\n";
		assert(EXIT_FAILURE);
		std::exit(EXIT_FAILURE);
	}

	// add compiled, linked, and validated shader program to
	_shaders[shdr_pgm_name] = shdr_pgm;

}
void Graphics::insert_shdrpgms(std::string shdr_pgm_name, std::string vtx_shdr_name, std::string frg_shdr_name)
{
	std::vector<std::pair<GLenum, std::string>> shdr_files{
		std::make_pair(GL_VERTEX_SHADER, vtx_shdr_name),
		std::make_pair(GL_FRAGMENT_SHADER, frg_shdr_name) };

	GLSLShader shdr_pgm;
	shdr_pgm.CompileLinkValidate(shdr_files);
	if (GL_FALSE == shdr_pgm.IsLinked())
	{
		//Logger::LoggerError("Unable to compile / link / validate shader programs\n" + vtx_shdr_name + " " + frg_shdr_name + shdr_pgm.GetLog());
		std::cerr << "Unable to compile/link/validate shader programs\n";
		std::cerr << vtx_shdr_name + " " + frg_shdr_name << std::endl;
		std::cerr << shdr_pgm.GetLog() << "\n";
		assert(EXIT_FAILURE);
		std::exit(EXIT_FAILURE);
	}

	// add compiled, linked, and validated shader program to
	_shaders[shdr_pgm_name] = shdr_pgm;
}
void Graphics::init_shaders()
{
	//To be renamed from tutorial-5  to smth else
	/*! ../ brings you to file location where .sln is at */
	insert_shdrpgms("default_shader", "Resources/Shaders/vertexShader.vert", "Resources/Shaders/FragmentShader.frag");
	insert_shdrpgms("forward_shader", "Resources/Shaders/forward.vert", "Resources/Shaders/forward.frag");
	insert_shdrpgms("skybox_shader", "Resources/Shaders/skybox.vert", "Resources/Shaders/skybox.frag");
	insert_shdrpgms("framebuffer_screen", "Resources/Shaders/framebuffer_screen.vert", "Resources/Shaders/framebuffer_screen.frag");
	insert_shdrpgms("wireframe_shader", "Resources/Shaders/wireframe_Shader.vert", "Resources/Shaders/wireframe_Shader.frag");
	insert_shdrpgms("simple_minimap", "Resources/Shaders/simple_minimap.vert", "Resources/Shaders/simple_minimap.frag");
	insert_shdrpgms("skinning_shader", "Resources/Shaders/skinning.vert", "Resources/Shaders/skinning.frag");
	insert_shdrpgms("ui_shader", "Resources/Shaders/UIShader.vert", "Resources/Shaders/UIShader.frag");
	insert_shdrpgms("ui3D_shader", "Resources/Shaders/UI3DShader.vert", "Resources/Shaders/UI3DShader.frag");
	insert_shdrpgms("font_shader", "Resources/Shaders/FontShader.vert", "Resources/Shaders/FontShader.frag");
	GCE
		insert_shdrpgms("simple_shadowmap", "Resources/Shaders/simple_shadowmap.vert", "Resources/Shaders/simple_shadowmap.frag", "Resources/Shaders/simple_shadowmap.geom");
	GCE
		//insert_shdrpgms("tex_shader", "./Assets/Shaders/basic-tex.vert", "./Assets/Shaders/basic-tex.frag");
		//insert_shdrpgms("wireframe_shader", "./Assets/Shaders/wireframe-Shader.vert", "./Assets/Shaders/wireframe-Shader.frag");

}
void Graphics::init_fboQuad()
{
	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &_quadVAO);
	glGenBuffers(1, &_quadVBO);
	glBindVertexArray(_quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, _quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

}
void Graphics::update_Render(const render_pair& pair, GLSLShader& shdr)
{
	auto& mCom = pair.second;
	auto& tCom = pair.first;
	if (mCom->m_model.find("Bone") != std::string::npos)
	{
		auto anim = Engine::_ecs.getComponent<AnimationCom>(mCom->entityID);

		Vector3 s = tCom->scale;
		Vector3 tmp = tCom->pos;
		Vector3 convertedRotate = tCom->rotate * ((float)M_PI / 180.0f);

		convertedRotate.y += (float)M_PI;
		if (Engine::_isPlay)
		{
			if (mCom->m_model.find("BossArm1Bone") != std::string::npos
				|| mCom->m_model.find("BossArm2Bone") != std::string::npos
				|| mCom->m_model.find("Pistol") != std::string::npos
				|| mCom->m_model.find("Railgun") != std::string::npos
				|| mCom->m_model.find("Rocket_launcher") != std::string::npos)
			{
				tmp.y = tCom->pos.y;
			}
			else
				tmp.y -= static_cast<float>(s.y / 2 + 1.60);
		}

		glm::mat4 oScale(glm::scale(glm::identity<glm::mat4>(), s.Vec3toGlmVec3())),
			oTranslate(glm::translate(glm::identity<glm::mat4>(), tmp.Vec3toGlmVec3()));

		glm::mat4
			xRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.x,glm::vec3(1.0f,0.0f,0.0f)) },
			yRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.y,glm::vec3(0.0f,1.0f,0.0f)) },
			zRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.z,glm::vec3(0.0f,0.0f,1.0f)) };

		glm::mat4 oRotate = zRotate * yRotate * xRotate;// *yRotate* zRotate;
		glm::mat4 modelMatrix = oTranslate * oRotate * oScale;
		glm::mat4 normals = glm::mat4(glm::transpose(glm::inverse(modelMatrix)));

		if (!Engine::_ecs.getComponent<AnimationCom>(mCom->entityID))
		{
			Engine::_ecs.addComponentToEntity(mCom->entityID, AnimationCom());
			Animator anim_ = _animationInfo[mCom->m_model];
			Engine::_ecs.getSystem<AnimationSystem>()->allAnimators[mCom->entityID] = std::make_shared<Animator>(anim_);
		}

		auto animator = Engine::_ecs.getSystem<AnimationSystem>()->allAnimators[mCom->entityID];
		std::vector<glm::mat4> currentPose = animator->currentPose;
		shdr.SetUniform("model", modelMatrix);
		int jointMatrixLoc = glGetUniformLocation(shdr.GetHandle(), "bone_transforms");
		glUniformMatrix4fv(jointMatrixLoc, static_cast<GLsizei>(currentPose.size()), GL_FALSE, glm::value_ptr(currentPose[0]));
		//shdr.SetUniform("normals_matrix", normals);
		shdr.SetUniform("useBone", 1);
	}
	else
	{
		//Get obj's model to world transform
		glm::mat4 oScale(glm::scale(glm::identity<glm::mat4>(), tCom->scale.Vec3toGlmVec3())),
			oTranslate(glm::translate(glm::identity<glm::mat4>(), tCom->pos.Vec3toGlmVec3()));

		Vector3 convertedRotate = tCom->rotate * ((float)M_PI / 180.0f);
		convertedRotate.y += (float)M_PI; //For flipping and facing the right way "defaultly"

		glm::mat4
			xRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.x,glm::vec3(1.0f,0.0f,0.0f)) },
			yRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.y,glm::vec3(0.0f,1.0f,0.0f)) },
			zRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.z,glm::vec3(0.0f,0.0f,1.0f)) };

		glm::mat4 oRotate = zRotate * yRotate * xRotate;// *yRotate* zRotate;
		glm::mat4 mdl_xform = oTranslate * oRotate * oScale;

		shdr.SetUniform("model", mdl_xform);
		shdr.SetUniform("useBone", 0);
	}


	// Render OpenGL primitives encapsulated by this object's VAO
	// using glDrawElements or glDrawArrays  

	//GLSLShader& MDL_shdr = mCom->shader_ref->second;
	GLSLShader& MDL_shdr = shdr;
	//MDL_shdr.Use();
	//check texture
	if (mCom->tex_ref == _textures.end())
	{
		//use colour instead
		glm::vec3 clr = mCom->colour.Vec3toGlmVec3();
		MDL_shdr.SetUniform("vertex_Clr", clr);
		MDL_shdr.SetUniform("useTexture", 0);
	}
	else
	{
		MDL_shdr.SetUniform("useTexture", 1);

		// bind textures on corresponding texture units
		auto tex = mCom->tex_ref->second;

		int texLoc = glGetUniformLocation(MDL_shdr.GetHandle(), "uTex2d");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex.texhandle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glUniform1i(texLoc, 0);

		//gamma correction for _default models carrying sprites
		if (mCom->m_model == "default")
		{
			float newGamma = _defaultGamma - 0.825f;
			shdr.SetUniform("gammaCorrection", newGamma);
		}
		else
			shdr.SetUniform("gammaCorrection", _defaultGamma);
	}


	shdr.SetUniform("entityID", mCom->entityID);
	if (mCom->_alpha < -0.5f)
		mCom->_alpha = 1;
	else if (mCom->_alpha < 0)
		mCom->_alpha = 0;
	if (mCom->_alpha > 1)
		mCom->_alpha = 1;
	shdr.SetUniform("Alpha", mCom->_alpha);

	mCom->_saturation.y = glm::clamp(mCom->_saturation.y, 0.0f, 1.0f);
	glm::vec3 saturation = mCom->_saturation.Vec3toGlmVec3();
	shdr.SetUniform("saturation", saturation);

	GLModel& model = mCom->model_ref->second;
	glBindVertexArray(model.vaoid);

	glDrawElements(model.primitive_type, model.draw_cnt,
		GL_UNSIGNED_SHORT, NULL);


	glBindVertexArray(0);

}
void Graphics::delete_framebuffers()
{
	//delete render framebuffer
	glDeleteTextures(1, &_colorbuffer);
	glDeleteTextures(1, &_redbuffer);
	glDeleteRenderbuffers(1, &_rbo);
	glDeleteFramebuffers(1, &_framebuffer);



	//delete shadows
	glDeleteTextures(1, &depthTex);
	glDeleteFramebuffers(1, &shadowFBO);

	//delete minimaps
	glDeleteTextures(1, &_minimapClr);
	glDeleteRenderbuffers(1, &_rbo2);
	glDeleteFramebuffers(1, &_minimapBuffer);
}
void Graphics::init_skybox()
{
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	// skybox VAO
	glGenVertexArrays(1, &_skyboxVAO);
	glGenBuffers(1, &_skyboxVBO);
	glBindVertexArray(_skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, _skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	ChangeSkybox(startingSkybox);
}


void Graphics::ChangeSkybox(const std::string filename)
{
	std::string textureName = filename + "_texture";
	//check if exists
	if (_textures.find(filename + "_texture") != _textures.end())
	{
		currSkybox = textureName;
		return;
	}

	// does not exist


	std::vector<std::string> faces;
	for (unsigned i = 0; i < skybox_prefix.size(); ++i)
		faces.push_back(skybox_prefix[i] + filename + "/" + filename + skybox_postfix[i]);


	unsigned cubeMapID = load_cubeMap(faces);
	add_texture(textureName, cubeMapID);
	currSkybox = textureName;
}

void Graphics::init_textures()
{
	//add_texture("unicorn", mdl.texId);
	//Grab all file paths to any graphic Jsons.
	//load_textures(jsonPath.string());

	//std::cout << "Current path" << fs::current_path() << std::endl;
	const std::string inDir{ "Resources/Textures" };
	if (fs::exists(inDir))
		for (auto const& dir_entry : std::filesystem::directory_iterator{ inDir })
		{
			//std::cout << dir_entry.path().extension() << std::endl;
			if (dir_entry.path().has_extension()) //using soil library to load in textures
				add_texture(dir_entry.path().stem().string(), modelLoader::mLoader::SOIL_LoadTextureDDS(dir_entry.path().string().c_str()));

		}

	const std::string inDir2{ "Resources/Textures/UI" };
	if (fs::exists(inDir2))
	{
		for (auto const& dir_entry : std::filesystem::directory_iterator{ inDir2 })
		{
			//std::cout << dir_entry.path().extension() << std::endl;
			if (dir_entry.path().has_extension())
				add_texture(dir_entry.path().stem().string(), modelLoader::mLoader::SOIL_LoadTextureDDS(dir_entry.path().string().c_str()));
			//LoadTexture(dir_entry.path().string().c_str(), dir_entry.path().stem().string());
		}
	}
	const std::string inDir3{ "Resources/Textures/Cutscene" };
	if (fs::exists(inDir3))
	{
		for (auto const& dir_entry : std::filesystem::directory_iterator{ inDir3 })
		{
			//std::cout << dir_entry.path().extension() << std::endl;
			if (dir_entry.path().has_extension())
				add_texture(dir_entry.path().stem().string(), modelLoader::mLoader::SOIL_LoadTextureDDS(dir_entry.path().string().c_str()));
			//LoadTexture(dir_entry.path().string().c_str(), dir_entry.path().stem().string());
		}
	}
}

void Graphics::resize_viewport(int w, int h)
{
	_viewport.w = w;
	_viewport.h = h;
	resize_framebuffer(w, h);
}
void Graphics::resize_framebuffer(int w, int h)
{
	(void)w;
	(void)h;

	delete_framebuffers();
	Init_framebuffer();

}
Viewport Graphics::getViewport()
{
	if (!_renderToFrameBuffer)//fullscreen -playmode-
		return _viewport;
	else
		return editor_viewport;
}
/*Initialize Framebuffer*/
void Graphics::Init_framebuffer()
{

	glGenFramebuffers(1, &_framebuffer);
	//Binds the frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

	//Create color attachment texture
	glGenTextures(1, &_colorbuffer);
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, _colorbuffer);
	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _viewport.w, _viewport.h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//bind it to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorbuffer, 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, _colorbuffer, 0);

	////Create color attachment texture
	//glGenTextures(1, &_redbuffer);
	//// "Bind" the newly created texture : all future texture functions will modify this texture
	//glBindTexture(GL_TEXTURE_2D, _redbuffer);
	//// Give an empty image to OpenGL ( the last "0" )
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _viewport.w, _viewport.h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	////bind it to framebuffer
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _redbuffer, 0);
	//glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, _redbuffer, 0);

	glGenTextures(1, &_redbuffer);
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, _redbuffer);
	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, _viewport.w, _viewport.h, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//bind it to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _redbuffer, 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, _redbuffer, 0);

	//create a renderbuffer object for depth and stencil attachments
	glGenRenderbuffers(1, &_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
	// use a single renderbuffer object for both a depth AND stencil buffer.
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _viewport.w, _viewport.h);
	//bind it to framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo);


	glGenFramebuffers(1, &shadowFBO);
	// create depth texture
	glGenTextures(1, &depthTex);
	glBindTexture(GL_TEXTURE_2D_ARRAY, depthTex);

	glTexImage3D(
		GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F,
		depthMapResolution, depthMapResolution,
		//_viewport.w, _viewport.h,
		int(shadowCascadeLevels.size()) + 1,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTex, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);


	auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Frame buffer not complete: " << status << std::endl;;
	}


	glGenFramebuffers(1, &_minimapBuffer);
	//Binds the frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, _minimapBuffer);

	//Create color attachment texture
	glGenTextures(1, &_minimapClr);
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, _minimapClr);
	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _viewport.w, _viewport.h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//bind it to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _minimapClr, 0);
	//glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, _minimapClr, 0);

	//create a renderbuffer object for depth and stencil attachments
	glGenRenderbuffers(1, &_rbo2);
	glBindRenderbuffer(GL_RENDERBUFFER, _rbo2);
	// use a single renderbuffer object for both a depth AND stencil buffer.
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _viewport.w, _viewport.h);
	//bind it to framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo2);



	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDepthMask(GL_TRUE);

	//Sanity check
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	//Reset framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//when adding new framebuffers here, remember to modify resize_framebuffer too!
}

//Create a window
void Graphics::init()
{


	//SetUpShadowMapAndFBO();
	// Part 1: Initialize OpenGL state ...
	GCE //clean opengl error cache

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	videoStart = false;
	initVideoTexture();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/*  Hidden surface removal */
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	/*  Enable writing to depth buffer */
	glDepthMask(GL_TRUE);

	glEnable(GL_CULL_FACE);     /*  For efficiency, not drawing back-face */
	glCullFace(GL_BACK);

	//use the entire window as viewport ...
	glViewport(0, 0, _viewport.w, _viewport.h);
	editor_viewport = _viewport;
	//Set Current viewport to be the same in EDITOR

	// set to true - rendering/using imgui window in editor
	// set to false- rendering to full app window
	_renderToFrameBuffer = true;

	_render_lightbox = true;

	//stbi_set_flip_vertically_on_load(true);
	init_skybox();
	init_models(); GCE
		init_shaders(); GCE
		init_uniformBlockData();
	init_textures();
	Init_framebuffer();
	init_fboQuad();
	//init_texts();
	int tempW = 1280, tempH = 720; //since aspect Ratio is Fixed for font and sprites
	//init_text_rendering("Resources/Fonts/Aldrich.png", "Resources/Fonts/Aldrich.meta", _viewport.w, _viewport.h);
	init_text_rendering("Resources/Fonts/Aldrich.png", "Resources/Fonts/Aldrich.meta", tempW, tempH);
	//int hello_id = add_text("Hello World", 0, 0, 50.0, 1, 0, 0, 1);
	//int hello_id2 = add_text("Hello 1", 0.5, -0.5, 100.0, 1, 0, 0, 1);


}

#pragma endregion

//FrameBuffer Function
GLuint Graphics::getFrameBuffer() const
{
	return _framebuffer;
}

GLuint Graphics::getColorBuffer() const
{
	return _colorbuffer;
}

GLuint Graphics::getRedBuffer() const
{
	return _redbuffer;
}

GLuint Graphics::getVideoBuffer() const
{
	return video_handle;
}


bool Graphics::RenderToFrameBuffer()
{
	return _renderToFrameBuffer;
}


void Graphics::RenderToFrameBuffer(bool flag)
{
	_renderToFrameBuffer = flag;
}

void Graphics::render_frameBuffer()
{
	//if we are rendereing into the editor's window, don't run
	if (!_renderToFrameBuffer)
	{
		////bind default
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);

		GLSLShader& screenShader = _shaders["framebuffer_screen"];
		screenShader.Use();

		glClearColor(_bgClr.x, _bgClr.y, _bgClr.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		/*float BGMvol1 = RESOURCE->GetSoundMap()["BGM_Level1"].volume;
		float BGMvol2 = RESOURCE->GetSoundMap()["BGM_Level2"].volume;
		float BGMvol3 = RESOURCE->GetSoundMap()["BGM_Level3"].volume;*/

		
		screenShader.SetUniform("playingVideo", videoplaying);
		if (videoplaying)
		{
			if (videoStart == true)
			{
				openVideo();
				videoStart = false;
				skipOnce = true;
				static bool enterOnce = false;
				timer = 0.0;
				auto AUDIOSYSTEM = Engine::_ecs.getSystem<AudioSystem>();
				if (videoName == "boss_nebulas.mp4")
				{
					AUDIOSYSTEM->StopAllSFX();
					AUDIOSYSTEM->StopBGM("BG_Level3");
					AUDIOSYSTEM->PlayUISound("lv3BossEnter");
					AUDIOSYSTEM->PlayBGM("BG_Level3_Boss");
					RESOURCE->setSoundVolume("BG_Level3_Boss", 0.05f);
					enterOnce = false;
				}
				else if (videoName == "final_ending.mp4" && !enterOnce)
				{
					AUDIOSYSTEM->StopAllSFX();
					AUDIOSYSTEM->StopBGM("BG_Level3_Boss");
					AUDIOSYSTEM->PlayBGM("BG_Level3");
					RESOURCE->setSoundVolume("BG_Level3", 0.05f);
					AUDIOSYSTEM->PlayUISound("Lv3BossDefeatCutScene");
					enterOnce = true;
				}
				else if (videoName == "intro.mp4")
				{
					AUDIOSYSTEM->PlayUISound("intro");
					RESOURCE->setSoundVolume("BG_Level1", 0.05f);
				}
				else if (videoName == "boss_lazerernator.mp4")
				{	
					AUDIOSYSTEM->StopAllSFX();
					AUDIOSYSTEM->StopBGM("BG_Level1");
					AUDIOSYSTEM->PlayBGM("BG_Level3_Boss");
					RESOURCE->setSoundVolume("BG_Level3_Boss", 0.08f);
					AUDIOSYSTEM->PlayUISound("otherLvBossEnter");
				}
				else if (videoName == "weapon_laser.mp4")
				{
					AUDIOSYSTEM->StopAllSFX();
					AUDIOSYSTEM->StopBGM("BG_Level3_Boss");
					AUDIOSYSTEM->PlayBGM("BG_Level1");
					RESOURCE->setSoundVolume("BG_Level1", 0.05f);
					AUDIOSYSTEM->PlayUISound("OtherDefeatCutScene");
				}
				else if (videoName == "cutscene_map1.mp4")
				{
					AUDIOSYSTEM->PlayUISound("NOSound");
					AUDIOSYSTEM->StopUISound("OtherDefeatCutScene");
					RESOURCE->setSoundVolume("BG_Level1", 0.05f);
				}
				else if (videoName == "boss_rocketernator.mp4")
				{
					AUDIOSYSTEM->StopAllSFX();
					AUDIOSYSTEM->StopBGM("BG_Level2");
					AUDIOSYSTEM->PlayBGM("BG_Level3_Boss");
					RESOURCE->setSoundVolume("BG_Level3_Boss", 0.08f);
					AUDIOSYSTEM->PlayUISound("otherLvBossEnter");
				}
				else if (videoName == "weapon_rocket.mp4")
				{
					AUDIOSYSTEM->StopAllSFX();
					AUDIOSYSTEM->StopBGM("BG_Level3_Boss");
					AUDIOSYSTEM->PlayBGM("BG_Level2");
					RESOURCE->setSoundVolume("BG_Level2", 0.05f);
					AUDIOSYSTEM->PlayUISound("OtherDefeatCutScene");
				}
				else if (videoName == "cutscene_map2.mp4")
				{
					AUDIOSYSTEM->PlayUISound("NOSound");
					AUDIOSYSTEM->StopUISound("OtherDefeatCutScene");
					RESOURCE->setSoundVolume("BG_Level2", 0.05f);
				}
				else if (videoName == "credits.mp4")
				{
					AUDIOSYSTEM->PlayUISound("NOSound");
				}
			}
			//update_v5();
			glBindTexture(GL_TEXTURE_2D, video_handle);
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, vr_state.width, vr_state.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame_data);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, _colorbuffer);

			RESOURCE->setSoundVolume("BG_Level1", 0.15f);
			RESOURCE->setSoundVolume("BG_Level2", 0.2f);
			RESOURCE->setSoundVolume("BG_Level3", 0.2f);

			RESOURCE->setSoundVolume("BG_Level3_Boss", 0.2f);
		}
			

		glm::mat4 screenPos = glm::identity<glm::mat4>();
		screenShader.SetUniform("screenPOS", screenPos);
		glBindVertexArray(_quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		/*glBindVertexArray(_quadVAO);
		glBindTexture(GL_TEXTURE_2D, _colorbuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);*/


		if (_renderMinimap)
		{
			float scaleVal = 1.7f;

			glm::mat4 trans = glm::translate(glm::identity<glm::mat4>(), minimapPos);
			glm::mat4 scale = glm::scale(glm::identity<glm::mat4>(), minimapScale);
			screenPos = trans * scale;
			screenShader.SetUniform("screenPOS", screenPos);

			//draw minimap texture
			glBindTexture(GL_TEXTURE_2D, _minimapClr);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			//draw border on top of minimap
			scale = glm::scale(glm::identity<glm::mat4>(), minimapScale * scaleVal);
			screenPos = trans * scale;
			screenShader.SetUniform("screenPOS", screenPos);

			auto tex = _textures["MinimapBorder"];
			glBindTexture(GL_TEXTURE_2D, tex.texhandle);
			glDrawArrays(GL_TRIANGLES, 0, 6);

		}
		screenShader.UnUse();
		glBindVertexArray(0);
	}
}

bool Graphics::RenderLightbox() const
{
	return _render_lightbox;
}

void Graphics::RenderLightbox(bool flag)
{
	_render_lightbox = flag;
}

void Graphics::RenderShadowMap(const std::vector<render_pair>& renderList, const CameraCom& cam)
{

	GLSLShader& shdr = _shaders["simple_shadowmap"];
	shdr.Use();
	// Bind to shadowmap's FBO.
	//shadowFBO
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glViewport(0, 0, depthMapResolution, depthMapResolution); // Viewport for the texture.

	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT); //peter panning
	glEnable(GL_DEPTH_TEST);

	lightMatrices = getLightSpaceMatrices(cam);		GCE


		// Render function
		for (auto& pair : renderList)
		{
			auto& mCom = pair.second;
			auto& tCom = pair.first;

			if (mCom->m_model == "BasicTile" ||
				mCom->m_model == "Road_Tile" ||
				mCom->m_model == "BendRoadTile" ||
				mCom->m_model == "default" ||
				mCom->m_model == "MiddleRoadTile")
				continue;
			if (mCom->entityTag == "exclude_shadow")
				continue;

			if (mCom->m_model.find("Bone") != std::string::npos)
			{
				auto anim = Engine::_ecs.getComponent<AnimationCom>(mCom->entityID);

				Vector3 s = tCom->scale;
				Vector3 tmp = tCom->pos;
				Vector3 convertedRotate = tCom->rotate * ((float)M_PI / 180.0f);

				convertedRotate.y += (float)M_PI;
				if (Engine::_isPlay)
				{
					if (mCom->m_model.find("BossArm1Bone") != std::string::npos
						|| mCom->m_model.find("BossArm2Bone") != std::string::npos
						|| mCom->m_model.find("Pistol") != std::string::npos
						|| mCom->m_model.find("Railgun") != std::string::npos
						|| mCom->m_model.find("Rocket_launcher") != std::string::npos)
					{
						tmp.y = tCom->pos.y;
					}
					else
						tmp.y -= static_cast<float>(s.y / 2 + 1.60);
				}

				glm::mat4 oScale(glm::scale(glm::identity<glm::mat4>(), s.Vec3toGlmVec3())),
					oTranslate(glm::translate(glm::identity<glm::mat4>(), tmp.Vec3toGlmVec3()));

				glm::mat4
					xRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.x,glm::vec3(1.0f,0.0f,0.0f)) },
					yRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.y,glm::vec3(0.0f,1.0f,0.0f)) },
					zRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.z,glm::vec3(0.0f,0.0f,1.0f)) };

				glm::mat4 oRotate = zRotate * yRotate * xRotate;// *yRotate* zRotate;
				glm::mat4 modelMatrix = oTranslate * oRotate * oScale;
				glm::mat4 normals = glm::mat4(glm::transpose(glm::inverse(modelMatrix)));

				if (!Engine::_ecs.getComponent<AnimationCom>(mCom->entityID))
				{
					Engine::_ecs.addComponentToEntity(mCom->entityID, AnimationCom());
					Animator anim_ = _animationInfo[mCom->m_model];
					Engine::_ecs.getSystem<AnimationSystem>()->allAnimators[mCom->entityID] = std::make_shared<Animator>(anim_);
				}

				auto animator = Engine::_ecs.getSystem<AnimationSystem>()->allAnimators[mCom->entityID];
				std::vector<glm::mat4> currentPose = animator->currentPose;
				shdr.SetUniform("model", modelMatrix);
				int jointMatrixLoc = glGetUniformLocation(shdr.GetHandle(), "bone_transforms");
				glUniformMatrix4fv(jointMatrixLoc, static_cast<GLsizei>(currentPose.size()), GL_FALSE, glm::value_ptr(currentPose[0]));
				//shdr.SetUniform("normals_matrix", normals);
				shdr.SetUniform("useBone", 1);
			}
			else
			{
				//Get obj's model to world transform
				glm::mat4 oScale(glm::scale(glm::identity<glm::mat4>(), tCom->scale.Vec3toGlmVec3())),
					oTranslate(glm::translate(glm::identity<glm::mat4>(), tCom->pos.Vec3toGlmVec3()));

				Vector3 convertedRotate = tCom->rotate * ((float)M_PI / 180.0f);
				convertedRotate.y += (float)M_PI; //For flipping and facing the right way "defaultly"

				glm::mat4
					xRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.x,glm::vec3(1.0f,0.0f,0.0f)) },
					yRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.y,glm::vec3(0.0f,1.0f,0.0f)) },
					zRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.z,glm::vec3(0.0f,0.0f,1.0f)) };

				glm::mat4 oRotate = zRotate * yRotate * xRotate;// *yRotate* zRotate;
				glm::mat4 mdl_xform = oTranslate * oRotate * oScale;

				shdr.SetUniform("model", mdl_xform);
				shdr.SetUniform("useBone", 0);
			}

			GLModel& model = mCom->model_ref->second;
			glBindVertexArray(model.vaoid);
			glDrawElements(model.primitive_type, model.draw_cnt,
				GL_UNSIGNED_SHORT, NULL);
			glBindVertexArray(0);
		}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glCullFace(GL_BACK);
}

void Graphics::render_AABB(const std::shared_ptr<iRigid>& obj, GLSLShader& shader, GLModel& model)
{
	//Get obj's model to world transform (based on rigid body)
	glm::mat4 oScale(glm::scale(glm::identity<glm::mat4>(), obj->getScale().Vec3toGlmVec3())),
		oTranslate(glm::translate(glm::identity<glm::mat4>(), obj->getPos().Vec3toGlmVec3()));
	Quat qRot = obj->getRot();
	Vector3 Rotato = Quat_to_Euler(qRot);
	Vector3 convertedRotate = Rotato * ((float)M_PI / 180.0f);
	convertedRotate.y += (float)M_PI; //For flipping and facing the right way "defaultly"
	glm::mat4
		xRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.x,glm::vec3(1.0f,0.0f,0.0f)) },
		yRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.y,glm::vec3(0.0f,1.0f,0.0f)) },
		zRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.z,glm::vec3(0.0f,0.0f,1.0f)) };

	glm::mat4 oRotate = zRotate * yRotate * xRotate;
	glm::mat4 mdl_xform = oTranslate * oRotate * oScale;
	//set matrix
	shader.SetUniform("model", mdl_xform);

	// Render OpenGL primitives encapsulated by this object's VAO
	// using glDrawElements or glDrawArrays  
	glDrawElements(model.primitive_type, model.draw_cnt,
		GL_UNSIGNED_SHORT, NULL);
}

void Graphics::render_wireframe(const render_pair& pair, const CameraCom& m_cam)
{
	auto& obj = *pair.first;
	GLSLShader& shader = _shaders["default_shader"];
	GLModel& model = pair.second->model_ref->second;

	glBindVertexArray(model.vaoid);

	// enable shader program
	shader.Use();

	//Getting uniform variables and setting them.
	glUseProgram(shader.GetHandle());

	//Set to render only lines
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Get obj's model to world transform
	glm::mat4 oScale(glm::scale(glm::identity<glm::mat4>(), obj.scale.Vec3toGlmVec3())),
		oTranslate(glm::translate(glm::identity<glm::mat4>(), obj.pos.Vec3toGlmVec3()));

	Vector3 convertedRotate = obj.rotate * ((float)M_PI / 180.0f);
	convertedRotate.y += (float)M_PI; //For flipping and facing the right way "defaultly"

	glm::mat4
		xRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.x,glm::vec3(1.0f,0.0f,0.0f)) },
		yRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.y,glm::vec3(0.0f,1.0f,0.0f)) },
		zRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.z,glm::vec3(0.0f,0.0f,1.0f)) };

	//glm::mat4 oRotate = glm::toMat4(obj.rotate);

	glm::mat4 oRotate = zRotate * yRotate * xRotate;
	glm::mat4 mdl_xform = oTranslate * oRotate * oScale;
	shader.SetUniform("model", mdl_xform);

	////combine camera world to ndc matrix with obj's mdl-to-world transform
	Matrix4 projMat = m_cam._proj_mtx;
	Matrix4 viewMat = m_cam._view_mtx;
	glm::mat4 glm_trans = projMat.Mtx4toGlmMat4() * viewMat.Mtx4toGlmMat4() * mdl_xform;
	shader.SetUniform("uMVP", glm_trans);

	/*GLint uniform_var_loc2 = glGetUniformLocation(shader.GetHandle(), "uMVP");
	if (uniform_var_loc2 >= 0)
	{
		glUniformMatrix4fv(uniform_var_loc2, 1, GL_FALSE,
			glm::value_ptr(glm_trans));
	}*/

	//Blending to enable transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Render OpenGL primitives encapsulated by this object's VAO
	// using glDrawElements or glDrawArrays  
	glDrawElements(model.primitive_type, model.draw_cnt,
		GL_UNSIGNED_SHORT, NULL);

	//Clean up
	// Breaking the binding set up in Part 2: glBindVertexArray(0);
	// Deinstall the shader program installed in Part 1 using
	glBindVertexArray(0);
	shader.UnUse();


	//keep this line just in case and it means no harm;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void error_uniform_not_found(std::string variable)
{
	std::string errormsg = "Uniform " + variable + " variable doesn't exist!!!\n";
	std::cout << errormsg << std::endl;
	//std::exit(EXIT_FAILURE);
}

void Graphics::exit()
{
	//TO NOTE: There's a weird bug occuring around here where containers not deleting
	// properly after loading scenes twice.
	// Due to safe checking of iterators by visual studio during debug mode
	// it would loop through infinite iterator hell. So to temporarily fix it
	// I have set _HAS_ITERATOR_DEBUGGING to 0 (release mode equivalent)  &
	// I have set _ITERATOR_DEBUG_LEVEL  to 0 (release mode equivalent)  in
	// Project Pages/Configuration Properties/C++/Preprocessor/Preprocessor Definitions
	// to not get stuck. :/

	//Cleanup for models, shader programs, textures and framebuffer
	//made during runtime.

	for (auto i = _textures.begin(); i != _textures.end(); ++i)
		glDeleteTextures(1, &i->second.texhandle);

	for (auto& x : _shaders)
		x.second.DeleteShaderProgram();

	for (auto& x : _models)
	{
		glDeleteVertexArrays(1, &x.second.vaoid);
		glDeleteBuffers(1, &x.second.vboid);
	}

	for (auto& x : _textures)
		glDeleteTextures(1, &x.second.texhandle);

	//delete render framebuffer
	delete_framebuffers();

	//delete skybox stuffs
	glDeleteBuffers(1, &_skyboxVBO);
	glDeleteVertexArrays(1, &_skyboxVAO);

	free_text();

	glDeleteTextures(1, &video_handle);
	video_reader_close(&vr_state);
	//glDeleteFramebuffers(1, &video_handle);
	_aligned_free(frame_data);
}

void Graphics::update(float /*dt*/, const CameraCom& cam, const std::vector<render_pair>& renderList)
{
	if (renderList.size() == 0)
		return;
	//TEST
   //if (Window::f9_flag)
	/*if (Inputs::get_Key_Triggered(GLFW_KEY_F9))
		_renderToFrameBuffer = !_renderToFrameBuffer;*/

		//======Viewport cutting==================
		//Using the viewport stored in Graphics.
	glViewport(_viewport.x,
		_viewport.y,
		_viewport.w,
		_viewport.h);
	//Set up scissor
	//glScissor(_viewport.x,
	//    _viewport.y, 
	//    _viewport.w,
	//    _viewport.h);
	////enable scissor test
	//glEnable(GL_SCISSOR_TEST);

	//Bind to framebuffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebuffer);
	glDrawBuffers(2, _buffers);

	glEnable(GL_DEPTH_TEST);

	//use clear
	glClearColor(_bgClr.x, _bgClr.y, _bgClr.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//rendering done here//
	for (auto& t : renderList)
		render_wireframe(t, cam);

	//Reset framebuffer(unbind)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Graphics::update_AABB(float /*dt*/, const CameraCom&, const std::vector<std::shared_ptr<iRigid>>& rigidList, unsigned selected)
{
	if (rigidList.size() == 0)
		return;
	//Bind to framebuffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebuffer);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glm::vec3 highlightClr{ 1.0f,0.0f,0.0f };
	glm::vec3 normalClr{ 0.0f,0.0f,1.0f };

	GLSLShader& shader = _shaders["wireframe_shader"];
	//GLModel& model = _models["wireframe_box"];
	GLModel& model = _models["default"];
	glBindVertexArray(model.vaoid);
	// enable shader program
	shader.Use();

	//combine camera world to ndc matrix with obj's mdl-to-world transform
	//Matrix4 projMat = cam._proj_mtx;
	//Matrix4 viewMat = cam._view_mtx;
	//glm::mat4 glm_trans = projMat.Mtx4toGlmMat4() * viewMat.Mtx4toGlmMat4();
	//shader.SetUniform("viewProj", glm_trans);

	//Set to render only lines
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(3.0f);              //size of AABB wireframe Render

	for (unsigned i = 0; i < rigidList.size(); ++i)
	{
		if (i == selected)
			shader.SetUniform("colour_modifier", highlightClr);
		else
			shader.SetUniform("colour_modifier", normalClr);
		render_AABB(rigidList[i], shader, model);
	}

	glLineWidth(1.0f);				//reset linewidth

	//Clean up
	// Breaking the binding set up in Part 2: glBindVertexArray(0);
	// Deinstall the shader program installed in Part 1 using
	glBindVertexArray(0);
	shader.UnUse();
	//reset depth test to normal
	glDepthFunc(GL_LESS);
	////keep this line just in case and it means no harm;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//Reset framebuffer(unbind)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Graphics::update_v4(float /*dt*/, const CameraCom& cam,
	const std::vector<render_pair>& renderList,
	const std::vector<std::shared_ptr<LightCom>>& lightList)
{
	destroy_text();
	if (renderList.size() == 0)
		return;

	//for objects that have transparency
	std::map<float, render_pair> transparent_List;
	//initialising any material components
	for (auto& pair : renderList)
	{
		// Render OpenGL primitives encapsulated by this object's VAO
		// using glDrawElements or glDrawArrays  

		auto mCom = pair.second;
		if (!mCom->m_shader.compare("skinning_shader") &&
			((mCom->m_model.find("Bone") != std::string::npos)))
		{
			continue;
		}
		if (mCom->isInitialised == false)
		{
			//init material
			auto mIT = _models.find(mCom->m_model);
			if (mIT == _models.end())
				mIT = _models.find("default");//no valid model found
			mCom->model_ref = mIT;



			auto sIT = _shaders.find(mCom->m_shader);
			if (sIT == _shaders.end())
				sIT = _shaders.find("forward_shader");//no valid shader found
			mCom->shader_ref = sIT;



			//if no texture found, just use end iterator so that we have a way to check
			mCom->tex_ref = _textures.find(mCom->m_texture);

			auto taglist = Engine::_ecs.getEntityTag(mCom->entityID);

			if (taglist.find("Player") != taglist.end())
				mCom->entityTag = "Player";
			else if (taglist.find("Enemy") != taglist.end())
				mCom->entityTag = "Enemy";
			else if (taglist.find("Helicopter") != taglist.end())
				mCom->entityTag = "Helicopter";
			else if (taglist.find("exclude_shadow") != taglist.end())
				mCom->entityTag = "exclude_shadow";
			else
				mCom->entityTag = "None";

			mCom->isInitialised = true;
		}
	}

	RenderShadowMap(renderList, cam);



	//======Viewport cutting==================
	//Using the viewport stored in Graphics.
	if (!_renderToFrameBuffer)//fullscreen -playmode-
	{
		if (_viewport.h != Engine::_ecs.getSystem<WindowSystem>()->getWindowHeight() ||
			_viewport.w != Engine::_ecs.getSystem<WindowSystem>()->getWindowWidth())
		{
			resize_viewport(Engine::_ecs.getSystem<WindowSystem>()->getWindowWidth(),
				Engine::_ecs.getSystem<WindowSystem>()->getWindowHeight());
		}
		glViewport(
			0,
			0,
			_viewport.w,
			_viewport.h);
	}
	else
	{
		if (_viewport.h != editor_viewport.h ||
			_viewport.w != editor_viewport.w)
			resize_viewport(editor_viewport.w, editor_viewport.h);

		glViewport(
			0,
			0,
			editor_viewport.w,
			editor_viewport.h);
	}

	//Bind to framebuffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebuffer);
	glDrawBuffers(2, _buffers);

	//use clear
	glClearColor(_bgClr.x, _bgClr.y, _bgClr.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render Skybox here
	render_skybox(cam);

	//rebind framebuffer since skybox unbinds it
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebuffer);
	glDrawBuffers(2, _buffers);


	glEnable(GL_DEPTH_TEST);

	//blending to enable transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	GLSLShader& lightShader = _shaders["forward_shader"];
	glUseProgram(lightShader.GetHandle());


	glm::vec3 camPos = Vector3{ cam._pos }.Vec3toGlmVec3();
	lightShader.SetUniform("viewPos", camPos);
	lightShader.SetUniform("material.shininess", 90.0f);

	std::vector<LightCom::DirLight> DirList;
	std::vector<LightCom::PointLight> PointList;
	std::vector<LightCom::SpotLight> SpotList;

	//sort the lights
	for (auto& x : lightList)
	{
		switch (x->_type_light)
		{
		case(LightCom::directional):
			DirList.push_back(x->return_as_Light<LightCom::DirLight>());
			break;
		case(LightCom::point):
			PointList.push_back(x->return_as_Light<LightCom::PointLight>());
			break;
		case(LightCom::spot):
			SpotList.push_back(x->return_as_Light<LightCom::SpotLight>());
			break;

		default:
			break;
		}

	}

#pragma region _lights
	// directional light
	int numDirLights = MAX_DIR_LIGHTS > DirList.size() ? (int)DirList.size() : MAX_DIR_LIGHTS;
	lightShader.SetUniform("numDir", numDirLights);
	for (int i = 0; i < numDirLights; ++i)
	{
		Send_dirLight(i, lightShader, DirList[i]);

		Vector3 convertedRotate = DirList[i].direction;// *((float)M_PI / 180.0f);
		lightLookAt = glm::normalize(convertedRotate.Vec3toGlmVec3());
	}


	// point light
	int numPointLights = NR_MAX_LIGHTS > PointList.size() ? (int)PointList.size() : NR_MAX_LIGHTS;
	lightShader.SetUniform("numPoint", numPointLights);
	for (int i = 0; i < numPointLights; ++i)
	{
		Send_pointLight(i, lightShader, PointList[i]);
	}

	// Spot Light
	int numSpotLights = NR_MAX_LIGHTS > SpotList.size() ? (int)SpotList.size() : NR_MAX_LIGHTS;
	lightShader.SetUniform("numSpot", numSpotLights);
	for (int i = 0; i < numSpotLights; ++i)
	{
		Send_spotLight(i, lightShader, SpotList[i]);
	}

	////get camera eye from view cam
	//glm::mat4 view = Matrix4(cam._view_mtx).Mtx4toGlmMat4();
	//// view/projection transformations
	//glm::mat4 projection = Matrix4(cam._proj_mtx).Mtx4toGlmMat4();
	//lightShader.SetUniform("projection", projection);
	//lightShader.SetUniform("view", view);


	//glm::vec3 camEye = glm::vec3(-view[0][2], -view[1][2], -view[2][2]);
	//// spotLight
	//lightShader.SetUniform("spotLight.position", camPos);
	//lightShader.SetUniform("spotLight.direction", camEye);
	//lightShader.SetUniform("spotLight.ambient", 0.6f, 0.6f, 0.6f);
	//lightShader.SetUniform("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	//lightShader.SetUniform("spotLight.specular", 1.0f, 1.0f, 1.0f);
	//lightShader.SetUniform("spotLight.constant", 3.0f);
	//lightShader.SetUniform("spotLight.linear", 0.009f);
	//lightShader.SetUniform("spotLight.quadratic", 0.092f);

	//lightShader.SetUniform("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	//lightShader.SetUniform("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
#pragma endregion

	//Shadow Uniforms
	int depthLoc = glGetUniformLocation(lightShader.GetHandle(), "shadowMap");
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D_ARRAY, depthTex);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glUniform1i(depthLoc, 1);

	lightShader.SetUniform("farPlane", cam._m_far);
	lightShader.SetUniform("cascadeCount", (int)shadowCascadeLevels.size());
	for (size_t i = 0; i < shadowCascadeLevels.size(); ++i)
	{
		std::string str_ = "cascadePlaneDistances[" + std::to_string(i) + "]";
		lightShader.SetUniform(str_.c_str(), shadowCascadeLevels[i]);
	}


	//get camera eye from view cam
	glm::mat4 view = Matrix4(cam._view_mtx).Mtx4toGlmMat4();
	glm::vec3 camEye = glm::vec3(-view[0][2], -view[1][2], -view[2][2]);
	camEye = glm::normalize(camEye);
	for (auto& pair : renderList)
	{
		//===temporary simple culling===//
		glm::vec3 iobjPos = pair.first->pos.Vec3toGlmVec3() - camPos;
		float proj = glm::dot(iobjPos, camEye);
		if (proj < 0)
		{
			glm::vec3 objScale = pair.first->scale.Vec3toGlmVec3();
			//turn proj value positive
			proj *= -1;
			float highestScale = std::max(objScale.x, std::max(objScale.y, objScale.z));
			//Hard coded/eye power/ distance values
			//the floor
			if ((highestScale <= 0.55f && highestScale >= 0.29) && proj > 54)
				continue; //don't render
			//buildings and stuffs
			else if (highestScale <= 0.2f && proj > 13)
				continue; //don't render
		}
		//=================================================//

		auto mCom = pair.second;
		if (!mCom->m_shader.compare("skinning_shader") &&
			((mCom->m_model.find("Bone") != std::string::npos)))
		{
			continue;
		}

		//add renderPair to a transparency list
		if (mCom->_alpha > 0 && mCom->_alpha < 1.0f)
		{
			glm::vec3 objPos = pair.first->pos.Vec3toGlmVec3() - camPos;
			auto iproj = glm::dot(objPos, camEye);
			float secretIncrement = 0.0001f;
			while (transparent_List.count(iproj) > 0)
			{
				iproj += secretIncrement;
			}
			transparent_List[iproj] = pair;
			continue;
		}

		update_Render(pair, lightShader);

	}

	//second render for transparent objects
	//int i = 1;
	for (auto rIT = transparent_List.rbegin(); rIT != transparent_List.rend(); ++rIT)
		update_Render(rIT->second, lightShader);

	lightShader.UnUse();

	//draw light boxes
	if (_render_lightbox)
	{
		float lightScale = 0.5f;
		GLSLShader& shader = _shaders["wireframe_shader"];
		shader.Use();


		GLModel& model = _models["default"];
		glBindVertexArray(model.vaoid);

		for (auto& x : lightList)
		{
			if (x->_type_light != LightCom::None)
			{
				glm::vec3 poss = x->position.Vec3toGlmVec3();
				glm::mat4  oTrans(glm::translate(glm::identity<glm::mat4>(), poss)),
					oScale(glm::scale(glm::identity<glm::mat4>(), glm::vec3(lightScale)));
				glm::mat4 mdl_xform = oTrans * oScale;
				shader.SetUniform("model", mdl_xform);


				Vector3 color = x->ambient + x->diffuse + x->specular;
				glm::vec3 clr = color.Vec3toGlmVec3();
				shader.SetUniform("colour_modifier", clr);
				glDrawElements(model.primitive_type, model.draw_cnt,
					GL_UNSIGNED_SHORT, NULL);
			}

		}
		glBindVertexArray(0);
		shader.UnUse();
	}



	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//mini map testing
	RenderMiniMap(renderList, cam);

}
void Graphics::update_v5(float delta_time)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set up orphographic projection
	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, _viewport.w, _viewport.h, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);*/

	// Read a new frame and load it into texture
	if (_pauseCutScene)
		return;

	if (!(vr_state.av_format_ctx || vr_state.av_codec_ctx || vr_state.av_frame || vr_state.av_packet))
		return;

	if (skipOnce)
	{
		timer += delta_time;
		if (timer >= 1.5)
		{
			skipOnce = false;
			timer = 0.0;
		}
	}

	int64_t pts{ 1 };
	if (!video_reader_read_frame(&vr_state, frame_data, &pts)) {
		printf("Couldn't load video frame\n");
	}

	if (first_frame) {
		staringTime = glfwGetTime();
		first_frame = false;
	}
	
	
	double pt_in_seconds = (pts * (double)vr_state.time_base.num / (double)vr_state.time_base.den) + staringTime;
	while (pt_in_seconds > glfwGetTime()) {
		glfwWaitEventsTimeout(pt_in_seconds - glfwGetTime());
	}
	if ((glfwGetTime() > (staringTime + videoEndTime) || Inputs::get_Key_Triggered(GLFW_KEY_Z) || Inputs::get_GamePatButton_Trigger(GLFW_GAMEPAD_BUTTON_A)) && !skipOnce)
	{
		auto UISYSTEM = Engine::_ecs.getSystem<UISystem>();
		
		UISYSTEM->toggleCutscene(false);
		_playingCutscene = false;
		UISYSTEM->set_RenderSystem_Minimap(showMinimapAfter);
		toggleMinimap(showMinimapAfter);

		//_renderMinimap = true;
		skipOnce = true;
		videoStart = false;
		videoplaying = false;
		first_frame = true;
		auto AUDIOSYSTEM = Engine::_ecs.getSystem<AudioSystem>();
		if (videoName == "boss_nebulas.mp4")
		{
			AUDIOSYSTEM->StopUISound("lv3BossEnter");
			//AUDIOSYSTEM->UnpauseBGM("BG_Level3");
		}
		else if (videoName == "final_ending.mp4")
		{
			Engine::_ecs.instantiatePrefab(Engine::_ecs.getPrefabID("Prefab_simple_Black_Screen"), "simple_Black_Screen");
			AUDIOSYSTEM->StopUISound("Lv3BossDefeatCutScene");
		}
		else if (videoName == "boss_lazerernator.mp4" || videoName == "boss_rocketernator.mp4")
		{
			AUDIOSYSTEM->StopUISound("otherLvBossEnter");
		}
		else if (videoName == "weapon_laser.mp4" || videoName == "weapon_rocket.mp4")
		{
			Engine::_ecs.instantiatePrefab(Engine::_ecs.getPrefabID("Prefab_simple_Black_Screen"), "simple_Black_Screen");
			AUDIOSYSTEM->StopUISound("OtherDefeatCutScene");
		}
		else if (videoName == "intro.mp4")
		{
			AUDIOSYSTEM->StopUISound("intro");
		}
		else if (videoName == "credits.mp4")
		{
			AUDIOSYSTEM->StopUISound("Lv3BossDefeatCutScene");
		}

		video_reader_close_now(&vr_state);
			
		uint screenID = Engine::_ecs.getPrefabID("Prefab_simple_FadeBlack_Screen");
		Engine::_ecs.instantiatePrefab(screenID, "simple_FadeBlack_Screen");

		/*video_reader_close(&vr_state);
		if (frame_data)
		{
			auto temp = frame_data;
			frame_data = nullptr;
			glDeleteTextures(sizeof(frame_data) / 8, frame_data);
			free(temp);
		}*/
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, video_handle);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, vr_state.width, vr_state.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame_data);
	}
	//// Render whatever you want
 //   glEnable(GL_TEXTURE_2D);
 //   glBindTexture(GL_TEXTURE_2D, video_handle);
 //   glBegin(GL_QUADS);
 //   glTexCoord2d(0, 0); glVertex2i(0, 0);
 //   glTexCoord2d(1, 0); glVertex2i(vr_state.width, 0);
 //   glTexCoord2d(1, 1); glVertex2i(vr_state.width, vr_state.height);
 //   glTexCoord2d(0, 1); glVertex2i(0, vr_state.height);
 //   glEnd();
 //   glDisable(GL_TEXTURE_2D);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Graphics::initVideoTexture()
{
	//if (!video_reader_open(&vr_state, std::string("Resources/Videos/" + videoName).c_str())) {
	//	printf("Couldn't open video file (make sure you set a video file that exists)\n");
	//}

	// Generate texture
	glGenTextures(1, &video_handle);
	glBindTexture(GL_TEXTURE_2D, video_handle);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Allocate frame buffer
	constexpr int ALIGNMENT = 128;
	const int frame_width = 1920;
	const int frame_height = 1080;
	if (posix_memalign((void**)&frame_data, ALIGNMENT, frame_width * frame_height * 4) != 0) {
		printf("Couldn't allocate frame buffer\n");
	}
}
//void Graphics::render_anim(const CameraCom& cam, EntityID id)
//{
//	MaterialCom& m = *Engine::_ecs.getComponent<MaterialCom>(id);
//
//	if (m.m_shader == "skinning_shader" &&
//		((m.m_model.find("Bone") != std::string::npos)))
//	{
//		if (!Engine::_ecs.getComponent<AnimationCom>(id))
//		{
//			Engine::_ecs.addComponentToEntity(id, AnimationCom());
//			Animator tmp = _animationInfo[m.m_model];
//
//			tmp.texture = m.m_texture;
//			Engine::_ecs.getSystem<AnimationSystem>()->allAnimators[id] = std::make_shared<Animator>(tmp);
//			//updateAnimationCom(*idIter);
//		}
//		//double check to ensure texture correct
//		Engine::_ecs.getSystem<AnimationSystem>()->allAnimators[id]->texture = m.m_texture;
//	}
//
//	EntityID eid = id;
//
//	GLSLShader& shdr = _shaders["skinning_shader"];
//	shdr.Use();
//	//glUseProgram(shdr.GetHandle());
//
//	auto anim = Engine::_ecs.getComponent<AnimationCom>(eid);
//	auto tCom = Engine::_ecs.getComponent<TransformCom>(eid);
//
//	Vector3 s = tCom->scale;
//	Vector3 tmp = tCom->pos;
//	Vector3 convertedRotate = tCom->rotate * ((float)M_PI / 180.0f);


void Graphics::render_sprite3D(const CameraCom&, const std::vector<renderUI_pair>& renderList)
{
	if (renderList.size() == 0)
		return;

	if (!_renderToFrameBuffer)//fullscreen -playmode-
		glViewport(
			0,
			0,
			_viewport.w,
			_viewport.h);
	else
		glViewport(
			0,
			0,
			editor_viewport.w,
			editor_viewport.h);

	//Bind to framebuffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebuffer);
	glDrawBuffers(2, _buffers);


	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLSLShader& shader = _shaders["ui3D_shader"];

	shader.Use();

	auto mdl = _models.find("plane");
	glBindVertexArray(mdl->second.vaoid);

	for (auto& pair : renderList)
	{
		if (/*Engine::_isPlay && */pair.second->_isBar)
			mdl->second.BAR_update_uv(pair.second->barPercentage / 100.0f);
		draw_sprite3D(pair, shader);
		glDrawArrays(mdl->second.primitive_type, 0, mdl->second.draw_cnt);

		if (/*Engine::_isPlay && */pair.second->_isBar)
			mdl->second.reset_uv();
	}

	glBindVertexArray(0);

	shader.UnUse();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
void Graphics::render_skybox(const CameraCom&)
{
	//Bind to framebuffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebuffer);
	glDrawBuffers(1, _buffers);
	// draw skybox as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

	GLSLShader& skyboxShader = _shaders["skybox_shader"];
	skyboxShader.Use();
	//glm::mat4 vi = Matrix4(cam._view_mtx).Mtx4toGlmMat4(); // remove translation from the view matrix
	//glm::mat4 view = glm::mat4(glm::mat3(vi)); // remove translation from the view matrix
	//glm::mat4 projection = Matrix4(cam._proj_mtx).Mtx4toGlmMat4();
	//skyboxShader.SetUniform("view", view);
	//skyboxShader.SetUniform("projection",projection);
	// skybox cube
	Texture cubeMaptexture = _textures[currSkybox];
	glBindVertexArray(_skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMaptexture.texhandle);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default

	skyboxShader.UnUse();

	//Reset framebuffer(unbind)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Graphics::Send_pointLight(const int& pos, GLSLShader& shdr, LightCom::PointLight& pL)
{
	const std::string prefix = "pointLights[" + std::to_string(pos) + "].";
	std::string str;
	str = prefix + "position";
	glm::vec3 dir = pL.direction.Vec3toGlmVec3();
	shdr.SetUniform(str.c_str(), dir);
	str = prefix + "ambient";
	glm::vec3 ambient = pL.ambient.Vec3toGlmVec3();
	shdr.SetUniform(str.c_str(), ambient);

	str = prefix + "diffuse";
	glm::vec3 diffuse = pL.diffuse.Vec3toGlmVec3();
	shdr.SetUniform(str.c_str(), diffuse);

	str = prefix + "specular";
	glm::vec3 specular = pL.specular.Vec3toGlmVec3();
	shdr.SetUniform(str.c_str(), specular);

	str = prefix + "constant";
	shdr.SetUniform(str.c_str(), pL.constant);

	str = prefix + "linear";
	shdr.SetUniform(str.c_str(), pL.linear);
	str = prefix + "quadratic";
	shdr.SetUniform(str.c_str(), pL.quadratic);
}
void Graphics::Send_dirLight(const int& pos, GLSLShader& shdr, LightCom::DirLight& dL)
{
	const std::string prefix = "dirLights[" + std::to_string(pos) + "].";
	std::string str;

	str = prefix + "direction";
	glm::vec3 test = dL.direction.Vec3toGlmVec3();
	shdr.SetUniform(str.c_str(), test);

	str = prefix + "ambient";
	glm::vec3 ambient = dL.ambient.Vec3toGlmVec3();
	shdr.SetUniform(str.c_str(), ambient);

	str = prefix + "diffuse";
	glm::vec3 diffuse = dL.diffuse.Vec3toGlmVec3();
	shdr.SetUniform(str.c_str(), diffuse);

	str = prefix + "specular";
	glm::vec3 specular = dL.specular.Vec3toGlmVec3();
	shdr.SetUniform(str.c_str(), specular);
}

void Graphics::Send_spotLight(const int& pos, GLSLShader& shdr, LightCom::SpotLight& sL)
{
	const std::string prefix = "spotLights[" + std::to_string(pos) + "].";
	std::string str;
	str = prefix + "direction";
	glm::vec3 dir = sL.direction.Vec3toGlmVec3();
	shdr.SetUniform(str.c_str(), dir);

	str = prefix + "ambient";
	glm::vec3 ambient = sL.ambient.Vec3toGlmVec3();
	shdr.SetUniform(str.c_str(), ambient);

	str = prefix + "diffuse";
	glm::vec3 diffuse = sL.diffuse.Vec3toGlmVec3();
	shdr.SetUniform(str.c_str(), diffuse);

	str = prefix + "specular";
	glm::vec3 specular = sL.specular.Vec3toGlmVec3();
	shdr.SetUniform(str.c_str(), specular);

	str = prefix + "position";
	glm::vec3 po = sL.position.Vec3toGlmVec3();
	shdr.SetUniform(str.c_str(), po);

	str = prefix + "cutOff";
	shdr.SetUniform(str.c_str(), sL.cutOff);

	str = prefix + "outerCutOff";
	shdr.SetUniform(str.c_str(), sL.outerCutOff);

	str = prefix + "constant";
	shdr.SetUniform(str.c_str(), sL.constant);

	str = prefix + "linear";
	shdr.SetUniform(str.c_str(), sL.linear);

	str = prefix + "quadratic";
	shdr.SetUniform(str.c_str(), sL.quadratic);
}

void Graphics::gl_checkerror(const char* function, const char* file, int line)
{
	GLenum error = 0;
	while ((error = glGetError()) != GL_NO_ERROR)
	{
		switch (error)
		{
		case GL_INVALID_ENUM:
			std::cout << "GL_INVALID_ENUM" << std::endl;
			break;
		case GL_INVALID_VALUE:
			std::cout << "GL_INVALID_VALUE" << std::endl;
			break;
		case GL_INVALID_OPERATION:
			std::cout << "GL_INVALID_OPERATION" << std::endl;
			break;
		case GL_STACK_OVERFLOW:
			std::cout << "GL_STACK_OVERFLOW" << std::endl;
			break;
		case GL_STACK_UNDERFLOW:
			std::cout << "GL_STACK_UNDERFLOW" << std::endl;
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl;
			break;
		default:
			std::cout << "Unknown error code: " << error << std::endl;
		}
		std::cout << "encountered at function: "
			<< function << " ("
			<< file << ", "
			<< line << ")" << std::endl;
	}

}

void Graphics::openVideo()
{
	if (!video_reader_open(&vr_state, std::string("Resources/Videos/" + videoName).c_str())) {
		printf("Couldn't open video file (make sure you set a video file that exists)\n");
	}
}

void Graphics::init_uniformBlockData()
{
	for (auto& x : _shaders)
	{
		int ubIndex = glGetUniformBlockIndex(x.second.GetHandle(), "Matrices");
		if (ubIndex >= 0)
		{
			//std::cout << x.first << " found!" << std::endl;
			glUniformBlockBinding(x.second.GetHandle(), ubIndex, BUFFERLOC::Matrices);
		}

		ubIndex = glGetUniformBlockIndex(x.second.GetHandle(), "LightSpaceMatrices");
		if (ubIndex >= 0)
		{
			glUniformBlockBinding(x.second.GetHandle(), ubIndex, BUFFERLOC::LightMatrices);
		}
		//else
		//	std::cout << x.first << ": not found" << std::endl;

		// texture object texobj will use texture image unit 6
		/*glBindTextureUnit(6, tex.texhandle);
		glTextureParameteri(tex.texhandle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(tex.texhandle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		GLuint tex_loc = glGetUniformLocation(shdr.GetHandle(), "diff_texture");
		if (tex_loc >= 0)
			glUniform1i(tex_loc, 6);*/

	}

	//create matrix buffer
	unsigned int uboMatrices;
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);

	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, BUFFERLOC::Matrices);
	// define the range of the buffer that links to a uniform binding point
	glBindBufferRange(GL_UNIFORM_BUFFER, BUFFERLOC::Matrices, uboMatrices, 0, 2 * sizeof(glm::mat4));

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	_UBOs[BUFFERLOC::Matrices] = uboMatrices;
	GCE
		// configure light matrix UBO
		unsigned int matricesUBO;
	glGenBuffers(1, &matricesUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
	int lightmatrices_size = 5;
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * lightmatrices_size, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, BUFFERLOC::LightMatrices);
	glBindBufferRange(GL_UNIFORM_BUFFER, BUFFERLOC::LightMatrices, matricesUBO, 0, lightmatrices_size * sizeof(glm::mat4));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	_UBOs[BUFFERLOC::LightMatrices] = matricesUBO;
	GCE
}

void Graphics::sendBufferData(CameraCom& cam)
{
	glm::mat4 matrices[2];
	matrices[0] = glm::transpose(cam._proj_mtx.Mtx4toGlmMat4());
	matrices[1] = glm::transpose(cam._view_mtx.Mtx4toGlmMat4());



	glBindBuffer(GL_UNIFORM_BUFFER, _UBOs[BUFFERLOC::Matrices]);
	/*glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(cam._proj_mtx.Mtx4toGlmMat4()));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4),sizeof(glm::mat4) * 2,
		glm::value_ptr(cam._view_mtx.Mtx4toGlmMat4()));*/
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4) * 2, matrices); GCE
		glBindBuffer(GL_UNIFORM_BUFFER, 0);


	if (!lightMatrices.empty())
	{
		glm::mat4 lMatrices[5];
		for (uint i = 0; i < lightMatrices.size(); ++i)
			lMatrices[i] = lightMatrices[i];
		glBindBuffer(GL_UNIFORM_BUFFER, _UBOs[BUFFERLOC::LightMatrices]); GCE
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4) * 5, lMatrices); GCE

			glBindBuffer(GL_UNIFORM_BUFFER, 0); GCE
	}
	GCE
}

void Graphics::AddGammaValue(float val)
{
	_defaultGamma += val;

	if (_defaultGamma > 0.4f)
		_defaultGamma = 0.4f;
	else if (_defaultGamma < -0.6f)
		_defaultGamma = -0.6f;
}

float Graphics::getGammaValue()
{
	return _defaultGamma;
}


void Graphics::setGammaVal(float val)
{
	_defaultGamma = val;
}

void Graphics::toggleMinimap(bool val)
{
	_renderMinimap = val;
}

bool Graphics::toggleMinimap()
{
	return _renderMinimap;
}

void Graphics::toggleCutscene(bool val)
{
	_playingCutscene = val;
}

bool Graphics::toggleCutscene()
{
	return _playingCutscene;
}



void Graphics::SetWindow_Handle(GLFWwindow* winPtr)
{
	_windowptr = winPtr;
}
void Graphics::set_bgClr(const glm::vec3& clr)
{
	_bgClr = clr;
}

glm::vec3 Graphics::get_bgClr()
{
	return _bgClr;
}
void Graphics::add_texture(const std::string& name, const GLuint& texID)
{
	Texture tex;
	tex.texhandle = texID;
	_textures.emplace(name, tex);

}
int Graphics::readPixel(uint32_t attachmentIndex, int x, int y)
{
	int pixelData = -1;
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
	glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return pixelData;
}

std::vector<std::string> Graphics::GetModels()
{
	std::vector<std::string> strList;
	for (auto& x : _models)
		strList.push_back(x.first);
	return strList;
}
std::vector<std::string> Graphics::GetShaders()
{
	std::vector<std::string> strList;
	for (auto& x : _shaders)
		strList.push_back(x.first);
	return strList;
}
std::vector<std::string> Graphics::GetTextures()
{
	std::vector<std::string> strList;
	for (auto& x : _textures)
		strList.push_back(x.first);
	return strList;
}


Graphics::model_iterator Graphics::GetModelRefByKey(std::string key)
{
	model_iterator m_it = _models.find(key);

	if (m_it == _models.end())
	{
		std::cout << key << " was not found in models container" << std::endl;
		//m_it == _models.begin();
		m_it = _models.find("default");
	}


	return m_it;
}

Graphics::shader_iterator Graphics::GetShaderRefByKey(std::string key)
{
	shader_iterator s_it = _shaders.find(key);

	//If not found set basic shader
	if (s_it == _shaders.end())
	{
		std::cout << key << " was not found in shaders container" << std::endl;
		s_it = _shaders.find("tex_shader");
	}

	return s_it;
}

Graphics::tex_iterator Graphics::GetTexRefByKey(std::string key)
{
	tex_iterator t_it = _textures.find(key);

	//If not found, set basic texture
	if (t_it == _textures.end())
	{
		std::cout << key << " was not found in texture container" << std::endl;
		t_it = _textures.find("missing_tex");
	}


	return t_it;
}

unsigned int Graphics::load_cubeMap(const std::vector<std::string>& faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		if (!modelLoader::mLoader::loadCubeMapTextureDDS(faces[i].c_str(), i))
			std::cout << "Cubemap[" + faces[i] + "] not loaded" << std::endl;
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return textureID;
}

void Graphics::draw_all_sprite(const std::vector<renderUI_pair>& render, const std::vector<std::pair<SpriteCom*, EntityID>> spriteList)
{
	if (render.size() == 0)
		return;

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebuffer);
	glDrawBuffers(1, _buffers);

	glClear(GL_DEPTH_BUFFER_BIT);

	GLSLShader& shader = _shaders["ui_shader"];
	glUseProgram(shader.GetHandle());
	shader.Use();
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//usin default standard ratio 1.77
	float aRatio = 1280.0f / 720.0f;

	//glm::mat4 proj = glm::ortho(0.0f,(float)_viewport.w, 0.0f, (float)_viewport.h, -1.0f, 1.0f);
	glm::mat4 proj = glm::ortho(-aRatio, aRatio, -1.0f, 1.0f, -100.0f, 100.0f);
	shader.SetUniform("_projection", proj);

	shader.SetUniform("gammaCorrection", _defaultGamma);

	auto mdl = _models.find("plane");
	glBindVertexArray(mdl->second.vaoid);

	// sort the sprites before rendering
	// ---------------------------------------------
	std::map<float, renderUI_pair> sorted;
	float secretIncrement = 0.0001f;
	for (auto& _sprites : render)
	{
		float val = _sprites.first->pos.z;
		while (sorted.count(val) > 0)
		{
			val += secretIncrement;
		}
		sorted[val] = _sprites;

		//cap 0 <= barPercentage <= 100
		if (_sprites.second->barPercentage < 0)
			_sprites.second->barPercentage = 0;
		if (_sprites.second->barPercentage > 100)
			_sprites.second->barPercentage = 100;
	}


	for (auto& pair : sorted)
	{
		if (/*Engine::_isPlay && */pair.second.second->_isBar)
			mdl->second.BAR_update_uv(pair.second.second->barPercentage / 100.0f);
		draw_sprite(pair.second, shader);
		glDrawArrays(mdl->second.primitive_type, 0, mdl->second.draw_cnt);

		if (/*Engine::_isPlay && */pair.second.second->_isBar)
			mdl->second.reset_uv();
	}

	glBindVertexArray(0);

	shader.UnUse();
	glEnable(GL_DEPTH_TEST);

	//if playing cutscene don't render text (for now)
	if (!_playingCutscene)
		draw_texts(&proj[0][0]);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//void Graphics::draw_sprite(const std::shared_ptr<SpriteCom> Sprites, const std::shared_ptr<UICom> UI)
void Graphics::draw_sprite(const renderUI_pair& rPair, GLSLShader& shader)
{
	glm::mat4 model = glm::identity<mat4>();

	auto trans = rPair.first;
	auto sprite = rPair.second;
	static float reductionSize = 100.0f;
	auto scal = trans->scale;
	auto pos = trans->pos;
	auto color = sprite->_colour.Vec3toGlmVec3();

	size_t spriteCheck = Engine::_ecs.getSystem<AnimationSystem>()->allSpriteSheets.count(sprite);

	float barPercentage = 1.0f;
	//x-axis bar
	if (sprite->_isBar)
	{
		barPercentage = sprite->barPercentage / 100.0f;
		float barOffsetAmount = (1.0f - barPercentage) * sprite->barXAmount;
		pos.x -= barOffsetAmount;

	}
	/*if (sprite->barXAmount > 0.0f)
	{
		std::cout << "bar offset: " << barOffsetAmount << std::endl;
		std::cout << "bar before: " <<pos.x << std::endl;
		std::cout << "bar After: " << pos.x << std::endl;
	}*/
	scal /= reductionSize;
	pos /= reductionSize;

	model = glm::scale(model, glm::vec3(scal.x * barPercentage, scal.y, 1.0f));
	model = glm::rotate(model, glm::radians(trans->rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
	//model = glm::translate(model, glm::vec3(pos.x,pos.y, pos.z));
	model = glm::translate(model, glm::vec3(pos.x, pos.y, 0.0f));

	float* pModel = (float*)glm::value_ptr(model);
	glm::mat4 i{ 0.f,0.f,0.f,0.f,
					0.f,0.f,0.f,0.f,
					0.f,0.f,0.f,0.f,
					pModel[12] - pos.x, pModel[13] - pos.y,0.f,0.f
	};

	model = model - i;

	glm::vec4 texOffsetScale = glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f };

	if (sprite->_isAnimation)
	{
		AnimationClip* clip = nullptr;
		texOffsetScale = glm::vec4(sprite->_u, sprite->_v, 1.0f, 1.0f);
		//if (sprite->_animatorIndex > -1 && sprite->_animatorIndex < Animator::_animatorCont.size())
		if (spriteCheck > 0)
		{
			clip = &(*Engine::_ecs.getSystem<AnimationSystem>()->allSpriteSheets[sprite]);
			sprite->_u = clip->GetCurrentFrameX();
			sprite->_v = clip->GetCurrentFrameY();
			texOffsetScale.z = (float)clip->_numOfSpritesRow;
			texOffsetScale.w = (float)clip->_numOfSpritesCol;
		}
		else
		{
			AnimationClip a = Engine::_ecs.getSystem<AnimationSystem>()->_animatorCont[sprite->SpriteName];
			a._clipSpeed = sprite->_clipSpeed;
			Engine::_ecs.getSystem<AnimationSystem>()->allSpriteSheets[&(*sprite)] = std::make_shared<AnimationClip>(a);
		}


		if (clip)
		{

		}
		shader.SetUniform("spriteAnim", true);
	}
	shader.SetUniform("texOffsetScale", texOffsetScale);

	shader.SetUniform("model", model);
	shader.SetUniform("UseSprite", sprite->_useSprite);
	shader.SetUniform("spriteColor", color);

	auto tex = _textures.find(sprite->SpriteName);
	if (tex != _textures.end())
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex->second.texhandle);
	}

	//set alpha
	shader.SetUniform("Alpha", sprite->_alphaVal);
	//set brightness
	shader.SetUniform("brightVal", sprite->_brightVal);
}

void Graphics::draw_sprite3D(const renderUI_pair& rPair, GLSLShader& shader)
{
	auto& tCom = rPair.first;
	auto& sprite = rPair.second;
	auto scal = tCom->scale;
	auto pos = tCom->pos;

	size_t spriteCheck = Engine::_ecs.getSystem<AnimationSystem>()->allSpriteSheets.count(sprite);

	float barPercentage = 1.0f;
	//x-axis bar
	if (sprite->_isBar)
	{
		barPercentage = sprite->barPercentage / 100.0f;
		float barOffsetAmount = (1.0f - barPercentage) * sprite->barXAmount;
		pos.x -= barOffsetAmount;

	}

	glm::mat4 model = glm::identity<mat4>();

	model = glm::scale(model, glm::vec3(scal.x * barPercentage, scal.y, 1.0f));
	model = glm::rotate(model, glm::radians(tCom->rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));
	float* pModel = (float*)glm::value_ptr(model);


	glm::mat4 i{ 0.f,0.f,0.f,0.f,
					0.f,0.f,0.f,0.f,
					0.f,0.f,0.f,0.f,
					pModel[12] - pos.x, pModel[13] - pos.y,0.f,0.f
	};
	model = model - i;

	glm::vec4 texOffsetScale = glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f };

	if (sprite->_isAnimation)
	{
		AnimationClip* clip = nullptr;
		texOffsetScale = glm::vec4(sprite->_u, sprite->_v, 1.0f, 1.0f);
		//if (sprite->_animatorIndex > -1 && sprite->_animatorIndex < Animator::_animatorCont.size())
		if (spriteCheck > 0)
		{
			clip = &(*Engine::_ecs.getSystem<AnimationSystem>()->allSpriteSheets[sprite]);
			sprite->_u = clip->GetCurrentFrameX();
			sprite->_v = clip->GetCurrentFrameY();
			texOffsetScale.z = (float)clip->_numOfSpritesRow;
			texOffsetScale.w = (float)clip->_numOfSpritesCol;
		}
		else
		{
			AnimationClip a = Engine::_ecs.getSystem<AnimationSystem>()->_animatorCont[sprite->SpriteName];
			a._clipSpeed = sprite->_clipSpeed;
			Engine::_ecs.getSystem<AnimationSystem>()->allSpriteSheets[&(*sprite)] = std::make_shared<AnimationClip>(a);
		}


		if (clip)
		{

		}
		shader.SetUniform("spriteAnim", true);
	}
	shader.SetUniform("texOffsetScale", texOffsetScale);

	shader.SetUniform("model", model);

	auto tex = _textures.find(sprite->SpriteName);
	if (tex != _textures.end())
	{
		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex->second.texhandle);
	}
	

	shader.SetUniform("brightVal", sprite->_brightVal);
	shader.SetUniform("Alpha", sprite->_alphaVal);
	glm::vec3 clr = sprite->_colour.Vec3toGlmVec3();
	shader.SetUniform("ColourMod", clr);
	shader.SetUniform("UseSprite", true);

}


void Graphics::RenderMiniMap(const std::vector<render_pair>& renderList, const CameraCom& cam)
{
	if (!_renderMinimap)
		return;
	//mini map testing
	GLSLShader& shdr = _shaders["simple_minimap"];
	shdr.Use();
	// Bind to shadowmap's FBO.
	//shadowFBO
	glBindFramebuffer(GL_FRAMEBUFFER, _minimapBuffer);
	glViewport(0, 0, _viewport.w, _viewport.h); // Viewport for the texture.
	glClear(GL_DEPTH_BUFFER_BIT);

	glCullFace(GL_FRONT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	float near_plane = 1.0f;
	static float far_plane = 50.5f;
	static float distance = 80.0f;
	glm::mat4 projMat = glm::ortho(-distance, distance, -distance, distance, near_plane, far_plane);


	// View transformation matrix.

	//fake cam
	glm::vec3 camUP = glm::vec3(0, 0, 1);
	glm::vec3 camFRONT = glm::vec3(0, -1, 0);
	Vector3 campos = cam._pos;
	glm::vec3 camPOS = campos.Vec3toGlmVec3();
	//making a view that looks down from above
	glm::mat4 viewMat = glm::lookAt(
		(-camFRONT) * (far_plane - near_plane) / 2.0f,
		glm::vec3(0, 0, 0), camUP);

	//translating the view
	glm::mat4 transMAT = glm::translate(glm::identity<glm::mat4>(), -camPOS);
	viewMat = viewMat * transMAT;

	glm::mat4 projview = projMat * viewMat;
	shdr.SetUniform("projview", projview);

	// Render function
	for (auto& pair : renderList)
	{
		auto& mCom = pair.second;

		//if (mCom->m_model == "BasicTile" ||
		//	mCom->m_model == "Road_Tile" ||
		//	mCom->m_model == "BendRoadTile" ||
		//	mCom->m_model == "MiddleRoadTile")
		//	continue;

		if (!mCom->m_shader.compare("skinning_shader") &&
			((mCom->m_model.find("Bone") != std::string::npos)))
		{
			continue;
		}

		if (mCom->entityTag != "Enemy"
			&& mCom->entityTag != "Player"
			&& mCom->entityTag != "Helicopter"
			)
		{
			auto& tCom = pair.first;
			//Get obj's model to world transform
			glm::mat4 oScale(glm::scale(glm::identity<glm::mat4>(), tCom->scale.Vec3toGlmVec3())),
				oTranslate(glm::translate(glm::identity<glm::mat4>(), tCom->pos.Vec3toGlmVec3()));

			Vector3 convertedRotate = tCom->rotate * ((float)M_PI / 180.0f);
			convertedRotate.y += (float)M_PI;

			glm::mat4
				xRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.x,glm::vec3(1.0f,0.0f,0.0f)) },
				yRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.y,glm::vec3(0.0f,1.0f,0.0f)) },
				zRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.z,glm::vec3(0.0f,0.0f,1.0f)) };

			glm::mat4 oRotate = zRotate * yRotate * xRotate;// *yRotate* zRotate;
			glm::mat4 mdl_xform = oTranslate * oRotate * oScale;
			shdr.SetUniform("model", mdl_xform);

			if (mCom->m_texture == "unicorn")
			{
				//use colour instead
				glm::vec3 clr = mCom->colour.Vec3toGlmVec3();
				clr = glm::vec3(0, 1, 0);
				shdr.SetUniform("vertex_Clr", clr);
				shdr.SetUniform("useTexture", 0);
			}
			else
			{
				//check texture
				if (mCom->tex_ref == _textures.end())
				{
					//use colour instead
					glm::vec3 clr = mCom->colour.Vec3toGlmVec3();
					shdr.SetUniform("vertex_Clr", clr);
					shdr.SetUniform("useTexture", 0);
				}
				else
				{
					shdr.SetUniform("useTexture", 1);

					// bind textures on corresponding texture units
					auto tex = mCom->tex_ref->second;

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, tex.texhandle);			//gamma correction for _default models carrying sprites
					if (mCom->m_model == "default")
					{
						float newGamma = _defaultGamma - 0.825f;
						shdr.SetUniform("gammaCorrection", newGamma);
					}
					else
						shdr.SetUniform("gammaCorrection", _defaultGamma);

				}
			}


			mCom->_saturation.y = glm::clamp(mCom->_saturation.y, 0.0f, 1.0f);
			glm::vec3 saturation = mCom->_saturation.Vec3toGlmVec3();
			shdr.SetUniform("saturation", saturation);

			GLModel& model = mCom->model_ref->second;
			glBindVertexArray(model.vaoid);

			glDrawElements(model.primitive_type, model.draw_cnt,
				GL_UNSIGNED_SHORT, NULL);
			glBindVertexArray(0);
		}
		else //if it is Player or Enemy
		{
			auto& tCom = pair.first;
			//Get obj's model to world transform
			glm::vec3 mapScale{ 5.0f };
			Vector3 tRotate = tCom->rotate;
			if (mCom->entityTag == "Player")
			{
				mapScale = vec3{ 30.0f,30.0f,0.1f };
				shdr.SetUniform("useTexture", 1);
				tRotate.x += 90.0f;
				// bind textures on corresponding texture units
				auto tex = _textures["PlayerMarker"];

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tex.texhandle);

				//gamma correction for _default models carrying sprites
				float newGamma = _defaultGamma - 0.825f;
				shdr.SetUniform("gammaCorrection", newGamma);
			}
			else //Enemy
			{
				glm::vec3 clr;
				clr = glm::vec3(1, 0, 0);
				shdr.SetUniform("vertex_Clr", clr);
				shdr.SetUniform("useTexture", 0);
				shdr.SetUniform("gammaCorrection", _defaultGamma);
			}


			glm::mat4 oScale(glm::scale(glm::identity<glm::mat4>(), mapScale)),
				oTranslate(glm::translate(glm::identity<glm::mat4>(), tCom->pos.Vec3toGlmVec3()));

			Vector3 convertedRotate = tRotate * ((float)M_PI / 180.0f);
			convertedRotate.y += (float)M_PI;

			glm::mat4
				xRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.x,glm::vec3(1.0f,0.0f,0.0f)) },
				yRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.y,glm::vec3(0.0f,1.0f,0.0f)) },
				zRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.z,glm::vec3(0.0f,0.0f,1.0f)) };

			glm::mat4 oRotate = zRotate * yRotate * xRotate;// *yRotate* zRotate;
			glm::mat4 mdl_xform = oTranslate * oRotate * oScale;
			shdr.SetUniform("model", mdl_xform);

			mCom->_saturation.y = glm::clamp(mCom->_saturation.y, 0.0f, 1.0f);
			glm::vec3 saturation = mCom->_saturation.Vec3toGlmVec3();
			shdr.SetUniform("saturation", saturation);


			GLModel& model = _models["default"];
			glBindVertexArray(model.vaoid);
			glDrawElements(model.primitive_type, model.draw_cnt,
				GL_UNSIGNED_SHORT, NULL);

			glBindVertexArray(0);
		}


	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glCullFace(GL_BACK);
}

std::vector<glm::vec4> Graphics::getFrustumCornersWorldSpace(const glm::mat4& projview)
{
	const auto inv = glm::inverse(projview);

	std::vector<glm::vec4> frustumCorners;
	for (unsigned int x = 0; x < 2; ++x)
	{
		for (unsigned int y = 0; y < 2; ++y)
		{
			for (unsigned int z = 0; z < 2; ++z)
			{
				const glm::vec4 pt = inv * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
				frustumCorners.push_back(pt / pt.w);
			}
		}
	}

	return frustumCorners;
}

std::vector<glm::vec4> Graphics::getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view)
{
	return getFrustumCornersWorldSpace(proj * view);
}

glm::mat4 Graphics::getLightSpaceMatrix(const CameraCom& cam, const float nearPlane, const float farPlane)
{
	const auto proj = glm::perspective(
		glm::radians(cam._m_fov), (float)_viewport.w / (float)_viewport.h, nearPlane,
		farPlane);
	auto view = glm::transpose(cam._view_mtx.Mtx4toGlmMat4());

	const auto corners = getFrustumCornersWorldSpace(proj, view);


	glm::vec3 center = glm::vec3(0, 0, 0);
	for (const auto& v : corners)
	{
		center += glm::vec3(v);
	}
	center /= corners.size();

	auto viewMat = glm::lookAt(-lightLookAt, glm::vec3(0.0f), lightUpVector);
	glm::mat4 lightCenter = glm::translate(glm::identity<glm::mat4>(), -center);

	const auto lightView = viewMat * lightCenter;

	float minX = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::min();
	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::min();
	float minZ = std::numeric_limits<float>::max();
	float maxZ = std::numeric_limits<float>::min();
	for (const auto& v : corners)
	{
		const auto trf = lightView * v;
		minX = std::min(minX, trf.x);
		maxX = std::max(maxX, trf.x);
		minY = std::min(minY, trf.y);
		maxY = std::max(maxY, trf.y);
		minZ = std::min(minZ, trf.z);
		maxZ = std::max(maxZ, trf.z);
	}

	// Tune this parameter according to the scene
	constexpr float zMult = 2.0f;

	if (minZ < 0)
		minZ *= zMult;
	else
		minZ /= zMult;

	if (maxZ < 0)
		maxZ /= zMult;
	else
		maxZ *= zMult;

	const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
	return lightProjection * lightView;
}

std::vector<glm::mat4> Graphics::getLightSpaceMatrices(const CameraCom& cam)
{
	cameraFarPlane = cam._m_far;
	shadowCascadeLevels = std::vector<float>{ cameraFarPlane / 50.0f, cameraFarPlane / 25.0f,
		cameraFarPlane / 10.0f, cameraFarPlane / 2.0f };
	std::vector<glm::mat4> ret;
	for (size_t i = 0; i < shadowCascadeLevels.size() + 1; ++i)
	{
		if (i == 0)
		{
			ret.push_back(getLightSpaceMatrix(cam, cameraNearPlane, shadowCascadeLevels[i]));
		}
		else if (i < shadowCascadeLevels.size())
		{
			ret.push_back(getLightSpaceMatrix(cam, shadowCascadeLevels[i - 1], shadowCascadeLevels[i]));
		}
		else
		{
			ret.push_back(getLightSpaceMatrix(cam, shadowCascadeLevels[i - 1], cameraFarPlane));
		}
	}

	return ret;
}