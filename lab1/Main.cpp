#include <iostream>
#include <fstream>
#include <conio.h>
#include <string>
#include <windows.h>
#include "employee.h"
#pragma warning(disable: 4996)
//C4996 is generated for the line on which the function is declared and for the line on which the function is used.
//Some CRT functions have been deprecated in favor of new, more secure functions. 
//создание файла
char* get_binary_data_for_creator(char* binary, int n) {
	char data[200] = "Creator.exe ";
	strcat(data, binary);
	strcat(data, " ");
	strcat(data, to_string(n).c_str());
	return data;
}
//создание процесса
void runCreatorProcess(char data[]) {
	STARTUPINFO si;
	PROCESS_INFORMATION piApp;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);


	if (!CreateProcess(NULL, data, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp)) {
		std::cout << "Process is not created\n";
		return;
	}
	WaitForSingleObject(piApp.hProcess, INFINITE);
	CloseHandle(piApp.hThread);
	CloseHandle(piApp.hProcess);

}

//чтение из бинарного файла
void readFile(char* fileName) {
	ifstream in;
	in.open(fileName, ios::binary);
	cout << "Employees: \n";
	while (EOF != in.peek()) {
		Employee employee;
		in.read((char*)&employee, sizeof(Employee));
		std::cout << "Employee number:\n" << employee.num << "\nEmployee name:\n" << employee.name << "\nEmployee hours:\n" << employee.hours << "\n\n";
	}
	in.close();
}

//получение данных для репортера
char* get_binary_data_for_reporter(char* binaryFile, char* fileName, int pay) {
	char data[100] = "Reporter.exe ";
	strcat(data, binaryFile);
	strcat(data, " ");
	strcat(data, fileName);
	strcat(data, " ");
	strcat(data, to_string(pay).c_str());
	return data;
}
//запуск процесса-обработика функции репортера
void runReporterProcess(char dataForReporter[]) {
	STARTUPINFO si;
	PROCESS_INFORMATION piApp;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);


	if (!CreateProcess(NULL, dataForReporter, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp)) {
		std::cout << "Process is not created\n";
		return;
	}
	WaitForSingleObject(piApp.hProcess, INFINITE);
	CloseHandle(piApp.hThread);
	CloseHandle(piApp.hProcess);

}


void readFile(char* fileName) {
	ifstream in;

	char line[200];
	in.open(fileName);
	while (!in.eof()) {
		in.getline(line, 200);
		std::cout << line << "\n";
	}
}

int main() {

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	char* binaryFileName = new char[30];
	std::cout << "Enter binary file name:\n";
	std::cin >> binaryFileName;
	std::cout << "\n";

	int reportAmount;
	std::cout << "Enter amount of reports:\n";
	std::cin >> reportAmount;
	std::cout << "\n";

	char dataForCreator[100];
	strcpy(dataForCreator, get_binary_data_for_creator(binaryFileName, reportAmount));
	runCreatorProcess(dataForCreator);

	readBinaryFile(binaryFileName);

	char* reporFileName = new char[30];
	std::cout << "\nEnter report file name:\n";
	std::cin >> reporFileName;
	std::cout << "\n";

	double pay;
	std::cout << "Enter pay for hour:\n";
	std::cin >> pay;
	std::cout << "\n";

	char dataForReporter[100];

	strcpy(dataForReporter, get_binary_data_for_reporter(binaryFileName ,reporFileName, pay));
	runReporterProcess(dataForReporter);

	readFile(reporFileName);


	return 0;
}
