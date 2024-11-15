#include "BetterAchievementNotifier.hpp"

bool BetterAchievementNotifier::init(char const* title, char const* desc, char const* icon, bool quest) {
    std::string notifierType = Mod::get()->getSettingValue<std::string>("achievement-notifier-type");

    if (notifierType == "Playstation 3") {
        Build<CCSprite>::createSpriteName("GJ_colorBtn_001.png")
            .width(80.f)
            .height(40.f)
            .id("ps3-trophy")
            .center()
            .parent(this);
    } else if (notifierType == "Playstation 4") {

    } else if (notifierType == "Playstation 5") {

    } else if (notifierType == "Xbox 360") {

    } else if (notifierType == "Xbox One") {

    }

    return true;
}

BetterAchievementNotifier* BetterAchievementNotifier::create(char const* title, char const* desc, char const* icon, bool quest) {
    auto ret = new BetterAchievementNotifier();
    if (ret && ret->init(title, desc, icon, quest)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}