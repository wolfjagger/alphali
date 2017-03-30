#pragma once

#include <set>
#include <functional>
#include <map>



namespace alphali {

	class death_publisher;

	class death_subscriber {

		using set = std::set<death_publisher*>;
		using map = std::multimap<
			const death_publisher*, std::function<void()>>;

	private:

		set list_pubs;
		map map_to_death_fcn;

	public:

		death_subscriber();

		death_subscriber(const death_subscriber& other);
		death_subscriber& operator=(const death_subscriber& other);

		death_subscriber(death_subscriber&& other);
		death_subscriber& operator=(death_subscriber&& other);

		~death_subscriber();



		void subscribe(
			death_publisher& pub,
			std::function<void()> fcn_pub_death);

		void unsubscribe(death_publisher& pub);



	private:

		void pub_moved(
			death_publisher& old_pub, death_publisher& new_pub);

		void pub_killed(death_publisher& pub);



		void detach_all();



		friend class death_publisher;

	};

}