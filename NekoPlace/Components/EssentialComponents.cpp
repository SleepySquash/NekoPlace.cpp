//
//  EssentialComponents.cpp
//  NovelSome
//
//  Created by Никита Исаенко on 08/09/2018.
//  Copyright © 2018 Melancholy Hill. All rights reserved.
//

#include "EssentialComponents.hpp"

namespace ns
{
    namespace EssentialComponents
    {
        DebugComponent::DebugComponent(sf::String build)
        {
            buildText.setString(build);
        }
        void DebugComponent::Init()
        {
            buildText.setFillColor(sf::Color::White);
            buildText.setOutlineColor(sf::Color::Black);
            buildText.setOutlineThickness(1.f);
            buildText.setCharacterSize(15);
            buildText.setFont(*fc::GetFont(L"Pacifica.ttf"));
            fontLoaded = (buildText.getFont() != nullptr);
            
            projectText.setString("minEH by SlSq");
            projectText.setFillColor(sf::Color::White);
            projectText.setOutlineColor(sf::Color::Black);
            projectText.setOutlineThickness(1.f);
            projectText.setCharacterSize(23);
            projectText.setFont(*fc::GetFont(L"Pacifica.ttf"));
            
            performanceText.setString("Calculating...");
            performanceText.setFillColor(sf::Color::White);
            performanceText.setOutlineColor(sf::Color::Black);
            performanceText.setOutlineThickness(1.f);
            performanceText.setCharacterSize(15);
            performanceText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        }
        void DebugComponent::Update(const sf::Time& elapsedTime)
        {
            deltaTime += elapsedTime;
            frames++;
            
            if (deltaTime.asSeconds() > 0.3 && frames > 0)
            {
                frames = (float)frames/deltaTime.asSeconds();
                std::wstring perfString = L"FPS is ";
                perfString += std::to_wstring(frames);
                
                performanceText.setString(perfString);
                performanceText.setPosition(gs::width - performanceText.getLocalBounds().width - 10*gs::scale, performanceText.getPosition().y);
                
                deltaTime = sf::Time::Zero;
                frames = 0;
            }
        }
        void DebugComponent::Draw(sf::RenderWindow* window)
        {
            if (fontLoaded)
            {
                window->draw(buildText);
                window->draw(projectText);
                window->draw(performanceText);
            }
        }
        void DebugComponent::Resize(unsigned int width, unsigned int height)
        {
            buildText.setCharacterSize((unsigned int)(15 * gs::scale));
            projectText.setCharacterSize((unsigned int)(23 * gs::scale));
            performanceText.setCharacterSize((unsigned int)(15 * gs::scale));
            
            int yy = height - 10*gs::scale;
            buildText.setPosition(width - buildText.getLocalBounds().width - 10*gs::scale, yy - buildText.getLocalBounds().height);
            yy -= buildText.getLocalBounds().height*2 - 10*gs::scale;
            projectText.setPosition(width - projectText.getLocalBounds().width - 10*gs::scale, yy - projectText.getLocalBounds().height);
            yy -= projectText.getLocalBounds().height*2 - 20*gs::scale;
            performanceText.setPosition(width - performanceText.getLocalBounds().width - 10*gs::scale, yy - performanceText.getLocalBounds().height);
        }
    }
}
