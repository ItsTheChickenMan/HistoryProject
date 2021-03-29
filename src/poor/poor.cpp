// Poor engine front end

#include <tgmath.h>
#include <stdio.h>
#define SFML_STATIC
//#include <SFML/Audio.hpp>

#include <poor/poor.h>
#include <poor/window_callbacks.h>
#include <poor/math.h>
#include <poor/entity.h>
#include <poor/camera.h>

// GLOBALS //

// request handling
static std::vector<NetworkingUpdateRequest> activeRequests; //active requests, get using networkingGetUpdateRequests(&activeRequests)

static int self; // id of the client's physicsEntity (don't render data for this entity, change camera position for this entity, etc.)

// clientside values
static Map currentMap;
static Map collisionMap;

static double delta; // time between last frame and current frame
static std::chrono::high_resolution_clock::time_point lastFrame; // the time at last frame
static double intended = 1000.0/60.0; // milliseconds between frames intended

// settings
static bool useServersideMovement = true; // when true, uses server movement, when false, movement is clientside (defaults to true)

void poorSetServersideMovement(bool state){
	useServersideMovement = state;
}

void poorSetCurrentMap(char* map, char* cmap){
	currentMap = loadMap(map);
	collisionMap = loadMap(cmap);
}

void poorInit(){
	setup_rng();
	graphicsInit();
	
	set_callbacks();

	// networking set up	
	networkingInit();
	
	networkingConnect();
	
	// load map
	double thing[] = {-1};
	NetworkingUpdateRequest mapRequest = networkingCreateUpdateRequest(NETWORKING_MAP, thing, 1);
	networkingSendUpdateRequest(&mapRequest);
}

// start = when the update was called
void poorUpdate(std::chrono::high_resolution_clock::time_point start){
	// calculate delta values
	delta = std::chrono::duration_cast<std::chrono::duration<double>>(start - lastFrame).count();
	lastFrame = start;
	
	//printf("framerate: %f\n", ((intended/delta)*60)/1000);
	
	// note: is there any reason to do graphics things before networking things?
	activeRequests.clear(); //clear update requests so that we don't parse anything twice
	int numRequests = networkingGetUpdateRequests(&activeRequests); //retrieve requests from the server
	poorParseActiveUpdateRequests(numRequests);
	
	// check if we need to quiet down music
	if( getActiveSounds()->entities.size() > 0){
		getActiveMusic()->setVolume(5.0f);
	} else {
		getActiveMusic()->setVolume(50.0f);
	}
	
	// update the environment if there's no serverside movement
	if(!useServersideMovement){
		//handleAllCollisions(poorMapToPhysics(&currentMap)); //handle all collisions, map and player
		std::vector<PhysicsEntity> mapObjects = poorMapToPhysics(&collisionMap);
		handleAllMapCollisions(&mapObjects);
		updateEnvironment(delta/intended);
	}
	
	terminateInactiveSounds();
	
	poorSetCamera();
	tempParseInput();
	
	poorUpdateRenderData();
	graphicsUpdate();
}

void poorEnd(){
	networkingDisconnect(); //leave the server
	graphicsEnd(); // close up graphics
	audioStopMusic(); // stop da music
	audioStopAllSounds(); // stop da sound
	terminateAllSoundAssets(); // destroy all sound assets
	terminateAssets(); // destroy textures
}

