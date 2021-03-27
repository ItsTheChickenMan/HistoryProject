// asset management (for images and the like)

#include <poor/asset.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <vector>

// stores all loaded textures
std::vector<Texture_Info> loadedAssets;

// null asset
Texture_Info nullAsset;

// get nullAsset
Texture_Info *getNullAsset(){
	return &nullAsset;
}

// gets an asset by char* name, appointed by user
Texture_Info *getAssetByName(char* name){
	for(std::vector<Texture_Info>::iterator i = loadedAssets.begin(); i != loadedAssets.end(); i++){
		if( strcmp(name, (*i).name) == 0)
			return &(*i);
	}
	
	return &nullAsset;
}

// gets an asset by char* path
Texture_Info *getAssetByPath(char* path){
	for(std::vector<Texture_Info>::iterator i = loadedAssets.begin(); i != loadedAssets.end(); i++){
		if( strcmp(path, (*i).path) == 0)
			return &(*i);
	}
	
	return &nullAsset;
}

// gets an asset by integer id, appointed by user
Texture_Info *getAssetById(int id){
	for(std::vector<Texture_Info>::iterator i = loadedAssets.begin(); i != loadedAssets.end(); i++){
		if(id == (*i).id)
			return &(*i);
	}
	
	return &nullAsset;
}

// load an asset from path into a TextureInfo struct and returns a pointer to the loaded asset
Texture_Info *loadAsset(char* path, char* name, int id){
	stbi_set_flip_vertically_on_load(true);
	
	s32 width, height, channels;
	u8* data = stbi_load(path, &width, &height, &channels, 0);
	
	if (!data){
		assetError(strcat("Failed to load asset: ", path), -1);
		return &nullAsset;
	}
	
	loadedAssets.insert(loadedAssets.begin(), (Texture_Info){width, height, channels, data, 32.0f, name, (char*)path, id} );
	
	// return a pointer to the first element (the one we just pushed)
	return &(loadedAssets[0]);
}

// free all assets
void terminateAssets(){
	for(std::vector<Texture_Info>::iterator i = loadedAssets.begin(); i != loadedAssets.end(); i++){
		// free data
		stbi_image_free( (*i).data );
	}
	
	loadedAssets.clear();
}

int assetLog(char* log){
	printf("Poor Engine: Asset Log: %s\n", log);
	
	return 0;
}

int assetError(char* error, int code){
	printf("Poor Engine: Asset Error: %s\n", error);
	
	return code;
}