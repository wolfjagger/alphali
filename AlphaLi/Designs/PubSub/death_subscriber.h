#pragma once

#include <unordered_set>
#include <functional>
#include <unordered_map>



class death_publisher;

class death_subscriber {

	using Set = std::unordered_set<death_publisher*>;
	using Map = std::unordered_map<
		const death_publisher*, std::function<void()>>;

private:

	Set list_pubs;
	Map map_to_death_fcn;

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

	void pubMoved(
		death_publisher& pubOld, death_publisher& pubNew);

	void pub_killed(death_publisher& pub);



	void detach_all();



	friend class death_publisher;

};
