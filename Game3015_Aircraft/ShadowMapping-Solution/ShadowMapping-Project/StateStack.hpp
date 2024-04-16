#pragma once

#include "State.hpp"
#include "StateIdentifiers.hpp"

#include <vector>
#include <utility>
#include <functional>
#include <map>

class StateStack
{
public:
	enum Action
	{
		Push,
		Pop,
		Clear,
	};


public:
	explicit			StateStack(State::Context context);

	template <typename T>
	void				registerState(States::ID stateID);

	void				update(const GameTimer& dt);
	void				buildRenderItems();
	void				draw();
	void				handleEvent(int input);

	void				pushState(States::ID stateID);
	void				popState();
	void				clearStates();
	void				setStateActive(States::ID ID, bool active);

	bool				isEmpty() const;

	void				applyPendingChanges();
private:
	State::Ptr			createState(States::ID stateID);
	


private:
	struct PendingChange
	{
		explicit			PendingChange(Action action, States::ID stateID = States::None);

		Action				action;
		States::ID			stateID;
	};


private:
	std::vector<State::Ptr>								mStack;
	std::vector<PendingChange>							mPendingList;

	State::Context										mContext;
	std::map<States::ID, std::function<State::Ptr()>>	mFactories;

	Game* game;
};


template <typename T>
void StateStack::registerState(States::ID stateID)
{
	mFactories[stateID] = [this]()
		{
			return State::Ptr(new T(*this, mContext));
		};
}

