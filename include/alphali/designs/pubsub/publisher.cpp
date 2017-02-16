#include "publisher.h"
#include <algorithm>

using namespace alphali;



publisher::publisher()
	: pub_death_pub(),
	pub_death_sub(),
	list_subs() {

}



publisher::publisher(const publisher& other) {
	*this = other;
}
publisher& publisher::operator=(const publisher& other) {

	if(this != &other) {

		pub_death_pub = death_publisher();
		pub_death_sub = death_subscriber();
		list_subs = other.list_subs;

		for(auto* p_sub : list_subs) {
			auto fcn_sub_killed = [this, &p_sub]() {
				list_subs.remove(p_sub);
			};
			pub_death_sub.subscribe(
				p_sub->sub_death_pub, std::move(fcn_sub_killed));

			p_sub->pub_copied(other, *this);
		}

	}

	return *this;

}

publisher::publisher(publisher&& other) {
	*this = std::move(other);
}
publisher& publisher::operator=(publisher&& other) {

	if(this != &other) {

		pub_death_pub = death_publisher();
		pub_death_sub = death_subscriber();
		list_subs = std::move(other.list_subs);

		for(auto* pSub : list_subs) {
			auto fcnSubKilled = [this, &pSub]() {
				list_subs.remove(pSub);
			};
			other.pub_death_sub.unsubscribe(pSub->sub_death_pub);
			pub_death_sub.subscribe(
				pSub->sub_death_pub, std::move(fcnSubKilled));

			pSub->pub_moved(other, *this);
		}

	}

	return *this;

}



publisher::~publisher() {

}



void publisher::publish() {

	auto fcn_update = [this](subscriber* sub) {
		sub->update(*this);
	};

	std::for_each(list_subs.begin(), list_subs.end(), fcn_update);

}



void publisher::attach(subscriber& subscriber) {

	auto fcn_sub_killed = [this, &subscriber]() {
		list_subs.remove(&subscriber);
	};
	pub_death_sub.subscribe(subscriber.sub_death_pub, fcn_sub_killed);

	list_subs.emplace_front(&subscriber);

}

void publisher::detach(subscriber& subscriber) {

	pub_death_sub.unsubscribe(subscriber.sub_death_pub);
	list_subs.remove(&subscriber);

}
