#pragma once

#include "MessageOutput.h"
#include "HealthBar.h"
#include "Info.h"

#include <vector>
#include <memory>

class Game;
class World;

class HudManager final
{
public:
	MessageOutput msgout;

	HudManager(const Game &game, World &world) noexcept;

	void update() noexcept;
	void render() const noexcept;

private:
	HealthBar healthBar;
	Info info;
};