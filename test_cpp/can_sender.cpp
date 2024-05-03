#include <iostream>
#include <fstream>
#include <unordered_map>
#include <memory>
#include <cstring>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include "dbcppp/include/dbcppp/Network.h"

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

    auto iter = messages.find(255);
    if (iter == messages.end()) {
        std::cerr << "Message with ID 255 not found in DBC.\n";
        return 1;
    }

    const auto& msg = *iter->second;
    struct can_frame frame;
    frame.can_id = 255;
    frame.can_dlc = 8;

    while (true) {
        std::vector<uint8_t> frame_data(8, 0);

        for (const auto& sig : msg.Signals()) {
            if (sig.Name() == "Speed") {
                double value = 10; // Set Speed to 10
                auto raw_value = sig.PhysToRaw(value);
                sig.Encode(raw_value, frame_data.data());
            }
        }

        std::memcpy(frame.data, frame_data.data(), frame_data.size());
        int nbytes = write(sock, &frame, sizeof(struct can_frame));
        if (nbytes != sizeof(struct can_frame)) {
            std::cerr << "Error while sending frame" << std::endl;
        } else {
            std::cout << "Message sent successfully." << std::endl;
        }

        sleep(1); // Wait for 1 second before sending the next frame
    }

    close(sock);
    return 0;
}
