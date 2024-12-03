// #include <Arduino.h>
// #include <WebServer.h>
// #include <WiFi.h>
// #include "secrets.h"  // add WLAN Credentials in here.
// #include <FS.h>       // File System for Web Server Files
// #include <LittleFS.h> // This file system is used.
// #include <Preferences.h>
// #include "AsyncWebSocket.h"
// #include "AsyncUDP.h"
// #include "dummyHttpCmdHandler.h"
// #include "WebSocketsServer.h"
// // 用于保存wifi信息
// Preferences preferences;
// // Udp服务端
// AsyncUDP udp;
// // Websocket服务器
// WebSocketsServer webSocket = WebSocketsServer(81);
// // WebServer服务器
// WebServer server(80);

// String ssid;
// String pass;
// // mark parameters not used
// #define UNUSED __attribute__((unused))

// // TRACE output simplified, can be deactivated here
// #define TRACE(...) Serial.printf(__VA_ARGS__)

// // name of the server. You reach it using http://webserver
// #define HOSTNAME "webserver"

// // local time zone definition (Berlin)
// #define TIMEZONE "CET-1CEST,M3.5.0,M10.5.0/3"

// // The text of builtin files are in this header file
// #include "builtinfiles.h"

// // enable the CUSTOM_ETAG_CALC to enable calculation of ETags by a custom function
// #define CUSTOM_ETAG_CALC

// wifi_mode_t wifiMode = WIFI_STA;
// // 重定向
// void handleRedirect()
// {
//   TRACE("Redirect...\n");
//   if (wifiMode == WIFI_AP)
//   {
//     server.send(200, "text/html", FPSTR(setWiFi));
//     return;
//   }
//   String url = "/index.htm";
//   server.sendHeader("Location", url, true);
//   server.send(302);
// }
// // 设置Wifi
// void handleSetWifi()
// {
//   TRACE("set wifi");
//   preferences.putString("ssid", server.arg("ssid"));
//   preferences.putString("pass", server.arg("pass"));
//   Serial.println("Wifi 设置成功");
//   String result;

//   result += "{\n";
//   result += "  \"code\":0 ,\n";
//   result += "  \"msg\": 设置成功,系统重启\n";
//   result += "}";

//   server.sendHeader("Cache-Control", "no-cache");
//   server.send(200, "text/javascript; charset=utf-8", result);
//   ESP.restart();
// }
// // 返回系统信息
// void handleSysInfo()
// {
//   String result;

//   result += "{\n";
//   result += "\"code\":0 \n,";
//   result += ("\"ssid\":\"" + ssid + "\"\n,");
//   result += "\"password\":\"" + pass + "\"\n,";
//   result += ("\"ip\":\"" + WiFi.localIP().toString() + "\"\n,");
//   result += "\"udpPort\":9999\n";
//   result += "}";

//   server.sendHeader("Cache-Control", "no-cache");
//   server.send(200, "text/javascript; charset=utf-8", result);
// }

// IPAddress apIp;
// // 初始化Wifi
// void WiFiInit()
// {
//   WiFi.setHostname(HOSTNAME);
//   // 读取保存数据

//   // 读取wifi 的ssid
//   ssid = preferences.getString("ssid", "");
//   // 初始状态，没有设置wifi
//   if (ssid == "")
//   {
//     wifiMode = WIFI_AP;

