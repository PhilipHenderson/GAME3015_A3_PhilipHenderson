#pragma once

#include "State.hpp"
#include "SceneNode.hpp"
#include "UIObject.hpp"
#include <string>
#include <vector>

class MenuState : public State
{
public:
	MenuState(StateStack& stack, Context context);

	virtual void			draw();
	virtual bool			update(const GameTimer& dt);
	virtual bool			handleEvent(int input);
	virtual void			buildRenderItems();
	virtual States::ID		getStateID() { return States::Menu; }

	void					updateOptionText();


private:
	enum OptionNames
	{
		Play,
		Exit,
	};


private:
	std::string					mBackgroundMaterial;
	std::vector<UIObject*>	mOptions;
	std::string					mTextMaterialPlay;
	std::string					mTextMaterialExit;
	std::string					mTextMaterialPlayActive;
	std::string					mTextMaterialExitActive;
	std::size_t					mOptionIndex;

	Game* game;

	SceneNode* mSceneGraph;

	float inputDelayTime;
	float inputDelayTimeCtr;
	bool inputActive;
};

