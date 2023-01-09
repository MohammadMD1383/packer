#ifndef PACKER_UTIL_HPP
#define PACKER_UTIL_HPP

#ifndef WINDOWS
#include <cstdio>
#include <unistd.h>
#include <termios.h>
#else

#include <windows.h>
#include <conio.h>

#endif

#ifndef WINDOWS
typedef decltype(getchar()) KeyboardKey;
#else
typedef decltype(_getch()) KeyboardKey;
#endif

KeyboardKey readKey() {
#ifndef WINDOWS
	static termios oldTerminal, newTerminal;
	
	tcgetattr(STDIN_FILENO, &oldTerminal);
	newTerminal = oldTerminal;
	newTerminal.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newTerminal);
	auto key = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldTerminal);
	
	return key;
#else
	return _getch();
#endif
}

#ifdef WINDOWS

void setConsoleColor(uint8_t color) {
	static auto hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	static uint8_t defaultColor = []() {
		CONSOLE_SCREEN_BUFFER_INFO info{};
		GetConsoleScreenBufferInfo(hConsole, &info);
		return info.wAttributes;
	}();
	
	SetConsoleTextAttribute(hConsole, color == 0 ? defaultColor : color);
}

const char *windowsInfo() {
	setConsoleColor(0x3F);
	return "";
}

const char *windowsWarning() {
	setConsoleColor(0x6F);
	return "";
}

const char *windowsError() {
	setConsoleColor(0x4F);
	return "";
}

const char *windowsNormal() {
	setConsoleColor(0);
	return "";
}

#endif

#endif //PACKER_UTIL_HPP
