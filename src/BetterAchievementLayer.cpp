#include "BetterAchievementLayer.hpp"

bool BetterAchievementLayer::init() {
    if (!CCLayer::init()) {
        return false;
    }

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
    float tabScale = 0.75f;
    this->getAllAchievements();

    this->setID("BetterAchievementLayer");

    CCSprite* backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");

    CCMenuItemSpriteExtra* backBtn = Build<CCMenuItemSpriteExtra>::create(backSpr, this, menu_selector(BetterAchievementLayer::onClose))
        .id("back-button")
        .collect();

    CCMenu* backMenu = Build<CCMenu>::create()
        .id("back-menu")
        .pos(ccp(director->getScreenLeft() + 25.f, director->getScreenTop() - 22.f))
        .zOrder(1)
        .parent(this)
        .child(backBtn)
        .collect();

    this->setKeyboardEnabled(true);
    this->setKeypadEnabled(true);

    auto bg = geode::createLayerBG();
    bg->setID("content-background");
    this->addChild(bg);

    auto listView = ListView::create(CCArray::create(), 0.f, 358.f, 220.f);

    Build<GJListLayer>::create(listView, "Main Levels", ccc4(168, 85, 44, 255), 358.f, 220.f, 0)
        .anchorPoint({0.5f, 0.55f})
        .id("list-layer")
        .parent(this)
        .ignoreAnchorPointForPos(false)
        .center()
        .store(m_listLayer);

    Build<ScrollLayer>::create(winSize / 1.5f)
        .id("achievement-scroll-layer")
        .zOrder(1)
        .scale(0.9f)
        .anchorPoint({0.45f, 0.55f})
        .ignoreAnchorPointForPos(false)
        .center()
        .parent(this)
        .store(m_scrollLayer);

    m_scrollLayer->m_scrollLimitTop = 35.f;
    m_scrollLayer->m_scrollLimitBottom = -15.f;
    m_scrollLayer->setContentHeight(240.f);
    m_scrollLayer->m_contentLayer->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
            ->setAutoGrowAxis(this->getContentSize().height)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(true)
            ->setGap(37.5f)
            
    );

    m_scrollLayer->scrollToTop();
    m_scrollLayer->setPositionX(m_scrollLayer->getPositionX() - 4.f);

    auto topBar = m_listLayer->getChildByID("top-border");

    Build<CCMenu>::create()
        .id("button-menu")
        .contentSize(topBar->getScaledContentSize())
        .pos({topBar->getPositionX(), topBar->getPositionY() + 25.f})
        .zOrder(0)
        .layout(RowLayout::create()->setAutoScale(false)->setGap(1.f))
        .parent(m_listLayer)
        .store(buttonMenu);

    auto createTab = [=](TabButton* tab, std::string text, int tag) {
        tab = TabButton::create(text.c_str(), this, menu_selector(BetterAchievementLayer::onLoadPage));
        tab->setScale(tabScale);
        tab->setTag(tag);
        buttonMenu->addChild(tab);

        auto tabButtonSprite = static_cast<TabButtonSprite*>(tab->getChildren()->objectAtIndex(0));
        auto tabText = static_cast<CCLabelBMFont*>(tabButtonSprite->getChildren()->objectAtIndex(0));
        this->m_unselectedTabs.push_back(tabButtonSprite);

        auto selectedTabSprite = CCSprite::createWithSpriteFrameName("tab-selected.png"_spr);
        auto selectedTab = static_cast<CCMenuItemSpriteExtra*>(tab->getChildren()->objectAtIndex(1));
        selectedTab->setID("tab-selected");
        selectedTab->setTag(tag);
        selectedTab->setSprite(selectedTabSprite);
        this->m_selectedTabs.push_back(selectedTab);

        auto selectedTabText = static_cast<CCLabelBMFont*>(tabText->getChildren()->objectAtIndex(0));
        selectedTab->addChild(selectedTabText);
    };

    createTab(m_mainLevelsTab, "Main Levels", 1001);
    createTab(m_userLevelsTab, "User Levels", 1002);
    createTab(m_accountTab, "Account", 1003);
    createTab(m_secretTab, "Secret", 1004);
    createTab(m_shardsAndPathsTab, "Shards and Paths", 1005);

    m_categoryTitle = static_cast<CCLabelBMFont*>(m_listLayer->getChildByID("title"));

    this->loadPage(this->mainLevelsKeys);
    this->m_selectedTabs.front()->setVisible(true);

    Build<CCSprite>::createSpriteName("percent-button.png"_spr)
        .id("percent-button")
        .scale(.75f)
        .intoMenuItem([this]() {
            FLAlertLayer::create(
                "Achievement Progress",
                fmt::format("<cg>Total</c> achievements: {}%\n<cg>{}</c> achievements: {}%", 
                    (int)this->getAllAchievementsPercent(),
                    m_categoryTitle->getString(),
                    (int)this->getCategoryAchievementsPercent()
                ),
                "Ok"
            )->show();
        })
        .pos({director->getScreenLeft() + 20.f, director->getScreenBottom() + 20.f})
        .intoNewParent(CCMenu::create())
        .pos(0.f, 0.f)
        .parent(this);

    buttonMenu->updateLayout();

    return true;
}

