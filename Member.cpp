#include <iostream>
#include "Member.h"

Member::Member(const std::string& ID, const std::string& name_, const std::string& nmbr, const std::string& mail) 
: userID(ID), name(name_), phoneNumber(nmbr), email(mail) {}

std::string Member::getUserID() const {return userID;}
std::string Member::getName() const {return name;}
std::string Member::getNumber() const {return phoneNumber;}
std::string Member::getMail() const {return email;}

void Member::setNumber(const std::string& nmbr) {phoneNumber = nmbr;}
void Member::setMail(const std::string& mail) {email = mail;}

void Member::print() const{
    std::cout << "User ID: " << userID << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Number: " << phoneNumber << std::endl;
    std::cout << "EMail: " << email << std::endl;
    std::cout << "------------------------" << std::endl;
}