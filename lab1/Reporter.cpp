include <windows.h>
#include <iostream>
#include <fstream>
#include <list>
#include "employee.h"

#pragma warning(disable : 4996)

//C4996 is generated for the line on which the function is declared and for the line on which the function is used.
//Some CRT functions have been deprecated in favor of new, more secure functions.


list<Employee> readEmployees(char* fileName) {
	ifstream in;
	in.open(fileName, ios::binary);
	list<Employee> employees;

	while (in.peek() != EOF)
	{
		Employee employee;
		in.read((char*)&employee, sizeof(Employee));
		employees.push_back(employee);
	}

	in.close();

	return employees;
}

bool employeeComparator(const Employee& employee1, const Employee& employee2) {
	return employee1.num < employee2.num;
}

int main(int argc, char* argv[]) {
	ofstream out;
	out.open(argv[2]);
	double pay = atof(argv[3]);

	list<Employee> employees = readEmployees(argv[1]);
	employees.sort(employeeComparator);

	out << "Data: " " << argv[0] << "\"\n";
	for (auto const& iterator : employees) {
		out << iterator.num << "\t" << iterator.name << "\t" << iterator.hours * pay << "\n";
	}
	out.close();
	return 0;
}
