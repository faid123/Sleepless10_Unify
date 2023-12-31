#ifndef UI_LOGIC_H
#define UI_LOGIC_H

#include "MathLib/Vector2.h"
#include "Graphics/TransformCom.h"
#include "Graphics/SpriteCom.h"

#include <memory>

//enum Type_UI
//{
//	Health = 0,
//	Ammo,
//	ColourBar
//};

class UI_Logic
{
private:
	//float MaxHealthBarScale;
	//float MaxColourBarScale;
	//float _Health;
	//float MaxHealth;
	//float CamOffSetX;

	
	//bool _IsHealthActive;
	//bool _IsAmmoActive;
	//bool _IsColourBarActive;
	//bool _IsHit;
	//bool _IsHeal;

	//Vector2 _intialPos;

public:
	//components i wanna link to
	std::shared_ptr<TransformCom>	_tComp;
	std::shared_ptr<SpriteCom>		_sComp;

	int type_ui;
	bool _IsUIActive;


	UI_Logic() //:
		//MaxHealthBarScale{ 0.0f },
		//MaxColourBarScale{ 0.0f },
		//_Health{ 100.0f },
		//MaxHealth{ 100.0f }, 
		//CamOffSetX{ 0.0f },
		//_IsUIActive{ true },
		//_IsHealthActive{ true },
		//_IsAmmoActive{ true },
		//_IsColourBarActive{ true },
		//_IsHit { false },
		//_IsHeal { false },
		//_intialPos{0.0f, 0.0f}
	{
	}
	
	/*void UI_Logic_Init();*/
	bool keyActive = false;
	
	/*float getHealth();
	void SetHealthToMax();
	float GetMaxHealth();
	float GetDeductionHealth();

	void  SetMaxHealthBarScale(float scale);
	float GetMaxHealthBarScale();

	void SetCamoffSetX(float posX);
	float GetCamOffSetX();

	bool IsHit();
	void SetIsHit(bool set);

	bool IsHeal();
	void SetIsHeal(bool set);*/

	bool IsUIActive();
	void SetUIActive();

	//bool IsUITypeActive(Type_UI UI);
	//void SetUITypeActive(Type_UI UI);

	void HealthUILogic();
	void AmmoUILogic();
	void ColourUILogic();

	//void SetInitalPos(Vector2 pos);
	//Vector2 GetInitalPos();

	void UIReset();
};

#endif // !UI_LOGIC_H