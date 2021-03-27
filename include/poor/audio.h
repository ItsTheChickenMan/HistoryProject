// audio header

#ifndef AUDIO_H
#define AUDIO_H

#define SFML_STATIC
#include <SFML/Audio.hpp>
#include <poor/entity.h>

struct SoundAsset {
	sf::SoundBuffer buffer;
	
	char* path;
	char* name;
	int id;
};

// entity structs for sound a music
struct Sound {
	sf::Sound soundInstance;
	
	int id;
};

SoundAsset *audioLoadSound(char* path, char* name, int id);

SoundAsset *getSoundAsset(int id);
SoundAsset *getSoundAsset(char* name);

void terminateSoundAsset(char* name);
void terminateSoundAsset(int id);
void terminateSoundAsset(SoundAsset *s);
void terminateAllSoundAssets();

Sound *audioPlaySound(SoundAsset *asset);
void terminateInactiveSounds();
void audioStopAllSounds();

sf::Music *getActiveMusic();
EntityManager<Sound> *getActiveSounds();
int audioPlayMusic(char* source);
void audioPauseMusic();
void audioStopMusic();

#endif