#include <iostream>
#include <windows.h>
#include <conio.h> 
#include <shellapi.h>
#include <strsafe.h>
#include <commctrl.h>
#include "Resource.h"

#define WM_SHOWTASK (WM_USER+114)
#define IDR_PAUSE 12

using namespace std;
RECT rswls;

void BallonMsg(int MsgType,HWND hwnd,LPSTR INFOTITLETEXT,LPSTR INFOTEXT)//气泡通知 
{
	NOTIFYICONDATAA nid = {};
	nid.cbSize = sizeof(nid);
	nid.hWnd = hwnd;
	nid.uFlags = NIF_MESSAGE | NIF_GUID | NIF_INFO;
	nid.uCallbackMessage=WM_USER;
	StringCchCopyA(nid.szInfoTitle, ARRAYSIZE(nid.szInfoTitle),INFOTITLETEXT);
	StringCchCopyA(nid.szInfo, ARRAYSIZE(nid.szInfo),INFOTEXT);
    nid.uTimeout=10000;
	switch(MsgType)
	{
		case 1:{
			nid.dwInfoFlags=NIIF_INFO;
			Shell_NotifyIcon(NIM_MODIFY, &nid);
			break;
		}
		case 2:{
			nid.dwInfoFlags=NIIF_WARNING;
			Shell_NotifyIcon(NIM_MODIFY, &nid);
			break;
		}
		case 3:{
			nid.dwInfoFlags=NIIF_ERROR;
			Shell_NotifyIcon(NIM_MODIFY, &nid);
			break;
		}
		case 4:{
			nid.dwInfoFlags=NIIF_USER;
			Shell_NotifyIcon(NIM_MODIFY, &nid);
			break;
		}
		default:{
			nid.dwInfoFlags=NIIF_NONE | NIIF_NOSOUND;
			Shell_NotifyIcon(NIM_MODIFY, &nid);
			break;
		}
	} 
 } 
 
void TrayWindowIcon(HINSTANCE hInstance,HWND hWnd,LPSTR TIPTEXT)//托盘图标 
 {
 	HINSTANCE hins = hInstance; 
	NOTIFYICONDATAA nid = {};
	nid.hWnd = hWnd;
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_GUID | NIF_INFO;
	nid.hIcon =LoadIcon(hins, MAKEINTRESOURCE(IDI_NOTIFICATIONICON)); 
	StringCchCopyA(nid.szTip, ARRAYSIZE(nid.szTip),TIPTEXT);
	nid.uCallbackMessage=WM_USER;
	Shell_NotifyIcon(NIM_ADD, &nid);
 }
 
void OnTrayIcon(HWND hWnd,LPARAM lParam)
{
	POINT pt;//用于接收鼠标坐标
	MENUINFO mi;
	int menu_rtn;//用于接收菜单选项返回值
	HMENU hmenu = CreatePopupMenu();//生成菜单
	HBITMAP hbmp = (HBITMAP)LoadImageA(NULL,"bg.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_DEFAULTSIZE|LR_CREATEDIBSECTION);
	mi.cbSize = sizeof(MENUINFO);
	mi.fMask = MIM_BACKGROUND | MIM_STYLE;
	mi.dwStyle = MNS_NOCHECK | MNS_AUTODISMISS;
	mi.hbrBack = CreatePatternBrush(hbmp);
	SetMenuInfo(hmenu,&mi);
	AppendMenu(hmenu, MF_STRING, IDM_HIDEFORGE, "隐藏forge服务端");
	AppendMenu(hmenu, MF_STRING, IDM_SHOWFORGE, "解除隐藏forge服务端");
	AppendMenu(hmenu, MF_STRING, IDM_HIDEORIGON, "隐藏原版服务端");
	AppendMenu(hmenu, MF_STRING, IDM_SHOWORIGON, "解除隐藏原版服务端");
	AppendMenu(hmenu, MF_STRING, IDM_ABOUT, "关于我们");
	AppendMenu(hmenu, MF_STRING, IDM_EXIT, "退出");
	if (lParam == WM_RBUTTONDOWN||lParam == WM_LBUTTONDOWN)
	{
		GetCursorPos(&pt);//取鼠标坐标
		SetForegroundWindow(hWnd);
		EnableMenuItem(hmenu, IDR_PAUSE, MF_GRAYED);
		menu_rtn = TrackPopupMenu(hmenu, TPM_RETURNCMD, pt.x, pt.y, NULL, hWnd, NULL );//显示菜单并获取选项ID
		if (menu_rtn == IDM_HIDEFORGE){
			HWND mcsf = FindWindow("ConsoleWindowClass","MCS-forge-1.19.1");
			GetClientRect(mcsf,&rswls);
			SetWindowPos(mcsf,HWND_BOTTOM,0,0,rswls.right,rswls.bottom,SWP_HIDEWINDOW);
			}
		if (menu_rtn == IDM_SHOWFORGE){
			HWND mcsf = FindWindow("ConsoleWindowClass","MCS-forge-1.19.1");
			GetClientRect(mcsf,&rswls);
			SetWindowPos(mcsf,HWND_TOP,0,0,rswls.right,rswls.bottom,SWP_SHOWWINDOW);
			}
		if (menu_rtn == IDM_HIDEORIGON){
			HWND mcso = FindWindow("ConsoleWindowClass","MCS-1.19.1");
			GetClientRect(mcso,&rswls);
			SetWindowPos(mcso,HWND_BOTTOM,0,0,rswls.right,rswls.bottom,SWP_HIDEWINDOW);
			}
		if (menu_rtn == IDM_SHOWORIGON){
			HWND mcso = FindWindow("ConsoleWindowClass","MCS-1.19.1");
			GetClientRect(mcso,&rswls);
			SetWindowPos(mcso,HWND_TOP,0,0,rswls.right,rswls.bottom,SWP_SHOWWINDOW);
			}
		if (menu_rtn == IDM_ABOUT){
			system("start http://43.139.35.247");
			}
		if (menu_rtn == IDM_EXIT){
		    BallonMsg(0,hWnd,"已退出"," ");
		    Sleep(100);
		    PostQuitMessage(0);
			}
	}
}

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		case WM_USER:{
			OnTrayIcon(hwnd, lParam);
			break;
		}
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	void TrayWindowIcon(HINSTANCE hInstance,HWND hWnd,LPSTR TIPTEXT); 
	void OnTrayIcon(HWND hWnd,LPARAM lParam);
	void BallonMsg(int MsgType,HWND hwnd,LPSTR INFOTITLETEXT,LPSTR INFOTEXT);
	
	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","SeewoFU",WS_MINIMIZE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		64, /* width */
		48, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
		TrayWindowIcon(hInstance,hwnd,"MC服务器窗口控制");
		BallonMsg(0,hwnd,"已启动"," ");

	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}
