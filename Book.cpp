#include "Book.h"

Book::Book(const std::string& bookID, const std::string& title, const std::string& author, const std::string& category, int stock)
: bookID(bookID), title(title), author(author), category(category), availableStock(stock < 0 ? 0 : stock) {}

std::string Book::getBookID() const {return bookID;}
std::string Book::getTitle() const {return title;}
std::string Book::getAuthor() const {return author;}
std::string Book::getCategory() const {return category;}
int Book::getAvailableStock() const {return availableStock;}

void Book::increaseStock(){
    ++availableStock;
}
void Book::decreaseStock(){
    availableStock = (availableStock <= 0 ? 0 : --availableStock);
}

bool Book::operator==(const Book& other) const {
    return bookID == other.bookID;
}

