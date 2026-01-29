#include <cstdlib>
#include <iostream>
#include <ctime>
#include <limits>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

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
        vector<Quote> quotes;
        vector<Log> logs;
    public:
        string get_readable();
};

class DataBase {
    private:
        string books_file;
        string quotes_file;
        string logs_file;
        vector<Book> books;
    public:
        void load_db();
        void write_db();
};

bool h_clean_buf();

void ui_home();
void ui_search_book();
void ui_view_sessions();
void ui_view_quotes();
void ui_edit_book();
void ui_add_session();
void ui_delete_session();
void ui_add_quote();
void ui_delete_quote();

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
    "option> ";
    cin >> option;

    if (h_clean_buf()) {
        option = -1;
    }

    switch (option) {
        case 1:
            ui_search_book();
        case 2:
            ui_view_sessions();
        case 3:
            ui_view_quotes();
        case 4:
            exit(0);
        default:
            return;
    }
}
void ui_search_book() {
    string name;

    uih_clear();
    uih_logo();

    cout << "book name> ";
    getline(cin, name);

    // TODO: finish
}

void ui_view_sessions() {
    // replace with declarations

    uih_clear();
    uih_logo();

    // get the list of books that have matching names
    // list the index + 1 as the number and allow the user to press n to go next and press p for previous list
    // if a number is entered then select a book and go to the book page
}

void ui_view_quotes() {

    uih_clear();
    uih_logo();

};

void ui_edit_book() {

    uih_clear();
    uih_logo();

};

void ui_add_session() {

    uih_clear();
    uih_logo();

};

void ui_delete_session() {

    uih_clear();
    uih_logo();

};

void ui_add_quote() {

    uih_clear();
    uih_logo();

};

void ui_delete_quote() {

    uih_clear();
    uih_logo();

};

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
