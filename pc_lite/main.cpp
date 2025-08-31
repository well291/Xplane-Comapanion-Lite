#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

// Simple PC companion stub that sends a single X-Plane command via UDP.
// Usage: ./pc_lite_app "sim/operation/pause_toggle"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <xplane_command>" << std::endl;
        return 1;
    }

    const char* command = argv[1];

    // Build the packet: 4-byte label "CMND" followed by the command string.
    std::string packet("CMND");
    packet.append(command);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(49000); // X-Plane listens on UDP 49000
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    ssize_t sent = sendto(sock, packet.data(), packet.size(), 0,
                          reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    if (sent < 0) {
        perror("sendto");
        close(sock);
        return 1;
    }

    std::cout << "Sent command: " << command << std::endl;
    close(sock);
    return 0;
}

