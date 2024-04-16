#include "World.hpp"
#include "Sky.hpp"

World::World(Game* game)
	: mSceneGraph(new SceneNode(game))
	, mGame(game)
	, mPlayerAircraft(nullptr)
	, mBackground(nullptr)
	, mWorldBounds(-1.5f, 1.5f, 200.0f, 0.0f) //Left, Right, Down, Up
	, mSpawnPosition(0.f, 0.f)
	, mScrollSpeed(1.0f)
{
}

void World::update(const GameTimer& gt)
{

	mPlayerAircraft->setVelocity(0.0f, 0.0f, 0.0f);
	while (!mCommandQueue.isEmpty())
		mSceneGraph->onCommand(mCommandQueue.pop(), gt);

	adaptPlayerVelocity();

	mSceneGraph->update(gt);

	adaptPlayerPosition();
}

void World::draw()
{
	mSceneGraph->draw();
}

void World::buildScene()
{
	std::unique_ptr<Sky> sky(new Sky(mGame));
	sky->setPosition(0.0, 0.0, 0.0);
	sky->setScale(1.0, 1.0, 1.0);
	mSceneGraph->attachChild(std::move(sky));

	std::unique_ptr<Aircraft> player(new Aircraft(Aircraft::Eagle, mGame));
	mPlayerAircraft = player.get();
	mPlayerAircraft->setPosition(0, -5, 0.0);
	mPlayerAircraft->setScale(0.003f, 0.003f, 0.003f);
	//mPlayerAircraft->setVelocity(0, 0, mScrollSpeed);
	mSceneGraph->attachChild(std::move(player));

	std::unique_ptr<Aircraft> enemy1(new Aircraft(Aircraft::Raptor, mGame));
	auto raptor = enemy1.get();
	raptor->setPosition(2, 0, -5);
	mPlayerAircraft->attachChild(std::move(enemy1));

	std::unique_ptr<Aircraft> enemy2(new Aircraft(Aircraft::Raptor, mGame));
	auto raptor2 = enemy2.get();
	raptor2->setPosition(-2, 0, -5);
	mPlayerAircraft->attachChild(std::move(enemy2));

	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(mGame));
	mBackground = backgroundSprite.get();
	//mBackground->setPosition(mWorldBounds.left, mWorldBounds.top);
	mBackground->setPosition(0, -6.0, 0.0);
	mBackground->setScale(3.0, 1.0, 3.0);
	mBackground->setWorldRotation(0, 0, 0);
	mBackground->setScale(1.0, 1.0, 1.0);
	//mBackground->setVelocity(-mScrollSpeed, 0);
	mSceneGraph->attachChild(std::move(backgroundSprite));


	mSceneGraph->build();
}

CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}

void World::adaptPlayerPosition()
{
	//// Keep player's position inside the screen bounds, at least borderDistance units from the border
	//sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
	//const float borderDistance = 100.f;

	//sf::Vector2f position = mPlayerAircraft->getPosition();
	//position.x = std::max(position.x, viewBounds.left + borderDistance);
	//position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
	//position.y = std::max(position.y, viewBounds.top + borderDistance);
	//position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
	//mPlayerAircraft->setPosition(position);
}

void World::adaptPlayerVelocity()
{
	XMFLOAT3 velocity = mPlayerAircraft->getVelocity();

	// If moving diagonally, reduce velocity (to have always same velocity)
	if (velocity.x != 0.0f && velocity.y != 0.0f)
		mPlayerAircraft->setVelocity(velocity.x / std::sqrt(2.f), velocity.y / std::sqrt(2.f), velocity.z / std::sqrt(2.f));

	// Add scrolling velocity
	//mPlayerAircraft->accelerate(0.0f, 0.0f, mScrollSpeed);
}

void World::Reset()
{
	mPlayerAircraft->setPosition(0, -5.0, 0.0);
}
