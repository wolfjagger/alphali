#include "subscriber.h"
#include "publisher.h"



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

		for(auto* p_pub : other.list_pubs) {
			auto fcn_pub_killed = [this, &p_pub]() {
				list_pubs.erase(p_pub);
			};
			sub_death_sub.subscribe(
				p_pub->pub_death_pub, std::move(fcn_pub_killed));

			p_pub->attach(*this);
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

		for(auto* p_pub : list_pubs) {
			auto fcn_pub_killed = [this, &p_pub]() {
				list_pubs.erase(p_pub);
			};
			other.sub_death_sub.unsubscribe(p_pub->pub_death_pub);
			sub_death_sub.subscribe(
				p_pub->pub_death_pub, std::move(fcn_pub_killed));

			p_pub->detach(other);
			p_pub->attach(*this);
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
			fcnUpdate();
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



void subscriber::update(publisher& pub) {
	
	map_to_update_fcn.at(&pub)();

}



void subscriber::pub_copied(
	const publisher& pubOld, publisher& pub_new) {

	auto fcn_pub_killed = [this, &pub_new]() {
		map_to_update_fcn.erase(&pub_new);
	};
	sub_death_sub.subscribe(pub_new.pub_death_pub, fcn_pub_killed);

	//TODO: Find another waaaaaaay!!!
	auto fcnUpdate = map_to_update_fcn.at(&pubOld);
	map_to_update_fcn.emplace(&pub_new, std::move(fcnUpdate));

}

void subscriber::pub_moved(
	publisher& pub_old, publisher& pub_new) {

	auto fcnPubKilled = [this, &pub_new]() {
		map_to_update_fcn.erase(&pub_new);
	};
	sub_death_sub.unsubscribe(pub_old.pub_death_pub);
	sub_death_sub.subscribe(pub_new.pub_death_pub, fcnPubKilled);

	auto fcnUpdate = std::move(map_to_update_fcn.at(&pub_old));
	map_to_update_fcn.erase(&pub_old);
	map_to_update_fcn.emplace(&pub_new, std::move(fcnUpdate));

}
