#ifndef PROJECTWAAAGH_RANDOM_HPP
#define PROJECTWAAAGH_RANDOM_HPP

#include "Defines.hpp"
#include "Durations.hpp"

#include <limits>

PRW_FRAMEWORK_API int32 irand(int32 min, int32 max);
PRW_FRAMEWORK_API uint32 urand(uint32 min, uint32 max);

PRW_FRAMEWORK_API uint32 urandms(uint32 min, uint32 max);
PRW_FRAMEWORK_API uint32 rand32();

PRW_FRAMEWORK_API Milliseconds randtime(Milliseconds min, Milliseconds max);

PRW_FRAMEWORK_API float frands(float min, float max);
PRW_FRAMEWORK_API double rand_norm();
PRW_FRAMEWORK_API double rand_chance();

PRW_FRAMEWORK_API uint32 urandweighted(size_t count, double const* chances);

inline bool roll_chance_f(float chance) {
	return chance > rand_chance();
}
inline bool roll_chance_i(int chance) {
	return chance > irand(0, 99);
}

class PRW_FRAMEWORK_API RandomEngine {
public:
	typedef uint32 result_type;

	static constexpr result_type min() {
		return std::numeric_limits<result_type>::min();
	}
	static constexpr result_type max() {
		return std::numeric_limits<result_type>::max();
	}
	result_type operator()() const {
		return rand32();
	}


	static RandomEngine& Instance();
};

#endif // !PROJECTWAAAGH_RANDOM_HPP
