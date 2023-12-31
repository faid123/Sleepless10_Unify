#include "UI_Logic.h"
#include "System/System.h"
#include "Engine.h"
#include "Input.h"
#include "Graphics/RenderSystem/RenderSystem.h"
#include <iostream>

bool UI_Logic::IsUIActive()
{
	return this->_IsUIActive;
}

void UI_Logic::SetUIActive()
{
	if (this->_IsUIActive)
	{
		this->_IsUIActive = false;
		std::cout << "UI Deactivate" << std::endl;
	}
	else {
		this->_IsUIActive = true;
		std::cout << "UI Activate" << std::endl;
	}
}


void UI_Logic::HealthUILogic()
{
	if (Inputs::get_Key_Triggered(61) && keyActive == false)
	{
		/*if (_Health < 100)
		{
			++_Health;
			this->SetIsHeal(true);
		}*/
		keyActive = true;
		//std::cout << "Health Increase - " << _Health << std::endl;
	} 

	if (Inputs::get_Key_Triggered(45) && keyActive == false)
	{
		/*if (_Health > 0)
		{
			--_Health;
			this->SetIsHit(true);
		}
		else {
			std::cout << "You Die!" << std::endl;
		}*/

		keyActive = true;
		//std::cout << "Health Decrease - " << _Health << std::endl;
	}

	//if (Inputs::get_Key_Triggered(85) && keyActive == false)
	//{
	//	keyActive = true;

	//	if (_IsUIActive == true)
	//	{
	//		_IsUIActive = false;
	//		std::cout << "UI Turn Off" << std::endl;
	//	}
	//	else {
	//		_IsUIActive = true;
	//		std::cout << "UI Turn On" << std::endl;
	//	}
	//}

	if ((Inputs::get_KeyReleaseTrigger(45) || 
		Inputs::get_KeyReleaseTrigger(61)) &&
		keyActive)
	{
		keyActive = false;
	}


}

void UI_Logic::AmmoUILogic()
{

}

void UI_Logic::ColourUILogic()
{

}

//void UI_Logic::SetUITypeActive(Type_UI )
//{
//	return;
//}
//bool UI_Logic::IsUITypeActive(Type_UI UI)
//{
//	switch (UI)
//	{
//	case Type_UI::Health:
//		return this->_IsHealthActive;
//
//	case Type_UI::Ammo:
//		return this->_IsAmmoActive;
//
//	case Type_UI::ColourBar:
//		return this->_IsColourBarActive;
//	default:
//		return false;
//	}
//}
//
//void UI_Logic::SetUITypeActive(Type_UI UI)
//{
//	switch (UI)
//	{
//	case Type_UI::Health:
//		if (this->_IsHealthActive)
//		{
//			this->_IsHealthActive = false;
//			std::cout << "Health Bar UI Deactivate" << std::endl;
//		}
//		else {
//			this->_IsHealthActive = true;
//			std::cout << "Health Bar UI Activate" << std::endl;
//		}
//
//		break;
//	case Type_UI::Ammo:
//		if (this->_IsAmmoActive)
//		{
//			this->_IsAmmoActive = false;
//			std::cout << "Ammo UI Deactivate" << std::endl;
//		}
//		else {
//			this->_IsAmmoActive = true;
//			std::cout << "Ammo UI Activate" << std::endl;
//		}
//
//		break;
//	case Type_UI::ColourBar:
//		if (this->_IsColourBarActive)
//		{
//			this->_IsColourBarActive = false;
//			std::cout << "Colour Bar UI Deactivate" << std::endl;
//		}
//		else {
//			this->_IsColourBarActive = true;
//			std::cout << "Colour Bar  UI Activate" << std::endl;
//		}
//
//		break;
//	}
//}
//float UI_Logic::getHealth()
//{
//	return this->_Health;
//}
//
//float UI_Logic::GetMaxHealth()
//{
//	return this->MaxHealth;
//}
//
//float UI_Logic::GetDeductionHealth()
//{
//	return (this->MaxHealth - this->_Health);
//}
//
//void UI_Logic::SetMaxHealthBarScale(float scale)
//{
//	this->MaxHealthBarScale = scale;
//}
//
//float UI_Logic::GetMaxHealthBarScale()
//{
//	return this->MaxHealthBarScale;
//}
//
//bool UI_Logic::IsHit()
//{
//	return this->_IsHit;
//}
//
//void UI_Logic::SetIsHit(bool set)
//{
//	this->_IsHit = set;
//}
//
//bool UI_Logic::IsHeal()
//{
//	return this->_IsHeal;
//}
//
//void UI_Logic::SetIsHeal(bool set)
//{
//	this->_IsHeal = set;
//}
//
//void UI_Logic::SetCamoffSetX(float posX)
//{
//	this->CamOffSetX = posX;
//}
//
//float UI_Logic::GetCamOffSetX()
//{
//	return this->CamOffSetX;
//}
//
//void UI_Logic::SetInitalPos(Vector2 pos)
//{
//	this->_intialPos = pos;
//}
//
//Vector2 UI_Logic::GetInitalPos()
//{
//	return this->_intialPos;
//}
//
//void UI_Logic::SetHealthToMax()
//{
//	_Health = MaxHealth;
//}