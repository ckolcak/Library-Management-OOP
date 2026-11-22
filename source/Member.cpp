#include "Member.h"

Member::Member(const std::string& ID, const std::string& name, const std::string& number, const std::string& mail) 
: userID(ID), name(name), phoneNumber(number), email(mail) {}

std::string Member::getUserID() const {return userID;}
std::string Member::getName() const {return name;}
std::string Member::getNumber() const {return phoneNumber;}
std::string Member::getMail() const {return email;}

