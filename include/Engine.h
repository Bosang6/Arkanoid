#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Input.h"
#include "Game.h"
#include "D3DRenderer.h"
#include "Camera2D.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;

class Engine
{
public:
	Engine() = default;
	Engine(const Engine&) = delete;
	~Engine() = default;

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_gameName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	Input* m_Input = nullptr;
	Game* m_Game = nullptr;
	D3DRenderer* m_Renderer = nullptr;
	Camera2D* m_Camera2D = nullptr;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static Engine* EngineHandle = nullptr;