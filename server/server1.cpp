#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <cstring>

#include "../common/move.h" // Your Move struct
#define PORT 54000
#define MAX_EVENTS 100

struct Game {
    int p1 = -1;
    int p2 = -1;
    bool active = false;
};

std::vector<int> waitingPlayers;
std::vector<Game> activeGames;

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "Bind failed\n";
        return 1;
    }

    if(listen(server_fd, 100) < 0) {
        std::cerr << "Listen failed\n";
        return 1;
    }

    std::cout << "Server waiting for players...\n";

    int epfd = epoll_create1(0);
    if(epfd < 0) {
        std::cerr << "epoll_create1 failed\n";
        return 1;
    }

    epoll_event ev{};
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, server_fd, &ev);

    epoll_event events[MAX_EVENTS];

    while(true) {
        int n = epoll_wait(epfd, events, MAX_EVENTS, -1);
        for(int i = 0; i < n; i++) {
            int fd = events[i].data.fd;

            if(fd == server_fd) {
                // New player connection
                int client_fd = accept(server_fd, nullptr, nullptr);
                if(client_fd < 0) continue;

                std::cout << "Player connected: " << client_fd << "\n";

                ev.events = EPOLLIN;
                ev.data.fd = client_fd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev);

                waitingPlayers.push_back(client_fd);

                // If we have 2 players, create a game
                if(waitingPlayers.size() >= 2) {
                    Game g;
                    g.p1 = waitingPlayers.back(); waitingPlayers.pop_back();
                    g.p2 = waitingPlayers.back(); waitingPlayers.pop_back();
                    g.active = true;
                    activeGames.push_back(g);

                    // Send player IDs
                    int id1 = 1, id2 = 2;
                    send(g.p1, &id1, sizeof(id1), 0);
                    send(g.p2, &id2, sizeof(id2), 0);

                    std::cout << "Game started: " << g.p1 << " vs " << g.p2 << "\n";
                }

            } else {
                // Check if this fd is in an active game
                Move move{};
                int bytes = recv(fd, &move, sizeof(move), 0);
                if(bytes <= 0) {
                    // Remove player from epoll
                    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, nullptr);
                    close(fd);
                    std::cout << "Player disconnected: " << fd << "\n";

                    // Remove from active games
                    for(auto it = activeGames.begin(); it != activeGames.end(); ++it) {
                        if(it->p1 == fd || it->p2 == fd) {
                            int other = (it->p1 == fd) ? it->p2 : it->p1;
                            epoll_ctl(epfd, EPOLL_CTL_DEL, other, nullptr);
                            close(other);
                            std::cout << "Ending game due to disconnect: " << other << "\n";
                            activeGames.erase(it);
                            break;
                        }
                    }
                    continue;
                }

                // Forward move to the other player
                for(auto &g : activeGames) {
                    if(g.p1 == fd) send(g.p2, &move, sizeof(move), 0);
                    else if(g.p2 == fd) send(g.p1, &move, sizeof(move), 0);
                }
            }
        }
    }

    close(server_fd);
    return 0;
}
