#include "Simulation.h"
#include <iostream>
#include <typeinfo>
#include "Action.h"
#include "Auxiliary.h"
#include "Settlement.h"


using std::cout;
using std::endl;

Simulation::Simulation(const string &configFilePath)
    : isRunning(false),
      planCounter(0),                  
      actionsLog(),                    
      plans(),                          
      settlements(),                    
      facilitiesOptions()   {
            

    // Read configuration lines
    std::vector<std::string> configLines = Auxiliary::readConfigFile(configFilePath);

    // Process each line
    for (const auto &line : configLines) {
        // Parse the line into arguments
        std::vector<std::string> arguments = Auxiliary::parseArguments(line);

        if (arguments.empty()) {
            continue; // Skip empty lines
        }

        const std::string &command = arguments[0]; // The first word determines the command type
        

        if (command == "settlement") {
            std::string name = arguments[1];
            int type = std::stoi(arguments[2]);
            
            SettlementType settlementT;
                switch (type)
            {
            case 0:
                settlementT = SettlementType::VILLAGE;
                break;
            
            case 1:
                settlementT = SettlementType::CITY;
                break;

            default:
                settlementT = SettlementType::METROPOLIS;
                break;
            }


            addSettlement(new Settlement(name, settlementT));

        } 
        else if (command == "facility") {
            std::string name = arguments[1];
            int category = std::stoi(arguments[2]);
            int price = std::stoi(arguments[3]);
            int lifeq = std::stoi(arguments[4]);
            int eco = std::stoi(arguments[5]);
            int env = std::stoi(arguments[6]);


            FacilityCategory facilityC;
            switch (category)
            {
            case 0:
                facilityC = FacilityCategory::LIFE_QUALITY;
                break;
            
            case 1:
                facilityC = FacilityCategory::ECONOMY;
                break;

            default:
                facilityC = FacilityCategory::ENVIRONMENT;
                break;
            }
            addFacility(FacilityType(name, facilityC, price, lifeq, eco, env));

        } 
        else if (command == "plan") {
            std::string settlementName = arguments[1];
            std::string policyType = arguments[2];

            Settlement *settlement = getSettlement(settlementName);
            if (!settlement) {
                throw std::runtime_error("Settlement not found: " + settlementName);
            }

            SelectionPolicy* policy;
            if(policyType == "nve")
                policy = new NaiveSelection();
            else if(policyType == "bal")
                policy = new BalancedSelection(0,0,0);
            else if(policyType == "eco")
                policy = new EconomySelection();
            else if(policyType == "env")
                policy = new SustainabilitySelection();

            addPlan(settlement,policy);

        } 
        else {
            throw std::invalid_argument("Unknown configuration command: " + command);
        }
    }

}

void Simulation::start() {
    open();
    cout << "the simulation has started." << endl;
    std::string input;

    while (isRunning) {
        std::cout << "> "; // Command prompt
        std::getline(std::cin, input);

        std::vector<std::string> arguments = Auxiliary::parseArguments(input);
        if (arguments.empty()) {
            continue; // ignore empty inputa
        }

        const std::string &command = arguments[0];
        if (command == "step") {
            if (arguments.size() < 2) {
                std::cout <<("Usage: simulate_steps NUM")<< endl;
                
            }
            else{
                int numSteps = std::stoi(arguments[1]);
                SimulateStep* action =  new SimulateStep(numSteps);
                action->act(*this);
                addAction(action);
            }
        } 
        
        else if (command == "plan") {
            if (arguments.size() < 3) {
                std::cout <<("Usage: add_plan NAME POLICY")<< endl;
            }
            else{
                std::string settlementName = arguments[1];
                std::string policy = arguments[2];
                auto action = new AddPlan(settlementName, policy);
                action->act(*this);
                addAction(action);
            }
        } 
        
        else if (command == "settlement") {
            if (arguments.size() < 3) {
                std::cout <<("Usage: add_settlement NAME TYPE")<< endl;
            }
            else{
                std::string name = arguments[1];
                int type = std::stoi(arguments[2]);
                SettlementType settlementType = static_cast<SettlementType>(type);
                AddSettlement* action = new AddSettlement(name, settlementType);
                action->act(*this);
                addAction(action);
            }
        } 
        
        else if (command == "facility") {
            if (arguments.size() < 7) {
                std::cout <<("Usage: add_facility NAME CATEGORY PRICE LIFEQ ECO ENV")<< endl;
            }
            else{
                std::string name = arguments[1];
                FacilityCategory category = static_cast<FacilityCategory>(std::stoi(arguments[2]));
                int price = std::stoi(arguments[3]);
                int lifeQualityScore = std::stoi(arguments[4]);
                int economyScore = std::stoi(arguments[5]);
                int environmentScore = std::stoi(arguments[6]);
                AddFacility* action = new AddFacility(name, category, price, lifeQualityScore, economyScore, environmentScore);
                action->act(*this);
                addAction(action);
            }
        }
        
        else if (command == "planStatus") {
            if (arguments.size() < 2) {
                std::cout <<("Usage: print_plan_status ID")<< endl;
            }
            else{
                int planId = std::stoi(arguments[1]);
                PrintPlanStatus* action = new PrintPlanStatus(planId);
                action->act(*this);
                addAction(action);
            }

        } 
        
        else if (command == "changePolicy") {//check whats the actual command for changing policy
            if (arguments.size() < 3) {
                std::cout <<("Usage: change_plan_policy ID POLICY")<< endl;
            }
            else{
                int planId = std::stoi(arguments[1]);
                std::string newPolicy = arguments[2];
                auto action = new ChangePlanPolicy(planId, newPolicy);
                action->act(*this);
                addAction(action);
            }
        } 
        
        else if (command == "log") {
            PrintActionsLog* action = new PrintActionsLog();
            action->act(*this);
            addAction(action);

        } 
        
        else if (command == "backup") {
            BackupSimulation* action = new BackupSimulation();
            action->act(*this);
            addAction(action);
        } 
        
        else if (command == "restore") {
            RestoreSimulation* action = new RestoreSimulation();
            action->act(*this);
            addAction(action);
        } 
        
        else if (command == "close") {
            Close action = Close();
            action.act(*this);
        } 
        
        else {
            std::cout <<("Unknown command: " + command) << endl;
        } 
    }
    
}

