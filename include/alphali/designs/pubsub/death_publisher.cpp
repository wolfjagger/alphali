#include "death_publisher.h"
#include <algorithm>
#include <iostream>

using namespace alphali;



namespace {
	const bool DEBUG = false;
}



death_publisher::death_publisher()
	: list_subs() {

	if (DEBUG) std::cout << "DeathPub ctor" << std::endl;

}



death_publisher::death_publisher(death_publisher&& other) {
	*this = std::move(other);
}
death_publisher& death_publisher::operator=(death_publisher&& other) {

	if(this != &other) {

		if (DEBUG) std::cout << "DeathPub copy" << std::endl;

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
	
	if (DEBUG) std::cout << "DeathPub dtor" << std::endl;

	publish();

}



void death_publisher::attach(death_subscriber& sub) {

	if (DEBUG) std::cout << "DeathPub attach" << std::endl;

	list_subs.emplace(&sub);

}

void death_publisher::detach(death_subscriber& sub) {

	if (DEBUG) std::cout << "DeathPub detach" << std::endl;

	list_subs.erase(&sub);

}



void death_publisher::publish() {

	if (DEBUG) std::cout << "DeathPub publish" << std::endl;

	auto fcn_killed = [this](death_subscriber* sub) {
		sub->pub_killed(*this);
	};

	std::for_each(
		list_subs.begin(), list_subs.end(), fcn_killed);

}
