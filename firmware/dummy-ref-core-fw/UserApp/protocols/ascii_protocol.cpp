#include "common_inc.h"

extern DummyRobot dummy;
void handelCmd(const char* _cmd, size_t _len, StreamSink &_responseChannel) {

    uint8_t  i;
    /*---------------------------- ↓ Add Your CMDs Here ↓ -----------------------------*/
    if (_cmd[0] == '!' )
    {
        std::string s(_cmd);
        if (s.find("STOP") != std::string::npos)
        {
            dummy.commandHandler.EmergencyStop();
            Respond(_responseChannel, "Stopped ok");
        } else if (s.find("START") != std::string::npos)
        {
            dummy.SetEnable(true);
            Respond(_responseChannel, "Started ok");
        } else if (s.find("HOME") != std::string::npos)
        {
            dummy.Homing();
            Respond(_responseChannel, "HOME ok");
        } else if (s.find("CALIBRATION") != std::string::npos)
        {
            dummy.CalibrateHomeOffset();
            Respond(_responseChannel, "calibration ok");
        } else if (s.find("RESET") != std::string::npos)
        {
            dummy.Resting();
            Respond(_responseChannel, "RESET ok");
        } else if (s.find("DISABLE") != std::string::npos)
        {
            dummy.SetEnable(false);
            Respond(_responseChannel, "Disabled ok");
        }
    } else if (_cmd[0] == '#')
    {
        std::string s(_cmd);
        if (s.find("GETJOINTSLIMITA") != std::string::npos) {

            Respond(_responseChannel,"JLA %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f" ,
                dummy.motorJ[1]->angleLimitMin,dummy.motorJ[1]->angleLimitMax,
                dummy.motorJ[2]->angleLimitMin,dummy.motorJ[2]->angleLimitMax,
                dummy.motorJ[3]->angleLimitMin,dummy.motorJ[3]->angleLimitMax);
        } else if (s.find("GETJOINTSLIMITB") != std::string::npos) {
            Respond(_responseChannel,"JLB %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f" ,
                           dummy.motorJ[4]->angleLimitMin,dummy.motorJ[4]->angleLimitMax,
                           dummy.motorJ[5]->angleLimitMin,dummy.motorJ[5]->angleLimitMax,
                           dummy.motorJ[6]->angleLimitMin,dummy.motorJ[6]->angleLimitMax);
        } else if (s.find("SET_IP") != std::string::npos) {
            char ip[50];
            sscanf(_cmd,"#SET_IP %s",ip);
            dummy.SetIP(ip);
            Respond(_responseChannel, "SET_IP ok");
        }else if (s.find("SET_WIFI_MODE") != std::string::npos) {
            char ip[50];
            sscanf(_cmd,"#SET_WIFI_MODE %s",ip);
            dummy.SetWiFiMode(ip);
            Respond(_responseChannel, "SET_WIFI_MODE ok");
        }else if (s.find("AUTO_SEND_POS") != std::string::npos) {
            bool flag = false;
            sscanf(_cmd,"#AUTO_SEND_POS %lu",&flag);
            dummy.SetAutoSendPos(flag);
            Respond(_responseChannel,"AUTO_SEND_POS ok %d",flag);
        }
        else if (s.find("GETJPOS") != std::string::npos)
        {
            Respond(_responseChannel, "pos %.2f %.2f %.2f %.2f %.2f %.2f",
                    dummy.currentJoints.a[0], dummy.currentJoints.a[1],
                    dummy.currentJoints.a[2], dummy.currentJoints.a[3],
                    dummy.currentJoints.a[4], dummy.currentJoints.a[5]);
        } else if (s.find("SET_ENABLE_TEMP") != std::string::npos) {
            bool flag = false;
            sscanf(_cmd,"#SET_ENABLE_TEMP %s",&flag);
            for (int i = 1; i <=6 ;i++) {
                dummy.motorJ[i]->SetEnableTemp(flag);
            }
            Respond(_responseChannel,"SET_ENABLE_TEMP ok");
        }
        else if (s.find("GETLPOS") != std::string::npos)
        {
            dummy.UpdateJointPose6D();
            Respond(_responseChannel, "posl %.2f %.2f %.2f %.2f %.2f %.2f",
                    dummy.currentPose6D.X, dummy.currentPose6D.Y,
                    dummy.currentPose6D.Z, dummy.currentPose6D.A,
                    dummy.currentPose6D.B, dummy.currentPose6D.C);
        } else if (s.find("SET_DCE_KP") != std::string::npos)
        {
            uint32_t kp;
            uint32_t node;
            sscanf(_cmd, "#SET_DCE_KP %lu %lu", &node, &kp);
            if (node >= 1 & node <= 6){
                dummy.motorJ[node]->SetDceKp(kp);
                Respond(_responseChannel, "ok SET MOTOR [%lu] DCE_KP [%lu]", node, kp);
            }
            else {
                Respond(_responseChannel, "error SET MOTOR [%lu] DCE_KP [%lu] is wrong", node, kp);
            }
        } else if (s.find("SET_DCE_KI") != std::string::npos)
        {
            uint32_t kp;
            uint32_t node;
            sscanf(_cmd, "#SET_DCE_KI %lu %lu", &node, &kp);
            if (node >= 1 & node <= 6){
                dummy.motorJ[node]->SetDceKi(kp);
                Respond(_responseChannel, "ok SET MOTOR [%lu] DCE_KI [%lu]", node, kp);
            }
            else {
                Respond(_responseChannel, "error SET MOTOR [%lu] DCE_KI [%lu] is wrong", node, kp);
            }
        } else if (s.find("SET_DCE_KD") != std::string::npos)
        {
            uint32_t kp;
            uint32_t node;
            sscanf(_cmd, "#SET_DCE_KD %lu %lu", &node, &kp);
            if (node >= 1 & node <= 6){
                dummy.motorJ[node]->SetDceKd(kp);
                Respond(_responseChannel, "ok SET MOTOR [%lu] DCE_KD [%lu]", node, kp);
            }
            else {
                Respond(_responseChannel, "error SET MOTOR [%lu] DCE_KD [%lu] is wrong", node, kp);
            }
        } else if (s.find("SET_CURRENT") != std::string::npos) {
            float current;
            uint32_t node;
            sscanf(_cmd, "#SET_CURRENT %lu %lu", &node, &current);
            if (node >= 1 & node <= 6){
                dummy.motorJ[node]->SetCurrentLimit(current);
                Respond(_responseChannel, "ok SET MOTOR [%lu] Max Current [%lu]", node, current);
            }
            else {
                Respond(_responseChannel, "error SET MOTOR [%lu] Max Current [%lu] is wrong", node, current);
            }
        }
        else if (s.find("GETKP") != std::string::npos) {
            for(int i= 1;i <=6;i++) {
                 dummy.motorJ[i]->GetDceKp();
                osDelay(100);
            }

            Respond(_responseChannel, "KP %d %d %d %d %d %d ", dummy.motorJ[1]->kp, dummy.motorJ[2]->kp, dummy.motorJ[3]->kp,
                dummy.motorJ[4]->kp,dummy.motorJ[5]->kp,dummy.motorJ[6]->kp);
        }
        else if (s.find("GETKI") != std::string::npos) {
            for(int i= 1;i <=6;i++) {
                dummy.motorJ[i]->GetDceKi();
                osDelay(100);
            }

            Respond(_responseChannel, "KI %d %d %d %d %d %d ", dummy.motorJ[1]->ki, dummy.motorJ[2]->ki, dummy.motorJ[3]->ki,
                dummy.motorJ[4]->ki,dummy.motorJ[5]->ki,dummy.motorJ[6]->ki);
        }
        else if (s.find("GETKD") != std::string::npos) {
            for(int i= 1;i <=6;i++) {
                dummy.motorJ[i]->GetDceKd();
                osDelay(100);
            }

            Respond(_responseChannel, "KD %d %d %d %d %d %d ", dummy.motorJ[1]->kd, dummy.motorJ[2]->kd, dummy.motorJ[3]->kd,
                dummy.motorJ[4]->kd,dummy.motorJ[5]->kd,dummy.motorJ[6]->kd);
        }
        else if (s.find("GETTEMPERATURE") != std::string::npos) {
            Respond(_responseChannel, "tm %.2f %.2f %.2f %.2f %.2f %.2f ", dummy.motorJ[1]->temperature, dummy.motorJ[2]->temperature, dummy.motorJ[3]->temperature,
                           dummy.motorJ[4]->temperature,dummy.motorJ[5]->temperature,dummy.motorJ[6]->temperature);
        }
        else if (s.find("REBOOT") != std::string::npos)
        {
            uint32_t node;
            sscanf(_cmd, "#REBOOT %lu", &node);
            if (node >= 1 & node <= 6){
                dummy.motorJ[node]->Reboot();
                Respond(_responseChannel, "ok REBOOT MOTOR [%lu]", node);
            }
            else {
                Respond(_responseChannel, "error REBOOT MOTOR [%lu] is wrong", node);
            }
        }else if (s.find("CMDMODE") != std::string::npos)
        {
            uint32_t mode;
            sscanf(_cmd, "#CMDMODE %lu", &mode);
            dummy.SetCommandMode(mode);
            Respond(_responseChannel, "ok Set command mode to [%lu]", mode);
        } else
            Respond(_responseChannel, "ok#");
    } else if (_cmd[0] == '>' || _cmd[0] == '@' || _cmd[0] == '&')
    {
        uint32_t freeSize = dummy.commandHandler.Push(_cmd);
        Respond(_responseChannel, "%d", freeSize);
    }
}
void OnUsbAsciiCmd(const char* _cmd, size_t _len, StreamSink &_responseChannel)
{
    handelCmd(_cmd,_len,_responseChannel);
    return;
    uint8_t  i;
    /*---------------------------- ↓ Add Your CMDs Here ↓ -----------------------------*/
    if (_cmd[0] == '!' )
    {
        std::string s(_cmd);
        if (s.find("STOP") != std::string::npos)
        {
            dummy.commandHandler.EmergencyStop();
            Respond(_responseChannel, "Stopped ok");
        } else if (s.find("START") != std::string::npos)
        {
            dummy.SetEnable(true);
            Respond(_responseChannel, "Started ok");
        } else if (s.find("HOME") != std::string::npos)
        {
            dummy.Homing();
            Respond(_responseChannel, "HOME ok");
        } else if (s.find("CALIBRATION") != std::string::npos)
        {
            dummy.CalibrateHomeOffset();
            Respond(_responseChannel, "calibration ok");
        } else if (s.find("RESET") != std::string::npos)
        {
            dummy.Resting();
            Respond(_responseChannel, "RESET ok");
        } else if (s.find("DISABLE") != std::string::npos)
        {
            dummy.SetEnable(false);
            Respond(_responseChannel, "Disabled ok");
        }
    } else if (_cmd[0] == '#')
    {
        std::string s(_cmd);
        if (s.find("GETJOINTSLIMITA") != std::string::npos) {

            Respond(_responseChannel,"%0.2f %0.2f %0.2f %0.2f %0.2f %0.2f" ,
                dummy.motorJ[1]->angleLimitMin,dummy.motorJ[1]->angleLimitMax,
                dummy.motorJ[2]->angleLimitMin,dummy.motorJ[2]->angleLimitMax,
                dummy.motorJ[3]->angleLimitMin,dummy.motorJ[3]->angleLimitMax);
        } else if (s.find("GETJOINTSLIMITB") != std::string::npos) {
            Respond(_responseChannel,"%0.2f %0.2f %0.2f %0.2f %0.2f %0.2f" ,
                           dummy.motorJ[4]->angleLimitMin,dummy.motorJ[4]->angleLimitMax,
                           dummy.motorJ[5]->angleLimitMin,dummy.motorJ[5]->angleLimitMax,
                           dummy.motorJ[6]->angleLimitMin,dummy.motorJ[6]->angleLimitMax);
        } else if (s.find("SET_IP") != std::string::npos) {
            char ip[50];
            sscanf(_cmd,"#SET_IP %s",ip);
            dummy.SetIP(ip);
            Respond(_responseChannel, "ok");
        }
        else if (s.find("GETJPOS") != std::string::npos)
        {
            Respond(_responseChannel, "ok3 %.2f %.2f %.2f %.2f %.2f %.2f",
                    dummy.currentJoints.a[0], dummy.currentJoints.a[1],
                    dummy.currentJoints.a[2], dummy.currentJoints.a[3],
                    dummy.currentJoints.a[4], dummy.currentJoints.a[5]);
        } else if (s.find("GETLPOS") != std::string::npos)
        {
            dummy.UpdateJointPose6D();
            Respond(_responseChannel, "ok %.2f %.2f %.2f %.2f %.2f %.2f",
                    dummy.currentPose6D.X, dummy.currentPose6D.Y,
                    dummy.currentPose6D.Z, dummy.currentPose6D.A,
                    dummy.currentPose6D.B, dummy.currentPose6D.C);
        } else if (s.find("SET_DCE_KP") != std::string::npos)
        {
            uint32_t kp;
            uint32_t node;
            sscanf(_cmd, "#SET_DCE_KP %lu %lu", &node, &kp);
            if (node >= 1 & node <= 6){
                dummy.motorJ[node]->SetDceKp(kp);
                Respond(_responseChannel, "ok SET MOTOR [%lu] DCE_KP [%lu]", node, kp);
            }
            else {
                Respond(_responseChannel, "error SET MOTOR [%lu] DCE_KP [%lu] is wrong", node, kp);
            }
        } else if (s.find("SET_DCE_KI") != std::string::npos)
        {
            uint32_t kp;
            uint32_t node;
            sscanf(_cmd, "#SET_DCE_KI %lu %lu", &node, &kp);
            if (node >= 1 & node <= 6){
                dummy.motorJ[node]->SetDceKi(kp);
                Respond(_responseChannel, "ok SET MOTOR [%lu] DCE_KI [%lu]", node, kp);
            }
            else {
                Respond(_responseChannel, "error SET MOTOR [%lu] DCE_KI [%lu] is wrong", node, kp);
            }
        } else if (s.find("SET_DCE_KD") != std::string::npos)
        {
            uint32_t kp;
            uint32_t node;
            sscanf(_cmd, "#SET_DCE_KD %lu %lu", &node, &kp);
            if (node >= 1 & node <= 6){
                dummy.motorJ[node]->SetDceKd(kp);
                Respond(_responseChannel, "ok SET MOTOR [%lu] DCE_KD [%lu]", node, kp);
            }
            else {
                Respond(_responseChannel, "error SET MOTOR [%lu] DCE_KD [%lu] is wrong", node, kp);
            }
        } else if (s.find("REBOOT") != std::string::npos)
        {
            uint32_t node;
            sscanf(_cmd, "#REBOOT %lu", &node);
            if (node >= 1 & node <= 6){
                dummy.motorJ[node]->Reboot();
                Respond(_responseChannel, "ok REBOOT MOTOR [%lu]", node);
            }
            else {
                Respond(_responseChannel, "error REBOOT MOTOR [%lu] is wrong", node);
            }
        }else if (s.find("CMDMODE") != std::string::npos)
        {
            uint32_t mode;
            sscanf(_cmd, "#CMDMODE %lu", &mode);
            dummy.SetCommandMode(mode);
            Respond(_responseChannel, "ok Set command mode to [%lu]", mode);
        } else
            Respond(_responseChannel, "ok");
    } else if (_cmd[0] == '>' || _cmd[0] == '@' || _cmd[0] == '&')
    {
        uint32_t freeSize = dummy.commandHandler.Push(_cmd);
        Respond(_responseChannel, "%d", freeSize);
    }

/*---------------------------- ↑ Add Your CMDs Here ↑ -----------------------------*/
}


