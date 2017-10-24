#pragma once
#include <memory>
#include "../components/RenderComponent.hpp"
#include "../Tile.hpp"

struct ActorSystem;

struct Terrain {

	Terrain(int tile = NOTHING, std::string name = "Ground", bool isTransparent = true, bool isWalkable = false,
			TCODColor fg = TCODColor::white, TCODColor bg = TCODColor::black, bool isEscape = false)
		: _isTransparent(isTransparent), _isWalkable(isWalkable), _isEscape(isEscape), _actor(nullptr)
	{
		_renderer = std::make_shared<RenderComponent>(tile, name);
		_renderer->_fg = fg;
		_renderer->_bg = bg;
	}
	~Terrain() {}

	bool _isTransparent;
	bool _isWalkable;
	bool _isEscape;
	std::shared_ptr<ActorSystem> _actor;
	std::shared_ptr<RenderComponent> _renderer;
};