#include <cctype>
#include <cstdlib>
#include <fstream>
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

        long get_comparable() {
            return stol(this->year + this->month + this->day
                    + this->hour + this->minute + this->second);
        }
};

class Quote {
    private:
        string content;
        int page;
        string* book_title; 
    public:
        Quote(string content, int page) {
            this->content = content;
            this->page = page;
        }
        string get_str() {
            return *this->book_title +
                " [page " + to_string(this->page) + "]\n" + this->content;
        }
        string get_line_str() {
            return "[page " + to_string(this->page) + "] " + this->content;
        }
        int get_page() {
            return this->page;
        }
        string get_content() {
            return this->content;
        }
        int get_comparable() {
            return this->page;
        }
        void set_book_title(string* book_title) {
            this->book_title = book_title;
        }
};

class Log {
    private:
        int pages;
        Date time;
        string* book_title;
    public:
        Log(int pages, string time) {
            this->pages = pages;
            this->time = Date(time);
        }
        int get_pages() {
            return this->pages;
        }
        string get_date() {
            return this->time.get_str(true);
        }
        string get_str() {
            return *this->book_title + "\n" + to_string(this->pages)
                + " pages [" + this->time.get_str(true) + "]";
        }
        string get_line_str() {
            return to_string(this->pages) + " pages [" + this->time.get_str(true) + "]";
        }
        int get_comparable() {
            return this->time.get_comparable();
        }
        void set_book_title(string* book_title) {
            this->book_title = book_title;
        }
};

class Book {
    private:
        string title, author;
        Date release_date;
        int pages;
        vector<Quote> quotes;
        vector<Log> logs;

        void sort_logs() {
            sort(this->logs.begin(), this->logs.end(), [](Log& a, Log& b) {
                return a.get_comparable() > b.get_comparable();
            });
        }

