#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        Simulation(const string &configFilePath);
        void start();
        void addPlan(const Settlement *settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);
        Settlement *getSettlement(const string &settlementName);
        Plan &getPlan(const int planID);
        int getPlanCounter();
        vector<BaseAction*> getActionLog();
        void step();
        void close();
        void open();
        ~Simulation();                                      
        Simulation(const Simulation &other);               
        Simulation &operator=(const Simulation &other);    
        Simulation(Simulation &&other) noexcept;           
        Simulation &operator=(Simulation &&other) noexcept; 

    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;
};