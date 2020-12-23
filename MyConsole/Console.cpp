#include "Console.h"

Console* Console::console = nullptr;

Console* Console::create_console(std::string main_path) {
	if (console == nullptr) {
		console = new Console(main_path);
		return console;
	}
	else {
		std::cerr << "You can build only one console in programm" << std::endl;
		return console;
	}
}

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

void Console::print_help_message(std::string command, std::string description) {
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 3));
	std::cout << command;
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 6));
	std::cout << description << std::endl;
}

void Console::print_error_message(std::string message) {
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 12));
	std::cout << "Ошибка: " << message << std::endl << std::endl;
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 7));
}

void Console::help() {
	print_help_message("    cd <директория>             ", "Переход в другую директорию");
	print_help_message("    cmd <параметр>              ", "Открыть командную строку");
	print_help_message("    copy                        ", "Копирует файлы в необходимую папку");
	print_help_message("    crd <директория>            ", "Создает новую директорию");
	print_help_message("    dir                         ", "Выводи список всех файлов в директории");
	print_help_message("    exit                        ", "Выйти из программы");
	print_help_message("    g                           ", "Открыть галю");
	print_help_message("    git                         ", "Открыть git-cmd");
	print_help_message("    np                          ", "Открыть блокнот");
	print_help_message("    open <файл>                 ", "Открывает указанный файл");
	print_help_message("    size <файл>                 ", "Выводи размер файла");
	print_help_message("    rename                      ", "Поменять имя файла/директории");
	print_help_message("    replace                     ", "Поменять местоположение файла");
	print_help_message("    shutdown <параметр>         ", "Выключает компьютер");
	print_help_message("    vs                          ", "Открыть visual studio");
}

void Console::size(std::string file) {
	if (file == "")
		throw std::invalid_argument("Введите название файла");
	else {
		std::string new_path = current_path + file;
		if (fs::exists(new_path) == true) {
			if (fs::is_directory(new_path))
				print_error_message("Вы находитесь в директории. Выберите файл");
			else
				std::cout << "Размер файла составляет " << fs::file_size(new_path) << " байт" << std::endl;
		}
		else
			throw std::invalid_argument("Данного файла не существует");
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
			print_error_message("Вы не можете перейти в файл");
		else if (fs::is_directory(new_current) == true && new_dir != ".") {
			if (fs::exists(new_current + '/') == true) {
				new_current += '/';
				current_path = new_current;
			}
			else
				throw std::invalid_argument("Данной директории не существует");
		}
		else
			throw std::invalid_argument("Данной директории не существует");
	}
}

void Console::change_disk(std::string disk) {
	if (current_path[0] == 'C' && disk == "D") current_path = "D:/";
	if (current_path[0] == 'D' && disk == "C") current_path = "C:/";
}

void Console::copy(std::string key) {
	if (key.length() == 0) {
		bool file_is_in_folder = false;
		std::string file_name;
		std::string old_path;
		std::string new_directory;
		std::string new_path;
		std::cout << "Введите название файла: ";
		std::getline(std::cin, file_name);
		std::cout << "Введите папку, куда необходимо скопировать файл: ";
		std::getline(std::cin, new_directory);

		if (file_name[0] != 'C' && file_name[0] != 'D')
			old_path = current_path + file_name;
		else
			old_path = file_name;

		if (fs::exists(old_path) == true) {
			if (new_directory[0] != 'C' && new_directory[0] != 'D')
				new_path = current_path + new_directory;
			else
				new_path = new_directory;

			if (fs::exists(new_path)) {
				if (new_path[new_path.length() - 1] != '/')
					new_path += "/";

				for (fs::directory_entry& path : fs::directory_iterator(new_path)) {
					std::string file_name = old_path;
					file_name.erase(0, file_name.find_last_of("/") + 1);
					if (path.path().filename() == file_name)
						file_is_in_folder = true;
#ifdef DEBUG
					std::cout << path.path().filename() << std::endl;
					std::cout << file_name << std::endl;
#endif
				}

				std::string tmp_path = old_path;
				new_path += tmp_path.erase(0, tmp_path.find_last_of("/") + 1);

#ifdef DEBUG
				std::cout << old_path << std::endl;
				std::cout << new_path << std::endl;
#endif

				if (file_is_in_folder == false) {
					fs::copy(old_path, new_path);
				}
				else
					throw std::invalid_argument("Файл уже существует данной папке");
			}
			else
				throw std::invalid_argument("Данного пути не существует");
		}
		else
			throw std::invalid_argument("Данного файла/директории не существует");
	}
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
		if (fs::exists(old_path))
			fs::rename(old_path, new_path);
		else 
			throw std::invalid_argument("Данного файла/директории не существует");
	}
}

