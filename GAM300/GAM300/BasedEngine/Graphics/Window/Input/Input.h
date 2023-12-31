/******************************************************************************
* \file			Input.h
* \brief		Contain all the Input related function
* \author 		Ang, Pheng Siang , 100% Code Contribution
*
* \copyright	Copyright (C) 2021 DigiPen Institute of Technology.
				Reproduction or disclosure of this file or its contents without the
				prior written consent of DigiPen Institute of Technology is prohibited.
 /******************************************************************************/
#pragma once

#include <GL/glew.h> // for access to OpenGL API declarations 
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <glm/glm.hpp>
#include <vector>
#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"
/*****************************************************************************/
/*
 \fn class Camera
 \brief Camera class with all the needed function
*/
/*****************************************************************************/
class Inputs
{
public:
	/*****************************************************************************/
	/*
	 \fn static void key_Callback(GLFWwindow* pwin, int key, int scancode, int action, 
									int mod);
	 \brief For engine to know when any key in the keyboard is pressed
	*/
	/*****************************************************************************/
	static void key_Callback(GLFWwindow* pwin, int key, int scancode, int action, int mod);
	/*****************************************************************************/
	/*
	 \fn static void mouseButton_Callback(GLFWwindow* pwin, int button, int action, 
	 int mod);
	 \brief For engine to know when any is mouse click
	*/
	/*****************************************************************************/
	static void mouseButton_Callback(GLFWwindow* pwin, int button, int action, int mod);
	/*****************************************************************************/
	/*
	 \fn static void mouseScroll_Callback(GLFWwindow* pwin, double xoffset, 
	 double yoffset);
	 \brief For engine to know when any is mouse scrolling
	*/
	/*****************************************************************************/
	static void mouseScroll_Callback(GLFWwindow* pwin, double xoffset, double yoffset);
	/*****************************************************************************/
	/*
	 \fn static void mousePos_Callback(GLFWwindow* pwin, double xpos, double ypos);
	 \brief For engine to know when any is mouse position
	*/
	/*****************************************************************************/
	static void mousePos_Callback(GLFWwindow* pwin, double xpos, double ypos);
	/*****************************************************************************/
	/*
	 \fn		static bool get_Key(int key_Num);
	 \brief		getter function for the Keyinput
	 \return	return true if the key is pressed on. Else false
	*/
	/*****************************************************************************/
	static bool get_Key(int key_Num);
	/*****************************************************************************/
	/*
	 \fn		static bool get_Key_Triggered(int key);
	 \brief		getter function for the Triggered of Keyinput
	 \return	return True once when Triggered
	*/
	/*****************************************************************************/
	static bool get_Key_Triggered(int key);
	/*****************************************************************************/
	/*
	 \fn		static glm::vec2 get_MosPos();
	 \brief		getter function for the Mouse Position
	*/
	/*****************************************************************************/
	static glm::vec2 get_MosPos();


	static void set_MosPos(int x, int y);
	//static void cursorPos(GLFWwindow* pwin, double xpos, double ypos);


