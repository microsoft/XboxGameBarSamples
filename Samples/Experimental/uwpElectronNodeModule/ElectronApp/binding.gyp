{
  "targets": [
    {
      "target_name": "GbRpc",
      "sources": [ "GameBarInterface.idl","build\GameBarInterface_s.c", "rpc-server.cpp", "GbNodeModule.cc" ],
      "include_dirs" : [ "<!(node -e \"require('nan')\")"],
      "conditions" : [
         [
            "OS=='win'",
            {
              "link_settings": {
                  "libraries": [
                      "-lrpcrt4.lib",
                  ]
              }
            }
         ]
      ]
    }
  ]
}