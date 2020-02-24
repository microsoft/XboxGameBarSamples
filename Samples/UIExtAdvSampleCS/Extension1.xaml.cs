﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
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

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace UIExtAdvSampleCS
{


    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class Extension1 : Page
    {
        private XboxGameBarUIExtension uiExtension = null;
        private XboxGameBarExtensionControl extensionControl = null;
        private EventRegistrationToken settingsToken;

        public Extension1()
        {
            this.InitializeComponent();
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            uiExtension = e.Parameter as XboxGameBarUIExtension;
            extensionControl = new XboxGameBarExtensionControl(uiExtension);

            SetPinnedStateTextBox();
            SetFavoritedState();
            SetRequestedThemeState();


            //Hook up events for when the ui is updated.
            uiExtension.SettingsClicked += UiExtension_SettingsClicked;
            uiExtension.PinnedChanged += UiExtension_PinnedChanged;
            uiExtension.FavoritedChanged += UiExtension_FavoritedChanged;
            uiExtension.RequestedThemeChanged += UiExtension_RequestedThemeChanged;
        }

        private async void ActivateAsyncAppExtIdButton_Click(object sender, RoutedEventArgs e)
        {
            await extensionControl.ActivateAsync(ActivateAsyncAppExtId.Text);
        }

        private async void ActivateAsyncAppIdButton_Click(object sender, RoutedEventArgs e)
        {
            await extensionControl.ActivateAsync(ActivateAsyncAppId.Text, ActivateAsyncAppExtId.Text);
        }

        private async void ActivateWithUriAsyncButton_Click(object sender, RoutedEventArgs e)
        {
            Uri uri = new Uri(ActivateAsyncUri.Text);
            await extensionControl.ActivateWithUriAsync(uri);
        }

        private async void MinimizeAsyncAppIdButton_Click(object sender, RoutedEventArgs e)
        {
            await extensionControl.MinimizeAsync(ActivateAsyncAppExtId.Text);
        }

        private async void MinimizeAsyncAppExtIdButton_Click(object sender, RoutedEventArgs e)
        {
            await extensionControl.MinimizeAsync(ActivateAsyncAppId.Text, ActivateAsyncAppExtId.Text);
        }

        private async void RestoreAsyncAppExtIdButton_Click(object sender, RoutedEventArgs e)
        {
            await extensionControl.RestoreAsync(ActivateAsyncAppExtId.Text);
        }

        private async void RestoreAsyncAppIdButton_Click(object sender, RoutedEventArgs e)
        {
            await extensionControl.RestoreAsync(ActivateAsyncAppId.Text, ActivateAsyncAppExtId.Text);
        }

        private async void CloseAsyncAppExtIdButton_Click(object sender, RoutedEventArgs e)
        {
            await extensionControl.CloseAsync(ActivateAsyncAppExtId.Text);
        }

        private async void CloseAsyncAppIdButton_Click(object sender, RoutedEventArgs e)
        {
            await extensionControl.CloseAsync(ActivateAsyncAppId.Text, ActivateAsyncAppExtId.Text);
        }

        private async void TryResizeWindowAsync_Click(object sender, RoutedEventArgs e)
        {
            Windows.Foundation.Size size;
            size.Height = int.Parse(WindowHeightBox.Text);
            size.Width = int.Parse(WindowWidthBox.Text);
            await uiExtension.TryResizeWindowAsync(size);
        }

        private async void UiExtension_SettingsClicked(XboxGameBarUIExtension sender, object args)
        {
            await uiExtension.ActivateSettingsAsync();
        }

        private async void UiExtension_PinnedChanged(XboxGameBarUIExtension sender, object args)
        {
            await PinnedStateTextBlock.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.High, () =>
            {
                SetPinnedStateTextBox();
            });
            
        }

        private async void UiExtension_FavoritedChanged(XboxGameBarUIExtension sender, object args)
        {
            await FavoritedTextBlock.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.High, () => 
            {
                SetFavoritedState();
            });
        }

        private async void UiExtension_RequestedThemeChanged(XboxGameBarUIExtension sender, object args)
        {
            await RequestedThemeTextBlock.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.High, () =>
            {
                SetRequestedThemeState();
            });
        }

        private void SetPinnedStateTextBox()
        {
            String isPinned = uiExtension.Pinned ? "true" : "false";
            PinnedStateTextBlock.Text = isPinned;
        }

        private void SetFavoritedState()
        {
            String isFavorited = uiExtension.Favorited ? "true" : "false";
            FavoritedTextBlock.Text = isFavorited;
        }

        private void SetRequestedThemeState()
        {
            String requestedTheme = "";

            switch(uiExtension.RequestedTheme)
            {
                case ElementTheme.Light:
                    requestedTheme = requestedTheme + "Light";
                    break;
                case ElementTheme.Dark:
                    requestedTheme = requestedTheme + "Dark";
                    break;
            }

            RequestedThemeTextBlock.Text = requestedTheme;
        }
    }
}