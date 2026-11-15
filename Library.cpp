#include "Library.h"
#include "TimeUtils.h"
#include <vector>
#include <iostream>
#include <optional>


bool Library::addBookByISBN(const std::string& isbn, int stock) {
    if(stock <= 0) {
        std::cerr << "Stock must be positive" << std::endl; 
        return false;
    }

    std::optional<Book> bk = dbManager.findBookByISBN(isbn);
    if(bk.has_value()) {
        if(!dbManager.increaseBookStock(isbn, stock)) {
            std::cerr << "Failed to update stock for ISBN: " + isbn << std::endl;
            return false;
        }
        return true;
    }

    std::string response = bookApiClient.fetchBookByISBN(isbn);
    bk = bookApiClient.parseBook(response, isbn, stock);
    if(bk == std::nullopt) {
        std::cerr << "Book not found" << std::endl;
        return false;
    }
    Book b = bk.value();
    dbManager.addBook(isbn, b.getTitle(), b.getAuthor(), b.getCategory(), b.getAvailableStock());
    return true;
}

bool Library::removeBook(const std::string& isbn) {
    if(dbManager.bookHasActiveBorrow(isbn)) {
        std::cerr << "Cannot remove book: it is currently borrowed" << std::endl;
        return false;
    }

    if(!dbManager.removeBook(isbn)) {
        std::cerr << "Book not found" << std::endl;
        return false;
    }
    return true;
}


bool Library::addMember(const Member& m) {
    std::optional <Member> mem = dbManager.findMemberByID(m.getUserID());
    if(mem.has_value()) {
        std::cerr << "Member already exists" << std::endl;
        return false;
    }

    dbManager.addMember(m.getUserID(), m.getName(), m.getNumber(), m.getMail());
    return true;
}

bool Library::removeMember(const std::string& id) {
    if(dbManager.memberHasActiveBorrow(id)) {
        std::cerr << "Cannot remove member: they have a borrowed book" << std::endl;
        return false;
    }

    if(!dbManager.removeMember(id)) {
        std::cerr << "Member not found" << std::endl;
        return false;
    }
    return true;
}


bool Library::borrowBook(const std::string& memberID, const std::string& isbn) {
    std::optional <Member> mem = dbManager.findMemberByID(memberID);
    if(mem == std::nullopt) {
        std::cerr << "Member not found" << std::endl;
        return false;
    }

    if(dbManager.memberHasActiveBorrow(memberID)) {
        std::cerr << "Member already has a borrowed book" << std::endl;
        return false;
    }
    BorrowRecord br(memberID, isbn);
    if(!dbManager.borrowBook(br)) {
        std::optional<Book> bk = dbManager.findBookByISBN(isbn);
        if(bk == std::nullopt) {
            std::cerr << "Book not found" << std::endl;
            return false;
        }
        std::cerr << "Book is out of stock" << std::endl;
        return false;
    }
    return true;
}

bool Library::returnBook(const std::string& memberID, const std::string& isbn) {
    if(!dbManager.returnBook(memberID, isbn)) {
        std::cerr << "No active borrow found for this member and book" << std::endl;
        return false;
    }
    return true;
}


void Library::displayBooks(int limit, int offset) const {
    if(limit <= 0 || offset < 0) {
        std::cerr << "Invalid limit or offset" << std::endl;
        return;
    } 
    std::vector<Book> bk;
    dbManager.fetchBooks(bk, limit, offset);
    if(bk.empty()) {
        std::cout << "No books to display" << std::endl;
        return;
    }
    for(size_t i = 0; i < bk.size(); i++) {
        std::cout << bk[i].getTitle() << " | "
              << bk[i].getAuthor() << " | "
              << bk[i].getBookID() << " | "
              << bk[i].getCategory() << " | "
              << bk[i].getAvailableStock() << std::endl;
    }
}

void Library::displayMembers(int limit, int offset) const {
    if(limit <= 0 || offset < 0) {
        std::cerr << "Invalid limit or offset" << std::endl;
        return;
    } 
    std::vector<Member> mem;
    dbManager.fetchMembers(mem,limit,offset);
    if(mem.empty()) {
        std::cout << "No members to display" << std::endl;
        return;
    }
    for(size_t i = 0; i < mem.size(); i++) {
        std::cout << mem[i].getName() << " | "
              << mem[i].getUserID() << " | "
              << mem[i].getNumber() << " | "
              << mem[i].getMail() << std::endl;
    }
}

void Library::displayBorrowRecords(int limit, int offset) const {
     if(limit <= 0 || offset < 0) {
        std::cerr << "Invalid limit or offset" << std::endl;
        return;
    }
    std::vector<BorrowRecord> br;
    dbManager.fetchBorrowRecords(br, limit, offset);
    if(br.empty()) {
        std::cout << "No borrow records to display" << std::endl;
        return;
    }
    for (size_t i = 0; i < br.size(); i++) {
    std::optional<SysClock::time_point> returnDate = br[i].getReturnDate();

    std::cout << br[i].getUserID() << " | "
              << br[i].getBorrowedBookID() << " | "
              << timeToString(br[i].getBorrowDate()) << " | "
              << timeToString(br[i].getDueDate()) << " | "
              << (returnDate.has_value() ? timeToString(returnDate.value()) : "Not returned")
              << std::endl;
}
}