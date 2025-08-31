
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

    std::string packet("CMND");
    packet.append(command);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");

    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(49000); // X-Plane listens on UDP 49000
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    ssize_t sent = sendto(sock, packet.data(), packet.size(), 0,
                          reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    if (sent < 0) {
        perror("sendto");

    return 0;
}