//     ssid = "DummyEsp32";
//     WiFi.mode(WIFI_AP);
//     if (!WiFi.softAP(ssid))
//     {
//       log_e("热点创建失败.");
//       // 创建失败了，重启ESP
//       // ESP.restart();
//     }
//     apIp = WiFi.softAPIP();
//     Serial.print("AP IP address: ");
//     Serial.println(apIp);
//     String mode = "#SET_WIFI_MODE AP";
//     Serial1.println(mode.c_str());
//     String ip = "#SET_IP " + apIp.toString();
//     Serial1.println(ip.c_str());
//   }
//   else
//   {
//     pass = preferences.getString("pass", "");
//     TRACE("ssid:%s  pass:%s\n", ssid, pass);
//     WiFi.mode(WIFI_STA);
//     // 启动WiFi
//     WiFi.begin(ssid, pass);
//     // 连接到Wifi
//     TRACE("Connect to WiFi...\n");
//     // 连接失败记数
//     int failureCount = 0;
//     while (WiFi.status() != WL_CONNECTED)
//     {
//       delay(1000);
//       failureCount++;
//       TRACE(".");
//       // 如果越过30秒还没有连接上，则认为这个wifi不可用，将其清除，并重启。再重启时就使用了AP模式，
//       // 可以用手机连接后，重新设置wifi信息。
//       if (failureCount > 30)
//       {
//         TRACE("超过连接时间，该wifi不可用，系统将以AP模式重启\n");
//         preferences.putString("ssid", "");
//         preferences.end();
//         ESP.restart();
//       }
//     }
//     String ip = "#SET_IP " + WiFi.localIP().toString();
//     String mode = "#SET_WIFI_MODE STA";
//     Serial1.println(mode.c_str());
//     Serial.println(ip.c_str());
//     Serial1.write(ip.c_str(), ip.length());
//     Serial1.println();
//     TRACE("connected.\n");
//   }
// }
// void UdpInit()
// {
//   if (udp.listen(9999))
//   {
//     udp.onPacket([](AsyncUDPPacket packet)
//                  {
//                   //转发给STM32
//                   Serial2.write(packet.data(),packet.length());
//                   Serial2.println();
//                   packet.print("sent to Stm32"); });
//   }
// }
// void hexdump(const void *mem, uint32_t len, uint8_t cols = 16)
// {
//   const uint8_t *src = (const uint8_t *)mem;
//   Serial.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
//   for (uint32_t i = 0; i < len; i++)
//   {
//     if (i % cols == 0)
//     {
//       Serial.printf("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
//     }
//     Serial.printf("%02X ", *src);
//     src++;
//   }
//   Serial.printf("\n");
// }
// uint8_t websocketClientId = 0;
// void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
// {

//   switch (type)
//   {
//   case WStype_DISCONNECTED:
//     Serial.printf("[%u] Disconnected!\n", num);
//     break;
//   case WStype_CONNECTED:
//   {
//     websocketClientId = num;
//     IPAddress ip = webSocket.remoteIP(num);
//     Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

//     // send message to client
//     webSocket.sendTXT(num, "Connected");
//   }
//   break;
//   case WStype_TEXT:
//     Serial.printf("[%u] get Text: %s\n", num, payload);

//     // send message to client
//     // webSocket.sendTXT(num, "message here");

//     // send data to all connected clients
//     // webSocket.broadcastTXT("message here");
//     break;
//   case WStype_BIN:
//     Serial.printf("[%u] get binary length: %u\n", num, length);
//     hexdump(payload, length);

//     // send message to client
//     // webSocket.sendBIN(num, payload, length);
//     break;
//   case WStype_ERROR:
//   case WStype_FRAGMENT_TEXT_START:
//   case WStype_FRAGMENT_BIN_START:
//   case WStype_FRAGMENT:
//   case WStype_FRAGMENT_FIN:
//     break;
//   }
// }
// // STM32串口回调函数
// void onSerial2Receive()
// {
//   size_t available = Serial1.available();
//   String read;
//   while (available --) {
//     char c = Serial1.read();
//     read += c;
//     if (c == '\n'){
//       webSocket.sendTXT(websocketClientId,read);
//        Serial.println(read);
//        read="";
//     }
    
//   }
//    webSocket.sendTXT(websocketClientId,read);
//        Serial.println(read);
// }
// // Setup everything to make the webserver work.
// void setup(void)
// {
//   delay(3000); // wait for serial monitor to start completely.
//   TRACE("Starting WebServer\n");
//   // 调试信息串口
//   Serial.begin(115200);
//   Serial.setDebugOutput(false);
//   // 与STM32通信
//   Serial1.begin(115200, SERIAL_8N1, 27, 26);
//  Serial1.onReceive(onSerial2Receive, false);
//   // 开启存储
//   preferences.begin("Dummy", false);
//   TRACE("Starting WebServer\n");
//   WiFiInit();
//   UdpInit();

