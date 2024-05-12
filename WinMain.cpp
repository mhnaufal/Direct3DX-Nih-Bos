#include "ChiliWin.h"

/* NOTE:
* https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-winmain
*/
int CALLBACK WinMain(
	HINSTANCE hInstance,		/* NOTE: context in vulkan, contains data of current window program */
	HINSTANCE hPrevInstance,	/* NOTE: old API, always nullptr now */
	LPSTR lpCmdLine,			/* NOTE; command line param that we can pass through window */
	int nCmdShow				/* NOTE: command show option */
) {
	/* NOTE:
	* Register window class to this program. Remember this is a "class"
	* https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerclassexa
	* https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexa
	*/
	const LPCSTR pClassName = "3DirectXNihBos";
	WNDCLASSEX wc{ 0 };
	wc.cbSize = sizeof(wc);				/* NOTE: sizeof this struct */
	wc.style = CS_OWNDC;				/* NOTE: class style enum */
	//wc.lpfnWndProc = DefWindowProc;	/* NOTE pointer to window procedure that this class want to use */
	wc.lpfnWndProc = WndProc;			/* NOTE: window procedure is just what the window do when it's running */
	wc.cbClsExtra = 0;					/* NOTE: extra byte we want to allocate */
	wc.cbWndExtra = 0;					/* NOTE: number of extra byte allocated */
	wc.hInstance = hInstance;			/* NOTE: instance of this window program */
	wc.hbrBackground = nullptr;			/* NOTE: background color */
	wc.hCursor = nullptr;
	wc.lpszMenuName = nullptr;
	wc.hIcon = nullptr;					/* NOTE: window icon that we want to use */
	wc.hIconSm = nullptr;				/* small icon that we use */
	wc.lpszClassName = pClassName;		/* the window class name that we use */

	RegisterClassEx(&wc);

	/* NOTE:
	* Create a window based on the window class that we use
	* https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa
	*/
	HWND hwnd = CreateWindowEx(
		0,
		pClassName,									/* NOTE: which window class it is */
		g_window_name,								/* NOTE: window name */
		WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION,	/* NOTE: window style that we want to use */
		300, 300,									/* NOTE: window position */
		SCREEN_WIDTH, SCREEN_HEIGHT,				/* NOTE: window size */
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	/* NOTE:
	* Show the window that we just made
	* https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow
	*/
	ShowWindow(
		hwnd,
		SW_SHOW /* NOTE: enum how we want to show it */
	);

	/* NOTE:
	* Initialize Directx3D 
	*/
	InitDX3D(hwnd);

	InitPipeline();
	InitGraphic();

	/* NOTE:
	* Looping the event message between the window and program
	* https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getmessage
	*/
	BOOL gResult{};
	MSG message{};							/* NOTE: Windows message struct */
	while (gResult = GetMessage(&message, nullptr, 0, 0) != 0) {
		if (gResult == -1) {
			return -1;
		}

		TranslateMessage(&message);			/* NOTE: translate virtual key message into character message */
		DispatchMessage(&message);

		RenderFrame();
	}

	/* NOTE: Deinitialize DirectX3D */
	CleanDX3D();

	return (int)message.wParam; // 313
}


/////////////////////////////
// Windows callback procedure
/////////////////////////////
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(313);		/* NOTE: tell the window to close the program when 'X' button is clicked */
		break;
	case WM_KEYDOWN:				/* NOTE: keystroke input */
		if (wparam == 'F') {
			SetWindowText(hwnd, (LPCSTR)"New Window Name");
		}
		break;
	case WM_KEYUP:
		if (wparam == 'F') {
			SetWindowText(hwnd, g_window_name);
		}
		break;
	case WM_CHAR:					/* NOTE: sequence of word input */
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


////////////////////////////
// Initialize DirectX 11
///////////////////////////
void InitDX3D(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC swc_struct{}; /* NOTE: struct holds information about swap chain */

	ZeroMemory(&swc_struct, sizeof(DXGI_SWAP_CHAIN_DESC));

	/* NOTE: fill the swap chain struct. 
		https://learn.microsoft.com/en-us/windows/win32/api/dxgi/ns-dxgi-dxgi_swap_chain_desc
		https://learn.microsoft.com/en-us/windows/win32/direct3ddxgi/dxgi-usage
	*/
	swc_struct.BufferCount = 1;									/* NOTE: one back buffer & one front buffer (in pair) */
	swc_struct.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	/* NOTE: color format used. Use 32 bit color */
	swc_struct.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	/* NOTE: swap chain behaviour flags. How swap chain to be used */
	swc_struct.OutputWindow = hWnd;								/* NOTE: which window context to be used */
	swc_struct.SampleDesc.Count = 4;							/* NOTE: anti-aliasing. max 4 min 1 */
	swc_struct.Windowed = TRUE;									/* NOTE: Windowed or fullscreen */
	swc_struct.BufferDesc.Width = SCREEN_WIDTH;
	swc_struct.BufferDesc.Height = SCREEN_HEIGHT;
	swc_struct.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	/* NOTE: allow fullscreen mode */

	// https://learn.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-d3d11createdeviceandswapchain
	D3D11CreateDeviceAndSwapChain(
		NULL,							/* NOTE: graphic adapter that we use */
		D3D_DRIVER_TYPE_HARDWARE,		/* NOTE: is it hardware or software rendering? */
		NULL,							/* NOTE: set the software code */
		NULL,							/* NOTE: flags */
		NULL,							/* NOTE: spesific hardware features */
		NULL,							/* NOTE: how manny feature that we want */
		D3D11_SDK_VERSION,				/* NOTE: version */
		&swc_struct,					/* NOTE: pointer to the swapchain */
		&swapchain,						/* NOTE: pointer of pointer to swapchain object */
		&dev,							/* NOTE: pointer of pointer to device object */
		NULL,							/* NOTE: pointer to feature level */
		&devcon							/* NOTE: pointer of pointer to device context object */
	);

	/* NOTE:
	* Get the address of the back buffer
	* https://learn.microsoft.com/en-us/windows/win32/api/dxgi/nf-dxgi-idxgiswapchain-getbuffer
	*/
	ID3D11Texture2D* pBackBuffer{};												/* NOTE: texture/image to be rendered */
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer); /* NOTE: find the backbuffer in swapchain and use it to create backbuffer texture object */

	/* NOTE: Use the back buffer to create the render target */
	dev->CreateRenderTargetView(pBackBuffer, NULL, &backBuffer);		/* NOTE: create the render target object */
	pBackBuffer->Release();

	/* NOTE: set the render target as the back buffer */
	devcon->OMSetRenderTargets(1, &backBuffer, NULL);					/* NOTE: set the render target to be used */

	/* NOTE: set the viewport */
	D3D11_VIEWPORT viewport{};
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;

	devcon->RSSetViewports(1, &viewport); /* NOTE: activate the viewport to be used */
}