void poorHandleRequest(NetworkingUpdateRequest *request){
	PhysicsEntity *entity = getEntityById<PhysicsEntity>(getManager(), (int)request->data[0]);
	
	// because we can (probably) trust the server to not spam us with entities, create a new entity if it wasn't found
	if(entity == &(getManager()->nullEntity) && (int)request->data[0] != -1){
		entity = pushEntity<PhysicsEntity>(getManager(), createPhysicsEntity(), (int)request->data[0]);
		
		// assign clientside values
		if(!useServersideMovement){
			entity->position = createVector3(27.5, 1.55, 0);
			entity->boundingBox = createVector3(1, 3, 1);
			entity->boundingCircle = calculateBoundingCircle(entity->boundingBox);
			entity->entityType = PHYSICS_ENTITY_DYNAMIC;
			entity->friction = 0.15;
		}
	
		// don't push a render_data if it's an id request because we don't want to render ourselves
		//if(request->update_type != NETWORKING_ID) // kind of a quick-fix, but because we don't need other entities for this project just don't render entities if we're not using serverside movement
			//pushEntity<Render_Data>(getRenderManager(), createRenderData(getAssetByName("default")), entity->id);
	}
	
	// TODO: same thing with server, this is fine for testing, but find a better way to check how to react to request
	Vector3 data = createVector3(request->data[1], request->data[2], request->data[3]);
	
	switch(request->update_type){
		case NETWORKING_POSITION:
			entity->position = data;
			break;
		case NETWORKING_ROTATION:
			entity->rotation = data;
			break;
		case NETWORKING_VELOCITY:
			entity->velocity = data;
			break;
		case NETWORKING_FORCE:
			forcePhysicsEntity(entity, data);
			break;
		case NETWORKING_ID:
			self = entity->id;
			break;
		case NETWORKING_ID_PULL:
			pullEntity<Render_Data>( getRenderManager(), (int)request->data[0] );
			pullEntity<PhysicsEntity>( getManager(), (int)request->data[0] );
			break;
		case NETWORKING_SERVER_NOTICE:
			networkingParseServerNotice( (int)request->data[0] );
			break;
		case NETWORKING_MAP:
			poorRenderMap( currentMap );
			break;
			
		// clientside request handlers
		case NETWORKING_CONNECT:
			entity->id = generateEntityId( getManager() );
			self = entity->id;
			break;
		default:
			break;
	}
}

void poorParseActiveUpdateRequests(int numRequests){
	for(int i = 0; i < numRequests; i++){
		poorHandleRequest( &(activeRequests[i]) );
	}
}

// NOTE: let ethan know about this function, just to update it in case the functions change
void poorUpdateRenderData(){
	for(std::vector<Render_Data>::iterator i = getRenderManager()->entities.begin(); i < getRenderManager()->entities.end(); i++){
		if( (*i).id == self || &(*i) == &(getRenderManager()->nullEntity)) continue; //we don't want to update data that doesn't exist (which can happen occasionally, if we're trying to update ourselves or a pushed entity)
		
		PhysicsEntity *ent = getEntityById<PhysicsEntity>( getManager(), (*i).id );
		
		if(ent == &(getManager()->nullEntity) ) continue; //on the off chance that there's no entity, continue so we don't crash
		
		update_render_data( &(*i), (Transform){toGlmVec3( ent->position ), toGlmVec3( ent->rotation ), toGlmVec3( createVector3(1.0, 1.0, 1.0) )}); //TODO: a bit better way to pass transform?
	}
}

// sets the camera's position to our own position
void poorSetCamera(){
	PhysicsEntity *selfEntity = poorGetSelfEntity();
	
	if( selfEntity == getNullEntity() ){
		return;
	}
	
	//fetch camera
	extern Camera camera;
	
	camera.position = toGlmVec3( selfEntity->position );
}

// get the PhysicsEntity of ourselves from self
PhysicsEntity *poorGetSelfEntity(){
	return getEntityById<PhysicsEntity>(getManager(), self);
}

// push render data objects for map
// TODO: this only works for static maps, and when the server doesn't change the map while it's running.  fix it
void poorRenderMap(Map map){
	Transform transform; // temporary transform to store the location and size 
	
	for(std::vector<MapObject>::iterator i = map.objects.begin(); i != map.objects.end(); i++){
		Vector3 rotation = createVector3( toDegrees((*i).rotation.x), toDegrees((*i).rotation.y), toDegrees((*i).rotation.z) );
		
		transform.position = toGlmVec3( (*i).position );
		transform.rotation = toGlmVec3( rotation );
		transform.scale = toGlmVec3( (*i).boundingBox );
		
		update_render_data( pushEntity<Render_Data>(getRenderManager(), createRenderData( getAssetById((*i).texture) ), -1), transform);
	}
		
	// push point lights
	for(std::vector<MapObject>::iterator i = map.lights.begin(); i != map.lights.end(); i++){
		pushLight(toGlmVec3((*i).position));
	}
	
	/*for(std::vector<MapObject>::iterator i = map.pointLights.begin(); i != map.pointLights.end(); i++){
		pushPointLight(toGlmVec3((*i).position), toGlmVec3(createVector3(.2f, .2f, .2f)), toGlmVec3((*i).boundingBox), toGlmVec3(createVector3(0, 0, 0)), (*i).rotation.x, (*i).rotation.y, (*i).rotation.z);
	}
	
	for(std::vector<MapObject>::iterator i = map.spotlights.begin(); i != map.spotlights.end(); i++){
		pushSpotlight(toGlmVec3((*i).position), toGlmVec3((*i).boundingBox), (*i).directionalVector.x, (*i).directionalVector.y, toGlmVec3(createVector3(.2f, .2f, .2f)), toGlmVec3(createVector3(.9f, .9f, .9f)), toGlmVec3(createVector3(0, 0, 0)), (*i).rotation.x, (*i).rotation.y, (*i).rotation.z);
	}*/
}

