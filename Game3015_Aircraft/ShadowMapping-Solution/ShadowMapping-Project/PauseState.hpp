#pragma once
#include "State.hpp"
#include "SceneNode.hpp"
#include "UIObject.hpp"
#include <string>
#include <vector>

class PauseState : public State
{
public:
	PauseState(StateStack& stack, Context context);

	virtual void			draw();
	virtual bool			update(const GameTimer& dt);
	virtual bool			handleEvent(int input);
	virtual void			buildRenderItems();
	virtual States::ID		getStateID() { return States::Pause; }

	void					updateOptionText();


private:
	enum OptionNames
	{
		Resume,
		Menu,
	};


private:
	std::string					mBackgroundMaterial;
	std::vector<UIObject*>	mOptions;
	std::string					mTextMaterialResume;
	std::string					mTextMaterialMenu;
	std::string					mTextMaterialResumeActive;
	std::string					mTextMaterialMenuActive;
	std::size_t					mOptionIndex;

	Game* game;

	SceneNode* mSceneGraph;

	Camera gameStateCamera;
};

