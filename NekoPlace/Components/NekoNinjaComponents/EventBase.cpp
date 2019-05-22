//
//  EventBase.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 07/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "EventBase.hpp"

namespace NekoNinja
{
    EventBase::EventBase(const std::string& name, const std::wstring& description) : name(name), description(description) { }
    void EventBase::OnAction() { }
    void EventBase::OnEnd() { }
    void EventBase::OnUpdate(const sf::Time& elapsedTime) { }
    void EventBase::OnDraw(sf::RenderWindow* window) { }
}
