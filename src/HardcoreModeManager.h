#pragma once
#include "LivesBox.h"

#include <Geode/modify/LevelInfoLayer.hpp>

class HardcoreModeManager {
private:
    static HardcoreModeManager* instancePtr;
public:
    int CurrentLives = 4; // just in case
    LivesBox* livesBox;
    bool IsInHardcoreMode = false;
    bool notFirstLevelReset = false;
    LevelInfoLayer* currentLevelInfoLayer;
    CCObject* currentOnPlaySender;

    HardcoreModeManager() { }
    static HardcoreModeManager* getInstance();

    void PostPlayLayerInit(PlayLayer* layer);
    void AfterPlayLayerResettedLevel();
    void writeLockoutForLevel(int levelId);
    int checkIfLockedOutOfLevelId(int levelId);
};