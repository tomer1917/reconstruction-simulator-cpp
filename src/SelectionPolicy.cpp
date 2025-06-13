#include "SelectionPolicy.h"

// NaiveSelection
NaiveSelection::NaiveSelection()
    : lastSelectedIndex(-1) {}

const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    lastSelectedIndex++;

    if (static_cast<int>(facilitiesOptions.size()) >= lastSelectedIndex)
        lastSelectedIndex = 0;

    return facilitiesOptions.at(lastSelectedIndex);
}

const string NaiveSelection::toString() const {
    return "SelectionPolicy: nve";
}


//ToDo
NaiveSelection* NaiveSelection::clone() const {
     NaiveSelection* navClone = new NaiveSelection();
     return navClone;
}







// BalancedSelection




BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore)
    : LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore) {}

int BalancedSelection::balRate(FacilityType facility){
    int lifeQuality = LifeQualityScore + facility.getLifeQualityScore();
    int economy = EconomyScore + facility.getEconomyScore();
    int environment = EnvironmentScore + facility.getEnvironmentScore();
    int min = std::min(lifeQuality, std::min(economy, environment));
    int max = std::max(lifeQuality, std::max(economy, environment));
    return max - min;
}


const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {

    int min = balRate(facilitiesOptions.at(0));


    int resultFacilityIndex = 0;
    for (int i = 1; i < static_cast<int>(facilitiesOptions.size()); i++) {
        if (balRate(facilitiesOptions.at(i)) < min) {
            min = balRate(facilitiesOptions.at(i));
            resultFacilityIndex = i;
        }
    }

    LifeQualityScore += facilitiesOptions.at(resultFacilityIndex).getLifeQualityScore();
    EconomyScore += facilitiesOptions.at(resultFacilityIndex).getEconomyScore();
    EnvironmentScore += facilitiesOptions.at(resultFacilityIndex).getEnvironmentScore();

    return facilitiesOptions.at(resultFacilityIndex);
}

const string BalancedSelection::toString() const {
    return "SelectionPolicy: bal";
}

BalancedSelection* BalancedSelection::clone() const {
    BalancedSelection* balClone = new BalancedSelection(LifeQualityScore, EconomyScore, EnvironmentScore);
    return balClone;
}




// EconomySelection
EconomySelection::EconomySelection()
    : lastSelectedIndex(0) {}

const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    //check if options are not out of bond
    lastSelectedIndex++;
    if(lastSelectedIndex == static_cast<int>(facilitiesOptions.size()))
        lastSelectedIndex = 0;
    


    for (int i = lastSelectedIndex; i < static_cast<int>(facilitiesOptions.size()); i++)
    {
        if(facilitiesOptions.at(i).getCategory() == FacilityCategory::ECONOMY)
        {
            lastSelectedIndex = i;
            return facilitiesOptions.at(i);
        }
    }
    return facilitiesOptions.at(lastSelectedIndex);
    
}

const string EconomySelection::toString() const {
    return "SelectionPolicy: eco";
}


EconomySelection* EconomySelection::clone() const {
    EconomySelection* ecoClone = new EconomySelection();
    return ecoClone;
}




// SustainabilitySelection
SustainabilitySelection::SustainabilitySelection()
    : lastSelectedIndex(0) {}

const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    //check if options are not out of bond
    lastSelectedIndex++;
     if (lastSelectedIndex == static_cast<int>(facilitiesOptions.size()))
        lastSelectedIndex = 0;
    


    for (int i = lastSelectedIndex; i < static_cast<int>(facilitiesOptions.size()); i++)
    {
        if(facilitiesOptions.at(i).getCategory() == FacilityCategory::ENVIRONMENT)
        {
            lastSelectedIndex = i;
            return facilitiesOptions.at(i);
        }
    }
    
    return facilitiesOptions.at(lastSelectedIndex);
    
    }

const string SustainabilitySelection::toString() const {
    return "SelectionPolicy: env";
}

SustainabilitySelection* SustainabilitySelection::clone() const {
    SustainabilitySelection* susClone = new SustainabilitySelection();
    return susClone;
}
