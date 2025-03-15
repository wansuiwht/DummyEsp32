#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>
#include "secrets.h"  // add WLAN Credentials in here.
#include <FS.h>       // File System for Web Server Files
#include <LittleFS.h> // This file system is used.
#include <Preferences.h>
#include "AsyncUDP.h"
#include "dummyHttpCmdHandler.h"
#include "dummyUdpCmdHandler.h"
#include "dummyCmd.h"
#include "WebSocketsServer.h"
// ros
#include <micro_ros_platformio.h>
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <micro_ros_utilities/type_utilities.h>
#include <micro_ros_utilities/string_utilities.h>
#include <std_msgs/msg/int32.h>
#include <std_msgs/msg/string.h>
#include <sstream>
// 用于保存wifi信息
Preferences preferences;
// Udp服务端
AsyncUDP udp;
// Websocket服务器
WebSocketsServer webSocket = WebSocketsServer(81);
// WebServer服务器
WebServer server(80);
// UDP CMD
DummyUdpCmdHandler udpCmd;
DummyCmd dummyCmd;
bool useJoystick = false;

struct JointLimit {
  float min = 0;
  float max = 0;
};
String ssid;
String pass;
float jointsPos[6]{0};
float jointsCmd[6]{0};
JointLimit jointLimit[6];

IPAddress ip;
String agent_ip;
uint16_t agent_port;
rcl_publisher_t publisher;
rcl_subscription_t subscriber;
std_msgs__msg__Int32 msg;
std_msgs__msg__String strMsg, strMsg2;
rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
rcl_timer_t timer;

// mark parameters not used
#define UNUSED __attribute__((unused))

// TRACE output simplified, can be deactivated here
#define TRACE(...) Serial.printf(__VA_ARGS__)

// name of the server. You reach it using http://webserver
#define HOSTNAME "webserver"

// local time zone definition (Berlin)
#define TIMEZONE "CET-1CEST,M3.5.0,M10.5.0/3"

// The text of builtin files are in this header file
#include "builtinfiles.h"
/******************************ROS2************************************************************ */
// enable the CUSTOM_ETAG_CALC to enable calculation of ETags by a custom function
#define CUSTOM_ETAG_CALC

#define RCCHECK(fn)              \
  {                              \
    rcl_ret_t temp_rc = fn;      \
    if ((temp_rc != RCL_RET_OK)) \
    {                            \
      error_loop();              \
    }                            \
  }
#define RCSOFTCHECK(fn)          \
  {                              \
    rcl_ret_t temp_rc = fn;      \
    if ((temp_rc != RCL_RET_OK)) \
    {                            \
    }                            \
  }

// Error handle loop
void error_loop()
{
  while (1)
  {
    delay(100);
  }
}

void timer_callback(rcl_timer_t *timer, int64_t last_call_time)
{
  RCLC_UNUSED(last_call_time);
  if (timer != NULL)
  {
    RCSOFTCHECK(rcl_publish(&publisher, &strMsg, NULL));
  }
}

