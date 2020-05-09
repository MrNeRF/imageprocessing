#ifndef IOBSERVABLE_H
#define IOBSERVABLE_H

#include <memory>
#include "IEvent.h"
#include "IObserver.h"

class IObservable
{
	public:
		virtual ~IObservable() = default;
		virtual void attach(std::shared_ptr<IObserver>) = 0;
		virtual void detach(std::shared_ptr<IObserver>) = 0;
		virtual void notify(const EventType&, std::unique_ptr<IEvent>) = 0;
};

#endif
