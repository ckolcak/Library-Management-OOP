#include "BookApiClient.h"
#include <iostream>
#include <cstdlib>

size_t BookApiClient::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp){
    size_t fullSize = size * nmemb;
    userp->append((char*)contents,fullSize);
    return fullSize;
}

std::string BookApiClient::fetchBookByISBN(const std::string& ISBN){
    const char* key = std::getenv("GOOGLE_BOOKS_API_KEY"); //environment variableyi getenv ile çekip const char* türüne eşitledim. getnev c fonksiyonu ondan char* dondurur.
    if(!key){
        std::cout << "Environment variable not find!" << std::endl;
        return "";
    }
    std::string apiKey = key;
    std::string URL = "https://www.googleapis.com/books/v1/volumes?q=isbn:" + ISBN + "&key=" + apiKey;

    CURL* curl = curl_easy_init();
    if(!curl) {
        std::cout << "Failed to initialize CURL." << std::endl;
        return "";
    }
    CURLcode result;
    std::string readBuffer;

    curl_easy_setopt(curl,CURLOPT_URL, URL.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,WriteCallback);
    curl_easy_setopt(curl,CURLOPT_WRITEDATA,&readBuffer);

    result = curl_easy_perform(curl);

    if(result != CURLE_OK){
        std::cout << curl_easy_strerror(result) << std::endl;
        curl_easy_cleanup(curl);
        return "";
    }

    curl_easy_cleanup(curl);

    return readBuffer;

}

std::optional<Book> BookApiClient::parseBook(const std::string& readBuffer, const std::string& ISBN, int stock) {
    if(readBuffer.empty()) {
        return std::nullopt;
    }
    nlohmann::json j = nlohmann::json::parse(readBuffer);
    if(!j.contains("items") || j["items"].empty()) {
        return std::nullopt;
    }
    if(!(j["items"][0]["volumeInfo"].contains("title")) || 
       j["items"][0]["volumeInfo"]["title"].empty() ||
       !(j["items"][0]["volumeInfo"].contains("authors")) || 
       j["items"][0]["volumeInfo"]["authors"].empty()) {
        return std::nullopt;
    }
    std::string title = j["items"][0]["volumeInfo"]["title"];
    std::string author = j["items"][0]["volumeInfo"]["authors"][0];
    std::string category = "Unknown";
    if(j["items"][0]["volumeInfo"].contains("categories") && !(j["items"][0]["volumeInfo"]["categories"].empty())) {
        category = j["items"][0]["volumeInfo"]["categories"][0];
    }

    Book book(ISBN, title, author, category, stock);
    return book;
    
}