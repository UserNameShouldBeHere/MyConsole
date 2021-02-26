#include <iostream>
#include "Console.h"

int main() {
	setlocale(LC_ALL, "russian");
	SetConsoleTitle(L"Console");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	Console* filesystem = Console::create_console("C:\\Users\\");

	filesystem->run();

	return 0;
}