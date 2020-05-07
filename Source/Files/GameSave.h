#pragma once

#include "File.h"

#include <string>

class GameSave final : public File
{
public:
	int level = 1;

	GameSave(const std::string &path) noexcept;

	void save() const noexcept override;
	void load() noexcept override;

	void reset() noexcept;
};