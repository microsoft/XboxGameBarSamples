#pragma once

#undef WINAPI_FAMILY
#define WINAPI_FAMILY WINAPI_FAMILY_DESKTOP_APP
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define NOMINMAX
#include <windows.h>
#ifdef GetCurrentTime
#undef GetCurrentTime
#endif
#ifdef GetObject
#undef GetObject
#endif
#ifdef SendMessage
#undef SendMessage
#endif
#ifdef GetUserName
#undef GetUserName
#endif

#include <sddl.h>
#include <winstring.h>
#include <winsvc.h>

// STL (required by below WIL headers)
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <unordered_map>
#include <functional>
#include <chrono>

// WIL
// wil/cppwinrt.h enabled mixing of C++/WinRT and WIL exception types in a compatible way
// This file must come before other wil macro headers
#include <wil/cppwinrt.h>
#include <wil/com.h>
#include <wil/resource.h>
#include <wil/result_macros.h>
#include <wil/result.h>
#include <wil/token_helpers.h>

// WRL
#include <wrl.h>
#include <wrl/client.h>
#include <wrl/wrappers/corewrappers.h>

// WinRT
#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Windows.Data.Json.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.Diagnostics.h>
#include <winrt/Windows.Management.Deployment.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.System.Threading.h>
#include <winrt/Windows.UI.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.Xaml.h>
#include <winrt/Windows.UI.Xaml.Controls.h>
#include <winrt/Windows.UI.Xaml.Hosting.h>
#include <winrt/Windows.UI.Xaml.Media.h>

// Local utils
#include <cppwinrt_wrl_priv.h>
#include <singleton_base.h>
