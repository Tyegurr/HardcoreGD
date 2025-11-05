#include "LifeSprite.h"

bool LifeSprite::init() {
    if (!CCNode::init()) return false;

    deadSprite = CCSprite::createWithSpriteFrameName("HeartBroken.png"_spr);
    aliveSprite = CCSprite::createWithSpriteFrameName("Heart.png"_spr);
    aliveSprite->setZOrder(1);
    this->addChild(deadSprite);
    this->addChild(aliveSprite);

    return true;
}

LifeSprite* LifeSprite::create() {
    LifeSprite* newLifeSprite = new LifeSprite();
    newLifeSprite->init();
    return newLifeSprite;
}