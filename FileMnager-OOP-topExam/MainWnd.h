#pragma once
#include "ViewPort.h"
#include <conio.h>
#include <Windows.h>

class MainWnd
{
	ViewPort m_lvp;
	ViewPort m_rvp;
	ViewPort m_footer;
	CONSOLE_SCREEN_BUFFER_INFO m_screen;
	HANDLE m_console;
	bool m_fullScreen;
public:
	MainWnd()
	{
		m_lvp(20, 20);
		m_rvp(20, 20);
		m_fullScreen = false;
		ResizeByWindows();
	}
	void SwitchMode()
	{
		m_console = GetStdHandle(STD_OUTPUT_HANDLE);
		if (m_console == INVALID_HANDLE_VALUE)
		{
			std::cout << "\nError get console";
			return;
		}
		if (SetConsoleDisplayMode(m_console, (m_fullScreen) ? 2 : 1, 0))
		{
			m_fullScreen = !m_fullScreen;
			ResizeByWindows();
		}
		else
		{
			std::cout << "\nError switch FullScreen mode";
			system("pause");
			return;
		}
	}
	void ResizeByWindows()
	{
		m_console = GetStdHandle(STD_OUTPUT_HANDLE);
		if (m_console == INVALID_HANDLE_VALUE)
		{
			std::cout << "\nError get console";
			return;
		}
		if (!GetConsoleScreenBufferInfo(m_console, &m_screen))
		{
			std::cout << "\nError get screen size";
			return;
		}
		int w = m_screen.srWindow.Right - m_screen.srWindow.Left;
		int h = m_screen.srWindow.Bottom - m_screen.srWindow.Top;
		m_lvp(w / 2, h - 4);
		m_rvp(w / 2, h - 4);
		m_footer((w % 2 == 0) ? w : w - 1, 3);
	}
	void Show()
	{
		int lal, ral;
		system("cls");
		m_lvp.FillFiles("e:\\1\\");
		m_rvp.FillFiles("e:\\1\\");
		m_lvp.Fill();
		m_rvp.Fill();
		m_footer.Fill(1);
		ResizeByWindows();
		(m_lvp + m_rvp).Show();
		m_footer.Show();
		std::cout << "\n";
	}
	void LoopMsg()
	{
		ViewPort* aVP = &m_lvp;
		int key = 0;
		while (true)
		{
			Show();
			key = _getch();
			if (key == 224)
				key = _getch();
			std::cout << "\n" << key;
			switch (key)
			{
			case 72:

			case 80:
				aVP->MoveCursor(key);
				break;
			case 75:
			case 77:
				(aVP == &m_lvp) ? aVP = &m_rvp : aVP = &m_lvp;
				break;
			case 27://Esc
				return;
				break;
			case 134://F12 //fullscreen switch
				SwitchMode();
				break;
			}
		}
	}
};