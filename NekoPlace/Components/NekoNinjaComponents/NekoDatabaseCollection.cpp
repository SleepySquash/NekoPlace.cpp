//
//  NekoDatabaseCollection.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 27/02/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "NekoDatabaseCollection.hpp"

namespace NekoNinja
{
    void NekoBase::NovelTalkTo(Entity* entity)
    {
        auto* novel = entity->AddComponent<VNLightComponents::Novel>();
        novel->lines.push_back(L"blackend");
        novel->lines.push_back(L"show " + name + L" center message:no");
        novel->lines.push_back(name + L" \"Йей, приветь!!~ :3\"");
        novel->lines.push_back(L"disappear fade:0.2");
    }
    
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
        
        
        
        void AkakosBlessings::OnAction() { nns::scoreMultiplier = 1.03f; }
    }
    
    
    
    
    
    
    
    
    
    void NekoDatabaseCollection_LoadNeko()
    {
        nl::neko.push_back(new ndc::Azuki());
        nl::neko.push_back(new ndc::Shigure());
        nl::neko.push_back(new ndc::Poop());
        nl::neko.push_back(new ndc::Cinnamon());
        nl::neko.push_back(new ndc::Maple());
        nl::neko.push_back(new ndc::Coconut());
        nl::neko.push_back(new ndc::Vanilla());
        nl::neko.push_back(new ndc::Chocola());
        
        nl::neko.push_back(new ndc::Miho());
        nl::neko.push_back(new ndc::Kay());
        nl::neko.push_back(new ndc::Kanade());
        nl::neko.push_back(new ndc::Sims());
        nl::neko.push_back(new ndc::Katyusha());
        nl::neko.push_back(new ndc::Riria());
        nl::neko.push_back(new ndc::Asuna());
        nl::neko.push_back(new ndc::Cthulhu());
        nl::neko.push_back(new ndc::MiyuEdelfelt());
        nl::neko.push_back(new ndc::Anchovy());
        nl::neko.push_back(new ndc::Darjeeling());
        nl::neko.push_back(new ndc::Altera());
        nl::neko.push_back(new ndc::Flandre());
        nl::neko.push_back(new ndc::ElizabethBathory());
        
        std::sort(nl::neko.begin(), nl::neko.end(), [](const NekoBase* a, const NekoBase* b) { return a->chance > b->chance; });
    }
    
    
    namespace ndc
    {
        void Cinnamon::NovelTalkTo(Entity* entity)
        {
            auto* novel = entity->AddComponent<VNLightComponents::Novel>();
            novel->lines.push_back(L"blackend");
            novel->lines.push_back(L"show Cinnamon cleft message:no");
            novel->lines.push_back(name + L" \"Оппа, а я другая новелла!!!!!!~ :3\"");
            novel->lines.push_back(L"disappear fade:0.2");
        }
    }
}
