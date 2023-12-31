#include "SpriteAnimation.h"

AnimationClip::AnimationClip(const size_t& numOfFrames, const size_t& rowCount, const size_t& columnCount, const float& clipSpeed,
	int start, int end, std::string name,bool looping) :
	_numOfFrames{ numOfFrames },
	_numOfSpritesRow{ rowCount },
	_rowOffset{ 1.0f / _numOfSpritesRow },
	_numOfSpritesCol{ columnCount },
	_columnOffset{ 1.0f / _numOfSpritesCol },
	_clipSpeed{ clipSpeed },
	_currentFrame{ 0 },
	_fCurrentFrame{ 0.0f },
	_rangeStart{ start },
	_rangeEnd{ end < 0 ? (int)(_numOfFrames - 1) : end },
	_looping{ looping }
{
	spriteName = name;
}

//AnimationClip& AnimationClip::operator=(const AnimationClip& rhs)
//{
//	_numOfFrames = rhs._numOfFrames;
//	_numOfSpritesRow = rhs._numOfSpritesRow;
//	_rowOffset = rhs._rowOffset;
//
//	_numOfSpritesCol = rhs._numOfSpritesCol;
//		_columnOffset = rhs._columnOffset;
//		_clipSpeed = rhs._clipSpeed;
//		_currentFrame = rhs._currentFrame;
//		_fCurrentFrame = rhs._fCurrentFrame;
//		_rangeStart = rhs._rangeStart;
//		_rangeEnd = rhs._rangeEnd;
//		_looping = rhs._looping;
//		spriteName = rhs.spriteName;
//}

float AnimationClip::GetCurrentFrameX()
{
	return (_numOfSpritesRow == 1 ? 1 : _rowOffset * (_currentFrame % _numOfSpritesRow));
}

float AnimationClip::GetCurrentFrameY()
{
	return (1 - _columnOffset * (floorf((float)_currentFrame / _numOfSpritesRow) + 1));
}

float AnimationClip::GetClipSpeed()
{
	return _clipSpeed;
}

void AnimationClip::ChangeClipSpeed(const float& newSpeed)
{
	_clipSpeed = newSpeed;
}