void OnUart4AsciiCmd(const char* _cmd, size_t _len, StreamSink &_responseChannel)
{
    handelCmd(_cmd,_len,_responseChannel);
    return;
    /*---------------------------- ↓ Add Your CMDs Here ↓ -----------------------------*/
    if (_cmd[0] == '!')
    {
        std::string s(_cmd);
        if (s.find("STOP") != std::string::npos)
        {
            dummy.commandHandler.EmergencyStop();
            Respond(_responseChannel, "Stopped ok");
        } else if (s.find("START") != std::string::npos)
        {
            dummy.SetEnable(true);
            Respond(_responseChannel, "Started ok");
        } else if (s.find("HOME") != std::string::npos)
        {
            dummy.Homing();
            Respond(_responseChannel, "Started ok");
        } else if (s.find("CALIBRATION") != std::string::npos)
        {
            dummy.CalibrateHomeOffset();
            Respond(_responseChannel, "calibration ok");
        } else if (s.find("RESET") != std::string::npos)
        {
            dummy.Resting();
            Respond(_responseChannel, "Started ok");
        } else if (s.find("DISABLE") != std::string::npos)
        {
            dummy.SetEnable(false);
            Respond(_responseChannel, "Disabled ok");
        }
    } else if (_cmd[0] == '#')
    {
        Respond(_responseChannel, "bb %s",_cmd+1);
        std::string s(_cmd);
        if (s.find("GETJOINTSLIMITA") != std::string::npos) {

            Respond(_responseChannel,"%0.2f %0.2f %0.2f %0.2f %0.2f %0.2f" ,
                dummy.motorJ[1]->angleLimitMin,dummy.motorJ[1]->angleLimitMax,
                dummy.motorJ[2]->angleLimitMin,dummy.motorJ[2]->angleLimitMax,
                dummy.motorJ[3]->angleLimitMin,dummy.motorJ[3]->angleLimitMax);
        } else if (s.find("GETJOINTSLIMITB") != std::string::npos) {
            Respond(_responseChannel,"%0.2f %0.2f %0.2f %0.2f %0.2f %0.2f" ,
                           dummy.motorJ[4]->angleLimitMin,dummy.motorJ[4]->angleLimitMax,
                           dummy.motorJ[5]->angleLimitMin,dummy.motorJ[5]->angleLimitMax,
                           dummy.motorJ[6]->angleLimitMin,dummy.motorJ[6]->angleLimitMax);
        } else if (s.find("SET_IP") != std::string::npos) {
            char ip[50];
            sscanf(_cmd,"#SET_IP %s",ip);
            dummy.SetIP(ip);
            Respond(_responseChannel, "ok");
        }
        else if (s.find("GETJPOS") != std::string::npos)
        {
            Respond(_responseChannel, "ok %.2f %.2f %.2f %.2f %.2f %.2f",
                    dummy.currentJoints.a[0], dummy.currentJoints.a[1],
                    dummy.currentJoints.a[2], dummy.currentJoints.a[3],
                    dummy.currentJoints.a[4], dummy.currentJoints.a[5]);
        } else if (s.find("GETLPOS") != std::string::npos)
        {
            dummy.UpdateJointPose6D();
            Respond(_responseChannel, "ok %.2f %.2f %.2f %.2f %.2f %.2f",
                    dummy.currentPose6D.X, dummy.currentPose6D.Y,
                    dummy.currentPose6D.Z, dummy.currentPose6D.A,
                    dummy.currentPose6D.B, dummy.currentPose6D.C);
        } else if (s.find("SET_DCE_KP") != std::string::npos)
        {
            uint32_t kp;
            uint32_t node;
            sscanf(_cmd, "#SET_DCE_KP %lu %lu", &node, &kp);
            if (node >= 1 & node <= 6){
                dummy.motorJ[node]->SetDceKp(kp);
                Respond(_responseChannel, "ok SET MOTOR [%lu] DCE_KP [%lu]", node, kp);
            }
            else {
                Respond(_responseChannel, "error SET MOTOR [%lu] DCE_KP [%lu] is wrong", node, kp);
            }
        } else if (s.find("SET_DCE_KI") != std::string::npos)
        {
            uint32_t kp;
            uint32_t node;
            sscanf(_cmd, "#SET_DCE_KI %lu %lu", &node, &kp);
            if (node >= 1 & node <= 6){
                dummy.motorJ[node]->SetDceKi(kp);
                Respond(_responseChannel, "ok SET MOTOR [%lu] DCE_KI [%lu]", node, kp);
            }
            else {
                Respond(_responseChannel, "error SET MOTOR [%lu] DCE_KI [%lu] is wrong", node, kp);
            }
        } else if (s.find("SET_DCE_KD") != std::string::npos)
        {
            uint32_t kp;
            uint32_t node;
            sscanf(_cmd, "#SET_DCE_KD %lu %lu", &node, &kp);
            if (node >= 1 & node <= 6){
                dummy.motorJ[node]->SetDceKd(kp);
                Respond(_responseChannel, "ok SET MOTOR [%lu] DCE_KD [%lu]", node, kp);
            }
            else {
                Respond(_responseChannel, "error SET MOTOR [%lu] DCE_KD [%lu] is wrong", node, kp);
            }
        } else if (s.find("REBOOT") != std::string::npos)
        {
            uint32_t node;
            sscanf(_cmd, "#REBOOT %lu", &node);
            if (node >= 1 & node <= 6){
                dummy.motorJ[node]->Reboot();
                Respond(_responseChannel, "ok REBOOT MOTOR [%lu]", node);
            }
            else {
                Respond(_responseChannel, "error REBOOT MOTOR [%lu] is wrong", node);
            }
        }else if (s.find("CMDMODE") != std::string::npos)
        {
            uint32_t mode;
            sscanf(_cmd, "#CMDMODE %lu", &mode);
            dummy.SetCommandMode(mode);
            Respond(_responseChannel, "Set command mode to [%lu]", mode);
        } else
            Respond(_responseChannel, "ok");
    } else if (_cmd[0] == '>' || _cmd[0] == '@' || _cmd[0] == '&')
    {
        uint32_t freeSize = dummy.commandHandler.Push(_cmd);
        Respond(_responseChannel, "%d", freeSize);
    }

/*---------------------------- ↑ Add Your CMDs Here ↑ -----------------------------*/
}


void OnUart5AsciiCmd(const char* _cmd, size_t _len, StreamSink &_responseChannel)
{
    /*---------------------------- ↓ Add Your CMDs Here ↓ -----------------------------*/

/*---------------------------- ↑ Add Your CMDs Here ↑ -----------------------------*/
}
