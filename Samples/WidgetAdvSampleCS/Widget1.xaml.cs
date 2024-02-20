using Microsoft.Gaming.XboxGameBar;
using Microsoft.Gaming.XboxGameBar.Authentication;
using Microsoft.Gaming.XboxGameBar.Input;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.System;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

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
        private XboxGameBarAppTargetTracker appTargetTracker = null;
        private XboxGameBarWidgetNotificationManager notificationManager = null;
        private XboxGameBarHotkeyWatcher hotkeyWatcher = null;
        private XboxGameBarWidgetActivity widgetActivity = null;

        private SolidColorBrush widgetDarkThemeBrush =  null;
        private SolidColorBrush widgetLightThemeBrush = null;
        private double? opacityOverride = null;

        public Widget1()
        {
            this.InitializeComponent();
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            widget = e.Parameter as XboxGameBarWidget;
            widgetControl = new XboxGameBarWidgetControl(widget);
            gameBarWebAuth = new XboxGameBarWebAuthenticationBroker(widget);
            appTargetTracker = new XboxGameBarAppTargetTracker(widget);
            notificationManager = new XboxGameBarWidgetNotificationManager(widget);

            widgetDarkThemeBrush = new SolidColorBrush(Windows.UI.Color.FromArgb(255, 38, 38, 38));
            widgetLightThemeBrush = new SolidColorBrush(Windows.UI.Color.FromArgb(255, 219, 219, 219));

            // Hook up events for when the ui is updated.
            widget.FavoritedChanged += Widget_FavoritedChanged;
            widget.GameBarDisplayModeChanged += Widget_GameBarDisplayModeChanged;
            widget.PinnedChanged += Widget_PinnedChanged;
            widget.SettingsClicked += Widget_SettingsClicked;
            widget.RequestedOpacityChanged += Widget_RequestedOpacityChanged;
            widget.RequestedThemeChanged += Widget_RequestedThemeChanged;
            widget.VisibleChanged += Widget_VisibleChanged;
            widget.WindowBoundsChanged += Widget_WindowBoundsChanged;
            widget.WindowStateChanged += Widget_WindowStateChanged;

            appTargetTracker.SettingChanged += AppTargetTracker_TargetChanged;
            notificationManager.SettingChanged += NotificationManager_SettingChanged;

            if (appTargetTracker.Setting == XboxGameBarAppTargetSetting.Enabled)
            {
                appTargetTracker.TargetChanged += AppTargetTracker_TargetChanged;
            }

            SetFavoritedState();
            SetGameBarDisplayMode();
            SetPinnedState();
            SetRequestedOpacityState();
            SetBackgroundOpacity();
            SetBackgroundColor();
            SetRequestedThemeState();
            OutputVisibleState();
            OutputWindowState();
            SetTargetInfo();

            HorizontalResizeSupportedCheckBox.IsChecked = widget.HorizontalResizeSupported;
            VerticalResizeSupportedCheckBox.IsChecked = widget.VerticalResizeSupported;
            PinningSupportedCheckBox.IsChecked = widget.PinningSupported;
            SettingsSupportedCheckBox.IsChecked = widget.SettingsSupported;

            MinWindowHeightBox.Text = widget.MinWindowSize.Height.ToString();
            MinWindowWidthBox.Text = widget.MinWindowSize.Width.ToString();
            MaxWindowHeightBox.Text = widget.MaxWindowSize.Height.ToString();
            MaxWindowWidthBox.Text = widget.MaxWindowSize.Width.ToString();

            NotificiationSettingTextBlock.Text = notificationManager.Setting.ToString();

            // Setup hotkey watcher for CTRL + ALT + T
            List<VirtualKey> virtualKeys = new List<VirtualKey> { VirtualKey.Control, VirtualKey.Menu /*ALT*/, VirtualKey.T };
            hotkeyWatcher = XboxGameBarHotkeyWatcher.CreateWatcher(widget, virtualKeys);
            hotkeyWatcher.HotkeySetStateChanged += HotkeyWatcher_HotkeySetStateChanged;
        }

        private async void AppTargetTracker_TargetChanged(XboxGameBarAppTargetTracker sender, object args)
        {
            await TargetTrackerSettingTextBlock.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, () =>
            {
                SetTargetInfo();
            });
        }

        private async void HotkeyWatcher_HotkeySetStateChanged(XboxGameBarHotkeyWatcher sender, HotkeySetStateChangedArgs args)
        {
            await HotkeyPressedTextBlock.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, () =>
            {
                HotkeyPressedTextBlock.Text = args.HotkeySetDown ? "Pressed" : "Not Pressed";
            });
        }

        private void SetTargetInfo()
        {
            var settingEnabled = appTargetTracker.Setting == XboxGameBarAppTargetSetting.Enabled;

            XboxGameBarAppTarget target = null;
            if (settingEnabled)
            {
                target = appTargetTracker.GetTarget();
            }

            TargetTrackerSettingTextBlock.Text = settingEnabled.ToString();
            DisplayNameTextBlock.Text = target.DisplayName;
            AumIdTextBlock.Text = target.AumId;
            TitleIdTextBlock.Text = target.TitleId;
            IsFullscreenTextBlock.Text = target.IsFullscreen.ToString();
            IsGameTextBlock.Text = target.IsGame.ToString();
        }

        private async void NotificationManager_SettingChanged(XboxGameBarWidgetNotificationManager sender, object args)
        {
            await NotificiationSettingTextBlock.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, () =>
            {
                NotificiationSettingTextBlock.Text = notificationManager.Setting.ToString();
            });
        }

        private async void Widget_WindowBoundsChanged(XboxGameBarWidget sender, object args)
        {
            await WindowBoundsTextBlock.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, () =>
            {
                var bounds = widget.WindowBounds;
                WindowBoundsTextBlock.Text = "H(" + bounds.Height + ") W(" + bounds.Width + ") X(" + bounds.X + ") Y(" + bounds.Y + ")";
            });
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

        private async void CenterWindowAsync_Click(object sender, RoutedEventArgs e)
        {
            await widget.CenterWindowAsync();
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

        private async void LaunchUriAsyncButton_Click(object sender, RoutedEventArgs e)
        {
            Uri uri = new Uri(LaunchUriAsyncText.Text);
            bool result = await widget.LaunchUriAsync(uri);
            if (!result)
            {
                Debug.WriteLine("LaunchUriAsync returned false");
            }
        }

        private async void LaunchUriAsyncAdvancedButton_Click(object sender, RoutedEventArgs e)
        {
            Uri uri = new Uri(LaunchUriAsyncText.Text);
            
            LauncherOptions options = new LauncherOptions();
            options.TargetApplicationPackageFamilyName = "Microsoft.WindowsCalculator_8wekyb3d8bbwe";

            ValueSet valueSet = new ValueSet();
            valueSet.Add("testKey1", false);
            valueSet.Add("testKey2", true);

            bool result = await widget.LaunchUriAsync(uri, options, valueSet);
            if (!result)
            {
                Debug.WriteLine("LaunchUriAsync returned false");
            }
        }

        private void HorizontalResizeSupportedCheckBox_Checked(object sender, RoutedEventArgs e)
        {
            widget.HorizontalResizeSupported = true;
        }

        private void HorizontalResizeSupportedCheckBox_Unchecked(object sender, RoutedEventArgs e)
        {
            widget.HorizontalResizeSupported = false;
        }

        private void VerticalResizeSupportedCheckBox_Checked(object sender, RoutedEventArgs e)
        {
            widget.VerticalResizeSupported = true;
        }

        private void VerticalResizeSupportedCheckBox_Unchecked(object sender, RoutedEventArgs e)
        {
            widget.VerticalResizeSupported = false;
        }

        private void PinningSupportedCheckBox_Checked(object sender, RoutedEventArgs e)
        {
            widget.PinningSupported = true;
        }

        private void PinningSupportedCheckBox_Unchecked(object sender, RoutedEventArgs e)
        {
            widget.PinningSupported = false;
        }

        private void SettingsSupportedCheckBox_Checked(object sender, RoutedEventArgs e)
        {
            widget.SettingsSupported = true;
        }

        private void SettingsSupportedCheckBox_Unchecked(object sender, RoutedEventArgs e)
        {
            widget.SettingsSupported = false;
        }

        private void MinWindowSize_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                Size size;
                size.Height = Convert.ToDouble(MinWindowHeightBox.Text);
                size.Width = Convert.ToDouble(MinWindowWidthBox.Text);
                widget.MinWindowSize = size;
            }
            catch (FormatException)
            {
                Debug.WriteLine("Text box must contain valid number");
            }
        }

        private void MaxWindowSize_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                Size size;
                size.Height = Convert.ToDouble(MaxWindowHeightBox.Text);
                size.Width = Convert.ToDouble(MaxWindowWidthBox.Text);
                widget.MaxWindowSize = size;
            }
            catch (FormatException)
            {
                Debug.WriteLine("Text box must contain valid number");
            }
        }

        private void OpacityOverride_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                opacityOverride = Double.Parse(OpacityOverrideBox.Text, System.Globalization.NumberStyles.Any);
            }
            catch (Exception)
            {
                opacityOverride = null;
            }

            // - Value cannot be empty
            // - Value cannot be non-numeric (wtof returns 0)
            // - Value must be between 0 and 1.
            if (   !opacityOverride.HasValue
                || opacityOverride.Value < 0
                || opacityOverride.Value > 1)
            {
                // Invalid
                opacityOverride = null;
                OpacityOverrideBox.Text = "";
            }

            SetRequestedOpacityState();
            SetBackgroundOpacity();
        }

        private async void Widget_SettingsClicked(XboxGameBarWidget sender, object args)
        {
            await widget.ActivateSettingsAsync();
        }

        private async void Widget_PinnedChanged(XboxGameBarWidget sender, object args)
        {
            await PinnedStateTextBlock.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, () =>
            {
                SetPinnedState();
            });
        }

        private async void Widget_FavoritedChanged(XboxGameBarWidget sender, object args)
        {
            await FavoritedTextBlock.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, () => 
            {
                SetFavoritedState();
            });
        }

        private async void Widget_RequestedOpacityChanged(XboxGameBarWidget sender, object args)
        {
            if (!opacityOverride.HasValue)
            {
                await RequestedOpacityTextBlock.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, () =>
                {
                    SetRequestedOpacityState();
                    SetBackgroundOpacity();
                });
            }
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
            SetGameBarDisplayMode();
        }

        private void SetPinnedState()
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
            BackgroundGrid.Background = (widget.RequestedTheme == ElementTheme.Dark) ? widgetDarkThemeBrush : widgetLightThemeBrush;
        }

        private void SetBackgroundOpacity()
        {
            if (opacityOverride.HasValue)
            {
                BackgroundGrid.Opacity = opacityOverride.Value;
            }
            else
            {
                BackgroundGrid.Opacity = widget.RequestedOpacity;
            }
        }

        private void SetRequestedOpacityState()
        {
            if (opacityOverride.HasValue)
            {
                RequestedOpacityTextBlock.Text = opacityOverride.Value.ToString();
            }
            else
            {
                RequestedOpacityTextBlock.Text = widget.RequestedOpacity.ToString();
            }
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

        private void SetGameBarDisplayMode()
        {
            Debug.WriteLine("Game Bar View Mode: " + widget.GameBarDisplayMode.ToString());
        }

        private void StartActivityButton_Click(object sender, RoutedEventArgs e)
        {
            if (widgetActivity == null)
            {
                widgetActivity = new XboxGameBarWidgetActivity(widget, "uniqueActivityId");
            }
        }

        private void StopActivityButton_Click(object sender, RoutedEventArgs e)
        {
            if (widgetActivity != null)
            {
                widgetActivity.Complete();
                widgetActivity = null;
            }
        }

        private void StartHotkeyWatcher_Click(object sender, RoutedEventArgs e)
        {
            HotkeyPressedTextBlock.Text = "Not Pressed";
            hotkeyWatcher.Start();
        }

        private void StopHotkeyWatcher_Click(object sender, RoutedEventArgs e)
        {
            hotkeyWatcher.Stop();
            HotkeyPressedTextBlock.Text = "Watcher not running";
        }

        private async void ShowBasicNotification_Click(object sender, RoutedEventArgs e)
        {
            var widgetNotification = new XboxGameBarWidgetNotificationBuilder("This is a toast title")
                .PrimaryImageFromPublicFolder("SamplePrimaryImage.png", XboxGameBarWidgetNotificationImageCrop.Circle)
                .BuildNotification();

            // Call async show to unwind calling thread while result is obtained
            // Success means toast was succesfully shown or queued to be shown
            var result = await notificationManager.TryShowAsync(widgetNotification);
        }

        private async void ShowAdvancedNotification_Click(object sender, RoutedEventArgs e)
        {
            var widgetNotification =
                // (Required) Short toast title
                new XboxGameBarWidgetNotificationBuilder("This is a toast title that can wrap to two lines if necessary")
                // (Optional) Short toast description
                .Content("This is a toast description that can wrap to two lines if necessary")
                // (Optional) Widget activation payload on toast activation
                .ActivationPayload("id=testId&context=additionalContext")
                // (Optional) Primary image
                // URI scheme: Call .PrimaryImageFromUri(...)
                // File name or relative path scheme: .PrimaryImageFromPublicFolder(...)
                // Note: using both schemes both will cause an exception
                .PrimaryImageFromPublicFolder("SamplePrimaryImage.png", XboxGameBarWidgetNotificationImageCrop.Default)
                // (Optional) Secondary image - same as above
                // Note: secondary image always shown with circle image crop type
                .SecondaryImageFromPublicFolder("SampleSecondaryImage.png")
                // (Optional) Sound - specify file name or relative path
                .SoundFromPublicFolder("Sounds\\SampleSound.mp3")
                // (Optional) Ignore Windows quiet hours setting for important, time sensitive notifications
                .IgnoreQuietHours(true)
                // (Optional) Avoid bringing Game Bar to the foreground for external toast activations
                .IsBackgroundActivation(false)
                // Call this to build the notification object to pass to the notification manager
                .BuildNotification();

            var result = await notificationManager.TryShowAsync(widgetNotification);
        }
    }
}
