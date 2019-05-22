//
//  AbilityBase.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 28/02/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef AbilityBase_hpp
#define AbilityBase_hpp

#include <iostream>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "../../Engine/MessageHolder.hpp"

using std::cin;
using std::cout;
using std::endl;

namespace NekoNinja
{
    struct AbilityBase
    {
        std::wstring name, display{ L"Абилка" }, description;
        bool passive, isActive{ false };
        unsigned int level{ 1 };
        float cooldownTime{ 0.f };
        unsigned int nekoCounter{ 40 };
        ns::MessageSender* sender{ nullptr };
        
        AbilityBase(const std::wstring& name = L"Ability", const std::wstring& description =  L"", const bool& passive = true);
        virtual void OnAction();
        virtual void OnEnd();
        virtual void OnUpdate(const sf::Time& elapsedTime);
        virtual void OnDraw(sf::RenderWindow* window);
    };
}

#endif /* AbilityBase_hpp */
