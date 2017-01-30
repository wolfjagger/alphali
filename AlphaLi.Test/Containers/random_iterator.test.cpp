#include "stdafx.h"
#include <vector>
#include "Containers/random_iterator.h"

using namespace alphali;



TEST_CASE("Containers", "[Containers]") {

	auto vec = std::vector<int>();
	for (auto i = 0; i < 10; ++i) vec.push_back(i);
	auto pred = [](int i) { return i >= 2 && i < 8; };
	auto engine = std::mt19937_64(std::random_device()());

	auto rand_iter = random_find_if(vec.cbegin(), vec.cend(), pred, engine);

	SECTION("Random iterator satisfies predicate.") {
		CHECK(pred(*rand_iter));
	}

	SECTION("Random iterator is >= first value with predicate and <= last.") {
		auto first_iter = std::find_if(vec.cbegin(), vec.cend(), pred);
		auto last_iter = std::find_if(vec.crbegin(), vec.crend(), pred);
		CHECK((*rand_iter >= *first_iter && *rand_iter <= *last_iter));
	}

	SECTION("At least one different from original in several calls.") {

		auto found_different = false;
		for (auto i = 0; i < 5; ++i) {
			auto rand_iter2 = random_find_if(
				vec.cbegin(), vec.cend(), pred, engine);
			if (rand_iter != rand_iter2) found_different = true;
		}
		CHECK(found_different);

	}

}
