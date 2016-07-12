#include "death_subscriber.h"
#include <algorithm>
#include "death_publisher.h"



death_subscriber::death_subscriber()
	: list_pubs(),
	map_to_death_fcn() {

}



death_subscriber::death_subscriber(const death_subscriber& other) {
	*this = other;
}
death_subscriber& death_subscriber::operator=(const death_subscriber& other) {
	
	if(this != &other) {

		detach_all();

		list_pubs = other.list_pubs;
		map_to_death_fcn = other.map_to_death_fcn;
		for(auto* p_pub : list_pubs) {
			p_pub->attach(*this);
		}

	}

	return *this;

}

death_subscriber::death_subscriber(death_subscriber&& other) {
	*this = std::move(other);
}
death_subscriber& death_subscriber::operator=(death_subscriber&& other) {
	
	if(this != &other) {

		detach_all();

		list_pubs = std::move(other.list_pubs);
		map_to_death_fcn = std::move(other.map_to_death_fcn);
		for(auto* p_pub : list_pubs) {
			p_pub->detach(other);
			p_pub->attach(*this);
		}

	}

	return *this;

}



death_subscriber::~death_subscriber() {

	detach_all();

}



void death_subscriber::subscribe(
	death_publisher& pub,
	std::function<void()> fcn_pub_death) {

	if(map_to_death_fcn.count(&pub) > 0) {

		auto& oldFcn = map_to_death_fcn.at(&pub);
		auto totFcn = [oldFcn, fcn_pub_death]() {
			oldFcn();
			fcnPubDeath();
		};
		map_to_death_fcn.at(&pub) = std::move(totFcn);

	} else {

		pub.attach(*this);
		list_pubs.emplace(&pub);
		map_to_death_fcn.emplace(&pub, std::move(fcn_pub_death));

	}

}

void death_subscriber::unsubscribe(death_publisher& pub) {

	if(map_to_death_fcn.count(&pub) > 0) {
		pub.detach(*this);
		list_pubs.erase(&pub);
		map_to_death_fcn.erase(&pub);
	}

}



void death_subscriber::pubMoved(
	death_publisher& pub_old, death_publisher& pub_new) {

	auto fcn = std::move(map_to_death_fcn.at(&pub_old)); // Bad?

	list_pubs.erase(&pub_old);
	list_pubs.emplace(&pub_new);

	map_to_death_fcn.erase(&pub_old);
	map_to_death_fcn.emplace(&pub_new, std::move(fcn));
	
}

void death_subscriber::pub_killed(death_publisher& pub) {

	map_to_death_fcn.at(&pub)();
	list_pubs.erase(&pub);
	map_to_death_fcn.erase(&pub);

}



void death_subscriber::detach_all() {

	auto fcn = [this](death_publisher* pub) {
		pub->detach(*this);
	};

	std::for_each(list_pubs.begin(), list_pubs.end(), fcn);

}
