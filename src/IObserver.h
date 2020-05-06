#ifndef OBSERVER_H
#define OBSERVER_H

#include "IEvent.h"
#include <memory>

class IObserver
{
	public:
		virtual ~IObserver() = default;
		virtual void onNotify(const EventType&, std::unique_ptr<IEvent>) = 0;
};

#endif
