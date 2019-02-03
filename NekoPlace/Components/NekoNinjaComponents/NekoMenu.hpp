//
//  NekoMenu.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 29/01/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef NekoMenu_hpp
#define NekoMenu_hpp

#include <iostream>
#include <fstream>
#include <vector>
#include <list>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "../../Essentials/ResourcePath.hpp"
#include "../../Engine/EntitySystem.hpp"
#include "../../Engine/StaticMethods.hpp"
#include "../../Engine/GUIInterface.hpp"

#include "NekoNinja.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::list;

using namespace ns;

namespace NekoNinja
{
    struct Room
    {
        static int roomWidth, roomHeight;
        static float x, y;
        static float scale;
        
        bool active{ false };
        int move_dx{ 0 }, move_dy{ 0 };
        
        sf::Sprite sprite;
        bool spriteLoaded{ false };
        
        Room();
        ~Room();
        void Switch(bool on);
        void Destroy();
        void Update(const sf::Time& elapsedTime);
        void PollEvent(sf::Event& event);
        void Resize(unsigned int width, unsigned int height);
        void Draw(sf::RenderWindow* window);
    };
    
    
    struct MainMenu : Component
    {
        Room room;
        sf::Sprite background;
        sf::Sprite neko;
        bool spriteLoaded{ false };
        bool lastInGame{ false };
        
        enum class Screen { main, room, account };
        Screen screen{ Screen::main };
        
        GUI::TextButton playButton;
        GUI::SpriteButton roomButton;
        GUI::SpriteButton roomBackButton;
        GUI::TextButton accountButton;
        GUI::TextButton accountBackButton;
        GUI::TextButton resetButton;
        
        sf::Text dialogueText;
        GUI::SpriteButton dialogueButton;
        
        sf::Text helperText;
        sf::RectangleShape backShape;
        sf::RectangleShape accountBackShape;
        sf::RectangleShape expShape;
        
        void Init() override;
        void Destroy() override;
        void Update(const sf::Time& elapsedTime) override;
        void PollEvent(sf::Event& event) override;
        void Resize(unsigned int width, unsigned int height) override;
        void Draw(sf::RenderWindow* window) override;
    };
}

#endif /* NekoMenu_hpp */