        void sort_quotes() {
            sort(this->quotes.begin(), this->quotes.end(), [](Quote& a, Quote& b) {
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
        }

        bool add_log(Log log) {
            if (log.get_pages() + this->get_pages_read() > this->pages) {
                return false;
            }
            log.set_book_title(&this->title);
            logs.push_back(log);
            this->sort_logs();
            return true;
        }

        bool add_quote(Quote quote) {
            if (quote.get_page() <= 0 || quote.get_page() > this->pages) {
                return false;
            }
            quote.set_book_title(&this->title);
            quotes.push_back(quote);
            this->sort_quotes();
            return true;
        }

        string get_line_str() {
            return this->title + ", " + author ;
        }

        string get_str() {
            return "Title: " + this->title + "\n"
                + "Author: " + this->author + "\n"
                + "Release Date: " + this->release_date.get_str(false) + "\n"
                + "Pages: " + to_string(this->pages) + "\n"
                + "Completed: " + to_string(this->get_pages_read()) + "\n"
                + "Sessions: " + to_string(this->logs.size()) + "\n"
                + "Quotes: " + to_string(this->quotes.size()) + "\n";
        }

        vector<Quote> get_quotes() {
            return this->quotes;
        }

        vector<Log> get_logs() {
            return this->logs;
        }

        string get_title() {
            return this->title;
        }

        string get_author() {
            return this->author;
        }

        string get_date() {
            return this->release_date.get_str(false);
        }

        int get_pages() {
            return this->pages;
        }

        string get_id() {
            return this->title + this->author;
        }

        void set_title(string title) {
            this->title = title;
        }

        void set_author(string author) {
            this->author = author;
        }

        void set_date(string date) {
            this->release_date = Date(date);
        }

        void set_pages(int pages) {
            this->pages = pages;
        }

        void delete_log(int index) {
            this->logs.erase(this->logs.begin() + index);
        }

        void delete_quote(int index) {
            this->quotes.erase(this->quotes.begin() + index);
        }
};

// FUNCTIONS
void ui_home();
void ui_search_book();
void ui_view_books(vector<Book> filtered_books);
void ui_view_book(Book& book);
void ui_add_book();
void ui_view_logs();
void ui_view_quotes();
void ui_edit_book(Book& book);
void ui_add_log(Book& book);
void ui_delete_log(Book& book);
void ui_add_quote(Book& book);
void ui_delete_quote(Book& book);

void uih_clear();
void uih_header();
void uih_list(vector<string>& items, int set, bool numbered);

vector<Book> search_book(string book_title);
void delete_book(string book_id);
vector<string> get_all_logs();
vector<string> get_all_quotes();
bool load_db();
void write_db();

bool h_clean_buf();
bool h_valid_date(string date);
int h_find_book(string book_id);
bool h_load_books();
bool h_load_logs();
bool h_load_quotes();

// GLOBAL CONSTANTS
int const MAX_LIST_ITEMS = 3;
string const CONNECTOR = "\n|\n|\n";
string const BOOK_FILE = "books.txt";
string const LOG_FILE = "sessions.txt";
string const QUOTE_FILE = "quotes.txt";

// GLOBAL VARIABLES
vector<Book> books;
string alert = "";

int main() {
    if (!load_db()) {
        cout << "files are corrupted, delete the existing files or fix them.\n";
        exit(1);
    }
    while (true) {ui_home();}
}

void ui_home() {
    int option;

    uih_clear();
    uih_header();

    cout << "(1) search book\n"
            "(2) add book\n"
            "\n"
            "(3) view sessions\n"
            "(4) view quotes\n"
            "\n"
            "(5) exit\n\n";

    cout << "option> ";
    cin >> option;
    if (h_clean_buf()) { option = -1; }

    switch (option) {
        case 1:
            ui_search_book();
            break;
        case 2:
            ui_add_book();
            break;
        case 3:
            ui_view_logs();
            break;
        case 4:
            ui_view_quotes();
            break;
        case 5:
            write_db();
            exit(0);
            break;
        default:
            alert = "invalid option";
            return;
    }
}

void ui_search_book() {
    string book_title;

    uih_clear();
    uih_header();

    cout << "book title> ";
    getline(cin, book_title);

    vector<Book> filtered_books = search_book(book_title);
    ui_view_books(filtered_books);
}

vector<Book> search_book(string input) {
    for (char& c : input) { c = tolower(c); }

    vector<Book> filtered_books;

    for (Book book : books) {
        string book_title = book.get_title();
        for (char& c : book_title) { c = tolower(c); }

        if (book_title.find(input) != string::npos) {
            filtered_books.push_back(book);
        }
    }

    return filtered_books;
}

void ui_view_books(vector<Book> filtered_books) {
    if (filtered_books.size() == 0) {
        alert = "no books found";
        return;
    }

    string option;
    int cur_set = 0;
    int max_sets = ceil((float) filtered_books.size() / MAX_LIST_ITEMS);
    string book_id;

    vector<string> str_books;
    for (Book book : filtered_books) {
        str_books.push_back(book.get_line_str());
    }
    while (true) {
        uih_clear();
        uih_header();

        uih_list(str_books, cur_set, true);
        cout << "\n\n(p) previous (n) next (q) quit\n\n";

        cout << "option> ";
        getline(cin, option);

        if (option == "n") {
            if (cur_set < max_sets - 1) {
                cur_set++;
            } else {
                alert = "no next page";
            }
        } else if (option == "p") {
            if (cur_set > 0) {
                cur_set--;
            } else {
                alert = "no previous page";
            }
        } else if (option == "q") {
            return;
        } else {
            bool is_digits = true;
            
            if (option.length() == 0) {
                alert = "invalid option";
                continue;
            }

            for (char c: option) {
                if (!isdigit(c)) {
                    is_digits = false;
                    break;
                }
            }

            if (!is_digits) {
                alert = "invalid option";
                continue;
            }

            int int_option = stoi(option);

            if (!(int_option > 0) || !(int_option <= filtered_books.size())) {
                alert = "invalid option";
                continue;
            }
            book_id = filtered_books.at(int_option - 1).get_id();
            ui_view_book(books.at(h_find_book(book_id)));
            return;
        }
    }
}

void ui_view_book(Book &book) {
    int option;

    while (true) {
        uih_clear();
        uih_header();

        cout << book.get_str() +
        "+============================+\n";

        cout << "(1) edit book\n"
                "\n"
                "(2) add session\n"
                "(3) delete session\n"
                "\n"
                "(4) add quote\n"
                "(5) delete quote\n"
                "\n"
                "(6) delete book\n"
                "\n"
                "(7) exit\n\n";

        cout << "option> ";
        cin >> option;
        if (h_clean_buf()) { option = -1; }

        switch (option) {
            case 1:
                ui_edit_book(book);
                break;
            case 2:
                ui_add_log(book);
                break;
            case 3:
                ui_delete_log(book);
                break;
            case 4:
                ui_add_quote(book);
                break;
            case 5:
                ui_delete_quote(book);
                break;
            case 6:
                delete_book(book.get_id());
                break;
            case 7:
                return;
            default:
                alert = "invalid option";
        }
    }
}

void delete_book(string book_comparable) {
    for (int i = 0; i < books.size(); i++) {
        if (books.at(i).get_id() == book_comparable) {
            books.erase(books.begin() + i);
            alert = "book deleted";
            return;
        }
    }
}

void ui_add_book() {
    string title, author, release_date;
    int pages;

    uih_clear();
    uih_header();

    cout << "title> ";
    getline(cin, title);

    cout << "author> ";
    getline(cin, author);

    cout << "release date (yyyy-mm-dd)> ";
    getline(cin, release_date);

    cout << "pages> ";
    cin >> pages;
    if (h_clean_buf()) {
        pages = -1;
    }

    if (!(title.length() > 0) || !(author.length() > 0) || !(pages > 0) || !h_valid_date(release_date)) {
        alert = "invalid data";
        return;
    }

    if (h_find_book(title + author) > 0) {
        alert = "book already exists";
        return;
    }

    books.push_back(Book(title, author, release_date, pages));
    alert = "book created";
}

void ui_edit_book(Book& book) {
    string title, author, release_date;
    int pages = 0;

    uih_clear();
    uih_header();
    
    cout << "Press enter to skip.\nEnter 0 for pages to skip.\n\n";
    cout << "title (" + book.get_title() + ")> ";
    getline(cin, title);

    cout << "author (" + book.get_author() + ")> ";
    getline(cin, author);

    cout << "release date (" + book.get_date() + ")> ";
    getline(cin, release_date);

    cout << "pages (" + to_string(book.get_pages()) + ")> ";
    cin >> pages;
    if (h_clean_buf()) {
        pages = -1;
    }
    
    if (title != book.get_title() || author != book.get_author()) {
        if (h_find_book(title + author) != -1) {
            alert = "book already exists";
            return;
        }
    } 

    // default to original values
    if (title.length() == 0) {
        title = book.get_title();
    }
    if (author.length() == 0) {
        author = book.get_author();
    }
    if (release_date.length() == 0) {
        release_date = book.get_date();
    }
    if (pages == 0) {
        pages = book.get_pages();
    }

    if (!h_valid_date(release_date)) {
        alert = "invalid date";
        return;
    }
    
    book.set_title(title);
    book.set_author(author);
    book.set_date(release_date);
    book.set_pages(pages);
    alert = "book edited";
}

void ui_view_logs() {
    vector<string> str_logs = get_all_logs();
    
    if (str_logs.size() == 0) {
        alert = "no sessions exist";
        return;
    }

    int max_sets = ceil((float) str_logs.size() / MAX_LIST_ITEMS);
    int cur_set = 0;
    char option;

    while (true) {
        uih_clear();
        uih_header();

        uih_list(str_logs, cur_set, false);
        cout << "\n\n(p) previous (n) next (q) quit\n\n";

        cout << "option> ";
        cin >> option;
        if (h_clean_buf()) { option = '/'; }

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
            default:
                alert = "invalid option";
        }
    }
}

vector<string> get_all_logs() {
    vector<Log> logs;
    vector<string> str_logs;

    for (Book& b: books) {
        for (Log& l: b.get_logs()) {
            logs.push_back(l);
        }
    }

    sort(logs.begin(), logs.end(), [](Log& a, Log& b) {
        return a.get_comparable() > b.get_comparable();
    });

    for (Log& l: logs) {
        str_logs.push_back(l.get_str());
    }

    return str_logs;
}

void ui_view_quotes() {
    vector<string> str_quotes = get_all_quotes();
    
    if (str_quotes.size() == 0) {
        alert = "no quotes exist";
        return;
    }

    int max_sets = ceil((float) str_quotes.size() / MAX_LIST_ITEMS);
    int cur_set = 0;
    char option;

    while (true) {
        uih_clear();
        uih_header();

        uih_list(str_quotes, cur_set, false);
        cout << "\n\n(p) previous (n) next (q) quit\n\n";

        cout << "option> ";
        cin >> option;
        if (h_clean_buf()) { option = '/'; }

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
            default:
                alert = "invalid option";
        }
    }
}

