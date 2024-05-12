/******************************************************************************************
*	Chili Direct3D Engine																  *
*	Copyright 2018 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of Chili Direct3D Engine.											  *
*																						  *
*	Chili Direct3D Engine is free software: you can redistribute it and/or modify		  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili Direct3D Engine is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili Direct3D Engine.  If not, see <http://www.gnu.org/licenses/>.    *
******************************************************************************************/
#pragma once

/*
// target Windows 7 or later
#define _WIN32_WINNT 0x0601
#include <sdkddkver.h>
#define NOTEXTMETRIC
*/

#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMEMMGR
#define NOMETAFILE
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE
#define STRICT
#define NOMINMAX

/*
* Include files
*/
// Window 
#include <Windows.h>
#include <windowsx.h>

// DirectX11
#include <d3d11.h>		// Direct3D 11 main library (mandatory)
#include <d3dx11.h>		// Direct3D 11 extension library (optional)
#include <d3dx10.h>		// Direct3D 10 extension library (optional)

#pragma comment (lib, "d3d11.lib") // include the Direct3D 11 lib to our executable
#pragma comment (lib, "d3dx11.lib") 
#pragma comment (lib, "d3dx10.lib")

// STL
#include <iostream>

/* 
* Global Declarations
*/
auto g_window_name = (LPCSTR)"3DirectXNihBos";
IDXGISwapChain* swapchain;					// the pointer to the swap chain interface
ID3D11Device* dev;							// the pointer to our Direct3D device interface
ID3D11DeviceContext* devcon;				// the pointer to our Direct3D device context (physical device)
ID3D11RenderTargetView* backBuffer;			// variable that holds information about the render target
ID3D11VertexShader* pVS;					// COM object for vertex shader
ID3D11PixelShader* pPS;						// COM object for pixel shader
ID3D11Buffer* pVBuffer;						// vertex buffer
ID3D11InputLayout* pLayout;

constexpr auto SCREEN_WIDTH = 800;
constexpr auto SCREEN_HEIGHT = 600;

struct VERTEX
{
	FLOAT X, Y, Z;
	D3DXCOLOR Color;
};

/* 
* Function prototypes
*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
void InitDX3D(HWND hWnd);     // sets up and initializes Direct3D
void CleanDX3D(void);         // closes Direct3D and releases memory
void RenderFrame(void);
void InitPipeline(void);
void InitGraphic(void);

