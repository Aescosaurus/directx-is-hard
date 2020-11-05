#pragma once

#include <windows.ui.core.h>

using namespace Windows::UI::Core;

class AescKbd
{
public:
	enum class Key
	{
		Up = int( Windows::System::VirtualKey::Up ),
		Down = int( Windows::System::VirtualKey::Down ),
		Left = int( Windows::System::VirtualKey::Left ),
		Right = int( Windows::System::VirtualKey::Right )
	};
public:
	static bool KeyIsPressed( Key key )
	{
		static const auto wnd = CoreWindow::GetForCurrentThread();
		return( wnd->GetAsyncKeyState( Windows::System::VirtualKey( key ) ) == CoreVirtualKeyStates::Down );
	}
};