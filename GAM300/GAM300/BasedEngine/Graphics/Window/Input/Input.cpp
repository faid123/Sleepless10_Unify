/******************************************************************************
* \file			Input.cpp
* \brief		Contain all the Input related function
* \author 		Ang, Pheng Siang , 100% Code Contribution
*
* \copyright	Copyright (C) 2021 DigiPen Institute of Technology.
				Reproduction or disclosure of this file or its contents without the
				prior written consent of DigiPen Institute of Technology is prohibited.
 /******************************************************************************/
#include "Input.h"
#include "../Time/Time.h"
#include "Engine.h"
#include "../Window/Window.h"


 // keyboard
bool Inputs::Key_Triggered_List[1024]{};
bool Inputs::Key_List[1024]{};
bool Inputs::Pre_Key_Triggered_List[1024]{ false };
std::vector<int> Inputs::Key_Triggered_Vector;
bool Inputs::Key_ReleaseTriggered_List[1024]{ false };
bool Inputs::Pre_Key_ReleaseTriggered_List[1024]{ false };
std::vector<int> Inputs::Key_ReleaseTriggered_Vector;
// mos
glm::vec2 Inputs::MosPos;
bool Inputs::Mos_List[10]{ false };
bool Inputs::Mos_Triggered_list[10]{ false };
bool Inputs::Mos_ReleaseTriggered_list[10]{ false };
double Inputs::Mos_Scroll = 0.0;
double Inputs::Mos_Scroll_Trigger = 0.0;
double Inputs::Pre_Mos_Scroll_Trigger = 0.0;
bool Inputs::Mos_Movement[4];
glm::vec2 Inputs::Pre_MosPos;
glm::vec2 Inputs::Total_MosMovement{ 0.0f };
bool Inputs::Pre_Mos_Triggered_list[3]{ false };
bool Inputs::Pre_Mos_ReleaseTriggered_list[3]{ false };
std::vector<int> Inputs::Mos_Triggered_Vector;
std::vector<int> Inputs::Mos_ReleaseTriggered_Vector;

//controller
int Inputs::JoystickPresent = 0;
const float* Inputs::JoystickAxes = nullptr;
GLFWgamepadstate Inputs::GamePatButton{};
GLFWgamepadstate Inputs::GamePatButton_Trigger{};
GLFWgamepadstate Inputs::Pre_GamePatButton_Trigger{};
GLFWgamepadstate Inputs::GamePatButton_ReleaseTrigger{};
GLFWgamepadstate Inputs::Pre_GamePatButton_ReleaseTrigger{};


