//
//  Inventory.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 12/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "InventoryUI.hpp"

namespace NekoNinja
{
    void InventoryUI::Init()
    {
        helperText.setFillColor(sf::Color::White);
        helperText.setOutlineColor(sf::Color::Black);
        helperText.setString(L"Уровень:");
        helperText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        backShape.setFillColor(sf::Color(0, 0, 0, 150));
        
        itemButtons.setFont(L"Pacifica.ttf");
        itemButtons.setCharacterSize(54);
        itemButtons.halign = Halign::Right;
        itemUseButton.setFont(L"Pacifica.ttf");
        itemUseButton.setCharacterSize(66);
        itemUseButton.setString(L"Использовать");
        itemUseButton.shape.setFillColor(sf::Color(0,0,0,130));
        itemDescriptionShape.setFillColor(sf::Color(0,0,0,130));
        itemDescriptionShape.setOutlineColor(sf::Color(255,255,255,180));
        itemText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        itemText.setOutlineColor(sf::Color::Black);
        
        closeButton.setFont(L"Pacifica.ttf");
        closeButton.setCharacterSize(90);
        closeButton.setString(L"Закрыть");
        closeButton.valign = Valign::Bottom;
        
        if (Inventory::list.empty())
        {
            Inventory::Add("Cupcake", 4);
            Inventory::Add("Star", 1);
            Player::self->SaveItems();
        }
    }
    void InventoryUI::PollEvent(sf::Event& event)
    {
        if (!active || !gs::isActiveInterface(this)) return;
        
        float yy; int i; bool deselect;
        if (itemIsSelected && selectedItem->usable && itemUseButton.PollEvent(event))
        {
            if (selectedItem->name == "Lootbox") entity->SendMessage({"Lootbox :: Show"});
        }
        deselect = (event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::TouchEnded);
        yy = itemScrollYY; i = 0;
        for (auto& item : Inventory::list)
        {
            itemButtons.setString(std::to_wstring(item->count));
            if (gs::verticalOrientation) {
                itemButtons.shape.setSize({4*gs::width/5.f, itemButtons.text.getGlobalBounds().height + 15*gs::scaley});
                itemButtons.setPosition(gs::width/10, yy); }
            else {
                itemButtons.shape.setSize({4.4f*gs::width/10.f, itemButtons.text.getGlobalBounds().height + 15*gs::scaley});
                itemButtons.setPosition(gs::width/20, yy); } itemButtons.index = i++;
            if (itemButtons.PollEvent(event)) { SelectItem(item); deselect = false; break; }
            yy += itemButtons.shape.getGlobalBounds().height + 10*gs::scaley;
        }
        if (closeButton.PollEvent(event)) { if (gs::verticalOrientation && itemIsSelected) deselect = true; else entity->SendMessage({"Inventory :: Close"}); }
        if (deselect) { itemIsSelected = false; ic::DeleteImage(itemTexturePath); }
        if (event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::TouchEnded) itemButtons.eventPolled(event);
    }
    void InventoryUI::Resize(unsigned int width, unsigned int height)
    {
        if (!active) return;
        
        helperText.setOutlineThickness(gs::scale);
        helperText.setCharacterSize((unsigned int)(78 * gs::scale));
        helperText.setString(L"Уровень:");
        
        closeButton.Resize(width, height);
        closeButton.setPosition(width/2, height - 60*gs::scaley);
        backShape.setSize({ (float)width, (float)height });
        
        itemButtons.Resize(width, height);
        if (gs::verticalOrientation) {
            itemDescriptionShape.setSize({8*gs::width/10.f, gs::height/5.f});
            itemDescriptionShape.setPosition(gs::width/10.f, 2.5f*gs::height/5.f); }
        else {
            itemDescriptionShape.setSize({2*gs::width/5.f, 1.5f*gs::height/5.f});
            itemDescriptionShape.setPosition(gs::width - itemDescriptionShape.getGlobalBounds().width*1.1, 2.5f*gs::height/5.f); }
        if (gs::verticalOrientation) itemUseButton.shape.setSize({itemDescriptionShape.getGlobalBounds().width, itemUseButton.text.getGlobalBounds().height*1.3f});
        else itemUseButton.shape.setSize({4*itemDescriptionShape.getGlobalBounds().width/5.f, itemUseButton.text.getGlobalBounds().height*1.3f});
        itemUseButton.Resize(width, height);
        itemUseButton.shape.setSize({itemUseButton.shape.getSize().x, itemUseButton.text.getGlobalBounds().height*1.3f});
        itemUseButton.setPosition(itemDescriptionShape.getGlobalBounds().left + (itemDescriptionShape.getGlobalBounds().width - itemUseButton.shape.getGlobalBounds().width)/2, itemDescriptionShape.getGlobalBounds().top + itemDescriptionShape.getGlobalBounds().height + 12*gs::scaley);
        itemUseButton.text.setPosition(itemUseButton.text.getPosition().x, itemUseButton.text.getPosition().y - 10*gs::scaley);
        itemText.setOutlineThickness(2*gs::scale);
        itemText.setCharacterSize(51*gs::scale);
        itemText.setString(L"Пока что здесь пусто");
        itemText.setOrigin(itemText.getLocalBounds().width/2, itemText.getLocalBounds().height/2);
        if (itemIsSelected) {
            itemDescription = nss::GetStringWithLineBreaks(itemText, selectedItem->description, itemDescriptionShape.getGlobalBounds().width - 20*gs::scale);
            float neededScale = (itemTexture->getSize().x > itemTexture->getSize().y) ? gs::height/(2.5*itemTexture->getSize().x) : gs::height/(2.5*itemTexture->getSize().y);
            itemSprite.setScale(neededScale, neededScale);
            itemSprite.setPosition(itemDescriptionShape.getGlobalBounds().left + itemDescriptionShape.getGlobalBounds().width/2, itemDescriptionShape.getGlobalBounds().top - itemSprite.getGlobalBounds().height/2 - 15*gs::scaley);
        }
    }
    void InventoryUI::Draw(sf::RenderWindow* window)
    {
        if (!active || gs::ignoreDraw) return;
        window->draw(backShape);
        if (!Inventory::list.empty())
        {
            if (!gs::verticalOrientation || !itemIsSelected)
            {
                helperText.setCharacterSize((unsigned int)(50 * gs::scale));
                float yy = itemScrollYY; int i = 0;
                for (auto& item : Inventory::list)
                {
                    itemButtons.setString(std::to_wstring(item->count));
                    if (gs::verticalOrientation) {
                        itemButtons.shape.setSize({4*gs::width/5.f, itemButtons.text.getGlobalBounds().height + 15*gs::scaley});
                        itemButtons.setPosition(gs::width/10, yy); }
                    else {
                        itemButtons.shape.setSize({4.4f*gs::width/10.f, itemButtons.text.getGlobalBounds().height + 15*gs::scaley});
                        itemButtons.setPosition(gs::width/20, yy); }
                    itemButtons.shape.setFillColor(item->color);
                    if (item->rarity == 100) itemButtons.shape.setOutlineThickness(7*gs::scale);
                    else itemButtons.shape.setOutlineThickness(2*gs::scale);
                    itemButtons.text.setPosition(itemButtons.text.getPosition().x - 5*gs::scalex, itemButtons.text.getPosition().y - 10*gs::scaley);
                    helperText.setString(item->display); itemButtons.index = i++;
                    helperText.setPosition(itemButtons.shape.getGlobalBounds().left + 10*gs::scalex, yy - 5*gs::scaley);
                    itemButtons.Draw(window); window->draw(helperText);
                    yy += itemButtons.shape.getGlobalBounds().height + 10*gs::scaley;
                }
            }
            
            if (itemIsSelected)
            {
                window->draw(itemDescriptionShape);
                itemText.setCharacterSize(70*gs::scale);
                itemText.setPosition(itemDescriptionShape.getGlobalBounds().left + itemDescriptionShape.getGlobalBounds().width/2, itemDescriptionShape.getGlobalBounds().top - 5*gs::scaley);
                itemText.setString(selectedItem->display);
                itemText.setOrigin(itemText.getGlobalBounds().width/2, 0);
                window->draw(itemText); float yy = itemText.getGlobalBounds().height;
                
                itemText.setCharacterSize(51*gs::scale);
                itemText.setString(itemDescription); itemText.setOrigin(0, 0);
                itemText.setPosition(itemDescriptionShape.getGlobalBounds().left + 10*gs::scalex, itemDescriptionShape.getGlobalBounds().top + (itemDescriptionShape.getGlobalBounds().height - yy)/2 );
                window->draw(itemText); window->draw(itemSprite);
                if (selectedItem->usable) itemUseButton.Draw(window);
            }
        }
        else
        {
            itemText.setPosition(gs::width/2, gs::height/2);
            itemText.setString(L"Пока что здесь пусто");
            window->draw(itemText);
        }
        closeButton.Draw(window);
    }
    void InventoryUI::RecieveMessage(MessageHolder& message)
    {
        if (message.info == "Inventory :: Show" && !active) { Switch(true); giftSelection = false; }
        else if (message.info == "Inventory :: Close" && active) { Switch(false); }
        else if (message.info == "Inventory :: Gift") { Switch(true); giftSelection = true; }
        else if (message.info == "Inventory :: SelectItem") { SelectItem(reinterpret_cast<ItemBase*>(message.address)); }
        else if (message.info == "Inventory :: Switch") Switch(!active);
    }
    void InventoryUI::Switch(const bool& on)
    {
        if (on && !active) { gs::PushInterface(this); active = true; entity->SortAbove(this); Resize(gs::width, gs::height); }
        else if (active) { gs::RemoveInterface(this); active = false; Resize(gs::width, gs::height); }
    }
    void InventoryUI::SelectItem(ItemBase* item)
    {
        if (!item) return;
        itemIsSelected = true; selectedItem = item;
        itemText.setCharacterSize(51*gs::scale);
        
        itemTexturePath = L"Data/Items/" + utf16(item->name) + L".png";
        itemTexture = ic::LoadTexture(itemTexturePath);
        if (itemTexture)
        {
            itemSprite.setTexture(*itemTexture, true);
            itemSprite.setOrigin(itemTexture->getSize().x/2, itemTexture->getSize().y/2);
            float neededScale = (itemTexture->getSize().x > itemTexture->getSize().y) ? gs::height/(2.5*itemTexture->getSize().x) : gs::height/(2.5*itemTexture->getSize().y);
            itemSprite.setScale(neededScale, neededScale);
            itemSprite.setPosition(itemDescriptionShape.getGlobalBounds().left + itemDescriptionShape.getGlobalBounds().width/2, itemDescriptionShape.getGlobalBounds().top - itemSprite.getGlobalBounds().height/2 - 15*gs::scaley);
        }
        
        itemDescription = nss::GetStringWithLineBreaks(itemText, item->description, itemDescriptionShape.getGlobalBounds().width - 20*gs::scale);
    }
    
    
    
    
    
