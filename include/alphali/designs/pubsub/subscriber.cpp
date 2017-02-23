#include "subscriber.h"
#include "publisher.h"
#include "collaborator.h"

using namespace alphali;



subscriber::subscriber()
	: sub_death_pub(),
	sub_death_sub(),
	list_pubs(),
	map_to_update_fcn() {

}



subscriber::subscriber(const subscriber& other) {
	*this = other;
}
subscriber& subscriber::operator=(const subscriber& other) {

	if(this != &other) {

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

}



void subscriber::subscribe(
	publisher& publisher, std::function<void()> fcn_update) {

	if(map_to_update_fcn.count(&publisher) > 0) {

		auto& old_fcn = map_to_update_fcn.at(&publisher);
		auto tot_fcn = [old_fcn, fcn_update]() {
			old_fcn();
			fcn_update();
		};
		map_to_update_fcn.at(&publisher) = std::move(tot_fcn);

	} else {

		auto fcn_pub_killed = [this, &publisher]() {
			map_to_update_fcn.erase(&publisher);
		};
		sub_death_sub.subscribe(publisher.pub_death_pub, fcn_pub_killed);
	
		publisher.attach(*this);

		list_pubs.emplace(&publisher);
		map_to_update_fcn.emplace(&publisher, std::move(fcn_update));

	}

}



void subscriber::unsubscribe(publisher& publisher) {

	if(map_to_update_fcn.count(&publisher) > 0) {

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
	
	map_to_update_fcn.at(&pub)();

}



void subscriber::pub_copied(
	const publisher& old_pub, publisher& new_pub) {

	auto fcn_pub_killed = [this, &new_pub]() {
		map_to_update_fcn.erase(&new_pub);
	};
	sub_death_sub.subscribe(new_pub.pub_death_pub, fcn_pub_killed);

	//TODO: Find another waaaaaaay!!!
	auto fcn_update = map_to_update_fcn.at(&old_pub);
	map_to_update_fcn.emplace(&new_pub, std::move(fcn_update));

}

void subscriber::pub_moved(
	publisher& old_pub, publisher& new_pub) {

	auto fcn_pub_killed = [this, &new_pub]() {
		map_to_update_fcn.erase(&new_pub);
	};
	sub_death_sub.unsubscribe(old_pub.pub_death_pub);
	sub_death_sub.subscribe(new_pub.pub_death_pub, fcn_pub_killed);

	auto fcn_update = std::move(map_to_update_fcn.at(&old_pub));
	map_to_update_fcn.erase(&old_pub);
	map_to_update_fcn.emplace(&new_pub, std::move(fcn_update));

}
