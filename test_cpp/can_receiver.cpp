#include <iostream>
#include <fstream>
#include <unordered_map>
#include <memory>
#include <cstring>
#include <chrono>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include "dbcppp/include/dbcppp/Network.h"

using namespace std::chrono;
using canid_t = uint32_t;

int main() {
    std::ifstream idbc("../example.dbc");
    auto net = dbcppp::INetwork::LoadDBCFromIs(idbc);
    if (!net) {
        std::cerr << "Failed to parse DBC file.\n";
        return 1;
    }

    std::unordered_map<uint64_t, const dbcppp::IMessage*> messages;
    for (const auto& msg : net->Messages()) {
        messages[msg.Id()] = &msg;
    }

    int sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sock < 0) {
        std::cerr << "Error while opening socket" << std::endl;
        return 1;
    }

    struct sockaddr_can addr;
    struct ifreq ifr;
    strcpy(ifr.ifr_name, "vcan0");
    ioctl(sock, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(sock, (struct sockaddr *)&addr, sizeof(addr));

    uint64_t last_received = 0;
    auto last_received_time = steady_clock::now();
    struct can_frame frame;
    constexpr auto message_timeout = seconds(2); // Timeout for message switching

    while (true) {
        int nbytes = read(sock, &frame, sizeof(struct can_frame));
        if (nbytes > 0) {
            auto current_time = steady_clock::now();
            auto iter = messages.find(frame.can_id);
            if (iter != messages.end()) {
                const auto& msg = *iter->second;
                // Check if SpeedMessage2 is received or if SpeedMessage1 should be reactivated
                if (msg.Id() == 254 || (msg.Id() == 255 && (last_received != 254 || duration_cast<seconds>(current_time - last_received_time) >= message_timeout))) {
                    std::cout << "Received Message: " << msg.Name() << "\n";
                    for (const auto& sig : msg.Signals()) {
                        std::cout << "\t" << sig.Name() << " = " << sig.RawToPhys(sig.Decode(frame.data)) << " " << sig.Unit() << "\n";
                    }
                    last_received = msg.Id();
                    last_received_time = current_time; // Update the last received time
                }
            }
        }
        // Check timeout for switching back to SpeedMessage1
        if (last_received == 254) {
            auto current_time = steady_clock::now();
            if (duration_cast<seconds>(current_time - last_received_time) >= message_timeout) {
                last_received = 0; // Allow SpeedMessage1 to be printed again
            }
        }
    }
    close(sock);
    return 0;
}
