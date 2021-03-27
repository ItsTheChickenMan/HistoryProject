// note: both the server and the client should have the SAME COPY of this file.
// why does this exist?: so if I want to change a type's value, I don't have to replace every instance of it

#ifndef NETWORKING_TYPES

#define NETWORKING_TYPES

// update types
#define NETWORKING_POSITION 0
#define NETWORKING_ROTATION 1
#define NETWORKING_VELOCITY 2
#define NETWORKING_FORCE 3
#define NETWORKING_CONNECT 4
#define NETWORKING_DISCONNECT 5
#define NETWORKING_ID 6
#define NETWORKING_ID_PULL 7
#define NETWORKING_SERVER_NOTICE 8
#define NETWORKING_MAP 9

// protocol
#define NETWORKING_UDP 0
#define NETWORKING_TCP 1

// notice types
#define NETWORKING_SERVER_FULL -1

#endif