void Simulation::addPlan(const Settlement *settlement, SelectionPolicy *selectionPolicy) {
    Plan newPlan(planCounter, *settlement->clone(), selectionPolicy, facilitiesOptions);
    plans.push_back(newPlan);
    planCounter++;
}



void Simulation::addAction(BaseAction *action) {
    actionsLog.push_back(action);
    
}
vector<BaseAction*> Simulation::getActionLog(){
    return actionsLog;
}



bool Simulation::addSettlement(Settlement *settlement) {
    if (isSettlementExists(settlement->getName())) {
        delete settlement;
        return false;
    }
    settlements.push_back(settlement);
    return true;
}

bool Simulation::addFacility(FacilityType facility) {
    bool isFacilityExists = false;
    for (FacilityType tempFacilityType : facilitiesOptions) {
        if (tempFacilityType.getName() == facility.getName())
        {
            isFacilityExists = true;
            break;
        }
    }
    if (isFacilityExists)
    {
        return false;
    }
    
    facilitiesOptions.push_back(facility);
    return true;
}

bool Simulation::isSettlementExists(const string &settlementName) {
    bool isSettlementExists = false;
    for (Settlement* tempSettlement : settlements) {
        if (settlementName == tempSettlement->getName())
        {
            isSettlementExists = true;
            break;
        }
    }
    return isSettlementExists;
}

Settlement* Simulation::getSettlement(const string &settlementName) {
    for (auto settlement : settlements) {
        if (settlement->getName() == settlementName) {
            return settlement;
        }
    }
    return nullptr;
}

Plan& Simulation::getPlan(const int planID) {
    return plans.at(planID);
}
int Simulation::getPlanCounter(){
    return planCounter;
}


void Simulation::step() {
    for (auto &plan : plans) {
        plan.step();
    }
}

void Simulation::close() {
    isRunning = false;
    string output = "";
    for (auto plan : plans) {
        output += plan.printClose()+"\n";
    }
    cout << output<< endl;
    //the program will be ended and the distructor will be automaticly called
}

void Simulation::open() {
    isRunning = true;
}

Simulation::~Simulation() {
    for (auto action : actionsLog) {
        delete action;
    }
    actionsLog.clear();
    for (auto settlement : settlements) {
        delete settlement;
    }
    settlements.clear();
    // for (auto plan : plans) {
    //    plan.~Plan();
    // }
    plans.clear();
    facilitiesOptions.clear();
}



Simulation::Simulation(const Simulation &other)
    : isRunning(other.isRunning),
      planCounter(other.planCounter),
      actionsLog(),
      plans() ,
      settlements(),
      facilitiesOptions(other.facilitiesOptions)
     { 
    for (BaseAction* action : other.actionsLog) {
        actionsLog.push_back(action->clone()); 
    }
    for (auto settlement : other.settlements) {
        settlements.push_back(settlement->clone());
    }

    for (auto plan : other.plans) {
        plans.push_back(Plan(plan));//will deep copy other.plan: create another settlement clone and will handle it in plan
    }
}

Simulation &Simulation::operator=(const Simulation &other) {
    if (this != &other) {
        // Create temporary copies of resources
        std::vector<BaseAction*> newActionsLog;
        for (auto action : other.actionsLog) {
            newActionsLog.push_back(action->clone());
        }

        std::vector<Settlement*> newSettlements;
        for (auto settlement : other.settlements) {
            newSettlements.push_back(settlement->clone());
        }

        // Copy simple members
        bool newIsRunning = other.isRunning;
        int newPlanCounter = other.planCounter;
        std::vector<FacilityType> newFacilitiesOptions = other.facilitiesOptions;
        std::vector<Plan> newPlans = other.plans;
        // Clean up existing resources
        for (auto action : actionsLog) {
            delete action;
        }
        for (auto settlement : settlements) {
            delete settlement;
        }

        actionsLog.clear();
        settlements.clear();

        // Assign new resources
        actionsLog = std::move(newActionsLog);
        settlements = std::move(newSettlements);
        isRunning = newIsRunning;
        planCounter = newPlanCounter;
        facilitiesOptions = std::move(newFacilitiesOptions);
        plans = std::move(newPlans);
    }
    return *this;
}


Simulation::Simulation(Simulation &&other) noexcept
    : isRunning(other.isRunning),
      planCounter(other.planCounter),
      actionsLog(std::move(other.actionsLog)),
      plans(std::move(other.plans)),
      settlements(std::move(other.settlements)),
      facilitiesOptions(std::move(other.facilitiesOptions)) {
    other.isRunning = false;
}

Simulation &Simulation::operator=(Simulation &&other) noexcept {
    if (this != &other) {
        // Clean up current resources
        for (auto action : actionsLog) {
            delete action;
        }
        actionsLog.clear();

        for (auto settlement : settlements) {
            delete settlement;
        }
        settlements.clear();

        // Transfer ownership of resources
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        actionsLog = std::move(other.actionsLog);
        plans = std::move(other.plans);
        settlements = std::move(other.settlements);
        facilitiesOptions = std::move(other.facilitiesOptions);

        other.isRunning = false;
    }
    return *this;
}