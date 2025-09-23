#include "pch.h"

// Globals
HWND g_hwnd = nullptr;

typedef IRenderer* (*FNP_Renderer_Create)();
typedef void (*FNP_Renderer_Destroy)(IRenderer* renderer);

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
	// Register class
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"DX11SpriteWndClass";
	RegisterClassEx(&wc);

	RECT wr = { 0,0,1280,800 };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
	g_hwnd = CreateWindowEx(0, wc.lpszClassName, L"DirectX11 Sprite Template", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top, nullptr, nullptr, hInstance, nullptr);

	ShowWindow(g_hwnd, nCmdShow);

	// Create renderer.
	HMODULE rendererDLL = LoadLibrary(L"../DLL/Renderer_Debug_x64.dll");
	FNP_Renderer_Create Renderer_Create = (FNP_Renderer_Create)GetProcAddress(rendererDLL, "Renderer_Create");
	FNP_Renderer_Destroy Renderer_Destroy = (FNP_Renderer_Destroy)GetProcAddress(rendererDLL, "Renderer_Destroy");

	IRenderer* renderer = Renderer_Create();
	renderer->Init(g_hwnd, 1280, 800);

	// Create manager and system.
	EntityManager entityManager;
	
	ComponentArray<TransformComponent> transforms;
	ComponentArray<SpriteComponent> sprites;
	ComponentArray<MovementComponent> movements;

	TransformSystem transfromSystem{ &transforms };
	InputSystem inputSystem{ &transforms };
	MovementSystem movementSystem{ &transforms, &movements, &inputSystem };
	RenderSystem renderSystem{ &transforms, &sprites, renderer };

	ResourceManager resourceManager;

	// Create entity.
	// - Add transform component.
	Entity entity = entityManager.Create();
	transforms.Add(entity, TransformComponent{ Vector2(100.0f, 100.0f), 0.0f, Vector2(1.0f) });

	// - Add movement component.
	movements.Add(entity, MovementComponent{ 200.0f });
	
	// - Add sprite component.
	std::vector<BYTE> pixels;
	int width;
	int height;
	LoadImageRGBA(L"beara.png", pixels, width, height);
	uint32 textureId = resourceManager.LoadTexture(renderer, L"beara.png", pixels, width, height);
	sprites.Add(entity, SpriteComponent{ textureId, Vector2(0.0f, 0.0f), Vector2((float)width, (float)height), Vector2(0.0f), Vector2(1.0f), Vector4(1.0f), 0 });

	// Timing
	auto prev = std::chrono::high_resolution_clock::now();

	// Main loop
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_KEYDOWN || msg.message == WM_KEYUP)
			{
				bool isKeyDown = (msg.message == WM_KEYDOWN);
				switch (msg.wParam)
				{
				case VK_LEFT:
					inputSystem.SetKey(entity, Input_Left, isKeyDown);
					break;
				case VK_RIGHT:
					inputSystem.SetKey(entity, Input_Right, isKeyDown);
					break;
				case VK_UP:
					inputSystem.SetKey(entity, Input_Up, isKeyDown);
					break;
				case VK_DOWN:
					inputSystem.SetKey(entity, Input_Down, isKeyDown);
					break;
				default:
					/** For debug */
					break;
				}
			}
		}

		auto now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> dt = now - prev;
		prev = now;
		float deltaTime = dt.count();
		
		movementSystem.Update(deltaTime);
		transfromSystem.Update(deltaTime);
		renderSystem.Render();
		renderer->RenderFrame();
	}

	resourceManager.UnloadAll(renderer);
	Renderer_Destroy(renderer);
	FreeLibrary(rendererDLL);

	return 0;
}

// Process window messages (we'll still use GetAsyncKeyState for continuous input)
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_DESTROY: PostQuitMessage(0); return 0;
	case WM_SIZE:
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
