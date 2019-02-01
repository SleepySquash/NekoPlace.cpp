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
    struct MainMenu : Component
    {
        sf::Sprite background;
        sf::Sprite neko;
        bool spriteLoaded{ false };
        
        GUIButton playButton;
        
        void Init() override;
        void Destroy() override;
        void PollEvent(sf::Event& event) override;
        void Resize(unsigned int width, unsigned int height) override;
        void Draw(sf::RenderWindow* window) override;
    };
}

#endif /* NekoMenu_hpp */
