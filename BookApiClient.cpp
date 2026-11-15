#include "BookApiClient.h"
#include <iostream>
#include <cstdlib>//std::getenv
#include <thread>//std::this_thread::sleep_for
#include <chrono>
#include <stdexcept>

using json = nlohmann::json;

size_t BookApiClient::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp){
    size_t fullSize = size * nmemb;
    userp->append((char*)contents,fullSize);
    return fullSize;
}

std::string BookApiClient::fetchBookByISBN(const std::string& ISBN){
    const char* key = std::getenv("GOOGLE_BOOKS_API_KEY");
    if(!key)
        throw std::runtime_error("Environment variable not found!");
    
    std::string apiKey = key;
    std::string URL = "https://www.googleapis.com/books/v1/volumes?q=isbn:" + ISBN + "&key=" + apiKey;

    CURL* curl = curl_easy_init();
    if(!curl)
        throw std::runtime_error("Failed to initialize CURL.");
    
    CURLcode result;
    std::string readBuffer;

    curl_easy_setopt(curl,CURLOPT_URL, URL.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,WriteCallback);
    curl_easy_setopt(curl,CURLOPT_WRITEDATA,&readBuffer);

    int cntr = 0;

    while(cntr < 3) {
        result = curl_easy_perform(curl);// WriteCallback gelen veriyi buffer'ın sonuna ekliyor, bu yüzden her tekrar denemeden önce buffer temizleniyor.                             
        if(result != CURLE_OK) {
            std::string error = curl_easy_strerror(result);
            curl_easy_cleanup(curl);
            throw std::runtime_error("Failed to perform HTTP request: " + error);
        }

        long statusCode = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
        
        switch(statusCode) {
            case 200: 
                curl_easy_cleanup(curl);
                return readBuffer;
            case 400:
                curl_easy_cleanup(curl);
                throw std::runtime_error("Bad request: The request format is invalid");
            case 404:
                curl_easy_cleanup(curl);
                throw std::runtime_error("Not found: No book was found for the given ISBN");
            case 429: 
            case 500: 
            case 503:
                std::this_thread::sleep_for(std::chrono::seconds(1));
                readBuffer.clear();
                break;
            default:
                curl_easy_cleanup(curl);
                throw std::runtime_error("Unexpected HTTP status code: " + std::to_string(statusCode));
        }
        cntr++;
    }
    curl_easy_cleanup(curl);
    throw std::runtime_error("Request failed after 3 attempts");
}

std::optional<Book> BookApiClient::parseBook(const std::string& readBuffer, const std::string& ISBN, int stock) {
    std::string title;
    std::string author = "Unknown";
    std::string category = "Uncategorized";

    try {
        json j = json::parse(readBuffer);

        if(!j.contains("items") || j["items"].empty())
            return std::nullopt;

        if(!(j["items"][0].contains("volumeInfo")))
            return std::nullopt;

        if(!(j["items"][0]["volumeInfo"].contains("title")))
            return std::nullopt;
        
        title = j["items"][0]["volumeInfo"]["title"];
        if(title.empty())
            return std::nullopt;

        if((j["items"][0]["volumeInfo"].contains("authors")) && !(j["items"][0]["volumeInfo"]["authors"].empty())) {
            author = j["items"][0]["volumeInfo"]["authors"][0];
        }
        if(j["items"][0]["volumeInfo"].contains("categories") && !(j["items"][0]["volumeInfo"]["categories"].empty())) {
            category = j["items"][0]["volumeInfo"]["categories"][0];
        }
    }
    catch (const json::exception& e) {
        std::string errorMsg = e.what();
        throw std::runtime_error("JSON error: " + errorMsg);
    }
    Book book(ISBN, title, author, category, stock);
    return book;
}