    void LootboxUI::Init()
    {
        backShape.setFillColor(sf::Color(0, 0, 0, 190));
        
        closeButton.setFont(L"Pacifica.ttf");
        closeButton.setCharacterSize(90);
        closeButton.setString(L"Отмена");
        closeButton.valign = Valign::Bottom;
        
        openButton.setFont(L"Pacifica.ttf");
        openButton.setCharacterSize(105);
        openButton.setString(L"Открыть");
    }
    void LootboxUI::PollEvent(sf::Event& event)
    {
        if (!active || !gs::isActiveInterface(this)) return;
        
        if (closeButton.PollEvent(event)) entity->SendMessage({"Lootbox :: Close"});
        else if (openButton.PollEvent(event))
        {
            int randomItem = rand() % Inventory::map.size();
            int i = 0; auto it = Inventory::map.begin();
            for (; i < randomItem && it != Inventory::map.end(); ++it) ++i;
            if (it != Inventory::map.end())
            {
                if ((*it).second->name == "Lootbox") { }
                else
                {
                    int randomAmount = rand() % 3 + 1;
                    Inventory::Push((*it).second->name, randomAmount);
                    Inventory::Sort(); Player::self->SaveItems();
                }
            }
            entity->SendMessage({"Lootbox :: Close"});
        }
    }
    void LootboxUI::Resize(unsigned int width, unsigned int height)
    {
        if (!active) return;
        
        backShape.setSize({ (float)width, (float)height });
        
        lootboxSprite.setPosition(width/2, height/2);
        lootboxSprite.setScale(20*gs::scale, 20*gs::scale);
        //lootboxWindowShape.setSize({ width*5/6.f, (float)height*5/6.f });
        
        closeButton.Resize(width, height);
        closeButton.setPosition(width/2, height - 60*gs::scaley);
        openButton.Resize(width, height);
        openButton.setPosition(width/2, lootboxSprite.getGlobalBounds().top + lootboxSprite.getGlobalBounds().height/2);
    }
    void LootboxUI::Draw(sf::RenderWindow* window)
    {
        if (!active || gs::ignoreDraw) return;
        window->draw(backShape);
        window->draw(lootboxSprite);
        closeButton.Draw(window);
        openButton.Draw(window);
    }
    void LootboxUI::RecieveMessage(MessageHolder& message)
    {
        if (message.info == "Lootbox :: Show" && !active) Switch(true);
        else if (message.info == "Lootbox :: Close" && active) Switch(false);
    }
    void LootboxUI::Switch(const bool& on)
    {
        if (on && !active)
        {
            sf::Texture* texture = ic::LoadTexture(L"Data/Images/lootbox1.png");
            if (texture)
            {
                texture->setSmooth(false);
                lootboxSprite.setTexture(*texture);
                lootboxSprite.setOrigin(texture->getSize().x/2, texture->getSize().y/2);
            }
            gs::PushInterface(this); active = true; entity->SortAbove(this);
            Resize(gs::width, gs::height);
        }
        else if (active) { gs::RemoveInterface(this); active = false; ic::DeleteImage(L"Data/Images/lootbox1.png"); }
    }
}
