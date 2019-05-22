//
//  NekoEntity.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 17/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "NekoEntity.hpp"

namespace NekoUI
{
    void NekoEntity::Init()
    {
        x = rand() % Room::width;
        y = rand() % Room::height;
        
        sf::Texture* texture = ic::LoadTexture(L"Data/Images/dialogue1_n.png");
        if (texture)
        {
            dialogueSprite.setTexture(*texture);
            dialogueSprite.setOrigin(texture->getSize().x/2, texture->getSize().y);
            dialogue.setFont(*fc::GetFont(L"Pacifica.ttf"));
            dialogue.setFillColor(sf::Color::White);
        }
        
        texture = ic::LoadTexture(L"Data/Neko/Chibi/Vanilla.png");
        if ((spriteLoaded = texture))
        {
            sprite.setTexture(*texture);
            sprite.setOrigin(texture->getSize().x/2, texture->getSize().y - texture->getSize().y/15);
            relScale = (static_cast<float>(gs::relativeHeight)/static_cast<float>(texture->getSize().y)) * 0.41;
        }
        
        int i{ 0 }; do {
            x = rand() % Room::width;
            y = rand() % Room::height; ++i;
        } while (Room::mask && Room::mask->getPixel(floor(x/Room::roomScale), floor(y/Room::roomScale)).r && i < 100);
        if (i >= 100) { x = Room::width/2; y = Room::height/2; }
        sprite.setPosition(x * Room::scale * gs::scale, y * Room::scale * gs::scale);
        newPoint = true; elapsedWaiting = (rand() % 1000)/1000.f;
    }
    void NekoEntity::Destroy() { ic::DeleteImage(L"Data/Neko/Chibi/Vanilla.png"); ic::DeleteImage(L"Data/Images/dialogue1_n.png"); }
    void NekoEntity::Update(const sf::Time& elapsedTime)
    {
        if (beingActionedWith) return;
        if (elapsedDialogue > 0) elapsedDialogue -= elapsedTime.asSeconds();
        else
        {
            gs::requestWindowRefresh = true;
            if (drawDialogue) { drawDialogue = false; elapsedDialogue = (rand() % 12000) / 1000.f; }
            else
            {
                drawDialogue = true;
                dialogue.setString(GenerateRoomDialogue());
                elapsedDialogue = 2.1f + 0.08 * dialogue.getString().getSize();// + (rand() % 2000) / 2000.f;
                ResizeDialogue();
                dialogue.setOrigin(dialogue.getLocalBounds().width/2, dialogue.getLocalBounds().height + 44*gs::scale);
            }
        }
        
        if (elapsedWaiting > 0) elapsedWaiting -= elapsedTime.asSeconds();
        else
        {
            if (Room::mask)
            {
                float xMove{ 0 }, yMove{ 0 };
                if (moveLeft || moveRight) xMove = xySpd * elapsedTime.asSeconds();
                if (moveUp || moveDown) yMove = xySpd * elapsedTime.asSeconds();
                
                if (moveLeft) {
                    if (!Room::mask->getPixel(floor((x - xMove)/Room::roomScale), floor(y/Room::roomScale)).r)
                        x -= xMove; else moveLeft = false; }
                if (moveRight) {
                    if (!Room::mask->getPixel(floor((x + xMove)/Room::roomScale), floor(y/Room::roomScale)).r)
                        x += xMove; else moveRight = false; }
                if (moveUp) {
                    if (!Room::mask->getPixel(floor(x/Room::roomScale), floor((y - yMove)/Room::roomScale)).r)
                        y -= yMove; else moveUp = false; }
                if (moveDown) {
                    if (!Room::mask->getPixel(floor(x/Room::roomScale), floor((y + yMove)/Room::roomScale)).r)
                        y += yMove; else moveDown = false; }
            }
            else
            {
                if (moveLeft) x -= xySpd * elapsedTime.asSeconds();
                if (moveRight) x += xySpd * elapsedTime.asSeconds();
                if (moveUp) y -= xySpd * elapsedTime.asSeconds();
                if (moveDown) y += xySpd * elapsedTime.asSeconds();
            }
            if (moveLeft || moveRight || moveUp || moveDown)
            {
                UpdateDepthPosition(); gs::requestWindowRefresh = true;
                sprite.setPosition((Room::x + x) * Room::scale * gs::scale, (Room::y + y) * Room::scale * gs::scale);
                if (drawDialogue) { dialogueSprite.setPosition(sprite.getPosition().x, sprite.getGlobalBounds().top);
                    dialogue.setPosition(dialogueSprite.getPosition()); }
            }
            
            
            
            if (newPoint)
            {
                if (Room::mask && Room::mask->getPixel(floor(x/Room::roomScale), floor(y/Room::roomScale)).r && Room::mask->getPixel(floor(x/Room::roomScale) - 1, floor(y/Room::roomScale)).r && Room::mask->getPixel(floor(x/Room::roomScale) + 1, floor(y/Room::roomScale)).r && Room::mask->getPixel(floor(x/Room::roomScale), floor(y/Room::roomScale) - 1).r && Room::mask->getPixel(floor(x/Room::roomScale), floor(y/Room::roomScale) + 1).r) { x = Room::width/2; y = Room::height/2; }
                /*if (occupiedAt != -1)
                {
                    Room::nekoActivity[occupiedAt].occupied = false;
                    occupiedAt = -1;
                }
                int occupyRandom = rand() % 20;
                if (!occupyRandom)
                {
                    occupiedAt = -1; int i{ 0 };
                    do { occupiedAt = rand() % Room::nekoActivity.size(); }
                    while (Room::nekoActivity[occupiedAt].occupied && i < 5);
                    if (i == 5) { occupiedAt = -1;
                        for (i = 0; i < Room::nekoActivity.size() && occupiedAt == -1; ++i)
                            if (!Room::nekoActivity[i].occupied) occupiedAt = i;
                    }
                    if (occupiedAt != -1)
                    {
                        Room::nekoActivity[occupiedAt].occupied = true;
                        movingTo_x = Room::nekoActivity[occupiedAt].x * Room::roomScale;
                        movingTo_y = Room::nekoActivity[occupiedAt].y * Room::roomScale;
                        cout << base::utf8(info->name) << ": I'm occupied at: " << movingTo_x << " " << movingTo_y << endl;
                    }
                }
                else
                {*/
                    int i{ 0 }; do {
                        movingTo_x = rand() % Room::width;
                        movingTo_y = rand() % Room::height; ++i;
                    } while (Room::mask && Room::mask->getPixel(floor(movingTo_x/Room::roomScale), floor(movingTo_y/Room::roomScale)).r && i < 100);
                //}
                newPoint = false;
            }
            else
            {
                if (!newPoint && !moveUp && !moveDown && !moveLeft && !moveRight) {
                    newPoint = true; if (occupiedAt == -1) elapsedWaiting = waitingTime + (rand() % 1000)/1000.f;
                    else { elapsedWaiting = waitingTime*4 + (rand() % 5000)/5000.f; } }
                
                // TODO: Pathfinding algorithm!
                if (x < movingTo_x - 30) { moveRight = true; moveLeft = false; }
                else if (x > movingTo_x + 30) { moveLeft = true; moveRight = false; }
                else { moveLeft = false; moveRight = false; }
                
                if (y < movingTo_y - 30) { moveDown = true; moveUp = false; }
                else if (y > movingTo_y + 30) { moveUp = true; moveDown = false; }
                else { moveDown = false; moveUp = false; }
            }
        }
    }
    void NekoEntity::UpdateDepthPosition()
    {
        /*if (positionInArray > 0)
        {
            NekoEntity* prev = rl::neko[positionInArray - 1];
            if (prev->y > y) { rl::neko[positionInArray - 1] = this; rl::neko[positionInArray] = prev;
                --positionInArray; ++prev->positionInArray; }
        }
        if (positionInArray + 1 < rl::neko.size())
        {
            NekoEntity* next = rl::neko[positionInArray + 1];
            if (next->y < y) { rl::neko[positionInArray + 1] = this; rl::neko[positionInArray] = next;
                ++positionInArray; --next->positionInArray; }
        }*/
    }
    void NekoEntity::PollEvent(sf::Event& event) { }
    void NekoEntity::Resize()
    {
        sprite.setScale(0.7 * relScale * Room::scale * gs::scale, 0.7 * relScale * Room::scale * gs::scale);
        sprite.setPosition((Room::x + x) * Room::scale * gs::scale, (Room::y + y) * Room::scale * gs::scale);
        
        if (drawDialogue) {ResizeDialogue();
            dialogue.setOrigin(dialogue.getLocalBounds().width/2, dialogue.getLocalBounds().height + 44*gs::scale); }
    }
    void NekoEntity::UpdatePosition()
    {
        sprite.setPosition((Room::x + x) * Room::scale * gs::scale, (Room::y + y) * Room::scale * gs::scale);
        if (drawDialogue)
        {
            dialogueSprite.setPosition(sprite.getPosition().x, sprite.getGlobalBounds().top);
            dialogue.setPosition(dialogueSprite.getPosition());
        }
    }
    void NekoEntity::ResizeDialogue()
    {
        dialogue.setCharacterSize(24 * gs::scale);
        dialogue.setScale(Room::scale, Room::scale);
        
        dialogueSprite.setScale(1.1*dialogue.getGlobalBounds().width/dialogueSprite.getLocalBounds().width, 1.5 * Room::scale * gs::scale);
        dialogueSprite.setPosition(sprite.getPosition().x, sprite.getGlobalBounds().top);
        dialogue.setPosition(dialogueSprite.getPosition());
    }
    void NekoEntity::Draw(sf::RenderWindow* window)
    {
        if (spriteLoaded) window->draw(sprite);
        if (drawDialogue) { window->draw(dialogueSprite); window->draw(dialogue); }
    }
    std::wstring NekoEntity::GenerateRoomDialogue() { return L"Хозяин! >3<"; }
}
