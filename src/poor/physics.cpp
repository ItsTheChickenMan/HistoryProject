// physics stuff
// TODO: rename the functions to match the syntax of everything else

#include "poor/physics.h"

// GLOBALS //

static EntityManager<PhysicsEntity> manager;

// UTILS //
PhysicsEntity *getNullEntity(){
	return &(manager.nullEntity);
}

EntityManager<PhysicsEntity> *getManager(){
	return &manager;
}

double toRadians(double degrees){
	return degrees * (PI/180);
}

double toDegrees(double radians){
	return radians * (180/PI);
}
// VECTOR FUNCTIONS // 

// create a new Vector3
Vector3 createVector3(double x, double y, double z){
	Vector3 c;
	c.x = x;
	c.y = y;
	c.z = z;
	
	return c;
}

// create a vector 3 with x y and z all 0
Vector3 createBlankVector3(){
	return createVector3(0.0f, 0.0f, 0.0f);
}

// takes in a pointer to an array and assigns it vector values, and returns the pointer to it
double *toArrayVector3(Vector3 c, double *arr){
	arr[0] = c.x;
	arr[1] = c.y;
	arr[2] = c.z;
	
	return arr;
}

// creates a new vector from pointer array data (doesn't affect array data)
Vector3 fromArrayVector3(double vector_array[3]){
	return createVector3(vector_array[0], vector_array[1], vector_array[2]);
}

