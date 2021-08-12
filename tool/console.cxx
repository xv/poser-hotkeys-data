/* console.cxx
 * Copyright (C) 2021 Jad Altahan (https://github.com/xv)
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#pragma once
#include "console.h"

Console::Console() {
	m_pWi = new Screen;
	if (!m_pWi)
		m_pWi = nullptr;

	m_pWi->handle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (m_pWi->handle != nullptr)
	{
		if (!GetConsoleScreenBufferInfo(m_pWi->handle, &m_pWi->bufferInfo))
			m_pWi->handle = nullptr;
	}
}

void Console::SetTextColor(Color fg, Color bg)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (m_pWi == nullptr || !GetConsoleScreenBufferInfo(m_pWi->handle, &csbi))
		return;

	WORD color = csbi.wAttributes;
	if (fg != Console::Color::COLOR_NONE)
		color = (color & 0xFFF0) | (int)fg;

	if (bg != Console::Color::COLOR_NONE)
		color = (color & 0xFF0F) | ((int)bg << 4);

	if (m_pWi->handle != nullptr)
		SetConsoleTextAttribute(m_pWi->handle, color);
}

void Console::ResetTextColor()
{
	if (m_pWi->handle != nullptr)
		SetConsoleTextAttribute(m_pWi->handle, m_pWi->bufferInfo.wAttributes);
}

Console::~Console()
{
	if (m_pWi)
		delete m_pWi;
}