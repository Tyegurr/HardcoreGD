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
                auto castedPlayMenu = static_cast<CCMenu*>(playMenu);
                castedPlayMenu->setOpacity(80);
            }
            auto leftSideMenu = this->getChildByID("left-side-menu");
            if (leftSideMenu)
            {
                auto copyButton = leftSideMenu->getChildByID("copy-button");
                if (copyButton)
                {
                    auto castedCopyButton = static_cast<CCMenuItemSpriteExtra*>(copyButton);
                    castedCopyButton->setOpacity(80);
                }
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
            auto leftSideMenu = this->getChildByID("left-side-menu");
            if (leftSideMenu)
            {
                auto copyButton = leftSideMenu->getChildByID("copy-button");
                if (copyButton)
                {
                    auto castedCopyButton = static_cast<CCMenuItemSpriteExtra*>(copyButton);
                    castedCopyButton->setOpacity(80);
                }
            }
        } else {
            auto playMenu = this->getChildByID("play-menu");
            if (playMenu) {
                auto castedPlayMenu = static_cast<CCMenu*>(playMenu);
                castedPlayMenu->setOpacity(255);
            }
            auto leftSideMenu = this->getChildByID("left-side-menu");
            if (leftSideMenu)
            {
                auto copyButton = leftSideMenu->getChildByID("copy-button");
                if (copyButton)
                {
                    auto castedCopyButton = static_cast<CCMenuItemSpriteExtra*>(copyButton);
                    castedCopyButton->setOpacity(255);
                }
            }
        }
    }

    void tryCloneLevel(CCObject* sender) {
        HardcoreModeManager::getInstance()->currentOnPlaySender = sender;
        int lockoutCheck = HardcoreModeManager::getInstance()->checkIfLockedOutOfLevelId(m_fields->m_currentLevel->m_levelID.value());
        if (lockoutCheck != -1)
        {
            FLAlertLayer::create("Uh oh!", fmt::format("You're locked out of this level for the next {} seconds!", lockoutCheck), "Okay...")->show();
            return;
        }
        LevelInfoLayer::tryCloneLevel(sender);
    }

    void onPlay(CCObject* sender) {
        HardcoreModeManager::getInstance()->currentOnPlaySender = sender;
        if (Mod::get()->getSettingValue<bool>("always-hardcore") == false) {
            if (m_fields->m_inDifficultyPrompt == false) {
                int lockoutCheck = HardcoreModeManager::getInstance()->checkIfLockedOutOfLevelId(m_fields->m_currentLevel->m_levelID.value());
                if (lockoutCheck != -1)
                {
                    FLAlertLayer::create("Uh oh!", fmt::format("You're locked out of this level for the next {} seconds!", lockoutCheck), "Okay...")->show();
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
        } else {
            HardcoreModeManager::getInstance()->IsInHardcoreMode = true;
            LevelInfoLayer::onPlay(sender);
        }
    }
};