// gets magnitude of Vector3 v
double getMagVector3(Vector3 v){
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

// normalizes a vector (returns a new vector)
Vector3 normalizeVector3(Vector3 v){
	Vector3 c;
	double mag = getMagVector3(v);
	
	c.x = v.x / mag;
	c.y = v.y / mag;
	c.z = v.z / mag;
	
	return c;
}

// magnifies a vector (returns a new vector)
Vector3 magnifyVector3(Vector3 v, double amount){
	Vector3 c;
	
	c.x = v.x * amount;
	c.y = v.y * amount;
	c.z = v.z * amount;
	
	return c;
}

// multiply two vectors
Vector3 multiplyVector3(Vector3 a, Vector3 b){
	Vector3 c;
	
	c.x = a.x*b.x;
	c.y = a.y*b.y;
	c.z = a.z*b.z;
	
	return c;
}

// creates a new vector of a + b (doesn't affect a or b)
Vector3 addVector3(Vector3 a, Vector3 b){
	Vector3 c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	
	return c;
}

// creates a new vector of a - b (doesn't affect a or b)
Vector3 subtractVector3(Vector3 a, Vector3 b){
	Vector3 c;
	
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	
	return c;
}

// returns the dot product of a and b
double dotVector3(Vector3 a, Vector3 b){
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

// returns a new vector of the cross product of a x b (doesn't affect a or b)
Vector3 crossVector3(Vector3 a, Vector3 b){
	Vector3 c;
	
	c.x = a.y*b.z-a.z*b.y;
	c.y = a.z*b.x-a.x*b.z;
	c.z = a.x*b.y-a.y*b.x;
	
	return c;
}

// rotate vector3 globally (kinda) a certain amount (in radians) on a certain axis (can only rotate on x y or z, TODO: change that)
Vector3 rotateVector3(Vector3 *vec, double theta, int axis){
	Vector3 newVec;
	
	// TODO: this is realy unintuitive, fixit i moron
	switch(axis){
		case PHYSICS_X_AXIS:
			newVec = createVector3(vec->x, cos(theta)*vec->y - sin(theta)*vec->z, cos(theta)*vec->z + sin(theta)*vec->y);
			break;
		case PHYSICS_Y_AXIS:
			newVec = createVector3(cos(theta)*vec->x + sin(theta)*vec->z, vec->y, cos(theta)*vec->z + sin(theta)*(-1*vec->x));
			break;
		case PHYSICS_Z_AXIS:
			newVec = createVector3(cos(theta)*vec->x - sin(theta)*vec->y, cos(theta)*vec->y + sin(theta)*vec->x, vec->z);
			break;
		default:
			newVec = createBlankVector3();
			break;
	}
	
	*vec = newVec;
	
	return newVec;
}

glm::vec3 toGlmVec3(Vector3 v){
	return glm::vec3(v.x, v.y, v.z);
}

bool compareVector3(Vector3 *a, Vector3 *b){
	return (a->x == b->x) && (a->y == b->y) && (a->z == b->z);
}

bool isVector3Empty(Vector3 *a){
	return (a->x == 0) && (a->y == 0) && (a->z == 0);
}

// formats and prints a vector3 (newlines after if newline == true)
void printVector3(Vector3 v, bool newline){
	printf("x: %f; y: %f; z: %f", v.x, v.y, v.z);
	if(newline) printf("\n");
}

// ENTITY FUNCTIONS //

// create a blank PhysicsEntity
PhysicsEntity createPhysicsEntity(){
	PhysicsEntity c;
	
	c.position = createBlankVector3();
	c.rotation = createBlankVector3();
	c.velocity = createBlankVector3();
	c.boundingBox = createVector3(1, 1, 1);
	c.friction = 0;
	c.entityType = PHYSICS_ENTITY_DYNAMIC;
	c.boundingCircle = calculateBoundingCircle(c.boundingBox);
	
	return c;
}

// create a PhysicsEntity with position, rotation, and velocity
PhysicsEntity createPhysicsEntity(Vector3 position, Vector3 rotation, Vector3 velocity){
	PhysicsEntity c;
	
	c.position = position;
	c.rotation = rotation;
	c.velocity = velocity;
	c.boundingBox = createVector3(1, 1, 1);
	c.friction = 0;
	c.entityType = PHYSICS_ENTITY_DYNAMIC;
	c.boundingCircle = calculateBoundingCircle(c.boundingBox);
	
	return c;
}

// create a PhysicsEntity with position, rotation, and velocity, a bounding box, and some friction
PhysicsEntity createPhysicsEntity(Vector3 position, Vector3 rotation, Vector3 velocity, Vector3 boundingBox, double friction, int entityType){
	PhysicsEntity c;
	
	c.position = position;
	c.rotation = rotation;
	c.velocity = velocity;
	c.boundingBox = boundingBox;
	c.friction = friction;
	c.entityType = entityType;
	c.boundingCircle = calculateBoundingCircle(c.boundingBox);
	
	return c;
}

// returns diameter of sphere which encapsulates boundingBox fully
double calculateBoundingCircle(Vector3 boundingBox){
	return getMagVector3(boundingBox);
}

// gets the position of a corner of a cube.  height -1 = lower, height 1 = upper, width -1 = left, width 1 = right, depth -1 = front, depth 1 = back
Vector3 generateCorner(PhysicsEntity *e, int width, int height, int depth){
	// corners localized about the origin
	Vector3 localized = createVector3(width*(e->boundingBox.x/2), height*(e->boundingBox.y/2), depth*(e->boundingBox.z/2));
	
	// rotate to e's rotation
	rotateVector3(&localized, e->rotation.x, PHYSICS_X_AXIS);
	rotateVector3(&localized, e->rotation.y, PHYSICS_Y_AXIS);
	rotateVector3(&localized, e->rotation.z, PHYSICS_Z_AXIS);
	
	// localize to e's position
	localized = addVector3(e->position, localized);
	
	return localized;
}

// apply vector force to an entity's velocity, basically an easier way of doing entity.velocity = addVector3(entity.velocity, force)
void forcePhysicsEntity(PhysicsEntity *entity, Vector3 force){
	entity->velocity = addVector3(entity->velocity, force);
}

// update entity's position based on its velocity, easier way of doing entity.position = addVector3(entity.position, entity.velocity);
void updatePhysicsEntity(PhysicsEntity *entity, double deltaFactor){
	// amount of anti force to apply to entity for friction
	Vector3 frictionForce = createVector3(-entity->velocity.x*(entity->friction), -entity->velocity.y*(entity->friction), -entity->velocity.z*(entity->friction));
	
	// apply force
	forcePhysicsEntity(entity, frictionForce);
	
	entity->position = addVector3(entity->position, entity->velocity);
}

// get where the physics entity (a) will be in the next frame according to velocity
Vector3 getNextFramePhysicsEntity(PhysicsEntity *a){
	Vector3 next;

	next.x = a->position.x + a->velocity.x;
	next.y = a->position.y + a->velocity.y;
	next.z = a->position.z + a->velocity.z;
	
	return next;
}

// returns true if a and b are colliding, and false if not (NOTE: doesn't handle collision)
bool checkPhysicsEntityCollision(PhysicsEntity *a, PhysicsEntity *b){
	Vector3 aNext = getNextFramePhysicsEntity(a);
	Vector3 bNext = getNextFramePhysicsEntity(b);
	Vector3 blank = createBlankVector3();
	
	// check if there's any chance of the entities colliding via bounding circle
	if( getMagVector3( subtractVector3(a->position, b->position) ) > (a->boundingCircle/2 + b->boundingCircle/2)){
		// there's no chance of collision, exit
		return false;
	}	
	
	// if there's no rotation, use cube-cube collision for efficiency
	if( compareVector3(&(a->rotation), &blank) && compareVector3(&(b->rotation), &blank) ){
		return (aNext.x+a->boundingBox.x/2 > bNext.x-b->boundingBox.x/2 && aNext.x-a->boundingBox.x/2 < bNext.x+b->boundingBox.x/2 && aNext.y+a->boundingBox.y/2 > bNext.y-b->boundingBox.y/2 && aNext.y-a->boundingBox.y/2 < bNext.y+b->boundingBox.y/2 && aNext.z+a->boundingBox.z/2 > bNext.z-b->boundingBox.z/2 && aNext.z-a->boundingBox.z/2 < bNext.z+b->boundingBox.z/2);
	}
	
	// if there is rotation in at least one entity, we use separating axis theorum
	// TODO: make this work with different shapes/models?
	// order: lower front left, lower back left, lower front right, lower back right, upper front left, upper back left, upper front right, upper back right
	Vector3 aCorners[8];
	Vector3 bCorners[8];
	
	// temporary positions of a and b so we can assign next frame positions without loss
	Vector3 aTemp = a->position;
	Vector3 bTemp = b->position;
	
	// assign next frame positions
	a->position = aNext;
	b->position = bNext;
	
	// generate corners
	int corner = 0;
	//height -1 = lower, height 1 = upper, width -1 = left, width 1 = right, depth -1 = front, depth 1 = back
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 2; j++){
			for(int k = 0; k < 2; k++){
				aCorners[corner] = generateCorner(a, i == 0 ? -1 : 1, j == 0 ? -1 : 1, k == 0 ? -1 : 1);
				bCorners[corner] = generateCorner(b, i == 0 ? -1 : 1, j == 0 ? -1 : 1, k == 0 ? -1 : 1);
			
				corner++;
			}
		}
	}
	
	// reassign old positions
	a->position = aTemp;
	b->position = bTemp;
	
	// generate each axis, local to 0, 0, 0
	Vector3 axisA1 = subtractVector3(aCorners[6], aCorners[4]); //ur-ul (depth irrelevant)
	Vector3 axisA2 = subtractVector3(aCorners[6], aCorners[2]); //ur-lr (depth irrelevant)
	Vector3 axisA3 = subtractVector3(aCorners[6], aCorners[7]); //fr-br (height irrelevant)
	Vector3 axisB1 = subtractVector3(bCorners[6], bCorners[4]); //ur-ul (depth irrelevant)
	Vector3 axisB2 = subtractVector3(bCorners[6], bCorners[2]); //ur-lr (depth irrelevant)
	Vector3 axisB3 = subtractVector3(bCorners[6], bCorners[7]); //fr-br (height irrelevant)
	
	Vector3 *axes[] = {&axisA1, &axisA2, &axisA3, &axisB1, &axisB2, &axisB3};
	
	bool colliding = true;
	
	for(int i = 0; i < 6; i++){
		Vector3 *axis = axes[i]; // current axis to check

		// minimum and maximum scalar values representing position on axis
		double amin = 0;
		double amax = 0;
		double bmin = 0;
		double bmax = 0;

		// project corners and determine min/max values
		for(int j = 0; j < 8; j++){
			// corner projected onto current axis
			Vector3 aProjected = magnifyVector3(*axis, dotVector3(aCorners[j], *axis) / getMagVector3(*axis)*getMagVector3(*axis) );
			Vector3 bProjected = magnifyVector3(*axis, dotVector3(bCorners[j], *axis) / getMagVector3(*axis)*getMagVector3(*axis) );
			
			// temp values to check against min and max
			double atemp = dotVector3(aProjected, *axis);
			double btemp = dotVector3(bProjected, *axis);
			
			// assign values to min and max if necessary
			if(atemp < amin || amin == 0)
				amin = atemp;
			else if(atemp > amax || amax == 0)
				amax = atemp;
			
			if(btemp < bmin || bmin == 0)
				bmin = btemp;
			else if(btemp > bmax || bmax == 0)
				bmax = btemp;
		}
		
		// if the min/max values aren't intersecting, break and exit with colliding = false
		if(bmin > amax || bmax < amin){
			colliding = false;
			break;
		}
		// otherwise, continue until no more axes to check
	}
	
	return colliding;
}

