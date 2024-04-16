#pragma once
#include "Entity.hpp"
#include <string>

class Sky :
    public Entity
{
public:
	enum Type
	{
		Eagle,
		Raptor,
	};


public:
	Sky(Game* game);


private:
	virtual void		drawCurrent() const;
	virtual void		buildCurrent();


private:
	Type				mType;
	std::string			mSprite;
};
