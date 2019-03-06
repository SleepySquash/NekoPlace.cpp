//
//  NekoDatabaseCollection.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 27/02/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef NekoDatabaseCollection_hpp
#define NekoDatabaseCollection_hpp

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <list>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "../../Essentials/ResourcePath.hpp"
#include "../../Engine/StaticMethods.hpp"

#include "../VNLightComponents/Novel.hpp"
#include "NekoBase.hpp"
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
            TheWorld() : AbilityBase(L"The Warudo", L"Позволяет останавливать время на 2 секунды каждые 3 секунды.", false) { display = L"Зе Варудо"; }
            
            sf::CircleShape shape;
            float radius{ 0.f };
            
            float elapsedSeconds{ 0.f };
            float lastTimeMultiplier{ 1.f }, lastGetHarderTimeMultiplier{ 1.f }, lastVelocityMultiplier{ 1.f };
            void OnAction() override;
            void OnEnd() override;
            void OnUpdate(const sf::Time& elapsedTime) override;
            void OnDraw(sf::RenderWindow* window) override;
        };
        
        struct AkakosBlessings : AbilityBase
        {
            AkakosBlessings() : AbilityBase(L"Akako's Blessing", L"Увеличивает получемое количество очков за ловлю кошечки на 3%.", true) { display = L"Благословение Акако"; }
            void OnAction() override;
        };
    }
    
    
    
    
    
    
    
    
    
    
    void NekoDatabaseCollection_LoadNeko();
    namespace ndc /// NekoDatabaseCollection
    {
        struct Shigure : NekoBase
        {
            Shigure() : NekoBase(L"Shigure", 1.f, 0, 135, 0,     155, 30, 120, 70, true) { display = L"Шигуре"; ability = new adc::AkakosBlessings(); }
        };
        struct Azuki : NekoBase
        {
            Azuki() : NekoBase(L"Azuki", 1.f, 0, 150, 3,     115, 75, 98, 80) { display = L"Азуки"; }
            std::wstring RandomRoomDialogue() override
            {
                int printRandom = rand() % 5;
                switch (printRandom)
                {
                    case 0: return L"Рррррр!!! >:3"; break;
                    case 1: return L"Возьми меня на прогулку, хозяин! >3<"; break;
                    case 2: return L"Поиграй со мной, поиграааай!! :з"; break;
                    case 3: return L"Хоба-хоба, хочу кусь oWo"; break;
                    case 4: return L"Ора-ора-ора-ора-орааа!!! >3<"; break;
                    default: return L""; break;
                }
            }
        };
        struct Poop : NekoBase
        {
            Poop() : NekoBase(L"Poop", 0.68f, 0, 140, 0,   20, 20, 20, 20) { display = L"Какашка"; }
        };
        struct Cinnamon : NekoBase
        {
            Cinnamon() : NekoBase(L"Cinnamon", 0.5f, 0, 120, 8,     144, 50, 144, 70) { display = L"Синнамон"; }
            void NovelTalkTo(Entity* entity) override;
        };
        struct Maple : NekoBase
        {
            Maple() : NekoBase(L"Maple", 0.4f, 0, 140, 10,    115, 90, 138, 100) { display = L"Мапл";
                ability = new adc::TheWorld(); }
            std::wstring RandomRoomDialogue() override
            {
                int printRandom = rand() % 6;
                switch (printRandom)
                {
                    case 0: return L"Кажется, весна наступает?~"; break;
                    case 1: return L"Хорошая погода, не так ли? с:"; break;
                    case 2: return L"Могу ли я пойти на прогулку с тобой, хозяин? <3"; break;
                    case 3: return L"*хочет обнимашек, но скрывает этого* >3<"; break;
                    case 4: return L"Я больше люблю осень~ Она красива, как и ты <3"; break;
                    case 5: return L"Листик-листик, ня~ :з"; break;
                    default: return L""; break;
                }
            }
        };
        struct Coconut : NekoBase
        {
            Coconut() : NekoBase(L"Coconut", 0.3f, 0, 145, 12,    195, 75, 97, 80) { display = L"Кокнаут"; }
            std::wstring RandomRoomDialogue() override
            {
                int printRandom = rand() % 8;
                switch (printRandom)
                {
                    case 0: return L"Мм, а ты сегодня... возбуждён, я смотрю? >:3"; break;
                    case 1: return L"Здравствуй, хозяин. Поговорим? :з"; break;
                    case 2: return L"Побыть бы рядом с хозяином... Ах, сводит с ума~ <3"; break;
                    case 3: return L"Могу я... поспать с тобой, хозяин? >:з"; break;
                    case 4: return L"Я бы лично устроила тебе приватный показ...~"; break;
                    case 5: return L"Не хочешь прогуляться?~"; break;
                    case 6: return L"Тебе нравится моя кожа? <3"; break;
                    case 7: return L"Я одеваюсь лишь для твоих глаз~ <3"; break;
                    default: return L""; break;
                }
            }
        };
        struct Vanilla : NekoBase
        {
            Vanilla() : NekoBase(L"Vanilla", 0.1f, 1, 130, 15,    185, 75, 167, 70) { display = L"Ванилла"; }
            std::wstring RandomRoomDialogue() override
            {
                int printRandom = rand() % 5;
                switch (printRandom)
                {
                    case 0: return L"Стесняюсь >///<"; break;
                    case 1: return L"А... мм, всё хорошо? с:"; break;
                    case 2: return L"Чокола, где ты?? >3<"; break;
                    case 3: return L"А хозяин милый с:"; break;
                    case 4: return L"... *смущаеться*"; break;
                    default: return L""; break;
                }
            }
        };
        struct Chocola : NekoBase
        {
            Chocola() : NekoBase(L"Chocola", 0.1f, 1, 140, 15, 168,    80, 160, 70) { display = L"Чокола"; personSprite_offsetY = 40; }
            std::wstring RandomRoomDialogue() override
            {
                int printRandom = rand() % 5;
                switch (printRandom)
                {
                    case 0: return L"Ванилла? Иди сюда! :3"; break;
                    case 1: return L"О-хо-хо, какую я шалость придумала >3<"; break;
                    case 2: return L"Уаа, как-то скучно~ >.<"; break;
                    case 3: return L"Хочу к хозяину!! В обнимашки! :з"; break;
                    case 4: return L"Помурлыкать бы на ушко тебе, ня~ с:"; break;
                    default: return L""; break;
                }
            }
        };
        struct Miho : NekoBase
        {
            Miho() : NekoBase(L"Miho", 0.07f, 0, 130, 5,     80, 92, 125, 130) { display = L"Михо"; }
        };
        struct Kay : NekoBase
        {
            Kay() : NekoBase(L"Kay", 0.07f, 0, 140, 5,     80, 92, 125, 130) { display = L"Кей"; }
        };
        struct Kanade : NekoBase
        {
            Kanade() : NekoBase(L"Kanade", 0.06f, 0, 110, 5,     74, 86, 108, 70) { display = L"Канаде"; }
        };
        struct Sims : NekoBase
        {
            Sims() : NekoBase(L"Sims", 0.02f,   0, 130, 0,     74, 86, 108, 70) { display = L"Симс"; }
        };
        struct Katyusha : NekoBase
        {
            Katyusha() : NekoBase(L"Katyusha", 0.018f, 0, 140, 0,     74, 86, 108, 70) { display = L"Катюша"; }
        };
        struct Riria : NekoBase
        {
            Riria() : NekoBase(L"Riria", 0.007f, 0, 140, 0,     74, 86, 108, 70) { display = L"Ририя"; }
        };
        struct Asuna : NekoBase
        {
            Asuna() : NekoBase(L"Asuna", 0.006f, 1, 140, 0,     194, 120, 220, 135) { display = L"Асуна"; }
        };
        struct Cthulhu : NekoBase
        {
            Cthulhu() : NekoBase(L"Cthulhu", 0.006f, 1, 140, 0,     194, 120, 220, 135) { display = L"Ктулуху"; }
        };
        struct MiyuEdelfelt : NekoBase
        {
            MiyuEdelfelt() : NekoBase(L"Miyu Edelfelt", 0.005f, 1, 140, 0,     194, 120, 220, 135) { display = L"Мию Эделфелт"; }
        };
        struct Anchovy : NekoBase
        {
            Anchovy() : NekoBase(L"Anchovy", 0.004f, 2, 140, 0,     112, 113, 50, 100) { display = L"Анчови"; }
        };
        struct Darjeeling : NekoBase
        {
            Darjeeling() : NekoBase(L"Darjeeling", 0.004f, 2, 140, 0,     112, 113, 50, 100) { display = L"Даржилинг"; }
        };
        struct Altera : NekoBase
        {
            Altera() : NekoBase(L"Altera", 0.004f, 2, 140, 0,     112, 113, 50, 100) { display = L"Алтера"; }
        };
        struct Flandre : NekoBase
        {
            Flandre() : NekoBase(L"Flandre", 0.003f, 2, 140, 0,     112, 113, 50, 100) { display = L"Фландре"; }
        };
        struct ElizabethBathory : NekoBase
        {
            ElizabethBathory() : NekoBase(L"Elizabeth Bathory", 0.002f, 2, 140, 0,     112, 113, 50, 1000) { display = L"Элизабет Бафори"; }
        };
    }
}

#endif /* NekoDatabaseCollection_hpp */
