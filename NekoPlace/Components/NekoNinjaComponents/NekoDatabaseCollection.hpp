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
#include "../../Engine/Settings.hpp"

#include "../VNLightComponents/Novel.hpp"
#include "NekoBase.hpp"
#include "NekoNinjaSettings.hpp"
#include "AbilityDatabaseCollection.hpp"

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
    void NekoDatabaseCollection_LoadNeko();
    void NekoDatabaseCollection_SortNeko();
    namespace ndc /// NekoDatabaseCollection
    {
        struct MezumiNakayano : NekoBase
        {
            MezumiNakayano() : NekoBase(L"MezumiNakayano", 1.f, 0, 135, 0,  {300, 60, 260, 140},  0.48f, 1.f, 1.f, true) { display = L"Мезуми Накаяно"; }
        };
        struct AmariMami : NekoBase
        {
            AmariMami() : NekoBase(L"AmariMami", 1.f, 0, 135, 0,  {105, 200, 120, 148},  0.59f, 1.f, 1.2f, true) { display = L"Амари Мами"; ability = new adc::ScratchClaw(); }
        };
        struct KashikoHola : NekoBase
        {
            KashikoHola() : NekoBase(L"KashikoHola", 1.f, 0, 150, 0,  {105, 200, 120, 148},  0.52f, 1.f, 1.2f, true) { display = L"Кашико Хола";  ability = new adc::MusicForAHeart(); }
        };
        
        
        struct Shigure : NekoBase
        {
            Shigure() : NekoBase(L"Shigure", 1.f, 0, 135, 0,  {300, 60, 240, 140},  0.48f, 1.f, 1.f, true) { display = L"Шигуре"; ability = new adc::AkakosBlessings(); }
        };
        struct Azuki : NekoBase
        {
            Azuki() : NekoBase(L"Azuki", 1.f, 0, 150, 3,  {230, 150, 196, 160}) { display = L"Азуки"; ability = new adc::ScratchClaw(); }
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
            void NovelTalkTo(Entity* entity) override;
        };
        struct Poop : NekoBase
        {
            static NekoBase* ptr;
            Poop() : NekoBase(L"Poop", 0.f, 0, 140, 0,  {73, 258, 73, 73},  0.24f) { display = L"Какашка"; }
        };
        struct Cinnamon : NekoBase
        {
            Cinnamon() : NekoBase(L"Cinnamon", 0.5f, 0, 120, 8,  {168, 80, 168, 120}) { display = L"Синнамон"; ability = new adc::UselessPassive(); }
            void NovelTalkTo(Entity* entity) override;
        };
        struct Maple : NekoBase
        {
            Maple() : NekoBase(L"Maple", 0.4f, 0, 140, 10,  {210, 160, 256, 180}) { display = L"Мапл";
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
            Coconut() : NekoBase(L"Coconut", 0.3f, 0, 145, 12,  {370, 130, 170, 140}) { display = L"Кокнаут"; }
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
            Vanilla() : NekoBase(L"Vanilla", 0.1f, 1, 130, 15,  {350, 130, 314, 120}) { display = L"Ванилла"; }
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
            Chocola() : NekoBase(L"Chocola", 0.1f, 1, 140, 15,  {336, 160, 320, 140}) { display = L"Чокола"; personSprite_offsetY = 40; }
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
            Miho() : NekoBase(L"Miho", 0.07f, 0, 130, 5,  {160, 180, 250, 160}) { display = L"Михо"; }
        };
        struct Kay : NekoBase
        {
            Kay() : NekoBase(L"Kay", 0.07f, 0, 140, 5,  {160, 180, 250, 160}) { display = L"Кей"; }
        };
        struct Kanade : NekoBase
        {
            Kanade() : NekoBase(L"Kanade", 0.06f, 0, 110, 5,  {140, 160, 200, 140}) { display = L"Канаде"; }
        };
        struct Sims : NekoBase
        {
            Sims() : NekoBase(L"Sims", 0.02f,   0, 130, 0,  {140, 160, 200, 140}) { display = L"Симс"; }
        };
        struct Katyusha : NekoBase
        {
            Katyusha() : NekoBase(L"Katyusha", 0.018f, 0, 140, 0,  {140, 160, 200, 140}) { display = L"Катюша"; }
        };
        struct Riria : NekoBase
        {
            Riria() : NekoBase(L"Riria", 0.007f, 0, 140, 0,  {148, 180, 216, 140}) { display = L"Ририя"; }
        };
        struct Asuna : NekoBase
        {
            Asuna() : NekoBase(L"Asuna", 0.006f, 1, 140, 0,  {160, 60, 160, 75}) { display = L"Асуна"; }
        };
        struct Cthulhu : NekoBase
        {
            Cthulhu() : NekoBase(L"Cthulhu", 0.006f, 1, 140, 0,  {290, 180, 338, 224}) { display = L"Ктулуху"; }
        };
        struct MiyuEdelfelt : NekoBase
        {
            MiyuEdelfelt() : NekoBase(L"MiyuEdelfelt", 0.005f, 1, 140, 0,  {194, 70, 220, 70}) { display = L"Мию Эделфелт"; }
        };
        struct Anchovy : NekoBase
        {
            Anchovy() : NekoBase(L"Anchovy", 0.004f, 2, 140, 0,  {220, 140, 100, 140}) { display = L"Анчови"; }
        };
        struct Darjeeling : NekoBase
        {
            Darjeeling() : NekoBase(L"Darjeeling", 0.004f, 2, 140, 0,  {220, 140, 100, 140}) { display = L"Даржилинг"; }
        };
        struct Altera : NekoBase
        {
            Altera() : NekoBase(L"Altera", 0.004f, 2, 140, 0,  {220, 140, 100, 140}) { display = L"Алтера"; }
        };
        struct Flandre : NekoBase
        {
            Flandre() : NekoBase(L"Flandre", 0.003f, 2, 140, 0,  {220, 140, 100, 140}) { display = L"Фландре"; }
        };
        struct ElizabethBathory : NekoBase
        {
            ElizabethBathory() : NekoBase(L"ElizabethBathory", 0.002f, 2, 140, 0,  {220, 140, 100, 140}) { display = L"Элизабет Бафори"; }
        };
    }
}

#endif /* NekoDatabaseCollection_hpp */