// handles a collisions between entities in collision group
// collisions are handled with consideration of every entity in the group
void handlePhysicsEntityCollisions(std::vector<PhysicsEntity*> *collisionGroup){
	// because velocity flow simulation is fairly expensive, we only run do it with three or more entities because the result of two entities colliding is always a velocity exchange
	// so if there's two we exchange velocity (collisionGroup should never have a size of 1)
	if( collisionGroup->size() == 2){
		PhysicsEntity *a = collisionGroup->at(0);
		PhysicsEntity *b = collisionGroup->at(1);
		
		Vector3 aVelocity = a->velocity;
		Vector3 bVelocity = b->velocity;
		
		a->velocity = bVelocity;
		b->velocity = aVelocity;
	} else {
		// velocity flow simulation (TODO: maybe wrap this into a function?)
		std::vector<PhysicsEntity*> activeEntities; // contain entities which currently have velocity that (may) need to flow elsewhere
		
		// find entities with velocity to be active at first
		for(std::vector<PhysicsEntity*>::iterator i = collisionGroup->begin(); i < collisionGroup->end(); i++){
			// velocity can't flow if there's none of it!  push any entity that has velocity which can flow
			if( !isVector3Empty( &((*i)->velocity) ) ){
				activeEntities.push_back(*i);
			}
		}
		
		// loop until we have no more active entities 
		while(activeEntities.size() > 0){
			PhysicsEntity *activeEntity = activeEntities[0]; //start from the first active entity
			
			Vector3 activeEntityVelocity = activeEntity->velocity; //store this so we can transfer velocity to multiple entities without loss
			
			// loop through each entity (again) and find one to transfer velocity to
			for(std::vector<PhysicsEntity*>::iterator j = collisionGroup->begin(); j != collisionGroup->end(); j++){
				if( activeEntity->id == (*j)->id ) continue; //move to next entity if we're checking on ourselves
				
				// check if we can transfer velocity
				Vector3 direction = subtractVector3((*j)->position, activeEntity->position); //get vector between two entities
				
				// TODO: this is really dumb, we've already checked collisions, do we need to do it again?
				// check if we're colliding and if our velocity is "facing" the entity
				// if it is, transfer time!
				if( checkPhysicsEntityCollision( *j, activeEntity ) && dotVector3(direction, activeEntityVelocity) > 0){
					activeEntity->velocity = (*j)->velocity;
					(*j)->velocity = activeEntityVelocity;
					
					activeEntities.push_back(*j); //add entity to active entities
				}
			}
			
			activeEntities.erase(activeEntities.begin()); //erase first entity in activeEntities (the one we just checked)
		}
		
		// if no entity has velocity, then the entities spawned in each other and we don't really need to do anything about that
	}
}

