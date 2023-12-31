#include "RenderSystem.h"
#include "Engine.h"
#include "Editor/EditorSystem/EditorSystem.h"
#include "Animation/AnimationSystem.h"
#include "UserIntered/UICom.h"
#include <Graphics/FreeType/text.h>

void RenderSystem::init()
{
	auto windowSystem = Engine::_ecs.getSystem<WindowSystem>();
	Set_windowHandle(windowSystem->getWindowPtr());
	resize_viewport(windowSystem->getWindowWidth(), windowSystem->getWindowHeight());
	_graphics.init();
	set_bgClr(glm::vec3(0.3f));
	fadeToBlackOnce = false;
}

void RenderSystem::load()
{
	init_texts();

	//plane = plane
	//default = cube
	//unicorn = unicorn

   //Engine::_ecs.addComponentToEntity(
   //	// this is the cube
   //	Engine::_ecs.getEntityID("Player"),
   //	MaterialCom{ "default","default","unicorn" }
   //);
   //Engine::_ecs.addComponentToEntity(
   //	Engine::_ecs.getEntityID("Floor"),
   //	MaterialCom{ "default","default","none" }
   //);

   //EntityID bt = Engine::_ecs.createNewEntity("bt");
   //EntityID brt = Engine::_ecs.createNewEntity("brt");
   //EntityID mrt = Engine::_ecs.createNewEntity("mrt");
   //EntityID rt = Engine::_ecs.createNewEntity("rt");
   //Engine::_ecs.addComponentToEntity(
   //	bt,
   //	TransformCom
   //	{ 
   //			glm::vec3(-50.0f,20.0f,-80.0f),
   //			glm::vec3(1.0f),
   //			glm::vec3(0.0f),
   //			State::None
   //	});
   //Engine::_ecs.addComponentToEntity(
   //	bt,
   //	MaterialCom{ "BasicTile","default","BasicTile" });


   //Engine::_ecs.addComponentToEntity(
   //	brt,
   //	TransformCom
   //	{
   //			glm::vec3(-40.0f,20.0f,-80.0f),
   //			glm::vec3(1.0f),
   //			glm::vec3(0.0f),
   //			State::None
   //	});
   //Engine::_ecs.addComponentToEntity(
   //	brt,
   //	MaterialCom{ "BendRoadTile","default","BendRoadTile" });

   //Engine::_ecs.addComponentToEntity(
   //	mrt,
   //	TransformCom
   //	{
   //			glm::vec3(-30.0f,20.0f,-80.0f),
   //			glm::vec3(1.0f),
   //			glm::vec3(0.0f),
   //			//State::None
   //	});
   //Engine::_ecs.addComponentToEntity(
   //	mrt,
   //	MaterialCom{ "building_test1","default","building_test1" });

   //Engine::_ecs.addComponentToEntity(
   //	rt,
   //	TransformCom
   //	{
   //			glm::vec3(-20.0f,20.0f,-80.0f),
   //			glm::vec3(1.0f),
   //			glm::vec3(0.0f),
   //			State::None
   //	});
   //Engine::_ecs.addComponentToEntity(
   //	rt,
   //	MaterialCom{ "Road_Tile","default","Road_Tile" });



   //setCamID(Engine::_ecs.getEntityID("Camera"));

   /*glm::vec3 pointLightPositions[4] = {
	   glm::vec3(0.7f,  0.2f,  2.0f),
	   glm::vec3(2.3f, -3.3f, -4.0f),
	   glm::vec3(-4.0f,  2.0f, -12.0f),
	   glm::vec3(0.0f,  0.0f, -3.0f)
   };*/

   //EntityID pt_lights[4];

   //for (unsigned i = 0; i < 4; ++i)
   //{
   //	pt_lights[i] =Engine::_ecs.createNewEntity("Lights" + std::to_string(i));

   //	Engine::_ecs.addComponentToEntity(
   //		pt_lights[i],
   //		TransformCom{
   //			pointLightPositions[i],
   //			glm::vec3(4.0f),
   //			glm::vec3(0.5f),
   //			State::None
   //		});
   //	Engine::_ecs.addComponentToEntity(
   //		pt_lights[i],
   //		LightCom{ LightCom::point }
   //	);
   //}

   //for (auto const& entity : _entities)
   //{
   //	auto ui = Engine::_ecs.getComponent<UICom>(entity);
   //	if (ui != nullptr)
   //	{
   //		auto sprite = Engine::_ecs.getComponent<SpriteCom>(entity);
   //		if (sprite != nullptr)
   //		{
   //			if (sprite->SpriteName == "HealthRedBar2")
   //			{
   //				sprite->_pos = { 600.0f, 100.0f };
   //				sprite->_scale = { 250.0f, 50.0f };
   //			}
   //		}
   //	}
   //}

   //EntityID _HealthBar = Engine::_ecs.createNewEntity("HealthBar");
   //Engine::_ecs.addComponentToEntity(
   //	_HealthBar,
   //	SpriteCom
   //	{
   //		Vector2(600.0f, 100.0f),
   //		Vector2(250.0f, 50.0f),
   //		float(0.0f),
   //		"HealthRedBar2"
   //	}
   //);
   //Engine::_ecs.addComponentToEntity(
   //	_HealthBar,
   //	UICom{
   //		(int)UI_State::Default
   //	}
   //);
}

