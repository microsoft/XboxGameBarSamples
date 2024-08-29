#pragma once

#undef WINAPI_FAMILY
#define WINAPI_FAMILY WINAPI_FAMILY_DESKTOP_APP
#define WIN32_LEAN_AND_MEAN
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

// WIL
// wil/cppwinrt.h enabled mixing of C++/WinRT and WIL exception types in a compatible way
// This file must come before other wil macro headers
#include <wil/cppwinrt.h>
#include <wil/com.h>
#include <wil/resource.h>
#include <wil/result_macros.h>
#include <wil/result.h>
#include <wil/token_helpers.h>

// WinRT
#include <unknwn.h>
#include <restrictederrorinfo.h>
#include <hstring.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.ApplicationModel.Activation.h>

#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.Xaml.h>
#include <winrt/Windows.UI.Xaml.Controls.h>
#include <winrt/Windows.UI.Xaml.Controls.Primitives.h>
#include <winrt/Windows.UI.Xaml.Data.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Windows.UI.Xaml.Markup.h>
#include <winrt/Windows.UI.Xaml.Navigation.h>
#include <winrt/Windows.System.h>
#include <winrt/Microsoft.Gaming.XboxGameBar.h>

// Local headers
#include "WidgetFTServer.h"
