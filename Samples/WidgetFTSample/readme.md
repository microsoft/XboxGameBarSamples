This Widget sample demonstrates how to pair your widget with a full trust (standard user privelege) COM server. Widgets run in an AppContainer and don't always have access to what you need.
This sample shows how to build a full trust COM server that hosts WinRT objects, allowing you to seamlessly use full trust object oriented (RAII compliant) objects hosted out-of-proc. This sample demonstrates using C++/WinRT, but you can leave the WidgetFTServer as-is and access it using C# as well. You'll just need to replicate what is done in WidgetFTServer.h in C# (using PInvoke to CoCreate instance, then cast to WinRT factory). 

Key pieces of the solution
- WidgetFTSample (the Game Bar widget)
- WidgetFTServer (the COM server)
- WidgetFTSample.vcxproj is updated to pull in both WidgetFTServer.exe and WidgetFT.winmd
- WidgetFTSample's package.appxmanifest declares the WidgetFTServer.exe as a full trust COM server with a single CLSID of the WidgetFTFactory COM object. 
- WidgetFTFactory is used to create whatever component objects you want. SampleComponent is an example of the first component exposed by the COM server. Modify it and then add as many as you like.
- WidgetFT.winmd is critical as it is used to create your API projection into your C++/WinRT widget project or your C# widget project. Further, it also must be deployed along side your exe binaries to provide runtime marshaling of your components. This allows you to avoid writing COM proxy stub dlls. COM just uses the winmd (AKA Metadata Based Marshaling (MBM)). Note the winmd name comes from the namespace you define in your idl(s). Both WidgetFTFactory.idl and SampleComponent.idl are both using WidgetFT as the namespace, thus a single WidgetFT.winmd is created. 
- Even if you use C# for your client (Widget), I would recommend keeping the COM server written in C++/WinRT. I don't have a recommended solution to try and run C#/.NET code as your COM server. 

Lifetime management of the server is directly tied to outstanding object connections from the client. When the last reference is release, the COM server will shutdown.

If you want your client (widget) to be robust against your full trust COM server crashing, notice the ProcessHandle method off the WidgetFTFactory object. This allows you to get the process handle of your COM server process. From that, you can use normal Win32 APIs (like WaitForSingleObject) to know when the process exits. From there, you can run recovery logic if necessary to restart and reconnect to the COM server. 

Please note: Even though the COM server runs as full trust, it still runs in a helium container. This means some registry read/writes file read/writes may be virtualized. If you want to avoid those things as well you need to declare `<rescap:Capability Name="unvirtualizedResources" />` capability in your app manifest as well as `<desktop6:RegistryWriteVirtualization>disabled</desktop6:RegistryWriteVirtualization>` and/or `<desktop6:FileSystemWriteVirtualization>disabled</desktop6:FileSystemWriteVirtualization>`. See [this link](https://learn.microsoft.com/en-us/uwp/schemas/appxpackage/uapmanifestschema/element-desktop6-filesystemwritevirtualization) for more info.