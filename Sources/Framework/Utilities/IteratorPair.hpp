#ifndef PROJECTWAAAGH_ITERATORPAIR_HPP
#define PROJECTWAAAGH_ITERATORPAIR_HPP

#include <utility>

namespace ProjectWaaagh {

	template<class iterator>
	class IteratorPair {
	public:
		constexpr IteratorPair() : mIterators() {}
		constexpr IteratorPair(iterator first, iterator second) : mIterators(first, second) {}
		constexpr IteratorPair(std::pair<iterator, iterator> iterators) : mIterators(iterators) {}

		constexpr iterator begin() const { return mIterators.first; }
		constexpr iterator end() const { return mIterators.second; }

	private:
		std::pair<iterator, iterator> mIterators;
	};

	namespace Containers {
		template<typename iterator>
		constexpr ProjectWaaagh::IteratorPair<iterator> MakeIteratorPair(iterator first, iterator second) {
			return { first, second };
		}

		template<typename iterator>
		constexpr ProjectWaaagh::IteratorPair<iterator> MakeIteratorPair(std::pair<iterator, iterator> iterators) {
			return iterators;
		}

		template<class M>
		inline auto MapEqualRange(M& map, typename M::key_type const& key) -> IteratorPair<decltype(map.begin())> {
			return { map.equal_range(key) };
		}
	}
}

#endif // !PROJECTWAAAGH_ITERATORPAIR_HPP
