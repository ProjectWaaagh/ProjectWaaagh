#ifndef PROJECTWAAAGH_CONTAINERS_HPP
#define PROJECTWAAAGH_CONTAINERS_HPP

#include "Defines.hpp"
#include "Random.hpp"

#include <algorithm>
#include <exception>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>

namespace ProjectWaaagh {

	template<class T>
	constexpr inline T* AddressOrSelf(T* ptr) {
		return ptr;
	}

	template<class T>
	constexpr inline T* AddressOrSelf(T& not_ptr) {
		return std::addressof(not_ptr);
	}

	template<class T>
	class CheckedBufferOutputIterator {
	public:
		using iterator_category = std::output_iterator_tag;
		using value_type = void;
		using pointer = T*;
		using reference = T&;
		using difference_type = std::ptrdiff_t;

		CheckedBufferOutputIterator(T* buf, size_t n) : mBuf(buf), mEnd(buf + n) {}

		T& operator*() const { check(); return *mBuf; }
		CheckedBufferOutputIterator& operator++() { check(); ++mBuf; return *this; }
		CheckedBufferOutputIterator operator++(int) { CheckedBufferOutputIterator v = *this; operator++(); return v; }

		size_t remaining() const { return (mEnd - mBuf); }

	private:
		T* mBuf;
		T* mEnd;
		void check() const {
			if (!(mBuf < mEnd))
				throw std::out_of_range("index");
		}
	};

	namespace Containers {

		/// <summary>
		/// 
		/// </summary>
		/// <typeparam name="C"></typeparam>
		/// <param name="container"></param>
		/// <param name="requestedSize"></param>
		template<class C>
		void RandomResize(C& container, std::size_t requestedSize) {

			static_assert(std::is_base_of<std::forward_iterator_tag, typename std::iterator_traits<typename C::iterator>::iterator_category>::value,
				"Invalid container passed to ProjectWaaagh::Containers::RandomResize");
			if (std::size(container) <= requestedSize)
				return;

			auto keepIt = std::begin(container), curIt = std::begin(container);
			uint32 elementsToKeep = requestedSize, elementsToProcess = std::size(container);
			while (elementsToProcess) {
				if (urand(1, elementsToProcess) <= elementsToKeep) {
					if (keepIt != curIt)
						*keepIt = std::move(*curIt);
					++keepIt;
					--elementsToKeep;
				}
				++curIt;
				--elementsToProcess;
			}
			container.erase(keepIt, std::end(container));
		}

		/// <summary>
		/// 
		/// </summary>
		/// <typeparam name="C"></typeparam>
		/// <typeparam name="Predicate"></typeparam>
		/// <param name="container"></param>
		/// <param name="predicate"></param>
		/// <param name="requestedSize"></param>
		template<class C, class Predicate>
		void RandomResize(C& container, Predicate&& predicate, std::size_t requestedSize) {
			
			C containerCopy;
			std::copy_if(std::begin(container), std::end(container), std::inserter(containerCopy, std::end(containerCopy)), predicate);

			if (requestedSize)
				RandomResize(containerCopy, requestedSize);

			container = std::move(containerCopy);
		}

		/// <summary>
		/// Select a random element from a container.
		/// Note: container cannot be emty!
		/// </summary>
		/// <typeparam name="C"></typeparam>
		/// <param name="container"></param>
		/// <returns></returns>
		template<class C>
		inline auto SelectRandomContainerElement(C const& container) -> typename std::add_const<decltype(*std::begin(container))>::type& {
			
			auto it = std::begin(container);
			std::advance(it, urand(0, uint32(std::size(container)) - 1));
			return *it;
		}

