#include "GameSave.h"
#include "../Debug.h"

#include <fstream>
#include <ios>

GameSave::GameSave(const std::string &path) noexcept
	: File{path}
{
}

void GameSave::save() const noexcept
{
	std::ofstream stream{path, std::ios_base::binary | std::ios_base::trunc};
	if (!stream)
	{
		LOG_ERROR(SDL_LOG_CATEGORY_ERROR, "Could not open save for writing.")
		return;
	}

	stream.write(reinterpret_cast<const char *>(&level), sizeof(level));
	stream.close();
}

void GameSave::load() noexcept
{
	std::ifstream stream{path, std::ios_base::binary};
	if (!stream)
	{
		LOG_ERROR(SDL_LOG_CATEGORY_ERROR, "Could not open save for reading.")
			return;
	}

	stream.read(reinterpret_cast<char *>(&level), sizeof(level));
	stream.close();
}

void GameSave::reset() noexcept
{
	level = 1;
}
