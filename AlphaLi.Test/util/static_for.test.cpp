#include "stdafx.h"
#include "util/static_for.h"
#include <array>
#include <iostream>

using namespace alphali;



TEST_CASE("Static for loop", "[util]") {

	SECTION("For loop over numbers.") {

		constexpr size_t size_loop = 4;
		std::array<int, size_loop> arr = {0,0,0,0};

		auto pred = [&arr](int i) {
			arr[i] = i;
		};

		static_for<0, 4>()(pred);

		for (auto i = 0; i < 4; ++i) CHECK(arr[i] == i);
		static_for<0, 3>()([&arr](int i) { CHECK(arr[i] == i); });

	}

}
