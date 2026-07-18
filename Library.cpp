#include "Library.h"
#include <iostream>

void Library::addBook (const Book& b) {books.push_back(b);}
void Library::addMember (const Member& m) {members.push_back(m);}

Book* Library::findBookByID(int id){
    for(int i = 0; i < books.size(); i++){
        if(books[i].getBookID() == id){
        return &books[i];
        }
    }
    return nullptr;
}

Member* Library::findMemberByID(const std::string& id){ 
    for(int i = 0; i < members.size(); i++){            
        if(members[i].getUserID() == id){               
        return &members[i];                             
        }                
    }                    
    return nullptr;
}

void Library::borrowBook(const std::string& memberID, int bookID){
    Member* user1 = findMemberByID(memberID);
    if(user1 == nullptr){
        std::cout << "Member not found!" << std::endl;
        return;
    }
    Book* book1 = findBookByID(bookID);
    if(book1 == nullptr){
        std::cout << "Book not found!" << std::endl;
        return;
    }
    if(user1->hasBook(bookID)){
        std::cout << "You already have the book!" << std::endl;
        return;
    }
    if(user1->getBorrowedBookID().size() >= user1->getMaxBorrowLimit()){
        std::cout << "You can't borrow more than 3 books!" << std::endl;
        return;
    }
    if(book1->getAvailableStock() <= 0){
        std::cout << "This book doesn't have available stock" << std::endl;
        return;
    }
    user1->addBook(bookID);
    book1->setAvailableStock(book1->getAvailableStock() - 1);
}

void Library::returnBook(const std::string& memberID, int bookID){
    Member* user2 = findMemberByID(memberID);
    if(user2 == nullptr){
        std::cout << "Member not found!" << std::endl;
        return;
    }
    Book* book2 = findBookByID(bookID);
    if(book2 == nullptr){
        std::cout << "Book not found!" << std::endl;
        return;
    }
    if(!(user2->hasBook(bookID))){
        std::cout << "You don't have this book!" << std::endl;
        return;
    }
    user2->subtractBook(bookID);
    book2->setAvailableStock(book2->getAvailableStock() + 1);
}