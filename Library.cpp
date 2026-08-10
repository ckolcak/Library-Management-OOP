#include "Library.h"
#include <iostream>
#include <algorithm>

void Library::addBook(const Book& b){
    for(int i = 0; i < books.size(); i++){
        if(b == books[i]){
            return;
        }
    }
    books.push_back(b);
}

void Library::removeBook(const std::string& id){
    if(borrowManager.isBookBorrowed(id)){
        std::cout << "Book is currently borrowed and cannot be removed." << std::endl;
        return;
    }
    for(int i = 0; i < books.size(); i++){
        if(books[i].getBookID() == id){
            books.erase(books.begin() + i);
            return;
        }
    }
    std::cout << "Book not found" << std::endl;
}

Book* Library::findBookByID(const std::string& id){
    for(int i = 0; i < books.size(); i++){
        if(books[i].getBookID() == id){
        return &books[i];
        }
    }
    return nullptr;
}

void Library::displayAllBooks() const {
    if(!books.size()){
        std::cout << "There are currently no books in the system" << std::endl;
        return;
    }
    for(int i = 0; i < books.size(); i++){
        std::cout << books[i].getTitle() << " | " 
        << books[i].getAuthor() << " | " 
        << books[i].getBookID() << " | " 
        << books[i].getCategory() << " | " 
        << books[i].getAvailableStock() << std::endl;
    }
}

void Library::addMember(const Member& m){
    for(int i = 0; i < members.size(); i++){
        if(m == members[i]){
            return;
        }
    }

    members.push_back(m);
}

void Library::removeMember(const std::string& id){
    if(borrowManager.getActiveBorrowCount(id) > 0){
        std::cout << "Cannot remove a member who has borrowed books" << std::endl;
        return; 
    }
    for(int i = 0; i < members.size(); i++){
        if(members[i].getUserID() == id) {
            members.erase(members.begin() + i);
            return;
        }
    }
    std::cout << "Member not found" << std::endl;
}

Member* Library::findMemberByID(const std::string& id){ 
    for(int i = 0; i < members.size(); i++){            
        if(members[i].getUserID() == id){               
        return &members[i];                             
        }                
    }                    
    return nullptr;
}

void Library::displayAllMembers() const {
    if(members.empty()){
        std::cout << "There are currently no members in the system" << std::endl;
        return;
    }
    for(int i = 0; i < members.size();i++){
        std::cout << members[i].getName() << " | " 
        << members[i].getUserID() << " | " 
        << members[i].getNumber() << " | " 
        << members[i].getMail() << std::endl;
    }
}

void Library::displayBorrowInfo() const {
    borrowManager.bm_displayBorrowInfo();
}

void Library::borrowBook(const std::string& memberID, const std::string& bookID){
    Member* user = findMemberByID(memberID);
    if(user == nullptr){
        std::cout << "Member not found" << std::endl;
        return;
    }
    Book* book = findBookByID(bookID);
    if(book == nullptr){
        std::cout << "Book not found" << std::endl;
        return;
    }
    if(book->getAvailableStock() <= 0){
        std::cout << "Book is out of stock" << std::endl;
        return;
    }
    bool check = borrowManager.bm_BorrowBook(memberID, bookID);
    if(check){
    book->decreaseStock();
    }
}

void Library::returnBook(const std::string& memberID, const std::string& bookID){
    Member* user = findMemberByID(memberID);
    if(user == nullptr){
        std::cout << "Member not found!" << std::endl;
        return;
    }
    Book* book = findBookByID(bookID);
    if(book == nullptr){
        std::cout << "Book not found!" << std::endl;
        return;
    }
    if(!borrowManager.hasActiveBorrow(memberID,bookID)){
        std::cout << "You don't have this book!" << std::endl;
        return;
    }
    bool check = borrowManager.bm_ReturnBook(memberID,bookID);
    if(check){
    book->increaseStock();
    }
}