void RenderSystem::update(float delta_time)
{
	if (!_graphics.videoplaying)
	{
		//adjusting gamma value
		/*if (Inputs::get_Key_Triggered(GLFW_KEY_RIGHT_BRACKET))
			addGamma(0.2f);
		if (Inputs::get_Key_Triggered(GLFW_KEY_LEFT_BRACKET))
			addGamma(-0.2f);*/

		std::shared_ptr<CameraCom> cam = nullptr;
		switch (Engine::_ecs.getSystem<CameraSystem>()->_camType)
		{
		case CameraSystem::CameraType::EDITOR:
			cam = Engine::_ecs.getComponent<CameraCom>(
				Engine::_ecs.getEntityID("Camera"));
			break;
		case CameraSystem::CameraType::PLAYER:
			cam = Engine::_ecs.getSystem<CameraSystem>()->getPlayerCamera();
			break;
		default:
			break;
		}

		std::vector<Graphics::render_pair> renderList;

		//std::vector<std::shared_ptr<TransformCom>> transList;
		//std::vector<std::shared_ptr<MaterialCom>> materialList;
		std::vector<std::shared_ptr<LightCom>> lightList;
		std::vector<std::shared_ptr<iRigid>> rigidList;
		//UI List for sprites rendered in 2d screen
		std::vector<Graphics::renderUI_pair> renderUIList;
		//UI List for sprites rendered in 3D screen
		std::vector<Graphics::renderUI_pair> render3DUIList;
		//list for sprite sheets 
		std::vector<std::pair<SpriteCom*, EntityID>> renderSpriteList;

		unsigned selected = 0; //for highlighting rigidbody
		for (auto const& entity : _entities)
		{
			auto transform = Engine::_ecs.getComponent<TransformCom>(entity);
			if (transform != nullptr)
			{
				auto lights = Engine::_ecs.getComponent<LightCom>(entity);
				if (lights == nullptr)
				{
					//not a light
					auto sprite = Engine::_ecs.getComponent<SpriteCom>(entity);
					if (sprite == nullptr)
					{
						//Not UI
						auto material = Engine::_ecs.getComponent<MaterialCom>(entity);
						if (material != nullptr)
						{
							material->entityID = entity;
							renderList.push_back(
								std::make_pair(transform, material));
						}

						//Finding for rigid body
						if (EditorSystem::_RenderObjAABB)
						{
							std::shared_ptr<iRigid> rigPtr;
							if (Engine::_ecs.hasComponent<RigidDynamicCom>(entity))
							{
								if (EditorSystem::_selectedEntity == entity)
									selected = (unsigned)rigidList.size();
								rigPtr = Engine::_ecs.getComponent<RigidDynamicCom>(entity);
								rigidList.push_back(rigPtr);
							}
							else if (Engine::_ecs.hasComponent<RigidStaticCom>(entity))
							{
								if (EditorSystem::_selectedEntity == entity)
									selected = (unsigned)rigidList.size();
								rigPtr = Engine::_ecs.getComponent<RigidStaticCom>(entity);
								rigidList.push_back(rigPtr);
							}
						}

					}
					else if (sprite->_drawSprite)
					{
						auto particle = Engine::_ecs.getComponent<ParticleCom>(entity);
						if (particle != nullptr)
						{
							if (particle->isActive == true)
							{
								/*sprite->entityID = (int)entity;
								renderList.push_back(
									std::make_pair(transform, material));*/
								render3DUIList.push_back(std::make_pair(&(*transform), &(*sprite)));
							}
						}
						else
						{
							if (sprite->_drawIn3D)
								render3DUIList.push_back(std::make_pair(&(*transform), &(*sprite)));
							else
							{
								renderUIList.push_back(std::make_pair(&(*transform), &(*sprite)));
								renderSpriteList.push_back(std::make_pair(&(*sprite), entity));
							}
						}

					}

				}
				else
				{
					lights->position = glm::vec3(transform->pos.x, transform->pos.y, transform->pos.z);
					lights->direction = glm::vec3(transform->rotate.x, transform->rotate.y, transform->rotate.z);
					lightList.push_back(lights);
				}
			}
		}
		for (auto& entity : ParticleSystem::_emitterList)
		{
			for (auto& particleData : ParticleSystem::_particleData[entity.first])
			{
				auto particle = &particleData._particleCom;

				if (particle->isActive == true)
				{
					/*sprite->entityID = (int)entity;
					renderList.push_back(
						std::make_pair(transform, material));*/
						/*std::cout << "entityID: " << entity.first << " x: " << particleData._transformCom.pos.x
							<< " y: " << particleData._transformCom.pos.z
							<< " z: " << particleData._transformCom.pos.z << std::endl;*/
					render3DUIList.push_back(std::make_pair((&particleData._transformCom), (&particleData._spriteCom)));
				}
			}

		}
		_graphics.sendBufferData(*cam);

		if (EditorSystem::_RenderObjWireframe)
			_graphics.update(delta_time, *cam, renderList); //wireframe drawing
		/*else if (_graphics.videoplaying)
			_graphics.update_v5();*/
		else
			_graphics.update_v4(delta_time, *cam, renderList, lightList);

		if (EditorSystem::_RenderObjAABB)
			_graphics.update_AABB(delta_time, *cam, rigidList, selected);


		//lightbox check
		toggle_renderLightbox(EditorSystem::_RenderLightBox);



		//render sprites in 3d space
		_graphics.render_sprite3D(*cam, render3DUIList);

		//render all sprites
		_graphics.draw_all_sprite(renderUIList, renderSpriteList);
	}
	else
	{
		_graphics.update_v5(delta_time);
	}


	//Render into the screen
	_graphics.render_frameBuffer();

	//glfwSwapBuffers(_winPtr);
}


