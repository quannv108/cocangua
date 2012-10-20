#include "AnimalUnit.h"
#include "Config.h"
#include "ClassicGameLayer.h"

/*
Chua nhung thong tin ve 1 con co
*/

AnimalUnit::AnimalUnit(int offset, CCNode* _p,CCPoint _initLocation, MapLocation *_m)
{
	this->map = _m;
	this->initLocation = _initLocation;//vi tri trong chuong
	this->location = _initLocation;
	this->path_went = -1;
	//this->isExploring = false;
	parent = _p;

	//setup danceAction
	const char* imageLink="";
	const char* plistLink="";
	char* temp="";
	switch(offset)
	{
	case 0:
		{
			imageLink=Config::animal0_init_image;
			plistLink=Config::animal0_init_plist;
			temp="pig%d copy.png";
			break;
		}
	case 1:
		{
			imageLink=Config::animal1_init_image;
			plistLink=Config::animal1_init_plist;
			temp="pig%d copy.png";//TODO
			break;
		}
	case 2:
		{
			imageLink=Config::animal2_init_image;
			plistLink=Config::animal2_init_plist;
			temp="pig%d copy.png";//TODO
			break;
		}
	case 3:
		{
			imageLink=Config::animal3_init_image;
			plistLink=Config::animal3_init_plist;
			temp="pig%d copy.png";//TODO
			break;
		}
	default: 
		break;
	}
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plistLink);
	CCSpriteBatchNode *danceSpriteBatchNode =  CCSpriteBatchNode::create(imageLink);
	char fn[128];
	CCAnimation* danceAnim =CCAnimation::create();
	
	for (int i = 1; i <= 2; i++) 
	{
		sprintf(fn, temp, i);
		CCSpriteFrame* pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fn);
		danceAnim->addSpriteFrame(pFrame);
	}
	sprintf(fn,temp,1);
    danceAnim->setDelayPerUnit(0.5f);
	this->sprite = CCSprite::createWithSpriteFrameName(fn);
	this->sprite->retain();

	danceAction = CCRepeatForever::create(CCAnimate::create(danceAnim));
	danceAction->setOriginalTarget(sprite);
	danceAction->retain();

	//create menu
	button = CCMenuItemSprite::create(sprite,sprite,sprite,parent,menu_selector(ClassicGameLayer::selectCallback));
	button->setUserData(this);
	button->setPosition(location);
	button->retain();

	CCMenu* pMenu = CCMenu::createWithItem(button);
	pMenu->setPosition(CCPointZero);
	parent->addChild(pMenu,1);

	//explore Action 
	//load effect
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(Config::disappearEffect_plist);
	CCSpriteBatchNode *effectSpriteSheet =  CCSpriteBatchNode::create(Config::disappearEffect_texture);
//	char fn[128];
	CCAnimation* effectAnimation =CCAnimation::create();
	for (int i = 0; i <= 7; i++) 
	{
		sprintf(fn, "move-effect-%d.png", i);
		CCSpriteFrame* pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fn);
		effectAnimation->addSpriteFrame(pFrame);
	}
    effectAnimation->setDelayPerUnit(0.1f);
	exploreAction = CCAnimate::create(effectAnimation);
	exploreAction->setOriginalTarget(sprite);
	exploreAction->retain();
	dance();
}
CCPoint AnimalUnit::getLocation(){
	return this->location;
}

AnimalUnit::~AnimalUnit(void)
{
	this->button->autorelease();
	this->sprite->autorelease();
	this->danceAction->autorelease();
}
void AnimalUnit::born()
{//tu chuong ra duong
	this->location = this->initLocation;
}
void AnimalUnit::go(int step)
{//di them dc step buoc
	//TODO 
	this->sprite->stopAllActions();
	//explore();
	CCPoint next = map->getNextPoint(Config::WAYMAP,location,step);
	CCFiniteTimeAction *moveAction = CCMoveTo::create(5.0f,next);

	this->button->runAction(moveAction);
	dance();
	this->location = next;
}
void AnimalUnit::finish(int x)
{//den dich buoc thu x
	//TODO
}
void AnimalUnit::die()
{//chet, ve lai chuong
	//TODO
}
void AnimalUnit::dance()
{
	this->sprite->runAction(danceAction);
}
void AnimalUnit::explore()
{
	this->sprite->runAction(exploreAction);
}