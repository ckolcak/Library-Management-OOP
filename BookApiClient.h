#ifndef BOOKAPICLIENT_H
#define BOOKAPICLIENT_H

#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <optional>
#include "Book.h"


class BookApiClient {
private:
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);

public:
    std::string fetchBookByISBN(const std::string& ISBN);
    std::optional<Book> parseBook(const std::string& readBuffer, const std::string& ISBN, int stock);
    
};

#endif
