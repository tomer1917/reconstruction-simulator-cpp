#include "Plan.h"
#include <iostream>
using std::cout;
using std::endl;

// Constructor
Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId),
      settlement(&settlement),
      selectionPolicy(selectionPolicy),
      status(PlanStatus::AVALIABLE),
      facilities(), 
      underConstruction(),
      facilityOptions(facilityOptions),
      life_quality_score(0),
      economy_score(0),
      environment_score(0) {
}

const int Plan::getlifeQualityScore() const {
    return life_quality_score;
}

const int Plan::getEconomyScore() const {
    return economy_score;
}

const int Plan::getEnvironmentScore() const {
    return environment_score;
}


void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy) {
    delete this->selectionPolicy; // delete the old selection pointer
    this->selectionPolicy = selectionPolicy;
}
 SelectionPolicy *Plan::getSelectionPolicy(){
    return selectionPolicy;
 }


void Plan::step() {
    int constructionLimit = settlement->getConstructionLimit();
    if (status == PlanStatus::AVALIABLE) {
        for (int i = 0; i < constructionLimit && !facilityOptions.empty(); ++i) {
            Facility* selectedFacility = new Facility(selectionPolicy->selectFacility(facilityOptions),settlement->getName());
            underConstruction.push_back(selectedFacility);
        }
    }

    //remove facilities that are no longer underconstruction
    for (auto it = underConstruction.begin(); it != underConstruction.end();) {
        if((*it)->step() == FacilityStatus::OPERATIONAL){
            addFacility(*it);
            underConstruction.erase(it);
        } else {
            ++it;
        }
    }
    
    status = (underConstruction.size() == static_cast<std::size_t>(constructionLimit)) 
         ? PlanStatus::BUSY 
         : PlanStatus::AVALIABLE;
}

void Plan::printStatus() {
    cout << "planStatus: " << (status == PlanStatus::AVALIABLE ? "Available" : "Busy") << endl;
}

const vector<Facility*> &Plan::getFacilities() const {
    return facilities;
}

void Plan::addFacility(Facility *facility) {
        //add selectedFacility's scores
        environment_score = facility->getEnvironmentScore();
        economy_score = facility->getEconomyScore();
        life_quality_score = facility->getLifeQualityScore();
        facilities.push_back(facility);
}

const string Plan::toString() const {
    string output = "";

    for(Facility* facility: underConstruction){
        output +="\n"+facility->toString();
    }
    for(Facility* facility: facilities){
        output +="\n"+facility->toString();
    }
    return "PlanID: " + std::to_string(plan_id)
    +"\n"+ settlement->toString() 
    +"\n"+ "planStatus: " +(status == PlanStatus::AVALIABLE ? "AVALIABLE" : "BUSY")
    +"\n"+ selectionPolicy->toString()
    +"\n"+ "LifeQualityScore: " + std::to_string(life_quality_score)
    +"\n"+ "EconomyScore: " + std::to_string(economy_score)
    +"\n"+ "EnvironmentScore: " + std::to_string(environment_score)
    + output;
}

const string Plan::printClose() const {
    return "PlanID: " + std::to_string(plan_id)
    +"\n"+ settlement->toString() 
    +"\n"+ "LifeQualityScore: " + std::to_string(life_quality_score)
    +"\n"+ "EconomyScore: " + std::to_string(economy_score)
    +"\n"+ "EnvironmentScore: " + std::to_string(environment_score);
}


Plan::Plan(const Plan& other)
    : plan_id(other.plan_id),
      settlement(other.settlement->clone()),
      selectionPolicy(other.selectionPolicy ? other.selectionPolicy->clone() : nullptr),
      status(other.status),
      facilities(),
      underConstruction(),
      facilityOptions(other.facilityOptions),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score) {

    // Deep copy facilities and underConstruction
    for (const auto& facility : other.facilities) {
        facilities.push_back(facility->clone());
    }
    for (const auto& facility : other.underConstruction) {
        underConstruction.push_back(facility->clone());
    }
}

Plan& Plan::operator=(const Plan& other) {
        if (this != &other) {
                for (auto& facility : facilities) {
                    delete facility;
                }
        
        facilities.clear();

        for (auto& facility : underConstruction) {
            delete facility;
        }
        underConstruction.clear();


        delete selectionPolicy;
        selectionPolicy = nullptr;
        plan_id = other.plan_id;
        delete settlement;
        settlement = nullptr;
        settlement = other.settlement->clone();
        selectionPolicy = other.selectionPolicy ? other.selectionPolicy->clone() : nullptr;
        status = other.status;
        life_quality_score = other.life_quality_score;
        economy_score = other.economy_score;
        environment_score = other.environment_score;

        // Deep copy facilities and underConstruction
        for (const auto& facility : other.facilities) {
            facilities.push_back(facility->clone());
        }
        for (const auto& facility : other.underConstruction) {
            underConstruction.push_back(facility->clone());
        }
    }
    return *this;
}

Plan::~Plan() {
    for (auto& facility : facilities) {
        delete facility;
    }
    facilities.clear();

    for (auto& facility : underConstruction) {
        delete facility;
    }
    underConstruction.clear();

    delete selectionPolicy;
    delete settlement;
    selectionPolicy = nullptr;
    settlement = nullptr;

}