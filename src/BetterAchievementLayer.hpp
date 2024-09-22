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

    std::vector<std::string> mainLevelsKeys = {"level", "tower", "coins", "mappacks", "gauntlets", "daily"};
    std::vector<std::string> userLevelsKeys = {"demon", "stars", "moons", "usercoins", "diamonds", "lists"};
    std::vector<std::string> accountKeys = {"custom", "rate", "jump", "attempts", "like", "friends", "submit", "creator"};
    std::vector<std::string> secretKeys = {"special", "secret"};
    std::vector<std::string> shardsAndPathsKeys = {"shard", "path"};
    std::vector<std::string> miscKeys = {"facebook", "youtube", "twitter", "steam"};

    std::vector<Achievement*> achievements;
    void getAllAchievements();
    void loadPage(std::vector<std::string> keys);

    void onClose(CCObject*);
    void onLoadPage(CCObject* sender);

public:
    static BetterAchievementLayer* create();
    static CCScene* scene();
    bool init();
};