#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

#include "BetterAchievementLayer.hpp"

using namespace geode::prelude;

class $modify(MenuLayer) {
    void onAchievements(CCObject* s) {
        auto betterAchievementsLayer = BetterAchievementLayer::scene();
        CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, betterAchievementsLayer));
    }
};