#include <Geode/modify/PlayLayer.hpp>
#include "HardcoreModeManager.h"

using namespace geode::prelude;

class $modify(HardcorePlayLayer, PlayLayer) {
    struct Fields {
        GJGameLevel* m_currentLevel;
        bool m_notFirstLevelReset;
    };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;
        HardcoreModeManager::getInstance()->PostPlayLayerInit(this);
        m_fields->m_currentLevel = level;
        return true;
    }

    void destroyPlayer(PlayerObject* p0, GameObject* p1) {
        PlayLayer::destroyPlayer(p0, p1);
    }

    void levelComplete() {
        PlayLayer::levelComplete();
        HardcoreModeManager::getInstance()->CurrentLives = 4;
    }

    void resetLevel() {
        PlayLayer::resetLevel();
        if (HardcoreModeManager::getInstance()->notFirstLevelReset == false) {
            HardcoreModeManager::getInstance()->CurrentLives = 4;
            HardcoreModeManager::getInstance()->AfterPlayLayerResettedLevel();
            HardcoreModeManager::getInstance()->notFirstLevelReset = true;
        } else {
            HardcoreModeManager::getInstance()->AfterPlayLayerResettedLevel();
        }

        if (HardcoreModeManager::getInstance()->CurrentLives <= 0) {
            PlayLayer::onQuit();
            HardcoreModeManager::getInstance()->writeLockoutForLevel(m_fields->m_currentLevel->m_levelID.value());
        }
    }

    void onQuit() {
        PlayLayer::onQuit();
        HardcoreModeManager::getInstance()->notFirstLevelReset = false;
    }
};