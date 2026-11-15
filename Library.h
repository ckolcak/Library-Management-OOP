#ifndef LIBRARY_H
#define LIBRARY_H

#include "Member.h"
#include "BookApiClient.h"
#include "DatabaseManager.h"
#include <string>

class Library{
private:
   BookApiClient bookApiClient;
   DatabaseManager dbManager;

public:
   bool addBookByISBN(const std::string& isbn, int stock);
   bool removeBook(const std::string& isbn);

   bool addMember(const Member& m);
   bool removeMember(const std::string& id);

   void displayBooks(int limit, int offset) const;
   void displayMembers(int limit, int offset) const;
   void displayBorrowRecords(int limit, int offset) const;
   
   bool borrowBook(const std::string& memberID, const std::string& isbn);
   bool returnBook(const std::string& memberID, const std::string& isbn);

};

#endif