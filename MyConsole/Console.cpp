#include "Console.h"

std::string Console::transfer_string(std::string command) {
	if (command[command.length() - 1] != ' ') command += " ";
	std::string new_string = command.erase(command.find_first_of(" "));
	return new_string;
}

bool Console::is_command(std::string command, std::string input, bool pass_context) {
	if (pass_context == true) {
		if (input.erase(command.length()) == command) return true;
		return false;
	}
	else {
		if (input == command) return true;
		return false;
	}
}

void Console::print_main_path() {
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 10));
	std::cout << current_path;
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 7));
	std::cout << '>';
}

void Console::print_error(std::string message) {
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 12));
	std::cout << "Ошибка: " << message << std::endl << std::endl;
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 7));
}

void Console::help() {
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 3));
	std::cout << "    cd <директория>             ";
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 6));
	std::cout << "Переход в другую директорию" << std::endl;

	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 3));
	std::cout << "    size <файл>                 ";
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 6));
	std::cout << "Выводи размер файла" << std::endl;

	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 3));
	std::cout << "    crd <директория>            ";
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 6));
	std::cout << "Создает новую директорию" << std::endl;

	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 3));
	std::cout << "    dir                         ";
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 6));
	std::cout << "Выводи список всех файлов в директории" << std::endl;

	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 3));
	std::cout << "    open <файл>                 ";
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 6));
	std::cout << "Открывает указанный файл" << std::endl;

	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 3));
	std::cout << "    rename                      ";
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 6));
	std::cout << "Поменять имя файла/директории" << std::endl;

	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 3));
	std::cout << "    replace                     ";
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 6));
	std::cout << "Поменять местоположение файла" << std::endl;

	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 3));
	std::cout << "    cmd <параметр>              ";
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 6));
	std::cout << "Открыть командную строку" << std::endl;

	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 3));
	std::cout << "    np                          ";
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 6));
	std::cout << "Открыть блокнот" << std::endl;

	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 3));
	std::cout << "    g                           ";
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 6));
	std::cout << "Открыть галю" << std::endl;

	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 3));
	std::cout << "    vs                          ";
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 6));
	std::cout << "Открыть visual studio" << std::endl;

	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 3));
	std::cout << "    git                         ";
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 6));
	std::cout << "Открыть git-cmd" << std::endl;

	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 3));
	std::cout << "    exit                        ";
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 6));
	std::cout << "Выйти из программы" << std::endl;
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 7));
}

void Console::size(std::string file) {
	if (file == "") {
		print_error("Введите название файла");
	}
	else {
		std::string new_path = current_path + file;
		if (fs::exists(new_path) == true) {
			if (fs::is_directory(new_path))
				print_error("Вы находитесь в директории. Выберите файл");
			else
				std::cout << "Размер файла составляет " << fs::file_size(new_path) << " байт" << std::endl;
		}
		else
			print_error("Данного файла не существует");
	}
}

void Console::get_files() {
	for (fs::directory_entry& file : fs::directory_iterator(current_path))
		if (fs::is_directory(file.path()) == true) {
			SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 3));
			std::cout << "    <DIR>   ";
			SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 6));
			std::cout << file.path().filename() << std::endl;
			SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 7));
		}
		else if (fs::is_regular_file(file.path()) == true) {
			SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 3));
			std::cout << "    <FILE>  ";
			SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 6));
			std::cout << file.path().filename() << std::endl;
			SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 7));
		}
}

void Console::create_directory(std::string new_dir_name) {
	std::string new_dir_path = current_path + new_dir_name + "/";
	fs::create_directory(new_dir_path);
}

void Console::change_directory(std::string new_dir) {
	std::string new_current = current_path;
	if (new_dir == ".." && (current_path.length() > 3)) {
		new_current.erase(new_current.find_last_of('/'));
		new_current.erase(new_current.find_last_of('/'));
		current_path = new_current + '/';
	}
	else if (new_dir != ".." && new_dir != "") {
		new_current += new_dir;
		if (fs::is_regular_file(new_current) == true)
			print_error("Вы не можете перейти в файл");
		else if (fs::is_directory(new_current) == true) {
			if (fs::exists(new_current + '/') == true) {
				new_current += '/';
				current_path = new_current;
			}
			else
				print_error("Данной директории не существует");
		}
		else
			print_error("Данной директории не существует");
	}
}

void Console::change_disk(std::string disk) {
	if (current_path[0] == 'C' && disk == "D") current_path = "D:/";
	if (current_path[0] == 'D' && disk == "C") current_path = "C:/";
}

void Console::rename(std::string key) {
	if (key.length() == 0) {
		std::string old_name;
		std::string new_name;
		std::cout << "Введите название файла: ";
		std::getline(std::cin, old_name);
		std::cout << "Введите новое название файла: ";
		std::getline(std::cin, new_name);

		std::string old_path = current_path + old_name;
		std::string new_path = current_path + new_name;
		if (fs::exists(old_path)) {
			fs::rename(old_path, new_path);
		}
		else {
			print_error("Данного файла/директории не существует");
		}
	}
}

