#include <iostream>
#include <vector>
#include <map>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include "../common/move.h"

#define MAX_EVENTS 64
#define PORT 54000

// Helper to make sockets non-blocking
void set_nonblocking(int sock) {
    int opts = fcntl(sock, F_GETFL);
    fcntl(sock, F_SETFL, opts | O_NONBLOCK);
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{AF_INET, htons(PORT), INADDR_ANY};
    bind(server_fd, (sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 10);
    set_nonblocking(server_fd);

    int epoll_fd = epoll_create1(0);
    struct epoll_event ev, events[MAX_EVENTS];
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev);

    std::vector<int> waiting_room;
    std::map<int, int> pairs; // Maps fd -> opponent_fd

    std::cout << "Epoll Server running on port " << PORT << "..." << std::endl;

    while (true) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == server_fd) {
                int client = accept(server_fd, nullptr, nullptr);
                set_nonblocking(client);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = client;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client, &ev);
                
                waiting_room.push_back(client);
                if (waiting_room.size() >= 2) {
                    int p1 = waiting_room[0];
                    int p2 = waiting_room[1];
                    waiting_room.erase(waiting_room.begin(), waiting_room.begin() + 2);
                    
                    pairs[p1] = p2; pairs[p2] = p1;
                    int id1 = 1, id2 = 2;
                    send(p1, &id1, sizeof(int), 0);
                    send(p2, &id2, sizeof(int), 0);
                    std::cout << "Game started between " << p1 << " and " << p2 << std::endl;
                }
            } else {
                int fd = events[i].data.fd;
                Move m;
                int bytes = recv(fd, &m, sizeof(Move), 0);
                if (bytes <= 0) {
                    std::cout << "Player disconnected: " << fd << std::endl;
                    close(fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr);
                    // Handle cleanup for opponent if needed
                } else if (pairs.count(fd)) {
                    send(pairs[fd], &m, sizeof(Move), 0);
                }
            }
        }
    }
    return 0;
}
