#include <iostream>
#include "Game.h"

class Background : public Game
{
    private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::RectangleShape shape;
    bool isWall;

    public:
    
    Background(std::string texturePath, float x, float y, bool wall) : isWall(wall)
    {
        if(!texture.loadFromFile(texturePath))
        {
            std::cout<<"Blad w ladowaniu grafiki!!!"<<std::endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition(x,y);

        shape.setSize(sf::Vector2f(48.0f, 48.0f));
        shape.setPosition(x, y);

        if (isWall)
        {
            shape.setFillColor(sf::Color(70, 70, 70));      
            shape.setOutlineColor(sf::Color(50, 50, 50));   
            shape.setOutlineThickness(-1.0f);
        }
        else
        {
            shape.setFillColor(sf::Color(150, 150, 150));   
            shape.setOutlineColor(sf::Color(130, 130, 130)); 
            shape.setOutlineThickness(-1.0f);
        }
    }

    void update() override {}

    void draw(sf::RenderWindow &window) override
    {
        window.draw(shape);
    }
    
    sf::FloatRect getBounds() override 
    {
        return shape.getGlobalBounds();
    } 

    bool getIsWall() const
    {
        return isWall;
    }
};