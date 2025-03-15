#include "dummyUdpCmdHandler.h"
#include<Arduino.h>
#include <string>
#define COROTATION 2
#define REVERSAL -2
#define STOP 0
DummyUdpCmdHandler::DummyUdpCmdHandler(/* args */)
{
}

DummyUdpCmdHandler::~DummyUdpCmdHandler()
{
}
void DummyUdpCmdHandler::ParseCmd(const uint8_t *buffer, size_t size){
    String cmd(buffer,size);
    if (cmd.equals("MenuL Pushed")){
        dummyCmd.Home();
        Serial.write("MenuL");
    }
    if (cmd.equals("MenuR Pushed")){
        dummyCmd.Reset();
        Serial.write("MenuR Pushed");
    }
    if (cmd.equals("ActionH Pushed")){
        dummyCmd.Start();
        Serial.write("Start");
    }
    if (cmd.equals("ActionV Pushed")){
        dummyCmd.Stop();
        Serial.write("Stop");
    }
    if (cmd.equals("ActionA Pushed")){

    }
    if (cmd.equals("ActionB Pushed")){

    }
    //ctrl J1 
    if (cmd.startsWith("JoyX")){
        double v = cmd.substring(cmd.indexOf(' ')).toDouble();
        if (v > 0){
            this->joints[0]  = REVERSAL;
        }else if (v < 0)
            this->joints[0]  = COROTATION ;
        else 
            this->joints[0]  = STOP;
        Serial.println(v);
    }
    //ctrl J2
    if (cmd.startsWith("JoyY")){
        double v = cmd.substring(cmd.indexOf(' ')).toDouble();
        if (v > 0){
            this->joints[1]  = REVERSAL;
        }else if (v < 0)
            this->joints[1]  = COROTATION ;
        else 
            this->joints[1]  = STOP;
        Serial.println(v);
    }
    //ctrl J4
    if (cmd.startsWith("CamX")){
        double v = cmd.substring(cmd.indexOf(' ')).toDouble();
        if (v > 0){
            this->joints[3]  = REVERSAL;
        }else if (v < 0)
            this->joints[3]  = COROTATION ;
        else 
            this->joints[3]  = STOP;
        Serial.println(v);
    }
    //ctrl J3
    if (cmd.startsWith("CamY")){
        double v = cmd.substring(cmd.indexOf(' ')).toDouble();
        if (v > 0){
            this->joints[2]  = REVERSAL;
        }else if (v < 0)
            this->joints[2]  = COROTATION ;
        else 
            this->joints[2]  = STOP;
        Serial.println(v);
    }
    //ctrl J5 up
    if (cmd.startsWith("TriggerL")){
        double v = cmd.substring(cmd.indexOf(' ')).toDouble();
        if (v > 0){
            this->joints[4]  = REVERSAL;
        }else if (v < 0)
            this->joints[4]  = COROTATION ;
        else 
            this->joints[4]  = STOP;
        Serial.println(v);
    }
    //ctrl J5 down
    if (cmd.startsWith("TriggerR")){
        double v = cmd.substring(cmd.indexOf(' ')).toDouble();
        if (v > 0){
            this->joints[4]  = COROTATION ;
        }else if (v < 0)
            this->joints[4]  = REVERSAL;
        else 
            this->joints[4]  = STOP;
        Serial.println(v);
    }
}

void DummyUdpCmdHandler::SetJoints(float * joints){
    this->joints = joints;
}