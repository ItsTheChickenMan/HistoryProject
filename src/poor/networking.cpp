// networking manager, uses SFML networking because it's easy

#include <stdio.h>
#include <cstring>

#include <poor/networking.h>

// settings
static int debug = 1; //whether or not to log events (ethan: disable this if you want to work on your own stuff without my logs in your way) (NOTE: networking errors are still logged)

static sf::UdpSocket socket; //udp socket, doesn't need to be connected because of nature of udp

// The ip address the udp socket sends data to for every networkingSend() and networkingSendPacket() call
static sf::IpAddress serverIp = sf::IpAddress::getLocalAddress(); //defaults to the computer's local ip, which basically means it checks if there's an open server on the computer the client is run from by default

static u16 listenPort = 0;
static u16 sendPort = 0;

// because it would be insensible to return the entire IpAddress object, it returns the value of it's address
char* networkingGetServerIp(){
	return &serverIp.toString()[0];
}

// sets the server ip and returns it's value
char* networkingSetServerIp(char* ip){
	serverIp = sf::IpAddress(ip);
	
	networkingLog(strcat("Set server ip to", ip));
	
	return &serverIp.toString()[0];
}

// returns a reference to listen port
u16 *networkingGetListenPort(){
	return &listenPort;
}

// sets listen port to unsigned short and returns reference to it
u16 *networkingSetListenPort(u16 port){
	listenPort = port;
	
	return networkingGetListenPort();
}

// return a reference to send port
u16 *networkingGetSendPort(){
	return &sendPort;
}

u16 *networkingSetSendPort(u16 port){
	sendPort = port;
	
	return networkingGetSendPort();
}

// returns a reference to the udp socket
sf::UdpSocket *networkingGetSocket(){
	return &socket;
}

int networkingInit(){
	socket.setBlocking(false); //set blocking to false
	
	if(socket.bind(listenPort) != sf::Socket::Done){
		return networkingError("couldn't bind to port, maybe it's taken?", errno);
	} else {
		networkingLog("Bound to port");
	}
	
	return 0;
}

// send data over the current serverIp (in most cases, it's recommended to use the sendRequest functions, which are designed to send specific requests to the server easily)
int networkingSend(void* data, int size){
	if(socket.send(data, size, serverIp, sendPort) != sf::Socket::Done){
		return networkingError("Failed to send data", errno);
	}
	
	return 0;
}

// send packet over the current serverIp (in most cases, it's recommended to use the sendRequest functions, which are designed to send specific requests to the server easily)
int networkingSendPacket(sf::Packet packet){
	if(socket.send(packet, serverIp, sendPort) != sf::Socket::Done){
		return networkingError("Failed to send packet", errno);
	}
	
	return 0;
}

// creates an update request, return pointer to request  TODO: Make this more flexible, allow client to request changes to other aspects as well)
NetworkingUpdateRequest networkingCreateUpdateRequest(int update_type, double data[], int data_length){
	NetworkingUpdateRequest request;
	
	request.update_type = update_type;
	request.data_length = data_length;
	
	for(int i = 0; i < request.data_length; i++){
		request.data[i] = data[i];
	}
	
	return request;
}

// pack a networking request into a packet to be sent to the server, return pointer of packet
sf::Packet networkingPackUpdateRequest(NetworkingUpdateRequest *request){
	sf::Packet packet;
	
	packet << request->update_type << request->data_length; //request data is sent individually because arrays can't properly be sent to the server
	
	for(int i = 0; i < request->data_length; i++){
		packet << request->data[i];
	}

	return packet;
}

// unpack a packet and store it to an update request
NetworkingUpdateRequest networkingUnpackUpdateRequest(sf::Packet *packet){
	NetworkingUpdateRequest request;
	
	(*packet) >> request.update_type >> request.data_length;
	
	for(int i = 0; i < request.data_length; i++){
		(*packet) >> request.data[i];
	}
	
	return request;
}

// automatically packs and sends an update request
int networkingSendUpdateRequest(NetworkingUpdateRequest *request){
	sf::Packet packet = networkingPackUpdateRequest(request);
	
	return networkingSendPacket(packet);
}

// listens for any and all update requests and adds them to requests (non-blocking), returns the amount of requests
// TODO make this more abstract
int networkingGetUpdateRequests(std::vector<NetworkingUpdateRequest> *requests){
	int receiving = 0; //whether or not there's more packets to receive, also doubles as the index to write received requests to
	int numRequests = 0;
	
	do {
		sf::IpAddress ip; //ip (just created to sure that we're not receiving requests from another server)
		sf::Packet packet; //packet
		unsigned short port; //port (just created so it has something to assign to)
		
		if(socket.receive(packet, ip, port) == sf::Socket::Done){
			if(ip.toInteger() != serverIp.toInteger()) continue; // if we're receiving a request outside of the server, ignore it
			NetworkingUpdateRequest request = networkingUnpackUpdateRequest(&packet); //unpack the request
			
			requests->push_back(request); //push request to requests
			
			receiving++; //increase receiving so that next request gets pushed right
		} else {
			numRequests = receiving;
			receiving = 0; //end it all
		}
	} while(receiving);
	
	return numRequests;
}

int networkingConnect(){
	NetworkingUpdateRequest request = networkingCreateUpdateRequest(NETWORKING_CONNECT, {}, 0);
	
	return networkingSendUpdateRequest(&request);
}

// send a disconnect request to the server
int networkingDisconnect(){
	NetworkingUpdateRequest request = networkingCreateUpdateRequest(NETWORKING_DISCONNECT, {}, 0);
	
	return networkingSendUpdateRequest(&request);
}

// parses a notice from the server (for now just prints it, but might need to look into making it visible to the user via a pop up or something)
void networkingParseServerNotice(int notice){
	switch(notice){
		case NETWORKING_SERVER_FULL:
			networkingLog("Server Notice: Connection refused, the server is currently full.");
		default:
			break;
	}
}

// log some text (newlines after logs)
int networkingLog(char* log){
	if(debug) 
		printf("Poor Engine: Networking: %s\n", log);
	
	return 0;
};

// log a networking error
int networkingError(char* error, int code){
	printf("Poor Engine: Networking Error: %s", error);
	
	return code;
}