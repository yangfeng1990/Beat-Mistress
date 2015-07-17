#include "Audio.h"
#include "SimpleAudioEngine.h"

Audio* Audio::m_instance = nullptr;

Audio* Audio::getInstance(){
	if(m_instance == nullptr){
		m_instance = new Audio();
	}
	return m_instance;
}

Audio::Audio(){
	Audio::prepare();
}

void Audio::playWelcome(){
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/menu.mp3",true);
}

void Audio::playSplatter(){
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/splatter.mp3",false);
}

void Audio::playStart(){
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/start.mp3",false);
}

void Audio::playThrow(){
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/throw.mp3",false);
}

void Audio::playBoom(){
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/boom.mp3",false);
}

void Audio::playCombo(int size){
	if(size < 5)
		return;
	if(size >= 10){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Music/combo_3.ogg",false);
	}else if(size >= 7){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Music/combo_2.ogg",false);
	}else {
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Music/combo_1.ogg",false);
	}
}
void Audio::prepare(){
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Music/music.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Music/readygo.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Music/pop.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Music/combo_1.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Music/combo_2.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Music/combo_3.ogg");
}
