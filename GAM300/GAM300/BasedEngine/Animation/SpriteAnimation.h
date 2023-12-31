/**********************************************************************************
* \file SpriteAnimation.h
* \brief File contains the function definitions for all functions that are used
         to 2d animation.
* \author Zhang Xinyu, 100% Code Contribution
*
* \copyright Copyright (C) 2022 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include <string>
#include <vector>
#include <map>
#include "System/System.h"

struct AnimationClip
{
    float _clipSpeed;
    size_t _numOfFrames;
    size_t _numOfSpritesRow;    // HOW MANY SPRITES IN 1 ROW
    float _rowOffset;
    size_t _numOfSpritesCol;    // HOW MANY SPRITES IN 1 COLUMN
    float _columnOffset;
    size_t _currentFrame;
    float _fCurrentFrame;

    int _rangeStart, _rangeEnd;

    bool _looping;
    std::string spriteName = "";

    AnimationClip() = default;
    // Custom Constructor
    AnimationClip(const size_t& numOfFrames, const size_t& rowCount, const size_t& columnCount, const float& clipSpeed = 1,
        int start = 0, int end = -1, std::string name="", bool looping = true);
    AnimationClip& operator=(const AnimationClip&) = default;
    ~AnimationClip() = default;

    float GetCurrentFrameX();
    float GetCurrentFrameY();
    float GetClipSpeed();
    void ChangeClipSpeed(const float& newSpeed);
};




           