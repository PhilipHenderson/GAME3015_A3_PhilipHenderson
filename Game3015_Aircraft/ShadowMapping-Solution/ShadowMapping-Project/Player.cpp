#include "Player.hpp"
#include "CommandQueue.hpp"
#include "Aircraft.hpp"

#include <map>
#include <string>
#include <algorithm>
#include <iostream>

struct AircraftMover
{
	AircraftMover(float vx, float vy, float vz)
		: velocity(vx, vy, vz)
	{
	}

	void operator() (Aircraft& aircraft, const GameTimer&) const
	{
		aircraft.accelerate(velocity);
	}

	XMFLOAT3 velocity;
};

Player::Player()
{
	// Set initial key bindings
	mKeyBinding['A'] = MoveLeft;
	mKeyBinding['D'] = MoveRight;
	mKeyBinding['Q'] = MoveUp;
	mKeyBinding['E'] = MoveDown;
	mKeyBinding['W'] = MoveForward;
	mKeyBinding['S'] = MoveBackward;

	// Set initial action bindings
	initializeActions();

	// Assign all categories to player's aircraft
	for (auto& pair : mActionBinding)
		pair.second.category = Category::PlayerAircraft;
}

void Player::handleEvent(int input, CommandQueue& commands)
{
	// Check if pressed key appears in key binding, trigger command if so
	auto found = mKeyBinding.find(input);
	if (found != mKeyBinding.end() && !isRealtimeAction(found->second))
		commands.push(mActionBinding[found->second]);
}

void Player::handleRealtimeInput(CommandQueue& commands)
{
	// Traverse all assigned keys and check if they are pressed
	for (auto pair : mKeyBinding)
	{
		// If key is pressed, lookup action and trigger corresponding command
		if (GetAsyncKeyState(pair.first) & 0x8000 && isRealtimeAction(pair.second))
			commands.push(mActionBinding[pair.second]);
	}
}

void Player::assignKey(Action action, char key)
{
	// Remove all keys that already map to action
	for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end(); )
	{
		if (itr->second == action)
			mKeyBinding.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	mKeyBinding[key] = action;
}

char Player::getAssignedKey(Action action) const
{
	for (auto pair : mKeyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}

	return NULL;
}

void Player::initializeActions()
{
	const float playerSpeed = 10.f;

	mActionBinding[MoveLeft].action = derivedAction<Aircraft>(AircraftMover(-playerSpeed, 0.0f, 0.0f));
	mActionBinding[MoveRight].action = derivedAction<Aircraft>(AircraftMover(+playerSpeed, 0.0f, 0.0f));
	mActionBinding[MoveUp].action = derivedAction<Aircraft>(AircraftMover(0.0f, +playerSpeed, 0.0f));
	mActionBinding[MoveDown].action = derivedAction<Aircraft>(AircraftMover(0.0f, -playerSpeed, 0.0f));
	mActionBinding[MoveForward].action = derivedAction<Aircraft>(AircraftMover(0.0f, 0.0f, +playerSpeed));
	mActionBinding[MoveBackward].action = derivedAction<Aircraft>(AircraftMover(0.0f, 0.0f, -playerSpeed));
}

bool Player::isRealtimeAction(Action action)
{
	switch (action)
	{
	case MoveLeft:
	case MoveRight:
	case MoveDown:
	case MoveUp:
		return true;

	default:
		return false;
	}
}
