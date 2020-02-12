//
// Extension2.xaml.h
// Declaration of the Extension2 class.
//

#pragma once

#include "Extension2.g.h"

namespace UIExtSampleCX
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public ref class Extension2 sealed
    {
    public:
        Extension2();

    private:
        void MyButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
    };
}
