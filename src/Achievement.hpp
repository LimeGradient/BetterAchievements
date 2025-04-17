#pragma once

#include <string>
#include <vector>

class Achievement {
public:
    Achievement(std::string i1, std::string i2, std::string i3, std::string i4, std::string i5, std::string i6, std::string i7) {
        this->identifier = i1;
        this->title = i2;
        this->achievedDescription = i3;
        this->unachievedDescription = i4;
        this->icon = i5;
        this->limits = i6;
        this->order = i7;
    }

    std::string identifier;
    std::string title;
    std::string achievedDescription;
    std::string unachievedDescription;
    std::string icon;
    std::string limits;
    std::string order;
};

class BetterAchievementManager {
public:
    static BetterAchievementManager* get() {
        static BetterAchievementManager instance;
        return &instance;
    }

    std::vector<Achievement*> pinnedAchievements;
};