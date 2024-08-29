#pragma once

#include "Widget1.g.h"

namespace winrt::WidgetFTSample::implementation
{
    struct Widget1 : Widget1T<Widget1>
    {
        Widget1();

        winrt::fire_and_forget CreateFTFactoryButton_Click(
            winrt::Windows::Foundation::IInspectable sender, 
            winrt::Windows::UI::Xaml::RoutedEventArgs e);

        winrt::fire_and_forget ReleaseFTFactoryButton_Click(
            winrt::Windows::Foundation::IInspectable sender,
            winrt::Windows::UI::Xaml::RoutedEventArgs e);

        winrt::fire_and_forget CreateSampleComponentButton_Click(
            winrt::Windows::Foundation::IInspectable sender,
            winrt::Windows::UI::Xaml::RoutedEventArgs e);

        winrt::fire_and_forget ReleaseSampleComponentButton_Click(
            winrt::Windows::Foundation::IInspectable sender,
            winrt::Windows::UI::Xaml::RoutedEventArgs e);

        winrt::fire_and_forget CallDemoSyncButton_Click(
            winrt::Windows::Foundation::IInspectable sender,
            winrt::Windows::UI::Xaml::RoutedEventArgs e);

        winrt::fire_and_forget CallDemoAsyncButton_Click(
            winrt::Windows::Foundation::IInspectable sender,
            winrt::Windows::UI::Xaml::RoutedEventArgs e);

        winrt::fire_and_forget DemoBoolPropertyCheckBox_Click(
            winrt::Windows::Foundation::IInspectable sender,
            winrt::Windows::UI::Xaml::RoutedEventArgs e);

    public: // From Widget1.idl

        bool CreateFTFactoryButtonEnabled();
        bool ReleaseFTFactoryButtonEnabled();
        bool CreateSampleComponentButtonEnabled();
        bool ReleaseSampleComponentButtonEnabled();
        bool CallDemoSyncButtonEnabled();
        bool CallDemoAsyncButtonEnabled();
        bool DemoBoolPropertyCheckBoxEnabled();
        bool IsDemoBoolPropertyChecked();

        winrt::event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;

    private: // Internal state property methods

        winrt::WidgetFT::WidgetFTFactory WidgetFTFactory();
        void WidgetFTFactory(winrt::WidgetFT::WidgetFTFactory const& value);

        winrt::WidgetFT::SampleComponent SampleComponent();
        void SampleComponent(winrt::WidgetFT::SampleComponent const& value);

        void RaiseDemoBoolPropertyChanged(bool newValue);
        void RaiseObjectStatesChanged();
        winrt::fire_and_forget RaisePropertyChanged(winrt::hstring propertyName);

    private:
        winrt::WidgetFT::SampleComponent::DemoBoolPropertyChanged_revoker m_demoBoolPropertyChangedRevoker;

        winrt::event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;

        wil::srwlock m_ftFactoryLock;
        winrt::WidgetFT::WidgetFTFactory m_ftFactory{ nullptr };

        wil::srwlock m_sampleComponentLock;
        winrt::WidgetFT::SampleComponent m_sampleComponent{ nullptr };
    };
}

namespace winrt::WidgetFTSample::factory_implementation
{
    struct Widget1 : Widget1T<Widget1, implementation::Widget1>
    {
    };
}
