#include "Library.h"
#include <iostream>
#include <algorithm>

void Library::addBook(const Book& b) {
    for(int i = 0; i < books.size(); i++){
        if(b == books[i]){
            return;
        }
    }
    books.push_back(b);
    saveBookToFile(b);
}

void Library::removeBook(const std::string& id) {
    if(borrowManager.isBookBorrowed(id)){
        std::cout << "Book is currently borrowed and cannot be removed." << std::endl;
        return;
    }
    for(int i = 0; i < books.size(); i++){
        if(books[i].getBookID() == id){
            books.erase(books.begin() + i);
            saveBooksToFile();
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

void Library::loadBooksFromFile() {
    std::ifstream textRead("Books.txt");
    std::string sentence;
    while(std::getline(textRead,sentence)){
        std::stringstream word(sentence);
        std::string bookid,bookname,authorname,category,stock;
        std::getline(word,bookid,'|');
        std::getline(word,bookname,'|');
        std::getline(word,authorname,'|');
        std::getline(word,category,'|');
        std::getline(word,stock,'|');
        Book book1(bookid,bookname,authorname,category,std::stoi(stock));
        addBook(book1);
    }
}


void Library::saveBookToFile(const Book& b) {
    std::ofstream textOpenBook;
    textOpenBook.open("Books.txt",std::ios::app);
    textOpenBook << b.getBookID() << "|" << b.getTitle() << "|" << b.getAuthor() << "|" << b.getCategory() << "|" << b.getAvailableStock() << std::endl;
    textOpenBook.close();
}

void Library::saveBooksToFile() {
    std::ofstream textOpenBook("Books.txt");
    for(int i = 0; i < books.size(); i++){
        textOpenBook << books[i].getBookID() << "|" 
        << books[i].getTitle() << "|" 
        << books[i].getAuthor() << "|" 
        << books[i].getCategory() << "|" 
        << books[i].getAvailableStock() << std::endl;
    }
    textOpenBook.close();
}

void Library::addMember(const Member& m) {
    for(int i = 0; i < members.size(); i++){
        if(m == members[i]){
            return;
        }
    }
    members.push_back(m);
    saveMemberToFile(m);
}

void Library::removeMember(const std::string& id) {
    if(borrowManager.getActiveBorrowCount(id) > 0){
        std::cout << "Cannot remove a member who has borrowed books" << std::endl;
        return; 
    }
    for(int i = 0; i < members.size(); i++) {
        if(members[i].getUserID() == id) {
            members.erase(members.begin() + i);
            saveMembersToFile();
            return;
        }
    }
    std::cout << "Member not found" << std::endl;
}

Member* Library::findMemberByID(const std::string& id) { 
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

void Library::loadMembersFromFile() {
    std::ifstream textRead("Members.txt");
    std::string sentence;
    while(std::getline(textRead,sentence)){
        std::stringstream word(sentence);
        std::string userid,name,number,mail;
        std::getline(word,userid,'|');
        std::getline(word,name,'|');
        std::getline(word,number,'|');
        std::getline(word,mail,'|');
        Member user1(userid,name,number,mail);
        addMember(user1);
    }
}

void Library::saveMemberToFile(const Member& m) {
    std::ofstream textOpenMember;
    textOpenMember.open("Members.txt",std::ios::app);
    textOpenMember << m.getUserID() << "|" << m.getName() << "|" << m.getNumber() << "|" << m.getMail() << std::endl;
    textOpenMember.close();
}

void Library::saveMembersToFile() {
    std::ofstream textOpenMember;
    textOpenMember.open("Members.txt");
    for(int i = 0; i < members.size(); i++){
        textOpenMember << members[i].getUserID() << "|" << members[i].getName() << "|" << members[i].getNumber() << "|" << members[i].getMail() << std::endl;
    }
    textOpenMember.close();
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