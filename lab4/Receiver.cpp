#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <string>

HANDLE *readyEvents;
const int &messageSize = 20;

int CreateSenders(int sendersCount, std::string fileName){

    readyEvents = new HANDLE[sendersCount];
    char buff[10];

    for(int i = 0; i < sendersCount; ++i){

        std::string eventName = "sender";
        eventName += std::string(itoa(i, buff, 10));
        readyEvents[i] = CreateEvent(NULL, 1, 0, eventName.c_str());

        if (NULL == readyEvents[i]) {
            std::cout << "Creating of event has failed.";
            return GetLastError();
        }

        std::string senderEXE = "Sender.exe ";
        senderEXE += fileName + " " + eventName;

        STARTUPINFO si;
        ZeroMemory(&si, sizeof(STARTUPINFO));
        si.cb = sizeof(STARTUPINFO);
        PROCESS_INFORMATION pi;

        char* copySenderEXE;
        strcpy(copySenderEXE, senderEXE.c_str());

        if (CreateProcess(NULL, copySenderEXE,
                          NULL, NULL, 0,
                          CREATE_NEW_CONSOLE, NULL,
                          NULL, &si, &pi) == 1) {
            std::cout <<"Creating process error.\n";

            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    }

    std::cout <<"Receiver process created " << sendersCount << " senders.\n";
}

std::string receiveMessage(std::string fileName){
    std::fstream in(fileName.c_str(), std::ios::binary | std::ios::in);
    if(in.is_open()){
        if(std::ifstream::traits_type::eof() != in.peek()) {
            char resultString[messageSize];
            in.read(resultString, messageSize);

            in.seekg(0, std::ios::end);
            int n = in.tellg();
            in.seekg(0, std::ios::beg);

            char* temp = new char[n];
            in.read(temp, n);
            in.close();
            in.open(fileName.c_str(), std::ios::binary | std::ios::out);
            in.clear();
            in.write(temp + messageSize, n - messageSize);
            in.close();

            return std::string(resultString);
        }

        return "File is empty.";
    }

    return "Error opening file.\n";

}

int main() {
    std::string fileName;
    std::cout <<"Input binary file name: ";
    std::cin >> fileName;
    std::fstream in(fileName.c_str(), std::ios::binary | std::ios::out);

    std::cout << "Input number of sender: ";
    int senderCount;
    std::cin >> senderCount;

    in.clear();
    in.close();

    HANDLE startAll = CreateEvent(NULL, 1, 0, "START_ALL");
    HANDLE fileMutex = CreateMutex(NULL, 0, "FILE_ACCESS");

    if(NULL == fileMutex) {
        std::cout << "Error in mutex creation.";
        return GetLastError();
    }

    HANDLE senderSemaphore = CreateSemaphore(NULL, 0,
                                             senderCount, "MESSAGES_COUNT_SEM");
    HANDLE mesReadEvent = CreateEvent(NULL, 0,
                                      0, "MESSAGE_READ");

    if (NULL == senderSemaphore || NULL == mesReadEvent) {
        return GetLastError();
    }

    CreateSenders(senderCount, fileName);
    WaitForMultipleObjects(senderCount, readyEvents, 1, INFINITE);

    std::cout << "All senders are ready. Starting." << std::endl;
    SetEvent(startAll);
    char tmp[messageSize];
    char message[messageSize];

    while(true){

        std::cin >> tmp;

        if(false == std::cin.eof()) {
            std::cout << "Waiting for a message." << std::endl;

            WaitForSingleObject(senderSemaphore, INFINITE);
            WaitForSingleObject(fileMutex, INFINITE);

            std::cout << receiveMessage(fileName) << '\n';

            ReleaseMutex(fileMutex);
            SetEvent(mesReadEvent);
        } else {
            break;
        }
    }
    delete[] readyEvents;
}
