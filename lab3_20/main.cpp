#include <iostream>
#include <vector>
#include <thread>
#include "threadInfo.h"


using std::cin;
using std::cout;
using std::vector;
using std::thread;

const int kSleepTime = 5;

std::mutex markerMutex;
std::mutex startMutex;
std::mutex stateMutex;

std::condition_variable start;
std::condition_variable stateChanged;

vector<int> arr;

bool isReady = false;
vector<bool> halteredThreads;


int generateRandomInt(const int&, const int&);

void printArray(const vector<int>&);

void markerFunction(threadInfo*);

bool checkResult(void);


int main(void)
{
	int sizeOfArray = generateRandomInt(5, 15);
	arr = vector<int>(sizeOfArray, 0);

	printArray(arr);

	int numberOfMarkers = generateRandomInt(3, 7);
	vector<thread> markers;
	vector<threadInfo*> markerInfo;

	halteredThreads = vector<bool>(numberOfMarkers, false);
	vector<bool> terminatedThreads(numberOfMarkers, false);
	for (int i = 0; i < numberOfMarkers; i++)
	{
		threadInfo* tmp = new threadInfo();
		tmp->size = sizeOfArray;
		tmp->idx = i + 1;
		tmp->actions[0] = false;
		tmp->actions[1] = false;

		markerInfo.push_back(tmp);
		markers.push_back(thread(markerFunction, markerInfo.back()));
	}

	cout << "The threads are ready to start!\n";
	isReady = true;
	start.notify_all();

	int numberOfHaltredThreads = 0;
	// logic loop
	while (numberOfHaltredThreads != numberOfMarkers)
	{
		std::unique_lock<std::mutex> lock(stateMutex);
		stateChanged.wait(lock, []()-> bool {
			for (const bool& elem : halteredThreads)
			{
				if (!elem)
				{
					return false;
				}
			}
			return true;
		});
	
		printArray(arr);

		cout << "All threads are halted!\n";
		cout << "Enter the marker threads's ID to eliminate:\t";
		int userInputID = -1;
		cin >> userInputID;

		if (0 >= userInputID || userInputID > numberOfMarkers)
		{
			cout << "Invalid ID! Please enter valid id!\n";
			continue;
		}

		int arrayIndex = userInputID - 1;
		
		if (terminatedThreads[arrayIndex])
		{
			cout << "The thread has been terminated!\n";
			continue;
		}


		markerInfo[arrayIndex]->actions[1] = true;
		markerInfo[arrayIndex]->start.notify_one();
		terminatedThreads[arrayIndex] = true;
		markers[arrayIndex].join();

		numberOfHaltredThreads++;
		printArray(arr);

		for (int i = 0; i < numberOfMarkers; i++)
		{
			if (!terminatedThreads[i])
			{
				halteredThreads[i] = false;
				markerInfo[i]->actions[0] = true;
				markerInfo[i]->start.notify_one();
			}
		}
	}

	cout << "All marker threads have been eliminated!\n";

	cout << "[CHECK] If the program works as expected, than the result array must contain only zeros.\n";
	bool isValidResult = checkResult();
	if (isValidResult)
	{
		cout << "Program completed succesfully!\n";
	}
	else
	{
		cout << "Program failed to produce the correct result!\n";
	}
	return 0;
}


int generateRandomInt(const int& lowerBound, const int& upperBound)
{
	srand(static_cast<unsigned int>(time(NULL)));
	return lowerBound + rand() % (upperBound - lowerBound + 1);
}


void printArray(const vector<int>& arr)
{
	std::lock_guard<std::mutex> lock(markerMutex);
	for (const int& elem : arr)
	{
		cout << elem << "\t";
	}
	cout << "\n";
	return;
}


void markerFunction(threadInfo* args)
{
	std::unique_lock<std::mutex> startLock(startMutex);
	start.wait(startLock, []()->bool {
		return isReady;
	});
	startLock.unlock();

	cout << "Thread " << args->idx << " has started!\n";
	
	srand(args->idx);
	int count = 0;
	
	// logic loop
	while (true)
	{
		std::unique_lock<std::mutex> lock(markerMutex);
		int i = rand() % args->size;
		if (0 == arr[i])
		{
			std::this_thread::sleep_for(
				std::chrono::milliseconds(kSleepTime));
			cout << "Thread #" << args->idx << " changed " <<
				arr[i] << " (index " << i << ") to" << args->idx << "\n";
				arr[i] = args->idx;

			count++;
			lock.unlock();
			std::this_thread::sleep_for(
				std::chrono::milliseconds(kSleepTime));
		}
		else
		{
			cout << "[Report]\tThread #" << args->idx
				<< ": Number of marked elements: " << count <<
				" Can not mark the element with index " << i << "\n";
			lock.unlock();
			halteredThreads[args->idx - 1] = true;
			stateChanged.notify_all();
			std::unique_lock<std::mutex> actionLock(args->actionMutex);
			args->start.wait(actionLock, [args]()->bool {
				return args->actions[1] || args->actions[0];
			});
			if (args->actions[1])
			{
				for (int& elem : arr)
				{
					if (elem == args->idx)
					{
						elem = 0;
					}
				}
				cout << "Thread #" << args->idx << "has been terminated.\n";
				break;
			}
			args->actions[0] = false;
		}
	}

	return;
}


bool checkResult(void)
{
	for (const int& elem : arr)
	{
		if (0 != elem)
		{
			return false;
		}
	}
	return true;
}
