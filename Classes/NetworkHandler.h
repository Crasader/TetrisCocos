#pragma once

#include "Board.h"
#include "Constants.h"
#include "message.pb.h"
#include "RakPeerInterface.h"


class NetworkHandler : public Subject
{
public:
	static NetworkHandler * getInstance();
	~NetworkHandler();
	void initRecieverServer();
	void initSenderServer();
	void pushDataToNetwork(const Board& board, t_network::Messagetype);
	void listen();

private:

	static RakNet::RakPeerInterface *rakPeer;
	static NetworkHandler *netInstance;
	NetworkHandler();
	void init();
	void createNTetrominoHelper(const Tetromino& from, tetris::proto::Tetromino* to);
};