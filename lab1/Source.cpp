#include <windows.h>
#include <iostream>
#include <fstream>

using namespace std;

struct Employee
{
	int num;
	char name[10];
	double hours;
};

int main(int argc, char* argv[])
{
	char* filename = argv[1];


	ofstream out(filename, ios::binary);

	int num = atoi(argv[2]);

	for (int i = 0; i < num; i++)
	{
		Employee employee;
		cout << "Enter employee number:\n";
		cin >> employee.num;

		cout << "Enter employee name:\n";
		cin >> employee.name;

		cout << "Enter employee hours:\n";
		cin >> employee.hours;

		out.write((char*)&employee, sizeof(struct Employee));
	}

	out.close();

	return 0;
}
