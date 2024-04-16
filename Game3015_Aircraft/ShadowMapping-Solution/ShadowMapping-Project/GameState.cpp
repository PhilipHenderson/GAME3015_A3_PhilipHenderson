#include "GameState.hpp"
#include "Game.hpp"


GameState::GameState(StateStack& stack, Context context)
	: State(stack, context)
	, mWorld(context.game)
	, mPlayer(*context.player)
	, game(context.game)
	, inputDelayTime(0.5f)
	, inputDelayTimeCtr(0.5f)
	, inputActive(false)
{
}

void GameState::draw()
{
	if (isActive)
	{
		mWorld.draw();
	}
}

bool GameState::update(const GameTimer& dt)
{
	if (isActive && !game->isPaused)
	{
		if (oneTimeEvent) 
		{
			Camera temp;

			game->mCamera = temp;
			game->mCamera.SetPosition(0.0f, 0.0f, -10.0f);
			game->mCamera.Pitch(3.14 / 6);
			game->mCamera.UpdateViewMatrix();

			game->isInGame = true;

			inputActive = false;
			inputDelayTimeCtr = inputDelayTime;

			if (game->isNewGame)
			{
				mWorld.Reset();
			}

			game->isNewGame = false;

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

		mWorld.update(dt);

		CommandQueue& commands = mWorld.getCommandQueue();
		mPlayer.handleRealtimeInput(commands);
	}

	return true;
}

bool GameState::handleEvent(int input)
{
	if (isActive && !game->isPaused && inputActive)
	{
		// Game input handling
		CommandQueue& commands = mWorld.getCommandQueue();
		mPlayer.handleEvent(input, commands);

		if (input == 'P')
		{
			requestSetStateActive(States::Pause, true);

			game->isPaused = true;
		}
	}

	return true;
}

void GameState::buildRenderItems() 
{
	mWorld.buildScene();
}