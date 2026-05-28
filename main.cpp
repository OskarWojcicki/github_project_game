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
    {'F','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'F','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'F','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','#','#','#','#','#','F','F','F','#','#','#','#','#','#'}
};

const char room2[11][15]=
{
    {'#','#','#','#','#','#','F','F','F','#','#','#','#','#','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','#','#','#','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}

};

void Rooms(int roomNumber, std::vector<Game*>& wordlObjects)
{
    player =nullptr;


    for(auto& object : wordlObjects)
    {
        delete object;
    }
    wordlObjects.clear();


    const char (*selectedRoom)[15];
    if(roomNumber == 1)
    {
        selectedRoom = room1;
    }
    else if(roomNumber == 2)
    {
        selectedRoom = room2;
    }
    else
    {
        return;
    }

    for(int row = 0;row<11;++row)
    {
        for(int col=0;col<15;++col)
        {
            float posX = col * 48.0f;
            float posY = row * 48.0f;


            if(selectedRoom[row][col] == '#')
            {
                wordlObjects.push_back(new Background("/Users/oskar/SFML/ZELDA/drzewa.png",posX, posY, true));
            }
            else if(selectedRoom[row][col] == 'F')
            {
                wordlObjects.push_back(new Background("floor1.png", posX,posY, false));
            }
        }
    }
    player = new Link(340.0f, 240.0f);
     wordlObjects.push_back(player);

}

int main()
{

    sf::RenderWindow window(sf::VideoMode(720,528), "The legend of Zelda");
    window.setFramerateLimit(60);

    GameState currentState = GameState::MainMenu;
    int currentRoom = 1;

    sf::Font font;
    if(!font.loadFromFile("Triforce-y07d.ttf"))
    {
        std::cout<<"!!! Blad w ladowaniu trzcionki !!!"<<std::endl;
    }

    sf:: Text TitleText("The legends of", font, 30);
    TitleText.setFillColor(sf::Color::White);

    sf:: Text TitleText2("Zelda", font, 150);
    TitleText2.setFillColor(sf::Color::White);
    
    sf:: Text menuText("NACISNIJ ENTER", font, 18);
    menuText.setFillColor(sf::Color::White);

    
    float width1 = TitleText.getLocalBounds().width;
    TitleText.setPosition((720.0f-width1)/2.0f, 50.0f);

    float width2 = TitleText2.getLocalBounds().width;
    TitleText2.setPosition((720.0f-width2)/2.0f, 80.0f);

    float width3 = menuText.getLocalBounds().width;
    menuText.setPosition((720.0f-width3)/2.0f, 400.0f);

    std::vector<Game*> wordlObjects;

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
                    std::cout<<"Gracz wcisnal Enter. Ladowanie gry ...\n";
                    currentRoom = 1;
                    Rooms(currentRoom,wordlObjects);
                    currentState = GameState::Gameplay;
                }
            }

            
            if(currentState == GameState::Gameplay && player != nullptr)
            {
                player->handleEvents(event); 
            }
        }

            
        for (auto& object : wordlObjects)
        {
            object -> update();
        }

        if(currentState == GameState::Gameplay && player !=nullptr)
        {
            sf::Vector2f playerPos = player -> getPosition();

            if(currentRoom == 1 && playerPos.y > 528.0f)
            {
                currentRoom = 2;
                Rooms(currentRoom,wordlObjects);

                player -> setPosition(playerPos.x,10.0f);
            }
            else if(currentRoom == 2&& playerPos.y<0.0f)
            {
                currentRoom = 1;
                Rooms(currentRoom, wordlObjects);

                player -> setPosition(playerPos.x,528.0f-48.0f);
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
            
            for(auto& object : wordlObjects)
            {
                object -> draw(window);
            }
        }
        window.display();
    }

    for(auto& object : wordlObjects)
    {
        delete object;
    }
    wordlObjects.clear();

    return 0;
}