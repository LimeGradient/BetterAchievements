#pragma once

#include <Geode/Geode.hpp>
#include <Geode/binding/AchievementManager.hpp>

#include "Achievement.hpp"
#include "BetterAchievementCell.hpp"

using namespace geode::prelude;

class BetterAchievementLayer : public CCLayer {
protected:
    CCScale9Sprite* m_bg;
    CCSprite* m_outline;
    ScrollLayer* m_scrollLayer;

    std::vector<Achievement*> achievements;
    void getAllAchievements();

    void onClose(CCObject*);

public:
    static BetterAchievementLayer* create();
    static CCScene* scene();
    bool init();
};