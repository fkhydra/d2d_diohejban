#include <windows.h>
#include <d2d1.h>
#include <d2d1helper.h>
#pragma comment(lib, "d2d1")

ID2D1Factory* pD2DFactory = NULL;
ID2D1HwndRenderTarget* pRT = NULL;

#define HIBA_00 TEXT("Error:Program initialisation process.")
HINSTANCE hInstGlob;
int SajatiCmdShow;
char szClassName[] = "WindowsApp";
HWND Form1; //Ablak kezeloje

LRESULT CALLBACK WndProc0(HWND, UINT, WPARAM, LPARAM);
void D2D_rajzolas(ID2D1HwndRenderTarget* pRT);

//*********************************
//A windows program bel�p�si pontja
//*********************************
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("StdWinClassName");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass0;
	SajatiCmdShow = iCmdShow;
	hInstGlob = hInstance;

	//*********************************
	//Ablak oszt�lyp�ld�ny elok�sz�t�se
	//*********************************
	wndclass0.style = CS_HREDRAW | CS_VREDRAW;
	wndclass0.lpfnWndProc = WndProc0;
	wndclass0.cbClsExtra = 0;
	wndclass0.cbWndExtra = 0;
	wndclass0.hInstance = hInstance;
	wndclass0.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass0.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass0.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wndclass0.lpszMenuName = NULL;
	wndclass0.lpszClassName = TEXT("WIN0");

	//*********************************
	//Ablak oszt�lyp�ld�ny regisztr�ci�ja
	//*********************************
	if (!RegisterClass(&wndclass0))
	{
		MessageBox(NULL, HIBA_00, TEXT("Program Start"), MB_ICONERROR);
		return 0;
	}

	//*********************************
	//Ablak l�trehoz�sa
	//*********************************
	Form1 = CreateWindow(TEXT("WIN0"),
		TEXT("Form1"),
		(WS_OVERLAPPED | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX),
		50,
		50,
		800,
		600,
		NULL,
		NULL,
		hInstance,
		NULL);

	//*********************************
	//Ablak megjelen�t�se
	//*********************************
	ShowWindow(Form1, SajatiCmdShow);
	UpdateWindow(Form1);

	//*********************************
	//Ablak �zenetkezel�s�nek aktiv�l�sa
	//*********************************
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

//*********************************
//Az ablak callback f�ggv�nye: esem�nykezel�s
//*********************************
LRESULT CALLBACK WndProc0(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (message)
	{
		//*********************************
		//Ablak l�trehoz�sakor k�zvetlen�l
		//*********************************
	case WM_CREATE:
		/*Init*/;
		D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
		pD2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(
				hwnd, D2D1::SizeU(800, 600)),
			&pRT);
		return 0;
		//*********************************
		//Ablak kliens ter�let�nek �jrarajzol�sa
		//*********************************
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		D2D_rajzolas(pRT);
		return 0;
		//*********************************
		//Ablak bez�r�sa, er�forr�sok felszabad�t�sa
		//*********************************
	case WM_CLOSE:
		pRT->Release();
		pD2DFactory->Release();
		DestroyWindow(hwnd);
		return 0;
		//*********************************
		//Ablak megsemmis�t�se
		//*********************************
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

void D2D_rajzolas(ID2D1HwndRenderTarget* pRT)
{
	pRT->BeginDraw();
	pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));

	ID2D1SolidColorBrush *ecset;
	pRT->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black, 1.0f),
		&ecset);

	// Forgatas
	pRT->SetTransform(
		D2D1::Matrix3x2F::Rotation(
			45.0f,
			D2D1::Point2F(150.0f, 150.0f))
	);
	pRT->DrawRectangle(D2D1::RectF(100, 100, 200, 200), ecset);

	// Atmeretezes
	pRT->SetTransform(
		D2D1::Matrix3x2F::Scale(
			D2D1::Size(1.3f, 1.3f),
			D2D1::Point2F(150.0f, 150.0f))
	);
	pRT->DrawRectangle(D2D1::RectF(100, 100, 200, 200), ecset);

	// Torzitas
	pRT->SetTransform(
		D2D1::Matrix3x2F::Skew(
			45.0f,
			0.0f,
			D2D1::Point2F(150.0f, 150.0f))
	);
	pRT->DrawRectangle(D2D1::RectF(100, 100, 200, 200), ecset);

	// Eltolas
	pRT->SetTransform(D2D1::Matrix3x2F::Translation(200, 10));
	pRT->DrawRectangle(D2D1::RectF(100, 100, 200, 200), ecset);

	pRT->EndDraw();
}
