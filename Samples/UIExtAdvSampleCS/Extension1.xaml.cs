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
using System.Diagnostics;

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


        private SolidColorBrush uiExtensionBlackBrush  =  null;
        private SolidColorBrush uiExtensionWhiteBrush = null;

        public Extension1()
        {
            this.InitializeComponent();
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            uiExtension = e.Parameter as XboxGameBarUIExtension;
            extensionControl = new XboxGameBarExtensionControl(uiExtension);

            uiExtensionBlackBrush = new SolidColorBrush(Windows.UI.Color.FromArgb(255, 38, 38, 38));
            uiExtensionWhiteBrush = new SolidColorBrush(Windows.UI.Color.FromArgb(255, 219, 219, 219));

            SetPinnedStateTextBox();
            SetFavoritedState();
            SetRequestedThemeState();
            OutputVisibleState();
            OutputWindowState();
            OutputGameBarDisplayMode();
            SetBackgroundColor();


            //Hook up events for when the ui is updated.
            uiExtension.SettingsClicked += UiExtension_SettingsClicked;
            uiExtension.PinnedChanged += UiExtension_PinnedChanged;
            uiExtension.FavoritedChanged += UiExtension_FavoritedChanged;
            uiExtension.RequestedThemeChanged += UiExtension_RequestedThemeChanged;
            uiExtension.VisibleChanged += UiExtension_VisibleChanged;
            uiExtension.WindowStateChanged += UiExtension_WindowStateChanged;
            uiExtension.GameBarDisplayModeChanged += UiExtension_GameBarDisplayModeChanged;
        }

        private async void ActivateAsyncAppExtIdButton_Click(object sender, RoutedEventArgs e)
        {
            String text = ActivateAsyncAppExtId.Text;
            await extensionControl.ActivateAsync(text);
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
            await PinnedStateTextBlock.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, () =>
            {
                SetPinnedStateTextBox();
            });
        }

        private async void UiExtension_FavoritedChanged(XboxGameBarUIExtension sender, object args)
        {
            await FavoritedTextBlock.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, () => 
            {
                SetFavoritedState();
            });
        }

        private async void UiExtension_RequestedThemeChanged(XboxGameBarUIExtension sender, object args)
        {
            await RequestedThemeTextBlock.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, () =>
            {
                SetRequestedThemeState();
                SetBackgroundColor();
            });
        }
        private void UiExtension_VisibleChanged(XboxGameBarUIExtension sender, object args)
        {
            OutputVisibleState();
        }

        private void UiExtension_WindowStateChanged(XboxGameBarUIExtension sender, object args)
        {
            OutputWindowState();
        }

        private void UiExtension_GameBarDisplayModeChanged(XboxGameBarUIExtension sender, object args)
        {
            OutputGameBarDisplayMode();
        }

        private void SetPinnedStateTextBox()
        {
            PinnedStateTextBlock.Text = uiExtension.Pinned.ToString();
        }

        private void SetFavoritedState()
        { 
            FavoritedTextBlock.Text = uiExtension.Favorited.ToString();
        }

        private void SetBackgroundColor()
        {
            this.RequestedTheme = uiExtension.RequestedTheme;
            this.Background = (uiExtension.RequestedTheme == ElementTheme.Dark) ? uiExtensionBlackBrush : uiExtensionWhiteBrush;
        }

        private void SetRequestedThemeState()
        {
            RequestedThemeTextBlock.Text = uiExtension.RequestedTheme.ToString();
        }

        private void OutputVisibleState()
        {
            Debug.WriteLine("Visible: " + uiExtension.Visible.ToString());
        }

        private void OutputWindowState()
        {
            Debug.WriteLine("Window State: " + uiExtension.WindowState.ToString());
        }

        private void OutputGameBarDisplayMode()
        {
            Debug.WriteLine("Game Bar View Mode: " + uiExtension.GameBarDisplayMode.ToString());
        }
    }
}
