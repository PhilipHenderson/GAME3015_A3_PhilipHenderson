#include "TitleState.hpp"
#include "Game.hpp"

TitleState::TitleState(StateStack& stack, Context context)
	: State(stack, context), game(context.game), mBackgroundMaterial("background"), mTextMaterial("anyKey"), mSceneGraph(new SceneNode(context.game)), textATime(0.5), textATimeCtr(0.5), textDTime(0.5f), textDTimeCtr(0.5f)
{
	isActive = true;
}

void TitleState::draw()
{
	if (isActive)
	{
		mSceneGraph->draw();
	}
}

bool TitleState::update(const GameTimer& dt)
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
			oneTimeEvent = false;
		}

		if (textATimeCtr <= 0 && anyKeyText->getIsActive())
		{
			anyKeyText->setIsActive(false);

			textATimeCtr = textATime;
		}
		else if (anyKeyText->getIsActive())
		{
			textATimeCtr -= dt.DeltaTime();
		}

		if (textDTimeCtr <= 0 && !anyKeyText->getIsActive())
		{
			anyKeyText->setIsActive(true);

			textDTimeCtr = textDTime;
		}
		else if (!anyKeyText->getIsActive())
		{
			textDTimeCtr -= dt.DeltaTime();
		}

		mSceneGraph->update(dt);
	}

	return true;
}

bool TitleState::handleEvent(int input)
{
	if (isActive)
	{
		requestSetStateActive(States::Menu, true);
		isActive = false;
	}
	
	return true;
}

void TitleState::buildRenderItems()
{
	std::unique_ptr<UIObject> background(new UIObject(mBackgroundMaterial, game));
	background->setScale(1.5, 1.0, 1.0);
	mSceneGraph->attachChild(std::move(background));

	std::unique_ptr<UIObject> anyKey(new UIObject(mTextMaterial, game));
	anyKeyText = anyKey.get();
	anyKeyText->setPosition(0, 0.5, 0);
	anyKeyText->setScale(0.5f, 1.0f, 0.4f);
	mSceneGraph->attachChild(std::move(anyKey));

	mSceneGraph->build();
}