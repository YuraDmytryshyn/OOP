#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <cstdlib>
using namespace std;

// check if symbol is a letter (english or ukrainian in cp1251)
bool isLetter(char ch) {
    unsigned char c = (unsigned char)ch;
    if (isalpha(c) && c < 128) {
        return true; // english letters
    }
    if (c >= 0xC0) {
        return true; // main ukrainian letters (À-ß, à-ÿ)
    }
    // special ukrainian letters: ¥ ´ ª º ² ³ ¯ ¿
    if (c == 0xA5 || c == 0xB4 || c == 0xAA || c == 0xBA ||
        c == 0xB2 || c == 0xB3 || c == 0xAF || c == 0xBF) {
        return true;
    }
    return false;
}

// mode 1 = name, mode 2 = group, mode 3 = thesis theme
bool checkString(const string& str, int mode) {
    // symbols allowed besides letters
    string extra;
    if (mode == 1) {
        extra = " -'";
    }
    else if (mode == 2) {
        extra = " -0123456789";
    }
    else {
        extra = " -,.:()'0123456789";
    }

    int letters = 0;
    for (size_t i = 0; i < str.size(); i++) {
        char c = str[i];
        if (isLetter(c)) {
            letters++;
        }
        else if (extra.find(c) == string::npos) {
            return false; // bad symbol
        }
    }
    // need at least 2 letters
    return letters >= 2;
}

// check that string has only digits
bool onlyDigits(const string& str) {
    if (str.empty() || str.size() > 9) {
        return false;
    }
    for (size_t i = 0; i < str.size(); i++) {
        if (!isdigit((unsigned char)str[i])) {
            return false;
        }
    }
    return true;
}

// read string until it is ok
string readString(istream& is, string prompt, int mode) {
    string value;
    while (true) {
        cout << prompt;
        getline(is, value);
        if (checkString(value, mode)) {
            return value;
        }
        cout << "Error! Wrong text or bad symbols, try again.\n";
    }
}

// read number in range until it is ok
int readInt(istream& is, string prompt, int minVal, int maxVal) {
    string line;
    while (true) {
        cout << prompt;
        getline(is, line); // read whole line, not just number
        if (!onlyDigits(line)) {
            cout << "Error! Enter only digits.\n";
            continue;
        }
        int value = atoi(line.c_str());
        if (value >= minVal && value <= maxVal) {
            return value;
        }
        cout << "Error! Value must be from " << minVal << " to " << maxVal << ".\n";
    }
}

// base class
class Person {
protected:
    string name;
    int age;
public:
    Person(string n = "", int a = 0) {
        name = n;
        age = a;
    }

    // base assignment
    Person& operator=(const Person& p) {
        if (this != &p) {
            name = p.name;
            age = p.age;
        }
        return *this;
    }

    // inherited methods
    string getName() const { return name; }
    int getAge() const { return age; }

    // output to any stream (screen or file)
    virtual void print(ostream& os) const {
        os << "Name: " << name << "\n";
        os << "Age: " << age << "\n";
    }

    // input from stream
    virtual void read(istream& is) {
        name = readString(is, "Enter name: ", 1);
        age = readInt(is, "Enter age (1-100): ", 1, 100);
    }

    // operators just call print and read
    friend ostream& operator<<(ostream& os, const Person& p) {
        p.print(os);
        return os;
    }

    friend istream& operator>>(istream& is, Person& p) {
        p.read(is);
        return is;
    }

    virtual ~Person() {}
};

// student class
class Student : public Person {
protected:
    string group;
    int course;
public:
    Student(string n = "", int a = 0, string g = "", int c = 0) : Person(n, a) {
        group = g;
        course = c;
    }

    // base assignment + own fields
    Student& operator=(const Student& s) {
        if (this != &s) {
            Person::operator=(s);
            group = s.group;
            course = s.course;
        }
        return *this;
    }

    string getGroup() const { return group; }
    int getCourse() const { return course; }

    void print(ostream& os) const {
        Person::print(os); // base part first
        os << "Group: " << group << "\n";
        os << "Course: " << course << "\n";
    }

    void read(istream& is) {
        Person::read(is); // base part first
        group = readString(is, "Enter group: ", 2);
        course = readInt(is, "Enter course (1-6): ", 1, 6);
    }
};

// diploma student class
class Diplomnik : public Student {
private:
    string theme;
    string supervisor;
public:
    Diplomnik(string n = "", int a = 0, string g = "", int c = 0,
        string t = "", string s = "") : Student(n, a, g, c) {
        theme = t;
        supervisor = s;
    }

    // student assignment + own fields
    Diplomnik& operator=(const Diplomnik& d) {
        if (this != &d) {
            Student::operator=(d);
            theme = d.theme;
            supervisor = d.supervisor;
        }
        return *this;
    }

    void print(ostream& os) const {
        Student::print(os);
        os << "Thesis theme: " << theme << "\n";
        os << "Supervisor: " << supervisor << "\n";
    }

    void read(istream& is) {
        Student::read(is);
        theme = readString(is, "Enter thesis theme: ", 3);
        supervisor = readString(is, "Enter supervisor: ", 1);
    }
};

int main() {
    // ukrainian letters in console
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // ready test data
    Person p("Ivan Petrenko", 45);
    Student s("Oleh Kovalenko", 18, "KN 11-1", 1);

    // data from user
    Diplomnik d;
    cout << "--- Enter diploma student ---\n";
    cin >> d;

    // check assignment
    Diplomnik d2;
    d2 = d;

    // write all to file
    ofstream fout("info.txt");
    if (!fout) {
        cout << "Cant open file for writing!\n";
        return 1;
    }
    fout << "=== Person ===\n" << p << "\n";
    fout << "=== Student ===\n" << s << "\n";
    fout << "=== Diplomnik ===\n" << d << "\n";
    fout << "=== Diplomnik copy ===\n" << d2 << "\n";
    fout.close();

    // read file back and show on screen
    ifstream fin("info.txt");
    if (!fin) {
        cout << "Cant open file for reading!\n";
        return 1;
    }
    cout << "\n--- File content ---\n";
    string line;
    while (getline(fin, line)) {
        cout << line << "\n";
    }
    fin.close();

    // inherited methods
    cout << "Inherited: " << d.getName() << ", " << d.getAge()
        << ", " << d.getGroup() << endl;

    return 0;
}