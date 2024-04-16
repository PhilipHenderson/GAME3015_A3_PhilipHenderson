#pragma once

#include "StateIdentifiers.hpp"
#include "..\..\Common\GameTimer.h"

#include <memory>

class StateStack;
class Player;
class Game;

class State
{
public:
	typedef std::unique_ptr<State> Ptr;

	struct Context
	{
		Context(Game* game, Player& player);

		Game* game;
		Player* player;
	};


public:
	State(StateStack& stack, Context context);
	virtual				~State();

	virtual void		buildRenderItems() = 0;
	virtual void		draw() = 0;
	virtual bool		update(const GameTimer& dt) = 0;
	virtual bool		handleEvent(int input) = 0;
	virtual States::ID		getStateID() = 0;

	void setIsActive(bool active) { isActive = active; }
	bool getIsActive() { return isActive; }

	void setOneTimeEvent(bool active) { oneTimeEvent = active; }
	bool getOneTimeEvent() { return oneTimeEvent; }


protected:
	void				requestStackPush(States::ID stateID);
	void				requestSetStateActive(States::ID ID, bool active);
	void				requestStackPop();
	void				requestStateClear();

	Context				getContext() const;

	bool isActive;
	bool oneTimeEvent;


private:
	StateStack* mStack;
	Context				mContext;
};