std::vector<PhysicsEntity> poorMapToPhysics(Map *map){
	std::vector<PhysicsEntity> ents;
	
	for(std::vector<MapObject>::iterator i = map->objects.begin(); i != map->objects.end(); i++){
		PhysicsEntity ent = createPhysicsEntity((*i).position, (*i).rotation, (*i).directionalVector, (*i).boundingBox, 0, PHYSICS_ENTITY_STATIC);
		
		ents.push_back(ent);
	}
	
	return ents;
}

std::chrono::high_resolution_clock::time_point poorGetTime(){
	return std::chrono::high_resolution_clock::now();
}

double poorGetDelta(){
	return delta;
}

// log some text (newlines after logs)
int poorLog(char* log){
	printf("Poor Engine Log: %s\n", log);
	
	return 0;
}

// log a networking error
int poorError(char* error, int code){
	printf("Poor Engine Error: %s\n", error);
	
	return code;
}

// TEMPORARY FUNCTIONS

// this function is only temporary because I'm not sure if this is final
// send an update request, but append our own id
void poorSendUpdateRequestClientside(NetworkingUpdateRequest *request){
	double newData[8];
	
	newData[0] = (double)self;
	
	for(int i = 1; i < request->data_length+1; i++){
		newData[i] = request->data[i-1];
	}
	
	NetworkingUpdateRequest clientRequest = networkingCreateUpdateRequest(request->update_type, newData, request->data_length+1);
	networkingSendUpdateRequest(&clientRequest);
}

// NOTE: this function is only temporary because it depends on other temporary functions, but with some changes this could be permanent
// (leaving this temp function in for the reason above)
void tempParseInput(){
	extern Camera camera; //fetch the camera
	
	Vector3 position = createBlankVector3();
	
	double yaw = toRadians((double)camera.yaw);
	
	double speed = 0.016*((delta/intended)*1000);
	
	if(	getKeyPressed(81) ){
		speed = 0.032*((delta/intended)*1000);
	}
	
	if ( getKeyPressed(87) ) { //W
			position.x += cos(yaw)*speed;
			position.z += sin(yaw)*speed;
	}
	if ( getKeyPressed(83) ){ //S
			position.x -= cos(yaw)*speed;
			position.z -= sin(yaw)*speed;
	}
	if ( getKeyPressed(65) ){ //A
			position.x += sin(yaw)*speed;
			position.z -= cos(yaw)*speed;
	}
	if ( getKeyPressed(68) ){ //D
			position.x -= sin(yaw)*speed;
			position.z += cos(yaw)*speed;
	}
	if ( getKeyPressed(32) ){ //SPACE
			//poorGetSelfEntity()->position.y += 0.01; // move up just enough to get us off of the floor
			position.y += speed;
	} else if( getKeyPressed(340) ){
			position.y -= speed;
	}
	
	// only send request if we have a request worth sending
	if( !isVector3Empty( &position ) ){
		// TODO: figure out a way to simplify converting vector3 to double
		double data[3];
		
		toArrayVector3(position, data);
		NetworkingUpdateRequest request = networkingCreateUpdateRequest(NETWORKING_FORCE, data, 3);
		
		if(useServersideMovement){
			networkingSendUpdateRequest(&request);
		} else {
			poorSendUpdateRequestClientside(&request);
		}
	}
}