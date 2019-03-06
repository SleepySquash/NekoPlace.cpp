//
//  AbilityBase.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 28/02/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "AbilityBase.hpp"

namespace NekoNinja
{
    AbilityBase::AbilityBase(const std::wstring& name, const std::wstring& description, const bool& passive) : name(name), description(description), passive(passive) { }
    void AbilityBase::OnAction() { }
    void AbilityBase::OnEnd() { }
    void AbilityBase::OnUpdate(const sf::Time& elapsedTime) { }
    void AbilityBase::OnDraw(sf::RenderWindow* window) { }
}
