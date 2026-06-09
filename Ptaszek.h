// #pragma once
// #include <SFML/Graphics.hpp>
// #include <iostream>
// #include "Game.h"

// class Ptaszek : public Game
// {
//     private:
//     sf::Texture texture;
//     sf::Sprite sprite;
//     float baseY;
//     float animationTimer;


//     public:
//     Ptaszek(const std::string& texturePath, float x, float y) : animationTimer(0.0f), baseY(y)
//     {
//         if(!texture.loadFromFile("ptaszek.png"))
//         {
//             std::cout<<"Bland w ladowaniu ptaszka"<<std::endl;
//         }
//         sprite.setTexture(texture);
//         sprite.setPosition(x,y);
//     }

//     ~Ptaszek() override = default;

//     void update(float deltaTime) override
//     {
//         animationTimer += deltaTime * 5.0f;
//         float offset = std::
//     }
//     void draw(sf::RenderWindow& window) override;

//     sf::FloatRect getBounds() override;
//     bool isSolid() const override;

// };