//
//  AbilityDatabaseCollection.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 11/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef AbilityDatabaseCollection_hpp
#define AbilityDatabaseCollection_hpp

#include <iostream>
#include <list>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "../../Essentials/ResourcePath.hpp"
#include "../../Engine/Settings.hpp"

#include "AbilityBase.hpp"
#include "NekoNinjaSettings.hpp"

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
    namespace adc
    {
        struct TheWorld : AbilityBase
        {
            TheWorld() : AbilityBase(L"The Warudo", L"Позволяет останавливать время на 2 секунды каждые 3 секунды.", false) { display = L"Зе Варудо"; /*cooldownTime = 3.f;*/ nekoCounter = 10; }
            
            sf::CircleShape shape;
            float radius{ 0.f };
            
            float elapsedSeconds{ 0.f };
            float lastTimeMultiplier{ 1.f }, lastGetHarderTimeMultiplier{ 1.f }, lastVelocityMultiplier{ 1.f };
            void OnAction() override;
            void OnEnd() override;
            void OnUpdate(const sf::Time& elapsedTime) override;
            void OnDraw(sf::RenderWindow* window) override;
        };
        
        struct ScratchClaw : AbilityBase
        {
            ScratchClaw() : AbilityBase(L"Scratch-claw", L"Ловит всех зверушек в видимом диапазоне.", false) { display = L"Цап-царап"; cooldownTime = 10.f; }
            
            void OnAction() override;
        };
        
        
        
        
        struct AkakosBlessings : AbilityBase
        {
            AkakosBlessings() : AbilityBase(L"Akako's Blessing", L"Увеличивает получемое количество очков за ловлю кошечки на 3%.", true) { display = L"Благословение Акако"; }
            void OnAction() override;
            void OnEnd() override;
        };
        
        struct MusicForAHeart : AbilityBase
        {
            MusicForAHeart() : AbilityBase(L"MusicForAHeart", L"Как музыка для сердца, даёт вам 1 дополнительную жизнь.", true) { display = L"Музыка для сердца"; }
            void OnAction() override;
        };
        
        struct UselessPassive : AbilityBase
        {
            UselessPassive() : AbilityBase(L"UselessPassive", L"Ничего не делает. Настолько пассивная, что пассивная.", true) { display = L"Бесполезная пассивка"; }
        };
    }
}

#endif /* AbilityDatabaseCollection_hpp */
