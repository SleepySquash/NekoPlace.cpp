//
//  Apartment.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 17/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef Apartment_hpp
#define Apartment_hpp

#include <iostream>
#include <fstream>
#include <vector>
#include <list>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "../../Essentials/ResourcePath.hpp"
#include "../../Engine/EntitySystem.hpp"
#include "../../Engine/Settings.hpp"
#include "../../Engine/Collectors.hpp"
#include "../../Engine/GUIInterface.hpp"

#include "Player.hpp"
#include "RoomLibrary.hpp"
#include "NekoEntity.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::list;
using ns::base::utf8;
using ns::base::utf16;

using namespace ns;

namespace NekoUI
{
    struct Apartment : Component
    {
        NekoEntity neko;
        sf::Sprite sprite;
        bool spriteLoaded, active{ true }, hasFocusOnNeko{ false }, pressedNeko{ false };
        
        sf::Vector2i dot, pressedNekoPos, middleZoom{ 0, 0 };
        float move_dx{ 0 }, move_dy{ 0 };
        float scrollSensitivity{ 0.02f }, prevZoom{ 1 }, prevDistanceZoom{ 1.f };
        
        Apartment();
        void Init() override;
        void Update(const sf::Time& elapsedTime) override;
        void PollEvent(sf::Event& event) override;
        void Resize(unsigned int width, unsigned int height) override;
        void Draw(sf::RenderWindow* window) override;
        void RecieveMessage(MessageHolder& message) override;
        void CalculateCameraPosition();
        void CalculateCameraScale();
    };
}

#endif /* Apartment_hpp */
