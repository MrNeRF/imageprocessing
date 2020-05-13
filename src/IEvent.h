#ifndef IEVENT_H
#define IEVENT_H

#include <Eigen/Dense>

enum class EventType
{
	MOUSE_LEFT_BTN_DRAG,
	MOUSE_MID_BTN_DRAG,
	MOUSE_CLICK,
	MOUSE_WHEEL,
	KEY_PRESS
};

struct IEvent
{
	virtual ~IEvent() = default;
};

struct MouseLeftBtnDragEvent : public IEvent
{
	MouseLeftBtnDragEvent(Eigen::Vector2f startCoordinates, Eigen::Vector2f endCoordinates) : 
		m_startCoordinates{startCoordinates},
		m_endCoordinates{endCoordinates}{};
	Eigen::Vector2f m_startCoordinates;		
	Eigen::Vector2f m_endCoordinates;		
};

struct MouseMidBtnDragEvent : public IEvent
{
	MouseMidBtnDragEvent(Eigen::Vector2f startCoordinates, Eigen::Vector2f endCoordinates) : 
		m_startCoordinates{startCoordinates},
		m_endCoordinates{endCoordinates}{};
	Eigen::Vector2f m_startCoordinates;		
	Eigen::Vector2f m_endCoordinates;		
};

struct MouseWheelEvent : public IEvent
{
	MouseWheelEvent(int xoffset, int yoffset) : m_xoffset{xoffset}, m_yoffset{yoffset} {};
	int m_xoffset = 0;
	int m_yoffset = 0;
};

struct KeyPressEvent : public IEvent
{
	KeyPressEvent(int key, int scancode, int action, int mods)
		: m_key{key}, m_scancode{scancode}, m_action{action}, m_mods{mods} {}
	int m_key;
	int m_scancode;
	int m_action;
	int m_mods;
};

#endif
