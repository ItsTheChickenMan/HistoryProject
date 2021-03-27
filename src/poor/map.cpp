// map module (kind of scattered, but should be ok for its purpose)

#include <cstdio>
#include <cstdlib>
#include <tgmath.h>
#include <ctime>

#include <poor/map.h>
#include <poor/io_util.h>

// COLLISION MAP GENERATION //
Map generateCollisionMap(Map *map){
	Map cmap;

	// run through each map object and create six faces for it, and their directional vectors
	// TODO: make this work for more than cubes
	for(std::vector<MapObject>::iterator i = map->objects.begin(); i != map->objects.end(); i++){
		MapObject face; // temp face
		
		for(int j = 0; j < 5; j++){
			face = createFace(&(*i), j);
			cmap.objects.push_back(face);
		}
	}
	
	return cmap;
}

// calculate how many bytes it would take to represent a map as map data (in text)
long calculateMapSize(Map *map){
	long size = 0;
	
	for(std::vector<MapObject>::iterator i = map->objects.begin(); i != map->objects.end(); i++){
		size += 2; //add two bytes for opening and closing brackets
		
		double data[] = {(*i).position.x, (*i).position.y, (*i).position.z, (*i).rotation.x, (*i).rotation.y, (*i).rotation.z, (*i).boundingBox.x, (*i).boundingBox.y, (*i).boundingBox.z};
		
		for(int j = 0; j < sizeof(data)/sizeof(double); j++){
			// we need to convert double value of the object to a string to see how much data we need to reserve
			size += doubleToString(data[j], 1).size() + 1; //add size required to write data to size, plus one for comma
		}
	}
	
	return size;
}

// create a byte representation from a map (remember to free(buffer)!)
char *createMapBuffer(Map *map){
	long size = calculateMapSize(map);
	
	char *buffer = (char*)malloc( size ); // allocate buffer for parsed map
	long bufferIndex = 0;
	
	for(std::vector<MapObject>::iterator i = map->objects.begin(); i != map->objects.end(); i++){
		buffer[bufferIndex] = '[';
		bufferIndex++;
		
		double data[] = {(*i).position.x, (*i).position.y, (*i).position.z, (*i).rotation.x, (*i).rotation.y, (*i).rotation.z, (*i).boundingBox.x, (*i).boundingBox.y, (*i).boundingBox.z};
		
		for(int j = 0; j < sizeof(data)/sizeof(double); j++){
			std::string dataPoint = doubleToString(data[j], 1);
			
			// wow!  triple for loop!
			for(int k = 0; k < dataPoint.size(); k++){
				buffer[bufferIndex] = dataPoint[k];
				bufferIndex++;
			}
			
			buffer[bufferIndex] = ',';
			bufferIndex++;
		}
		
		buffer[bufferIndex] = ']';
		bufferIndex++;
	}
	
	buffer[bufferIndex] = 0;
	
	return buffer;
}

// convert double d to a string and leave behind trailing number of zeroes
std::string doubleToString(double d, int trailing){
	std::string doubleString = std::to_string(d);
	
	doubleString.append(trailing, '0'); // to avoid erasing more than the string's length, append trailing amount of zeroes
	
	doubleString.erase(doubleString.find_last_not_of('0')+(trailing+1), std::string::npos); // erase trailing zeroes (except for 1 to maintain some precision)

	return doubleString;
}

