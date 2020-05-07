#pragma once

#include "File.h"

#include <string>

class Config final : public File
{
public:
	bool fullscreen = false;
	bool vSync = false;

	Config(const std::string &path) noexcept;

	void save() const noexcept override;
	void load() noexcept override;
};