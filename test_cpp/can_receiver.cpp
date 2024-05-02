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

using canid_t = uint32_t;

int main()
{
    // Load DBC file
    std::ifstream idbc("../example.dbc");
    auto net = dbcppp::INetwork::LoadDBCFromIs(idbc);
    if (!net)
    {
        std::cerr << "Failed to parse DBC file.\n";
        return 1;
    }

    std::unordered_map<uint64_t, const dbcppp::IMessage*> messages;
    for (const auto& msg : net->Messages())
    {
        messages[msg.Id()] = &msg;
    }

    // Setup SocketCAN
    int sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sock < 0)
    {
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

    struct can_frame frame;
    while (true)
    {
        int nbytes = read(sock, &frame, sizeof(struct can_frame));
        if (nbytes > 0)
        {
            auto iter = messages.find(frame.can_id);
            if (iter != messages.end())
            {
                const auto& msg = *iter->second;
                std::cout << "Received Message: " << msg.Name() << "\n";
                for (const auto& sig : msg.Signals())
                {
                    const auto* mux_sig = msg.MuxSignal();
                    if (!mux_sig || (sig.MultiplexerIndicator() == dbcppp::ISignal::EMultiplexer::MuxValue &&
                        mux_sig->Decode(frame.data) == sig.MultiplexerSwitchValue()))
                    {
                        std::cout << "\t" << sig.Name() << " = " << sig.RawToPhys(sig.Decode(frame.data)) << " " << sig.Unit() << "\n";
                    }
                }
            }
        }
    }
    close(sock);
    return 0;
}