// broken function which checks all entities against map collisions, don't use this. it's here for fun
void handleAllMapCollisions(std::vector<PhysicsEntity> *mapObjects){
	// loop through physics entities
	for(std::vector<PhysicsEntity>::iterator i = manager.entities.begin(); i != manager.entities.end(); i++){
		// loop through map objects
		for(std::vector<PhysicsEntity>::iterator j = mapObjects->begin(); j != mapObjects->end(); j++){
			// note: directional vector is stored in the velocity component because it's unused otherwise
			Vector3 directional = createVector3(1-abs( (*j).velocity.x ), 1-abs( (*j).velocity.y ), 1-abs( (*j).velocity.z) ); // store/inverse directional temporarily
			Vector3 oldDirectional = (*j).velocity;
			
			(*j).velocity = createBlankVector3(); // clear velocity so collisions are detected properly
			
			if( checkPhysicsEntityCollision( &(*i), &(*j) ) ){
				(*i).velocity = multiplyVector3( (*i).velocity, directional);
			}
			
			(*j).velocity = oldDirectional; // reapply velocity so other entities work
		}
	}
}

// handles collision for all physics entities
void handleAllCollisions(std::vector<PhysicsEntity> map){
	// TODO: optimize collision grouping
	std::vector<PhysicsEntity*> collisionGroup; //not the most memory-efficient, but create a new vector to temporarily store collision groups
	std::vector<PhysicsEntity*> collided; //physics entities that have already been in a collision group and don't need to be checked
	
	// no point in handling collisions for less than 2 entities
	if( manager.entities.size() < 2) return;
	
	for(std::vector<PhysicsEntity>::iterator i = manager.entities.begin(); i != manager.entities.end()-1; i++){
		// make sure we haven't already checked collisions for entity
		bool alreadyChecked = vectorContainsItem(&collided, &(*i));
		if( alreadyChecked ){
			continue;
		}
		
		// push current entity to collision group + collided
		collisionGroup.push_back(&(*i));
		collided.push_back(&(*i));
		
		// loop through each entity in front of it
		for(std::vector<PhysicsEntity>::iterator j = i+1; j != manager.entities.end(); j++){
			// make sure entity isn't already in collision group, skip if it is
			bool inGroup = vectorContainsItem(&collisionGroup, &(*j));
			if( inGroup ){
				continue;
			}
			
			// check if entity is colliding with any entity in collision group
			for(std::vector<PhysicsEntity*>::iterator k = collisionGroup.begin(); k != collisionGroup.end(); k++){
				// if we're colliding, push to collision group and restart the for loop
				if( checkPhysicsEntityCollision( &(*j), *k) ){
					collisionGroup.push_back(&(*j));
					collided.push_back(&(*j));
					
					j = i;
					break;
				}
			}
		}
		
		// handle collision group if we have more than one in it
		if( collisionGroup.size() > 1 ){
			handlePhysicsEntityCollisions(&collisionGroup);
		}
	
		// clear the collision group for the next iteration
		collisionGroup.clear();
	}
}

