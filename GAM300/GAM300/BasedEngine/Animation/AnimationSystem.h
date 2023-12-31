/**********************************************************************************
* \file AnimationSystem.h
* \brief File contains the function definitions for all functions that are used
		 to control all the 2d and 3d animations.
* \author Zhang Xinyu, 100% Code Contribution
*
* \copyright Copyright (C) 2022 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "AnimationCom.h"
#include "SpriteAnimation.h"

struct CameraCom;
struct SpriteCom;
class AnimationSystem : public ISystem
{
	//Entity ID reference for camera object
	//used during updates/rendering only
	EntityID _camID{};
public:
	std::map<EntityID, std::shared_ptr<Animator>> allAnimators{};
	std::map<SpriteCom*, std::shared_ptr<AnimationClip>> allSpriteSheets{};
	std::map<std::string, AnimationClip> _animatorCont;

	void SpriteInit();
	void SpriteUpdate(float);

	void addAnimatorToEntity();
	void updateAnimationCom(EntityID eid);
	void updateAnimators(float);

	void init();
	void load();
	void update(float delta_time);
	void unload() {}
	void release() {}
};

