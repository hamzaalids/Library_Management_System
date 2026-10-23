#include "Member.h"

Member::Member() : id(0) {}
Member::Member(int idValue, const string& nameValue, const string& emailValue,
               const string& phoneValue)
    : id(idValue), name(nameValue), email(emailValue), phone(phoneValue) {}

int Member::getId() const { return id; }
const string& Member::getName() const { return name; }
const string& Member::getEmail() const { return email; }
const string& Member::getPhone() const { return phone; }

void Member::setName(const string& value) { name = value; }
void Member::setEmail(const string& value) { email = value; }
void Member::setPhone(const string& value) { phone = value; }
