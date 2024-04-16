#include "PauseState.hpp"
#include "Game.hpp"

PauseState::PauseState(StateStack& stack, Context context)
	: State(stack, context)
	//, mOptions()
	, mOptionIndex(0), game(context.game), mBackgroundMaterial("pauseBackground"), mTextMaterialResume("resume"), mTextMaterialMenu("menu"), mSceneGraph(new SceneNode(context.game)), mTextMaterialResumeActive("resumeActive"), mTextMaterialMenuActive("menuActive")
{
	updateOptionText();
}

void PauseState::draw()
{
	if (isActive)
	{
		mSceneGraph->draw();
	}
}

bool PauseState::update(const GameTimer& dt)
{
	if (isActive)
	{
		if (oneTimeEvent)
		{
			gameStateCamera = game->mCamera;

			Camera temp;

			game->mCamera = temp;
			game->mCamera.SetPosition(0.0f, 37.0f, 0.0f);
			game->mCamera.Pitch(3.14 / 2);
			game->mCamera.UpdateViewMatrix();

			oneTimeEvent = false;
		}

		mSceneGraph->update(dt);
	}

	return true;
}

bool PauseState::handleEvent(int input)
{
	if (isActive)
	{
		if (input == VK_RETURN)
		{
			if (mOptionIndex == Resume)
			{
				game->isPaused = false;

				isActive = false;
			}
			else if (mOptionIndex == Menu)
			{
				game->isPaused = false;
				requestSetStateActive(States::Game, false);
				requestSetStateActive(States::Menu, true);
				isActive = false;
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

void PauseState::updateOptionText()
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

void PauseState::buildRenderItems()
{
	std::unique_ptr<UIObject> background(new UIObject(mBackgroundMaterial, game));
	background->setScale(1.5, 1.0, 1.0);
	mSceneGraph->attachChild(std::move(background));

	std::unique_ptr<UIObject> playButton(new UIObject(mTextMaterialResume, game));
	playButton->setScale(0.45f, 1.0f, 0.15f);
	playButton->setPosition(0.0f, 0.5f, 5.0f);
	mSceneGraph->attachChild(std::move(playButton));

	std::unique_ptr<UIObject> quitButton(new UIObject(mTextMaterialMenu, game));
	quitButton->setScale(0.45f, 1.0f, 0.15f);
	quitButton->setPosition(0.0f, 0.5f, -5.0f);
	mSceneGraph->attachChild(std::move(quitButton));

	std::unique_ptr<UIObject> playButtonActive(new UIObject(mTextMaterialResumeActive, game));
	mOptions.push_back(playButtonActive.get());
	playButtonActive->setScale(0.45f, 1.0f, 0.15f);
	playButtonActive->setPosition(0.0f, 1.0f, 5.0f);
	mSceneGraph->attachChild(std::move(playButtonActive));

	std::unique_ptr<UIObject> quitButtonActive(new UIObject(mTextMaterialMenuActive, game));
	mOptions.push_back(quitButtonActive.get());
	quitButtonActive->setScale(0.45f, 1.0f, 0.15f);
	quitButtonActive->setPosition(0.0f, 1.0f, -5.0f);
	quitButtonActive->setIsActive(false);
	mSceneGraph->attachChild(std::move(quitButtonActive));

	mSceneGraph->build();
}
