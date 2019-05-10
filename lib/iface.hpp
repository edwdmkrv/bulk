#ifndef __IFACE_INCLUDED
#define __IFACE_INCLUDED

#include <utility>
#include <vector>

namespace utl {

template <typename X>
struct ISubscriber;

template <typename X>
struct IPublisher {
	virtual void subscribe(ISubscriber<X> &) = 0;
	virtual void bulkupdate(X const &) noexcept = 0;
	virtual void bulkfinalize() noexcept = 0;

protected:
	virtual ~IPublisher() = default;
};

template <typename X>
struct ISubscriber {
	virtual void update(X const &) = 0;
	virtual void finalize() = 0;

protected:
	virtual ~ISubscriber() = default;
};

template <typename X>
class Subscriber: public ISubscriber<X> {
	void finalize() override {
	}
};

template <typename X>
class Publisher: IPublisher<X> {
	bool finalized{};
	std::vector<ISubscriber<X> *> subscribers;

protected:
	Publisher() noexcept = default;
	Publisher(Publisher &&) noexcept(noexcept(decltype(subscribers){std::declval<decltype(subscribers)>()})) = default;

	~Publisher() override {
		if (!finalized) {
			bulkfinalize();
		}
	}

	void bulkupdate(X const &x) noexcept override {
		for (auto &subscriber: subscribers) {
			try {
				subscriber->update(x);
			} catch (...) {
			}
		}
	}

	void bulkfinalize() noexcept override {
		for (auto &subscriber: subscribers) {
			try {
				subscriber->finalize();
			} catch (...) {
			}
		}

		finalized = true;
	}

public:
	void subscribe(ISubscriber<X> &subscriber) override {
		subscribers.push_back(&subscriber);
	}
};

} // namespace utl

#endif
