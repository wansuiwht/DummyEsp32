#include<Arduino.h>
class DummyCmd
{
private:
    
    /* data */
public:
    DummyCmd(/* args */);
    ~DummyCmd();
    bool ExecCmd(String cmd);
    bool Home();
    bool Start();
    bool Stop();
    bool Reset();
    bool Calibration();
    bool Reboot();
    bool GetJPos();
    bool GetLPos();
    bool SetDCEKP(int value,int node);
    bool SetDCEKI(int value,int node);
    bool SetDCEKD(int value,int node);
    bool SendCmd(String cmd);
};
