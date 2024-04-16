#include "State.hpp"
#include "StateStack.hpp"
#include "StateStack.hpp"


State::Context::Context(Game* game, Player& player)
	: game(game)
	, player(&player)
{
}

State::State(StateStack& stack, Context context)
	: mStack(&stack)
	, mContext(context)
	, isActive(false)
	, oneTimeEvent(true)
{
}

State::~State()
{
}

void State::requestStackPush(States::ID stateID)
{
	mStack->pushState(stateID);
}

void State::requestSetStateActive(States::ID ID, bool active)
{
	mStack->setStateActive(ID, active);
}

void State::requestStackPop()
{
	mStack->popState();
}

void State::requestStateClear()
{
	mStack->clearStates();
}

State::Context State::getContext() const
{
	return mContext;
}