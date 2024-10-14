#include "BetterAchievementCell.hpp"

bool BetterAchievementCell::init(Achievement* achievement) {
    bool isUnlocked = AchievementManager::sharedState()->isAchievementEarned(achievement->identifier.c_str());
    this->setID(achievement->identifier);
    
    m_bg = CCScale9Sprite::create("square02b_small.png");
    m_bg->setID("better-achievement-background");
    m_bg->setOpacity(0);
    m_bg->ignoreAnchorPointForPosition(false);
    m_bg->setAnchorPoint({ .0f, .0f });
    m_bg->setScale(.85f);
    m_bg->setScaleX(.95f);
    m_bg->setColor(ccWHITE);
    m_bg->setOpacity(25);
    this->addChild(m_bg);

    auto unlockType = this->iconToUnlockType(achievement->icon);
    auto iconIndex = this->iconToIconIndex(achievement->icon);

    if (static_cast<int>(unlockType) < 1) {
        unlockType = UnlockType::Cube;
    }

    if (iconIndex < 1) {
        iconIndex = 1;
    }

    m_icon = GJItemIcon::create(
        unlockType,
        iconIndex,
        ccc3(175, 175, 175),
        ccc3(255, 255, 255), 
        false, 
        false, 
        true, 
        ccc3(255, 255, 255)
    );
    m_icon->setID("achievement-icon");
    m_icon->setScale(0.625f);
    m_icon->setPositionY(17.5f);
    m_icon->setPositionX(17.5f);
    this->addChild(m_icon);

    if (m_icon->m_unlockType == UnlockType::Col1 || m_icon->m_unlockType == UnlockType::Col2) {
        auto colText = CCLabelBMFont::create((m_icon->m_unlockType == UnlockType::Col1) ? "1" : "2", "bigFont.fnt");
        colText->setScale(m_icon->getScale());
        colText->setPosition({m_icon->getPositionX() - 3.f, m_icon->getPositionY() - 2.f});
        m_icon->addChild(colText);
    }

    if (!isUnlocked) {
        auto lockSpr = CCSprite::createWithSpriteFrameName("GJ_lock_001.png");
        lockSpr->setScale(0.825f);
        lockSpr->setPosition({m_icon->getPositionX() - 3.f, m_icon->getPositionY() - 2.f});
        m_icon->addChild(lockSpr);
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
    log::info("{}", icon);

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