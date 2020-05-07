#include "Config.h"
#include "../Debug.h"

#include <fstream>
#include <ios>

Config::Config(const std::string &path) noexcept
	: File{path}
{
}

void Config::save() const noexcept
{
	std::ofstream stream{path};
	if (!stream)
	{
		LOG_ERROR(SDL_LOG_CATEGORY_ERROR, "Could not open config for writing.")
		return;
	}

	stream << fullscreen << "\n";
	stream << vSync << "\n";

	stream.close();
}

void Config::load() noexcept
{
	std::ifstream stream{path};
	if (!stream)
	{
		LOG_ERROR(SDL_LOG_CATEGORY_ERROR, "Could not open config for reading.")
		return;
	}

	stream >> fullscreen;
	stream >> vSync;

	stream.close();
}
