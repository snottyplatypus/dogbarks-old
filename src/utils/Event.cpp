#include "Event.hpp"
#include "EventManager.hpp"
#include "../level/Level.hpp"
#include "../ui/Gui.hpp"
#include <iostream>

void EndTurn::react(CommandedSystem & object)
{
	if (level._turnState->_id == "PlayerTurn") { //TEMP UNTIL INTERACTION/MOVEMENT COST
		if (object._id == "player") {
			object._updated = false;
			level._turnState->transit<OtherTurn>(level);
		}
	}
	else if (level._turnState->_id == "OtherTurn") { //ALSO TEMP
		bool updated = true;
		for (auto i : level._actors)
			if (!i->_updated)
				updated = false;
		if (updated)
			level._turnState->transit<PlayerTurn>(level);
	}
}

void TriggerLookingMode::react(CommandedSystem & object)
{
	level._turnState->transit<CursorModeL>(level);
	level._lookingCursor._pos->_x = level._player->_pos->_x;
	level._lookingCursor._pos->_y = level._player->_pos->_y;
	gui._state = START_MENU;
}

void TriggerAimingMode::react(CommandedSystem & object)
{
	level._turnState->transit<CursorModeF>(level);
	level._fireCursor._pos->_x = level._player->_pos->_x;
	level._fireCursor._pos->_y = level._player->_pos->_y;
	gui._state = START_MENU;
}

void TriggerEnter::react(CommandedSystem & object)
{
	if (level._turnState->_id == "CursorModeF") {
		eventManager.onAttack(object, level._fireCursor._lastPos, gui._attackSelect._bodyPart, gui._attackSelect._bullets);
		level._turnState->exit(level);
		gui._state = NOTHING_SPECIAL;
	}
}

void TriggerCancel::react(CommandedSystem & object)
{
	level._turnState->exit(level);
}

void MoveEvent::react(CommandedSystem & object)
{
	if (object._id != "cursor") {
		if (!object._body->_dead) {
			if (level._terrain[object._pos->_x + _x][object._pos->_y + _y]._isWalkable) {
				if (level._terrain[object._pos->_x + _x][object._pos->_y + _y]._actor != nullptr) {
					level._terrain[object._pos->_x + _x][object._pos->_y + _y]._actor->_pos->_x -= _x;
					level._terrain[object._pos->_x + _x][object._pos->_y + _y]._actor->_pos->_y -= _y;
					level._terrain[object._pos->_x + _x][object._pos->_y + _y]._actor = nullptr;
				}
				level._terrain[object._pos->_x][object._pos->_y]._actor = nullptr;
				object._pos->_x += _x;
				object._pos->_y += _y;
			}
		}
	}
	else {
		if (!level._terrain[object._pos->_x + _x][object._pos->_y + _y]._void) {
			object._pos->_x += _x;
			object._pos->_y += _y;
			gui._state = START_MENU;
		}
	}

}

void DeathEvent::react(CommandedSystem & object)
{
	if (object._body->_dead) {
		object._renderer->_fg = TCODColor::darkRed;
		if (object._renderer->_name.find(" - dead") == std::string::npos)
			object._renderer->_name += " - dead";
		if (object._renderer->_tile.find("_dead") == std::string::npos)
			object._renderer->_tile += "_dead";
	}
}

void LookingEvent::react(CommandedSystem & object)
{
	if (object._computing->_map->isInFov(_x, _y))
		gui.lookingInfo({ _x, _y });
}

void AimingEvent::react(CommandedSystem & object)
{
	auto target = level._terrain[_x][_y]._actor;
	if (object._computing->_map->isInFov(_x, _y)) {
		if (target != nullptr)
			gui.attackMenu(object, *target);
	}
}
