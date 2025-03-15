#ifndef DUMMY_UDP_CMD_HANDLER_
#define DUMMY_UDP_CMD_HANDLER_
#include <cstdint>
#include <stddef.h>
#include "dummyCmd.h"
class DummyUdpCmdHandler
{
private:
    DummyCmd dummyCmd;
    float * joints = nullptr;
    /* data */
public:
    DummyUdpCmdHandler(/* args */);
    ~DummyUdpCmdHandler();
    void ParseCmd(const uint8_t *buffer, size_t size);
    void SetJoints(float * joints);
};
#endif
