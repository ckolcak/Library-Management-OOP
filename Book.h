#ifndef BOOK_H
#define BOOK_H

#include <string>

class Book{
private:
    const int bookID;
    std::string title;
    std::string author;
    std::string category;
    int availableStock;
    
public:
    Book(int ID, const std::string& title_, const std::string& author_, const std::string& katagori, int stck);
    
    int getBookID() const;
    std::string getTitle() const;
    std::string getAuthor() const;
    std::string getCategory() const;
    int getAvailableStock() const;

    void setCategory(const std::string& kategori);
    void setAvailableStock(int stck);

    void print() const;
};

#endif