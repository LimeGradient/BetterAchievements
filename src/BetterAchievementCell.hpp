#pragma once

#include <Geode/Geode.hpp>
#include <Geode/binding/AchievementManager.hpp>
#include <Geode/modify/AchievementCell.hpp>

#include "Achievement.hpp"

using namespace geode::prelude;

class BetterAchievementCell : public CCNode {
protected:
    bool init(Achievement* achievement);
    UnlockType iconToUnlockType(std::string icon);
    int iconToIconIndex(std::string icon);

public:
    static BetterAchievementCell* create(Achievement* achievement);

    CCScale9Sprite* m_bg;
    GJItemIcon* m_icon;
    CCLabelBMFont* m_titleText;
    CCLabelBMFont* m_descText;
};