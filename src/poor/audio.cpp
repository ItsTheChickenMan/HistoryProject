// audio
#include <poor/audio.h>

#include <cstring>
#include <cstdlib>

// uses custom-made asset management because the entity manager doesn't really work for this
static std::vector<SoundAsset> soundAssets;
static SoundAsset nullSound;
static Sound nullSo;

// entity managers for active sounds and music
static EntityManager<Sound> activeSounds;

static sf::Music activeMusic; //due to the fact that sf::Music is non-copyable, we can only have one music track playing at a time

// load a new sound and store it by name and id
SoundAsset *audioLoadSound(char* path, char* name, int id){
	SoundAsset asset;
	
	if(!asset.buffer.loadFromFile(path)){
		return &nullSound;
	}
	
	asset.path = path;
	asset.name = name;
	asset.id = id;
	
	soundAssets.insert(soundAssets.begin(), asset);
	
	return &(soundAssets[0]);
}

// get a sound asset by id
SoundAsset *getSoundAsset(int id){
	for(std::vector<SoundAsset>::iterator i = soundAssets.begin(); i != soundAssets.end(); i++){
		if(id == (*i).id){
			return &(*i);
		}
	}
	
	return &nullSound;
}

// get a sound asset by name
SoundAsset *getSoundAsset(char* name){
	for(std::vector<SoundAsset>::iterator i = soundAssets.begin(); i != soundAssets.end(); i++){
		if( strcmp((*i).name, name) == 0){
			return &(*i);
		}
	}
	
	return &nullSound;
}

// destroy a sound asset by name (NOTE: do not use while a sound is playing using this asset!) (TODO: protection against the possibility outlined in the previous note?)
void terminateSoundAsset(char* name){
	for(std::vector<SoundAsset>::iterator i = soundAssets.begin(); i != soundAssets.end(); i++){
		if( strcmp((*i).name, name) == 0){
			soundAssets.erase(i);
		}
	}
}

// destroy a sound asset by id (NOTE: do not use while a sound is playing using this asset!)
void terminateSoundAsset(int id){
	for(std::vector<SoundAsset>::iterator i = soundAssets.begin(); i != soundAssets.end(); i++){
		if(id == (*i).id){
			soundAssets.erase(i);
		}
	}
	
}

// destoy a sound asset (NOTE: do not use while a sound is playing using this asset!)
void terminateSoundAsset(SoundAsset *s){
	for(std::vector<SoundAsset>::iterator i = soundAssets.begin(); i != soundAssets.end(); i++){
		if(s->id == (*i).id){
			soundAssets.erase(i);
		}
	}
}

// destroy all sound assets
void terminateAllSoundAssets(){
	soundAssets.clear();
}

// play sound from SoundAsset and return the active sound's id
Sound *audioPlaySound(SoundAsset *asset){
	Sound sound;
	
	Sound *pushed = pushEntity<Sound>( &activeSounds, sound, -1);
	
	pushed->soundInstance.setBuffer(asset->buffer);
	
	pushed->soundInstance.play();
	
	return pushed;
}

void terminateInactiveSounds(){
	for(std::vector<Sound>::iterator i = activeSounds.entities.begin(); i != activeSounds.entities.end(); i++){
		if( (*i).soundInstance.getStatus() == sf::SoundSource::Stopped){
			pullEntity<Sound>( &activeSounds, (*i).id);
			
			// for some technical reasons, we can only pull one at a time, so just break here
			break;
		}
	}
}

// stops all sounds from playing
void audioStopAllSounds(){
	for(std::vector<Sound>::iterator i = activeSounds.entities.begin(); i != activeSounds.entities.end(); i++){
		(*i).soundInstance.stop();
	}
}

sf::Music *getActiveMusic(){
	return &activeMusic;
}

EntityManager<Sound> *getActiveSounds(){
	return &activeSounds;
}

// play music directly from source
int audioPlayMusic(char* source){
	if(!activeMusic.openFromFile(source)){
		return -1;
	}
	
	activeMusic.play();
	
	return 0;
}

void audioPauseMusic(){
	activeMusic.pause();
}

void audioStopMusic(){
	activeMusic.stop();
}