#define right 0
#define left 1
#define up 2
#define down 3
#define x_axis 0
#define y_axis 1
/*****************************************************************************/
/*
 \fn static void key_Callback(GLFWwindow* pwin, int key, int scancode, int action,
								int mod);
 \brief For engine to know when any key in the keyboard is pressed
*/
/*****************************************************************************/
void Inputs::key_Callback(GLFWwindow*, int key, int, int action, int)
{
	if (key > 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
#ifdef _DEBUG
			//std::cout << "Key pressed " << key << std::endl;
#endif
			Key_Triggered_List[key] = true;
			Key_Triggered_Vector.push_back(key);
		}
		else if (action == GLFW_RELEASE)
		{
			Key_ReleaseTriggered_List[key] = true;
			Key_ReleaseTriggered_Vector.push_back(key);
		}
		Key_List[key] = (bool)action;
	}
}
/*****************************************************************************/
/*
 \fn static void mouseButton_Callback(GLFWwindow* pwin, int button, int action,
 int mod);
 \brief For engine to know when any is mouse click
*/
/*****************************************************************************/
void Inputs::mouseButton_Callback(GLFWwindow*, int button, int action, int)
{
	// which button
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
#ifdef _DEBUG
		//std::cout << "Left mouse button ";
#endif
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
#ifdef _DEBUG
		//std::cout << "Right mouse button ";
#endif
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
#ifdef _DEBUG
		//std::cout << "Mid mouse button ";
#endif
		break;
	}

	// if trigger
	switch (action) {
	case GLFW_PRESS:
#ifdef _DEBUG
		//std::cout << "pressed!!!" << std::endl;
#endif
		Mos_Triggered_list[button] = (bool)action;
		Mos_Triggered_Vector.push_back(button);
		break;
	case GLFW_RELEASE:
#ifdef _DEBUG
		//std::cout << "released!!!" << std::endl;
#endif
		Mos_ReleaseTriggered_list[button] = true;
		Mos_ReleaseTriggered_Vector.push_back(button);
		break;
	}
	Mos_List[button] = (bool)action;
}
/*****************************************************************************/
/*
 \fn static void mouseScroll_Callback(GLFWwindow* pwin, double xoffset,
 double yoffset);
 \brief For engine to know when any is mouse scrolling
*/
/*****************************************************************************/
void Inputs::mouseScroll_Callback(GLFWwindow*, double xoffset, double yoffset)
{
#ifdef _DEBUG
	//std::cout << "Mouse scroll wheel offset: ("
		//<< xoffset << ", " << yoffset << ")" << std::endl;
#endif
	//unused variable if release
	(void)xoffset;
	Mos_Scroll = yoffset;
	Mos_Scroll_Trigger = yoffset;
}
/*****************************************************************************/
/*
 \fn static void mousePos_Callback(GLFWwindow* pwin, double xpos, double ypos);
 \brief For engine to know when any is mouse position
*/
/*****************************************************************************/
void Inputs::mousePos_Callback(GLFWwindow*, double xpos, double ypos)
{
#ifdef _DEBUG
	//std::cout << "Mouse cursor position: (" << xpos << ", " << ypos << ")" << std::endl;
#endif
	MosPos = glm::vec2((float)xpos, (float)ypos);
	glm::vec2 offset = Pre_MosPos - MosPos;
	if (offset.x < 0.0f)
	{
		Mos_Movement[right] = true;
		Mos_Movement[left] = false;
	}
	if (offset.x > 0.0f)
	{
		Mos_Movement[right] = false;
		Mos_Movement[left] = true;
	}
	if (offset.y < 0.0f)
	{
		Mos_Movement[up] = true;
		Mos_Movement[down] = false;
	}
	if (offset.y > 0.0f)
	{
		Mos_Movement[up] = false;
		Mos_Movement[down] = true;
	}
	if (offset.x == 0 && offset.y == 0)
	{
		Mos_Movement[right] = false;
		Mos_Movement[left] = false;
		Mos_Movement[up] = false;
		Mos_Movement[down] = false;
	}

	Total_MosMovement.x += offset.x;
	Total_MosMovement.y += offset.y;

	Pre_MosPos = MosPos;
}
/*****************************************************************************/
/*
 \fn		static glm::vec2 get_MosPos();
 \brief		getter function for the Mouse Position
*/
/*****************************************************************************/
glm::vec2 Inputs::get_MosPos()
{
	return MosPos;
}
void Inputs::set_MosPos(int x, int y)
{
	/*auto temp =
		GLFWcursorposfun(
			Engine::_ecs.getSystem<WindowSystem>()->getWindowPtr(),
		(double)x, (double)y);
	glfwSetCursorPosCallback(
		Engine::_ecs.getSystem<WindowSystem>()->getWindowPtr(),
		temp
	);*/
	glfwSetCursorPos(Engine::_ecs.getSystem<WindowSystem>()->getWindowPtr(),
		(double)x, (double)y);
}
/*****************************************************************************/
/*
 \fn		static bool get_Mos(int mos);
 \brief		getter function for mouse input
 \param		0 left, 1 right, 2 mid
 \return	return true if the key is pressed on. Else false
*/
/*****************************************************************************/
bool Inputs::get_Mos(int mos)
{
	return Mos_List[mos];
}
/*****************************************************************************/
/*
 \fn		static bool get_Mos_Triggered(int mos);
 \brief		getter function for the Triggered of mouse input
 \param		0 left, 1 right, 2 mid
 \return	return True once when Triggered
*/
/*****************************************************************************/
bool Inputs::get_Mos_Triggered(int mos)
{
	return Mos_Triggered_list[mos];
}
/*****************************************************************************/
/*
 \fn		static double get_MosScroll_Trigger();
 \brief		getter function for the Triggered of mouse input
 \return	return 1 when its scroll upwards, -1 when its downwards
*/
/*****************************************************************************/
double Inputs::get_MosScroll_Trigger()
{
	return Mos_Scroll_Trigger;
}
/*****************************************************************************/
/*
 \fn		static bool get_Mos_Movement(int move);
 \brief		getter function to check if mos is moving at the position
 \return	return true if it is moving that direction
*/
/*****************************************************************************/
bool Inputs::get_Mos_Movement(int move)
{
	return Mos_Movement[move];
}
/*****************************************************************************/
/*
 \fn		static glm::vec2 get_Total_MosMovement();
 \brief		getter function find how much it move from starting point in the
			direction
 \return	distance of from starting point
*/
/*****************************************************************************/
glm::vec2 Inputs::get_Total_MosMovement()
{
	return Total_MosMovement;
}
/*****************************************************************************/
/*
 \fn		static void reset_TotalMosMovement();
 \brief		starting point of the distance calculation
*/
/*****************************************************************************/
void Inputs::reset_TotalMosMovement()
{
	Total_MosMovement[x_axis] = 0.0f;
	Total_MosMovement[y_axis] = 0.0f;
}
/*****************************************************************************/
/*
 \fn		static glm::vec2 MosPos_World(glm::mat3 screen2world, glm::vec2 WindowSize);
 \brief		calculation on making Mouse screen position into world position
 \return	position of mouse on screen
*/
/*****************************************************************************/
glm::vec2 Inputs::MosPos_World(glm::mat3 screen2world, glm::vec2 WindowSize)
{
	// geting pos base on centre of the screen in presentage
	// some how needs to flip the y-axis, not sure the reasoning but yes
	glm::vec3 mosPos_center{
				2.0f * Inputs::get_MosPos().x / WindowSize.x - 1.0f,
				-(2.0f * Inputs::get_MosPos().y / WindowSize.y - 1.0f),
				1.0f
	};
	glm::vec3 MosPos_World_vec3 = screen2world * mosPos_center;
	return glm::vec2(MosPos_World_vec3.x, MosPos_World_vec3.y);
}
/// <summary>
/// return true when it mos click is release
/// </summary>
/// <param name="mos"></param>
/// selection of which mos click
/// <returns></returns>
bool Inputs::get_MosReleaseTrigger(int mos)
{
	return Mos_ReleaseTriggered_list[mos];
}
/// <summary>
/// update all component in for input
/// </summary>
void Inputs::update()
{
	// check release keys set 1 -> 0 
	// check triggered , if 1 ->0 
	// check if on hold 1 ->0 and set release to 1 and triggered to 0
	// logic for mouse triggereed/release
	int counter = 0;
	for (auto& key : Key_Triggered_Vector)
	{
		if (Key_Triggered_List[key] != Pre_Key_Triggered_List[key])
			Pre_Key_Triggered_List[key] = Key_Triggered_List[key];
		else
			Key_Triggered_List[key] = !Key_Triggered_List[key];
		// erasing key that is done
		if (Key_Triggered_List[key] == false && Pre_Key_Triggered_List[key] == false)
		{
			Key_Triggered_Vector.erase(Key_Triggered_Vector.begin() + counter);
			continue;
		}
		++counter;
	}
	counter = 0;
	for (auto& key : Key_ReleaseTriggered_Vector)
	{
		if (Key_ReleaseTriggered_List[key] != Pre_Key_ReleaseTriggered_List[key])
			Pre_Key_ReleaseTriggered_List[key] = Key_ReleaseTriggered_List[key];
		else
			Key_ReleaseTriggered_List[key] = !Key_ReleaseTriggered_List[key];
		// erasing key that is done
		if (Key_ReleaseTriggered_List[key] == false && Pre_Key_ReleaseTriggered_List[key] == false)
		{
			Key_ReleaseTriggered_Vector.erase(Key_ReleaseTriggered_Vector.begin() + counter);
			continue;
		}
		++counter;
	}
	// mos
	// triggered
	counter = 0;
	for (auto& mos : Mos_Triggered_Vector)
	{
		if (Mos_Triggered_list[mos] != Pre_Mos_Triggered_list[mos])
			Pre_Mos_Triggered_list[mos] = Mos_Triggered_list[mos];
		else
			Mos_Triggered_list[mos] = !Mos_Triggered_list[mos];

		if (Mos_Triggered_list[mos] == false && Pre_Mos_Triggered_list[mos] == false)
		{
			Mos_Triggered_Vector.erase(Mos_Triggered_Vector.begin() + counter);
			continue;
		}
		++counter;
	}
	// realase triggered
	counter = 0;
	for (auto& mos : Mos_ReleaseTriggered_Vector)
	{
		if (Mos_ReleaseTriggered_list[mos] != Pre_Mos_ReleaseTriggered_list[mos])
			Pre_Mos_ReleaseTriggered_list[mos] = Mos_ReleaseTriggered_list[mos];
		else
			Mos_ReleaseTriggered_list[mos] = !Mos_ReleaseTriggered_list[mos];

		if (Mos_ReleaseTriggered_list[mos] == false && Pre_Mos_ReleaseTriggered_list[mos] == false)
		{
			Mos_ReleaseTriggered_Vector.erase(Mos_ReleaseTriggered_Vector.begin() + counter);
			continue;
		}
		++counter;
	}

	if (Pre_Mos_Scroll_Trigger != Mos_Scroll_Trigger)
		Pre_Mos_Scroll_Trigger = Mos_Scroll_Trigger;
	else
		Mos_Scroll_Trigger = 0.0;

	JoystickPresent = glfwJoystickPresent(GLFW_JOYSTICK_1);
	if (JoystickPresent == 1)
	{
		int axesCount = 0;
		JoystickAxes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);

		/*int buttonCount = 0;
		GamePatButton = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
		int 20 = sizeof(GamePatButton) / sizeof(GamePatButton[0]);*/
		glfwGetGamepadState(GLFW_JOYSTICK_1, &GamePatButton);
		// triggered
		/*if (GamePatButton_Trigger)
		{*/
			for (int i = 0; i < 20; ++i)
			{
				GamePatButton_Trigger.buttons[i] = GamePatButton.buttons[i];
				if (GamePatButton_Trigger.buttons[i] || Pre_GamePatButton_Trigger.buttons[i])
				{
					if (GamePatButton_Trigger.buttons[i] != Pre_GamePatButton_Trigger.buttons[i])
						Pre_GamePatButton_Trigger.buttons[i] = GamePatButton_Trigger.buttons[i];
					else
						GamePatButton_Trigger.buttons[i] = !Pre_GamePatButton_Trigger.buttons[i];
				}
			}
		//}
		//else
		//{
		//	/*GamePatButton_Trigger = new unsigned char[20];
		//	Pre_GamePatButton_Trigger = new unsigned char[20];*/

		//	for (int i = 0; i < 20; ++i)
		//	{
		//		GamePatButton_Trigger[i] = GamePatButton[i];
		//		Pre_GamePatButton_Trigger[i] = GamePatButton[i];
		//	}
		//}
		//// realase triggered
		//if (GamePatButton_ReleaseTrigger)
		//{
			for (int i = 0; i < 20; ++i)
			{
				GamePatButton_ReleaseTrigger.buttons[i] = !GamePatButton.buttons[i];
				if (GamePatButton_ReleaseTrigger.buttons[i] || Pre_GamePatButton_ReleaseTrigger.buttons[i])
				{
					if (GamePatButton_ReleaseTrigger.buttons[i] != Pre_GamePatButton_ReleaseTrigger.buttons[i])
						Pre_GamePatButton_ReleaseTrigger.buttons[i] = GamePatButton_ReleaseTrigger.buttons[i];
					else
						GamePatButton_ReleaseTrigger.buttons[i] = !Pre_GamePatButton_ReleaseTrigger.buttons[i];
				}
			}
		/*}
		else
		{
			GamePatButton_ReleaseTrigger = new unsigned char[20];
			Pre_GamePatButton_ReleaseTrigger = new unsigned char[20];

			for (int i = 0; i < 20; ++i)
			{
				GamePatButton_ReleaseTrigger[i] = !GamePatButton[i];
				Pre_GamePatButton_ReleaseTrigger[i] = !GamePatButton[i];
			}
		}*/
	}
}
/// <summary>
/// return true when key is release
/// </summary>
/// <param name="key"></param>
/// which key we want to check
/// <returns></returns>
/// return true when it is just release
bool Inputs::get_KeyReleaseTrigger(int key)
{
	return Key_ReleaseTriggered_List[key];
}
/*****************************************************************************/
/*
 \fn		static bool get_Key(int key_Num);
 \brief		getter function for the Keyinput
 \return	return true if the key is pressed on. Else false
*/
/*****************************************************************************/
bool Inputs::get_Key(int key_Num)
{
	return Key_List[key_Num];
}
/*****************************************************************************/
/*
 \fn		static bool get_Key_Triggered(int key);
 \brief		getter function for the Triggered of Keyinput
 \return	return True once when Triggered
*/
/*****************************************************************************/
bool Inputs::get_Key_Triggered(int key_Num)
{
	return Key_Triggered_List[key_Num];
}

