#include <Windows.h>

/*
* Window procedure
*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CLOSE: 
		PostQuitMessage(313); // tell the window to close the program when 'X' button is clicked
		break;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

/*
* https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-winmain
*/
int CALLBACK WinMain(
	HINSTANCE hInstance, // context in vulkan, contains data of current window program
	HINSTANCE hPrevInstance, // old API, always nullptr now
	LPSTR lpCmdLine, // command line param that we can pass through window
	int nCmdShow // command show option
) {
	/*
	* Register window class to this program. Remember this is a "class"
	* https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerclassexa
	* https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexa
	*/
	const LPCSTR pClassName = "3DirectXNihBos";
	WNDCLASSEX wc{ 0 };
	wc.cbSize = sizeof(wc); // sizeof this struct
	wc.style = CS_OWNDC; // class style enum
	//wc.lpfnWndProc = DefWindowProc; // pointer to window procedure that this class want to use
	wc.lpfnWndProc = WndProc; // window procedure is just what the window do when it's running
	wc.cbClsExtra = 0; // extra byte we want to allocate
	wc.cbWndExtra = 0; // number of extra byte allocated
	wc.hInstance = hInstance; // instance of this window program
	wc.hbrBackground = nullptr; // background color
	wc.hCursor = nullptr;
	wc.lpszMenuName = nullptr;
	wc.hIcon = nullptr; // window icon that we want to use
	wc.hIconSm = nullptr; // small icon that we use
	wc.lpszClassName = pClassName; // the window class name that we use

	RegisterClassEx(&wc);

	/*
	* Create a window based on the window class that we use
	* https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa
	*/
	HWND hwnd = CreateWindowEx(
		0,
		pClassName, // which window class it is
		(LPCSTR)"3DirectXNihBos", // window name
		WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION, // window style that we want to use
		300, 300, // window position
		640, 480, // window size
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	/*
	* Show the window that we just made
	* https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow
	*/
	ShowWindow(
		hwnd,
		SW_SHOW // enum how we want to show it
	);

	/*
	* Looping the event message between the window and program
	* https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getmessage
	*/
	BOOL gResult{};
	MSG message{}; // Windows message struct
	while (gResult = GetMessage(&message, nullptr, 0, 0) != 0) {
		if (gResult == -1) {
			return -1;
		}

		TranslateMessage(&message); // translate virtual key message into character message
		DispatchMessage(&message);
	}

	return message.wParam; // 313
}