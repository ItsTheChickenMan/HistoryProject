// asset management header

#ifndef ASSET_H
#define ASSET_H

#include <poor/types.h>

struct Texture_Info {
    s32 width;
    s32 height;
    s32 channel_count;
    u8* data;
		
		float shininess;
		
		char* name;
		char* path;
		int id;
};

Texture_Info *getNullAsset();

Texture_Info *getAssetByName(char* name);
Texture_Info *getAssetByPath(char* path);
Texture_Info *getAssetById(int id);

Texture_Info *loadAsset(char* path, char* name, int id);

void terminateAssets();

int assetLog(char* log);
int assetError(char* error, int code);

#endif