void RenderSystem::ChangeSkybox(const std::string& folderName)
{
	_graphics.ChangeSkybox(folderName);
}

void RenderSystem::unload()
{
	free_text();
}

void RenderSystem::release()
{
	_graphics.exit();
}

void RenderSystem::Set_windowHandle(GLFWwindow* window)
{
	_winPtr = window;
	_graphics.SetWindow_Handle(window);
}

void RenderSystem::resize_viewport(int w, int h)
{
	_graphics.resize_viewport(w, h);
	_UISys.resizeWindow();
}

Viewport RenderSystem::get_viewport()
{
	return _graphics.getViewport();
}

void RenderSystem::set_camID(const EntityID& camId)
{
	_camID = camId;
}

void RenderSystem::set_bgClr(const glm::vec3& clr)
{
	_graphics.set_bgClr(clr);
}

glm::vec3 RenderSystem::get_bgClr()
{
	return _graphics.get_bgClr();
}

void RenderSystem::toggle_renderFramebuffer(bool val)
{
	_graphics.RenderToFrameBuffer(val);
}

void RenderSystem::toggle_renderLightbox(bool val)
{
	_graphics.RenderLightbox(val);
}

void RenderSystem::toggleMinimap(bool val)
{
	_graphics.toggleMinimap(val);
}