// create a face for obj 
// to make it a little easier, face represents which face to create:
// 0 - x left	1 - x right	2 - z left 3 - z right 4 - y down 5 - y up
// TODO: optimize, but not too important since it only needs to be generated once every time the map is changed
MapObject createFace(MapObject *obj, int face){
	MapObject faceObject; // create the face
	double pi = 3.1415926535;
	
	// not the most efficient method, but for now, this works ok
	// TODO: make this work for different shapes/models without having to do this
	switch(face){
		case 0:
			faceObject.position = createVector3(obj->position.x-obj->boundingBox.x/2, obj->position.y, obj->position.z);
			faceObject.rotation = createVector3(0, obj->rotation.y, obj->rotation.z+(pi/2));
			break;
		case 1:
			faceObject.position = createVector3(obj->position.x+obj->boundingBox.x/2, obj->position.y, obj->position.z);
			faceObject.rotation = createVector3(0, obj->rotation.y, obj->rotation.z+(pi/2));
			break;
		case 2:
			faceObject.position = createVector3(obj->position.x, obj->position.y, obj->position.z-obj->boundingBox.z/2);
			faceObject.rotation = createVector3(obj->rotation.x+(pi/2), obj->rotation.y, 0);
			break;
		case 3:
			faceObject.position = createVector3(obj->position.x, obj->position.y, obj->position.z+obj->boundingBox.z/2);
			faceObject.rotation = createVector3(obj->rotation.x+(pi/2), obj->rotation.y, 0);
			break;
		case 4:
			faceObject.position = createVector3(obj->position.x, obj->position.y-obj->boundingBox.y/2, obj->position.z);
			faceObject.rotation = createVector3(obj->rotation.x, 0, obj->rotation.z);
			break;
		case 5:
			faceObject.position = createVector3(obj->position.x, obj->position.y+obj->boundingBox.y/2, obj->position.z+obj->boundingBox.z);
			faceObject.rotation = createVector3(obj->rotation.x, 0, obj->rotation.z);
			break;
		default:
			break;
	}

	faceObject.boundingBox = createVector3(obj->boundingBox.x, 0, obj->boundingBox.z);
	
	return faceObject;
}

// calculate directional vector for face (assumed to be a face)
// directional vectors are normalized vectors representing the direction that a face is facing (little confusing, best way I can think of to describe them at the moment, sorry)
// they're used to calculate how much velocity to reduce/add to an entity when colliding with a static object, and because the map is static, these can be pre-generated because they never change
Vector3 calculateDirectionalVector(MapObject *face){
	Vector3 up = createVector3(0, 1, 0); //vector representing a face with 0 rotation (pointing straight up)
	
	rotateVector3(&up, face->rotation.x, PHYSICS_X_AXIS);
	rotateVector3(&up, face->rotation.y, PHYSICS_Y_AXIS);
	rotateVector3(&up, face->rotation.z, PHYSICS_Z_AXIS);
	
	return up;
}

// PARSING //

// parses map from buffer (frees buffer)
Map parseMap(char* buffer){
	Map map; // create the map
	
	if(buffer == NULL) return map; // return an empty map if the buffer is null
	
	char* textureNames[] = {"greyfloor", "secretroom", "door", "framedlongtelegram", "berlinwall", "default", "floor"}; // this is great really good solid clean code
	
	double objData[12]; //stream to hold object data (position.x, position.y, position.z, rotation.x, rotation.y, rotation.z, boundingBox.x, boundingBox.y, boundingBox.z, *directionalVector.x, *directionalVector.y, *directionalVector.z) * = depends on mode
	char* texture;
	short dataIndex = 0; //current index being written to
	int parseMode = 0; // 0 objects, 1 point lights, 2 spot lights
	map.mode = MAP_PEP; //defaults to pep
	
	long dataBufferIndex = 0; //long to store position of uncomplete data
	
	long current = 0; //current byte being read from buffer
	
	// read until end of buffer
		// read until end of buffer
	while( buffer[current] != 0 ){
		char byte = buffer[current];
		
		// advance to next byte (only takes affect next loop)
		current++;
		
		// manage special characters
		if(byte == '!'){ //mode declaration byte, following chars are expected to either be PEP or PEM (only actually checks for the last character, so it could be either P or M, but PEP or PEM is neater)
			if(buffer[current-2] == 'M'){
				map.mode = MAP_PEM;
			} else {
				map.mode = MAP_PEP;
			}
		} else if(byte == '['){ //object seperator byte, (start a new object)
			MapObject obj;
		
			dataBufferIndex = current; // set data buffer to current byte
			dataIndex = 0;
			
			if(parseMode == 0){
				map.objects.insert( map.objects.begin(), obj); //insert object at start
			} else if(parseMode == 1){
				map.lights.insert( map.lights.begin(), obj); //insert object at start
			} else if(parseMode == 2){
				//map.spotlights.insert( map.spotlights.begin(), obj);
			}
			continue;
		} else if(byte == ','){ //data seperator byte, (write to data point, advance data point)
			char* data = (char*)malloc(current-dataBufferIndex-1);
				
			// read collected data into a string
			for(int i = 0; i < current-dataBufferIndex-1; i++){
				data[i] = buffer[i+dataBufferIndex];
			}
			
			data[current-dataBufferIndex-1] = 0;
			
			// write data
			objData[dataIndex] = strtod(data, NULL);
			free(data);
			
			// increase data index
			if(dataIndex != sizeof(objData)/sizeof(double)-1) // write to next data index only if there's room to write to
				dataIndex++;
		
			dataBufferIndex = current; // set data buffer to current byte
			continue;
		} else if(byte == '^'){ // indicates that the previous data was a texture name
			texture = (char*)malloc(current-dataBufferIndex-1);
			
			for(int i = 0; i < current-dataBufferIndex-1; i++){
				texture[i] = buffer[i+dataBufferIndex];
			}
			
			texture[current-dataBufferIndex-1] = 0;
			
			dataBufferIndex = current;
			continue;
		} else if(byte == ']'){ // object close byte (tells us to save any object data)					
			MapObject *obj;
			
			if(parseMode == 0){
				obj = &(map.objects[0]);
				
				int tid = -1;
				
				for(int i = 0; i < sizeof(textureNames)/sizeof(char*); i++){
					if(strcmp( textureNames[i], texture) == 0){
						tid = i;
					}
				}
				
				if(tid != -1)
					map.objects[0].texture = tid;
				
				free(texture);
			} else {
				obj = &(map.lights[0]);
			}
			
			writeDataToObj( obj, objData, map.mode);
			
			continue;
		} else if(byte == '$'){ // mode switch byte (tells us to start parsing a different aspect of the map)
			parseMode++;
			
			continue;
		}
	}
	
	// free byte buffer
	free(buffer);
	free(texture);
	
	return map;
}

