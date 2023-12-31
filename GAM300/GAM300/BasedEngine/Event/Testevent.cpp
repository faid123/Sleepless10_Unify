#include "Testevent.h"

void TestSystem::init()
{
	//TestSystem test;
	subscribeEvent<ButtonClickEvent>([](const iEvent& event) {
		ButtonClickEvent evt = event.castTo<ButtonClickEvent>();
		//std::cout << evt.msg << " was pressed. " << "TestSystem(Lambda) is currently handling the event" << std::endl;
		});
	subscribeEvent<ButtonClickEvent>(std::bind(&TestSystem::buttonClickHandler, this, std::placeholders::_1));
	//publishEvent(ButtonClickEvent("Start Game Button"));
	//unsubscribeEvent<ButtonClickEvent>(handle);
	//unsubscribeAll<ButtonClickEvent>();
}

void TestSystem::load()
{

}

void TestSystem::update(float dt)
{
	(void)dt;
	/*if (Inputs::get_Mos_Triggered(GLFW_MOUSE_BUTTON_LEFT))
		publishEvent(ButtonClickEvent("Start Game Button"));*/
}


void TestSystem::unload()
{
	clearSubscriberID();
}

void TestSystem::release()
{

}

void TestSystem::buttonClickHandler(const iEvent& event)
{
	ButtonClickEvent evt = event.castTo<ButtonClickEvent>();
	//std::cout << evt.msg << " was pressed. " << "TestSystem(bind) is currently handling the event" << std::endl;
}