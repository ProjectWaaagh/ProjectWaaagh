#ifndef PROJECTWAAAGH_ADVSTD_HPP
#define PROJECTWAAAGH_ADVSTD_HPP

#include <cstddef>
#include <type_traits>

namespace advstd {

	/**
	 * C++20 std::remove_cvref_t
	 */
	template<class T>
	using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

	/**
	 * C++20 std::type_identity
	 */
	template<class T>
	struct type_identity {
		using type = T;
	};

	/**
	 * C++20 std::type_identity_t
	 */
	template<typename T>
	using type_identity_t = typename type_identity<T>::type;
}

#endif // !PROJECTWAAAGH_ADVSTD_HPP
