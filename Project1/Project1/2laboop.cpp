#include <windows.h>
#include <iostream>
#include <string>
using namespace std;

const int MAX_MARKS = 10;
const int MAX_STUDENTS = 30;

class STUDENT {
private:
    string surname;
    int recordBookNumber;
    int marks[MAX_MARKS];
    int marksCount;

public:
    STUDENT() {
        surname = "";
        recordBookNumber = 0;
        marksCount = 0;
    }

    STUDENT(string s, int n) {
        surname = s;
        recordBookNumber = n;
        marksCount = 0;
    }

    ~STUDENT() {
    }

    void setData(string s, int n) {
        surname = s;
        recordBookNumber = n;
    }

    void addMark(int m) {
        if (marksCount < MAX_MARKS) {
            marks[marksCount] = m;
            marksCount++;
        }
    }

    string getSurname() {
        return surname;
    }

    int getRecordBookNumber() {
        return recordBookNumber;
    }

    double averageMark() {
        if (marksCount == 0) {
            return 0;
        }
        int sum = 0;
        for (int i = 0; i < marksCount; i++) {
            sum = sum + marks[i];
        }
        return (double)sum / marksCount;
    }
};

class GROUP {
private:
    STUDENT students[MAX_STUDENTS];
    int studentsCount;

public:
    GROUP() {
        studentsCount = 0;
    }

    ~GROUP() {
    }

    void inputData() {
        cout << "How many students in the group (max " << MAX_STUDENTS << "): ";
        cin >> studentsCount;
        if (studentsCount > MAX_STUDENTS) {
            studentsCount = MAX_STUDENTS;
        }

        for (int i = 0; i < studentsCount; i++) {
            string s;
            int n, k;

            cout << "\nStudent " << i + 1 << endl;
            cout << "Surname: ";
            cin >> s;
            cout << "Record book number: ";
            cin >> n;
            students[i].setData(s, n);

            cout << "How many marks (max " << MAX_MARKS << "): ";
            cin >> k;
            for (int j = 0; j < k; j++) {
                int m;
                cout << "Mark " << j + 1 << ": ";
                cin >> m;
                students[i].addMark(m);
            }
        }
    }

    void printAverages() {
        cout << "\nAverage mark of each student:" << endl;
        for (int i = 0; i < studentsCount; i++) {
            cout << students[i].getSurname() << " (record book " << students[i].getRecordBookNumber()
                << ") - " << students[i].averageMark() << endl;
        }
    }

    void printTop5() {
        int index[MAX_STUDENTS];

        for (int i = 0; i < studentsCount; i++) {
            index[i] = i;
        }

        // bublle 
        for (int i = 0; i < studentsCount - 1; i++) {
            for (int j = 0; j < studentsCount - 1 - i; j++) {
                if (students[index[j]].averageMark() < students[index[j + 1]].averageMark()) {
                    int temp = index[j];
                    index[j] = index[j + 1];
                    index[j + 1] = temp;
                }
            }
        }

        int count = 5;
        if (studentsCount < 5) {
            count = studentsCount;
        }

        cout << "\nTop students with the highest average mark:" << endl;
        for (int i = 0; i < count; i++) {
            cout << i + 1 << ". " << students[index[i]].getSurname()
                << " - " << students[index[i]].averageMark() << endl;
        }
    }
};

int main() {

    GROUP g;
    g.inputData();
    g.printAverages();
    g.printTop5();

    return 0;
}