#include <iostream>
#include <windows.h>
#include <vector>
#include <iterator>

using namespace std;

int _size;
vector<int> arr;

volatile int _min;
volatile int _max;
volatile double avg;


DWORD WINAPI min_max(void * _arr) {
	int* arr = (int *)_arr;
	_min = arr[0];
	_max = arr[0];
	for (int i = 0; i < _size; i++) {
		if (arr[i] > _max) {
			_max = arr[i];
		}
		Sleep(7);
		if (arr[i] < _min) {
			_min = arr[i];
		}
		Sleep(7);
	}
	cout << "Min: " << _min << "\n";
	cout << "Max: " << _max << "\n";
	return 0;
}

DWORD WINAPI average(void * _arr) {
	int* arr = (int*)_arr;
	int sum = 0;
	for (int i = 0; i < _size; i++) {
		sum += arr[i];
		Sleep(12);
	}
	avg = 1. * sum / _size;
	cout << "Average: " << avg << "\n";
	return 0;
}




int main() {
	
	HANDLE minMaxHThread;
	DWORD minMaxIDThread;
	HANDLE averageHThread;
	DWORD averageIDThread;
	int* arr;




	cout << "Enter size of array:\n";
	cin >> _size;
	arr = new int[_size];
	cout << "Enter elements of array:\n";

	for (int i = 0; i < _size; i++) {
		cin >> arr[i];
	}

	minMaxHThread = CreateThread(NULL, 0, min_max, arr, 0, &minMaxIDThread);
	averageHThread = CreateThread(NULL, 0, average, arr, 0, &averageIDThread);
	
	WaitForSingleObject(minMaxHThread, INFINITE);
	WaitForSingleObject(averageHThread, INFINITE);

	CloseHandle(minMaxHThread);
	CloseHandle(averageHThread);


	for (int i = 0; i < _size; i++) {
		if (arr[i] == _min || arr[i] == _max) {
			arr[i] = avg;
		}
	}
	for (int i = 0; i < _size; i++) {
		cout << arr[i] << " ";
	}
	cout << "\n";


	return 0;
}
