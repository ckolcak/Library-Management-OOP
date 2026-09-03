#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t totalSize = size * nmemb;
    userp->append((char*)contents, totalSize);
    return totalSize;
}

int main(){

    CURL* curl;
    CURLcode result;

    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();

    if(!curl) {
        std::cout << "HTTPS request failed!" << std::endl;
        curl_global_cleanup();
        return -1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, "https://jsonplaceholder.typicode.com/posts/1");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    result = curl_easy_perform(curl);

    if(result != CURLE_OK){
        std::cout << curl_easy_strerror(result) << std::endl;
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return -1;
    }

    curl_easy_cleanup(curl);

    curl_global_cleanup();

    std::cout << readBuffer << std::endl;

    nlohmann::json j = nlohmann::json::parse(readBuffer);

    std::cout << j["userId"] << std::endl;
    std::cout << j["title"] << std::endl;
}