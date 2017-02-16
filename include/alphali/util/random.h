#pragma once

#include <algorithm>
#include <random>
#include <vector>
#include "alphali/containers/random_iterator.h"



namespace alphali {
	
	namespace random {
		static std::random_device rd;
		static auto rand_engine = std::mt19937_64(rd());
		static auto rand_uniform = std::uniform_real_distribution<double>();
	}


	// Boolean

	// Does event with probability prob happen?
	inline bool take_chance(double prob) {
		auto bern_prob = std::bernoulli_distribution(prob);
		return bern_prob(random::rand_engine);
	}
	
	// 50/50 coin flip
	inline bool coin_flip() noexcept {
		return take_chance(0.5);
	}


	// Integer

	// Random integer btwn min and max, inclusive
	inline int rand_inclusive(int min, int max) {
		auto dist = std::uniform_int_distribution<int>(min, max);
		return dist(random::rand_engine);
	}

	// Random integer in [0, num]
	inline int rand_leq(unsigned int num) noexcept {
		return rand_inclusive(0, static_cast<int>(num));
	}


	// Int arrays

	namespace detail {

		inline std::vector<int> rand_vec_inclusive_recurse(
			int min, int max, unsigned int vec_size) {

			if (vec_size == 0) {
				return std::vector<int>();
			} else if (vec_size == 1) {

				auto vec = std::vector<int>();
				vec.push_back(rand_inclusive(min, max));
				return vec;

			} else {

				auto prev_array = rand_vec_inclusive_recurse(min, max, vec_size - 1);

				// This implementation is best for vec_size << max-min
				auto new_num = rand_inclusive(min, max);

				constexpr auto escape_max = 20;
				auto escape = 0;
				while (std::any_of(prev_array.begin(), prev_array.end(),
					[new_num](int n) { return n == new_num; })) {

					new_num = rand_inclusive(min, max);

					++escape;

					// If after escape_max tries, still can't find a good random,
					//  construct vec of remainder and choose index randomly.
					if (escape >= escape_max) {
						
						auto remaining_vec = std::vector<int>();
						remaining_vec.reserve(static_cast<size_t>(max - min + 1));
						for (auto i = min; i <= max; ++i) {
							if(std::none_of(prev_array.cbegin(), prev_array.cend(),
								[i](int j) { return i == j;	})) {

								remaining_vec.push_back(i);

							}
						}

						// Guaranteed that size > 0, since otherwise
						//  rand_vec_inclusive would have called vec_size=0 function
						auto new_num_idx = static_cast<size_t>(rand_leq(
							static_cast<unsigned int>(remaining_vec.size()-1)));

						new_num = remaining_vec[new_num_idx];

					}

				}

				prev_array.push_back(new_num);

				return prev_array;

			}

		}

	}

	// A vector of size vec_size with random numbers in [min, max],
	//  with or without possible duplicates
	inline std::vector<int> rand_vec_inclusive(
		int min, int max,
		unsigned vec_size, bool with_duplicates = false) {

		if (with_duplicates) {

			auto vec = std::vector<int>(vec_size);

			std::for_each(vec.begin(), vec.end(),
				[min, max](int& i) { i = rand_inclusive(min, max); });

			return vec;

		} else {

			// If range smaller than vec_size, can't fit all the values
			if ((max - min + 1) < int(vec_size)) {
				throw std::exception("Random range too small for array.");
			}

			auto half_range = static_cast<unsigned int>((max - min + 1) / 2);
			if (vec_size > half_range) {

				// If half range smaller than vec_size, faster to fill values
				//  that will NOT be in the vec and then get remainder

				auto excluded_size = (max - min + 1) - vec_size;
				auto excluded_nums = detail::rand_vec_inclusive_recurse(min, max, excluded_size);

				auto vec = std::vector<int>();
				vec.reserve(vec_size);
				for (auto i = min; i <= max; ++i) {
					if (std::find(excluded_nums.cbegin(), excluded_nums.cend(), i) == excluded_nums.cend())
						vec.push_back(i);
				}

				return vec;

			} else {

				// Otherwise, simply call recursive formula
				return detail::rand_vec_inclusive_recurse(min, max, vec_size);

			}

		}
		
	}

	// A vector of size vec_size with random numbers in [0, num),
	//  with or without possible duplicates
	inline std::vector<int> rand_vec_leq(
		unsigned num, unsigned vec_size, bool with_duplicates = false) {
		return rand_vec_inclusive(
			0, static_cast<int>(num), vec_size, with_duplicates);
	}


	// Double
	
	// Random number btwn 0 and 1
	inline double unit_rand() noexcept {
		return random::rand_uniform(random::rand_engine);
	}

	// Random number btwn min and max
	inline double rand_btwn(double min, double max) noexcept {
		return min + unit_rand()*(max-min);
	}



	// Shuffle
	template<class RandomIt>
	void shuffle(RandomIt first, RandomIt last) {

		std::shuffle(first, last, random::rand_engine);

	}

}
