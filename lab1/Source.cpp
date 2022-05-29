#include <windows.h>
#include <iostream>
#include <fstream>
#include "employee.h"

int main(int argc, char* argv[])
{
	char* filename = argv[1];


	ofstream out(filename, ios::binary);

	int num = atoi(argv[2]);

	for (int i = 0; i < num; i++)
	{
		Employee employee;
		std::cout << "Enter employee number:\n";
		std::cin >> employee.num;

		std::cout << "Enter employee name:\n";
		std::cin >> employee.name;

		std::cout << "Enter employee hours:\n";
		std::cin >> employee.hours;

		out.write((char*)&employee, sizeof(struct Employee));
	}

	out.close();

	return 0;
}