vector<string> get_all_quotes() {
    vector<Quote> quotes;
    vector<string> str_quotes;

    for (Book& b: books) {
        for (Quote& q: b.get_quotes()) {
            quotes.push_back(q);
        }
    }

    sort(quotes.begin(), quotes.end(), [](Quote& a, Quote& b) {
        return a.get_comparable() > b.get_comparable();
    });

    for (Quote& q: quotes) {
        str_quotes.push_back(q.get_str());
    }

    return str_quotes;
}

void ui_add_log(Book& book) {
    int pages;
    string time;

    uih_clear();
    uih_header();

    cout << "pages> ";
    cin >> pages;
    if (h_clean_buf() || pages <= 0) {
        alert = "invalid pages";
        return;
    }

    cout << "(leave time empty for current time)\n";
    cout << "time> ";
    getline(cin, time);
    if (time.length() == 0) {
        time = Date().get_str(true);
    } else if (!h_valid_date(time)) {
        alert = "invalid date";
        return;
    }
    
    if (!book.add_log(Log(pages, time))) {
        alert = "invalid pages";
    }
}

void ui_delete_log(Book& book) {
    uih_clear();
    uih_header();

    vector<string>str_logs;
    for (Log l: book.get_logs()) {
        str_logs.push_back(l.get_line_str());
    }
    
    if (str_logs.size() == 0) {
        alert = "no sessions exist";
        return;
    }

    int max_sets = ceil((float) str_logs.size() / MAX_LIST_ITEMS);
    int cur_set = 0;
    string option;

    while (true) {
        uih_clear();
        uih_header();

        uih_list(str_logs, cur_set, true);
        cout << "\n\n(p) previous (n) next (q) quit\n\n";

        cout << "option> ";
        getline(cin, option);

        if (option == "n") {
            if (cur_set < max_sets - 1) {
                cur_set++;
            } else {
                alert = "no next page";
            }
        } 
        else if (option == "p") {
            if (cur_set > 0) {
                cur_set--;
            } else {
                alert = "no previous page";
            }
        } 
        else if (option == "q") {
            return;
        } 
        else {
            bool is_digits = true;
            
            if (option.length() == 0) {
                alert = "invalid option";
                continue;
            }

            for (char c: option) {
                if (!isdigit(c)) {
                    is_digits = false;
                    break;
                }
            }

            if (!is_digits) {
                alert = "invalid option";
                continue;
            }

            int int_option = stoi(option);

            if (!(int_option > 0) || !(int_option <= str_logs.size())) {
                alert = "invalid option";
                continue;
            }
            
            book.delete_log(int_option - 1);
            alert = "deleted session";
            return;
        }
    }
}

