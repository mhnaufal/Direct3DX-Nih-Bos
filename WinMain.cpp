/*
* Include files
*/
// Window 
#include <Windows.h>
#include <windowsx.h>

// DirectX11
#include <d3d11.h> // Direct3D 11 main library (mandatory)
#include <d3dx11.h> // Direct3D 11 extension library (optional)
#include <d3dx10.h> // Direct3D 10 extension library (optional)

#pragma comment (lib, "d3d11.lib") // include the Direct3D 11 lib to our executable
#pragma comment (lib, "d3dx11.lib") 
#pragma comment (lib, "d3dx10.lib")

// STL
#include <iostream>

// global declarations
IDXGISwapChain* swapchain;             // the pointer to the swap chain interface
ID3D11Device* dev;                     // the pointer to our Direct3D device interface
ID3D11DeviceContext* devcon;           // the pointer to our Direct3D device context (physical device)
auto wind_name_g = (LPCSTR)"3DirectXNihBos";
ID3D11RenderTargetView* backBuffer;		// variable that holds information about the render target

// function prototypes
void InitDX3D(HWND hWnd);     // sets up and initializes Direct3D
void CleanDX3D(void);         // closes Direct3D and releases memory
void RenderFrame(void);

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

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
	case WM_KEYDOWN: // keystroke input
		if (wparam == 'F') {
			SetWindowText(hwnd, (LPCSTR)"New Window Name");
		}
		break;
	case WM_KEYUP:
		if (wparam == 'F') {
			SetWindowText(hwnd, wind_name_g);
		}
		break;
	case WM_CHAR: // sequence of word input
		break;
	case WM_LBUTTONDBLCLK:
		int x_pos{}, y_pos{};
		x_pos = GET_X_LPARAM(lparam);
		y_pos = GET_Y_LPARAM(lparam);
		std::cout << x_pos << " -- " << y_pos << std::endl;
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
		wind_name_g, // window name
		WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION, // window style that we want to use
		300, 300, // window position
		SCREEN_WIDTH, SCREEN_HEIGHT, // window size
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

	/* Initialize Directx3D */
	InitDX3D(hwnd);

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

		RenderFrame();
	}

	/* Deinitialize DirectX3D */
	CleanDX3D();

	return message.wParam; // 313
}

/*
* Initialize the DirectX 3D 11
*/
void InitDX3D(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC swc_struct{}; // struct holds information about swap chain

	ZeroMemory(&swc_struct, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain struct. 
	// https://learn.microsoft.com/en-us/windows/win32/api/dxgi/ns-dxgi-dxgi_swap_chain_desc
	// https://learn.microsoft.com/en-us/windows/win32/direct3ddxgi/dxgi-usage
	swc_struct.BufferCount = 1;									// one back buffer & one front buffer (in pair)
	swc_struct.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// color format used. Use 32 bit color
	swc_struct.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// swap chain behaviour flags. How swap chain to be used
	swc_struct.OutputWindow = hWnd;								// which window context to be used
	swc_struct.SampleDesc.Count = 4;							// anti-aliasing. max 4 min 1
	swc_struct.Windowed = TRUE;									// Windowed or fullscreen
	swc_struct.BufferDesc.Width = SCREEN_WIDTH;
	swc_struct.BufferDesc.Height = SCREEN_HEIGHT;
	swc_struct.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// allow fullscreen mode

	// https://learn.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-d3d11createdeviceandswapchain
	D3D11CreateDeviceAndSwapChain(
		NULL,							// graphic adapter that we use
		D3D_DRIVER_TYPE_HARDWARE,		// is it hardware or software rendering?
		NULL,							// set the software code
		NULL,							// flags
		NULL,							// spesific hardware features
		NULL,							// how manny feature that we want
		D3D11_SDK_VERSION,				// version
		&swc_struct,					// pointer to the swapchain
		&swapchain,						// pointer of pointer to swapchain object
		&dev,							// pointer of pointer to device object
		NULL,							// pointer to feature level
		&devcon							// pointer of pointer to device context object
	);

	/* Get the address of the back buffer
	* https://learn.microsoft.com/en-us/windows/win32/api/dxgi/nf-dxgi-idxgiswapchain-getbuffer
	*/
	ID3D11Texture2D* pBackBuffer{};												// texture/image to be rendered
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer); // find the backbuffer in swapchain and use it to create backbuffer texture object

	/* Use the back buffer to create the render target */
	dev->CreateRenderTargetView(pBackBuffer, NULL, &backBuffer);		// create the render target object
	pBackBuffer->Release();

	/* set the render target as the back buffer */
	devcon->OMSetRenderTargets(1, &backBuffer, NULL);					// set the render target to be used

	/* set the viewport */
	D3D11_VIEWPORT viewport{};
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;

	devcon->RSSetViewports(1, &viewport); // activate the viewport to be used

}

void RenderFrame(void)
{
	devcon->ClearRenderTargetView(backBuffer, D3DXCOLOR(0.5f, 0.2f, 0.8f, 1.0f)); // fills the render target with a certain color

	swapchain->Present(0, 0);	// "swap" the backbuffer to the frontbuffer to be shown

	//devcon->ClearRenderTargetView(backBuffer, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//swapchain->Present(0, 0);
}

void CleanDX3D(void)
{
	swapchain->SetFullscreenState(FALSE, NULL); // turn off the fullscreen
	swapchain->Release();
	backBuffer->Release();
	dev->Release();
	devcon->Release();
}