// testingin34tinisdtnistnistionto
bool vectorContainsItem(std::vector<PhysicsEntity> *v, PhysicsEntity *p){
	for(std::vector<PhysicsEntity>::iterator i = v->begin(); i != v->end(); i++){
		if( (*i).id == p->id)
			return true;
	}
	
	return false;
}

bool vectorContainsItem(std::vector<PhysicsEntity*> *v, PhysicsEntity *p){
	for(std::vector<PhysicsEntity*>::iterator i = v->begin(); i != v->end(); i++){
		if( (*i)->id == p->id)
			return true;
	}
	
	return false;
}

// formats and prints physics entity information (newlines after if newline == true)
void printPhysicsEntity(PhysicsEntity *e, bool newline){
	printf("Printing details of PhysicsEntity with id: %d...\nPosition:  ", e->id);
	printVector3(e->position, true);
	printf("Rotation:  ");
	printVector3(e->rotation, true);
	printf("Velocity:  ");
	printVector3(e->velocity, true);
	printf("Done.");
	if(newline) printf("\n");
}

// ENVIRONMENT FUNCTIONS //

// updates all entities
void updateEnvironment(double deltaFactor){
	for (std::vector<PhysicsEntity>::iterator i = manager.entities.begin(); i != manager.entities.end(); i++){
		updatePhysicsEntity(&(*i), deltaFactor);
	}
}