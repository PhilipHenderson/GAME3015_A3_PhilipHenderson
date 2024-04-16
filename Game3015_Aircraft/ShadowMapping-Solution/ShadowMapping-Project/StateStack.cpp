#include "StateStack.hpp"
#include "Game.hpp"
#include <cassert>


StateStack::StateStack(State::Context context)
	: mStack()
	, mPendingList()
	, mContext(context)
	, mFactories()
	, game(context.game)
{
}

void StateStack::update(const GameTimer& dt)
{
	// Iterate from top to bottom, stop as soon as update() returns false
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (!(*itr)->update(dt))
			break;
	}

	applyPendingChanges();
}

void StateStack::buildRenderItems()
{
	for (State::Ptr& state : mStack)
		state->buildRenderItems();
}

void StateStack::draw()
{
	// Draw all active states from bottom to top
	for (State::Ptr& state : mStack)
		state->draw();
}

void StateStack::handleEvent(int input)
{
	// Iterate from top to bottom, stop as soon as handleEvent() returns false
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (!(*itr)->handleEvent(input))
			break;
	}

	applyPendingChanges();
}

// Takes the target state, find out wif activeate or deactivate

void StateStack::setStateActive(States::ID ID, bool active)
{
	for (auto& state : mStack)
	{
		if (state->getStateID() == ID)
		{
			state->setIsActive(active);
		}

		state->setOneTimeEvent(true);
	}
	game->isInGame = false;
}

void StateStack::pushState(States::ID stateID)
{
	mPendingList.push_back(PendingChange(Push, stateID));
	applyPendingChanges();
}

void StateStack::popState()
{
	mPendingList.push_back(PendingChange(Pop));
}

void StateStack::clearStates()
{
	mPendingList.push_back(PendingChange(Clear));
}

bool StateStack::isEmpty() const
{
	return mStack.empty();
}

State::Ptr StateStack::createState(States::ID stateID)
{
	auto found = mFactories.find(stateID);
	assert(found != mFactories.end());

	return found->second();
}

void StateStack::applyPendingChanges()
{
	for (PendingChange change : mPendingList)
	{
		switch (change.action)
		{
		case Push:
			mStack.push_back(createState(change.stateID));
			break;

		case Pop:
			mStack.pop_back();
			break;

		case Clear:
			mStack.clear();
			break;
		}
	}

	mPendingList.clear();
}

StateStack::PendingChange::PendingChange(Action action, States::ID stateID)
	: action(action)
	, stateID(stateID)
{
}