#include <nan.h>
#include "rpc-server.h"

NAN_METHOD(ServerStart)
{
  unsigned int serverStarted = RpcServerStart();
  info.GetReturnValue().Set(serverStarted);
}

NAN_METHOD(ServerDisconnect)
{
  RpcServerDisconnect();
  info.GetReturnValue();
}

NAN_MODULE_INIT(Initialize) 
{
  NAN_EXPORT(target, ServerStart);
  NAN_EXPORT(target, ServerDisconnect);
}

NAN_MODULE_WORKER_ENABLED(addon, Initialize)