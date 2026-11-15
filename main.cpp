#include "Library.h"
#include <iostream>
#include <stdexcept>

void space() {
    std::cout << "\n\n";
}

int main() {
    try {
        Library library;

        library.addBookByISBN("9781853261589", 4);
        library.addBookByISBN("9781853261589", 8);   
        library.addBookByISBN("9780141915685", 5);
        library.addBookByISBN("9786057406071", 3);
        library.addBookByISBN("9780141915685", -2);  
        library.addBookByISBN("0000000000000", 1);   
        library.displayBooks(10, 0);
        space();

        library.addMember(Member("00ADC43", "Civan Kolcak", "05678902345", "civan@gmail.com"));
        library.addMember(Member("00ADC44", "Test User", "05551112233", "test@example.com"));
        library.addMember(Member("00ADC43", "Duplicate", "000", "dup@example.com")); 
        library.displayMembers(10, 0);
        space();

        library.borrowBook("00ADC43", "9781853261589");  
        library.borrowBook("00ADC43", "9780141915685");  
        library.borrowBook("NOPE", "9781853261589");     
        library.borrowBook("00ADC44", "0000000000000");  
        library.displayBorrowRecords(10, 0);
        space();
        library.displayBooks(10, 0);
        space();                 

        library.removeMember("00ADC43");                 
        library.removeBook("9781853261589");             

        library.returnBook("00ADC43", "9781853261589");  
        library.returnBook("00ADC43", "9781853261589");  
        library.displayBorrowRecords(10, 0);  
        space();           
        library.displayBooks(10, 0); 
        space();                    

        library.displayBooks(2, 0); 
        space();                    
        library.displayBooks(2, 2); 
        space();                
        library.displayBooks(0, 0); 
        space();                     

        library.removeMember("00ADC44");                
        library.removeMember("00ADC44");                
        library.removeMember("00ADC43");                 
        library.removeBook("9786057406071");             
        library.displayMembers(10, 0);
        space();
        library.displayBooks(10, 0);
        space();
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}

//çalıştırma komutu:
/*
g++ -std=c++17 -Wall -Wextra main.cpp Book.cpp Member.cpp BorrowRecord.cpp TimeUtils.cpp DatabaseManager.cpp BookApiClient.cpp Library.cpp -I/opt/homebrew/include -I/opt/homebrew/opt/sqlite/include -L/opt/homebrew/opt/sqlite/lib -lcurl -lsqlite3 -o app
rm -f main.db && ./app
*/

