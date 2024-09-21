#include "BetterAchievementLayer.hpp"

bool BetterAchievementLayer::init() {
    if (!CCLayer::init()) {
        return false;
    }

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    this->setID("BetterAchievementLayer");

    CCSprite* backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    CCMenuItemSpriteExtra* backBtn = CCMenuItemSpriteExtra::create(backSpr, this, menu_selector(BetterAchievementLayer::onClose));
    backBtn->setID("back-button");

    CCMenu* backMenu = CCMenu::create();
    backMenu->setID("back-menu");
    backMenu->addChild(backBtn);
    this->addChild(backMenu, 1);

    backMenu->setPosition(ccp(director->getScreenLeft() + 25.f, director->getScreenTop() - 22.f));
    setKeyboardEnabled(true);
    setKeypadEnabled(true);

    auto bg = geode::createLayerBG();
    bg->setID("content-background");
    this->addChild(bg);

    m_scrollLayer = ScrollLayer::create(winSize / 1.5f);
    m_scrollLayer->m_contentLayer->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setAutoGrowAxis(this->getContentSize().height)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(true)
            ->setGap(37.5f)
            
    );
    m_scrollLayer->setID("achievement-scroll-layer");
    m_scrollLayer->m_scrollLimitBottom = 30.f;
    m_scrollLayer->setPosition({director->getScreenRight() / 2 - 145.f, director->getScreenTop() / 2 - 143.f});
    m_scrollLayer->setContentHeight(m_scrollLayer->getContentHeight() + 40.f);
    m_scrollLayer->setZOrder(1);
    m_scrollLayer->setScale(0.9f);
    this->addChild(m_scrollLayer);

    auto scrollLayerBG = CCLayerColor::create({ 168, 85, 44, 255 });
    scrollLayerBG->setID("scrollLayerBG");
    scrollLayerBG->setContentSize({m_scrollLayer->getContentWidth(), m_scrollLayer->getContentHeight() + 20.f});
    scrollLayerBG->ignoreAnchorPointForPosition(false);
    scrollLayerBG->setPosition({director->getScreenRight() / 2 + 5.f, director->getScreenTop() / 2 + 5.f});
    scrollLayerBG->setScale(0.9f);
    this->addChild(scrollLayerBG);

    auto topBar = CCSprite::createWithSpriteFrameName("GJ_table_top_001.png");
    topBar->setID("top-bar");
    topBar->setPosition({scrollLayerBG->getPositionX(), scrollLayerBG->getContentSize().height + 2.f});
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
    rightBar->setPosition({scrollLayerBG->getContentSize().width + 75.f, scrollLayerBG->getPositionY()});
    rightBar->setScaleY(3.725f);
    rightBar->setFlipX(true);
    this->addChild(rightBar);

    this->getAllAchievements();
    for (auto achievement : this->achievements) {
        if (achievement->identifier.find("level") != std::string::npos) {
            auto achievementCell = BetterAchievementCell::create(achievement);
            achievementCell->m_bg->setContentWidth(m_scrollLayer->getScaledContentWidth());

            m_scrollLayer->m_contentLayer->addChild(achievementCell);
            m_scrollLayer->m_contentLayer->updateLayout();
        }
    }

    m_scrollLayer->scrollToTop();

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

void BetterAchievementLayer::onClose(CCObject*) {
    auto mainMenu = MenuLayer::scene(false);
    CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, mainMenu));
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