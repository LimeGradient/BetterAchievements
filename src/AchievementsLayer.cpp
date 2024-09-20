#include <Geode/Geode.hpp>
#include <Geode/modify/AchievementsLayer.hpp>
#include <Geode/binding/AchievementManager.hpp>

using namespace geode::prelude;

class $modify(BetterAchievementsLayer, AchievementsLayer) {
    void customSetup() {
        AchievementsLayer::customSetup();

        log::info("achievements: {}", AchievementManager::sharedState()->m_allAchievementsSorted);
    }
};