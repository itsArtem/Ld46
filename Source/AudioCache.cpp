#include "AudioCache.h"

#include <SDL_error.h>

#include <stdexcept>

AudioCache::AudioCache() noexcept = default;

AudioCache::AudioCache(AudioCache &&audioc) noexcept
{
	moveData(audioc);
}

AudioCache::~AudioCache()
{
	clearChunks();
	clearMusic();
}

AudioCache &AudioCache::operator =(AudioCache &&audioc) noexcept
{
	clearChunks();
	clearMusic();
	moveData(audioc);

	return *this;
}

void AudioCache::loadChunk(const std::string &path)
{
	Mix_Chunk *const chunk = Mix_LoadWAV(path.c_str());
	if (!chunk) [[unlikely]]
		throw std::runtime_error{SDL_GetError()};

	chunks.emplace_back(chunk);
}

Mix_Chunk *AudioCache::getChunk(std::size_t index) const noexcept
{
	return chunks[index];
}

void AudioCache::removeChunk(std::size_t index) noexcept
{
	Mix_FreeChunk(chunks[index]);
	chunks.erase(chunks.begin() + index);
}

void AudioCache::clearChunks() noexcept
{
	for (Mix_Chunk *const chunk : chunks)
		Mix_FreeChunk(chunk);

	chunks.clear();
}

void AudioCache::loadMusic(const std::string &path)
{
	Mix_Music *const track = Mix_LoadMUS(path.c_str());
	if (!track) [[unlikely]]
		throw std::runtime_error{SDL_GetError()};

	music.emplace_back(track);
}

Mix_Music *AudioCache::getMusic(std::size_t index) const noexcept
{
	return music[index];
}

void AudioCache::removeMusic(std::size_t index) noexcept
{
	Mix_FreeMusic(music[index]);
	music.erase(music.begin() + index);
}

void AudioCache::clearMusic() noexcept
{
	for (Mix_Music *music : music)
	{
		music = nullptr;
		Mix_FreeMusic(music);
	}

	music.clear();
}

std::size_t AudioCache::getChunkCount() const noexcept
{
	return chunks.size();
}

std::size_t AudioCache::getMusicCount() const noexcept
{
	return music.size();
}

void AudioCache::moveData(AudioCache &audioc) noexcept
{
	chunks = std::move(audioc.chunks);
	music = std::move(audioc.music);
}
