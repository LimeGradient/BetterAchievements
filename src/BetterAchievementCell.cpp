#include "BetterAchievementCell.hpp"

bool BetterAchievementCell::init(Achievement* achievement) {
    bool isUnlocked = AchievementManager::sharedState()->isAchievementEarned(achievement->identifier.c_str());
    log::info("identifier: {}", achievement->identifier);
    
    m_bg = CCScale9Sprite::create("square02b_small.png");
    m_bg->setID("bg");
    m_bg->setOpacity(0);
    m_bg->ignoreAnchorPointForPosition(false);
    m_bg->setAnchorPoint({ .0f, .0f });
    m_bg->setScale(.7f);
    m_bg->setColor(ccWHITE);
    m_bg->setOpacity(25);
    this->addChild(m_bg);

    m_icon = CCSprite::createWithSpriteFrameName(fmt::format("{}.png", achievement->icon).c_str());
    this->addChild(m_icon);

    return true;
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