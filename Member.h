#ifndef MEMBER_H
#define MEMBER_H

#include <string>

class Member{
private:
    std::string userID;
    std::string name;
    std::string phoneNumber;
    std::string email;

public:
    Member(const std::string& ID, const std::string& name, const std::string& number, const std::string& mail);

    std::string getUserID() const;
    std::string getName() const;
    std::string getNumber() const;
    std::string getMail() const;

    void setNumber(const std::string& number);
    void setMail(const std::string& mail);

    bool operator==(const Member& other) const;

};

#endif