void ui_add_quote(Book& book) {
    string content;
    int page;
    int total_pages = book.get_pages();

    uih_clear();
    uih_header();

    cout << "page> ";
    cin >> page;
    if (h_clean_buf() || page <= 0 || page > total_pages) {
        alert = "invalid page";
        return;
    }

    cout << "quote> ";
    getline(cin, content);
    if (content.length() == 0) {
        alert = "invalid quote";
        return;
    }

    if (!book.add_quote(Quote(content, page))) {
        alert = "invalid page";
    }
}

void ui_delete_quote(Book& book) {
    uih_clear();
    uih_header();

    vector<string>str_quotes;
    for (Quote q: book.get_quotes()) {
        str_quotes.push_back(q.get_line_str());
    }
    
    if (str_quotes.size() == 0) {
        alert = "no quotes exist";
        return;
    }

    int max_sets = ceil((float) str_quotes.size() / MAX_LIST_ITEMS);
    int cur_set = 0;
    string option;

    while (true) {
        uih_clear();
        uih_header();

        uih_list(str_quotes, cur_set, true);
        cout << "\n\n(p) previous (n) next (q) quit\n\n";

        cout << "option> ";
        getline(cin, option);

        if (option == "n") {
            if (cur_set < max_sets - 1) {
                cur_set++;
            } else {
                alert = "no next page";
            }
        } 
        else if (option == "p") {
            if (cur_set > 0) {
                cur_set--;
            } else {
                alert = "no previous page";
            }
        } 
        else if (option == "q") {
            return;
        } 
        else {
            bool is_digits = true;
            
            if (option.length() == 0) {
                alert = "invalid option";
                continue;
            }

            for (char c: option) {
                if (!isdigit(c)) {
                    is_digits = false;
                    break;
                }
            }

            if (!is_digits) {
                alert = "invalid option";
                continue;
            }

            int int_option = stoi(option);

            if (!(int_option > 0) || !(int_option <= str_quotes.size())) {
                alert = "invalid option";
                continue;
            }
            
            book.delete_log(int_option - 1);
            alert = "deleted session";
            return;
        }
    }
}

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
        cout << alert +
            "\n+============================+\n";
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

