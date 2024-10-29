#include "BetterAchievementCell.hpp"

bool BetterAchievementCell::init(Achievement* achievement) {
    bool isUnlocked = AchievementManager::sharedState()->isAchievementEarned(achievement->identifier.c_str());
    this->setID(achievement->identifier);

    Build<CCScale9Sprite>::create("square02b_small.png")
        .id("background")
        .opacity(25)
        .ignoreAnchorPointForPos(false)
        .anchorPoint({0.f, 0.f})
        .scale(.85f)
        .scaleX(.95f)
        .color(ccWHITE)
        .parent(this)
        .store(m_bg);

    auto unlockType = this->iconToUnlockType(achievement->icon);
    auto iconIndex = this->iconToIconIndex(achievement->icon);

    if (static_cast<int>(unlockType) < 1) {
        unlockType = UnlockType::Cube;
    }

    if (iconIndex < 1) {
        iconIndex = 1;
    }

    Build<GJItemIcon>::create(unlockType, iconIndex, ccc3(175, 175, 175),ccc3(255, 255, 255), false, false, true, ccc3(255, 255, 255))
        .id("achievement-icon")
        .scale(0.625f)
        .pos({17.5f, 17.5f})
        .parent(this)
        .store(m_icon);

    if (m_icon->m_unlockType == UnlockType::Col1 || m_icon->m_unlockType == UnlockType::Col2) {
        Build<CCLabelBMFont>::create((m_icon->m_unlockType == UnlockType::Col1) ? "1" : "2", "bigFont.fnt")
            .scale(m_icon->getScale())
            .pos(m_icon->getContentSize() / 2)
            .parent(m_icon)
            .collect();
    }

    if (!isUnlocked) {
        Build<CCSprite>::createSpriteName("GJ_lock_001.png")
            .scale(.825f)
            .pos(m_icon->getContentSize() / 2)
            .parent(m_icon)
            .collect();
    }

    m_titleText = CCLabelBMFont::create(achievement->title.c_str(), "bigFont.fnt");
    m_titleText->setID("achievement-title");
    m_titleText->setScale(0.35f);
    m_titleText->setPosition({m_icon->getPositionX() + 145.f, m_icon->getPositionY() + 7.f});
    this->addChild(m_titleText);
    
    m_descText = CCLabelBMFont::create((isUnlocked) ? achievement->achievedDescription.c_str() : achievement->unachievedDescription.c_str(), "bigFont.fnt");
    m_descText->setID("achievement-description");
    m_descText->setScale(0.2f);
    m_descText->setPosition({m_titleText->getPositionX(), m_titleText->getPositionY() - 13.f});
    this->addChild(m_descText);

    if (Mod::get()->getSettingValue<bool>("show-achievement-percentage")) {
        int percentCompleted = std::clamp(AchievementManager::sharedState()->percentForAchievement(achievement->identifier.c_str()), 0, 100);
        auto percentageLabel = CCLabelBMFont::create(fmt::format("{}%", percentCompleted).c_str(), "bigFont.fnt");
        percentageLabel->setScale(0.35f);
        percentageLabel->setPosition({300.f, 18.f});
        this->addChild(percentageLabel);
    }

    return true;
}

std::vector<std::string> split(std::string s, char del) {
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string token;
    while(std::getline(ss, token, del)) {
        tokens.push_back(token);
    }
    return tokens;
}

UnlockType BetterAchievementCell::iconToUnlockType(std::string icon) {
    auto iconTypeRaw = split(icon, '_').front();

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
    if (iconTypeRaw == "streak") return UnlockType::Streak;
    if (iconTypeRaw == "death") return UnlockType::Death;

    return UnlockType::Cube;
}

int BetterAchievementCell::iconToIconIndex(std::string icon) {
    auto iconTypeRaw = split(icon, '_').back();
    return atoi(iconTypeRaw.c_str());
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