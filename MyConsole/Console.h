#pragma once
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

class Console {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	std::string current_path;

	std::string transfer_string(std::string);

	bool is_command(std::string, std::string, bool);

	void help();
	void size(std::string);
	void get_files();
	void create_directory(std::string);
	void change_directory(std::string);
	void change_disk(std::string);
	void copy(std::string);

	void print_main_path();
	void print_error(std::string);

	void open_file(std::string);
	void open_cmd(std::string);
	void open_notepad(std::string);
	void open_google(std::string);
	void open_visual_studio(std::string);
	void open_git(std::string);
	void rename(std::string);
	void replace(std::string);
	void shutdown(std::string);

public:
	Console(std::string path) : current_path(path) {}
	Console() : current_path("C:/Users/USER/Downloads/") {}

	void run();
};