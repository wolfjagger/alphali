#include "collaborator.h"

using namespace alphali;



collaborator::collaborator(const publisher& init_pub, const subscriber& init_sub) {
	set_publisher(init_pub);
	set_subscriber(init_sub);
}
collaborator::collaborator(const publisher& init_pub, subscriber&& init_sub) {
	set_publisher(init_pub);
	set_subscriber(std::move(init_sub));
}
collaborator::collaborator(publisher&& init_pub, const subscriber& init_sub) {
	set_publisher(std::move(init_pub));
	set_subscriber(init_sub);
}
collaborator::collaborator(publisher&& init_pub, subscriber&& init_sub) {
	set_publisher(std::move(init_pub));
	set_subscriber(std::move(init_sub));
}

void collaborator::set_publisher(const publisher& new_pub) {
	pub = new_pub;
}
void collaborator::set_publisher(publisher&& new_pub) {
	pub = std::move(new_pub);
}
void collaborator::set_subscriber(const subscriber& new_sub) {
	sub = new_sub;
}
void collaborator::set_subscriber(subscriber&& new_sub) {
	sub = std::move(new_sub);
}





void collaborator::publish() {
	pub.publish();
}

void collaborator::subscribe(collaborator& other, std::function<void()> fcn_update) {
	sub.subscribe(other, std::move(fcn_update));
}
void collaborator::unsubscribe(collaborator& other) {
	sub.unsubscribe(other);
}
void collaborator::subscribe(publisher& other, std::function<void()> fcn_update) {
	sub.subscribe(other, std::move(fcn_update));
}
void collaborator::unsubscribe(publisher& other) {
	sub.unsubscribe(other);
}



void alphali::connect(
	collaborator& collab1, collaborator& collab2,
	std::function<void()> responder1, std::function<void()> responder2) {

	collab1.subscribe(collab2, std::move(responder1));
	collab2.subscribe(collab1, std::move(responder2));

}



void alphali::disconnect(
	collaborator& collab1, collaborator& collab2) {

	collab1.unsubscribe(collab2);
	collab2.unsubscribe(collab1);

}