bool RenderSystem::getMinimap()
{
	return _graphics.toggleMinimap();
}

void RenderSystem::toggleCutscene(bool val)
{
	_graphics.toggleCutscene(val);
}

bool RenderSystem::getCutscene()
{
	return _graphics.toggleCutscene();
}

void RenderSystem::addGamma(float val)
{
	_graphics.AddGammaValue(val);
}

float RenderSystem::getGammaVal()
{
	return _graphics.getGammaValue();
}

void RenderSystem::setGammaVal(float val)
{
	_graphics.setGammaVal(val);
}

bool RenderSystem::RenderToFrameBuffer()
{
	return _graphics.RenderToFrameBuffer();
}

std::vector<std::string> RenderSystem::GetModels()
{
	return _graphics.GetModels();
}

std::vector<std::string> RenderSystem::GetShaders()
{
	return _graphics.GetShaders();
}
std::vector<std::string> RenderSystem::GetTextures()
{
	return _graphics.GetTextures();
}

GLSLShader& RenderSystem::GetShader(std::string name)
{
	return _graphics.get_shader(name);
}

void RenderSystem::setVideoName(std::string name)
{
	_graphics.videoName = name;
	if (!video_reader_open(&_graphics.vr_state, std::string("Resources/Videos/" + _graphics.videoName).c_str())) 
	{
		printf("Couldn't open video file (make sure you set a video file that exists)\n");
	}
	//_graphics.initVideoTexture();
}

std::string RenderSystem::getVideoName()
{
	return _graphics.videoName;
}

bool RenderSystem::hasVideoName()
{
	if (_graphics.videoName.size() <= 0)
	{
		return false;
	}
	return true;
}

void RenderSystem::setVideoEndTime(double time)
{
	_graphics.videoEndTime = time;
}

void RenderSystem::setVideoStart(bool isStart)
{
	_graphics.videoStart = isStart;
}

void RenderSystem::setPlayVideo(bool isPlay)
{
	if (isPlay == false)
	{
		_graphics.videoplaying = isPlay;
	}
	else if (fadeToBlackOnce == true || _graphics.videoName == "intro.mp4")
	{
		_graphics.videoplaying = isPlay;
		fadeToBlackOnce = false;
	}
	else //if (fadeToBlackOnce == false)
	{
		uint screenID = Engine::_ecs.getPrefabID("Prefab_simple_FadeIn_Screen");
		Engine::_ecs.instantiatePrefab(screenID, "simple_FadeIn_Screen");
		fadeToBlackOnce = true;
	}
}

bool RenderSystem::getPlayVideo()
{
	if (_graphics.videoplaying || fadeToBlackOnce)
		return true;
	else
		return false;
}

bool RenderSystem::getStartVideo()
{
	return _graphics.videoStart;
}

void RenderSystem::closeVideo()
{
	video_reader_close(&(_graphics.vr_state));
}

void RenderSystem::closeVideoNow()
{
	Engine::_ecs.getSystem<UISystem>()->_playingCutscene = false;
	auto RENDERSYSTEM = Engine::_ecs.getSystem<RenderSystem>();
	RENDERSYSTEM->setVideoStart(false);
	RENDERSYSTEM->setPlayVideo(false);
	first_frame = true;
	video_reader_close_now(&(_graphics.vr_state));
}

void RenderSystem::setVideoMinimapAfter(bool val)
{
	_graphics.showMinimapAfter = val;
}

bool RenderSystem::getVideoMinimapAfter()
{
	return _graphics.showMinimapAfter;
}

bool RenderSystem::getFadetoBlack()
{
	return fadeToBlackOnce;
}
