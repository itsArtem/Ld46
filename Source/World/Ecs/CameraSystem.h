#pragma once

class Entity;

class CameraSystem final
{
public:
	void update(Entity &e) const noexcept;
};