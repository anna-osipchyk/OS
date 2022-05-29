#include <iostream>

struct employee {
    int num;
    char name[10];
    double hours;

    void print(){
        std::cout << "ID: " << num << "\tName: " << name
            << "\tHours: " << hours << '\n';
    }
};

int empCmp(const void* p1, const void* p2){
    return ((employee*)p1)->num - ((employee*)p2)->num;
}

#ifndef LAB5_EMPLOYEE_H
#define LAB5_EMPLOYEE_H

#endif