//   // Ask for the current time using NTP request builtin into ESP firmware.
//   TRACE("Setup ntp...\n");
//   configTzTime(TIMEZONE, "pool.ntp.org");

//   TRACE("Register redirect...\n");

//   // register a redirect handler when only domain name is given.
//   server.on("/", HTTP_GET, handleRedirect);
//   server.on("/setWifi", HTTP_GET, handleSetWifi);
//   server.on("/getSysInfo",HTTP_GET,handleSysInfo);
//   TRACE("Register service handlers...\n");

//   // serve a built-in htm page
//   server.on("/index.htm", []()
//             { server.send(200, "text/html", FPSTR(uploadContent)); });

//   TRACE("Register file system handlers...\n");

//   server.addHandler(new DummyHttpCmdHandler());
//   // // enable CORS header in webserver results
//   server.enableCORS(true);

//   TRACE("Register default (not found) answer...\n");

//   // handle cases when file is not found
//   server.onNotFound([]()
//                     {
//     // standard not found in browser.
//   server.send(404, "text/html", FPSTR(notFoundContent)); });

//   server.begin();
//   // websocket初始化
//   webSocket.begin();
//   webSocket.onEvent(webSocketEvent);

//   TRACE("open <http://%s> or <http://%s>\n", WiFi.getHostname(), WiFi.localIP().toString().c_str());
// } // setup

// void loop(void)
// {
//   server.handleClient();
//   webSocket.loop();
// } // loop

// // end.
// #include <Arduino.h>
// #include <micro_ros_platformio.h>
// #include <WiFi.h>
// #include <rcl/rcl.h>
// #include <rclc/rclc.h>
// #include <rclc/executor.h>
// #include <std_msgs/msg/string.h>

// //执行器,用于驱动节点的事件循环和定时器。
// rclc_executor_t executor;
// //支持结构体,用于存储分配器和上下文。
// rclc_support_t support;
// //分配器,用于分配内存。
// rcl_allocator_t allocator;
// //节点,用于存储节点相关信息。
// rcl_node_t node;
// //发布者,用于发布消息。
// rcl_publisher_t publisher;
// //定时器,用于定时发布消息。
// rcl_timer_t timer;
// //消息,用于存储消息数据。
// std_msgs__msg__String msg;


// /* 
//  * timer_callback - 定时器回调函数 
//  * @timer: 定时器结构体指针 
//  * @last_call_time: 上一次回调执行时间 
//  */ 
// void timer_callback(rcl_timer_t * timer, int64_t last_call_time)
// {
//     static int times; // 静态变量记录回调执行次数
//     RCLC_UNUSED(last_call_time); // 防止编译器警告

//     if (timer != NULL) // 判空处理
//     {
//         rcl_publish(&publisher, &msg, NULL); // 使用发布者发布消息

//         // 分配内存与消息初始化
//         std_msgs__msg__String__init(&msg); // 初始化消息结构体
//         msg.data.data = (char*)malloc(20); // 分配20字节内存，用于存储消息数据
//         msg.data.capacity = 20; // 设置消息的缓冲区大小为20字节
//         snprintf(msg.data.data, msg.data.capacity,("Running times:"+std::to_string(times)).c_str()); // 生成消息的数据内容
//         msg.data.size = strlen(msg.data.data); // 设置消息的实际大小

//         times+=1; // 将回调执行次数加1，用于下一次生成消息内容时使用
//     }
// }



// void setup()
// {
//   Serial.begin(115200);
//   // 设置通过WIFI进行MicroROS通信
//   IPAddress agent_ip;
//   agent_ip.fromString("192.168.x.xxx");//注意此处IP应该与上位机局域网IP一致
//   // 设置wifi名称，密码，电脑IP,端口号
//   set_microros_wifi_transports("WIFI_NAME", "PASSWD", agent_ip, 8888);
//   // 延时时一段时间，等待设置完成
//   delay(2000);
//   // 初始化内存分配器
//   allocator = rcl_get_default_allocator();
//   // 创建初始化选项
//   rclc_support_init(&support, 0, NULL, &allocator);
//   // 创建节点 microros_wifi
//   rclc_node_init_default(&node, "microros_wifi", "", &support);
//   // 创建节点话题发布者
//   rclc_publisher_init_default(
//     &publisher,
//     &node,
//     ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
//     "Esp32_Time");
//   //创建定时器(1s发一次)
//   rclc_timer_init_default(
//     &timer,
//     &support,
//     RCL_MS_TO_NS(1000),
//     timer_callback);
//   // 创建执行器
//   rclc_executor_init(&executor, &support.context, 1, &allocator);
//   // 给执行器添加定时器
//   rclc_executor_add_timer(&executor, &timer);
// }