void subscription_callback(const void *msgin)
{
  const std_msgs__msg__String *msg = (const std_msgs__msg__String *)msgin;
  // micro_ros_string_utilities_get_c_str(msg->data);
  Serial.println(micro_ros_string_utilities_get_c_str(msg->data));
  Serial.println("lll");
}
//
void StartRosNode(){
   
  agent_ip = preferences.getString("agent_ip", "");
  agent_port = preferences.getShort("agent_port", 2018);
  agent_port = 2018;
  ip.fromString(agent_ip);
  set_microros_transports(ip, agent_port);
  Serial.println("uset_microros_transports compeled");
  allocator = rcl_get_default_allocator();
  // create init_options
  rclc_support_init(&support, 0, NULL, &allocator);
  // create node
  RCCHECK(rclc_node_init_default(&node, "dummy_ros_node", "", &support));
  // create publisher
  RCCHECK(rclc_publisher_init_default(
      &publisher,
      &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
      "dummy_ros_node_publisher"));
  micro_ros_utilities_create_message_memory(
      ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
      &strMsg,
      (micro_ros_utilities_memory_conf_t){});
  micro_ros_utilities_create_message_memory(
      ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
      &strMsg2,
      (micro_ros_utilities_memory_conf_t){});
  // create subscriber
  RCCHECK(rclc_subscription_init_default(
      &subscriber,
      &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
      "dummy_ros_node_subscriber"));
  // create executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 2, &allocator));
  RCCHECK(rclc_executor_add_timer(&executor, &timer));
  Serial.println(rclc_executor_add_subscription(&executor, &subscriber, &strMsg2, &subscription_callback, ON_NEW_DATA));
  msg.data = 0;
  strMsg.data = micro_ros_string_utilities_set(strMsg.data, "Dummy Started!");

  // create timer,
  const unsigned int timer_timeout = 1000;
  RCCHECK(rclc_timer_init_default(
      &timer,
      &support,
      RCL_MS_TO_NS(timer_timeout),
      timer_callback));
}
/****************************************************************************** */
/********************Web Server*********************************************** */
wifi_mode_t wifiMode = WIFI_STA;
// 重定向
void handleRedirect()
{
  TRACE("Redirect...\n");
  if (wifiMode == WIFI_AP)
  {
    server.send(200, "text/html", FPSTR(setWiFi));
    return;
  }
  String url = "/index.htm";
  server.sendHeader("Location", url, true);
  server.send(302);
}
// 设置Wifi
void handleSetWifi()
{
  TRACE("set wifi");
  preferences.putString("ssid", server.arg("ssid"));
  preferences.putString("pass", server.arg("pass"));
  Serial.println("Wifi 设置成功");
  String result;

  result += "{\n";
  result += "  \"code\":0 ,\n";
  result += "  \"msg\": 设置成功,系统重启\n";
  result += "}";

  server.sendHeader("Cache-Control", "no-cache");
  server.send(200, "text/javascript; charset=utf-8", result);
  ESP.restart();
}

