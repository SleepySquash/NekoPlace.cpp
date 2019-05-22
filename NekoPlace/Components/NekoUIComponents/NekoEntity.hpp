//
//  NekoEntity.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 17/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef NekoEntity_hpp
#define NekoEntity_hpp

#include <iostream>

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

#include "../../Essentials/ResourcePath.hpp"
#include "../../Engine/Settings.hpp"
#include "../../Engine/Collectors.hpp"

#include "Player.hpp"
#include "RoomLibrary.hpp"

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
    struct NekoEntity
    {
        float x, y, relScale{ 1.f };
        sf::Sprite sprite;
        bool spriteLoaded{ false };
        
        int occupiedAt{ -1 };
        bool beingActionedWith{ false };
        bool moveRight{ false }, moveLeft{ false }, moveDown{ false }, moveUp{ false };
        float xySpd{ 140 };
        
        enum class Mood { happy, sad, peaceful, desprate, angry, horny };
        Mood mood{ Mood::peaceful };
        std::wstring status{ L"" };
        float affection{ 0 };
        
        float movingTo_x{ 0 }, movingTo_y{ 0 };
        bool newPoint{ true };
        
        bool drawDialogue{ false };
        sf::Text dialogue; sf::Sprite dialogueSprite;
        float elapsedWaiting{ 0 }, waitingTime{ 1.2f }, elapsedDialogue{ (rand() % 12000)/1000.f };
        
        void Init();
        void Destroy();
        void Update(const sf::Time& elapsedTime);
        void UpdateDepthPosition();
        void PollEvent(sf::Event& event);
        void Resize();
        void UpdatePosition();
        void ResizeDialogue();
        void Draw(sf::RenderWindow* window);
        std::wstring GenerateRoomDialogue();
    };
}

#endif /* NekoEntity_hpp */
