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
    m_scrollLayer->m_scrollLimitBottom = 30.f;
    m_scrollLayer->setPosition({director->getScreenRight() / 2 - 158.f, director->getScreenTop() / 2 - 139.f});
    m_scrollLayer->setContentHeight(m_scrollLayer->getContentHeight() + 70.f);
    m_scrollLayer->setZOrder(1);
    m_scrollLayer->setScale(0.9f);
    this->addChild(m_scrollLayer);

    auto scrollLayerBG = CCLayerColor::create({ 168, 85, 44, 255 });
    scrollLayerBG->setID("scrollLayerBG");
    scrollLayerBG->setContentSize({369.f, m_scrollLayer->getContentHeight() + 5.f});
    scrollLayerBG->ignoreAnchorPointForPosition(false);
    scrollLayerBG->setPosition({director->getScreenRight() / 2 + 5.f, director->getScreenTop() / 2 + 5.f});
    scrollLayerBG->setScale(0.9f);
    this->addChild(scrollLayerBG);

    auto topBar = CCSprite::createWithSpriteFrameName("GJ_table_top_001.png");
    topBar->setID("top-bar");
    topBar->setPosition({scrollLayerBG->getPositionX(), scrollLayerBG->getContentSize().height - 8.f});
    topBar->setScale(0.9f);
    topBar->setZOrder(1);
    this->addChild(topBar);

    auto bottomBar = CCSprite::createWithSpriteFrameName("GJ_table_bottom_001.png");
    bottomBar->setID("bottom-bar");
    bottomBar->setPosition({scrollLayerBG->getPositionX(), scrollLayerBG->getPositionY() - 130.f});
    bottomBar->setScale(0.9f);
    bottomBar->setZOrder(1);
    this->addChild(bottomBar);

    auto leftBar = CCSprite::createWithSpriteFrameName("GJ_table_side_001.png");
    leftBar->setID("left-bar");
    leftBar->setPosition({scrollLayerBG->getPositionX() - 165.f, scrollLayerBG->getPositionY()});
    leftBar->setScaleY(3.725f);
    this->addChild(leftBar);

    auto rightBar = CCSprite::createWithSpriteFrameName("GJ_table_side_001.png");
    rightBar->setID("right-bar");
    rightBar->setPosition({scrollLayerBG->getContentSize().width + 85.f, scrollLayerBG->getPositionY()});
    rightBar->setScaleY(3.725f);
    rightBar->setFlipX(true);
    this->addChild(rightBar);

    m_scrollLayer->scrollToTop();

    buttonMenu = CCMenu::create();
    buttonMenu->setID("button-menu");
    buttonMenu->setScaledContentSize(topBar->getScaledContentSize());
    buttonMenu->setPosition({topBar->getPositionX(), topBar->getPositionY() + 23.f});
    buttonMenu->setZOrder(0);
    buttonMenu->setLayout(RowLayout::create()->setAutoScale(false)->setGap(1.f));
    this->addChild(buttonMenu);

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

    m_categoryTitle = CCLabelBMFont::create("Main Levels", "bigFont.fnt");
    m_categoryTitle->setID("category-title");
    m_categoryTitle->setScale(0.65f);
    m_categoryTitle->setPosition({topBar->getPositionX(), topBar->getPositionY() + 3.f});
    m_categoryTitle->setZOrder(1);
    this->addChild(m_categoryTitle);

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

std::vector<std::string> BAL_split(std::string s, char del) {
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string token;
    while(std::getline(ss, token, del)) {
        tokens.push_back(token);
    }
    return tokens;
}

void BetterAchievementLayer::loadPage(std::vector<std::string> keys) {
    m_scrollLayer->m_contentLayer->removeAllChildren();

    for (auto achievement : this->achievements) {
        auto achievementIdentifier = BAL_split(achievement->identifier, '.')[2];
        log::info("{}", achievementIdentifier);
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