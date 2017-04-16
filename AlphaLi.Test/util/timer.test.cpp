#include "stdafx.h"
#include "alphali/util/timer.h"

using namespace alphali;



TEST_CASE("Timer", "[util]") {

	auto t = timer();

	SECTION("Timer does not throw when stopped.") {
		t.stop();
		CHECK_NOTHROW(t.secs_elapsed());
	}

	SECTION("Timer throws when not stopped.") {
		CHECK_THROWS(t.secs_elapsed());
	}

}
