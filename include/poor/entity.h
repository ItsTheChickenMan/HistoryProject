// entity manager header (hooooo boy)

#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <cstdlib>

// will I ever be forgiven for using templates

// create an entity manager of EntityType (struct expected)
template <typename EntityType>
struct EntityManager {
	std::vector<EntityType> entities;
	
	EntityType nullEntity;
};

// template declarations (to avoid having reorder functions)

template <typename EntityType>
EntityType *pushEntity(EntityManager<EntityType> *manager, EntityType entity, int id);

template <typename EntityType>
EntityType pullEntity(EntityManager<EntityType> *manager, int id);

template <typename EntityType> 
EntityType *getEntityById(EntityManager<EntityType> *manager, int id);

template <typename EntityType>
int generateEntityId(EntityManager<EntityType> *manager);

template <typename EntityType>
bool idTaken(EntityManager<EntityType> *manager, int id);

// template definitions

// in order to reduce the amount of templates I'm using, I won't overload this function like usual.  To randomize the id, set it to -1, otherwise, it'll accept whatever id is
// push entity to an entity manager
template <typename EntityType>
EntityType *pushEntity(EntityManager<EntityType> *manager, EntityType entity, int id){
	if(id == -1){
		entity.id = generateEntityId<EntityType>(manager);
	} else {
		entity.id = id;
	}
	
	manager->entities.push_back(entity);
	
	return &( manager->entities[ manager->entities.size() - 1] );
}

// I won't implement destroyEntity, as this can act a lot like destroyEntity if you just ignore the return value
template <typename EntityType>
EntityType pullEntity(EntityManager<EntityType> *manager, int id){
	for(typename std::vector<EntityType>::iterator i = manager->entities.begin(); i != manager->entities.end(); i++){
		if( (*i).id == id){
			EntityType ent = *i;
			
			manager->entities.erase(i);
			return ent;
		}
	}
	
	return manager->nullEntity;
}

template <typename EntityType> 
EntityType *getEntityById(EntityManager<EntityType> *manager, int id){
	for(typename std::vector<EntityType>::iterator i = manager->entities.begin(); i != manager->entities.end(); i++){
		if( (*i).id == id){
			return &(*i);
		}
	}
	
	return &(manager->nullEntity);
}

// NOTE: the two id functions below don't really need to be templates...find a better way to do this, unless it doesn't affect much

// generates a new entity id from 0 to 32767 (which is the default random max)
template <typename EntityType>
int generateEntityId(EntityManager<EntityType> *manager){
	int id;
	
	do {
		id = rand() % 32767; //modulo with the usual rand_max just for safety
	} while(idTaken(manager, id));
	
	return id;
}

// returns true if id is taken, otherwise returns false
template <typename EntityType>
bool idTaken(EntityManager<EntityType> *manager, int id){
	for (typename std::vector<EntityType>::iterator i = manager->entities.begin(); i != manager->entities.end(); i++){
		if( (*i).id == id){
			return true;
		}
	}
	
	return false;
}

#endif