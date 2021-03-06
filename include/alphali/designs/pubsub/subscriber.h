#pragma once

#include <functional>
#include "death_publisher.h"



namespace alphali {

	class publisher;
	class collaborator;

	class subscriber {

		using set = std::set<publisher*>;
		using map = std::multimap<
			const publisher*, std::function<void()>>;

	private:

		death_publisher sub_death_pub;
		death_subscriber sub_death_sub;
		set list_pubs;
		map map_to_update_fcn;

	public:

		subscriber();

		subscriber(const subscriber& other);
		subscriber& operator=(const subscriber& other);

		subscriber(subscriber&& other);
		subscriber& operator=(subscriber&& other);

		~subscriber();



		// Note: Subscribing with a function object that holds a
		//  reference to either publisher or subscriber is undefined
		//  behavior and should be treated with care.
		void subscribe(
			publisher& pub, std::function<void()> fcn_update);

		void unsubscribe(publisher& pub);

		void subscribe(
			collaborator& collab, std::function<void()> fcn_update);

		void unsubscribe(collaborator& collab);



	private:

		void update(publisher& pub);



		void pub_copied(const publisher& old_pub, publisher& new_pub);

		void pub_moved(publisher& old_pub, publisher& new_pub);



		friend class publisher;

	};

}
