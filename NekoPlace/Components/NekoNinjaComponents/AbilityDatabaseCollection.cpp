//
//  AbilityDatabaseCollection.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 11/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "AbilityDatabaseCollection.hpp"

namespace NekoNinja
{
    namespace adc
    {
        void TheWorld::OnAction()
        {
            shape.setRadius(0);
            shape.setFillColor(sf::Color(0,50,255, 70));
            shape.setPointCount(16);
            
            elapsedSeconds = 2.f;
            lastTimeMultiplier = nns::timeMultiplier;
            lastVelocityMultiplier = nns::velocityMultiplier;
            lastGetHarderTimeMultiplier = nns::getHarderTimeMultiplier;
            nns::timeMultiplier = 0.f; nns::velocityMultiplier = 0.f; nns::getHarderTimeMultiplier = 0.f;
            isActive = true;
        }
        void TheWorld::OnEnd()
        {
            nns::timeMultiplier = lastTimeMultiplier;
            nns::velocityMultiplier = lastVelocityMultiplier;
            nns::getHarderTimeMultiplier = lastGetHarderTimeMultiplier;
            elapsedSeconds = 0.f; isActive = false;
        }
        void TheWorld::OnUpdate(const sf::Time& elapsedTime)
        {
            radius = ((gs::width > gs::height) ? gs::width : gs::height)/2 * (1 - elapsedSeconds/2.f);
            shape.setRadius(radius); shape.setOrigin(shape.getGlobalBounds().width/2, shape.getGlobalBounds().height/2);
            shape.setPosition(gs::width/2, gs::height/2);
            
            if (elapsedSeconds <= 0) { nns::timeMultiplier = lastTimeMultiplier;
                nns::velocityMultiplier = lastVelocityMultiplier;
                nns::getHarderTimeMultiplier = lastGetHarderTimeMultiplier;
                elapsedSeconds = 0.f; isActive = false; }
            else elapsedSeconds -= elapsedTime.asSeconds();
        }
        void TheWorld::OnDraw(sf::RenderWindow* window) { window->draw(shape); }
        
        
        
        void ScratchClaw::OnAction() { sender->SendMessage({"adc::ScratchClaw"}); }
        
        
        
        void AkakosBlessings::OnAction() { nns::scoreMultiplier *= 1.03f; }
        void AkakosBlessings::OnEnd() { nns::scoreMultiplier = 1.f; }
        
        void MusicForAHeart::OnAction() { ++nns::maxLifes; }
    }
}
