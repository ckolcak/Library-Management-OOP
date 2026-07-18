#include "Library.h"
#include <iostream>

int main() {
    Library nesne1;
    nesne1.addBook(Book(1, "KING CIVAN", "Nehir Balyedi" , "Destan", 50));
    nesne1.addBook(Book(2, "QUEEN NEHIR", "Civan Kolçak", "Destan" , 100));

    nesne1.addMember(Member("05AYH", "Max Versteppen" ,"05050904447", "civan.kolcak@gmail.com"));

    Member nesne2("056ER", "Joni Reis" ,"05671524525", "joni.reis@gmail.com" );
    nesne2.print();
    nesne1.addMember(nesne2);
}