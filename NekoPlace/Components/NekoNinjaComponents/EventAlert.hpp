//
//  EventAlert.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 11/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef EventAlert_hpp
#define EventAlert_hpp

#include <iostream>
#include <fstream>
#include <vector>
#include <list>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "../../Essentials/ResourcePath.hpp"
#include "../../Engine/NovelSystem.hpp"
#include "../../Engine/Settings.hpp"
#include "../../Engine/Collectors/Font.hpp"
#include "EventBase.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::list;

using namespace ns;

namespace NekoNinja
{
    struct EventAlert : NovelObject
    {
        sf::Text text;
        int alpha{ 255 };
        float elapsed{ 0.f }, waiting{ 0.8f };
        bool fontLoaded{ false }, sent{ false };
        
        std::wstring alert;
        MessageSender* sender;
        EventBase* event;
        
        EventAlert(MessageSender* sender, const std::wstring& alert, float waiting);
        EventAlert(MessageSender* sender, EventBase* event);
        void Init() override;
        void Update(const sf::Time& elapsedTime) override;
        void Resize(unsigned int width, unsigned int height) override;
        void Draw(sf::RenderWindow* window) override;
    };
}

#endif /* EventAlert_hpp */
