#pragma once

#include <Geode/Geode.hpp>
#include <Geode/binding/AchievementManager.hpp>

#include "Achievement.hpp"

using namespace geode::prelude;

class BetterAchievementCell : public CCNode {
protected:
    bool init(Achievement* achievement);

public:
    static BetterAchievementCell* create(Achievement* achievement);

    CCScale9Sprite* m_bg;
    CCSprite* m_icon;
};