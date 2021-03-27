// main entry point
// history project, made using a heavily modified version of the poor engine
// the code is messy because I was in a rush

#include <poor/poor.h>
#include <cstring>
#include <chrono>

int main(int argc, char *argv[]){
	// for speed, I'm defining painting locations here, but it would be better to have this built into the map
	Vector3 paintingSpots[] = {createVector3(15, 2.5, 5), createVector3(15, 2.5, -5), createVector3(1.66667, 2.5, 5), createVector3(1.66667, 2.5, -5), createVector3(-11.66667,2.5,4.97), createVector3(11.66667,2.5,4.97), createVector3(-25,2.5,4.97), createVector3(-29.97,2.5,0), createVector3(-29.97,2.5,-13.33333), createVector3(-20.03,2.5,-13.33333), createVector3(-29.97,2.5,-26.66667), createVector3(-20.03,2.5,-26.66667), createVector3(-29.97,2.5,-40), createVector3(-25,2.5,-44.97), createVector3(-11.66667,2.5,-35.03), createVector3(-11.66667,2.5,-44.97), createVector3(1.66667,2.5,-35.03), createVector3(1.66667,2.5,-44.97), createVector3(15,2.5,-35.03), createVector3(15,2.5,-44.97)};
	char* lines[] = {"longtelegramtalk", "longtelegramtalk", "ahem", "ahem", "ahem", "ahem", "ahem", "ahem", "ahem", "ahem", "ahem", "ahem", "ahem", "ahem", "ahem", "ahem", "ahem", "ahem", "ahem", "ahem"};
	
	networkingSetSendPort(22620);
	networkingSetListenPort(22620); // set these so we can send requests to ourselves
	
	poorSetServersideMovement(false);
	
	printf("loading map...");
	poorSetCurrentMap("./res/maps/museum.pep", "./res/maps/museum.pem"); // first is map, second is cmap
	printf("loaded\n");
	
	// load up sounds (in soundPaths, index 0 is path, 1 is name
	const char* soundPaths[] = {"./res/sounds/ahem.ogg", "ahem", "./res/sounds/leftfootstep.ogg", "leftfootstep", "./res/sounds/rightfootstep.ogg", "rightfootstep", "./res/sounds/longtelegramtalk.ogg", "longtelegramtalk", "./res/sounds/welcometalk.ogg", "welcometalk", "./res/sounds/berlinwalltalk.ogg", "berlinwalltalk"};
	
	printf("loading sounds...\n");
	// the id of the asset is based on the order in which it is defined in soundPaths
	for(int i = 0; i < sizeof(soundPaths)/sizeof(char*); i += 2){
		printf("loading %s...", soundPaths[i]);
		audioLoadSound((char*)soundPaths[i], (char*)soundPaths[i+1], i/2);
		printf("loaded\n");
	}
	
	// load up textures (TODO: wrap this into a backend, possibly make it built into map)
	const char* texturePaths[] = {"./res/textures/greyfloor.png", "greyfloor", "./res/textures/secretroomtexture.png", "secretroom", "./res/textures/MuseumDoor.png", "door", "./res/textures/framedlongtelegram.png", "framedlongtelegram", "./res/textures/berlinwall.png", "berlinwall", "./res/textures/default.png", "default", "./res/textures/floor.png", "floor"};
	
	printf("loading textures...\n");
	// the id of the asset is based on the order in which it is defined in soundPaths
	for(int i = 0; i < sizeof(texturePaths)/sizeof(char*); i += 2){
		printf("loading %s...", texturePaths[i]);
		loadAsset((char*)texturePaths[i], (char*)texturePaths[i+1], i/2);
		printf("loaded\n");
	}
	
	printf("textures loaded\n");
	
	// initialize the poor engine
	poorInit();
	
	audioPlayMusic("./res/sounds/arabesques.ogg");
	
	getActiveMusic()->setLoop(true);
	getActiveMusic()->setVolume(50.0f);
	
	audioPlaySound(getSoundAsset("welcometalk"));
	
	long frame = 0;
	while(!shouldCloseWindow()){
		// framerate
		std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
		
		// get self
		PhysicsEntity *self = poorGetSelfEntity();
		
		if(frame % 100 == 0){
			printVector3(self->position, true);
		}
		
		// see if we're looking at a painting
		for(int i = 0; i < sizeof(paintingSpots)/sizeof(Vector3); i++){
			// do distance calculations first to avoid having to do the calculations below if possible
			Vector3 distanceVector = subtractVector3(self->position, paintingSpots[i]);
			if(getMagVector3(distanceVector) > 3.9) continue;
			
			Vector3 direction = getDirection();
			
			double angle = dotVector3(normalizeVector3(distanceVector), direction);
			
			if(angle < -0.6 && getKeyPressed(340) && getActiveSounds()->entities.size() < 1){
				audioPlaySound(getSoundAsset(lines[i]));
			}
		}
		
		// stop all sounds for ctrl
		if( getKeyPressed(341) || getKeyPressed(345) ){
			audioStopAllSounds();
		}
		
		/*for(std::vector<PhysicsEntity>::iterator i = getManager()->entities.begin(); i != getManager()->entities.end(); i++){
			forcePhysicsEntity(&(*i), createVector3(0, -0.01, 0));
		}*/
		
		// update engine
		poorUpdate(currentTime);
		
		frame++;
	}
	
	poorEnd();
	
	return 0;
}