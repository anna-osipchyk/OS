#ifndef LAB_3_THREAD_H
#define LAB_3_THREAD_H
struct threadArgs{
    int* arr;
    int n;
    int num;
    HANDLE actions[2];
    threadArgs(int* _arr, int _n, int _num) : num(_num),  n(_n){
        arr = new int[n];
        for(int i = 0; i < n; i = -~i) {
            arr[i] = _arr[i];
        }
        actions[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
        actions[1] = CreateEvent(NULL, FALSE, FALSE, NULL);
    }
};
#endif //LAB_3_THREAD_H
