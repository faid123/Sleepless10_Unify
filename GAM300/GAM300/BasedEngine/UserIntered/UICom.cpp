#include "UICom.h"

#include "Engine.h"
#include "UI_System.h"

//#include <algorithm>


void UICom::update()
{


}

void UICom::increaseHealth()
{
}

void UICom::decreaseHealth()
{
}

void UICom::findHealth()
{

}

void UICom::swapToWeapon(WeaponType_UI weapType)
{
	//get 
	if (weaponList.empty())
		findWeapons();

	if (weapType != _currentAmmoType)
	{
		//this is just swapping the draw order for the sprites
		std::swap(weaponList[(WeaponType_UI)_currentAmmoType]->pos.z, weaponList[weapType]->pos.z);
		_currentAmmoType = weapType;
	}
}


void UICom::findWeapons()
{
	weaponList = Engine::_ecs.getSystem<UISystem>()->findWeapons();
}

void UICom::registerToSystem()
{
	//auto sPtr = std::make_shared<UICom>(this);
	auto sPtr = shared_from_this();
	Engine::_ecs.getSystem<UISystem>()->addToList(sPtr, 0);
}

















