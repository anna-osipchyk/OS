#include <iostream>
#include<windows.h>
#include <fstream>

const int &messageSize = 20;

void sendMessage(std::ofstream &out, std::string message, std::string fileName){
    out.open(fileName.c_str(), std::ios::binary | std::ios::app);
    out.write(message.c_str(), messageSize);

    out.close();
}

int main(int argc, char** argv) {
    std::string fileName = argv[1], nameOfEvent = argv[2];
    std::ofstream out(fileName.c_str(), std::ios::binary);

    if (out.is_open()) {
        HANDLE ReadyEvent = OpenEvent(EVENT_MODIFY_STATE, 0,
                                      nameOfEvent.c_str());
        HANDLE StartAll = OpenEvent(SYNCHRONIZE, 0, "START_ALL");

        if (NULL == ReadyEvent || NULL == StartAll) {
            std::cout << "Open event has been failed.\n";
            return GetLastError();
        }

        SetEvent(ReadyEvent);

        std::cout << "Ready...\nStarted...\n";

        HANDLE fileMutex = OpenMutex(SYNCHRONIZE, 0, "FILE_ACCESS");

        if(NULL == fileMutex){
            std::cout << "Open mutex has been failed.\n";

            return GetLastError();
        }

        HANDLE senderSemaphore = OpenSemaphore(SEMAPHORE_MODIFY_STATE,
                                               0, "MESSAGES_COUNT_SEM");
        HANDLE mesReadEvent = OpenEvent(EVENT_ALL_ACCESS, 0,
                                        "MESSAGE_READ");

        if (NULL == senderSemaphore || NULL == mesReadEvent) {
            return GetLastError();
        }

        WaitForSingleObject(StartAll, INFINITE);

        std::cout << "Input message: ";

        std::string message;
        while(true){
            std::cin >> message;
            message += '\n';

            if(false == std::cin.eof()) {
                WaitForSingleObject(fileMutex, INFINITE);
                sendMessage(out, message.c_str(), fileName);
                ReleaseMutex(fileMutex);

                if(ReleaseSemaphore(senderSemaphore, 1, NULL) != 1){
                    std::cout << "Message file is full. Waiting for receiver.\n";
                    ResetEvent(mesReadEvent);
                    WaitForSingleObject(mesReadEvent, INFINITE);
                    ReleaseSemaphore(senderSemaphore, 1, NULL);
                }
                std::cout << "Message has been sent.\n";
            } else {
                break;
            }
        }
        out.close();

        return 0;
    } else {
        std::cout << "File is not open." << std::endl;
        out.close();
        return -1;
    }
}
