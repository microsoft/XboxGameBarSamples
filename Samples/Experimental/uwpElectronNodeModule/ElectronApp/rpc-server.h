#pragma once

DWORD RpcServerStart();

void RpcServerDisconnect();

// Mock api to emulate reception of ui change event from Widget to electron app
void MockUiChange(bool change);





