#include <iostream>
using namespace std;

int& highestMark(int marks[], int size) {
    int maxIndex = 0;
    for (int i = 1; i < size; ++i) {
        if (marks[i] > marks[maxIndex]) maxIndex = i;
    }
    return marks[maxIndex];
}

const int& highestMarkConst(const int marks[], int size) {
    int maxIndex = 0;
    for (int i = 1; i < size; ++i) {
        if (marks[i] > marks[maxIndex]) maxIndex = i;
    }
    return marks[maxIndex];
}

int highestMarkCopy(const int marks[], int size) {
    int maxMark = marks[0];
    for (int i = 1; i < size; ++i) {
        if (marks[i] > maxMark) maxMark = marks[i];
    }
    return maxMark;
}

int highestMarkIndex(const int marks[], int size) {
    int maxIndex = 0;
    for (int i = 1; i < size; ++i) {
        if (marks[i] > marks[maxIndex]) maxIndex = i;
    }
    return maxIndex;
}

int* highestMarkPointer(int marks[], int size) {
    int maxIndex = 0;
    for (int i = 1; i < size; ++i) {
        if (marks[i] > marks[maxIndex]) maxIndex = i;
    }
    return &marks[maxIndex];
}

// Intentionally unsafe example: returning a reference to a local variable.
// Do not call this function; using its returned reference is undefined behaviour.
int& danglingReference() {
    int local = 100;
    return local;
}

int main() {
    int marks[] = {78, 92, 85, 88, 90};
    int size = sizeof(marks) / sizeof(marks[0]);

    cout << "Highest mark: " << highestMark(marks, size) << '\n';
    highestMark(marks, size) += 2;
    cout << "After moderation: " << marks[1] << '\n';

    cout << "Const reference: " << highestMarkConst(marks, size) << '\n';
    cout << "Returned copy: " << highestMarkCopy(marks, size) << '\n';

    int index = highestMarkIndex(marks, size);
    cout << "Index: " << index << ", mark: " << marks[index] << '\n';

    int* ptr = highestMarkPointer(marks, size);
    cout << "Pointer mark: " << *ptr << '\n';
    return 0;
}
