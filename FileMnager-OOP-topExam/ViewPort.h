#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
namespace fs = std::filesystem;
class ViewPort
{
	int m_width;
	int m_height;
	std::vector<std::string> m_vp;
	std::vector<fs::path> m_files;
	int m_active;
	int m_maxActive;
public:
	ViewPort() :m_width{ 0 }, m_height{ 0 }, m_active{ 0 }, m_maxActive{ 0 }
	{
		m_vp.clear();
	}
	ViewPort(int w, int h)
	{
		ReSize(w, h);
	}
	ViewPort& ReSize(int w, int h)
	{
		m_width = w;
		m_height = h;
		m_vp.resize(m_height);
		return *this;
	}
	ViewPort& operator()(int w, int h)
	{
		return ReSize(w, h);
	}
	ViewPort& FillFiles(fs::path fn)
	{
		m_files.clear();
		m_files.push_back(fn);
		int i = 1;
		for (auto p : fs::directory_iterator(m_files[0]))
		{
			m_files.push_back(p);
			i++;
		}
		m_maxActive = m_files.size() - 1;
		return *this;
	}
	ViewPort& MoveCursor(int key)
	{
		switch (key)
		{
		case 72:
			m_active--;
			if (m_active < 1)
				m_active = m_maxActive;
			break;
		case 80:
			m_active++;
			if (m_active > m_maxActive)
				m_active = 1;
			break;
			//case 13:
			//	return active;
		}
		return *this;
	}
	ViewPort& Fill(int key = 0)
	{
		std::string help1{ "F1 - Open" };
		std::string help2{ "F2 - Create" };
		std::string help3{ "F3 - Copy" };
		std::string help4{ "F4 - Move" };
		std::string help5{ "F5 - Delete" };
		switch (key)
		{
		case 1:
			m_vp[0] = "\xc9" + std::string(m_width - 2, '\xcd') + "\xbb";
			m_vp[1] = "\xba" + std::string(m_width / 6 - help1.length(), ' ') + help1 + std::string(m_width / 6 - help2.length(), ' ') + help2 +
				std::string(m_width / 6 - help3.length(), ' ') + help3 + std::string(m_width / 6 - help4.length(), ' ') + help4 +
				std::string(m_width / 6 - help5.length(), ' ') + help5 + std::string(m_width / 6 + m_width % 6 - 2, ' ') + "\xba";
			m_vp[2] = "\xc8" + std::string(m_width - 2, '\xcd') + "\xbc";
			break;
		default:
			m_vp[0] = "\xc9" + std::string(m_width - 2, '\xcd') + "\xbb";
			for (int i = 1; i < m_height - 1; i++)
			{
				if (i < m_files.size())
					m_vp[i] = "\xba" + std::string((m_active == i) ? "\x1b[30;47m" : "") + m_files[i].filename().string() + std::string((m_active == i) ? "\x1b[0m" : "") + std::string(m_width - 2 - m_files[i].filename().string().length(), ' ') + "\xba";
				else
					m_vp[i] = "\xba" + std::string(m_width - 2, ' ') + "\xba";
			}
			m_vp[m_height - 1] = "\xc8" + std::string(m_width - 2, '\xcd') + "\xbc";
			break;
		}
		
		return *this;
	}
	ViewPort& Show()
	{
		for (int i = 0; i < m_height; i++)
			std::cout << "\n" << m_vp[i];
		return *this;
	}
	ViewPort operator+(const ViewPort& b)
	{
		ViewPort c(m_width + b.m_width, std::max(m_height, b.m_height));
		for (int i = 0; i < c.m_height; i++)
		{
			std::string lvp = std::string(m_width, ' ');
			std::string rvp = std::string(b.m_width, ' ');
			if (i < m_height)
				lvp = m_vp[i];
			if (i < b.m_height)
				rvp = b.m_vp[i];
			c.m_vp[i] = lvp + rvp;
		}
		return c;
	}
};
