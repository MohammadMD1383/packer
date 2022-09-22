#ifndef PACKER_UTIL_HPP
#define PACKER_UTIL_HPP

#include <cstdio>
#include <termios.h>
#include <unistd.h>

typedef decltype(getchar()) KeyboardKey;

KeyboardKey readKey() {
	static termios oldTerminal, newTerminal;
	
	//tcgetattr gets the parameters of the current terminal
	//STDIN_FILENO will tell tcgetattr that it should write the settings
	// of stdin to oldTerminal
	tcgetattr(STDIN_FILENO, &oldTerminal);
	//now the settings will be copied
	newTerminal = oldTerminal;
	
	//two of the c_lflag will be turned off
	//ECHO which is responsible for displaying the input of the user in the terminal
	//ICANON is the essential one! Normally this takes care that one line at a time will be processed
	//that means it will return if it sees a "\n" or an EOF or an EOL
	newTerminal.c_lflag &= ~(ICANON | ECHO);
	
	//Those new settings will be set to STDIN
	//TCSANOW tells tcsetattr to change attributes immediately.
	tcsetattr(STDIN_FILENO, TCSANOW, &newTerminal);
	
	//now the char wil be requested
	auto key = getchar();
	
	//the oldTerminal settings will be written back to STDIN
	tcsetattr(STDIN_FILENO, TCSANOW, &oldTerminal);
	
	return key;
}

#endif //PACKER_UTIL_HPP
