//
//  NekoMenu.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 29/01/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "NekoMenu.hpp"

namespace NekoNinja
{
    void MainMenu::Init()
    {
        sf::Texture* texture = ic::LoadTexture(L"Data/Images/MainMenu1.jpg");
        if (texture)
        {
            background.setTexture(*texture);
            background.setOrigin(texture->getSize().x/2, texture->getSize().y/2);
        }
        spriteLoaded = (texture != nullptr);
        
        texture = ic::LoadTexture(L"Data/Neko/Vanilla Full.png");
        if (texture)
        {
            neko.setTexture(*texture);
            neko.setOrigin(texture->getSize().x/2, texture->getSize().y);
        }
        spriteLoaded = (texture != nullptr);
        
        playButton.setFont(L"Pacifica.ttf");
        playButton.setCharacterSize(140);
        playButton.setString(L"Прогулка");
        playButton.valign = GUIButton::valignEnum::bottom;
        
        /*ic::PreloadTexture(L"Data/Images/park.jpg", 0, false);
        ic::PreloadTexture(L"Data/Images/heartsshape.png", 0, false);
        ic::PreloadTexture(L"Data/Images/heart.png", 0, false);
        ic::PreloadTexture(L"Data/Images/groping-hand.png", 0, false);
        ic::PreloadTexture(L"Data/Images/pause_button.png", 0, false);
        for (int i = 0; i < nl::neko.size(); ++i)
        {
            ic::PreloadTexture(L"Data/Neko/" + nl::neko[i].name + L".png", 0, false);
            sc::PreloadSound(L"Data/Neko/" + nl::neko[i].name + L".ogg", 0, false);
        }*/
    }
    void MainMenu::Destroy()
    {
        ic::DeleteImage(L"Data/Images/MainMenu1.jpg");
        for (int i = 0; i < nl::neko.size(); ++i)
        {
            ic::SetDestroyable(L"Data/Neko/" + nl::neko[i].name + L".png", true);
            sc::SetDestroyable(L"Data/Sounds/" + nl::neko[i].name + L".ogg", true);
        }
        ic::SetDestroyable(L"Data/Images/heartsshape.png", true);
        ic::SetDestroyable(L"Data/Images/heart.png", true);
        ic::SetDestroyable(L"Data/Images/park.jpg", true);
        ic::SetDestroyable(L"Data/Images/groping-hand.png", true);
        ic::SetDestroyable(L"Data/Images/pause_button.png", true);
    }
    void MainMenu::PollEvent(sf::Event& event)
    {
        if (gs::inGame) return;
        if (playButton.PollEvent(event)) { gs::isPause = false; entity->AddComponent<NekoNinja::Controller>(true); }
    }
    void MainMenu::Resize(unsigned int width, unsigned int height)
    {
        if (gs::inGame) return;
        playButton.Resize(width, height);
        playButton.setPosition(width/2, height - height/10);
        
        if (spriteLoaded)
        {
            float factorX = (float)width / background.getTexture()->getSize().x;
            float factorY = (float)height / background.getTexture()->getSize().y;
            float scaleFactor = factorX > factorY ? factorX : factorY;
            background.setScale(scaleFactor, scaleFactor);
            background.setPosition(width/2, height/2);
            
            neko.setScale(1.5*gs::scScale, 1.5*gs::scScale);
            if (neko.getGlobalBounds().height + 20*gs::scale > gs::height)
                neko.setPosition(width/2, neko.getGlobalBounds().height + 20*gs::scale);
            else neko.setPosition(width/2, height);
        }
    }
    void MainMenu::Draw(sf::RenderWindow* window)
    {
        if (gs::inGame) return;
        if (spriteLoaded)
        {
            window->draw(background);
            window->draw(neko);
        }
        playButton.Draw(window);
    }
}