// void loop()
// {
//   delay(100);
//   // 循环处理数据
//   rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
// }
#define UXR_CREATE_ENTITIES_USING_REF  5 


/* The options below are needed to reduce the usage of static memory */

// Select only one communication method you wish to use.
// options: USER_TRANSPORT_TYPE_UDP, USER_TRANSPORT_TYPE_TCP, USER_TRANSPORT_TYPE_SERIAL
#define USER_TRANSPORT_TYPE   USER_TRANSPORT_TYPE_UDP

// Set the MTU of the transport.
#define USER_TRANSPORT_MTU    2048

// Set the maximum number of Pub/Subs to be created on the node.
#define USER_ROS2_PUBLISHER_MAX  10 
#define USER_ROS2_SUBSCRIBER_MAX 5
#include <ros2arduino.h>

#include <WiFi.h>
#include <WiFiUdp.h>
#define SSID       "TP-LINK_E15E"
#define SSID_PW    "wht69597105"
#define AGENT_IP   "192.168.0.113"
#define AGENT_PORT 2018 //AGENT port number

#define PUBLISH_FREQUENCY 10 //hz

void publishString(std_msgs::String* msg, void* arg)
{
  (void)(arg);

  static int cnt = 0;
  sprintf(msg->data, "Hello ros2arduino %d", cnt++);
}

class StringPub : public ros2::Node
{
public:
  StringPub()
  : Node("ros2arduino_pub_node")
  {
    ros2::Publisher<std_msgs::String>* publisher_ = this->createPublisher<std_msgs::String>("arduino_chatter");
    this->createWallFreq(PUBLISH_FREQUENCY, (ros2::CallbackFunc)publishString, nullptr, publisher_);
  }
};

WiFiUDP udp;


void setup() 
{
  delay(3000);
  Serial.begin(115200);
  Serial.println("wifi");
  WiFi.begin(SSID, SSID_PW);
  udp.begin(6655);
  while(WiFi.status() != WL_CONNECTED);
  Serial.println("roswInit");
  ros2::init(&udp, AGENT_IP, AGENT_PORT);
  Serial.println("roswInited");
}

void loop() 
{
  static StringPub StringNode;

  ros2::spin(&StringNode);
  delay(500);
}
// #include <ros2arduino.h>

// #include <WiFi.h>
// #include <WiFiClient.h>
//  #define SSID       "TP-LINK_E15E"
//  #define SSID_PW    "wht69597105"
//  #define AGENT_IP   "192.168.0.108"
// #define AGENT_PORT 2018 //AGENT port number

// #define PUBLISH_FREQUENCY 10 //hz

// void publishString(std_msgs::String* msg, void* arg)
// {
//   (void)(arg);

//   static int cnt = 0;
//   sprintf(msg->data, "Hello ros2arduino %d", cnt++);
// }

// class StringPub : public ros2::Node
// {
// public:
//   StringPub()
//   : Node("ros2arduino_pub_node")
//   {
//     ros2::Publisher<std_msgs::String>* publisher_ = this->createPublisher<std_msgs::String>("arduino_chatter");
//     this->createWallFreq(PUBLISH_FREQUENCY, (ros2::CallbackFunc)publishString, nullptr, publisher_);
//   }
// };

// WiFiClient client;

// void setup() 
// {
//    delay(3000);
//    Serial.begin(115200);
//    Serial.println("wifi");
//   WiFi.begin(SSID, SSID_PW);
//   while(WiFi.status() != WL_CONNECTED);

//   ros2::init(&client, AGENT_IP, AGENT_PORT);
// }

// void loop() 
// {
//   static StringPub StringNode;

//   ros2::spin(&StringNode);
// }