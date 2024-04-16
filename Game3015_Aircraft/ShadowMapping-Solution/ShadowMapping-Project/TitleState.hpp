#pragma once
#include "State.hpp"
#include "SceneNode.hpp"
#include "UIObject.hpp"
#include <string>
#include <vector>

class TitleState : public State
{
public:
	TitleState(StateStack& stack, Context context);

	virtual void		draw();
	virtual bool		update(const GameTimer& dt);
	virtual bool		handleEvent(int input);
	virtual void		buildRenderItems();
	virtual States::ID		getStateID() { return States::Title; }


private:
	std::string					mBackgroundMaterial;
	std::string					mTextMaterial;
	Game* game;

	SceneNode*					mSceneGraph;

	UIObject* anyKeyText;

	float textATime;
	float textATimeCtr;

	float textDTime;
	float textDTimeCtr;
};

