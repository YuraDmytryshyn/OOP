#include <windows.h>
#include <iostream>
#include <string>
using namespace std;

const int MAX_MARKS = 10;
const int MAX_STUDENTS = 30;
const int MIN_MARK = 1;
const int MAX_MARK = 10;

int inputInt(string text, int minValue, int maxValue) {
    string s;
    while (true) {
        cout << text;
        cin >> s;
        cin.ignore(1000, '\n');

        bool onlyDigits = true;
        for (int i = 0; i < s.length(); i++) {
            if (s[i] < '0' || s[i] > '9') {
                onlyDigits = false;
            }
        }

        if (!onlyDigits) {
            cout << "Error! Enter a number (digits only)." << endl;
        }
        else if (s.length() > 1 && s[0] == '0') {
            cout << "Error! Number cannot start with zero." << endl;
        }
        else if (s.length() > 9) {
            cout << "Error! Number is too big." << endl;
        }
        else {
            int x = stoi(s);
            if (x < minValue || x > maxValue) {
                cout << "Error! Number must be from " << minValue << " to " << maxValue << "." << endl;
            }
            else {
                return x;
            }
        }
    }
}

string inputSurname(string text) {
    string s;
    while (true) {
        cout << text;
        cin >> s;
        cin.ignore(1000, '\n');

        bool hasDigit = false;
        for (int i = 0; i < s.length(); i++) {
            if (s[i] >= '0' && s[i] <= '9') {
                hasDigit = true;
            }
        }

        if (hasDigit) {
            cout << "Error! Surname cannot contain digits." << endl;
        }
        else {
            return s;
        }
    }
}

class STUDENT {
private:
    string surname;
    int recordBookNumber;
    int* marks;
    int marksCount;

public:
    STUDENT() {
        surname = "";
        recordBookNumber = 0;
        marksCount = 0;
        marks = new int[MAX_MARKS];
    }

    STUDENT(string s, int n) {
        surname = s;
        recordBookNumber = n;
        marksCount = 0;
        marks = new int[MAX_MARKS];
    }

    ~STUDENT() {
        delete[] marks;
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
    STUDENT* students[MAX_STUDENTS];
    int studentsCount;

public:
    GROUP() {
        studentsCount = 0;
    }

    ~GROUP() {
        for (int i = 0; i < studentsCount; i++) {
            delete students[i];
        }
    }

    void inputData() {
        studentsCount = inputInt("How many students in the group (max " + to_string(MAX_STUDENTS) + "): ", 1, MAX_STUDENTS);

        for (int i = 0; i < studentsCount; i++) {
            string s;
            int n, k;

            students[i] = new STUDENT();

            cout << "\nStudent " << i + 1 << endl;

            s = inputSurname("Surname: ");

            n = inputInt("Record book number: ", 1, 999999999);
            students[i]->setData(s, n);

            k = inputInt("How many marks (max " + to_string(MAX_MARKS) + "): ", 1, MAX_MARKS);
            for (int j = 0; j < k; j++) {
                int m = inputInt("Mark " + to_string(j + 1) + ": ", MIN_MARK, MAX_MARK);
                students[i]->addMark(m);
            }
        }
    }

    void printAverages() {
        cout << "\nAverage mark of each student:" << endl;
        for (int i = 0; i < studentsCount; i++) {
            cout << students[i]->getSurname() << " (record book " << students[i]->getRecordBookNumber()
                << ") - " << students[i]->averageMark() << endl;
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
                if (students[index[j]]->averageMark() < students[index[j + 1]]->averageMark()) {
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
            cout << i + 1 << ". " << students[index[i]]->getSurname()
                << " - " << students[index[i]]->averageMark() << endl;
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