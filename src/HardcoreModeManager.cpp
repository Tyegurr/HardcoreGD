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
        layer->addChild(livesBox);
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

        std::ostringstream strBuilder_FileName; // yes that's how i see it you can't change my mind
        strBuilder_FileName << Mod::get()->getSaveDir().string() << "/" << levelId << ".gdhlf"; // GDHLF (Geometry Dash Hardcore Lockout File)

        std::time_t currentTime = time(0); // be gay and take it your way only at burger queer (this data type is not fixed for the year 2038 bug. but its fine for now. whatever works)

        std::ostringstream strBuilder_Data;
        strBuilder_Data << currentTime;
        //geode::utils::file::createDirectory("hardcoresavedata");
        geode::utils::file::writeString(strBuilder_FileName.str(), strBuilder_Data.str());
    }
}
int HardcoreModeManager::checkIfLockedOutOfLevelId(int levelId) {
    std::ostringstream fileName;
    fileName << Mod::get()->getSaveDir().string() << "/" << levelId << ".gdhlf";

    auto result = geode::utils::file::readString(fileName.str());

    if (result) {
        int valueInFile = std::stoi(*result);
        int lockoutTime = valueInFile + 900;

        std::time_t currentTime = time(0);
        int diff = lockoutTime - currentTime;
        if (diff <= 0) {
            // deletes file
            fs::remove(fileName.str());
            return -1;
        }
        return lockoutTime - currentTime;
    }

    return -1;
}