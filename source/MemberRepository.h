#ifndef MEMBERREPOSITORY_H
#define MEMBERREPOSITORY_H

#include "Database.h"
#include "Member.h"
#include <optional>
#include <vector>
#include <string>

class MemberRepository {
private:
    Database& db;

public:
    explicit MemberRepository(Database& database): db(database) {}

    void addMember(const Member& member);
    [[nodiscard]]bool removeMember(const std::string& id);

    std::optional<Member> findMemberByID(const std::string& id) const;

    void fetchMembers(std::vector <Member>& members, int limit, int offset) const;
    
};

#endif