		/// <summary>
		/// Select a random element from a container where each element has a different chance to be selected.
		/// </summary>
		/// <typeparam name="C"></typeparam>
		/// <param name="container">
		/// Container to select an element from.
		/// </param>
		/// <param name="weights">
		/// Chances of each element to be selected, must be in the same order as elements in container.
		///	Caller is responsible for checking that sum of all weights is greater than 0.
		/// </param>
		/// <returns></returns>
		template<class C>
		inline auto SelectRandomWeightedContainerElement(C const& container, std::vector<double> const& weights) -> decltype(std::begin(container)) {

			auto it = std::begin(container);
			std::advance(it, urandweighted(weights.size(), weights.data()));
			return it;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <typeparam name="C"></typeparam>
		/// <typeparam name="Fn"></typeparam>
		/// <param name="container"></param>
		/// <param name="weightExtractor"></param>
		/// <returns></returns>
		template<class C, class Fn>
		inline auto SelectRandomWeightedContainerElement(C const& container, Fn weightExtractor) -> decltype(std::begin(container)) {
		
			std::vector<double> weights;
			weights.reserve(std::size(container));
			double weightSum = 0.0;
			for (auto& val : container) {
				double weight = weightExtractor(val);
				weights.push_back(weight);
				weightSum += weight;
			}
			if (weightSum <= 0.0)
				weights.assign(std::size(container), 1.0);

			return SelectRandomWeightedContainerElement(container, weights);
		}

		/// <summary>
		/// Reorder the elements of the iterator range randomly.
		/// </summary>
		/// <typeparam name="Iterator"></typeparam>
		/// <param name="begin">Beginning of the range to reorder.</param>
		/// <param name="end">End of the range to reorder.</param>
		template<class Iterator>
		inline void RandomShuffle(Iterator begin, Iterator end) {

			std::shuffle(begin, end, RandomEngine::Instance());
		}

		/// <summary>
		/// Reorder the elements of the container randomly.
		/// </summary>
		/// <typeparam name="C"></typeparam>
		/// <param name="container">Container to reorder.</param>
		template<class C>
		inline void RandomShuffle(C& container) {
			RandomShuffle(std::begin(container), std::end(container));
		}

		/// <summary>
		/// Checks if two SORTED containers have a common element.
		/// </summary>
		/// <typeparam name="Iterator1"></typeparam>
		/// <typeparam name="Iterator2"></typeparam>
		/// <param name="first1">Iterator pointing to start of the first container</param>
		/// <param name="last1">Iterator pointing to end of the first container</param>
		/// <param name="first2">Iterator pointing to start of the second container</param>
		/// <param name="last2">Iterator pointing to end of the second container</param>
		/// <returns></returns>
		template<class Iterator1, class Iterator2>
		inline bool Intersects(Iterator1 first1, Iterator1 last1, Iterator2 first2, Iterator2 last2) {

			while (first1 != last1 && first2 != last2) {
				if (*first1 < *first2)
					++first1;
				else if (*first2 < *first1)
					++first2;
				else
					return true;
			}

			return false;
		}

		/// <summary>
		/// Checks if two SORTED containers have a common element.
		/// </summary>
		/// <typeparam name="Iterator1"></typeparam>
		/// <typeparam name="Iterator2"></typeparam>
		/// <param name="first1">Iterator pointing to start of the first container</param>
		/// <param name="last1">Iterator pointing to end of the first container</param>
		/// <param name="first2">Iterator pointing to start of the second container</param>
		/// <param name="last2">Iterator pointing to end of the second container</param>
		/// <param name="equalPred">Additional predicate to exclude elements</param>
		/// <returns>true if containers have a common element, false otherwise.</returns>
		template<class Iterator1, class Iterator2, class Predicate>
		inline bool Intersects(Iterator1 first1, Iterator1 last1, Iterator2 first2, Iterator2 last2, Predicate&& equalPred) {

			while (first1 != last1 && first2 != last2) {
				if (*first1 < *first2)
					++first1;
				else if (first2 < *first1)
					++first2;
				else if (!equalPred(*first1, *first2))
					++first1, ++first2;
				else
					return true;
			}

			return false;
		}

		template<class M>
		inline auto MapGetValuePtr(M& map, typename M::key_type const& key) -> decltype(AddressOrSelf(map.find(key)->second)) {
		
			auto itr = map.find(key);
			return itr != map.end() ? AddressOrSelf(itr->second) : nullptr;
		}

		template<class K, class V, template<class, class, class...> class M, class... Rest>
		inline void MultimapErasePair(M<K, V, Rest...>& multimap, K const& key, V const& value) {
		
			auto range = multimap.equal_range(key);
			for (auto itr = range.first; itr != range.second;) {
				if (itr->second == value)
					itr = multimap.erease(itr);
				else
					++itr;
			}
		}

		template<typename Container, typename Predicate>
		std::enable_if<std::is_move_assignable_v<decltype(*std::declval<Container>().begin())>, void> EreaseIf(Container& c, Predicate p) {

			auto wpos = c.begin();
			for (auto rpos = c.begin(), end = c.end(); rpos != end; ++rpos) {
				if (!p(*rpos)) {
					if (rpos != wpos)
						std::swap(*rpos, *wpos);
					++wpos;
				}
			}
			c.erase(wpos, c.end());
		}

		template<typename Container, typename Predicate>
		std::enable_if_t<!std::is_move_assignable_v<decltype(*std::declval<Container>().begin())>, void> EreaseIf(Container& c, Predicate p) {

			for (auto it = c.begin(); it != c.end();) {
				if (p(*it))
					it = c.erase(it);
				else
					++it;
			}
		}


		template<typename T>
		inline decltype(auto) EnsureWriteableVectorIndex(std::vector<T>& vec, typename std::vector<T>::size_type i) {

			if (i >= vec.size())
				vec.resize(i + 1);

			return vec;
		}

		template<typename T>
		inline decltype(auto) EnsureWriteableVectorIndex(std::vector<T>& vec, typename std::vector<T>::size_type i, T const& resizeDefault) {

			if (i >= vec.size())
				vec.resize(i + 1, resizeDefault);

			return vec[i];
		}
	}//!namespace Containers
} //!namespace ProjectWaaagh


#endif // !PROJECTWAAAGH_CONTAINERS_HPP