//unsigned char Inputs::get_GamePat(int key)
//{
//	return GamePatButton[key];
//}

bool Inputs::get_GamePatButton(int key)
{
	if (JoystickPresent)
		return GamePatButton.buttons[key];
	////std::cout << "dont have have Joystick" << std::endl;
	return false;
}

bool Inputs::get_GamePatButton_Trigger(int key)
{
	if (JoystickPresent)
		return GamePatButton_Trigger.buttons[key];
	////std::cout << "dont have have Joystick" << std::endl;
	return false;
}

bool Inputs::get_GamePatButton_ReleaseTrigger(int key)
{
	if (JoystickPresent)
		return GamePatButton_ReleaseTrigger.buttons[key];
	////std::cout << "dont have have Joystick" << std::endl;
	return false;
}


void Inputs::realase()
{
	/*if (GamePatButton_Trigger)
		delete[] GamePatButton_Trigger;
	if (Pre_GamePatButton_Trigger)
		delete[] Pre_GamePatButton_Trigger;
	if (GamePatButton_ReleaseTrigger)
		delete[] GamePatButton_ReleaseTrigger;
	if (Pre_GamePatButton_ReleaseTrigger)
		delete[] Pre_GamePatButton_ReleaseTrigger;*/
}

float Inputs::get_GamePatButton_LeftJoystick_Xaxis()
{
	if (JoystickPresent)
		return JoystickAxes[0];
	////std::cout << "dont have have Joystick" << std::endl;
	return 0.0f;
}

float Inputs::get_GamePatButton_LeftJoystick_Yaxis()
{
	if (JoystickPresent)
		return JoystickAxes[1];
	////std::cout << "dont have have Joystick" << std::endl;
	return 0.0f;
}

float Inputs::get_GamePatButton_RightJoystick_Xaxis()
{
	if (JoystickPresent)
		return JoystickAxes[2];
	//std::cout << "dont have have Joystick" << std::endl;
	return 0.0f;
}

float Inputs::get_GamePatButton_RightJoystick_Yaxis()
{
	if (JoystickPresent)
		return JoystickAxes[3];
	//std::cout << "dont have have Joystick" << std::endl;
	return 0.0f;
}

bool Inputs::get_GamePatButton_LeftTrigger()
{
	if (JoystickPresent)
		return GamePatButton.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] > 0.0f;
	//std::cout << "dont have have Joystick" << std::endl;
	return false;
}

bool Inputs::get_GamePatButton_RightTrigger()
{
	if (JoystickPresent)
		return GamePatButton.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] > 0.0f;
	//std::cout << "dont have have Joystick" << std::endl;
	return false;
}
