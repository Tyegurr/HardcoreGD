#include <ctime>
#include <sstream>
#include <string>
#include <filesystem>
#include "HardcoreModeManager.h"

namespace fs = std::filesystem;

HardcoreModeManager* HardcoreModeManager::instancePtr = nullptr;
HardcoreModeManager* HardcoreModeManager::getInstance() {
    if (instancePtr == nullptr) {
        instancePtr = new HardcoreModeManager();
    }
    return instancePtr;
}

void HardcoreModeManager::PostPlayLayerInit(PlayLayer* layer) {
    CurrentLives = 4;
    
    if (IsInHardcoreMode)
    {
        livesBox = LivesBox::create(CurrentLives);
        livesBox->setZOrder(200);
        layer->m_uiLayer->addChild(livesBox);
        //layer->addChild(livesBox);
        //layer->getChildByID("m_uiLayer")->addChild(livesBox);
    }
    //CurrentLives += 1; // another cheap solution but it works 
}

void HardcoreModeManager::AfterPlayLayerResettedLevel() {
    if (IsInHardcoreMode)
    {
        int OldLives = CurrentLives;

        if (notFirstLevelReset == true) {
            CurrentLives -= 1;
            livesBox->LifeSprites[OldLives - 1]->aliveSprite->setVisible(0);
        }
    }
}
void HardcoreModeManager::writeLockoutForLevel(int levelId) {
    if (IsInHardcoreMode) // cheap problems require cheap solutions or something
    {
        log::info("Locking player out of level id {}", levelId);
        std::time_t currentTime = time(0); // be gay and take it your way only at burger queer (this data type is not fixed for the year 2038 bug. but its fine for now. whatever works)
        Mod::get()->setSavedValue<int>(fmt::format("{}_gdhlf", levelId), currentTime);
    }
}
int HardcoreModeManager::checkIfLockedOutOfLevelId(int levelId) {
    //std::ostringstream fileName;
    //fileName << Mod::get()->getSaveDir().string() << "/" << levelId << ".gdhlf";

    auto result = Mod::get()->getSavedValue<int>(fmt::format("{}_gdhlf", levelId));

    if (result) {
        int lockoutTime = result + 900;

        std::time_t currentTime = time(0);
        int diff = lockoutTime - currentTime;
        // removes saved entry if the level is due to be unlocked
        if (diff <= 0) {
            Mod::get()->getSaveContainer().erase(fmt::format("{}_gdhlf", levelId)); // storage space is precious
            return -1;
        }
        return lockoutTime - currentTime;
    }

    return -1;
}