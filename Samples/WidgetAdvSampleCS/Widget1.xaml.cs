using System;
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
using Microsoft.Gaming.XboxGameBar.Authentication;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace WidgetAdvSampleCS
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class Widget1 : Page
    {
        private XboxGameBarWidget widget = null;
        private XboxGameBarWidgetControl widgetControl = null;
        private XboxGameBarWebAuthenticationBroker gameBarWebAuth = null;
        private SolidColorBrush widgetBlackBrush =  null;
        private SolidColorBrush widgetWhiteBrush = null;

        public Widget1()
        {
            this.InitializeComponent();
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            widget = e.Parameter as XboxGameBarWidget;
            widgetControl = new XboxGameBarWidgetControl(widget);
            gameBarWebAuth = new XboxGameBarWebAuthenticationBroker(widget);

            widgetBlackBrush = new SolidColorBrush(Windows.UI.Color.FromArgb(255, 38, 38, 38));
            widgetWhiteBrush = new SolidColorBrush(Windows.UI.Color.FromArgb(255, 219, 219, 219));

            SetPinnedStateTextBox();
            SetFavoritedState();
            SetRequestedThemeState();
            OutputVisibleState();
            OutputWindowState();
            OutputGameBarDisplayMode();
            SetBackgroundColor();

            // Hook up events for when the ui is updated.
            widget.SettingsClicked += Widget_SettingsClicked;
            widget.PinnedChanged += Widget_PinnedChanged;
            widget.FavoritedChanged += Widget_FavoritedChanged;
            widget.RequestedThemeChanged += Widget_RequestedThemeChanged;
            widget.VisibleChanged += Widget_VisibleChanged;
            widget.WindowStateChanged += Widget_WindowStateChanged;
            widget.GameBarDisplayModeChanged += Widget_GameBarDisplayModeChanged;
        }

        private async void ActivateAsyncAppExtIdButton_Click(object sender, RoutedEventArgs e)
        {
            String text = ActivateAsyncAppExtId.Text;
            await widgetControl.ActivateAsync(text);
        }

        private async void ActivateAsyncAppIdButton_Click(object sender, RoutedEventArgs e)
        {
            await widgetControl.ActivateAsync(ActivateAsyncAppId.Text, ActivateAsyncAppExtId.Text);
        }

        private async void ActivateWithUriAsyncButton_Click(object sender, RoutedEventArgs e)
        {
            Uri uri = new Uri(ActivateAsyncUri.Text);
            await widgetControl.ActivateWithUriAsync(uri);
        }

        private async void MinimizeAsyncAppIdButton_Click(object sender, RoutedEventArgs e)
        {
            await widgetControl.MinimizeAsync(ActivateAsyncAppExtId.Text);
        }

        private async void MinimizeAsyncAppExtIdButton_Click(object sender, RoutedEventArgs e)
        {
            await widgetControl.MinimizeAsync(ActivateAsyncAppId.Text, ActivateAsyncAppExtId.Text);
        }

        private async void RestoreAsyncAppExtIdButton_Click(object sender, RoutedEventArgs e)
        {
            await widgetControl.RestoreAsync(ActivateAsyncAppExtId.Text);
        }

        private async void RestoreAsyncAppIdButton_Click(object sender, RoutedEventArgs e)
        {
            await widgetControl.RestoreAsync(ActivateAsyncAppId.Text, ActivateAsyncAppExtId.Text);
        }

        private async void CloseAsyncAppExtIdButton_Click(object sender, RoutedEventArgs e)
        {
            await widgetControl.CloseAsync(ActivateAsyncAppExtId.Text);
        }

        private async void CloseAsyncAppIdButton_Click(object sender, RoutedEventArgs e)
        {
            await widgetControl.CloseAsync(ActivateAsyncAppId.Text, ActivateAsyncAppExtId.Text);
        }

        private async void TryResizeWindowAsync_Click(object sender, RoutedEventArgs e)
        {
            Windows.Foundation.Size size;
            size.Height = int.Parse(WindowHeightBox.Text);
            size.Width = int.Parse(WindowWidthBox.Text);
            await widget.TryResizeWindowAsync(size);
        }

        private async void AuthenticateAsync_Click(object sender, RoutedEventArgs e)
        {
            if (RequestUriBox.Text == "" || CallbackUriBox.Text == "")
            {
                return;
            }

            Uri requestUri = new Uri(RequestUriBox.Text);
            Uri callbackUri = new Uri(CallbackUriBox.Text);
            XboxGameBarWebAuthenticationResult result = await gameBarWebAuth.AuthenticateAsync(
                XboxGameBarWebAuthenticationOptions.None,
                requestUri,
                callbackUri);

            Debug.WriteLine("ResponseData: " + result.ResponseData);
            Debug.WriteLine("ResponseStatus: " + result.ResponseStatus.ToString());
            Debug.WriteLine("ResponseErrorDetail: " + result.ResponseErrorDetail);
        }

        private async void Widget_SettingsClicked(XboxGameBarWidget sender, object args)
        {
            await widget.ActivateSettingsAsync();
        }

        private async void Widget_PinnedChanged(XboxGameBarWidget sender, object args)
        {
            await PinnedStateTextBlock.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, () =>
            {
                SetPinnedStateTextBox();
            });
        }

        private async void Widget_FavoritedChanged(XboxGameBarWidget sender, object args)
        {
            await FavoritedTextBlock.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, () => 
            {
                SetFavoritedState();
            });
        }

        private async void Widget_RequestedThemeChanged(XboxGameBarWidget sender, object args)
        {
            await RequestedThemeTextBlock.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, () =>
            {
                SetRequestedThemeState();
                SetBackgroundColor();
            });
        }
        private void Widget_VisibleChanged(XboxGameBarWidget sender, object args)
        {
            OutputVisibleState();
        }

        private void Widget_WindowStateChanged(XboxGameBarWidget sender, object args)
        {
            OutputWindowState();
        }

        private void Widget_GameBarDisplayModeChanged(XboxGameBarWidget sender, object args)
        {
            OutputGameBarDisplayMode();
        }

        private void SetPinnedStateTextBox()
        {
            PinnedStateTextBlock.Text = widget.Pinned.ToString();
        }

        private void SetFavoritedState()
        { 
            FavoritedTextBlock.Text = widget.Favorited.ToString();
        }

        private void SetBackgroundColor()
        {
            this.RequestedTheme = widget.RequestedTheme;
            this.Background = (widget.RequestedTheme == ElementTheme.Dark) ? widgetBlackBrush : widgetWhiteBrush;
        }

        private void SetRequestedThemeState()
        {
            RequestedThemeTextBlock.Text = widget.RequestedTheme.ToString();
        }

        private void OutputVisibleState()
        {
            Debug.WriteLine("Visible: " + widget.Visible.ToString());
        }

        private void OutputWindowState()
        {
            Debug.WriteLine("Window State: " + widget.WindowState.ToString());
        }

        private void OutputGameBarDisplayMode()
        {
            Debug.WriteLine("Game Bar View Mode: " + widget.GameBarDisplayMode.ToString());
        }
    }
}
