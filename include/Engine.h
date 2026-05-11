#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Input.h"
#include "Game.h"
#include "D3DRenderer.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;


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
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static Engine* EngineHandle = nullptr;