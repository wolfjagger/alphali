#include "stdafx.h"
#include "util/math.h"
#include <algorithm>
#include <map>

using namespace alphali;



TEST_CASE("Powers of two", "[util]") {

	SECTION("Check first few values.") {

		constexpr auto num_trials = 10;

		auto num = 1;

		for (auto power = 1; power < num_trials; ++power) {
			num *= 2;
			REQUIRE(alphali::powtwo(power) == num);
		}

	}

}



namespace {
	size_t factorial(int n) {
		auto fact = 1;
		for (auto i = 2; i <= n; ++i) {
			fact *= i;
		}
		return fact;
	}
}

TEST_CASE("n-choose-k", "[util]") {

	SECTION("Check first few values.") {

		constexpr auto num_n = 10;

		for (auto n = 1; n < num_n; ++n) {
			auto fact_n = factorial(n);
			for (auto k = 0; k <= n; ++k) {
				auto fact_k = factorial(k);
				auto fact_n_k = factorial(n - k);
				REQUIRE(alphali::nchoosek(n, k) == fact_n/(fact_k*fact_n_k));
			}
		}

	}

}