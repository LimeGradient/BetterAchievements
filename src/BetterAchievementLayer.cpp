#include "BetterAchievementLayer.hpp"

bool BetterAchievementLayer::init() {
    if (!CCLayer::init()) {
        return false;
    }

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
    float tabScale = 0.75f;

    this->setID("BetterAchievementLayer");

    CCSprite* backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    CCMenuItemSpriteExtra* backBtn = CCMenuItemSpriteExtra::create(backSpr, this, menu_selector(BetterAchievementLayer::onClose));
    backBtn->setID("back-button");

    CCMenu* backMenu = CCMenu::create();
    backMenu->setID("back-menu");
    backMenu->addChild(backBtn);
    this->addChild(backMenu, 1);

    backMenu->setPosition(ccp(director->getScreenLeft() + 25.f, director->getScreenTop() - 22.f));
    this->setKeyboardEnabled(true);
    this->setKeypadEnabled(true);

    auto bg = geode::createLayerBG();
    bg->setID("content-background");
    this->addChild(bg);

    auto listView = ListView::create(CCArray::create(), 0.f, 358.f, 220.f);
    m_listLayer = GJListLayer::create(listView, "Main Levels", ccc4(168, 85, 44, 255), 358.f, 220.f, 0);
    m_listLayer->setAnchorPoint({0.f, 0.f});
    m_listLayer->setPosition({director->getScreenRight() / 5.25f + 5.f, director->getScreenTop() / 6.f - 15.f});
    m_listLayer->setID("m_listLayer");
    this->addChild(m_listLayer);

    m_scrollLayer = ScrollLayer::create(winSize / 1.5f);
    m_scrollLayer->m_contentLayer->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
            ->setAutoGrowAxis(this->getContentSize().height)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(true)
            ->setGap(37.5f)
            
    );
    m_scrollLayer->setID("achievement-scroll-layer");
    m_scrollLayer->m_scrollLimitTop = 70.f;
    m_scrollLayer->m_scrollLimitBottom = -15.f;
    m_scrollLayer->setPosition({17.5f, -10.f});
    m_scrollLayer->setContentHeight(m_scrollLayer->getContentHeight() + 70.f);
    m_scrollLayer->setZOrder(1);
    m_scrollLayer->setScale(0.9f);
    m_scrollLayer->setAnchorPoint({0.5f, 0.0f});
    m_listLayer->addChild(m_scrollLayer);

    m_scrollLayer->scrollToTop();

    auto topBar = m_listLayer->getChildByID("top-border");

    buttonMenu = CCMenu::create();
    buttonMenu->setID("button-menu");
    buttonMenu->setScaledContentSize(topBar->getScaledContentSize());
    buttonMenu->setPosition({topBar->getPositionX(), topBar->getPositionY() + 25.f});
    buttonMenu->setZOrder(0);
    buttonMenu->setLayout(RowLayout::create()->setAutoScale(false)->setGap(1.f));
    m_listLayer->addChild(buttonMenu);

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

    this->getAllAchievements();
    this->loadPage(this->mainLevelsKeys);
    this->m_selectedTabs.front()->setVisible(true);

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
                auto achievementCell = BetterAchievementCell::create(achievement);
                achievementCell->m_bg->setContentWidth(m_scrollLayer->getScaledContentWidth());

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

void BetterAchievementLayer::onClose(CCObject*) {
    auto mainMenu = MenuLayer::scene(false);
    CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, mainMenu));
}

void BetterAchievementLayer::keyBackClicked() {
    this->onClose(new CCObject());
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