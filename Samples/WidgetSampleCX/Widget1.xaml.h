//
// Widget1.xaml.h
// Declaration of the Widget1 class.
//

#pragma once

#include "Widget1.g.h"

namespace WidgetSampleCX
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public ref class Widget1 sealed
    {
    public:
        Widget1();

    private:
        void MyButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
    };
}
