#include "File.h"

#include <fstream>

bool File::doesFileExist() const noexcept
{
	std::ifstream stream{path};
	const bool exists = stream.is_open();

	stream.close();
	return exists;
}

File::File(const std::string &path) noexcept
	: path{path}
{
}
