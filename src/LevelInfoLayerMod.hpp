#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <sstream>
#include "HardcoreModeManager.h"

using namespace geode::prelude;


class $modify(HardcoreLevelInfoLayer, LevelInfoLayer) {
    struct Fields {
        GJGameLevel* m_currentLevel;
        bool m_inDifficultyPrompt;
    };

    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;
        m_fields->m_currentLevel = level;

        HardcoreModeManager::getInstance()->currentLevelInfoLayer = this;

        int lockoutCheck = HardcoreModeManager::getInstance()->checkIfLockedOutOfLevelId(m_fields->m_currentLevel->m_levelID.value());
        if (lockoutCheck != -1) {
            auto playMenu = this->getChildByID("play-menu");
            if (playMenu) {
                auto castedPlayMenu = dynamic_cast<CCMenu*>(playMenu);
                castedPlayMenu->setOpacity(80);
            }
        }
        return true;
    }

    void levelDownloadFinished(GJGameLevel* p0) {
        LevelInfoLayer::levelDownloadFinished(p0);

        int lockoutCheck = HardcoreModeManager::getInstance()->checkIfLockedOutOfLevelId(m_fields->m_currentLevel->m_levelID.value());
        if (lockoutCheck != -1) {
            auto playMenu = this->getChildByID("play-menu");
            if (playMenu) {
                auto castedPlayMenu = static_cast<CCMenu*>(playMenu);
                castedPlayMenu->setOpacity(80);
            }
        } else {
            auto playMenu = this->getChildByID("play-menu");
            if (playMenu) {
                auto castedPlayMenu = static_cast<CCMenu*>(playMenu);
                castedPlayMenu->setOpacity(255);
            }
        }
    }

    void onPlay(CCObject* sender) {
        HardcoreModeManager::getInstance()->currentOnPlaySender = sender;
        if (m_fields->m_inDifficultyPrompt == false) {
            int lockoutCheck = HardcoreModeManager::getInstance()->checkIfLockedOutOfLevelId(m_fields->m_currentLevel->m_levelID.value());
            if (lockoutCheck != -1)
            {
                FLAlertLayer::create("Uh oh!", fmt::format("You're locked out of this level for thenext {} seconds!", lockoutCheck), "Okay...")->show();
                return;
            }
            //FLAlertLayer* DifficultySelectionAlert = FLAlertLayer::create(newDifficultySelectionAlertLayerProtocol(sender, this), "GET READY", "CHOOSE YOURDIFFICULTY", "Normal", "HARDCORE"); // this    causes a memory leak for the love of god don't use it
            geode::createQuickPopup(
                "GET READY",
                "CHOOSE YOUR DIFFICULTY",
                "Normal", "HARDCORE",
                [](auto, bool btn2) {
                    HardcoreModeManager::getInstance()->IsInHardcoreMode = btn2;
                    HardcoreModeManager::getInstance()->currentLevelInfoLayer->onPlay(HardcoreModeManager::getInstance()->currentOnPlaySender);
                }
            );
            m_fields->m_inDifficultyPrompt = true;
        } else {
            LevelInfoLayer::onPlay(sender);
        }
    }
};