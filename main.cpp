#include <cctype>
#include <cstdlib>
#include <iostream>
#include <cmath>
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

// CLASSES
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
            for (Log l : this->logs) {
                s += l.get_pages();
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
                + "Release Date: " + this->release_date.get_str(false) + "\n"
                + "Pages: " + to_string(this->pages) + "\n"
                + "Sessions: " + to_string(this->logs.size()) + "\n"
                + "Quotes: " + to_string(this->quotes.size()) + "\n";
        };

        vector<Quote> get_quotes() {
            return this->quotes;
        };

        vector<Log> get_logs() {
            return this->logs;
        };

        string get_title() {
            return this->title;
        }
};

// FUNCTIONS
void ui_home();
void ui_search_book();
void ui_view_books(vector<Book> books);
void ui_view_logs();
void ui_view_quotes();
void ui_edit_book();
void ui_add_logs();
void ui_delete_logs();
void ui_add_quote();
void ui_delete_quote();

void uih_clear();
void uih_header();
void uih_list(vector<string>& items, string connect, int set);

vector<Book> search_book(string book_title);
void load_db();
void write_db();

bool h_clean_buf();

// GLOBAL CONSTANTS
string const CONNECTOR = "\n|\n|\n";
string const ARROW = "\n|\nv\n";
string const BOOK_FILE = "books.txt";
string const LOG_FILE = "logs.txt";
string const QUOTE_FILE = "quotes.txt";

// GLOBAL VARIABLES
vector<Book> books;
string alert = "";

int main() {
    load_db();
    while (true) {ui_home();}
}

void ui_home() {
    int option;

    uih_clear();
    uih_header();

    cout << "(1) search book\n"
            "(2) add book\n"
            "(3) view sessions\n"
            "(4) view quotes\n"
            "\n"
            "(5) exit\n";

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
            alert = "unavailable option";
            return;
    }
}
void ui_search_book() {
    string book_title;

    uih_clear();
    uih_header();

    cout << "book title> ";
    getline(cin, book_title);

    ui_view_books(search_book(book_title));
}

vector<Book> search_book(string book_title) {
    for (char& c : book_title) { c = tolower(c); }

    vector<Book> related_books;

    for (Book b : books) {
        string bt = b.get_title();
        for (char& c : bt) { c = tolower(c); }

        if (bt.find(book_title) != string::npos) {
            related_books.push_back(b);
        }
    }
    
    return related_books;
}

void ui_view_books(vector<Book> bs) {
    if (bs.size() == 0) {
        alert = "no books found";
        return;
    }

    char option;
    int cur_set = 0;
    int max_sets = ceil((float) bs.size() / 5);
    vector<string> str_books;
    for (Book b: bs) {str_books.push_back(b.get_line_str());};

    while (true) {
        uih_clear();
        uih_header();

        uih_list(str_books, CONNECTOR, cur_set);

        cout << "\n\n(p) previous (n) next (q) quit\n\n";
        cout << "option> ";
        cin >> option;

        switch (option) {
            case 'n':
                if (cur_set < max_sets - 1) {
                    cur_set++;
                } else {
                    alert = "no next page";
                }
                break;
            case 'p':
                if (cur_set > 0) {
                    cur_set--;
                } else {
                    alert = "no previous page";
                }
                break;
            case 'q':
                return;
        }
    }
}


void ui_view_logs() {

    uih_clear();
    uih_header();

    // TODO: implement
}

void ui_view_quotes() {

    uih_clear();
    uih_header();

    // TODO: implement
};

void ui_edit_book() {

    uih_clear();
    uih_header();

    // TODO: implement
};

void ui_add_logs() {

    uih_clear();
    uih_header();

    // TODO: implement
};

void ui_delete_logs() {

    uih_clear();
    uih_header();

    // TODO: implement
};

void ui_add_quote() {

    uih_clear();
    uih_header();

    // TODO: implement
};

void ui_delete_quote() {

    uih_clear();
    uih_header();

    // TODO: implement
};

void uih_header() {
    cout << "+============================+\n"
            "|       ______ ______        |\n"
            "|     _/      Y      \\_      |\n"
            "|    // ~ ~~  | ~ ~~~ \\\\     |\n"
            "|   // ~~ ~~~ | ~~~~ ~ \\\\    |\n"
            "|  //________.|.________\\\\   |\n"
            "|  '---------'-'---------'   |\n"
            "|        NOVELTY v1.0        |\n"
            "+============================+\n";

    if (!alert.empty()) {
        cout << alert + "\n\n";
        alert = "";
    }
}

void uih_clear() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

 void uih_list(vector<string>& items, string connect, int set) {
    int c = 5 * set;
    for (int i = c; i < c + 5; i++) {
        if (i < items.size() - 1) {
            if (!(i % 5 == 0)) {
                cout << connect;
            };
            cout << "(" << i + 1 << ") "
                << items.at(i);
        }
    }
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
    books.push_back(Book("The Great Gatsby", "F. Scott Fitzgerald", "1925-04-10", 180));
    books.push_back(Book("To Kill a Mockingbird", "Harper Lee", "1960-07-11", 324));
    books.push_back(Book("1984", "George Orwell", "1949-06-08", 328));
    books.push_back(Book("Pride and Prejudice", "Jane Austen", "1813-01-28", 432));
    books.push_back(Book("The Catcher in the Rye", "J.D. Salinger", "1951-07-16", 234));
    books.push_back(Book("Animal Farm", "George Orwell", "1945-08-17", 112));
    books.push_back(Book("Lord of the Flies", "William Golding", "1954-09-17", 224));
    books.push_back(Book("Brave New World", "Aldous Huxley", "1932-08-18", 311));
    books.push_back(Book("The Hobbit", "J.R.R. Tolkien", "1937-09-21", 310));
    books.push_back(Book("Fahrenheit 451", "Ray Bradbury", "1953-10-19", 194));
    books.push_back(Book("Jane Eyre", "Charlotte Brontë", "1847-10-16", 507));
    books.push_back(Book("Wuthering Heights", "Emily Brontë", "1847-12-01", 416));
    books.push_back(Book("The Odyssey", "Homer", "750-01-01", 484));
    books.push_back(Book("Crime and Punishment", "Fyodor Dostoevsky", "1866-01-01", 671));
    books.push_back(Book("The Brothers Karamazov", "Fyodor Dostoevsky", "1880-11-01", 824));
    books.push_back(Book("War and Peace", "Leo Tolstoy", "1869-01-01", 1225));
    books.push_back(Book("Anna Karenina", "Leo Tolstoy", "1877-01-01", 864));
};

void write_db() {
    // TODO: implement
};
