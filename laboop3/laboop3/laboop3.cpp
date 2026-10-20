#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <io.h>
#include <fcntl.h>
using namespace std;

// checks ukr
bool isUkrLetter(wchar_t c) {
    return (c >= 0x0400 && c <= 0x04FF);
}

// checks english
bool isEngLetter(wchar_t c) {
    return (c >= L'a' && c <= L'z') || (c >= L'A' && c <= L'Z');
}

// mode 1 - only ukrainian letters, mode 2 - only english letters, mode 3 - any of them
bool onlyLetters(const wstring& s, int mode) {
    if (s.empty()) return false; // empty string is bad
    for (int i = 0; i < (int)s.size(); i++) {
        bool ok = false;
        if (mode == 1) ok = isUkrLetter(s[i]);
        if (mode == 2) ok = isEngLetter(s[i]);
        if (mode == 3) ok = isUkrLetter(s[i]) || isEngLetter(s[i]);
        if (!ok) return false; // found a bad symbol
    }
    return true;
}

// letters small
wstring toLowerStr(wstring s) {
    for (int i = 0; i < (int)s.size(); i++) {
        wchar_t c = s[i];
        if (c >= L'A' && c <= L'Z') c = c + 32; // english
        else if (c >= 0x0410 && c <= 0x042F) c = c + 32; // normal ukrainian letters
        else if (c >= 0x0400 && c <= 0x040F) c = c + 80; // º, ³, ¿ are here
        else if (c == 0x0490) c = 0x0491; // ´
        s[i] = c;
    }
    return s;
}

wstring readWord(const wstring& text, int mode) {
    wstring s;
    while (true) {
        wcout << text;
        getline(wcin, s);
        if (onlyLetters(s, mode)) return toLowerStr(s);
        if (mode == 1) wcout << L"Error: use only ukrainian letters! Try again." << endl;
        else if (mode == 2) wcout << L"Error: use only english letters! Try again." << endl;
        else wcout << L"Error: use only letters (no numbers or signs)! Try again." << endl;
    }
}

// class WORD - keeps the ukrainian word and its translation
class Word {
    wstring* ukr; 
    wstring* eng;
public:
    Word() { ukr = nullptr; eng = nullptr; }

    Word(wstring u, wstring e) {
        ukr = new wstring(u);
        eng = new wstring(e);
    }

    // copy constructor, so two words dont use the same memory
    Word(const Word& o) {
        ukr = o.ukr ? new wstring(*o.ukr) : nullptr;
        eng = o.eng ? new wstring(*o.eng) : nullptr;
    }

    Word& operator=(const Word& o) {
        if (this != &o) {
            delete ukr;
            delete eng;
            ukr = o.ukr ? new wstring(*o.ukr) : nullptr;
            eng = o.eng ? new wstring(*o.eng) : nullptr;
        }
        return *this;
    }

    ~Word() {
        delete ukr;
        delete eng;
    }

    wstring getUkr() const {
        if (ukr == nullptr) throw runtime_error("Ukrainian word is not initialized!");
        return *ukr;
    }

    wstring getEng() const {
        if (eng == nullptr) throw runtime_error("English translation is not initialized!");
        return *eng;
    }

    bool operator<(const Word& o) const {
        return getUkr() < o.getUkr();
    }

    bool operator>=(const Word& o) const {
        return getUkr() >= o.getUkr();
    }

    void print() const {
        wcout << getUkr() << L" - " << getEng() << endl;
    }
};

class Dictionary {
    vector<Word> words; 
public:
    Dictionary operator+(const Word& w) const {
        Dictionary res = *this;
        res.words.push_back(w);
        return res;
    }

    Dictionary operator-(const wstring& ukrWord) const {
        Dictionary res = *this;
        for (int i = 0; i < (int)res.words.size(); i++) {
            if (res.words[i].getUkr() == ukrWord) {
                res.words.erase(res.words.begin() + i); // removing here
                return res;
            }
        }
        throw runtime_error("There is no such word in the dictionary!");
    }

