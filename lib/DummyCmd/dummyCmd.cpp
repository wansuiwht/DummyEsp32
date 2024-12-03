#include"dummyCmd.h"


DummyCmd::DummyCmd(/* args */)
{
}

DummyCmd::~DummyCmd()
{
}
bool DummyCmd::ExecCmd(String cmd){
    return SendCmd(cmd);
}
bool DummyCmd::Home(){
    return SendCmd("!HOME");
}
bool DummyCmd::Start(){
    return SendCmd("!START");
}
bool DummyCmd::Reset(){
    return SendCmd("!RESET");
}
bool DummyCmd::Calibration(){
    return SendCmd("!CALIBRATION");
}
bool DummyCmd::Stop(){
    return SendCmd("!STOP");
}
bool DummyCmd::GetJPos(){
    return SendCmd("#GETJPOS");
}
bool DummyCmd::GetLPos(){
    return SendCmd("#GETLPOS");
}
bool DummyCmd::Reboot(){
    return SendCmd("!REBOOT");
}
bool DummyCmd::SendCmd(String cmd){
   Serial1.write(cmd.c_str());
   //Serial1.write("#SET_IP ap\n");
    Serial1.println();
    return true;
}
bool DummyCmd::SetDCEKP(int value,int node){
     char cmd[64];
    size_t len = snprintf(cmd, sizeof(cmd), "#SET_DCE_KP %d %d", value,node);
    return SendCmd(cmd);
}
bool DummyCmd::SetDCEKI(int value,int node){
     char cmd[64];
    size_t len = snprintf(cmd, sizeof(cmd), "#SET_DCE_KI %d %d", value,node);
    return SendCmd(cmd);
}
bool DummyCmd::SetDCEKD(int value,int node){
     char cmd[64];
    size_t len = snprintf(cmd, sizeof(cmd), "#SET_DCE_KD %d %d", value,node);
    return SendCmd(cmd);
}