void uih_list(vector<string>& items, int set, bool numbered) {
    int c = MAX_LIST_ITEMS * set;
    for (int i = c; i < c + MAX_LIST_ITEMS; i++) {
        if (i < items.size()) {
            if (!(i % MAX_LIST_ITEMS == 0)) {
                cout << CONNECTOR;
            }
            if (numbered) {
                cout << "(" << i + 1 << ") "
                    << items.at(i);
            } else {
                cout << "* "
                    << items.at(i);
            }
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

bool h_valid_date(string date) {
    if (date.length() < 10) {
        return false;
    }
    if (date[4] != '-' || date[7] != '-') {
        return false;
    }

    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) {
            continue;
        }
        if (!isdigit(date[i])) {
            return false;
        }
    }

    int year = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));

    if (month < 1 || month > 12) {
        return false;
    }

    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    bool is_leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (is_leap) {
        days_in_month[1] = 29;
    }

    if (day < 1 || day > days_in_month[month - 1]) {
        return false;
    }

    if (date.length() > 10) {
        if (date.length() != 19) {
            return false;
        }
        if (date[10] != ' ' || date[13] != ':' || date[16] != ':') {
            return false;
        }

        for (int i = 11; i < 19; i++) {
            if (i == 13 || i == 16) {
                continue;
            }
            if (!isdigit(date[i])) {
                return false;
            }
        }

        int hour = stoi(date.substr(11, 2));
        int minute = stoi(date.substr(14, 2));
        int second = stoi(date.substr(17, 2));

        if (hour < 0 || hour > 23) {
            return false;
        }
        if (minute < 0 || minute > 59) {
            return false;
        }
        if (second < 0 || second > 59) {
            return false;
        }
    }

    return true;
}

int h_find_book(string book_id) {
    for (int i = 0; i < books.size(); i++) {
        if (book_id == books.at(i).get_id()) {
            return i;
        }
    }
    return -1;
}

bool load_db() {
    if (!h_load_books() || !h_load_logs() || !h_load_quotes()) {
        return false;
    }
    return true;
}

