#pragma once
#include <string>

//Event interface, all events to inherit from this class
struct iEvent
{
	virtual ~iEvent() {}

	//Cast to a specified type
	template <typename EventType>
	EventType castTo() const
	{
		return static_cast<const EventType&>(*this);
	}
};

//This is a test event example; inherit from Event struct
struct ButtonClickEvent : iEvent
{
	//This is where all the data members of the event are declared
	std::string msg;
	//Make sure to create a constructor for the event and initialize all data member through initializer list
	ButtonClickEvent(std::string _msg) : msg{ _msg } {}
};