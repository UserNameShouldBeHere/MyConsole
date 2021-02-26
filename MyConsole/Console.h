#pragma once
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <boost/filesystem.hpp>
#include <algorithm>

namespace fs = boost::filesystem;

class Console {
	static Console* console;
	Console(std::string main_path) : current_path(main_path) {}

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	std::string current_path;

	// вспомогательные команды
	std::string transfer_string(std::string command);

	bool is_command(std::string command, std::string input, bool pass_context);

	void copy_directory(std::string old_path, std::string new_path);

	// команды
	void help();
	void size(std::string file);
	void get_files();
	void create_directory(std::string new_dir_name);
	void change_directory(std::string new_dir);
	void change_disk(std::string disk);
	void copy(std::string key);

	void print_main_path();
	void print_help_message(std::string command, std::string description);
	void print_error_message(std::string message);

	void open_file(std::string file_name);
	void open_cmd(std::string key);
	void open_notepad(std::string key);
	void open_google(std::string key);
	void open_visual_studio(std::string key);
	void open_git(std::string key);
	void rename(std::string key);
	void replace(std::string key);
	void shutdown(std::string key);

public:
	static Console* create_console(std::string main_path);

	void run();
};