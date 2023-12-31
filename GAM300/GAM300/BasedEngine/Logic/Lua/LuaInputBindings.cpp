#include "Input.h"
int i = 0;
//Input key binding

/// <summary>
/// Binding input keys to use in LUA
/// </summary>
/// <param name="lua">The sol state</param>
void Inputs::RegisterLua(sol::state& lua)
{
	lua.new_usertype<Inputs>("Inputs",

		//---------- Member Functions ----------
		"get_Key", &Inputs::get_Key,
		"get_Key_Triggered", &Inputs::get_Key_Triggered,
		"get_KeyReleaseTrigger", &Inputs::get_KeyReleaseTrigger,
		"get_Mos", &Inputs::get_Mos,
		"get_Mos_Triggered", &Inputs::get_Mos_Triggered,
		"get_MosScroll_Trigger", &Inputs::get_MosScroll_Trigger,
		"get_MosReleaseTrigger", &Inputs::get_MosReleaseTrigger,
		"get_Controller", &Inputs::get_GamePatButton,
		"get_ControllerRelease_Trigger", & Inputs::get_GamePatButton_ReleaseTrigger,
		"get_Controller_Trigger", & Inputs::get_GamePatButton_Trigger,
		"get_GamePad_LJS_X", &Inputs::get_GamePatButton_LeftJoystick_Xaxis,
		"get_GamePad_LJS_Y", & Inputs::get_GamePatButton_LeftJoystick_Yaxis,
		"get_GamePad_RJS_X", & Inputs::get_GamePatButton_RightJoystick_Xaxis,
		"get_GamePad_RJS_Y", & Inputs::get_GamePatButton_RightJoystick_Yaxis,
		"get_GamePad_LT_Y", & Inputs::get_GamePatButton_LeftTrigger,
		"get_GamePad_RT_Y", & Inputs::get_GamePatButton_RightTrigger

		);
}

/// <summary>
/// Binding input enums to use in LUA
/// </summary>
/// <param name="input_table">The name of the table to hold the input keys in LUA</param>
/// <param name="lua">The sol state</param>
void Inputs::bindInputEnum(sol::table& input_table, sol::state& lua)
{
	input_table = lua.create_named_table("InputKey");
	input_table["Q_KEY"] = GLFW_KEY_Q;
	input_table["W_KEY"] = GLFW_KEY_W;
	input_table["S_KEY"] = GLFW_KEY_S;
	input_table["A_KEY"] = GLFW_KEY_A;
	input_table["D_KEY"] = GLFW_KEY_D;
	input_table["E_KEY"] = GLFW_KEY_E;
	input_table["Z_KEY"] = GLFW_KEY_Z;
	input_table["M_KEY"] = GLFW_KEY_M;
	input_table["L_KEY"] = GLFW_KEY_L;
	input_table["K_KEY"] = GLFW_KEY_K;
	input_table["X_KEY"] = GLFW_KEY_X;
	input_table["C_KEY"] = GLFW_KEY_C;
	input_table["P_KEY"] = GLFW_KEY_P;
	input_table["G_KEY"] = GLFW_KEY_G;  //godMode


	input_table["SPACE_KEY"] = GLFW_KEY_SPACE;
	input_table["LEFTSHIFT_KEY"] = GLFW_KEY_LEFT_SHIFT;
	input_table["KEY_1"] = GLFW_KEY_1;
	input_table["KEY_2"] = GLFW_KEY_2;
	input_table["KEY_3"] = GLFW_KEY_3;
	input_table["KEY_4"] = GLFW_KEY_4;
	input_table["KEY_5"] = GLFW_KEY_5;
	input_table["KEY_6"] = GLFW_KEY_6;
	input_table["KEY_7"] = GLFW_KEY_7;
	input_table["KEY_8"] = GLFW_KEY_8;
	input_table["KEY_9"] = GLFW_KEY_9;
	input_table["KEY_0"] = GLFW_KEY_0;
	input_table["LEFTMOUSE_BUTTON"] = GLFW_MOUSE_BUTTON_1;
	input_table["RIGHTMOUSE_BUTTON"] = GLFW_MOUSE_BUTTON_2;
	input_table["TAB_KEY"] = GLFW_KEY_TAB;
	input_table["ENTER_KEY"] = GLFW_KEY_ENTER;
	input_table["ESC_KEY"] = GLFW_KEY_ESCAPE;
}

/// Binding input for controller enums to use in LUA
/// </summary>
/// <param name="input_table">The name of the table to hold the input keys in LUA</param>
/// <param name="lua">The sol state</param>
void Inputs::bindInputControllerEnum(sol::table& input_table, sol::state& lua)
{
	input_table = lua.create_named_table("InputControllerKey");
	input_table["A_BUTTON"] = GLFW_GAMEPAD_BUTTON_A;
	input_table["B_BUTTON"] = GLFW_GAMEPAD_BUTTON_B;
	input_table["X_BUTTON"] = GLFW_GAMEPAD_BUTTON_X;
	input_table["Y_BUTTON"] = GLFW_GAMEPAD_BUTTON_Y;
	input_table["LEFT_BUMPER"] = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER;
	input_table["RIGHT_BUMPER"] = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER;
	input_table["BACK_BUTTON"] = GLFW_GAMEPAD_BUTTON_BACK;
	input_table["START_BUTTON"] = GLFW_GAMEPAD_BUTTON_START;
	input_table["LEFT_THUMB"] = GLFW_GAMEPAD_BUTTON_LEFT_THUMB;
	input_table["RIGHT_THUMB"] = GLFW_GAMEPAD_BUTTON_RIGHT_THUMB;
	input_table["UP_DPAD"] = GLFW_GAMEPAD_BUTTON_DPAD_UP;
	input_table["RIGHT_DPAD"] = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT;
	input_table["DOWN_DPAD"] = GLFW_GAMEPAD_BUTTON_DPAD_DOWN;
	input_table["LEFT_DPAD"] = GLFW_GAMEPAD_BUTTON_DPAD_LEFT;

}