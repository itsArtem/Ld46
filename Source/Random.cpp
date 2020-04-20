#include "Random.h"

int getRandomInt(int min, int max) noexcept
{
	engine.seed(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()));

	std::uniform_int_distribution<int> dist{min, max};
	return dist(engine);
}

bool getRandomBool() noexcept
{
	engine.seed(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()));

	std::uniform_int_distribution<int> dist{0, 1};
	return dist(engine) == 0;
}
