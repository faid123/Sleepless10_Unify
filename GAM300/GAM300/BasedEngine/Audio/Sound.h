/**********************************************************************************
* \file Sound.h
* \brief File contains the function definitions for all functions that are used
		 in Sound.
* \author Kenneth Yeo, 100% Code Contribution
*
* \copyright Copyright (C) 2022 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include <fmod.hpp>
#include <Serializer/ISerializer.h>

enum SoundType
{
	BGM,
	SFX,
	SFX_3D,
	UI
};

struct Sound
{
	FMOD::Sound* mSound;
	FMOD::Channel* mChannel;

	float volume;
	bool mute;
	int loopcount;
	SoundType mType;

	bool isplaying{ false };
};