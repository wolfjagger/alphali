#include "death_publisher.h"
#include <algorithm>

using namespace alphali;



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
		for(auto* sub : list_subs) {
			sub->pub_moved(other, *this);
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

	auto fcn_killed = [this](death_subscriber* sub) {
		sub->pub_killed(*this);
	};

	std::for_each(
		list_subs.begin(), list_subs.end(), fcn_killed);

}
