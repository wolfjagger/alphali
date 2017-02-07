#pragma once

#include <algorithm>
#include <random>
#include <vector>



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

	// A vector of size vec_size with random numbers in [min, max],
	//  with or without possible duplicates
	inline std::vector<int> rand_vec_inclusive(
		int min, int max,
		unsigned vec_size, bool with_duplicates = false) {
		
		if (vec_size == 1) {
			
			auto vec = std::vector<int>();
			vec.push_back(rand_inclusive(min, max));
			return vec;
			
		} else {
			
			if (!with_duplicates && (max - min + 1) < int(vec_size)) {
				throw std::exception("Random range too small for array.");
			}
		
			auto prev_array = rand_vec_inclusive(min, max, vec_size - 1, with_duplicates);
			auto new_num = rand_inclusive(min, max);
			
			if (!with_duplicates) {
				constexpr auto escape_max = 20;
				auto escape = 0;
				while (std::any_of(prev_array.begin(), prev_array.end(),
				[new_num](int n) { return n == new_num; })) {
					new_num = rand_inclusive(min, max);
					++escape;
					if (escape >= escape_max)
						throw std::exception("Couldn't find distinct nodes.");
				}
			}

			prev_array.push_back(new_num);
			
			return prev_array;
			
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

		shuffle(first, last, random::rd);

	}

}
