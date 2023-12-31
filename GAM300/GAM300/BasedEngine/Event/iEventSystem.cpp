#include "iEventSystem.h"


/// <summary>
/// Publish an event
/// </summary>
/// <param name="event">The type of event to publish</param>
void iEventSystem::publishEvent(const iEvent& event)
{
	E_Emitter.publish(event);
}

/// <summary>
/// Clear all IDs from the set of subscribers
/// </summary>
void iEventSystem::clearSubscriberID()
{
	_subscriberIDs.clear();
}