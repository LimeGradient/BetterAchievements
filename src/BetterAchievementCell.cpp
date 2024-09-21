#include "BetterAchievementCell.hpp"

bool BetterAchievementCell::init(Achievement* achievement) {
    bool isUnlocked = AchievementManager::sharedState()->isAchievementEarned(achievement->identifier.c_str());
    log::info("identifier: {}", achievement->identifier);
    log::info("icon: {}", achievement->icon);
    
    m_bg = CCScale9Sprite::create("square02b_small.png");
    m_bg->setID("bg");
    m_bg->setOpacity(0);
    m_bg->ignoreAnchorPointForPosition(false);
    m_bg->setAnchorPoint({ .0f, .0f });
    m_bg->setScale(.7f);
    m_bg->setColor(ccWHITE);
    m_bg->setOpacity(25);
    this->addChild(m_bg);

    this->iconToIconIndex(achievement->icon);

    return true;
}

std::vector<std::string> split(std::string s) {
    
}

UnlockType BetterAchievementCell::iconToUnlockType(std::string icon) {
    auto iconTypeRaw = icon.substr(0, icon.find("_"));

    if (iconTypeRaw == "icon") return UnlockType::Cube;
    if (iconTypeRaw == "ship") return UnlockType::Ship;
    if (iconTypeRaw == "ball") return UnlockType::Ball;
    if (iconTypeRaw == "bird") return UnlockType::Bird;
    if (iconTypeRaw == "dart") return UnlockType::Dart;
    if (iconTypeRaw == "robot") return UnlockType::Robot;
    if (iconTypeRaw == "spider") return UnlockType::Spider;
    if (iconTypeRaw == "swing") return UnlockType::Swing;
    if (iconTypeRaw == "jetpack") return UnlockType::Jetpack;
    if (iconTypeRaw == "color") return UnlockType::Col1;
    if (iconTypeRaw == "color2") return UnlockType::Col2;
}

int BetterAchievementCell::iconToIconIndex(std::string icon) {
    auto iconTypeRaw = icon.substr(1, icon.find("_"));
    log::info("raw shit: {}", iconTypeRaw);

    return 0;
}

BetterAchievementCell* BetterAchievementCell::create(Achievement* achievement) {
    auto ret = new BetterAchievementCell();
    if (ret && ret->init(achievement)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}