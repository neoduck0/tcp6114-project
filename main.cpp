#include <cstdlib>
#include <iostream>
#include <ctime>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

const int MAX_BOOKS = 100;
const int MAX_QUOTES = 100;
const int MAX_LOGS = 100;

class Date {
    private:
        string year, month, day, hour, minute, second;
    public:
        Date() {
            string year, month, day, hour, minute, second;

            time_t t = time(0);
            tm* current = localtime(&t);

            year = to_string(current->tm_year + 1900);

            if (current->tm_mon < 10) {
                month = "0" + to_string(current->tm_mon + 1);
            } else {
                month = to_string(current->tm_mon);
            }

            if (current->tm_mday < 10) {
                day = "0" + to_string(current->tm_mday);
            } else {
                day = to_string(current->tm_mday);
            }

            if (current->tm_hour < 10) {
                hour = "0" + to_string(current->tm_hour);
            } else {
                hour = to_string(current->tm_hour);
            }

            if (current->tm_min < 10) {
                minute = "0" + to_string(current->tm_min);
            } else {
                minute = to_string(current->tm_min);
            }

            if (current->tm_sec< 10) {
                second = "0" + to_string(current->tm_sec);
            } else {
                second = to_string(current->tm_sec);
            }

            this->year = year;
            this->month = month;
            this->day = day;
            this->hour = hour;
            this->minute = minute;
            this->second = second;
        }

        Date(string str_date) {
            this->year = str_date.substr(0, 4);
            this->month = str_date.substr(5, 2);
            this->day = str_date.substr(8, 2);

            if (str_date.length() > 10) {
                this->hour = str_date.substr(11, 2);
                this->minute = str_date.substr(14, 2);
                this->second = str_date.substr(17, 2);
            } else {
                this->hour = "00";
                this->minute = "00";
                this->second = "00";
            }
        }

        string get_readable(bool incl_time) {
            string str_date = this->year + "-"
                + this->month + "-"
                + this->day + " "
                + this->hour + ":"
                + this->minute + ":"
                + this->second;

            if (incl_time) {
                return str_date;
            } else {
                return str_date.substr(0, 10);
            }
        }
};

class Quote {
    private:
        string content;
        Date time;
    public:
        string get_readable();
};

class Log {
    private:
        int pages;
        Date time;
    public:
        string get_readable();
};

class Book {
    private:
        int id;
        string title;
        string author;
        Date release_date;
        Quote quotes[MAX_QUOTES];
        Log logs[MAX_LOGS];
    public:
        string get_readable();
};

class DataBase {
    private:
        string books_file;
        string quotes_file;
        string logs_file;
        Book books[MAX_BOOKS];
    public:
        void load_db();
        void write_db();
};

bool h_clean_buf();
void ui_home();
void uih_clear();
void uih_logo();

int main() {
    while (true) {ui_home();}
}

void ui_home() {
    int option;

    uih_clear();
    uih_logo();

    cout <<
"      (1) search book" << endl <<
"" << endl <<
"      (2) view sessions" << endl <<
"      (3) view quotes" << endl <<
"" << endl <<
"      (4) exit" << endl;


    cout << endl <<
    "option: ";
    cin >> option;

    if (h_clean_buf()) {
        option = -1;
    }

    switch (option) {
        case 1:
            exit(200);
        case 2:
            exit(200);
        case 3:
            exit(200);
        case 4:
            exit(0);
        default:
            return;
    }
}

void uih_logo() {
    cout <<
    "+============================+" << endl <<
    "|       ______ ______        |" << endl <<
    "|     _/      Y      \\_      |" << endl <<
    "|    // ~ ~~  | ~ ~~~ \\\\     |" << endl <<
    "|   // ~~ ~~~ | ~~~~ ~ \\\\    |" << endl <<
    "|  //________.|.________\\\\   |" << endl <<
    "|  '---------'-'----------'  |" << endl <<
    "|        NOVELTY v1.0        |" << endl <<
    "+============================+" << endl;
}

void uih_clear() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

bool h_clean_buf() {
    bool return_value = false;

    if (cin.fail()) {
        cin.clear();
        return_value = true;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    return return_value;
}
