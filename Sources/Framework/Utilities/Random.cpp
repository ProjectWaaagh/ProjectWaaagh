#include "Random.hpp"
#include "Errors.hpp"
#include "SFMTRand.hpp"

#include <memory>
#include <random>

static thread_local std::unique_ptr<SFMTRand> sfmtRand;
static RandomEngine engine;

static SFMTRand* GetRng() {
	if (!sfmtRand)
		sfmtRand = std::make_unique<SFMTRand>();

	return sfmtRand.get();
}

int32 irand(int32 min, int32 max) {
	ASSERT(max >= min);
}
