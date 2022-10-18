#pragma once

#include "Application.hpp"

#include "ECS/Scene.hpp"

#include <JamJar/Data/Memory/Refs.hpp>
#include <JamJar/Data/Collections/Maps/HashMap.hpp>

class Game
{
private:
	SharedRef<Application> m_app;
	HashMap<const TypeInfo&, SharedRef<Scene>> m_scenes;
public:
	Game(SharedRef<Application> app) : m_app(app) {}

	void Start(const GraphicsMode& graphicsMode)
	{
		m_app->Start(graphicsMode);
	}
};