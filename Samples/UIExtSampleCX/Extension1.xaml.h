//
// Extension1.xaml.h
// Declaration of the Extension1 class.
//

#pragma once

#include "Extension1.g.h"

namespace UIExtSampleCX
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class Extension1 sealed
	{
	public:
		Extension1();

	private:
		void MyButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
