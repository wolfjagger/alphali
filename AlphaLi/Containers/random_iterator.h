#pragma once

http://stackoverflow.com/questions/41906711/get-iterator-from-a-container-uniformly-random-from-iterators-that-satisfy-a-pre/41906959

#include <deque>
#include <random>



namespace alphali {

	template<typename ForwardIterator, typename Predicate, typename Generator>
	ForwardIterator random_find_if(
		ForwardIterator first,
		ForwardIterator last,
		Predicate& pred,
		Generator& rand_generator) {

		// store iterators to elements where the predicate is true
		std::deque<ForwardIterator> store;
		for (; first != last; ++first)
			if(pred(*first)) store.push_back(first);

		// if no element satisfies the predicate, return last
		if (store.size() == 0)
			return last;

		// get a random number in range [0, store.size())
		auto min = unsigned int(0);
		auto max = unsigned int(store.size() - 1);
		auto dist = std::uniform_int_distribution<unsigned int>(min, max);
		auto rand = dist(rand_generator);

		// return the corresponding iterator
		return store[rand];

	}

}
