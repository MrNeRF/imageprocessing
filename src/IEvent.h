#ifndef IEVENT_H
#define IEVENT_H

#include <Eigen/Dense>

enum class EventType
{
	MOUSEDRAG,
	MOUSECLICK
};

struct IEvent
{
	virtual ~IEvent() = default;
};

struct MouseDragEvent : public IEvent
{
	MouseDragEvent(Eigen::Vector2f startCoordinates, Eigen::Vector2f endCoordinates) : 
		m_startCoordinates{startCoordinates},
		m_endCoordinates{endCoordinates}{};
	Eigen::Vector2f m_startCoordinates;		
	Eigen::Vector2f m_endCoordinates;		
};



#endif
