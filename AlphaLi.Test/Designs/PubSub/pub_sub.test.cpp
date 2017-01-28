#include "stdafx.h"
#include <memory>
#include "Designs/PubSub/publisher.h"

using namespace alphali;



TEST_CASE("Death Publisher-Subscriber", "[Designs][PubSub]") {

	SECTION("Basic functionality.") {

		auto death_pub = death_publisher();
		auto death_sub = death_subscriber();
		auto test = false;
		auto fcn = [&test]() {
			test = true;
		};

		death_sub.subscribe(death_pub, std::move(fcn));

		SECTION("Killing publisher publishes.") {

			death_pub = death_publisher();

			CHECK(test == true);

		}

		SECTION("Not killing publisher does not publish.") {

			CHECK(test == false);

		}

		SECTION("Killing subscriber then publisher does not publish.") {

			death_sub = death_subscriber();

			CHECK(test == false);

		}

		SECTION("Unsubscribing subscriber then killing does not publish.") {

			death_sub.unsubscribe(death_pub);

			CHECK(test == false);

		}

		SECTION("Publisher publishes to all subscribers.") {

			auto death_sub2 = death_subscriber();
			auto test2 = false;
			auto fcn2 = [&test2]() {
				test2 = true;
			};

			death_sub2.subscribe(death_pub, std::move(fcn2));

			death_pub = death_publisher();

			CHECK(test == true);
			CHECK(test2 == true);

		}

	}



	SECTION("Reassignment functionality.") {

		auto death_pub1 = death_publisher();
		auto death_pub2 = death_publisher();
		auto death_sub1 = death_subscriber();
		auto death_sub2 = death_subscriber();
		auto testNum = 0;
		auto fcn = [&testNum]() {
			++testNum;
		};
		
		SECTION("Moving publisher.") {

			death_sub1.subscribe(death_pub1, std::move(fcn));
			death_pub2 = std::move(death_pub1);
			death_pub1 = death_publisher();

			CHECK(testNum == 0);

			death_pub2 = death_publisher();

			CHECK(testNum == 1);

		}

		SECTION("Copying subscriber.") {

			death_sub1.subscribe(death_pub1, std::move(fcn));
			death_sub2 = death_sub1;

			death_pub1 = death_publisher();

			CHECK(testNum == 2);

		}

		SECTION("Moving subscriber.") {

			death_sub1.subscribe(death_pub1, std::move(fcn));
			death_sub2 = std::move(death_sub1);
			death_sub1 = death_subscriber();

			death_pub1 = death_publisher();

			CHECK(testNum == 1);

		}

	}

}



TEST_CASE("Publisher-Subscriber", "[Designs][PubSub]") {

	SECTION("Basic functionality.") {

		auto pub = publisher();
		auto sub = subscriber();
		auto test = false;
		auto fcn = [&test]() {
			test = true;
		};

		sub.subscribe(pub, std::move(fcn));

		SECTION("If publisher is updated, subscriber function activates.") {

			pub.publish();

			CHECK(test == true);

		}

		SECTION("If publisher is not updated, subscriber function does not activate.") {

			CHECK(test == false);

		}

		SECTION("If publisher is killed, subscriber function does not activate.") {

			pub = publisher();

			CHECK(test == false);

		}

		SECTION("Killing subscriber, then publishing does not throw or activate function.") {

			sub = subscriber();

			CHECK_NOTHROW(pub.publish());
			CHECK(test == false);

		}

		SECTION("Publisher publishes to all subscribers.") {

			auto sub2 = subscriber();
			auto test2 = false;
			auto fcn2 = [&test2]() {
				test2 = true;
			};

			sub2.subscribe(pub, std::move(fcn2));

			pub.publish();

			CHECK((test && test2) == true);

		}

		SECTION("Subscribing again combines functions.") {

			auto test2 = false;
			auto fcn2 = [&test2]() {
				test2 = true;
			};

			sub.subscribe(pub, fcn2);

			pub.publish();

			CHECK((test && test2) == true);

		}

		SECTION("Repeated publishing activates the function repeatedly.") {

			auto test_int = 0;
			auto fcnInt = [&test_int]() {
				++test_int;
			};

			sub.subscribe(pub, std::move(fcnInt));

			pub.publish();
			pub.publish();

			CHECK(test_int == 2);

		}

	}



	SECTION("Reassignment functionality.") {

		auto pub1 = publisher();
		auto pub2 = publisher();
		auto sub1 = subscriber();
		auto sub2 = subscriber();
		auto testNum = 0;
		auto fcn = [&testNum]() {
			++testNum;
		};
		
		SECTION("Copying publisher.") {

			sub1.subscribe(pub1, std::move(fcn));
			pub2 = pub1;
			pub1.publish();

			CHECK(testNum == 1);

			pub2.publish();

			CHECK(testNum == 2);

		}

		SECTION("Moving publisher.") {

			sub1.subscribe(pub1, std::move(fcn));
			pub2 = std::move(pub1);
			pub1.publish();

			CHECK(testNum == 0);

			pub2.publish();

			CHECK(testNum == 1);

			pub1 = publisher();
			pub2.publish();

			CHECK(testNum == 2);

		}

		SECTION("Copying subscriber.") {

			sub1.subscribe(pub1, std::move(fcn));
			sub2 = sub1;

			pub1.publish();

			CHECK(testNum == 2);

		}

		SECTION("Moving subscriber.") {

			sub1.subscribe(pub1, std::move(fcn));
			sub2 = std::move(sub1);

			pub1.publish();

			CHECK(testNum == 1);

		}

	}

}
