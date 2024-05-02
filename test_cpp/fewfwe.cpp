#include <fstream>
#include <iostream>
#include <unordered_map>
#include <memory>
#include "dbcppp/include/dbcppp/Network.h"

// from uapi/linux/can.h
using canid_t = uint32_t;
struct can_frame
{
    canid_t    can_id;  /* 32 bit CAN_ID + EFF/RTR/ERR flags */
    uint8_t    can_dlc; /* frame payload length in byte (0 .. CAN_MAX_DLEN) */
    uint8_t    __pad;   /* padding */
    uint8_t    __res0;  /* reserved / padding */
    uint8_t    __res1;  /* reserved / padding */
    uint8_t    data[8];
};

void receive_frame_data(can_frame* frame)
{
    // Set data for test_msg_mux0 from src/Test/Test.dbc
    frame->can_id = 1;
    *reinterpret_cast<uint64_t*>(frame->data) = 0;
    // set mux_switch_value to 3 (m3)
    frame->data[0] |= 3;
    // set value for signal s3_1 to 13
    frame->data[2] |= 15;
}

int main()
{
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
        messages.insert({msg.Id(), &msg});
    }

    can_frame frame;
    while (true)
    {
        receive_frame_data(&frame);
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
        std::cout << std::flush;
    }

    return 0;
}
