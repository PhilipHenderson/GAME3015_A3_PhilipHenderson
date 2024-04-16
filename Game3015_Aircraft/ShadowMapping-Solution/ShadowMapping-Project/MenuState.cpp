#include "MenuState.hpp"
#include "Game.hpp"

MenuState::MenuState(StateStack& stack, Context context)
	: State(stack, context)
	//, mOptions()
	, mOptionIndex(0), game(context.game), mBackgroundMaterial("background"), mTextMaterialPlay("play"), mTextMaterialExit("exit"), mSceneGraph(new SceneNode(context.game)), mTextMaterialPlayActive("playActive"), mTextMaterialExitActive("exitActive"), inputDelayTime(1.0f), inputDelayTimeCtr(1.0f), inputActive(false)
{
	updateOptionText();
}

void MenuState::draw()
{
	if (isActive)
	{
		mSceneGraph->draw();
	}
}

bool MenuState::update(const GameTimer& dt)
{
	if (isActive)
	{
		if (oneTimeEvent)
		{
			Camera temp;

			game->mCamera = temp;
			game->mCamera.SetPosition(0.0f, 37.0f, 0.0f);
			game->mCamera.Pitch(3.14 / 2);
			game->mCamera.UpdateViewMatrix();

			inputActive = false;
			inputDelayTimeCtr = inputDelayTime;

			game->isNewGame = true;

			oneTimeEvent = false;
		}

		if (inputDelayTimeCtr <= 0 && !inputActive)
		{
			inputActive = true;
		}
		else if (!inputActive)
		{
			inputDelayTimeCtr -= dt.DeltaTime();
		}

		mSceneGraph->update(dt);
	}

	return true;
}

bool MenuState::handleEvent(int input)
{
	if (isActive && inputActive)
	{
		if (input == VK_RETURN)
		{
			if (mOptionIndex == Play)
			{
				requestSetStateActive(States::Game, true);
				isActive = false;
			}
			else if (mOptionIndex == Exit)
			{
				// The exit option was chosen, by removing itself, the stack will be empty, and the game will know it is time to close.
				PostQuitMessage(0);
			}
		}
		else if (input == VK_UP)
		{
			// Decrement
			if (mOptionIndex > 0)
				mOptionIndex--;

			updateOptionText();
		}
		else if (input == VK_DOWN)
		{
			// Increment
			if (mOptionIndex < mOptions.size() - 1)
				mOptionIndex++;

			updateOptionText();
		}
	}

	return true;
}

void MenuState::updateOptionText()
{
	if (mOptions.empty())
		return;

	// White all texts
	for (UIObject* active : mOptions)
	{
		active->setIsActive(false);
	}
		
	// Red the selected text
	mOptions[mOptionIndex]->setIsActive(true);
}

void MenuState::buildRenderItems()
{
	std::unique_ptr<UIObject> background(new UIObject(mBackgroundMaterial, game));
	background->setScale(1.5, 1.0, 1.0);
	mSceneGraph->attachChild(std::move(background));

	std::unique_ptr<UIObject> playButton(new UIObject(mTextMaterialPlay, game));
	playButton->setScale(0.45f, 1.0f, 0.15f);
	playButton->setPosition(0.0f, 0.5f, 3.0f);
	mSceneGraph->attachChild(std::move(playButton));

	std::unique_ptr<UIObject> quitButton(new UIObject(mTextMaterialExit, game));
	quitButton->setScale(0.45f, 1.0f, 0.15f);
	quitButton->setPosition(0.0f, 0.5f, -3.0f);
	mSceneGraph->attachChild(std::move(quitButton));

	std::unique_ptr<UIObject> playButtonActive(new UIObject(mTextMaterialPlayActive, game));
	mOptions.push_back(playButtonActive.get());
	playButtonActive->setScale(0.45f, 1.0f, 0.15f);
	playButtonActive->setPosition(0.0f, 1.0f, 3.0f);
	mSceneGraph->attachChild(std::move(playButtonActive)); 

	std::unique_ptr<UIObject> quitButtonActive(new UIObject(mTextMaterialExitActive, game)); 
	mOptions.push_back(quitButtonActive.get()); 
	quitButtonActive->setScale(0.45f, 1.0f, 0.15f);
	quitButtonActive->setPosition(0.0f, 1.0f, -3.0f);
	quitButtonActive->setIsActive(false);
	mSceneGraph->attachChild(std::move(quitButtonActive)); 

	mSceneGraph->build();
}