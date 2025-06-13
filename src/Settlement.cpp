#include <string>
#include "Settlement.h"

using namespace std;

Settlement::Settlement(const string& name, SettlementType type)
    : name(name), type(type) {}

const string& Settlement::getName() const{
    return name;
}

SettlementType Settlement::getType() const {
    return type;
}

int Settlement::getConstructionLimit() const {
    switch (type)
    {
    case SettlementType::VILLAGE:
        return 1;
        break;
    
    case SettlementType::CITY:
        return 2;
        break;

    default:
        return 3;
        break;
    }
}

Settlement* Settlement::clone() const{
    return new Settlement(name,type);
}
const string Settlement::toString() const {
    string value = "SettlementName: " + name;
    return value;
}