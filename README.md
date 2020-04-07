---
page_type: sample
languages:
- csharp
products:
- dotnet
description: "Add 150 character max description"
urlFragment: "update-this-to-unique-url-stub"
---

# Xbox Game Bar SDK Samples

<!-- 
Guidelines on README format: https://review.docs.microsoft.com/help/onboard/admin/samples/concepts/readme-template?branch=master

Guidance on onboarding samples to docs.microsoft.com/samples: https://review.docs.microsoft.com/help/onboard/admin/samples/process/onboarding?branch=master

Taxonomies for products and languages: https://review.docs.microsoft.com/new-hope/information-architecture/metadata/taxonomies?branch=master
-->

The Xbox Game Bar software development kit allows you to build widgets for Xbox Game Bar. An Xbox Game Bar widget is a Windows UWP XAML app that has a XAML view rendered into Xbox Game Bar's UI.

Aside from a slightly different app activation and a little "bootstrapping" when the app is activated, an Xbox Game Bar widget is no different than any other UWP XAML app. In fact your UWP XAML app can be a normal app (support foreground activation) and one or more Xbox Game Bar widgets (support Xbox Game Bar widget activation).

When your app is activated, it will know which activation type is happening so it can create the correct corresponding XAML view.

These samples will allow you to discover how to build your own Game Bar widget.

## Build Status
Master Branch
[![Build Status](https://microsoft.visualstudio.com/Xbox.Apps/_apis/build/status/XboxGameBar/XboxGameBarSamples_CI?branchName=master)](https://microsoft.visualstudio.com/Xbox.Apps/_build/latest?definitionId=50221&branchName=master)

## Documentation

All documentation describing the Game Bar SDK, these samples, OS requirements, and setting up your environment can be found on [docs.microsoft.com/gaming/game-bar](https://docs.microsoft.com/gaming/game-bar).

## Contributing

This project welcomes contributions and suggestions.  Most contributions require you to agree to a
Contributor License Agreement (CLA) declaring that you have the right to, and actually do, grant us
the rights to use your contribution. For details, visit https://cla.opensource.microsoft.com.

When you submit a pull request, a CLA bot will automatically determine whether you need to provide
a CLA and decorate the PR appropriately (e.g., status check, comment). Simply follow the instructions
provided by the bot. You will only need to do this once across all repos using our CLA.

This project has adopted the [Microsoft Open Source Code of Conduct](https://opensource.microsoft.com/codeofconduct/).
For more information see the [Code of Conduct FAQ](https://opensource.microsoft.com/codeofconduct/faq/) or
contact [opencode@microsoft.com](mailto:opencode@microsoft.com) with any additional questions or comments.