bool h_load_books() {
    string title, author, date, str_pages;

    ifstream b_check(BOOK_FILE);
    if (!b_check.good()) {
        ofstream b_create(BOOK_FILE);
        if (!b_create.good()) {
            return false;
        }
        b_create.close();
    }
    b_check.close();

    ifstream books_file(BOOK_FILE);
    if (!books_file.good()) {
        return false;
    }

    while (getline(books_file, title)) {
        if (title.empty()) {
            continue;
        }
        
        if (!getline(books_file, author) || author.empty() ||
            !getline(books_file, date) || date.empty() ||
            !getline(books_file, str_pages) || str_pages.empty()) {
            return false;
        }
        
        for (char c : str_pages) {
            if (!isdigit(c)) {
                return false;
            }
        }
        
        int pages = stoi(str_pages);
        if (pages <= 0) {
            return false;
        }
        
        books.push_back(Book(title, author, date, pages));
    }
    books_file.close();
    return true;
}

bool h_load_logs() {
    ifstream l_check(LOG_FILE);
    if (!l_check.good()) {
        ofstream l_create(LOG_FILE);
        if (!l_create.good()) {
            return false;
        }
        l_create.close();
    }
    l_check.close();

    ifstream logs_file(LOG_FILE);
    if (!logs_file.good()) {
        return false;
    }
    
    string book_id, str_pages, date;
    while (getline(logs_file, book_id)) {
        if (book_id.empty()) {
            continue;
        }
        
        if (!getline(logs_file, str_pages) || str_pages.empty() ||
            !getline(logs_file, date) || date.empty()) {
            return false;
        }
        
        for (char c : str_pages) {
            if (!isdigit(c)) {
                return false;
            }
        }
        
        int pages = stoi(str_pages);
        if (pages <= 0) { 
            return false;
        }
        
        for (Book& current_book : books) {
            if (current_book.get_id() == book_id) {
                current_book.add_log(Log(pages, date));
                break;
            }
        }
    }
    logs_file.close();
    return true;
}

bool h_load_quotes() {
    ifstream q_check(QUOTE_FILE);
    if (!q_check.good()) {
        ofstream q_create(QUOTE_FILE);
        if (!q_create.good()) {
            return false;
        }
        q_create.close();
    }
    q_check.close();

    ifstream quotes_file(QUOTE_FILE);
    if (!quotes_file.good()) {
        return false;
    }
    
    string quote_book_id, page_str, content_str;
    while (getline(quotes_file, quote_book_id)) {
        if (quote_book_id.empty()) {
            continue;
        }
        
        if (!getline(quotes_file, page_str) || page_str.empty() ||
            !getline(quotes_file, content_str) || content_str.empty()) {
            return false;
        }
        
        bool page_is_digits = true;
        for (char digit_char : page_str) {
            if (!isdigit(digit_char)) {
                page_is_digits = false;
                break;
            }
        }
        
        if (!page_is_digits) {
            return false;
        }
        
        int quote_page = stoi(page_str);
        if (quote_page <= 0) {
            return false;
        }
        
        for (Book& current_book : books) {
            if (current_book.get_id() == quote_book_id) {
                current_book.add_quote(Quote(content_str, quote_page));
                break;
            }
        }
    }
    quotes_file.close();
    return true;
}

void write_db() {
    ofstream books_file(BOOK_FILE);
    ofstream logs_file(LOG_FILE);
    ofstream quotes_file(QUOTE_FILE);

    for (Book& b : books) {
        books_file << b.get_title() << endl;
        books_file << b.get_author() << endl;
        books_file << b.get_date() << endl;
        books_file << b.get_pages() << endl;
        for (Log& l : b.get_logs()) {
            logs_file << b.get_id() << endl;
            logs_file << l.get_pages() << endl;
            logs_file << l.get_date() << endl;
        }
        for (Quote& q : b.get_quotes()) {
            quotes_file << b.get_id() << endl;
            quotes_file << q.get_page() << endl;
            quotes_file << q.get_content() << endl;
        }
    }

    books_file.close();
    logs_file.close();
    quotes_file.close();
}
