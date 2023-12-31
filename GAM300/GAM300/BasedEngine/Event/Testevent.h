#pragma once

#pragma once

#include "System/System.h"
#include "iEventSystem.h"
#include "Input.h"
class TestSystem : public ISystem, public iEventSystem
{
public:
	void init() override;
	void load() override;
	void update(float dt) override;
	void unload() override;
	void release() override;
	void buttonClickHandler(const iEvent& event);
};