void RenderFrame(void)
{
	devcon->ClearRenderTargetView(backBuffer, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)); /* NOTE: fills the render target with a certain color */

	/* NOTE: select which vertex buffer to be displayed */
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

	/* NOTE: select primitive we are using */
	//devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST); /* Note: shows as triangle */
	devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);	/* NOTE: shows as line */

	/* NOTE: draw the vertex buffer */
	devcon->Draw(5, 0);

	swapchain->Present(0, 0);	/* NOTE: "swap" the backbuffer to the frontbuffer to be shown */

	//devcon->ClearRenderTargetView(backBuffer, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//swapchain->Present(0, 0);
}

void InitPipeline(void)
{
	ID3D10Blob* VS;
	ID3D10Blob* PS;
	/* NOTE:
	* https://learn.microsoft.com/en-us/windows/win32/direct3d11/d3dx11compilefromfile
	* "Load the shaders1 file, then find the VShader function, compiles it as version 4.0 vertex shader and store the result in blob VS
	*/
	D3DX11CompileFromFile("shaders1.shader", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
	D3DX11CompileFromFile("shaders1.shader", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);

	/* NOTE:
	* create the shader COM object
	* 1st = address of the compiled shader/vertex
	* 2nd = size of the file data
	* 4th = address of the shader object
	*/
	dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

	/* NOTE: set the shader to the active shader */
	devcon->VSSetShader(pVS, 0, 0);
	devcon->PSSetShader(pPS, 0, 0);

	/* NOTE: create input layout object */
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	dev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
	devcon->IASetInputLayout(pLayout);
}

void InitGraphic(void)
{
	/* NOTE: the triangle vertices
			locations/coordinates are in X,Y,Z 
	*/
	VERTEX OurVertices[] =
	{
		//{0.0f, 0.5f, 0.9f, D3DXCOLOR(1.0f, 0.0f, 0.2f, 1.0f)},
		//{0.45f, -0.5, 0.0f, D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f)},
		//{-0.45f, -0.5f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
		//{0.1f, -0.8f, 0.0f, D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f)}

		{-0.5f, 0.8f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
		{0.0f, 0.5f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
		{0.5f, 0.8f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
		{0.0f, -0.6f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
		{-0.5f, 0.8f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
	};

	/* NOTE: create vertex buffer, each vertex buffer can only handle one given points(?) */
	D3D11_BUFFER_DESC bd{};
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX) * 5;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	dev->CreateBuffer(&bd, NULL, &pVBuffer);

	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));
	devcon->Unmap(pVBuffer, NULL);
}

void CleanDX3D(void)
{
	swapchain->SetFullscreenState(FALSE, NULL); /* NOTE: turn off the fullscreen */

	/* NOTE: release the shaders COM objects */
	pVS->Release();
	pPS->Release();

	swapchain->Release();
	backBuffer->Release();
	dev->Release();
	devcon->Release();
}
