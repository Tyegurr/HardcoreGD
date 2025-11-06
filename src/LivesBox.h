#pragma once
#include <Geode/Geode.hpp>
#include "LifeSprite.h"

#include <vector>

using namespace geode::prelude;

class LivesBox : public CCNode {
protected:
    virtual bool init(int numberOfLivesToReference);
public:
    static LivesBox* create(int numberOfLivesToReference);
    std::vector<LifeSprite*> LifeSprites;

    void update(float delta) override;
};