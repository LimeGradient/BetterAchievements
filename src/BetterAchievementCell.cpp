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

    auto icon = GJItemIcon::create(
        this->iconToUnlockType(achievement->icon),
        this->iconToIconIndex(achievement->icon),
        ccc3(175, 175, 175),
        ccc3(255, 255, 255), 
        false, 
        false, 
        true, 
        ccc3(255, 255, 255)
    );
    this->addChild(icon);

    return true;
}

std::vector<std::string> split(std::string s, std::string del) {
    std::vector<std::string> tokens;
    char* token = strtok(s.data(), del.c_str());
    while (token != NULL) {
        tokens.push_back(std::string(token));
        token = strtok(NULL, del.c_str());
    }
    return tokens;
}

UnlockType BetterAchievementCell::iconToUnlockType(std::string icon) {
    auto iconTypeRaw = split(icon, "_")[0];

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

    return UnlockType::Cube;
}

int BetterAchievementCell::iconToIconIndex(std::string icon) {
    if (!icon.empty()) {
        auto iconTypeRaw = split(icon, "_")[1];
        return atoi(iconTypeRaw.c_str());
    }

    return 10;
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