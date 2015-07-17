#ifndef _AUDIO_H_
#define _AUDIO_H_

class Audio{
public:
	static Audio* getInstance();

	void playPop();
	void prepare();
	void playCombo(int size);

	void playWelcome();
	void playSplatter();
	void playStart();
	void playThrow();
	void playBoom();
	Audio();
private:
	static Audio* m_instance;
};
#endif