#include <Arduino.h>
#include <WebServer.h>
#include "dummyCmd.h"
#define UNUSED __attribute__((unused))
extern IPAddress apIp;
class DummyHttpCmdHandler : public RequestHandler
{
private:
  DummyCmd cmd;
public:
DummyHttpCmdHandler(){};
  bool canHandle(HTTPMethod requestMethod, String UNUSED uri) override
  {
    return uri.startsWith("/Dummy");
  } // canHandle()

  bool canUpload(String uri) override
  {
    //不允许上传
    return false;
  } // canUpload()

  bool handle(WebServer &server, HTTPMethod requestMethod, String requestUri) override
  {
    String c =  server.arg("cmd");
    cmd.ExecCmd(c);
    Serial.println(c.c_str());
    //Serial1.setTimeout(10000);
   //String read = Serial1.readStringUntil('\n');
    server.send(200, "text/javascript; charset=utf-8","{\"code\":0,\"msg\":\"ok\"}");
    return true;
  } // handle()
};