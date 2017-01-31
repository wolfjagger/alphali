#include "stdafx.h"
#include "util/random.h"
#include <algorithm>
#include <map>

using namespace alphali;



TEST_CASE("Random bool", "[util]") {

	SECTION("Distribution is fair.") {

		constexpr auto num_trials = 10000;
		constexpr auto chance = 0.237;

		auto num_coin_t = 0;
		auto num_chance_t = 0;
		for (auto i = 0; i < num_trials; ++i) {
			if (coin_flip()) ++num_coin_t;
			if (take_chance(chance)) ++num_chance_t;
		}
		
		constexpr auto num_error_allowed = 500;

		CHECK(std::abs(num_coin_t - 0.5*num_trials) <= num_error_allowed);
		CHECK(std::abs(num_chance_t - chance*num_trials) <= num_error_allowed);

	}

}

TEST_CASE("Random int", "[util]") {

	SECTION("Distribution is fair.") {

		constexpr auto num_trials = 10000;
		constexpr auto min = -3;
		constexpr auto max = 8;

		using map_type = std::map<int, unsigned int>;
		auto num_leq = map_type();
		auto num_btwn = map_type();

		for (auto i = 0; i <= max; ++i) num_leq.emplace(i, 0);
		for (auto i = min; i <= max; ++i) num_btwn.emplace(i, 0);

		for (auto i = 0; i < num_trials; ++i) {
			++num_leq.at(rand_leq(max));
			++num_btwn.at(rand_inclusive(min, max));
		}
		
		using pair_type = std::pair<int, unsigned int>;
		auto lt_pred = [](pair_type pair1, pair_type pair2) {
			return pair1.second < pair2.second;
		};

		constexpr auto range_diff_allowed_leq = 200;
		auto minmax_leq = std::minmax_element(
			num_leq.cbegin(), num_leq.cend(), lt_pred);
		CHECK(minmax_leq.second->second - minmax_leq.first->second
			<= range_diff_allowed_leq);

		constexpr auto range_diff_allowed_btwn = 200;
		auto minmax_btwn = std::minmax_element(
			num_btwn.cbegin(), num_btwn.cend(), lt_pred);
		CHECK(minmax_btwn.second->second - minmax_btwn.first->second
			<= range_diff_allowed_btwn);

	}

	SECTION("rand_vec with duplicates.") {

		constexpr auto size = 500;
		constexpr auto min = -4;
		constexpr auto max = 6;

		auto vec_leq = rand_vec_leq(max, size, true);

		auto minmax_leq = std::minmax_element(vec_leq.cbegin(), vec_leq.cend());
		CHECK(*minmax_leq.first >= 0);
		CHECK(*minmax_leq.second <= max);

		auto vec_btwn = rand_vec_inclusive(min, max, size, true);

		auto minmax_btwn = std::minmax_element(vec_btwn.cbegin(), vec_btwn.cend());
		CHECK(*minmax_btwn.first >= min);
		CHECK(*minmax_btwn.second <= max);

	}

	SECTION("rand_vec without duplicates.") {

		constexpr auto size_throw = 8;
		constexpr auto size_real = 4;
		constexpr auto min = 1;
		constexpr auto max = 6;

		REQUIRE_THROWS(rand_vec_leq(max, size_throw));

		auto vec_leq = rand_vec_leq(max, size_real);

		auto minmax_leq = std::minmax_element(vec_leq.cbegin(), vec_leq.cend());
		CHECK(*minmax_leq.first >= 0);
		CHECK(*minmax_leq.second <= max);

		CHECK_THROWS(rand_vec_inclusive(min, max, size_throw));

		auto vec_btwn = rand_vec_inclusive(min, max, size_real);

		auto minmax_btwn = std::minmax_element(vec_btwn.cbegin(), vec_btwn.cend());
		CHECK(*minmax_btwn.first >= min);
		CHECK(*minmax_btwn.second <= max);

	}

}



TEST_CASE("Random float", "[util]") {

	SECTION("Distribution is fair.") {

		constexpr auto num_trials = 10000;
		constexpr auto min = -6;
		constexpr auto max = -2;
		constexpr auto num_bins = 10;
		constexpr auto bin_size = float(num_bins) / (max - min);

		auto vec_bins = std::vector<unsigned int>();
		vec_bins.resize(num_bins);
		std::fill(vec_bins.begin(), vec_bins.end(), 0);

		for (auto i = 0; i < num_trials; ++i) {
			auto rand = rand_btwn(min, max);
			auto bin_idx = static_cast<unsigned int>(
				std::floor((rand - min)*bin_size));
			++vec_bins.at(bin_idx);
		}
		
		constexpr auto range_diff_allowed = 200;
		auto minmax = std::minmax_element(
			vec_bins.cbegin(), vec_bins.cend());

		CHECK(*minmax.second - *minmax.first <= range_diff_allowed);

	}

}
