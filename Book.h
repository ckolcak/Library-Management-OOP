#ifndef BOOK_H
#define BOOK_H

#include <string>

class Book{
private:
    std::string bookID;
    std::string title;
    std::string author;
    std::string category;
    int availableStock;
    
public:
    Book(const std::string& bookID, const std::string& title, const std::string& author, const std::string& category, int stock);
    
    std::string getBookID() const;
    std::string getTitle() const;
    std::string getAuthor() const;
    std::string getCategory() const;
    int getAvailableStock() const;

    void increaseStock();
    void decreaseStock();

    bool operator==(const Book& other) const;

};

#endif