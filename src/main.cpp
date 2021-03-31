// main entry point
// history project, made using a heavily modified version of the poor engine
// the code is messy because I was in a rush

#include <poor/poor.h>
#include <cstring>
#include <chrono>

int main(int argc, char *argv[]){
	// for speed, I'm defining painting locations here, but it would be better to have this built into the map
	Vector3 paintingSpots[] = {createVector3(15, 2.5, 5), createVector3(15, 2.5, -5), createVector3(1.66667, 2.5, 5), createVector3(1.66667, 2.5, -5), createVector3(-11.66667,2.5,-4.97), createVector3(-11.66667,2.5,4.97), createVector3(-25,2.5,4.97), createVector3(-29.97,2.5,0), createVector3(-29.97,2.5,-13.33333), createVector3(-20.03,2.5,-13.33333), createVector3(-29.97,2.5,-26.66667), createVector3(-20.03,2.5,-26.66667), createVector3(-29.97,2.5,-40), createVector3(-25,2.5,-44.97), createVector3(-11.66667,2.5,-35.03), createVector3(-11.66667,2.5,-44.97), createVector3(1.66667,2.5,-35.03), createVector3(1.66667,2.5,-44.97), createVector3(15,2.5,-35.03)};
	char* lines[] = {"longtelegramtalk", "trumandoctrinetalk", "marshallplantalk", "communistchinatalk", "koreanwartalk", "secondredscaretalk", "mccarthytalk", "consumertalk", "babyboomtalk", "migrationtalk", "eisenhowertalk", "checkerstalk", "dullestalk", "domesticpoliciestalk", "election1960talk", "newfrontiertalk", "berlinwalltalk", "assassinationtalk", "greatsocietytalk"};
	bool playedLines[19];
	
	for(int i = 0; i < 19; i++){
		playedLines[i] = false;
	}
	
	networkingSetSendPort(22620);
	networkingSetListenPort(22620); // set these so we can send requests to ourselves
	
	// initialize the poor engine
	printf("initializing the poor engine...\n");
	poorInit();
	
	poorSetServersideMovement(false);
	
	printf("loading map...");
	poorSetCurrentMap("./res/maps/museum.pep", "./res/maps/museum.pem"); // first is map, second is cmap
	printf("loaded\n");
	
	// load up sounds (in paths, index 0 is path, 1 is name)
	const char* soundPaths[] = {"./res/sounds/exitmessage.ogg", "exitmessage", "./res/sounds/babyboomtalk.ogg", "babyboomtalk", "./res/sounds/migrationtalk.ogg", "migrationtalk", "./res/sounds/consumertalk.ogg", "consumertalk", "./res/sounds/greatsocietytalk.ogg", "greatsocietytalk", "./res/sounds/assassinationtalk.ogg", "assassinationtalk", "./res/sounds/berlinwalltalk.ogg", "berlinwalltalk", "./res/sounds/newfrontiertalk.ogg", "newfrontiertalk", "./res/sounds/election1960talk.ogg", "election1960talk", "./res/sounds/domesticpoliciestalk.ogg", "domesticpoliciestalk", "./res/sounds/dullestalk.ogg", "dullestalk", "./res/sounds/checkerstalk.ogg", "checkerstalk", "./res/sounds/eisenhowertalk.ogg", "eisenhowertalk", "./res/sounds/mccarthytalk.ogg", "mccarthytalk", "./res/sounds/secondredscaretalk.ogg", "secondredscaretalk", "./res/sounds/koreanwartalk.ogg", "koreanwartalk", "./res/sounds/communistchinatalk.ogg", "communistchinatalk", "./res/sounds/trumandoctrinetalk.ogg", "trumandoctrinetalk", "./res/sounds/marshallplantalk.ogg", "marshallplantalk", "./res/sounds/longtelegramtalk.ogg", "longtelegramtalk", "./res/sounds/welcomemessage.ogg", "welcomemessage"};
	
	printf("loading sounds...\n");
	// the id of the asset is based on the order in which it is defined in soundPaths
	for(int i = 0; i < sizeof(soundPaths)/sizeof(char*); i += 2){
		printf("loading %s...", soundPaths[i]);
		audioLoadSound((char*)soundPaths[i], (char*)soundPaths[i+1], i/2);
		printf("loaded\n");
	}
	
	// load up textures (TODO: wrap this into a backend, possibly make it built into map)
	const char* texturePaths[] = {"./res/textures/gibill.png", "gibill", "./res/textures/oswaldportrait.png", "oswaldportrait", "./res/textures/johnsonportrait.png", "johnsonportrait", "./res/textures/babyboomgraph.png", "babyboomgraph", "./res/textures/levitthouses.png", "levitthouses", "./res/textures/kennedyandjackie.png", "kandj", "./res/textures/museumdoor.png", "museumdoor", "./res/textures/televiseddebate.png", "televiseddebate", "./res/textures/highwayactmap.png", "highwayactmap", "./res/textures/dullesportrait.png", "dullesportrait", "./res/textures/checkersthedog.png", "checkersthedog", "./res/textures/ikecampaign.png", "ikecampaign", "./res/textures/mccarthyportrait.png", "mccarthyportrait", "./res/textures/redscarepropaganda.png", "redscarepropaganda", "./res/textures/koreanwarmap.png", "koreanwarmap", "./res/textures/chinesemedal.png", "chinesemedal", "./res/textures/marshallportrait.png", "marshall", "./res/textures/trumancartoon.png", "trumancartoon", "./res/textures/greyfloor.png", "greyfloor", "./res/textures/test.png", "secretroom", "./res/textures/MuseumDoor.png", "door", "./res/textures/framedlongtelegram.png", "framedlongtelegram", "./res/textures/berlinwall.png", "berlinwall", "./res/textures/default.png", "default", "./res/textures/floor.png", "floor"};
	 
	printf("loading textures...\n");
	// the id of the asset is based on the order in which it is defined in soundPaths
	for(int i = 0; i < sizeof(texturePaths)/sizeof(char*); i += 2){
		printf("loading %s...", texturePaths[i]);
		loadAsset((char*)texturePaths[i], (char*)texturePaths[i+1], i/2);
		printf("loaded\n");
	}
	
	printf("textures loaded\n");
	
	// play backgorund music
	audioPlayMusic("./res/sounds/arabesques.ogg");
	
	getActiveMusic()->setLoop(true);
	getActiveMusic()->setVolume(50.0f);
	
	// play opening talk sequence
	audioPlaySound(getSoundAsset("welcomemessage"));
	
	long frame = 0;
	int frameDelay = 1;
	while(!shouldCloseWindow()){
		bool printFrames = false;
		// framerate
		std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
		
		// get self
		PhysicsEntity *self = poorGetSelfEntity();
		
		// see if we're looking at a painting, also check if user has entered all exhibits
		bool seenEverything = true;
		for(int i = 0; i < sizeof(paintingSpots)/sizeof(Vector3); i++){
			if(!playedLines[i]){ 
				seenEverything = false;
			}
			
			// do distance calculations first to avoid having to do the calculations below if possible
			Vector3 distanceVector = subtractVector3(self->position, paintingSpots[i]);
			if(getMagVector3(distanceVector) > 3.9) continue;
			
			Vector3 direction = getDirection();
			
			double angle = dotVector3(normalizeVector3(distanceVector), direction);
			
			if(angle < -0.6 && getKeyPressed(340) && getActiveSounds()->entities.size() < 1){
				audioPlaySound(getSoundAsset(lines[i]));
				playedLines[i] = true;
			}
		}
		
		// check if user has viewed all exhibits, play exit message if they have (wait about a second before playing)
		if(seenEverything){
			if(frameDelay == 120){
				audioPlaySound(getSoundAsset("exitmessage"));
				frameDelay = 121;				
			} else {
				frameDelay++;
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
		poorUpdate(currentTime, printFrames);
		
		frame++;
	}
	
	poorEnd();
	
	return 0;
}