void Console::replace(std::string key) {
	if (key.length() == 0) {
		bool file_is_in_folder = false;
		std::string file_name;
		std::string old_path;
		std::string new_directory;
		std::string new_path;
		std::cout << "Введите название файла: ";
		std::getline(std::cin, file_name);
		std::cout << "Введите новое местоположение файла: ";
		std::getline(std::cin, new_directory);

		if (file_name[0] != 'C' && file_name[0] != 'D')
			old_path = current_path + file_name;
		else
			old_path = file_name;

		if (fs::exists(old_path) == true) {
			if (new_directory[0] != 'C' && new_directory[0] != 'D')
				new_path = current_path + new_directory;
			else
				new_path = new_directory;

			if (fs::exists(new_path)) {
				if (fs::is_directory(new_path) == true) {
					if (new_path[new_path.length() - 1] != '/')
						new_path += "/";

					for (fs::directory_entry& path : fs::directory_iterator(new_path)) {
						std::string file_name = old_path;
						file_name.erase(0, file_name.find_last_of("/") + 1);
						if (path.path().filename() == file_name)
							file_is_in_folder = true;
#ifdef _DEBUG
						std::cout << path.path().filename() << std::endl;
						std::cout << file_name << std::endl;
#endif
					}

					std::string tmp_path = old_path;
					new_path += tmp_path.erase(0, tmp_path.find_last_of("/") + 1);

#ifdef _DEBUG
					std::cout << old_path << std::endl;
					std::cout << new_path << std::endl;
#endif

					if (file_is_in_folder == false)
						fs::rename(old_path, new_path);
					else
						throw std::invalid_argument("Файл уже существует данной папке");
				}
				else
					throw std::invalid_argument("Выберите директорию, чтобы переместить файл");
			}
			else
				throw std::invalid_argument("Данного пути не существует");
		}
		else 
			throw std::invalid_argument("Данного файла/директории не существует");
	}
}

void Console::shutdown(std::string key) {
	if (key.length() == 0) 
		system("shutdown /s /t 60000");
	else {
		if (atoi(key.c_str()) >= 0 && atoi(key.c_str()) <= 315360000)
			system(("shutdown /s /t " + key).c_str());
		else 
			throw std::invalid_argument("Введите корректное значение задержки в мсек.");
	}
}

void Console::open_file(std::string file_name) {
	std::string file_path;
	if (file_name.length() == 0) throw std::invalid_argument("Введите название файла");
	else {
		file_path = current_path + file_name;
		if (fs::exists(file_path) == true)
			if (fs::is_regular_file(file_path) == true)
				ShellExecuteA(NULL, "open", file_path.c_str(), NULL, NULL, SW_RESTORE);
			else
				throw std::invalid_argument("Вы можете открывать только файлы");
		else
			throw std::invalid_argument("Данного файла не существует");
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

		try {

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

				// переместить файл
				if (tmp_command == "replace")
					if (is_command("replace", command, true) == true) {
						possible_command = true;
						replace(command.erase(0, 8));
					}

				// копировать файл
				if (tmp_command == "copy")
					if (is_command("copy", command, true) == true) {
						possible_command = true;
						copy(command.erase(0, 5));
					}

				// выключить комп
				if (tmp_command == "shutdown")
					if (is_command("shutdown", command, true) == true) {
						possible_command = true;
						shutdown(command.erase(0, 9));
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

				// открыть консоль
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
					if (is_command("exit", command, false) == true)
						break;

				if (possible_command == false)
					throw std::invalid_argument("'" + tmp_command + "'" + " не является командой или исполняемой программой");
			}
		}

		catch (std::invalid_argument ex) {
			print_error_message(ex.what());
		}
	}


	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 8));
	std::cout << "=================" << std::endl;
	std::cout << "\\  До связи...  /" << std::endl;
	std::cout << " ===============" << std::endl;
	Sleep(1000);
}