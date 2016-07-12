#include "death_publisher.h"
#include <algorithm>



death_publisher::death_publisher()
	: list_subs() {

}



death_publisher::death_publisher(death_publisher&& other) {
	*this = std::move(other);
}
death_publisher& death_publisher::operator=(death_publisher&& other) {

	if(this != &other) {

		// If in a state before moved to, publish to show death.
		publish();

		list_subs = std::move(other.list_subs);
		for(auto* pSub : list_subs) {
			pSub->pubMoved(other, *this);
		}

	}

	return *this;

}



death_publisher::~death_publisher() {
	
	publish();

}



void death_publisher::attach(death_subscriber& sub) {

	list_subs.emplace(&sub);

}

void death_publisher::detach(death_subscriber& sub) {

	list_subs.erase(&sub);

}



void death_publisher::publish() {

	auto fcnKilled = [this](death_subscriber* sub) {
		sub->pubKilled(*this);
	};

	std::for_each(
		list_subs.begin(), list_subs.end(), fcnKilled);

}
