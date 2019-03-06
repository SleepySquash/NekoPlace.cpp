//
//  NekoBase.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 27/02/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef NekoBase_hpp
#define NekoBase_hpp

#include <iostream>
#include <fstream>

#include <SFML/Main.hpp>
#include "../../Engine/EntitySystem.hpp"
#include "AbilityBase.hpp"

using std::cin;
using std::cout;
using std::endl;

using namespace ns;

namespace NekoNinja
{
    struct NekoBase
    {
        std::wstring name, display{ L"" };
        float chance, affection{ 0 };
        int difficultyAvailable;
        unsigned int levelNeeded;
        bool tamed;
        
        AbilityBase* ability{ nullptr };
        sf::Color color;
        sf::IntRect offsets;
        
        float xySpd;
        int personSprite_offsetX{ 0 }, personSprite_offsetY{ 0 };
        
        NekoBase();
        ~NekoBase();
        NekoBase(const std::wstring& name, const float& chance, int difficultyAvailable = 0, float xySpd = 140, unsigned int levelNeeded = 0, int l = 15, int t = 15, int w = 15, int h = 15, bool tamed = false, const sf::Color& ocolor = sf::Color::Transparent);
        virtual void Save(const std::wofstream& wof);
        virtual void Load(nss::CommandSettings& command);
        virtual void NovelTalkTo(Entity* entity);
        virtual std::wstring RandomRoomDialogue();
    };
}

#endif /* NekoBase_hpp */
