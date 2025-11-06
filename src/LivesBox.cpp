#include "LivesBox.h"

bool LivesBox::init(int numberOfLivesToReference) {
    if (!CCNode::init()) return false;

    this->setPositionY(20);
    this->setAnchorPoint({ 0.5, 0.0 });
    this->setContentSize({ 135, 0 });
    this->scheduleUpdate();

    for (int i = 0; i < numberOfLivesToReference; i++) {
        LifeSprite* lifeSprite = LifeSprite::create();
        lifeSprite->setPositionX(i*45);
        this->addChild(lifeSprite);
        LifeSprites.push_back(lifeSprite);
    }

    return true;
}

LivesBox* LivesBox::create(int numberOfLivesToReference) {
    LivesBox* newLivesBox = new LivesBox();
    newLivesBox->init(numberOfLivesToReference);
    return newLivesBox;
}

void LivesBox::update(float delta) {
    if (!CCNode::init()) return;

    // there's probably an easier way to do this that doesn't involve update method. but for now this will do
    CCSize contentSize = CCScene::get()->getContentSize();
    this->setPositionX(contentSize.width / 2);
}