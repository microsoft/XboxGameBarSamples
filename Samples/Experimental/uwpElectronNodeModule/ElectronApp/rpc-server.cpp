#include <windows.h>
#include <cstdio>
#include "build\GameBarInterface_h.h"
#include "rpc-server.h"

bool ShutdownRequested;
static RPC_BINDING_VECTOR *BindingVector = nullptr;

void StartBroadcast(
	/* [in] */ handle_t IDL_handle)
{
	UNREFERENCED_PARAMETER(IDL_handle);
}

void StopBroadcast(
	/* [in] */ handle_t IDL_handle)
{
	UNREFERENCED_PARAMETER(IDL_handle);
}

enum Color
{
	Brown, White, Green, Yellow, Red
};

void WidgetRpcMessage( 
    /* [in] */ handle_t IDL_handle,
    /* [string][in] */ unsigned char * message,
    /* [out] */ int * result)
{
	static unsigned int val = 0;
	
	if (*message == 'y') //TODO: change to strncmp
	{
		if (val >= 5)
		{
			val = 0;
		}

		*result = static_cast<Color>(val++);
		MockUiChange(true); // Send mock update ui event
	}
	else
	{
		*result = -1;
		MockUiChange(false);
	}
}

void MockUiChange(bool change)
{
	if (change)
	{
		printf("POST: Client requested UI change\n");
	}
	else
	{
		printf("RESPONSE: No UI change requested\n");
	}
}

DWORD RpcServerStart()
{
	HRESULT hr = S_OK;
	unsigned char protocol[] = "ncalrpc";
	unsigned char endpoint[] = "WidgetSampleGameBarEndpoint";
	unsigned int minCalls = 1;
	unsigned int dontWait = false;
	ShutdownRequested = false;
	handle_t handle;

	//
	// Register the RCP interface
	//
	hr = RpcServerRegisterIf(
		GameBarInterface_v1_0_s_ifspec,
		nullptr,
		nullptr);

	if (hr != S_OK) {
		return hr;
	}

	//
	// Make the server available on the network
	//
	hr = RpcServerUseProtseqEp(
		reinterpret_cast<RPC_CSTR>(protocol),
		RPC_C_PROTSEQ_MAX_REQS_DEFAULT, reinterpret_cast<RPC_CSTR>(endpoint),
		nullptr);

	if (hr != S_OK) {
		return hr;
	}

	//
	// Register endpoints
	//

	hr = RpcServerInqBindings(&BindingVector);

	if (hr != S_OK) {
		return hr;
	}

	hr = RpcEpRegister(GameBarInterface_v1_0_s_ifspec, BindingVector,
			   nullptr, nullptr);

	if (hr != S_OK) {
		return hr;
	}

	hr = RpcServerListen(minCalls, RPC_C_LISTEN_MAX_CALLS_DEFAULT,
			     dontWait);

	return hr;
}

//
// Notify rpc server to stop listening to incoming rpc calls
//
void RpcServerDisconnect()
{
	HRESULT hr = S_OK;
	ShutdownRequested = true;
	hr = RpcServerUnregisterIf(GameBarInterface_v1_0_s_ifspec, nullptr, 0);

	RpcEpUnregister(GameBarInterface_v1_0_s_ifspec, BindingVector, nullptr);

	if (BindingVector != nullptr) {
		RpcBindingVectorFree(&BindingVector);
		BindingVector = nullptr;
	}
}


/******************************************************/
/*         MIDL allocate and free                     */
/******************************************************/

void __RPC_FAR *__RPC_USER midl_user_allocate(_In_ size_t len)
{
	return (malloc(len));
}

void __RPC_USER midl_user_free(_In_ void __RPC_FAR *ptr)
{
	free(ptr);
}
