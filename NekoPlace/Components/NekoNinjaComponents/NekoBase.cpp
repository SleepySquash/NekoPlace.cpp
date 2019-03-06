//
//  NekoBase.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 27/02/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "NekoBase.hpp"

namespace NekoNinja
{
    NekoBase::~NekoBase() { delete ability; }
    NekoBase::NekoBase() { }
    NekoBase::NekoBase(const std::wstring& name, const float& chance, int difficultyAvailable, float xySpd, unsigned int levelNeeded, int l, int t, int w, int h, bool tamed, const sf::Color& ocolor) : name(name), chance(chance), difficultyAvailable(difficultyAvailable), xySpd(xySpd), levelNeeded(levelNeeded), offsets(l, t, w, h), tamed(tamed), color(ocolor)
    {
        if (ocolor == sf::Color::Transparent)
        {
            if (chance >= 0.09)          color = sf::Color(200, 200, 200, 120);
            else if (chance >= 0.05)     color = sf::Color(106, 143, 203, 255);
            else if (chance >= 0.008)    color = sf::Color(87, 173, 98, 255);
            else if (chance >= 0.005)    color = sf::Color(218, 113, 114, 255);
            else if (chance >= 0.0012)   color = sf::Color(188, 106, 167, 255);
            else if (chance >= 0.0006)   color = sf::Color(244, 200, 68, 255);
            else if (chance == 0)        color = sf::Color(0, 0, 0, 255);
            else                         color = sf::Color(255, 180, 180, 255);
            
            /*if (chance >= 0.25)         color = sf::Color(200, 200, 200, 120);
            else if (chance >= 0.08)    color = sf::Color(106, 143, 203, 255);
            else if (chance >= 0.02)    color = sf::Color(87, 173, 98, 255);
            else if (chance >= 0.008)   color = sf::Color(218, 113, 114, 255);
            else if (chance >= 0.004)   color = sf::Color(188, 106, 167, 255);
            else if (chance >= 0.001)   color = sf::Color(244, 200, 68, 255);
            else if (chance == 0)       color = sf::Color(0, 0, 0, 255);
            else                        color = sf::Color(255, 180, 180, 255);*/
        }
    }
    void NekoBase::Save(const std::wofstream& wof) { /* Saving: "wof << data << endl;" etc, but don't forget about loading it later */ }
    void NekoBase::Load(nss::CommandSettings& command) { /* Loading looks like if (nss::Command(...) { load_some_info... } else if... */ }
    std::wstring NekoBase::RandomRoomDialogue()
    {
        int printRandom = rand() % 8;
        switch (printRandom)
        {
            case 0: return L"Няяя!!! >3<"; break;
            case 1: return L"Хозяин, заметь меня! т.т"; break;
            case 2: return L"Ууу, кяк скучашкиии~ :3"; break;
            case 3: return L"Мур-ня, я в комнате, oWo"; break;
            case 4: return L"Хожу-брожу, ня~ <3"; break;
            case 5: return L"Привет, как дела? c:"; break;
            case 6: return L"Меня зовут " + display + L"!"; break;
            case 7: return L"Хотю любви~ <3"; break;
            default: return L""; break;
        }
    }
}
