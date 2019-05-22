//
//  NovelSystem.cpp
//  NovelSome
//
//  Created by Никита Исаенко on 08/09/2018.
//  Copyright © 2018 Melancholy Hill. All rights reserved.
//

#include "NovelSystem.hpp"

namespace ns
{
    NovelObject::~NovelObject() { }
    void NovelObject::Init() { }
    void NovelObject::Update(const sf::Time&) { }
    void NovelObject::Draw(sf::RenderWindow*) { }
    void NovelObject::Resize(unsigned int width, unsigned int height) { }
    void NovelObject::PollEvent(sf::Event& event) { }
    void NovelObject::Destroy() { }
    void NovelObject::ReceiveMessage(MessageHolder&) { }
    void NovelObject::SendMessage(MessageHolder message) { ReceiveMessage(message); }
    void NovelObject::SetPriority(int priority) { this->priority = priority; /* TODO: Sorting */ }
    void NovelObject::ChangePriority(int priority) {
        if (novelSystem != nullptr)
            novelSystem->ChangePriorityOf(this, priority); }
    void NovelObject::SetNovelSystem(NovelSystem* novelSystem) { this->novelSystem = novelSystem; }
    NovelSystem* NovelObject::GetNovelSystem() { return novelSystem; }
    
    
    
    NovelSystem::NovelSystem() { }
    void NovelSystem::Update(const sf::Time& elapsedTime)
    {
        list<NovelObject*>::iterator it = objects.begin();
        while (it != objects.end())
        {
            if ((*it)->offline) { delete (*it); objects.erase(it++); }
            else { (*it)->Update(elapsedTime); ++it; }
        }
    }
    void NovelSystem::Draw(sf::RenderWindow* window)
    {
        if (objects.size())
            for (auto e : objects)
                if (!e->offline) e->Draw(window);
    }
    void NovelSystem::Resize(unsigned int width, unsigned int height)
    {
        if (objects.size())
            for (auto e : objects)
                if (!e->offline) e->Resize(width, height);
    }
    void NovelSystem::PollEvent(sf::Event& event)
    {
        if (objects.size())
            for (auto e : objects)
                if (!e->offline) e->PollEvent(event);
    }
    void NovelSystem::PopComponent(NovelObject* component)
    {
        component->offline = true;
        component->Destroy();
    }
    void NovelSystem::SendMessage(MessageHolder message) { ReceiveMessage(message); }
    void NovelSystem::ReceiveMessage(MessageHolder& message) {
        if (objects.size())
            for (auto e : objects)
                if (!e->offline) e->ReceiveMessage(message);
    }
    void NovelSystem::clear()
    {
        list<NovelObject*>::iterator it = objects.begin();
        while (it != objects.end()) { if (!(*it)->offline) (*it)->Destroy(); delete (*it); objects.erase(it++); }
        objects.clear();
    }
    void NovelSystem::ChangePriorityOf(NovelObject* component, int priority)
    {
        std::list<NovelObject*>::iterator it = std::find(objects.begin(), objects.end(), component);
        if (it != objects.end())
        {
            objects.erase(it);
            
            bool done{ false };
            for (list<NovelObject*>::iterator it = objects.begin(); it != objects.end() && !done; ++it)
                if ((*it)->priority > priority)
                {
                    objects.insert(it, component);
                    done = true;
                }
            if (!done)
                objects.push_back(component);
        }
    }
    list<NovelObject*>::iterator NovelSystem::begin() { return objects.begin(); }
    list<NovelObject*>::iterator NovelSystem::end() { return objects.end(); }
    list<NovelObject*>::const_iterator NovelSystem::cbegin() const { return objects.cbegin(); }
    list<NovelObject*>::const_iterator NovelSystem::cend() const { return objects.cend(); }
}
