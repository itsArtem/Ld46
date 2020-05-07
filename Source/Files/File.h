#pragma once

#include <string>

class File 
{
public:
	File(const File &file) noexcept = default;
	File(File &&file) noexcept = default;
	
	virtual ~File() = default;

	File &operator =(const File &file) noexcept = default;
	File &operator =(File && file) noexcept = default;

	virtual void save() const noexcept = 0;
	virtual void load() noexcept = 0;

	[[nodiscard]] bool doesFileExist() const noexcept;

protected:
	const std::string path;

	File(const std::string &path) noexcept;
};