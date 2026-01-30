#include <cstdlib>
#include <iostream>
#include <ctime>
#include <limits>
#include <string>
#include <vector>
#include <algorithm>

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
                month = to_string(current->tm_mon + 1);
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

        string get_str(bool incl_time) {
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

        long get_long() {
            return stol(this->year + this->month + this->day
                    + this->hour + this->minute + this->second);
        }
};

class Quote {
    private:
        string content;
        int page;
    public:
        Quote(string content, int page) {
            this->content = content;
            this->page = page;
        };
        string get_str() {
            return "(" + to_string(this->page) + ") " + this->content;
        };
        int get_page() {
            return page;
        }
};

class Log {
    private:
        int pages;
    public:
        Date time;
        Log(int pages) {
            this->pages = pages;
        };

        Log(int pages, string time) {
            this->pages = pages;
            this->time = Date(time);
        };
        int get_pages() {
            return this->pages;
        }
        string get_str() {
            return + "[" + to_string(this->pages)
                + "] (" + this->time.get_str(true) + ")";
        };
};

class Book {
    private:
        string title;
        string author;
        Date release_date;
        int pages;
        vector<Quote> quotes;
        vector<Log> logs;

        void sort_logs() {
            sort(logs.begin(), logs.end(), [](Log& a, Log& b) {
                return a.time.get_long() < b.time.get_long();
            });
        }

        void sort_quotes() {
            sort(quotes.begin(), quotes.end(), [](Quote& a, Quote& b) {
                return a.get_page() < b.get_page();
            });
        }

        int get_pages_read() {
            int s = 0;
            for (int i = 0; i < this->logs.size(); i++) {
                s += logs[i].get_pages();
            }
            return s;
        }
    public:
        Book(string title, string author, string release_date, int pages) {
            this->title = title;
            this->author = author;
            this->release_date = Date(release_date);
            this->pages = pages;
        };

        string get_line_str() {
            return this->title + ", " + author ;
        };

        string get_str() {
            return "Title: " + this->title + "\n"
                + "Author: " + this->author + "\n"
                + "Release Date: " + this->release_date.get_str(false)
                + "Pages: " + to_string(this->pages)
                + "Sessions: " + to_string(this->logs.size())
                + "Quotes: " + to_string(this->quotes.size());
        };

        vector<Quote> get_quotes() {
            return this->quotes;
        };

        vector<Log> get_logs() {
            return this->logs;
        };
};

bool h_clean_buf();

void ui_home();
void ui_search_book();
void ui_view_logs();
void ui_view_quotes();
void ui_edit_book();
void ui_add_logs();
void ui_delete_logs();
void ui_add_quote();
void ui_delete_quote();

void uih_clear();
void uih_logo();

void load_db();
void write_db();

vector<Book> books;

string book_file = "books.txt";
string log_file = "logs.txt";
string quote_file = "quotes.txt";

int main() {
    load_db();
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
            break;
        case 2:
            ui_view_logs();
            break;
        case 3:
            ui_view_quotes();
            break;
        case 4:
            exit(0);
            break;
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

    // TODO: implement
}

void ui_view_logs() {

    uih_clear();
    uih_logo();

    // TODO: implement
}

void ui_view_quotes() {

    uih_clear();
    uih_logo();

    // TODO: implement
};

void ui_edit_book() {

    uih_clear();
    uih_logo();

    // TODO: implement
};

void ui_add_logs() {

    uih_clear();
    uih_logo();

    // TODO: implement
};

void ui_delete_logs() {

    uih_clear();
    uih_logo();

    // TODO: implement
};

void ui_add_quote() {

    uih_clear();
    uih_logo();

    // TODO: implement
};

void ui_delete_quote() {

    uih_clear();
    uih_logo();

    // TODO: implement
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

void load_db() {
    // TODO: implement
};

void write_db() {
    // TODO: implement
};
