//
//  EssentialComponents.hpp
//  NovelSome
//
//  Created by Никита Исаенко on 08/09/2018.
//  Copyright © 2018 Melancholy Hill. All rights reserved.
//

#ifndef EssentialComponents_hpp
#define EssentialComponents_hpp

#include <iostream>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "../Engine/EntitySystem.hpp"
#include "../Engine/Settings.hpp"
#include "../Engine/Collectors/Font.hpp"

using std::cin;
using std::cout;
using std::endl;

namespace ns
{
    namespace EssentialComponents
    {
        class DebugComponent : public Component
        {
        private:
            sf::Text performanceText;
            sf::Text projectText;
            sf::Text buildText;
            bool fontLoaded{ false };
            
            sf::Time deltaTime{ sf::Time::Zero };
            int frames{ 0 };
            
        public:
            DebugComponent(sf::String build);
            void Init() override;
            void Update(const sf::Time& elapsedTime) override;
            void Draw(sf::RenderWindow* window) override;
            void Resize(unsigned int width, unsigned int height) override;
        };
    }
}

#endif /* EssentialComponents_hpp */
