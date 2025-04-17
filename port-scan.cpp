#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

bool is_port_open(const string& ip, int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) return false;
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 100000;  // 100ms
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));

    int result = connect(sockfd, (sockaddr*)&addr, sizeof(addr));
    close(sockfd);

    return result == 0;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Foydalanish: " << argv[0] << " <IP manzil>\n";
        return 1;
    }

    string ip = argv[1];
    cout << "Port skanerlash boshlanmoqda: " << ip << endl;

    for (int port = 20; port <= 1024; ++port) {
        if (is_port_open(ip, port)) {
            cout << "Ochiq port topildi: " << port << endl;
        }
    }

    cout << "Skanerlash tugadi.\n";
    return 0;
}
