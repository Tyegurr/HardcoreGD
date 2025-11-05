#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class LifeSprite : public CCNode {
protected:
    virtual bool init();
public:
    CCSprite* deadSprite;
    CCSprite* aliveSprite;
    static LifeSprite* create();
};