#ifndef MEMBER_H
#define MEMBER_H

#include <string>
#include <vector>

class Member{
private:
    const std::string userID;
    std::string name;
    std::string email;
    std::string phoneNumber;

public:
    Member(const std::string& ID, const std::string& name_, const std::string& number, const std::string& mail);

    std::string getUserID() const;
    std::string getName() const;
    std::string getNumber() const;
    std::string getMail() const;

    void setNumber(const std::string& number);
    void setMail(const std::string& mail);

    void print() const;
};

#endif