    // sort by ukrainian (default)
    void sort() {
        for (int i = 0; i < (int)words.size() - 1; i++) {
            for (int j = 0; j < (int)words.size() - i - 1; j++) {
                if (words[j + 1] < words[j]) { // here we use our operator <
                    Word temp = words[j];
                    words[j] = words[j + 1];
                    words[j + 1] = temp;
                }
            }
        }
    }

    // overloaded sort: 1 - by ukrainian, 2 - by english
    void sort(int mode) {
        if (mode == 1) {
            sort();
            return;
        }
        for (int i = 0; i < (int)words.size() - 1; i++) {
            for (int j = 0; j < (int)words.size() - i - 1; j++) {
                if (words[j].getEng() > words[j + 1].getEng()) {
                    Word temp = words[j];
                    words[j] = words[j + 1];
                    words[j + 1] = temp;
                }
            }
        }
    }

    int find(const wstring& s) const {
        for (int i = 0; i < (int)words.size(); i++) {
            if (words[i].getUkr() == s || words[i].getEng() == s) {
                return i;
            }
        }
        return -1;
    }

    void printWord(int i) const {
        words[i].print();
    }

    // print the whole dictionary
    void print() const {
        if (words.empty()) {
            wcout << L"Dictionary is empty" << endl;
            return;
        }
        for (int i = 0; i < (int)words.size(); i++) {
            wcout << i + 1 << L". ";
            words[i].print();
        }
    }
};

int main() {
    // this makes the console work with unicode, so ukrainian letters are fine
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stdout), _O_U16TEXT);

    Dictionary d;
    int choice = -1;

    do {
        wcout << L"\n1 - Add word" << endl;
        wcout << L"2 - Delete word" << endl;
        wcout << L"3 - Sort by Ukrainian" << endl;
        wcout << L"4 - Sort by English" << endl;
        wcout << L"5 - Find word" << endl;
        wcout << L"6 - Show dictionary" << endl;
        wcout << L"7 - Error check (not initialized word)" << endl;
        wcout << L"0 - Exit" << endl;
        wcout << L"Choice: ";

        // read the choice as a line, so letters or signs dont break the program
        wstring line;
        getline(wcin, line);
        if (line.size() == 1 && line[0] >= L'0' && line[0] <= L'9') {
            choice = line[0] - L'0';
        }
        else {
            choice = -1; // wrong input
            wcout << L"Error: enter a number from the menu (0-7)!" << endl;
            continue;
        }

        try {
            if (choice == 1) {
                wstring u = readWord(L"Ukrainian word: ", 1);
                wstring e = readWord(L"English translation: ", 2);
                d = d + Word(u, e); // adding with +
                wcout << L"Added!" << endl;
            }
            else if (choice == 2) {
                wstring u = readWord(L"Which word to delete (ukrainian): ", 1);
                d = d - u; // deleting with -
                wcout << L"Deleted!" << endl;
            }
            else if (choice == 3) {
                d.sort();
                wcout << L"Sorted by Ukrainian" << endl;
            }
            else if (choice == 4) {
                d.sort(2);
                wcout << L"Sorted by English" << endl;
            }
            else if (choice == 5) {
                wstring s = readWord(L"What to search: ", 3);
                int pos = d.find(s);
                if (pos == -1) wcout << L"Not found" << endl;
                else d.printWord(pos);
            }
            else if (choice == 6) {
                d.print();
            }
            else if (choice == 7) {
                Word empty; // pointers are empty here
                empty.print(); // error will be thrown here
            }
            else if (choice != 0) {
                wcout << L"Error: there is no such menu item!" << endl;
            }
        }
        catch (exception& ex) {
            wcout << L"Error: " << ex.what() << endl;
        }

    } while (choice != 0);

    return 0;
}