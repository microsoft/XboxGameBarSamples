using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Threading;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Storage;
using Windows.Storage.Pickers;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using Microsoft.Gaming.XboxGameBar;
using Microsoft.Gaming.XboxGameBar.Restricted;
using Windows.UI.Core;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

namespace WidgetForegroundWorkerSample
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class Widget1 : Page
    {
        private XboxGameBarWidget widget = null;

        public Widget1()
        {
            this.InitializeComponent();
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            widget = e.Parameter as XboxGameBarWidget;
        }

        private void LaunchForegroundWorkButton_Click(object sender, RoutedEventArgs e)
        {
            _ = Task.Run(async () =>
            {
                IAsyncOperation<StorageFile> filePickerOperation = null;
                StorageFile file = null;

                // Step 1: Create a lambda for the UI work
                // If you are doing async work on the UI thread inside this lambda, it must be awaited before the lambda returns to ensure Game Bar is
                // in the right state for the entirety of the foreground operation.
                // We recommend using the Dispatcher RunTaskAsync task extension to make this easier
                // Look at Extensions/DispatcherTaskExtensions.cs
                // For more information you can read this blog post: https://devblogs.microsoft.com/oldnewthing/20190327-00/?p=102364
                // For another approach more akin to how C++/WinRT handles awaitable thread switching, read this blog post: https://devblogs.microsoft.com/oldnewthing/20190328-00/?p=102368
                ForegroundWorkHandler foregroundWork = () =>
                {
                    var primaryTask = Task.Run(async () =>
                    {
                        await Dispatcher.RunTaskAsync(async () =>
                        {
                            var fileOpenPicker = new FileOpenPicker
                            {
                                SuggestedStartLocation = Windows.Storage.Pickers.PickerLocationId.DocumentsLibrary
                            };
                            fileOpenPicker.FileTypeFilter.Add("*");

                            filePickerOperation = fileOpenPicker.PickSingleFileAsync();
                            file = await filePickerOperation;
                        });
                    });

                    var continueTask = primaryTask.ContinueWith(t =>
                    {
                        if (t.IsFaulted)
                        {
                            // If the task was faulted, you will need to look for an inner exception, and if it exists, re-throw that exception
                            // This will ensure that the right exception will come back to you when ExecuteAsync returns
                            if (t.Exception.InnerException != null)
                            {
                                throw t.Exception.InnerException;
                            }
                        }

                        return true;
                    }).AsAsyncOperation();

                    return continueTask;
                };

                // Step 2: Create an instance of an XboxGameBarForegroundWorker and pass it your widget instance and the delegate you created in step 1
                var foregroundWorker = new XboxGameBarForegroundWorker(widget, foregroundWork);

                // Step 3: Create an event handler and subscribe to the CancelOperationRequested event on the XboxGameBarForegroundWorker object
                // This will get called if Game Bar needs your app to dismiss its foreground UI for any reason
                var handler = new TypedEventHandler<XboxGameBarForegroundWorker, object>((XboxGameBarForegroundWorker senderForegroundWorker, object args) =>
                {
                    // When this event is received you should cancel your UI operation
                    // In this example it's the async operation that was returned by calling PickSingleFileAsync
                    filePickerOperation.Cancel();
                });
                foregroundWorker.CancelOperationRequested += handler;

                // Step 4: Call ExecuteAsync on the XboxGameBarForegroundWorker object
                // This call will put Game Bar into the right state for foreground operations and then will run your delegate code at the right time
                await foregroundWorker.ExecuteAsync();

                // Step 5: Unsubscribe from the CancelOperationRequested event
                // Also, at this time you should let the XboxGameBarForegroundWorker object fall out of scope, as it is only allowed to be used once
                foregroundWorker.CancelOperationRequested -= handler;

                if (file != null)
                {
                    // Now, the picked file can be operated on, for this example a simple text field is updated with the file name
                    await PickedFile.Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                     {
                         PickedFile.Text = file.Name;
                     });
                }
                else
                {
                    // Operation was cancelled
                }
            });
        }
    }
}
