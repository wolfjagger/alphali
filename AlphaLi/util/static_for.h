#pragma once



namespace alphali {

	template<int Begin, int End>
	struct static_for {
		template <typename Fn>
		void operator()(const Fn& fn) const {
			fn(Begin);
			static_for<Begin + 1, End>()(fn);
		}
	};

	template <int N>
	struct static_for<N, N> {
		template <typename Fn>
		void operator()(const Fn& fn) const {}
	};

}
