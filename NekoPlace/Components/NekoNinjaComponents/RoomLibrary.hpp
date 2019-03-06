//
//  RoomLibrary.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 27/02/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef RoomLibrary_hpp
#define RoomLibrary_hpp

#include <iostream>
#include <fstream>
#include <vector>
#include <list>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "../../Essentials/ResourcePath.hpp"
#include "../../Engine/StaticMethods.hpp"
#include "../../Engine/GUIInterface.hpp"

#include "../VNLightComponents/Novel.hpp"
#include "NekoBase.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::list;
using ns::base::utf8;
using ns::base::utf16;

using namespace ns;

namespace NekoNinja
{
    struct NekoEntity;
    struct RoomLibrary { static vector<NekoEntity*> neko; };
    namespace roomLibrary { void clear(); }
    struct NekoEntity
    {
        sf::Sprite sprite, dialogueSprite;
        bool spriteLoaded{ false };
        NekoBase* info{ nullptr };
        unsigned long positionInArray{ 0 };
        
        int occupiedAt{ -1 };
        bool inited{ false };
        float x{ 0 }, y{ 0 };
        
        bool beingActionedWith{ false };
        bool moveRight{ false }, moveLeft{ false }, moveDown{ false }, moveUp{ false };
        float xySpd{ 140 };
        
        enum class Mood { happy, sad, peaceful, desprate, angry, horny };
        Mood mood{ Mood::peaceful };
        std::wstring status{ L"" };
        float affection{ 0 };
        
        float movingTo_x{ 0 }, movingTo_y{ 0 };
        bool newPoint{ true }, drawDialogue{ false };
        sf::Text dialogue;
        float elapsedWaiting{ 0 }, waitingTime{ 1.2f }, elapsedDialogue{ (rand() % 12000)/1000.f };
        
        NekoEntity(NekoBase* info, unsigned long positionInArray = 0) : info(info), positionInArray(positionInArray) { if (info) xySpd = info->xySpd; }
        void Init();
        void Destroy();
        void Update(const sf::Time& elapsedTime);
        void UpdateDepthPosition();
        void PollEvent(sf::Event& event);
        void Resize();
        void UpdatePosition();
        void ResizeDialogue();
        void Draw(sf::RenderWindow* window);
    };
    typedef RoomLibrary rl;
}

#endif /* RoomLibrary_hpp */
