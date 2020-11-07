#include <iostream>
#include "Console.h"

int main() {
	setlocale(LC_ALL, "russian");
	SetConsoleTitle(L"Console");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	Console filesystem("C:/Users/USER/Downloads/");

	filesystem.run();

	return 0;
}