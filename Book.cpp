#include <iostream>
#include "Book.h"

Book::Book(int ID, const std::string& title_, const std::string& author_, const std::string& katagori, int stck) 
: bookID(ID), title(title_), author(author_), category(katagori), availableStock(stck < 0 ? 0 : stck) {}

int Book::getBookID() const {return bookID;}
std::string Book::getTitle() const {return title;}
std::string Book::getAuthor() const {return author;}
std::string Book::getCategory() const {return category;}
int Book::getAvailableStock() const {return availableStock;}

void Book::setCategory(const std::string& kategori) {category = kategori;}
void Book::setAvailableStock(int stck){
    availableStock = (stck < 0 ? 0 : stck);
}

void Book::print() const{
    std::cout << "ID: " << bookID << std::endl;
    std::cout << "Title: " << title << std::endl;
    std::cout << "Author: " << author << std::endl;
    std::cout << "Category: " << category << std::endl;
    std::cout << "Stock: " << availableStock << std::endl;
}
