﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.ApplicationModel;
using Windows.ApplicationModel.Activation;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using Microsoft.Gaming.XboxGameBar;

namespace UIExtAdvSampleCS
{
    /// <summary>
    /// Provides application-specific behavior to supplement the default Application class.
    /// </summary>
    sealed partial class App : Application
    {
        private XboxGameBarUIExtension uiExtension1 = null;
        private XboxGameBarUIExtension uiExtension1Settings = null;
        private XboxGameBarUIExtension uiExtension2 = null;

        /// <summary>
        /// Initializes the singleton application object.  This is the first line of authored code
        /// executed, and as such is the logical equivalent of main() or WinMain().
        /// </summary>
        public App()
        {
            this.InitializeComponent();
            this.Suspending += OnSuspending;
        }

        protected override void OnActivated(IActivatedEventArgs args)
        {
            XboxGameBarUIExtensionActivatedEventArgs uiExtArgs = null;
            if (args.Kind == ActivationKind.Protocol)
            {
                var protocolArgs = args as IProtocolActivatedEventArgs;
                string scheme = protocolArgs.Uri.Scheme;
                if (scheme.StartsWith("ms-gamebaruiextension"))
                {
                    uiExtArgs = args as XboxGameBarUIExtensionActivatedEventArgs;
                }
            }
            if (uiExtArgs != null)
            {
                //
                // If IsLaunchActivation is true, this is Game Bar's initial activation of us 
                // and we MUST create and hold onto XboxGameBarUIExtension.
                //
                // Otherwise this is a subsequent activation coming from Game Bar. We should
                // continue to hold the XboxGameBarUIExtension created during initial activation
                // and just observe the URI command here and act accordingly. It is ok to
                // perform a navigate on the root frame to switch views/pages if needed.
                //
                if (uiExtArgs.IsLaunchActivation)
                {
                    var rootFrame = new Frame();
                    rootFrame.NavigationFailed += OnNavigationFailed;
                    Window.Current.Content = rootFrame;

                    // Navigate to correct view
                    if (uiExtArgs.AppExtensionId == "Extension1")
                    {
                        uiExtension1 = new XboxGameBarUIExtension(
                            uiExtArgs,
                            Window.Current.CoreWindow,
                            rootFrame);
                        rootFrame.Navigate(typeof(Extension1), uiExtension1);
                    }
                    else if (uiExtArgs.AppExtensionId == "Extension1Settings")
                    {
                        uiExtension1Settings = new XboxGameBarUIExtension(
                            uiExtArgs,
                            Window.Current.CoreWindow,
                            rootFrame);
                        rootFrame.Navigate(typeof(Extension1Settings));
                    }
                    else if (uiExtArgs.AppExtensionId == "Extension2")
                    {
                        uiExtension2 = new XboxGameBarUIExtension(
                            uiExtArgs,
                            Window.Current.CoreWindow,
                            rootFrame);
                        rootFrame.Navigate(typeof(Extension2), uiExtArgs.Uri);
                    }
                    else
                    {
                        // Unknown - Game Bar should never send you an unknown App Extension Id
                        return;
                    }

                    Window.Current.Activate();
                }
                else
                {
                    // You can perform whatever behavior you need based on the URI payload. In our case
                    // we're simply renavigating to Extension2 and displaying the absolute URI.  You
                    // define your URI schema (subpath + query + fragment). 
                    Frame rootFrame = null;
                    rootFrame.NavigationFailed += OnNavigationFailed;
                    var content = Window.Current.Content;
                    if (content != null)
                    {
                        rootFrame = content as Frame;
                    }
                    rootFrame.Navigate(typeof(Extension2), uiExtArgs.Uri);
                }
            }
        }

        /// <summary>
        /// Invoked when the application is launched normally by the end user.  Other entry points
        /// will be used such as when the application is launched to open a specific file.
        /// </summary>
        /// <param name="e">Details about the launch request and process.</param>
        protected override void OnLaunched(LaunchActivatedEventArgs e)
        {
            Frame rootFrame = Window.Current.Content as Frame;

            // Do not repeat app initialization when the Window already has content,
            // just ensure that the window is active
            if (rootFrame == null)
            {
                // Create a Frame to act as the navigation context and navigate to the first page
                rootFrame = new Frame();

                rootFrame.NavigationFailed += OnNavigationFailed;

                if (e.PreviousExecutionState == ApplicationExecutionState.Terminated)
                {
                    //TODO: Load state from previously suspended application
                }

                // Place the frame in the current Window
                Window.Current.Content = rootFrame;
            }

            if (e.PrelaunchActivated == false)
            {
                if (rootFrame.Content == null)
                {
                    // When the navigation stack isn't restored navigate to the first page,
                    // configuring the new page by passing required information as a navigation
                    // parameter
                    rootFrame.Navigate(typeof(MainPage), e.Arguments);
                }
                // Ensure the current window is active
                Window.Current.Activate();
            }
        }

        /// <summary>
        /// Invoked when Navigation to a certain page fails
        /// </summary>
        /// <param name="sender">The Frame which failed navigation</param>
        /// <param name="e">Details about the navigation failure</param>
        void OnNavigationFailed(object sender, NavigationFailedEventArgs e)
        {
            throw new Exception("Failed to load Page " + e.SourcePageType.FullName);
        }

        /// <summary>
        /// Invoked when application execution is being suspended.  Application state is saved
        /// without knowing whether the application will be terminated or resumed with the contents
        /// of memory still intact.
        /// </summary>
        /// <param name="sender">The source of the suspend request.</param>
        /// <param name="e">Details about the suspend request.</param>
        private void OnSuspending(object sender, SuspendingEventArgs e)
        {
            var deferral = e.SuspendingOperation.GetDeferral();
            //TODO: Save application state and stop any background activity
            deferral.Complete();
        }
    }
}
