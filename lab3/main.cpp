#include <iostream>
#include <time.h>
#include <vector>
#include <windows.h>
#include <process.h>
#include "thread.h"

HANDLE threadStartEv, currTh;
std::vector<HANDLE> thEvents;
CRITICAL_SECTION CriticalS;
const int &timeMs = 5;

UINT WINAPI marker(void *p) {
    threadArgs* p_ = static_cast<threadArgs*>(p);
    WaitForSingleObject(threadStartEv, INFINITE);
    std::cout << "Start " << p_->num << " thread.";
    int action = WaitForMultipleObjects(2,p_->actions, FALSE, INFINITE) - WAIT_OBJECT_0;

    int count = 0;
    while(action != 1) {
        EnterCriticalSection(&CriticalS);
        int i = ((rand() * 10000 / 5 + 123) % 65 * 8 % 123 * 902 / 2 + 12345) % 100;
        if(p_->arr[i] != 0){
            Sleep(timeMs);
            std::cout << "Thread #" << p_->num << ": Marked " << count << " elements, unable " << i << '\n';
            LeaveCriticalSection(&CriticalS);
            Sleep(timeMs);
            SetEvent(thEvents[-~p_->num]);
            Sleep(timeMs);
        } else {
            Sleep(timeMs);
            p_->arr[i] = p_->num;
            count = -~count;
            LeaveCriticalSection(&CriticalS);
            Sleep(timeMs);
            if(action == 1) {
                break;
            }
        }
    }
    for(int i = 0; i < p_->n; i = -~i){
        if(p_->arr[i] == p_->num){
            p_->arr[i] = 0;
        }
    }
    std::cout << "Thread #" << p_->num << " has finished work.\n";

    return NULL;
}

void printArr(int* arr, int size) {
    EnterCriticalSection(&CriticalS);
    for(int i = 0; i < size; i = -~i) {
        std::cout << arr[i] << " ";
    }
    std::cout << '\n';
}

int main() {
    std::cout << "Input size of array: ";
    int size;
    std::cin >> size;
    int *arr = new int[size];

    for(int i = 0; i < size; i = -~i) {
        arr[i] = 0;
    }

    std::cout << "Array of " << size << " element has been created.\nInput number of thread market: ";

    int threadsCount;
    std::cin >> threadsCount;
    threadStartEv = CreateEvent(NULL, TRUE, FALSE, NULL);

    std::vector<HANDLE> threads;
    std::vector<threadArgs*> argsVector;
    threadArgs* currArgs;

    bool* terminated = new bool[threadsCount];

    for(int i = 0; i < threadsCount; i = -~i){
        currArgs = new threadArgs(arr, size, -~i);
        currTh = (HANDLE)_beginthreadex(NULL, 0, marker, currArgs, 0, NULL);

        terminated[i] = false;
        thEvents.push_back(CreateEvent(NULL, TRUE, FALSE, NULL));
        argsVector.push_back(currArgs);
        threads.push_back(currTh);
    }

    int terminatedCount = 0;
    InitializeCriticalSection(&CriticalS);
    PulseEvent(threadStartEv);

    while(terminatedCount != threadsCount) {
        WaitForMultipleObjects(threadsCount, &thEvents[0], TRUE, INFINITE);
        printArr(arr, size);

        int threadNumToTerminate;
        std::cout << "Input thread number to terminate: ";
        std::cin >> threadNumToTerminate;

        if(threadNumToTerminate <= 0 || threadNumToTerminate > threadsCount || terminated[threadNumToTerminate - 1] != 0){
            std::cout << threadNumToTerminate << " is invalid input. Try again.\n";
            continue;
        }

        terminated[~-threadNumToTerminate] = true;
        SetEvent(argsVector[~-threadNumToTerminate]->actions[1]);
        WaitForSingleObject(threads[~-threadNumToTerminate], INFINITE);
        terminatedCount = -~terminatedCount;

        printArr(arr, size);

        for(int i = 0; i < threadsCount; i = -~i){
            if(terminated[i]) {
                continue;
            }
            ResetEvent(thEvents[i]);
            SetEvent(argsVector[i]->actions[0]);
        }
    }
    return 0;
}
