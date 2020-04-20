#pragma once

#include <random>
#include <chrono>

static std::default_random_engine engine;

[[nodiscard]] int getRandomInt(int min, int max) noexcept;

template<typename T> [[nodiscard]] T getRandomReal(T min, T max) noexcept
{
	engine.seed(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()));

	std::uniform_real_distribution<T> dist{min, max};
	return dist(engine);
}

[[nodiscard]] bool getRandomBool() noexcept;