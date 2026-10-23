#ifndef MEMBER_H
#define MEMBER_H

#include <string>
using namespace std;

class Member {
private:
    int id;
    string name;
    string email;
    string phone;

public:
    Member();
    Member(int id, const string& name, const string& email, const string& phone);

    int getId() const;
    const string& getName() const;
    const string& getEmail() const;
    const string& getPhone() const;

    void setName(const string& value);
    void setEmail(const string& value);
    void setPhone(const string& value);
};

#endif
