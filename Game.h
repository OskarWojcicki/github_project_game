#pragma once
#include <SFML/Graphics.hpp>

class Game
{
    public:
    virtual void update()=0;
    virtual void draw(sf::RenderWindow &window)=0;
    virtual sf::FloatRect getBounds() =0;
    virtual bool isSolid() const 
    {
        return false;
    }

    virtual ~Game() {}
};