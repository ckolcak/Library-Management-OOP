#ifndef BOOKREPOSITORY_H
#define BOOKREPOSITORY_H

#include "Database.h"
#include "Book.h"
#include <optional>
#include <vector>
#include <string>

class BookRepository{
private:
    Database& db;//Referans olarak kulandıgımız için bookrepository kendi baglantısını oluşturmaz onun yerine db nin bağlantısını kullanır.

public:
    explicit BookRepository(Database& database): db(database) {} 

    void addBook(const Book& book);
    [[nodiscard]]bool removeBook(const std::string& isbn);

    [[nodiscard]]bool increaseBookStock(const std::string& isbn, int amount);
    [[nodiscard]]bool decreaseStockIfAvailable(const std::string& isbn);

    std::optional<Book> findBookByISBN(const std::string& isbn) const;

    void fetchBooks(std::vector<Book>& books, int limit, int offset) const;

};

#endif