#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include <vector>
#include "Member.h"
#include "Book.h"
#include "BorrowRecord.h"

class Library{
private:
   std::vector <Book> books;
   std::vector <Member> members;
   std::vector <BorrowRecord> borrowRecords;

public:
   void addBook(const Book& b);
   void addMember(const Member& m);
   
   Book* findBookByID(int id);
   Member* findMemberByID(const std::string& id);
   
   void borrowBook(const std::string& memberID, int bookID);
   void returnBook(const std::string& memberID, int bookID);

};

#endif