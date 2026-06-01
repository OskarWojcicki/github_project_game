#include <iostream>
#include "Game.h"
#include "Background.h"
#include "Link.h"

enum class GameState
{
    MainMenu,
    Gameplay,
    GameOver
};

Link* player = nullptr;

const char room1[11][15]=
{
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','F'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','F'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','F'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
};

const char room2[11][15]=
{
    {'#','#','#','#','#','#','F','F','F','#','#','#','#','#','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'F','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'F','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'F','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
};

const char room3[11][15]=
{
    {'#','#','#','#','#','#','F','F','F','#','#','#','#','#','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','#','#','#','#','#','F','F','F','#','#','#','#','#','#'}
};

const char room4[11][15]=
{
    {'#','#','#','#','#','#','F','F','F','#','#','#','#','#','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','#','#','#','#','#','F','F','F','#','#','#','#','#','#'}
};

const char room5[11][15]=
{
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'F','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'F','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'F','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
};

const char room6[11][15]=
{
    {'#','#','#','#','#','#','F','F','F','#','#','#','#','#','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','F'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','F'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','F'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','#','#','#','#','#','F','F','F','#','#','#','#','#','#'}
};

const char room7[11][15]=
{
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'F','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'F','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'F','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','#','#','#','#','#','F','F','F','#','#','#','#','#','#'}
};

const char room8[11][15]=
{
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'F','F','F','F','F','F','F','F','F','F','F','F','F','F','F'},
    {'F','F','F','F','F','F','F','F','F','F','F','F','F','F','F'},
    {'F','F','F','F','F','F','F','F','F','F','F','F','F','F','F'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
};

const char room9[11][15]=
{
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','F'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','F'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','F'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
};

const char (*worldMap[5][5])[15]=
{
    {room9,room8,room7,nullptr,nullptr},
    {nullptr,nullptr,room6,room5,nullptr},
    {nullptr,nullptr,room4,nullptr,nullptr},
    {nullptr,nullptr,room3,nullptr,nullptr},
    {nullptr,room1,room2,nullptr,nullptr},
};

void Rooms(int wx, int wy, std::vector<Game*>& wordlObjects, float startX, float startY)
{
    player = nullptr;

    for(auto& object : wordlObjects)
    {
        delete object;
    }
    wordlObjects.clear();

    const char (*selectedRoom)[15] = worldMap[wy][wx];

    if(selectedRoom == nullptr)
    {
        return;
    }

    float rozmiar_kafelka = 48.0f;

    for(int row = 0; row < 11; ++row)
    {
        for(int col = 0; col < 15; ++col)
        {
            float posX = col * rozmiar_kafelka;
            float posY = row * rozmiar_kafelka;

            if(selectedRoom[row][col] == '#')
            {   
                wordlObjects.push_back(new Background("drzewa.png", posX, posY, true));
            }
            else if(selectedRoom[row][col] == 'F')
            {
                wordlObjects.push_back(new Background("floor1.png", posX, posY, false));
            }
        }
    }
    player = new Link(startX, startY);
    wordlObjects.push_back(player);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(720, 528), "The legend of Zelda");
    window.setFramerateLimit(60);

    GameState currentState = GameState::MainMenu;
    int worldX = 1;
    int worldY = 4;

    sf::Font font;
    if(!font.loadFromFile("Triforce-y07d.ttf"))
    {
        std::cout << "!!! Blad w ladowaniu trzcionki !!!" << std::endl;
    }

    sf::Text TitleText("The legends of", font, 30);
    TitleText.setFillColor(sf::Color::White);

    sf::Text TitleText2("Zelda", font, 150);
    TitleText2.setFillColor(sf::Color::White);
    
    sf::Text menuText("NACISNIJ ENTER", font, 18);
    menuText.setFillColor(sf::Color::White);

    float width1 = TitleText.getLocalBounds().width;
    TitleText.setPosition((720.0f - width1) / 2.0f, 50.0f);

    float width2 = TitleText2.getLocalBounds().width;
    TitleText2.setPosition((720.0f - width2) / 2.0f, 80.0f);

    float width3 = menuText.getLocalBounds().width;
    menuText.setPosition((720.0f - width3) / 2.0f, 400.0f);

    std::vector<Game*> worldObjects;
    sf::Clock clock;

    while(window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        sf::Event event;

        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }

            if(currentState == GameState::MainMenu)
            {
                if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
                {
                    std::cout << "Gracz wcisnal Enter. Ladowanie gry ...\n";
                    worldX = 1;
                    worldY = 4;

                    Rooms(worldX, worldY, worldObjects, 340.0f, 240.0f);
                    currentState = GameState::Gameplay;
                }
            }

            if(currentState == GameState::Gameplay && player != nullptr)
            {
                player->handleEvents(event); 
            }
        }

        sf::Vector2f oldPlayerPos(0.0f, 0.0f);
        if(player != nullptr)
        {
            oldPlayerPos = player->getPosition(); 
        }

        for (auto& object : worldObjects)
        {
            object->update(deltaTime);
        }

        if(currentState == GameState::Gameplay && player != nullptr)
        {
            sf::Vector2f playerPos = player->getPosition();

            if(playerPos.x > 720.0f)
            {
                if(worldX + 1 < 5 && worldMap[worldY][worldX + 1] != nullptr)
                {
                    worldX++;
                    Rooms(worldX, worldY, worldObjects, 20.0f, playerPos.y);
                }
                else
                {
                    player->setPosition(710.0f, playerPos.y);
                }
            }
            else if(playerPos.x < 0.0f)
            {
                if(worldX - 1 >= 0 && worldMap[worldY][worldX - 1] != nullptr)
                {
                    worldX--;
                    Rooms(worldX, worldY, worldObjects, 700.0f, playerPos.y);
                }
                else
                {
                    player->setPosition(10.0f, playerPos.y);
                }
            }
            else if(playerPos.y > 528.0f)
            {
                if(worldY + 1 < 5 && worldMap[worldY + 1][worldX] != nullptr)
                {
                    worldY++;
                    Rooms(worldX, worldY, worldObjects, playerPos.x, 20.0f);
                }
                else
                {
                    player->setPosition(playerPos.x, 510.0f);
                }
            }
            else if(playerPos.y < 0.0f)
            {
                if(worldY - 1 >= 0 && worldMap[worldY - 1][worldX] != nullptr)
                {
                    worldY--;
                    Rooms(worldX, worldY, worldObjects, playerPos.x, 500.0f);
                }
                else 
                {
                    player->setPosition(playerPos.x, 10.0f);
                }
            }
        }

        if(currentState == GameState::Gameplay && player != nullptr)
        {
            sf::FloatRect playerBounds = player->getBounds();

            for(auto& object : worldObjects)
            {
                if(object == player)
                {
                    continue;
                }
                if(object->isSolid())
                {
                    sf::FloatRect wallBounds = object->getBounds();
                    sf::FloatRect overlap;

                    if(playerBounds.intersects(wallBounds, overlap)) 
                    {
                        bool wasLeft   = (oldPlayerPos.x + playerBounds.width <= wallBounds.left);
                        bool wasRight  = (oldPlayerPos.x >= wallBounds.left + wallBounds.width);
                        bool wasTop    = (oldPlayerPos.y + playerBounds.height <= wallBounds.top);
                        bool wasBottom = (oldPlayerPos.y >= wallBounds.top + wallBounds.height);

                        if(wasLeft && !wasTop && !wasBottom)
                        {
                            player->setPosition(wallBounds.left - playerBounds.width, playerBounds.top);
                        }
                        else if (wasRight && !wasBottom && !wasTop)
                        {
                            player->setPosition(wallBounds.left + wallBounds.width, playerBounds.top);
                        }
                        else if (wasTop && !wasLeft && !wasRight)
                        {
                            player->setPosition(playerBounds.left, wallBounds.top - playerBounds.height);
                        }
                        else if (wasBottom && !wasLeft && !wasRight)
                        {
                            player->setPosition(playerBounds.left, wallBounds.top + wallBounds.height);
                        }
                        else
                        {
                            if(overlap.width < overlap.height)
                            {
                                if(playerBounds.left + (playerBounds.width / 2.0f) < wallBounds.left + (wallBounds.width / 2.0f))
                                {
                                    player->setPosition(wallBounds.left - playerBounds.width, playerBounds.top);
                                }
                                else
                                {
                                    player->setPosition(wallBounds.left + wallBounds.width, playerBounds.top);
                                }
                            }
                            else
                            {
                                if(playerBounds.top + (playerBounds.height / 2.0f) < wallBounds.top + (wallBounds.height / 2.0f))
                                {
                                    player->setPosition(playerBounds.left, wallBounds.top - playerBounds.height);
                                }
                                else
                                {
                                    player->setPosition(playerBounds.left, wallBounds.top + wallBounds.height);
                                }
                            }
                        }
                        playerBounds = player->getBounds();
                    }
                }
            }
        }
        
        window.clear(sf::Color::Black);

        if(currentState == GameState::MainMenu)
        {
            window.draw(TitleText);
            window.draw(menuText);
            window.draw(TitleText2);
        }
        else if(currentState == GameState::Gameplay)
        {
            for(auto& object : worldObjects)
            {
                object->draw(window);
            }
        }
        window.display();
    }

    for(auto& object : worldObjects)
    {
        delete object;
    }
    worldObjects.clear();

    return 0;
}
