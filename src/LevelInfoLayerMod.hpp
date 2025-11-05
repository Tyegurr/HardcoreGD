#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <sstream>
#include "HardcoreModeManager.h"

using namespace geode::prelude;

class DifficultySelectionAlertLayerProtocol : public FLAlertLayerProtocol {
private:
    CCObject* _sender;
    LevelInfoLayer* _layer;
public:
    DifficultySelectionAlertLayerProtocol(CCObject* sender, LevelInfoLayer* layer) : FLAlertLayerProtocol() {
        _sender = sender;
        _layer = layer;
    }
    void FLAlert_Clicked(FLAlertLayer* p0, bool p1) override {
        log::info("yoo {}", p1);
        if (p1 == false) {
            HardcoreModeManager::getInstance()->IsInHardcoreMode = false;
            _layer->onPlay(_sender);
        } else {
            HardcoreModeManager::getInstance()->IsInHardcoreMode = true;
            _layer->onPlay(_sender);
        }
    }
};

class $modify(HardcoreLevelInfoLayer, LevelInfoLayer) {
    struct Fields {
        GJGameLevel* m_currentLevel;
        bool m_inDifficultyPrompt;
    };

    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;
        m_fields->m_currentLevel = level;

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
                auto castedPlayMenu = dynamic_cast<CCMenu*>(playMenu);
                castedPlayMenu->setOpacity(80);
            }
        } else {
            auto playMenu = this->getChildByID("play-menu");
            if (playMenu) {
                auto castedPlayMenu = dynamic_cast<CCMenu*>(playMenu);
                castedPlayMenu->setOpacity(255);
            }
        }
    }

    void onPlay(CCObject* sender) {
        if (m_fields->m_inDifficultyPrompt == false) {
            int lockoutCheck = HardcoreModeManager::getInstance()->checkIfLockedOutOfLevelId(m_fields->m_currentLevel->m_levelID.value());
            if (lockoutCheck != -1)
            {
                std::ostringstream alertPopupContent;
                alertPopupContent << "You're locked out of this level for the next " << lockoutCheck << " seconds!";
                FLAlertLayer::create("Uh oh!", alertPopupContent.str(), "Okay...")->show();
                return;
            }
            m_fields->m_inDifficultyPrompt = true;
            FLAlertLayer* DifficultySelectionAlert = FLAlertLayer::create(new DifficultySelectionAlertLayerProtocol(sender, this), "GET READY", "CHOOSE YOUR DIFFICULTY", "Normal", "HARDCORE");
            DifficultySelectionAlert->show();
        } else {
            LevelInfoLayer::onPlay(sender);
        }

        //LevelInfoLayer::onPlay(sender);
    }
};