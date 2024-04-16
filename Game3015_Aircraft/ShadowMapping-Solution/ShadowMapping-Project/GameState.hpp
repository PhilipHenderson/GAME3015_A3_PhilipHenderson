#pragma once

#include "State.hpp"
#include "World.hpp"
#include "Player.hpp"


class GameState : public State
{
public:
	GameState(StateStack& stack, Context context);

	virtual void		draw();
	virtual bool		update(const GameTimer& dt);
	virtual bool		handleEvent(int input);
	virtual void		buildRenderItems();
	virtual States::ID		getStateID() { return States::Game; };


private:
	World				mWorld;
	Player&				mPlayer;
	Game*				game;

	float inputDelayTime;
	float inputDelayTimeCtr;
	bool inputActive;
};

