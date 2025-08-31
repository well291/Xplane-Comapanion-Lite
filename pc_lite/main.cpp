#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

// Button representation for the simple UI
struct Button {
    SDL_Rect rect;
    std::string label;
};

// Send a CMND packet to localhost:49000
bool send_command(const std::string& command) {
    std::string packet("CMND");
    packet.append(command);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return false;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(49000);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    ssize_t sent = sendto(sock, packet.data(), packet.size(), 0,
                          reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    if (sent < 0) {
        perror("sendto");
    }

    close(sock);
    std::cout << "Sent command: " << command << std::endl;
    return sent >= 0;
}

// Load commands from a text file (one command per line)
std::vector<std::string> load_commands(const std::string& path) {
    std::ifstream file(path);
    std::vector<std::string> cmds;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) cmds.push_back(line);
    }
    return cmds;
}

int main(int argc, char* argv[]) {
    auto commands = load_commands("pc_lite/commands.txt");
    if (commands.empty()) {
        std::cerr << "No commands found in pc_lite/commands.txt" << std::endl;
        return 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    if (TTF_Init() != 0) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    const int buttonWidth = 380;
    const int buttonHeight = 40;
    const int padding = 10;
    int windowHeight = (buttonHeight + padding) * commands.size() + padding;

    SDL_Window* win = SDL_CreateWindow("X-Plane Commands", SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED, 400, windowHeight, 0);
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 16);
    if (!font) {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    std::vector<Button> buttons;
    for (size_t i = 0; i < commands.size(); ++i) {
        SDL_Rect r{padding, padding + static_cast<int>(i) * (buttonHeight + padding),
                   buttonWidth, buttonHeight};
        buttons.push_back({r, commands[i]});
    }

    bool quit = false;
    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mx = e.button.x;
                int my = e.button.y;
                for (const auto& b : buttons) {
                    if (mx >= b.rect.x && mx <= b.rect.x + b.rect.w &&
                        my >= b.rect.y && my <= b.rect.y + b.rect.h) {
                        send_command(b.label);
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(ren, 30, 30, 30, 255);
        SDL_RenderClear(ren);

        for (const auto& b : buttons) {
            SDL_SetRenderDrawColor(ren, 80, 80, 80, 255);
            SDL_RenderFillRect(ren, &b.rect);

            SDL_Surface* surf = TTF_RenderUTF8_Blended(font, b.label.c_str(),
                                                       SDL_Color{255, 255, 255, 255});
            SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);
            int tx, ty;
            SDL_QueryTexture(tex, nullptr, nullptr, &tx, &ty);
            SDL_Rect dst{b.rect.x + 5, b.rect.y + (b.rect.h - ty) / 2, tx, ty};
            SDL_RenderCopy(ren, tex, nullptr, &dst);
            SDL_DestroyTexture(tex);
            SDL_FreeSurface(surf);
        }

        SDL_RenderPresent(ren);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    TTF_Quit();
    SDL_Quit();
    return 0;
}

