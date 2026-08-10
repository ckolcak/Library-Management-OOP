#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include <vector>
#include "Member.h"
#include "Book.h"
#include "BorrowManager.h"

class Library{
private:
   std::vector<Book> books;
   std::vector<Member> members;
   BorrowManager borrowManager;

public:
   void addBook(const Book& b);
   void removeBook(const std::string& id);
   Book* findBookByID(const std::string& id);
   void displayAllBooks() const;

   void addMember(const Member& m);
   void removeMember(const std::string& id);
   Member* findMemberByID(const std::string& id);
   void displayAllMembers() const;

   void displayBorrowInfo() const;
   
   void borrowBook(const std::string& memberID, const std::string& bookID);
   void returnBook(const std::string& memberID, const std::string& bookID);

};

#endif