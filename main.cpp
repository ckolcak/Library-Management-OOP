#include "Library.h"
#include <iostream>
#include <fstream>
#include <sstream>

int main() {
   std::cout << "KÜTÜPHANE SİSTEMİ " << std::endl; 
   std::cout << "----------------- " << std::endl;

   int choose;std::cin >> choose;
   std::cout << std::endl;
   std::cin.ignore();

   std::string x,y,z,m;
   int a;

   Library library;


   std::ifstream s("Deneme.txt");
   std::string okunan;
   while(std::getline(s,okunan)){
       std::stringstream ss(okunan);
       std::string bookid,bookname,authorname,category,stock;
       std::getline(ss,bookid,'|');
       std::getline(ss,bookname,'|');
       std::getline(ss,authorname,'|');
       std::getline(ss,category,'|');
       std::getline(ss,stock,'|');
       Book book1(bookid,bookname,authorname,category,std::stoi(stock));
       library.addBook(book1);
   }
   
   switch(choose){
    case 1: {
       std::cout << "Kitabın adını giriniz : ";
       std::getline(std::cin,x);
       std::cout << "Kitabın idsini giriniz : ";
       std::getline(std::cin,y);
       std::cout << "Kitabın kategorisini giriniz : ";
       std::getline(std::cin,z);
       std::cout << "Kitabın yazarını giriniz : ";
       std::getline(std::cin,m);
       std::cout << "Kitabın mevvcut stogunu giriniz : ";
       std::cin >> a;
       Book book1(y,x,m,z,a);
       library.addBook(book1);
       std::ofstream dosya_acıcı;
       dosya_acıcı.open("Deneme.txt",std::ios::app);
       dosya_acıcı << book1.getBookID() << "|" << book1.getTitle() << "|" << book1.getAuthor() << "|" << book1.getCategory() << "|" << book1.getAvailableStock() << std::endl;
       dosya_acıcı.close();
       break;
    }
    case 2:{
       library.displayAllBooks();
    }
    }
}