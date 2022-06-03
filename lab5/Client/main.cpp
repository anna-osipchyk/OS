#include <iostream>
#include <conio.h>
#include <windows.h>
#include <string>
#include "../employee.h"

const int COMMAND_LENGTH = 12;
const int MS_PIPE_WAIT = 2000;
const std::string IP_NAME = "START_ALL";
const std::string PIPE_NAME = "\\\\.\\pipe\\pipe_name";

void errorMessage(std::string message) {
    std::cerr <<  message;
    getch();
}

void messaging(HANDLE handlePipe){
    std::cout << "If you want to quit, press Ctrl+Z...\n";

    while(true){
        std::cout << "Input r to read or w to write command and ID of employee: ";
        char command[COMMAND_LENGTH];
        std::cin.getline(command, COMMAND_LENGTH, '\n');

        if(std::cin.eof()) {
            errorMessage("File is empty... \n");
            return;
        }

        bool isSent;
        DWORD bytesWritten;
        isSent = WriteFile(handlePipe, command, COMMAND_LENGTH,
                           &bytesWritten, NULL);

        if(!isSent){
            errorMessage("Message cannot be sent...\n");
            return;
        }

        bool isRead;
        DWORD readBytes;
        employee tempEmployee;
        isRead = ReadFile(handlePipe, &tempEmployee,
                          sizeof(tempEmployee), &readBytes, NULL);

        if(!isRead){
            errorMessage("Error in receiving answer...\n");
            continue;
        }

        if(tempEmployee.num < 0) {
            errorMessage("Employee not found or being modified.\n");
            continue;
        }

        tempEmployee.print();

        if ('w' == command[0]) {
            std::cout << "Input ID of employee: ";
            std::cin >> tempEmployee.num;
            std::cout << "Input name of employee: ";
            std::cin >> tempEmployee.name;
            std::cout << "Input working hours of employee: ";
            std::cin >> tempEmployee.hours;

            std::cin.ignore(2, '\n');

            isSent = WriteFile(handlePipe, &tempEmployee,
                               sizeof(tempEmployee), &bytesWritten, NULL);

            if (!isSent) {
                errorMessage("Error in sending...\n");
                break;
            }

            std::cout << "New record has been sent.\n";
        }
    }
}

int main(int argc, char** argv) {
    HANDLE handleReadyEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, argv[1]);
    HANDLE handleStartEvent = OpenEvent(SYNCHRONIZE, FALSE, IP_NAME.c_str());

    if(handleStartEvent == NULL || handleReadyEvent == NULL){
        std::cerr << "Error in action with event...\n";
        getch();
        return GetLastError();
    }

    SetEvent(handleReadyEvent);
    WaitForSingleObject(handleStartEvent, INFINITE);
    HANDLE handlePipe;

    std::cout << "Process is started.\n";

    while(1) {
        handlePipe = CreateFile(PIPE_NAME.c_str(), GENERIC_WRITE | GENERIC_READ,
                                FILE_SHARE_READ, NULL,
                           OPEN_EXISTING, 0, NULL);

        const bool FLAG = WaitNamedPipe(PIPE_NAME.c_str(), MS_PIPE_WAIT)
                + (INVALID_HANDLE_VALUE != handlePipe);

        if (FLAG) {
            if(INVALID_HANDLE_VALUE != handlePipe) {
                break;
            }
            errorMessage(MS_PIPE_WAIT + "timed out...\n");
            return 0;
        }
    }
    std::cout << "Pipe has been connecting...\n";
    messaging(handlePipe);
    return 0;
}
