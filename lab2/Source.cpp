#include <iostream>
#include <windows.h>
#include <vector>
#include <iterator>
#include "array.h"


DWORD WINAPI min_max(void * _arr, int size) {
	myArray* array = static_cast<myArray*>(_arr);
	int min = arr[0];
	int max = arr[0];
	for (int i = 0; i < size; i++) {
		if (arr[i] > max) {
			max = arr[i];
		}
		Sleep(7);
		if (arr[i] < _min) {
			min = arr[i];
		}
		Sleep(7);
	}
	std::cout << "Min: " << min << "\n";
	std::cout << "Max: " << max << "\n";
	return 0;
}

DWORD WINAPI average(void * _arr) {
myArray* array = static_cast<myArray*>(_arr);
	int sum = 0;
	for (int i = 0; i < _size; i++) {
		sum += arr[i];
		Sleep(12);
	}
	avg = 1. * sum / _size;
	std::cout << "Average: " << avg << "\n";
	return 0;
}




int main() {
	
	HANDLE minMaxHThread;
	DWORD minMaxIDThread;
	HANDLE averageHThread;
	DWORD averageIDThread;

	int size;


	std::cout << "Enter size of array:\n";
	cin >> size;
	myArray *arr = new int[size];
	array->storage = new int[size];
    	array->size = size;
	cout << "Enter elements of array:\n";

	for(int i = 0; i < size; i = -~i) {
        std::cout << "Input " << -~i << " element of array: ";
        std::cin >> a.storage[i];
	
    }
	minMaxHThread = CreateThread(NULL, 0, min_max, arr, 0, &minMaxIDThread);
	averageHThread = CreateThread(NULL, 0, average, arr, 0, &averageIDThread);
	
	WaitForSingleObject(minMaxHThread, INFINITE);
	WaitForSingleObject(averageHThread, INFINITE);

	CloseHandle(minMaxHThread);
	CloseHandle(averageHThread);


	for (int i = 0; i < size; i++) {
		if (arr[i] == _min || arr[i] == _max) {
			arr[i] = avg;
		}
	}
	for (int i = 0; i < size; i++) {
		std::cout << arr[i] << " ";
	}
	std::cout << "\n";


	return 0;
}
