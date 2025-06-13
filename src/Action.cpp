#include <iostream>
#include "Action.h"
extern Simulation* backup; 

BaseAction::BaseAction() :  errorMsg(""), status(ActionStatus::ERROR){}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg) {
    this->errorMsg = errorMsg;
    status = ActionStatus::ERROR;
    std::cerr << "Error: " + errorMsg << std::endl;
}

const string& BaseAction::getErrorMsg() const {
    return errorMsg;
}







SimulateStep::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(Simulation &simulation) {
    for (int i = 0; i < numOfSteps; i++)
    {
        simulation.step();
    }
    complete();
}

const string SimulateStep::toString() const {
    return "SimulateStep: " + std::to_string(numOfSteps)+" " + (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR");
}

SimulateStep* SimulateStep::clone() const {
    return new SimulateStep(*this);
}









// AddPlan
AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy)
    : settlementName(settlementName), selectionPolicy(selectionPolicy) {}

void AddPlan::act(Simulation &simulation) {
    // create selection policy
    SelectionPolicy* policy;
    if(selectionPolicy == "nve")
        policy = new NaiveSelection();
    else if(selectionPolicy == "bal")
        policy = new BalancedSelection(0,0,0);
    else if(selectionPolicy == "eco")
        policy = new EconomySelection();
    else if(selectionPolicy == "env")
        policy = new SustainabilitySelection();
    else{
        error("Cannot create this plan");
        return;
    }
    Settlement *settlement = simulation.getSettlement(settlementName);
    if(settlement == nullptr){
        error("Cannot create this plan");
        delete policy;
        return;
    }
    simulation.addPlan(settlement,policy);
    complete();

}



const string AddPlan::toString() const {
    return "plan: " + settlementName+" " + selectionPolicy +" "+ (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR");
}

AddPlan* AddPlan::clone() const {
    return new AddPlan(*this);
}






AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType)
    : settlementName(settlementName), settlementType(settlementType) {}

void AddSettlement::act(Simulation &simulation) {
    
    if(!simulation.addSettlement(new Settlement(settlementName,settlementType)))
    {
        error("Settlement already exists");
        return;
    }
    complete();

 
}



const string AddSettlement::toString() const {
    string settlementT;
        switch (settlementType)
    {
    case SettlementType::VILLAGE:
        settlementT = "0";
        break;
    
    case SettlementType::CITY:
        settlementT = "1";
        break;

    default:
        settlementT = "2";
        break;
    }
    return "settlement: " + settlementName +" "+settlementT +" "+ (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR");
}

AddSettlement* AddSettlement::clone() const {
    return new AddSettlement(*this);
}








// AddFacility
AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore)
    : facilityName(facilityName), facilityCategory(facilityCategory), price(price), 
      lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore) {}

void AddFacility::act(Simulation &simulation) {
    FacilityType facilityType = FacilityType(facilityName,facilityCategory,price,lifeQualityScore,economyScore,environmentScore);

    if(!simulation.addFacility(facilityType)){
        error("Facility already exists");
    }
    complete();

}

const string AddFacility::toString() const {
    string facilityT;
        switch (facilityCategory)
    {
    case FacilityCategory::LIFE_QUALITY:
        facilityT = "0";
        break;
    
    case FacilityCategory::ECONOMY:
        facilityT = "1";
        break;

    default:
        facilityT = "2";
        break;
    }
    return "facility: " + facilityName+" " + facilityT +" "+std::to_string(price)+" "+std::to_string(lifeQualityScore)+" "+std::to_string(economyScore)+std::to_string(environmentScore)+" "+ (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR");
}

AddFacility* AddFacility::clone() const {
    return new AddFacility(*this);
}









PrintPlanStatus::PrintPlanStatus(int planId) : planId(planId) {}

void PrintPlanStatus::act(Simulation &simulation) {
    if(simulation.getPlanCounter() <= planId || planId<0)
        {
            error("Plan doesn't exist");
            return;
        }
    std::cout << simulation.getPlan(planId).toString()<< std::endl;
    complete();

}

const string PrintPlanStatus::toString() const {
    return "planStatus: " +std::to_string(planId) + " "+ (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR");
}

PrintPlanStatus* PrintPlanStatus::clone() const {
    return new PrintPlanStatus(*this);
}








ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy)
    : planId(planId), newPolicy(newPolicy) {}

void ChangePlanPolicy::act(Simulation &simulation) {
    if(simulation.getPlanCounter()-1 <=planId || planId<0)
    {
        error("Cannot change selection policy");
        return;
    }
    SelectionPolicy* policy;
    if(newPolicy == "nve")
        policy = new NaiveSelection();
    else if(newPolicy == "bal")
        policy = new BalancedSelection(0,0,0);
    else if(newPolicy == "eco")
        policy = new EconomySelection();
    else if(newPolicy == "env")
        policy = new SustainabilitySelection();
    else{
        error("Cannot create this plan");
        return;
    }

    string prevPolicy = simulation.getPlan(planId).getSelectionPolicy()->toString();
    if(policy->toString() ==prevPolicy)
    {
        delete policy;
        error("Cannot change selection policy");
        return;
    }
    (simulation.getPlan(planId)).setSelectionPolicy(policy);
    complete();
    std::cout << "planID: "+std::to_string(planId)+"\npreviousPolicy: "+prevPolicy+"\nnewPolicy: "+ newPolicy << std::endl;
}

const string ChangePlanPolicy::toString() const {
    return "changePolicy: " +std::to_string(planId) + " "+newPolicy+" "+ (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR");
}

ChangePlanPolicy* ChangePlanPolicy::clone() const {
    return new ChangePlanPolicy(*this);
}







PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Simulation &simulation) {
    vector<BaseAction*> actionLogs = simulation.getActionLog();
    for (BaseAction* action: actionLogs)
    {
        std::cout << action->toString() << std::endl;
    }
    complete();
}

const string PrintActionsLog::toString() const {
return std::string("log: ") + (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR");
}

PrintActionsLog* PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}








Close::Close() {}

void Close::act(Simulation &simulation) {
    simulation.close();
    complete();
    //backup is deletd in main
}

const string Close::toString() const {
    return std::string("close: ") + (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR");
}

Close* Close::clone() const {
    return new Close(*this);
}







// BackupSimulation
BackupSimulation::BackupSimulation() {}

void BackupSimulation::act(Simulation &simulation) {
    delete backup;
    backup = new Simulation(simulation);
    complete();
}

const string BackupSimulation::toString() const {
    return std::string("backup: ")+ (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR");

}

BackupSimulation* BackupSimulation::clone() const {
    return new BackupSimulation(*this);
}








RestoreSimulation::RestoreSimulation() {}

void RestoreSimulation::act(Simulation &simulation) {
    if(backup == nullptr){
        error("No backup available");
        return;
    }
    simulation = *backup;
    complete();

}

const string RestoreSimulation::toString() const {
    return std::string("restore: ")+ (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR");
}

RestoreSimulation* RestoreSimulation::clone() const {
    return new RestoreSimulation(*this);
}
