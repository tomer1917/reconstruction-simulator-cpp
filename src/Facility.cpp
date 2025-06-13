#include "Facility.h"

// FacilityType Constructor
FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
    : name(name), category(category), price(price), lifeQuality_score(lifeQuality_score), economy_score(economy_score), environment_score(environment_score) {}

// FacilityType Getters
const string &FacilityType::getName() const { return name;}
int FacilityType::getCost() const { return price;}
int FacilityType::getLifeQualityScore() const {return lifeQuality_score; }
int FacilityType::getEnvironmentScore() const { return environment_score;}
int FacilityType::getEconomyScore() const {return economy_score;}
FacilityCategory FacilityType::getCategory() const {return category;}







//Facility constructors
Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
    : FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score), settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(price) { }



Facility::Facility(const FacilityType &type, const string &settlementName)
    : FacilityType(type), settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(price) { }



// Facility Getters and Set
const string &Facility::getSettlementName() const {return settlementName; }
const int Facility::getTimeLeft() const {return timeLeft; }
const FacilityStatus& Facility::getStatus() const { return status; }

void Facility::setStatus(FacilityStatus status) { this->status = status;}

FacilityStatus Facility::step() { 
    timeLeft--;
    if (timeLeft == 0 )
        status = FacilityStatus::OPERATIONAL;
    return status;
}

Facility*  Facility::clone() const { 
    return new Facility(getName(), getSettlementName(), getCategory()
    ,getCost(), getLifeQualityScore(), getEconomyScore(),getEconomyScore());
}


const string Facility::toString() const { 
    string value = "FacilityName: " + name + "\n" + "FacilityStatus: ";
    switch (status)
    {
    case FacilityStatus::UNDER_CONSTRUCTIONS:
        value += "UNDER_CONSTRUCTION";
        break;
    
    default:
            value += "OPERATIONAL";
        break;
    }
    return value;
}
