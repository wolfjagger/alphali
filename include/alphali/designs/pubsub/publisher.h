#pragma once

#include "subscriber.h"
#include "death_publisher.h"



namespace alphali {

	// JoeNote: Could consider implementing this as a policy,
	//  giving different functionality for, e.g. connection
	//  and publish timing policies.

	class publisher {

	private:

		death_publisher pub_death_pub;
		death_subscriber pub_death_sub;
		std::set<subscriber*> list_subs;
	
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

}