	/*****************************************************************************/
	/*
	 \fn		static bool get_Mos(int mos);
	 \brief		getter function for mouse input
	 \return	return true if the key is pressed on. Else false
	*/
	/*****************************************************************************/
	static bool get_Mos(int mos);
	/*****************************************************************************/
	/*
	 \fn		static bool get_Mos_Triggered(int mos);
	 \brief		getter function for the Triggered of mouse input
	 \par		0 = left, 1 = right, 2 = mid
	 \return	return True once when Triggered
	*/
	/*****************************************************************************/
	static bool get_Mos_Triggered(int mos);
	/*****************************************************************************/
	/*
	 \fn		static double get_MosScroll_Trigger();
	 \brief		getter function for the Triggered of mouse input
	 \return	return 1 when its scroll upwards, -1 when its downwards
	*/
	/*****************************************************************************/
	static double get_MosScroll_Trigger();
	/*****************************************************************************/
	/*
	 \fn		static bool get_Mos_Movement(int move);
	 \brief		getter function to check if mos is moving at the position
	 \return	return true if it is moving that direction
	*/
	/*****************************************************************************/
	static bool get_Mos_Movement(int move);
	/*****************************************************************************/
	/*
	 \fn		static glm::vec2 get_Total_MosMovement();
	 \brief		getter function find how much it move from starting point in the 
				direction
	 \return	distance of from starting point
	*/
	/*****************************************************************************/
	static glm::vec2 get_Total_MosMovement();
	/*****************************************************************************/
	/*
	 \fn		static void reset_TotalMosMovement();
	 \brief		starting point of the distance calculation
	*/
	/*****************************************************************************/
	static void reset_TotalMosMovement();
	/*****************************************************************************/
	/*
	 \fn		static glm::vec2 MosPos_World(glm::mat3 screen2world, glm::vec2 WindowSize);
	 \brief		calculation on making Mouse screen position into world position
	 \return	position of mouse on screen
	*/
	/*****************************************************************************/
	static glm::vec2 MosPos_World(glm::mat3 screen2world, glm::vec2 WindowSize);
	/// <summary>
	/// return true when it mos click is release
	/// </summary>
	/// <param name="mos"></param>
	/// selection of which mos click
	/// <returns></returns>
	static bool get_MosReleaseTrigger(int mos);
	/// <summary>
	/// update all component in for input
	/// </summary>
	static void update();
	/// <summary>
	/// return true when key is release
	/// </summary>
	/// <param name="key"></param>
	/// which key we want to check
	/// <returns></returns>
	/// return true when it is just release
	static bool get_KeyReleaseTrigger(int key);
	static void RegisterLua(sol::state& lua);
	static void bindInputEnum(sol::table& input_table, sol::state& lua);
	static void bindInputControllerEnum(sol::table& input_table, sol::state& lua);

	static int JoystickPresent;

	//static unsigned char get_GamePat(int key);
	static bool get_GamePatButton(int key);
	static bool get_GamePatButton_Trigger(int key);
	static bool get_GamePatButton_ReleaseTrigger(int key);
	static void realase();

	//logitech controller
	static float get_GamePatButton_LeftJoystick_Xaxis();
	static float get_GamePatButton_LeftJoystick_Yaxis();
	static float get_GamePatButton_RightJoystick_Xaxis();
	static float get_GamePatButton_RightJoystick_Yaxis();
	static bool get_GamePatButton_LeftTrigger();
	static bool get_GamePatButton_RightTrigger();

private:
	// keyboard
	static bool Key_List[1024];
	static bool Key_Triggered_List[1024];
	static bool Pre_Key_Triggered_List[1024];
	static bool Key_ReleaseTriggered_List[1024];
	static bool Pre_Key_ReleaseTriggered_List[1024];
	static std::vector<int> Key_Triggered_Vector;
	static std::vector<int> Key_ReleaseTriggered_Vector;
	// mos
	static bool Mos_List[10];
	static bool Mos_Triggered_list[10];
	static bool Mos_ReleaseTriggered_list[10];
	static glm::vec2 MosPos;
	static double Mos_Scroll;
	static double Mos_Scroll_Trigger;
	static double Pre_Mos_Scroll_Trigger;
	static bool Mos_Movement[4];
	static glm::vec2 Pre_MosPos;
	static glm::vec2 Total_MosMovement;
	static bool Pre_Mos_Triggered_list[3];
	static bool Pre_Mos_ReleaseTriggered_list[3];
	static std::vector<int> Mos_Triggered_Vector;
	static std::vector<int> Mos_ReleaseTriggered_Vector;

	static const float* JoystickAxes;
	static GLFWgamepadstate GamePatButton;
	static GLFWgamepadstate GamePatButton_Trigger;
	static GLFWgamepadstate Pre_GamePatButton_Trigger;
	static GLFWgamepadstate GamePatButton_ReleaseTrigger;
	static GLFWgamepadstate Pre_GamePatButton_ReleaseTrigger;
};