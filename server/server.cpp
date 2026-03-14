#include <iostream>
#include <thread>
#include <vector>
#include <cstring>
#include <arpha/inet.h>
#include <unsitd.h>

const int PORT = 54000;
const int BOARD_SIZE = 8;
int board[BOARD_SIZE][BOARD_SIZE];

void sendBoard(int clientSocket){
	send(client,board,sizeof(board),0);
}

void handleClient(int clientSocket,int playerNumber){
	while(true){
		int x1,x2,y1,y2;

		int bytesRecieved = recv(clientSocket,&x1 , sizeof(int)){
			if (bytesReceived <= 0){
			}

	}

