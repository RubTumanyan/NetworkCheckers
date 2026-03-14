#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../common/move.h"

#define PORT 54000

int main()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 2);

    std::cout << "Server waiting for players...\n";

    int p1 = accept(server_fd, nullptr, nullptr);
    std::cout << "Player 1 connected\n";

    int p2 = accept(server_fd, nullptr, nullptr);
    std::cout << "Player 2 connected\n";

    Move move;

    while(true)
    {
        recv(p1, &move, sizeof(move), 0);
        std::cout << "Move from P1\n";

        send(p2, &move, sizeof(move), 0);

        recv(p2, &move, sizeof(move), 0);
        std::cout << "Move from P2\n";

        send(p1, &move, sizeof(move), 0);
    }

    close(p1);
    close(p2);
    close(server_fd);
}

