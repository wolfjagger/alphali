#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <limits>
#include <stdexcept>
#include <type_traits>



namespace alphali {

	constexpr auto pi = M_PI;

	template<class T>
	constexpr T powtwo(T exponent) {
		return (T(1) << exponent);
	}



	namespace detail {

		inline size_t exact_nchoosek(size_t n, size_t k) noexcept {

			if (k == 0) return 1;

			return n*exact_nchoosek(n - 1, k - 1)/k;

		}

	}

	template<typename measurement = double,
		typename = std::enable_if_t < std::is_floating_point_v<measurement> > >
	measurement sterling_ln_nchoosek(size_t n, size_t k) noexcept {

		constexpr auto half = measurement(0.5);
		return k * std::log(measurement(n) / k - half)
			+ k - half * static_cast<measurement>(std::log(2 * pi * k));

	}

	inline size_t nchoosek(size_t n, size_t k) {

		if (n < k) throw std::runtime_error("Error: n cannot be larger than k.");

		auto sterling_ln = sterling_ln_nchoosek(n, k);

		static const auto max_ln
			= std::log(std::numeric_limits<double>::max());
		if (sterling_ln > max_ln)
			throw std::runtime_error("Error: nchoosek too high.");

		constexpr auto turnover_ln = 12;
		if (sterling_ln > turnover_ln)
			return size_t(std::floor(std::exp(sterling_ln)));

		return detail::exact_nchoosek(n, k);

	}

}
