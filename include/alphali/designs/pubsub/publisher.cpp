#include "publisher.h"
#include <algorithm>
#include <iostream>

using namespace alphali;



namespace {
	const bool DEBUG = false;
}



publisher::publisher()
	: pub_death_pub(),
	pub_death_sub(),
	list_subs() {

	if (DEBUG) std::cout << "Pub ctor\n";

}



publisher::publisher(const publisher& other) {
	*this = other;
}
publisher& publisher::operator=(const publisher& other) {

	if(this != &other) {

		if (DEBUG) std::cout << "Pub copy\n";

		pub_death_pub = death_publisher();
		pub_death_sub = death_subscriber();
		list_subs = other.list_subs;

		for(auto* p_sub : list_subs) {
			auto fcn_sub_killed = [this, &p_sub]() {
				list_subs.erase(p_sub);
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

		if (DEBUG) std::cout << "Pub move\n";

		pub_death_pub = death_publisher();
		pub_death_sub = death_subscriber();
		list_subs = std::move(other.list_subs);

		for (auto* pSub : list_subs) {

			auto fcnSubKilled = [this, &pSub]() {
				list_subs.erase(pSub);
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
	if (DEBUG) std::cout << "Pub dtor\n";
}



void publisher::publish() {

	if (DEBUG) {
		std::cout << "publish" << std::endl;
		std::cout << "num: " << list_subs.size() << std::endl;
	}

	for (auto sub : list_subs) {
		sub->update(*this);
	}

}



void publisher::attach(subscriber& subscriber) {

	if (DEBUG) std::cout << "attach" << std::endl;

	auto fcn_sub_killed = [this, &subscriber]() {
		list_subs.erase(&subscriber);
	};
	pub_death_sub.subscribe(subscriber.sub_death_pub, fcn_sub_killed);

	list_subs.emplace(&subscriber);

}

void publisher::detach(subscriber& subscriber) {

	if (DEBUG) std::cout << "detach" << std::endl;

	pub_death_sub.unsubscribe(subscriber.sub_death_pub);
	list_subs.erase(&subscriber);

}
