#include "Library.h"
#include <iostream>

int main() {

    Library library;

    library.displayAllBooks();
    std::cout << "\n";
    library.displayAllMembers();
    std::cout << "\n";

    Book book1("0AAA", "Türk Mitolojisi", "Kenan Karakaya", "Macera", 5);
    Book book2("1BBB", "Suç ve Ceza", "Fyodor Dostoyevski", "Roman", 3);
    Book book3("2CCC", "Beyaz Diş", "Jack London", "Macera", 4);
    Book book4("3DDD", "Dönüşüm", "Franz Kafka", "Fantastik", 2);


    Member member1("987BN", "Mehmet Ayyıldız", "05677895625", "mehmet.ayyildiz@gmail.com");
    Member member2("654MK", "Ayşe Demir", "05324567891", "ayse.demir@gmail.com");
    Member member3("321LP", "Burak Yılmaz", "05539876543", "burak.yilmaz@gmail.com");
    Member member4("852TR", "Elif Kaya", "05412345678", "elif.kaya@gmail.com");

    library.addBook(book1);
    library.addBook(book2);
    library.addBook(book3);
    library.addBook(book4);

    library.addMember(member1);
    library.addMember(member2);
    library.addMember(member3);
    library.addMember(member4);

    Member* find = library.findMemberByID("987BN");
    if(find){
        std::cout << "MemberFound" << std::endl;
    }
    else std::cout <<"MemberNotFound" << std::endl;
    std::cout << "\n";

    Book* find2 = library.findBookByID("987BN");
    if(find2){
        std::cout << "BookFound" << std::endl;
    }
    else std::cout <<"BookNotFound" << std::endl;
    std::cout << "\n";

    library.displayAllBooks();
    std::cout << "\n";
    library.displayAllMembers();
    std::cout << "\n";

    library.removeBook("0AAA");
    library.displayAllBooks();
    std::cout << "\n";

    library.borrowBook("987BN","1BBB");
    library.displayBorrowInfo();
     std::cout << "\n";

    library.returnBook("987BN","1BBB");
    library.displayBorrowInfo();
     std::cout << "\n";

    

}