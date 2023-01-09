#ifndef PACKER_UTIL_HPP
#define PACKER_UTIL_HPP

#ifndef WINDOWS
#include <cstdio>
#include <unistd.h>
#include <termios.h>
#else

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

#endif //PACKER_UTIL_HPP
