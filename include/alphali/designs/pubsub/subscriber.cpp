#include "subscriber.h"
#include <iostream>
#include "publisher.h"
#include "collaborator.h"
#include "detail/pubsub_log.h"

using namespace alphali;



subscriber::subscriber()
	: sub_death_pub(),
	sub_death_sub(),
	list_pubs(),
	map_to_update_fcn() {

	if (pubsub_DEBUG) std::cout << "Sub ctor\n";

}



subscriber::subscriber(const subscriber& other) {
	*this = other;
}
subscriber& subscriber::operator=(const subscriber& other) {

	if(this != &other) {

		if (pubsub_DEBUG) std::cout << "Sub copy\n";

		sub_death_pub = death_publisher();
		sub_death_sub = death_subscriber();
		list_pubs = other.list_pubs;
		map_to_update_fcn = other.map_to_update_fcn;

		for(auto* pub : other.list_pubs) {
			auto fcn_pub_killed = [this, &pub]() {
				list_pubs.erase(pub);
			};
			sub_death_sub.subscribe(
				pub->pub_death_pub, std::move(fcn_pub_killed));

			pub->attach(*this);
		}

	}

	return *this;

}

subscriber::subscriber(subscriber&& other) {
	*this = std::move(other);
}
subscriber& subscriber::operator=(subscriber&& other) {

	if(this != &other) {

		if(pubsub_DEBUG) std::cout << "Sub move\n";

		sub_death_pub = death_publisher();
		sub_death_sub = death_subscriber();
		list_pubs = std::move(other.list_pubs);
		map_to_update_fcn = std::move(other.map_to_update_fcn);

		for(auto* pub : list_pubs) {
			auto fcn_pub_killed = [this, &pub]() {
				list_pubs.erase(pub);
			};
			other.sub_death_sub.unsubscribe(pub->pub_death_pub);
			sub_death_sub.subscribe(
				pub->pub_death_pub, std::move(fcn_pub_killed));

			pub->detach(other);
			pub->attach(*this);
		}

	}

	return *this;

}



subscriber::~subscriber() {
	if (pubsub_DEBUG) std::cout << "Sub dtor\n";
}



void subscriber::subscribe(
	publisher& publisher, std::function<void()> fcn_update) {

	if (pubsub_DEBUG) std::cout << "subscribe\n";

	if(list_pubs.count(&publisher) == 0) {

		auto fcn_pub_killed = [this, &publisher]() {
			map_to_update_fcn.erase(&publisher);
		};
		sub_death_sub.subscribe(publisher.pub_death_pub, fcn_pub_killed);

		publisher.attach(*this);
		list_pubs.emplace(&publisher);

	}

	map_to_update_fcn.emplace(&publisher, std::move(fcn_update));

}



void subscriber::unsubscribe(publisher& publisher) {

	if (pubsub_DEBUG) std::cout << "unsubscribe\n";

	if(list_pubs.count(&publisher) > 0) {

		sub_death_sub.unsubscribe(publisher.pub_death_pub);
		publisher.detach(*this);
		list_pubs.erase(&publisher);
		map_to_update_fcn.erase(&publisher);

	}

}



void subscriber::subscribe(
	collaborator& collab, std::function<void()> fcn_update) {

	subscribe(collab.pub, std::move(fcn_update));

}

void subscriber::unsubscribe(collaborator& collab) {

	unsubscribe(collab.pub);

}



void subscriber::update(publisher& pub) {
	
	if (pubsub_DEBUG) std::cout << "update\n";

	auto matches = map_to_update_fcn.equal_range(&pub);
	for (auto match = matches.first; match != matches.second; ++match) {
		match->second();
	}

}



void subscriber::pub_copied(
	const publisher& old_pub, publisher& new_pub) {

	if (pubsub_DEBUG) std::cout << "pub_copied\n";

	auto fcn_pub_killed = [this, &new_pub]() {
		map_to_update_fcn.erase(&new_pub);
	};
	sub_death_sub.subscribe(new_pub.pub_death_pub, fcn_pub_killed);

	auto matches = map_to_update_fcn.equal_range(&old_pub);
	for (auto match = matches.first; match != matches.second; ++match) {
		map_to_update_fcn.emplace(&new_pub, match->second);
	}

}

void subscriber::pub_moved(
	publisher& old_pub, publisher& new_pub) {

	if (pubsub_DEBUG) std::cout << "pub_moved\n";

	auto fcn_pub_killed = [this, &new_pub]() {
		map_to_update_fcn.erase(&new_pub);
	};
	sub_death_sub.unsubscribe(old_pub.pub_death_pub);
	sub_death_sub.subscribe(new_pub.pub_death_pub, fcn_pub_killed);

	for(auto match = map_to_update_fcn.lower_bound(&old_pub);
		match != map_to_update_fcn.upper_bound(&old_pub); ++match) {
		map_to_update_fcn.emplace(&new_pub, std::move(match->second));
	}

	map_to_update_fcn.erase(&old_pub);

}
