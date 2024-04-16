#pragma once

# include "Entity.hpp"
#include <string>

class UIObject : public Entity
{

public:
	UIObject(std::string sprite, Game* game);

	virtual unsigned int	getCategory() const;
	void setIsActive(bool active) { isActive = active; }
	bool getIsActive() { return isActive; }

private:
	virtual void		drawCurrent() const;
	virtual void		buildCurrent();


private:
	std::string			mSprite;
	bool isActive;

};

