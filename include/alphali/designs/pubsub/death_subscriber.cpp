#include "death_subscriber.h"
#include <algorithm>
#include <iostream>
#include "death_publisher.h"
#include "detail/pubsub_log.h"

using namespace alphali;



death_subscriber::death_subscriber()
	: list_pubs(),
	map_to_death_fcn() {

	if (pubsub_DEBUG) std::cout << "DeathSub ctor\n";

}



death_subscriber::death_subscriber(const death_subscriber& other) {
	*this = other;
}
death_subscriber& death_subscriber::operator=(const death_subscriber& other) {
	
	if(this != &other) {

		if (pubsub_DEBUG) std::cout << "DeathSub copy\n";

		detach_all();

		list_pubs = other.list_pubs;
		map_to_death_fcn = other.map_to_death_fcn;
		for(auto* pub : list_pubs) {
			pub->attach(*this);
		}

	}

	return *this;

}

death_subscriber::death_subscriber(death_subscriber&& other) {
	*this = std::move(other);
}
death_subscriber& death_subscriber::operator=(death_subscriber&& other) {
	
	if(this != &other) {

		if (pubsub_DEBUG) std::cout << "DeathSub move\n";

		detach_all();

		list_pubs = std::move(other.list_pubs);
		map_to_death_fcn = std::move(other.map_to_death_fcn);
		for(auto* pub : list_pubs) {
			pub->detach(other);
			pub->attach(*this);
		}

	}

	return *this;

}



death_subscriber::~death_subscriber() {

	if (pubsub_DEBUG) std::cout << "DeathSub dtor\n";

	detach_all();

}



void death_subscriber::subscribe(
	death_publisher& pub,
	std::function<void()> fcn_pub_death) {

	if (pubsub_DEBUG) std::cout << "DeathSub subscribe\n";

	if(list_pubs.count(&pub) == 0) {
		pub.attach(*this);
		list_pubs.emplace(&pub);
	}

	map_to_death_fcn.emplace(&pub, std::move(fcn_pub_death));

}

void death_subscriber::unsubscribe(death_publisher& pub) {

	if (pubsub_DEBUG) std::cout << "DeathSub unsubscribe\n";

	if(list_pubs.count(&pub) > 0) {
		pub.detach(*this);
		list_pubs.erase(&pub);
		map_to_death_fcn.erase(&pub);
	}

}



void death_subscriber::pub_moved(
	death_publisher& pub_old, death_publisher& pub_new) {

	if (pubsub_DEBUG) std::cout << "DeathSub pub_moved\n";

	list_pubs.erase(&pub_old);
	list_pubs.emplace(&pub_new);

	for (auto match = map_to_death_fcn.lower_bound(&pub_old);
		match != map_to_death_fcn.upper_bound(&pub_old); ++match) {
		map_to_death_fcn.emplace(&pub_new, std::move(match->second));
	}

	map_to_death_fcn.erase(&pub_old);
	
}

void death_subscriber::pub_killed(death_publisher& pub) {

	if (pubsub_DEBUG) std::cout << "DeathSub pub_killed\n";

	auto matches = map_to_death_fcn.equal_range(&pub);
	for (auto match = matches.first; match != matches.second; ++match) {
		match->second();
	}
	list_pubs.erase(&pub);
	map_to_death_fcn.erase(&pub);

}



void death_subscriber::detach_all() {

	if (pubsub_DEBUG) std::cout << "DeathSub detach_all\n";

	auto fcn = [this](death_publisher* pub) {
		pub->detach(*this);
	};

	std::for_each(list_pubs.begin(), list_pubs.end(), fcn);

}
