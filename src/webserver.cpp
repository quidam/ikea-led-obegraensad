#include "webserver.h"

AsyncWebServer server(80);

void initWebServer()
{
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Accept, Content-Type, Authorization");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Credentials", "true");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

  server.onNotFound([](AsyncWebServerRequest *request)
                    { request->send(404, "text/plain", "Page not found!"); });

  server.begin();
}