void Console::replace(std::string key) {
	if (key.length() == 0) {
		std::string file_name;
		std::string old_path;
		std::string new_path;
		std::cout << "Введите название файла: ";
		std::getline(std::cin, file_name);
		std::cout << "Введите новое местоположение файла: ";
		std::getline(std::cin, new_path);

		old_path = current_path + file_name;

		if (fs::exists(old_path) == true) {
			if (new_path[0] == 'C' || new_path[0] == 'D') {
				if (fs::exists(new_path)) {
					if (fs::is_directory(new_path) == true) {
						if (new_path[new_path.length() - 1] != '/') new_path += "/";
						new_path += file_name;
						fs::rename(old_path, new_path);
					}
					else print_error("Выберите директорию, чтобы переместить файл");
				}
				else print_error("Данного пути не существует");
			}
			else {
				new_path = current_path + new_path;
				if (fs::exists(new_path)) {
					if (fs::is_directory(new_path) == true) {
						if (new_path[new_path.length() - 1] != '/') new_path += "/";
						new_path += file_name;
						fs::rename(old_path, new_path);
					}
					else print_error("Выберите директорию, чтобы переместить файл");
				}
				else print_error("Данного пути не существует");
			}
		}
		else print_error("Данного файла/директории не существует");
	}
}

void Console::open_file(std::string file_name) {
	std::string file_path;
	if (file_name.length() == 0) print_error("Введите название файла");
	else {
		file_path = current_path + file_name;
		if (fs::exists(file_path) == true)
			if (fs::is_regular_file(file_path) == true)
				ShellExecuteA(NULL, "open", file_path.c_str(), NULL, NULL, SW_RESTORE);
			else
				print_error("Вы можете открывать только файлы");
		else
			print_error("Данного файла не существует");
	}
}

void Console::open_cmd(std::string key) {
	if (key.length() == 0)
		ShellExecuteA(NULL, NULL, "C:/Windows/System32/cmd.exe", NULL, NULL, SW_RESTORE);
	if (key == "-admin")
		ShellExecuteA(NULL, "runas", "C:/Windows/System32/cmd.exe", NULL, NULL, SW_RESTORE);
}

void Console::open_notepad(std::string key) {
	if (key.length() == 0)
		ShellExecuteA(NULL, "open", "notepad.exe", NULL, NULL, SW_RESTORE);
}

void Console::open_google(std::string key) {
	if (key.length() == 0)
		ShellExecuteA(NULL, "open", "C:/Program Files (x86)/Google/Chrome/Application/chrome.exe", NULL, NULL, SW_SHOWMAXIMIZED);
}

void Console::open_visual_studio(std::string key) {
	if (key.length() == 0)
		ShellExecuteA(NULL, "open", "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/Common7/IDE/devenv.exe", NULL, NULL, SW_RESTORE);
}

void Console::open_git(std::string key) {
	if (key.length() == 0)
		ShellExecuteA(NULL, "open", "C:/Program Files/Git/git-cmd.exe", NULL, NULL, SW_RESTORE);
}

void Console::run() {
	while (true) {
		setlocale(LC_ALL, "russian");
		std::string command = "";
		std::string tmp_command = "";
		bool possible_command = false;

		print_main_path();

		std::getline(std::cin, command);

		if (command != "") {
			tmp_command = transfer_string(command);

			// посмотреть размер файла
			if (tmp_command == "help")
				if (is_command("help", command, false) == true) {
					help();
					possible_command = true;
				}

			// перейти в директорию
			if (tmp_command == "cd" || tmp_command == "cd..") {
				if (tmp_command == "cd" && is_command("cd", command, true) == true) {
					change_directory(command.erase(0, 3));
					possible_command = true;
				}
				else {
					possible_command = true;
					change_directory(command.erase(0, 2));
				}
			}

			// посмотреть размер файла
			if (tmp_command == "size")
				if (is_command("size", command, true) == true) {
					possible_command = true;
					size(command.erase(0, 5));
				}

			// создать директорию
			if (tmp_command == "crd")
				if (is_command("crd", command, true) == true) {
					possible_command = true;
					create_directory(command.erase(0, 4));
				}

			// просмотреть список всех файлов/папок в директории
			if (tmp_command == "dir")
				if (is_command("dir", command, true) == true) {
					possible_command = true;
					get_files();
				}

			// переименовать файл
			if (tmp_command == "rename")
				if (is_command("rename", command, true) == true) {
					possible_command = true;
					rename(command.erase(0, 7));
				}

			// перемещает файл
			if (tmp_command == "replace")
				if (is_command("replace", command, true) == true) {
					possible_command = true;
					replace(command.erase(0, 8));
				}

			// перейти на диск D
			if (tmp_command == "D:")
				if (is_command("D:", command, false) == true) {
					possible_command = true;
					change_disk("D");
				}

			// перейти на диск C
			if (tmp_command == "C:")
				if (is_command("C:", command, false) == true) {
					possible_command = true;
					change_disk("C");
				}

			// открывает файл
			if (tmp_command == "open")
				if (is_command("open", command, true) == true) {
					possible_command = true;
					open_file(command.erase(0, 5));
				}

			// открыть блокнот
			if (tmp_command == "cmd")
				if (is_command("cmd", command, true) == true) {
					possible_command = true;
					open_cmd(command.erase(0, 4));
				}

			// открыть блокнот
			if (tmp_command == "np")
				if (is_command("np", command, true) == true) {
					possible_command = true;
					open_notepad(command.erase(0, 3));
				}

			// открыть галю
			if (tmp_command == "g")
				if (is_command("g", command, true) == true) {
					possible_command = true;
					open_google(command.erase(0, 2));
				}

			// открыть visual studio
			if (tmp_command == "vs")
				if (is_command("vs", command, true) == true) {
					possible_command = true;
					open_visual_studio(command.erase(0, 3));
				}

			// открыть git
			if (tmp_command == "git")
				if (is_command("git", command, true) == true) {
					possible_command = true;
					open_git(command.erase(0, 4));
				}

			// выйти
			if (tmp_command == "exit")
				if (is_command("exit", command, false) == true) break;

			if (possible_command == false) print_error("'" + tmp_command + "'" + " не является командой или исполняемой программой");
		}

	}
	std::cout << "До связи" << std::endl;
}