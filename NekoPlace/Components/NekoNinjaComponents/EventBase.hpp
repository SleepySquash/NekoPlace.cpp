//
//  EventBase.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 07/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef EventBase_hpp
#define EventBase_hpp

#include <iostream>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

using std::cin;
using std::cout;
using std::endl;

namespace NekoNinja
{
    struct EventBase
    {
        std::string name; std::wstring description;
        float alertDuration{ 0.3f }, duration{ 20.f };
        bool requireAlert{ true };
        
        EventBase(const std::string& name = "Event", const std::wstring& description =  L"");
        virtual void OnAction();
        virtual void OnEnd();
        virtual void OnUpdate(const sf::Time& elapsedTime);
        virtual void OnDraw(sf::RenderWindow* window);
    };
}

#endif /* EventBase_hpp */
