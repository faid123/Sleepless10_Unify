#include "System.h"
#include "Editor/EditorSystem/EditorSystem.h"

void SystemManager::systemUpdate(float dt)
{
#ifdef SHOW_IMGUI
	auto EditorPtr = std::dynamic_pointer_cast<EditorSystem>(getSystem<EditorSystem>());
#endif

	for (auto const& name : _systemOrder)
	{
#ifdef SHOW_IMGUI
		EditorSystem::SystemTimer systimer(name, EditorPtr.get());
#endif
		if (_systems[name]->_isActive)
			_systems[name]->update(dt);
	}
}

void SystemManager::systemFixedUpdate(float dt, int dtSteps)
{
	int steps = 0;

	do
	{
		if (steps >= 3)
		{
			float multiplier = std::min(60.0f, (float)(dtSteps - steps));
			dt *= multiplier;
		}

		for (auto const& name : _systemOrder)
		{
			if (name.find("Physic") != std::string::npos)
				if (_systems[name]->_isActive)
					_systems[name]->update(dt);
		}

		if (steps >= 3)
			break;

		++steps;
	} while (steps < dtSteps);
}
