#pragma once

#include <Geode/Geode.hpp>
#include <Geode/fmod/fmod.h>
#include <UIBuilder.hpp>

#include "Achievement.hpp"

using namespace geode::prelude;

class BetterAchievementNotifier : public CCNode {
protected:
    bool init(char const* title, char const* desc, char const* icon, bool quest);

public:
    static BetterAchievementNotifier* create(char const* title, char const* desc, char const* icon, bool quest);
};