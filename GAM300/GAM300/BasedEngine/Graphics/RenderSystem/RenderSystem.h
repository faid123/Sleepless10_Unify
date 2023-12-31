/**********************************************************************************
* \file RenderSystem.h
* \brief File contains the function definitions for all functions that are used
		 to provide as an interface for graphics class.
* \author Willy Lau, 100% Code Contribution
*
* \copyright Copyright (C) 2022 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "System/System.h"
#include "../Graphics/Graphics.h"
#include "../UserIntered/UI_System.h"

class RenderSystem : public ISystem
{
public:
	void init() override;
	void load() override;
	void update(float delta_time) override;
	void unload() override;
	void release() override;

	static void RegisterLua(sol::state& lua);

	void Set_windowHandle(GLFWwindow* window);
	void resize_viewport(int w, int h);
	Viewport get_viewport();
	void set_camID(const EntityID&);
	void set_bgClr(const glm::vec3&);
	glm::vec3 get_bgClr();
	

	bool RenderToFrameBuffer(); //get the boolean for rendering to fb

	//To toggle renderbuffer on or off
	void toggle_renderFramebuffer(bool);
	//To toggle rendering of light boxes for debug
	void toggle_renderLightbox(bool);
	
	//set the bool to render minimap
	void toggleMinimap(bool val);
	bool getMinimap(); //check for bool is renderminimap

		//this is to set the toggle to not render text during cutscene (for now)
	void toggleCutscene(bool val);
	bool getCutscene();//check for bool is playingCutscene

	//for gamma values
	void addGamma(float val); //can take negative valuess
	float getGammaVal();
	void setGammaVal(float Val);//for gamma white fade ins

	/*
		Function to change the skybox.
		parameter std::string is the name of the folder!
		make sure the filenames are the same as the folder 
	*/
	void ChangeSkybox(const std::string& folderName);

	std::vector<std::string> GetModels();
	std::vector<std::string> GetShaders();
	std::vector<std::string> GetTextures();

	GLSLShader& GetShader(std::string name);

	void setVideoName(std::string name);
	std::string getVideoName();
	bool hasVideoName();
	void setVideoEndTime(double time);
	void setVideoStart(bool isStart);
	void setPlayVideo(bool isPlay);
	bool getPlayVideo();
	bool getStartVideo();
	void closeVideo();
	void closeVideoNow();
	void setVideoMinimapAfter(bool);
	bool getVideoMinimapAfter();

	//for video fade to black
	bool fadeToBlackOnce;
	bool getFadetoBlack();


	//about to move to private
	Graphics _graphics{};
	UISystem _UISys{};
	//UI_Logic _UILogic{};
private:
	//Entity ID reference for camera object
	//used during updates/rendering only
	EntityID _camID{};
	GLFWwindow* _winPtr{};
};
static bool first_frame = true;