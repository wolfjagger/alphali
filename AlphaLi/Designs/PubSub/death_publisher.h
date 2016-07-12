#pragma once

#include <unordered_set>
#include "death_subscriber.h"



class death_publisher {

private:

	std::unordered_set<death_subscriber*> listSubs;

public:

	death_publisher();

	// Copies deleted to minimize confusion about double-deletes
	death_publisher(const death_publisher& other) = delete;
	death_publisher& operator=(const death_publisher& other) = delete;

	death_publisher(death_publisher&& other);
	death_publisher& operator=(death_publisher&& other);

	~death_publisher();



private:

	void attach(death_subscriber& sub);

	void detach(death_subscriber& sub);



	void publish();



	friend class death_subscriber;

};
