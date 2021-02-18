#include <windows.h>
#include <d2d1.h>
#include <d2d1helper.h>
#pragma comment(lib, "d2d1")
#include <Wincodec.h>
#pragma comment(lib, "Windowscodecs")

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
	IWICImagingFactory *pFactory = NULL;
	IWICBitmapDecoder *pDecoder = NULL;

	RECT rc; GetClientRect(Form1, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(Form1, size),
		&pRT);

	pRT->BeginDraw();
	pRT->Clear(D2D1::ColorF(D2D1::ColorF::Black));

	CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		(LPVOID*)&pFactory
	);

	pFactory->CreateDecoderFromFilename(
		L"kep.jpg",
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder);

	IWICBitmapFrameDecode *pSource;
	IWICFormatConverter *pConverter;
	pDecoder->GetFrame(0, &pSource);
	pFactory->CreateFormatConverter(&pConverter);

	pConverter->Initialize(
		pSource,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.f,
		WICBitmapPaletteTypeMedianCut
	);

	ID2D1Bitmap *ppBitmap;
	pRT->CreateBitmapFromWicBitmap(
		pConverter,
		NULL,
		&ppBitmap
	);

	ID2D1BitmapBrush *m_pBitmapBrush;
	pRT->CreateBitmapBrush(
		ppBitmap,
		&m_pBitmapBrush
	);
	m_pBitmapBrush->SetExtendModeX(D2D1_EXTEND_MODE_MIRROR);

	D2D1_RECT_F rc2;
	rc2.left = rc.left;
	rc2.top = rc.top;
	rc2.right = rc.right;
	rc2.bottom = rc.bottom;
	pRT->FillRectangle(&rc2, m_pBitmapBrush);

	pRT->EndDraw();
}

