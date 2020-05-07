#pragma once

#include <SDL_mixer.h>

#include <vector>
#include <string>
#include <cstddef>

class AudioCache final
{
public:
	AudioCache() noexcept;

	AudioCache(const AudioCache &audioc) = delete;
	AudioCache(AudioCache &&audioc) noexcept;
	
	~AudioCache();

	AudioCache &operator =(const AudioCache &audioc) = delete;
	AudioCache &operator =(AudioCache &&audioc) noexcept;

	void loadChunk(const std::string &path);
	[[nodiscard]] Mix_Chunk *getChunk(std::size_t index) const noexcept;

	void removeChunk(std::size_t index) noexcept;
	void clearChunks() noexcept;

	void loadMusic(const std::string &path);
	[[nodiscard]] Mix_Music *getMusic(std::size_t index) const noexcept;
	
	void removeMusic(std::size_t index) noexcept;
	void clearMusic() noexcept;

	std::size_t getChunkCount() const noexcept;
	std::size_t getMusicCount() const noexcept;

private:
	std::vector<Mix_Chunk *> chunks;
	std::vector<Mix_Music *> music;

	void moveData(AudioCache &audioc) noexcept;
};