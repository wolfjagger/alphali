#pragma once

#include <forward_list>
#include "subscriber.h"
#include "death_publisher.h"



// JoeNote: Could consider implementing this as a policy,
//  giving different functionality for, e.g. connection
//  and publish timing policies.

class publisher {

private:

	death_publisher pub_death_pub;
	death_subscriber pub_death_sub;
	std::forward_list<subscriber*> list_subs;
	
public:

	publisher();

	publisher(const publisher& other);
	publisher& operator=(const publisher& other);

	publisher(publisher&& other);
	publisher& operator=(publisher&& other);

	~publisher();



	void publish();



private:

	void attach(subscriber& subscriber);

	void detach(subscriber& sub);



	friend class subscriber;

};