void BetterAchievementLayer::getAllAchievements() {
    for (auto achievement : CCArrayExt<CCDictionary*>(AchievementManager::sharedState()->m_allAchievementsSorted)) {
        Achievement* _achievement = new Achievement(
            achievement->valueForKey("identifier")->m_sString,
            achievement->valueForKey("title")->m_sString,
            achievement->valueForKey("achievedDescription")->m_sString,
            achievement->valueForKey("unachievedDescription")->m_sString,
            achievement->valueForKey("icon")->m_sString,
            achievement->valueForKey("limits")->m_sString,
            achievement->valueForKey("order")->m_sString
        );

        this->achievements.push_back(_achievement);
    }
}

void BetterAchievementLayer::loadPage(std::vector<std::string> keys) {
    m_scrollLayer->m_contentLayer->removeAllChildren();

    for (auto achievement : this->achievements) {
        for (auto key : keys) {
            if (achievement->identifier.find(key) != std::string::npos) {
                auto hideCompleted = Mod::get()->getSettingValue<bool>("hide-completed-achievements");
                if (AchievementManager::sharedState()->isAchievementEarned(achievement->identifier.c_str()) && hideCompleted) {
                    continue;
                }

                auto achievementCell = BetterAchievementCell::create(achievement);
                achievementCell->m_bg->setContentWidth(m_scrollLayer->getScaledContentWidth() + 25.f);

                m_scrollLayer->m_contentLayer->addChild(achievementCell);
                m_scrollLayer->m_contentLayer->updateLayout();
            }
        }
    }

    this->m_scrollLayer->scrollToTop();
}

void BetterAchievementLayer::onLoadPage(CCObject* sender) {
    int tag = sender->getTag();

    for (auto tab : this->m_unselectedTabs) {
        tab->setVisible(true);
    }
    for (auto tab : this->m_selectedTabs) {
        tab->setVisible(false);
        if (tab->getTag() == tag) {
            tab->setVisible(true);

        }
    }

    this->m_scrollLayer->scrollToTop();

    switch(tag) {
        case 1001:
            this->loadPage(this->mainLevelsKeys);
            this->m_categoryTitle->setString("Main Levels");
            break;
        case 1002:
            this->loadPage(this->userLevelsKeys);  
            this->m_categoryTitle->setString("User Levels");     
            break;
        case 1003:
            this->loadPage(this->accountKeys);
            this->m_categoryTitle->setString("Account");
            break;
        case 1004:
            this->loadPage(this->secretKeys);
            this->m_categoryTitle->setString("Secret");
            break;
        case 1005:
            this->loadPage(this->shardsAndPathsKeys);
            this->m_categoryTitle->setString("Shards and Paths");
            break;
    }
}

float BetterAchievementLayer::getAllAchievementsPercent() {
    float achievementPercent = 0.f;
    int totalEarned = 0;
    for (auto achievement : this->achievements) {
        if (AchievementManager::sharedState()->isAchievementEarned(achievement->identifier.c_str())) {
            totalEarned++;
        }
    }
    achievementPercent = (static_cast<float>(totalEarned) / this->achievements.size()) * 100;
    return achievementPercent;
}

float BetterAchievementLayer::getCategoryAchievementsPercent() {
    float achievementPercent = 0.f;
    int totalEarned = 0;
    auto array = CCArrayExt<BetterAchievementCell*>(m_scrollLayer->m_contentLayer->getChildren());
    for (auto cell : array) {
        auto identifier = cell->getID();
        if (AchievementManager::sharedState()->isAchievementEarned(identifier.c_str())) {
            totalEarned++;
        }
    }
    achievementPercent = (static_cast<float>(totalEarned) / array.size()) * 100;
    return achievementPercent;
}

void BetterAchievementLayer::onClose(CCObject*) {
    auto mainMenu = MenuLayer::scene(false);
    CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, mainMenu));
}

void BetterAchievementLayer::keyBackClicked() {
    this->onClose(nullptr);
}

BetterAchievementLayer* BetterAchievementLayer::create() {
    auto ret = new BetterAchievementLayer();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

CCScene* BetterAchievementLayer::scene() {
    auto layer = BetterAchievementLayer::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}