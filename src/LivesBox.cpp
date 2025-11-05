#include "LivesBox.h"

bool LivesBox::init(int numberOfLivesToReference) {
    if (!CCNode::init()) return false;

    this->setPositionX(220);
    this->setPositionY(38);

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