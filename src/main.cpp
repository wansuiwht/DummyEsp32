#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>
#include "secrets.h"  // add WLAN Credentials in here.
#include <FS.h>       // File System for Web Server Files
#include <LittleFS.h> // This file system is used.
#include <Preferences.h>
#include "AsyncWebSocket.h"
#include "AsyncUDP.h"
#include "dummyHttpCmdHandler.h"
#include "WebSocketsServer.h"
// 用于保存wifi信息
Preferences preferences;
// Udp服务端
AsyncUDP udp;
// Websocket服务器
WebSocketsServer webSocket = WebSocketsServer(81);
// WebServer服务器
WebServer server(80);
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

// enable the CUSTOM_ETAG_CALC to enable calculation of ETags by a custom function
#define CUSTOM_ETAG_CALC

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

IPAddress apIp;
// 初始化Wifi
void WiFiInit()
{
  WiFi.setHostname(HOSTNAME);
  // 读取保存数据

  // 读取wifi 的ssid
  String ssid = preferences.getString("ssid", "");
  // 初始状态，没有设置wifi
  if (ssid == "")
  {
    wifiMode = WIFI_AP;

    ssid = "DummyEsp32";
    WiFi.mode(WIFI_AP);
    if (!WiFi.softAP(ssid))
    {
      log_e("热点创建失败.");
      //创建失败了，重启ESP
     // ESP.restart();
    }
    apIp = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(apIp);
  }
  else
  {
    String pass = preferences.getString("pass", "");
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
    TRACE("connected.\n");
  }
}
void UdpInit()
{
  if (udp.listen(9999))
  {
    udp.onPacket([](AsyncUDPPacket packet)
                 {
                  //转发给STM32
                  Serial2.write(packet.data(),packet.length());
                  Serial2.println();
                  packet.print("sent to Stm32"); });
  }
}
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
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{

  switch (type)
  {
  case WStype_DISCONNECTED:
    Serial.printf("[%u] Disconnected!\n", num);
    break;
  case WStype_CONNECTED:
  {
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
HardwareSerial S(2);
// Setup everything to make the webserver work.
void setup(void)
{
  delay(3000); // wait for serial monitor to start completely.
  TRACE("Starting WebServer\n");
  // 调试信息串口
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  // 与STM32通信
  S.begin(115200,SERIAL_8N1,15,16);
  // 开启存储
  preferences.begin("Dummy", false);
  TRACE("Starting WebServer\n");
  WiFiInit();
  UdpInit();

  // Ask for the current time using NTP request builtin into ESP firmware.
  TRACE("Setup ntp...\n");
  configTzTime(TIMEZONE, "pool.ntp.org");

  TRACE("Register redirect...\n");

  // register a redirect handler when only domain name is given.
  server.on("/", HTTP_GET, handleRedirect);
  server.on("/setWifi", HTTP_GET, handleSetWifi);
  TRACE("Register service handlers...\n");

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

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  TRACE("open <http://%s> or <http://%s>\n", WiFi.getHostname(), WiFi.localIP().toString().c_str());
} // setup


void loop(void)
{
  server.handleClient();
  webSocket.loop();
} //loop

// end.
