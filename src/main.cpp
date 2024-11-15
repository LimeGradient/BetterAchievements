#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/AchievementNotifier.hpp>
#include <Geode/modify/AchievementBar.hpp>

#include "BetterAchievementLayer.hpp"
#include "BetterAchievementNotifier.hpp"

using namespace geode::prelude;

class $modify(MenuLayer) {
    void onAchievements(CCObject* s) {
        auto betterAchievementsLayer = BetterAchievementLayer::scene();
        CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, betterAchievementsLayer));
    }
};

class $modify(AchievementBar) {
    bool init(char const* title, char const* desc, char const* icon, bool quest) {
        if (Mod::get()->getSettingValue<std::string>("achievement-notifier-type") == "Geometry Dash") {
            AchievementBar::init(title, desc, icon, quest);
        } else {
            BetterAchievementNotifier::create(title, desc, icon, quest);
        }
    }
}