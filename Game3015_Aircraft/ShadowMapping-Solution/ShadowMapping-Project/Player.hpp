#pragma once

#include "Command.hpp"
#include <map>

class CommandQueue;

class Player
{
public:
							Player();
	void					handleEvent(int input, CommandQueue& commands);
	void					handleRealtimeInput(CommandQueue& commands);

	//static const float		PlayerSpeed;

	enum Action
	{
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		MoveForward,
		MoveBackward,
		GetPosition,
		ActionCount
	};
	void					assignKey(Action action, char key);
	char					getAssignedKey(Action action) const;


private:
	void					initializeActions();
	static bool				isRealtimeAction(Action action);


private:
	std::map<int, Action>					mKeyBinding;
	std::map<Action, Command>				mActionBinding;
};
