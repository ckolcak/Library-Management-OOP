#include "MemberRepository.h"
#include <stdexcept>

void MemberRepository::addMember(const Member& member) {
    const char* insert2Members = 
    "INSERT INTO Members (id, name, number, mail)"
    "VALUES (?, ?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db.handle(), insert2Members, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db.handle());
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }

    std::string id = member.getUserID();
    std::string name = member.getName();
    std::string number = member.getNumber();
    std::string mail = member.getMail();
    
    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, number.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, mail.c_str(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);

    if(result != SQLITE_DONE) {
        std::string errorMsg = sqlite3_errmsg(db.handle());
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute the prepared statement: " + errorMsg);
    }

    sqlite3_finalize(stmt);
}

bool MemberRepository::removeMember(const std::string& id) {
    const char* removeMember = "DELETE FROM Members WHERE id = ?;";

    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db.handle(), removeMember, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db.handle());
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }

    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);
    if(result != SQLITE_DONE) {
        std::string errorMsg = sqlite3_errmsg(db.handle());
        sqlite3_finalize(stmt); 
        throw std::runtime_error("Failed to execute the prepared statement: " + errorMsg);
    }

    sqlite3_finalize(stmt); 

    if(!sqlite3_changes(db.handle())) //memberID yok demek false donunce
        return false;

    return true;
}


std::optional<Member> MemberRepository::findMemberByID(const std::string& id) const {
    const char* findMember = "SELECT name, number, mail FROM Members WHERE id = ?;";

    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db.handle(), findMember, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db.handle());
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }

    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);
    if(result == SQLITE_ROW) {
        std::string name = reinterpret_cast<const char*> (sqlite3_column_text(stmt,0));
        std::string number = reinterpret_cast<const char*> (sqlite3_column_text(stmt,1));
        std::string mail = reinterpret_cast<const char*> (sqlite3_column_text(stmt,2));
        sqlite3_finalize(stmt);
        Member member(id, name, number, mail);
        return member;
    }

    if(result == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return std::nullopt;
    }

    std::string errorMsg = sqlite3_errmsg(db.handle());
    sqlite3_finalize(stmt);
    throw std::runtime_error("Failed to execute the prepared statement: " + errorMsg);
}


void MemberRepository::fetchMembers(std::vector <Member>& members, int limit, int offset) const {
     members.clear();

    const char* showMembers = 
    "SELECT id, name, number, mail "
    "FROM Members "
    "ORDER BY name COLLATE NOCASE, id "
    "LIMIT ? OFFSET ?;";

    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db.handle(), showMembers, -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::string errorMsg = sqlite3_errmsg(db.handle());
        throw std::runtime_error("Failed to prepare the statement: " + errorMsg);
    }

    sqlite3_bind_int(stmt, 1, limit);
    sqlite3_bind_int(stmt, 2, offset);
    
    while((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        std::string id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string number = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string mail = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

        Member member(id, name, number, mail);
        members.push_back(member);
    }

    if(result != SQLITE_DONE) {
        members.clear();
        std::string errorMsg = sqlite3_errmsg(db.handle());
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to read members: " + errorMsg);
    }
    sqlite3_finalize(stmt);
}