void handleStartRosNode(){
  preferences.putString("agent_ip", server.arg("agent_ip"));
  preferences.putShort("agent_port", atoi(server.arg("agent_port").c_str()));
  Serial.println("Wifi 设置成功");
  String result;

  result += "{\n";
  result += "  \"code\":0 ,\n";
  result += "  \"msg\": 设置成功.\n";
  result += "}";
  server.sendHeader("Cache-Control", "no-cache");
  server.send(200, "text/javascript; charset=utf-8", result);
  StartRosNode();
}
// 返回系统信息
void handleSysInfo()
{
  String result;
  String ros_ip = preferences.getString("agent_ip", "");
  int16_t ros_port = preferences.getShort("agent_port", 2018);
  result += "{\n";
  result += "\"code\":0 \n,";
  result += ("\"ssid\":\"" + ssid + "\"\n,");
  result += "\"password\":\"" + pass + "\"\n,";
  result += ("\"ip\":\"" + WiFi.localIP().toString() + "\"\n,");
  result += "\"udpPort\":9999\n,";
  result += ("\"rosAgentIp\": \""+ ros_ip + "\"\n,");
  result += ("\"rosAgentPort\": " + String(ros_port) + "\n");
  result += "}";

  server.sendHeader("Cache-Control", "no-cache");
  //server.send(200, "text/javascript; charset=utf-8", result);
  server.send(200, "application/json; charset=utf-8", result);
}
void InitWebServer(){
// Ask for the current time using NTP request builtin into ESP firmware.
  TRACE("Setup ntp...\n");
  configTzTime(TIMEZONE, "pool.ntp.org");

  TRACE("Register redirect...\n");
  // register a redirect handler when only domain name is given.
  server.on("/", HTTP_GET, handleRedirect);
  server.on("/setWifi", HTTP_GET, handleSetWifi);
  server.on("/getSysInfo", HTTP_GET, handleSysInfo);
  server.on("/startRosNode",HTTP_GET,handleStartRosNode);
  TRACE("Register service handlers...\n");
  Serial.println("ros init completed");
  // serve a built-in htm page
  server.on("/index.htm", []()
            { server.send(200, "text/html", FPSTR(uploadContent)); });

  TRACE("Register file system handlers...\n");

  server.addHandler(new DummyHttpCmdHandler());
  // // enable CORS header in webserver results
  server.enableCORS(true);

  TRACE("Register default (not found) answer...\n");

  // handle cases when file is not found
  server.onNotFound([]()
                    {
    // standard not found in browser.
  server.send(404, "text/html", FPSTR(notFoundContent)); });
  server.begin();
}
/***************************WIFI****************************************************************** */
IPAddress apIp;
// 初始化Wifi
void WiFiInit()
{
  WiFi.setHostname(HOSTNAME);
  // 读取保存数据

  // 读取wifi 的ssid
  ssid = preferences.getString("ssid", "");
  // 初始状态，没有设置wifi
  if (ssid == "")
  {
    wifiMode = WIFI_AP;

    ssid = "DummyEsp32";
    WiFi.mode(WIFI_AP);
    if (!WiFi.softAP(ssid))
    {
      log_e("热点创建失败.");
      // 创建失败了，重启ESP
      // ESP.restart();
    }
    apIp = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(apIp);
    String mode = "#SET_WIFI_MODE AP";
    Serial1.println(mode.c_str());
    String ip = "#SET_IP " + apIp.toString();
    Serial1.println(ip.c_str());
  }
  else
  {
    wifiMode = WIFI_STA;
    pass = preferences.getString("pass", "");
    TRACE("ssid:%s  pass:%s\n", ssid, pass);
    WiFi.mode(WIFI_STA);
    // 启动WiFi
    WiFi.begin(ssid, pass);
    // 连接到Wifi
    TRACE("Connect to WiFi...\n");
    // 连接失败记数
    int failureCount = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(1000);
      failureCount++;
      TRACE(".");
      // 如果越过30秒还没有连接上，则认为这个wifi不可用，将其清除，并重启。再重启时就使用了AP模式，
      // 可以用手机连接后，重新设置wifi信息。
      if (failureCount > 30)
      {
        TRACE("超过连接时间，该wifi不可用，系统将以AP模式重启\n");
        preferences.putString("ssid", "");
        preferences.end();
        ESP.restart();
      }
    }
    String ip = "#SET_IP " + WiFi.localIP().toString();
    String mode = "#SET_WIFI_MODE STA";
    Serial1.println(mode.c_str());
    Serial.println(ip.c_str());
    Serial1.write(ip.c_str(), ip.length());
    Serial1.println();
    TRACE("connected.\n");
  }
}
/********************************Udp********************************************************/
void UdpInit()
{
  if (udp.listen(9999))
  {
    udpCmd.SetJoints(jointsCmd);
    udp.onPacket([](AsyncUDPPacket packet)
                 {
                  udpCmd.ParseCmd(packet.data(),packet.length());
                  // //转发给STM32
                  // Serial1.write(packet.data(),packet.length());
                  // Serial1.println();
                  // //print debug info
                  // Serial.write(packet.data(),packet.length());
                  // packet.print("sent to Stm32"); });
                 });
  }
}
/*******************************************************************************************/
/******************************Websocket************************************************/
void hexdump(const void *mem, uint32_t len, uint8_t cols = 16)
{
  const uint8_t *src = (const uint8_t *)mem;
  Serial.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
  for (uint32_t i = 0; i < len; i++)
  {
    if (i % cols == 0)
    {
      Serial.printf("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
    }
    Serial.printf("%02X ", *src);
    src++;
  }
  Serial.printf("\n");
}
uint8_t websocketClientId = 0;
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{

  switch (type)
  {
  case WStype_DISCONNECTED:
    Serial.printf("[%u] Disconnected!\n", num);
    break;
  case WStype_CONNECTED:
  {
    websocketClientId = num;
    IPAddress ip = webSocket.remoteIP(num);
    Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

    // send message to client
    webSocket.sendTXT(num, "Connected");
  }
  break;
  case WStype_TEXT:
    Serial.printf("[%u] get Text: %s\n", num, payload);

    // send message to client
    // webSocket.sendTXT(num, "message here");

    // send data to all connected clients
    // webSocket.broadcastTXT("message here");
    break;
  case WStype_BIN:
    Serial.printf("[%u] get binary length: %u\n", num, length);
    hexdump(payload, length);

    // send message to client
    // webSocket.sendBIN(num, payload, length);
    break;
  case WStype_ERROR:
  case WStype_FRAGMENT_TEXT_START:
  case WStype_FRAGMENT_BIN_START:
  case WStype_FRAGMENT:
  case WStype_FRAGMENT_FIN:
    break;
  }
}
void setJointsLimit(String s,int i){
  Serial.println(s);
  std::istringstream iss(s.c_str());
  std::string token;
  while (iss >> token) {  // 自动跳过空格并提取单词
    jointLimit[i].min = std::stof(token);
    iss >> token;
    jointLimit[i].max = std::stof(token);
    i++;
  }
}
/************************************************************************************************ */
// STM32串口回调函数
void onSerial2Receive()
{
  size_t available = Serial1.available();
  String read;
  while (available--)
  {
    char c = Serial1.read();
    read += c;
    if (c == '\n')
    {
      //recode pos
      if (read.startsWith("pos")){
        std::istringstream iss(read.substring(3).c_str());
        std::string token;
        int i = 0;
        while (iss >> token) {  // 自动跳过空格并提取单词
          jointsPos[i] = std::stof(token);
          i++;
        }
      }
    
      webSocket.sendTXT(websocketClientId, read);
      Serial.println(read);
      read = "";
    }
  }
    //remember limit
    if (read.startsWith("JLA")){
      String v = read.substring(3);
      setJointsLimit(v,0);
    }
    if (read.startsWith("JLB")){
      String v = read.substring(3);
      setJointsLimit(v,3);
    }
  webSocket.sendTXT(websocketClientId, read);
  Serial.println(read);
}
// Setup everything to make the webserver work.
void setup(void)
{
  delay(3000); // wait for serial monitor to start completely.
  TRACE("Starting WebServer\n");
  // 调试信息串口
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  // 与STM32通信
  Serial1.begin(115200, SERIAL_8N1, 27, 26);
  Serial1.onReceive(onSerial2Receive, false);
  // 开启存储
  preferences.begin("Dummy", false);
  TRACE("Starting WebServer\n");
  WiFiInit();
  UdpInit();
  InitWebServer();
  
  // websocket初始化
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

 
  Serial.println("usupport compeled");
  Serial.println("ros init completed");
  
  //UdpInit();
  TRACE("open <http://%s> or <http://%s>\n", WiFi.getHostname(), WiFi.localIP().toString().c_str());
  dummyCmd.GetJLA();
  dummyCmd.GetJLB();
  dummyCmd.GetJPos();
} // setup
int sendCmdCount = 0;
void checkJoystickCmd(){
  if (sendCmdCount< 50){
    sendCmdCount++;
    return ;
  }
  sendCmdCount = 0;
  if (jointsCmd[0] != 0 ||
      jointsCmd[1] != 0 ||
      jointsCmd[2] != 0 ||
      jointsCmd[3] != 0 ||
      jointsCmd[4] != 0 ||
      jointsCmd[5] != 0 
    ){
     
      std::string c(">");
      jointsPos[0] +=  jointsCmd[0];
      jointsPos[0] = jointsPos[0] > jointLimit[0].max ? jointLimit[0].max : jointsPos[0];
      jointsPos[0] = jointsPos[0] <jointLimit[0].min ? jointLimit[0].min : jointsPos[0];

      jointsPos[1] +=  jointsCmd[1];
      jointsPos[1] = jointsPos[1] > jointLimit[1].max ? jointLimit[1].max : jointsPos[1];
      jointsPos[1] = jointsPos[1] <jointLimit[1].min ? jointLimit[1].min : jointsPos[1];
      
      jointsPos[2] +=  jointsCmd[2];
      jointsPos[2] = jointsPos[2] > jointLimit[2].max ? jointLimit[2].max : jointsPos[2];
      jointsPos[2] = jointsPos[2] <jointLimit[2].min ? jointLimit[2].min : jointsPos[2];

      jointsPos[3] +=  jointsCmd[3];
      jointsPos[3] = jointsPos[3] > jointLimit[3].max ? jointLimit[3].max : jointsPos[3];
      jointsPos[3] = jointsPos[3] <jointLimit[3].min ? jointLimit[3].min : jointsPos[3];

      jointsPos[4] +=  jointsCmd[4];
      jointsPos[4] = jointsPos[4] > jointLimit[4].max ? jointLimit[4].max : jointsPos[4];
      jointsPos[4] = jointsPos[4] <jointLimit[4].min ? jointLimit[4].min : jointsPos[4];

      jointsPos[5] +=  jointsCmd[5];
      jointsPos[5] = jointsPos[5] > jointLimit[5].max ? jointLimit[5].max : jointsPos[5];
      jointsPos[5] = jointsPos[5] <jointLimit[5].min ? jointLimit[5].min : jointsPos[5];
      c.append(std::to_string(jointsPos[0]));
      c.append(",");
      c.append(std::to_string(jointsPos[1]));
      c.append(",");
      c.append(std::to_string(jointsPos[2]));
      c.append(",");
      c.append(std::to_string(jointsPos[3]));
      c.append(",");
      c.append(std::to_string(jointsPos[4]));
      c.append(",");
      c.append(std::to_string(jointsPos[5]));
      c.append(",80");
     
      Serial.println(c.c_str());
      dummyCmd.ExecCmd(c.c_str());
    }
}
void loop(void)
{
  server.handleClient();
  webSocket.loop();
  RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
  checkJoystickCmd();
} // loop

// end.