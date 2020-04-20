#pragma once

#define DEBUG _DEBUG

#if DEBUG

#include <SDL_log.h>

#define LOG(msg, ...) SDL_Log(msg, __VA_ARGS__);
#define LOG_INFO(category, msg, ...) SDL_LogInfo(category, msg, __VA_ARGS__ );
#define LOG_DEBUG(category, msg, ...) SDL_LogDebug(category, msg, __VA_ARGS__ );
#define LOG_ERROR(category, msg, ...) SDL_LogError(category, msg, __VA_ARGS__ );
#define LOG_CRITICAL(category, msg, ...) SDL_LogCritical(category, msg, __VA_ARGS__ );
#define LOG_MSG(category, priority, msg, ...) SDL_LogMessage(category, priority, msg, __VA_ARGS__ );

#else

#define LOG(msg, ...)
#define LOG_INFO(category, msg, ...)
#define LOG_DEBUG(category, msg, ...)
#define LOG_ERROR(category, msg, ...)
#define LOG_CRITICAL(category, msg, ...)
#define LOG_MSG(category, priority, msg, ...)

#endif