#pragma once

#include <chrono>
#include <iostream>
#include <memory>
#include <string>



namespace alphali {

	// Calculates time in secs accurate to ms
	class timer {

		using cl = std::chrono::system_clock;
		using tp = std::chrono::time_point<cl>;

		cl clock;
		tp beg_time;
		std::unique_ptr<tp> end_time;

	public:

		timer() :
			clock() {

			start();

		}



		void start() {
			end_time.reset();
			beg_time = clock.now();
		}

		void stop() {
			end_time = std::make_unique<tp>(clock.now());
		}



		void output(std::string code_block_description) {

			try {

				auto secs = secs_elapsed();

				if (code_block_description.empty()) {
					std::cout << "Took " << secs << " secs" << std::endl;
				} else {
					std::cout << std::move(code_block_description)
						<< " took " << secs << " secs" << std::endl;
				}

			} catch(std::exception& e) {

				std::cout << e.what() << std::endl;

			}

		}



		double secs_elapsed() const {

			if (end_time) {

				using namespace std::chrono;

				auto time_diff = duration_cast<milliseconds>(*end_time - beg_time);
				auto secs = double(time_diff.count()) / 1000;

				return secs;

			} else {

				throw std::exception("Timer not stopped before output request.");

			}

		}

	};

}
