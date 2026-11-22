#include "Book.h"

Book::Book(const std::string& isbn, const std::string& title, const std::string& author, const std::string& category, int stock)
: isbn(isbn), title(title), author(author), category(category), availableStock(stock < 0 ? 0 : stock) {}

std::string Book::getISBN() const {return isbn;}
std::string Book::getTitle() const {return title;}
std::string Book::getAuthor() const {return author;}
std::string Book::getCategory() const {return category;}
int Book::getAvailableStock() const {return availableStock;}