// NOTE: Free buffer when done with it.
// reads entire file at filename
char* read_entire_file(const char* file)
{
	FILE* source_file = fopen(file, "rb");
	if (!source_file)
	{
		perror("poor");
		return NULL;
	}
	fseek(source_file, 0, SEEK_END);
	long src_size = ftell(source_file);
	fseek(source_file, 0, SEEK_SET);
	char* buffer = (char*) malloc(src_size + 1);
	fread(buffer, 1, src_size, source_file);
	fclose(source_file);
	buffer[src_size] = 0;
	return buffer;
}

// write buffer to a source file (DOESN'T automatically free buffer)
long write_to_file(char *file, char *buffer){
	FILE* source_file = fopen(file, "w");
	if (!source_file)
	{
		perror("poor");
		return -1;
	}
	
	long current = 0;
	
	while(buffer[current] != 0){
		if( fputc((int)(buffer[current]), source_file) == EOF){
			perror("poor");
			return -2;
		}
		
		current++;
	}
	
	fclose(source_file);
	return current;
}

long write_to_file(const char *file, char *buffer){
	FILE* source_file = fopen(file, "w");
	if (!source_file)
	{
		perror("poor");
		return -1;
	}
	
	long current = 0;
	
	while(buffer[current] != 0){
		if( fputc((int)(buffer[current]), source_file) == EOF){
			perror("poor");
			return -2;
		}
		
		current++;
	}
	
	fclose(source_file);
	return current;
}

// 0: PEP, 1: PEM
void writeDataToObj(MapObject *obj, double *data, int mode){
	obj->position.x = data[0];
	obj->position.y = data[1];
	obj->position.z = data[2];
	obj->rotation.x = data[3];
	obj->rotation.y = data[4];
	obj->rotation.z = data[5];
	obj->boundingBox.x = data[6];
	obj->boundingBox.y = data[7];
	obj->boundingBox.z = data[8];
	
	if(mode == 1){
		obj->directionalVector.x = data[9];
		obj->directionalVector.y = data[10];
		obj->directionalVector.z = data[11];
	}
}

// easier way of going parseMap(read_entire_file(map));
Map loadMap(char* path){
	return parseMap(read_entire_file(path));
}

Map loadMap(const char* path){
	return parseMap(read_entire_file(path));
}

void printMapObj(MapObject *e){
	printf("Printing details of MapObject...\nPosition:  ");
	printVector3(e->position, true);
	printf("Size:  ");
	printVector3(e->boundingBox, true);
	printf("Done.\n");
}