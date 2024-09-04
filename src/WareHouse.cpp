#include "../include/WareHouse.h"
#include "../include/Action.h"
#include "../include/Parser.h"

//Constructor
WareHouse::WareHouse(const string &configFilePath):isOpen(false), actionsLog(), volunteers(),
        pendingOrders(), inProcessOrders(), completedOrders(),
      customers(), customerCounter(0), volunteerCounter(0), orderCounter(0),
      defaultV(new CollectorVolunteer(-1, "None", -1)),
      defaultC(new SoldierCustomer(-1, "None", -1, -1)),
      defaultO(new Order(-1, -1, -1)) {
    Parser::parseConfigurationFile(configFilePath,*this);
}

//copy Constructor
WareHouse::WareHouse(const WareHouse &other):isOpen(other.isOpen), actionsLog(), volunteers(),
    pendingOrders(), inProcessOrders(),completedOrders(), customers(), customerCounter(other.customerCounter),
    volunteerCounter(other.volunteerCounter), orderCounter(other.orderCounter),
    defaultV(other.defaultV->clone()), defaultC(other.defaultC->clone()),defaultO(other.defaultO->clone()) {
    
    customers.clear();
    for (auto &customer : other.customers) {
        customers.push_back(customer->clone());
    }

    volunteers.clear();
    for (const auto &volunteer : other.volunteers) {
        volunteers.push_back(volunteer->clone());
    }

    pendingOrders.clear();
    for (const auto &order : other.pendingOrders) {
        pendingOrders.push_back(order->clone());
    }

    inProcessOrders.clear();
    for (const auto &order : other.inProcessOrders) {
        inProcessOrders.push_back(order->clone());
    }

    completedOrders.clear();
    for (const auto &order : other.completedOrders) {
        completedOrders.push_back(order->clone());
    }
    
    actionsLog.clear();
    for (const auto & action : other.actionsLog) {
        actionsLog.push_back(action->clone());
    }
}
// Copy Assignment Operator
WareHouse &WareHouse::operator=(const WareHouse &other) {
    if (this != &other) {
        for (auto &customer : customers) {
            delete customer;
        }
        customers.clear();

        for (auto &volunteer : volunteers) {
            delete volunteer;
        }
        volunteers.clear();

        for (auto &order : pendingOrders) {
            delete order;
        }
        pendingOrders.clear(); 

        for (auto &order : inProcessOrders) {
            delete order;
        }
        inProcessOrders.clear();

        for (auto &order : completedOrders) {
            delete order;
        }
        completedOrders.clear();
        
        for (auto &action : actionsLog) {
            delete action;
        }
        actionsLog.clear();
        
        delete defaultV;
        delete defaultC;
        delete defaultO;

        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;

        for (const auto &customer : other.customers) {
                this->customers.push_back(customer->clone());
        }

        for (const auto &volunteer : other.volunteers) {
                this->volunteers.push_back(volunteer->clone());
        }

        for (const auto &order : other.pendingOrders) {
                this->pendingOrders.push_back(order->clone());
        }

        for (const auto &order : other.inProcessOrders) {
                this->inProcessOrders.push_back(order->clone());
        }

        for (const auto &order : other.completedOrders) {
                this->completedOrders.push_back(order->clone());
        }

        for (const auto &action : other.actionsLog) {
                this->actionsLog.push_back(action->clone());
        }

        defaultV = other.defaultV->clone();
        defaultC = other.defaultC->clone();
        defaultO = other.defaultO->clone();
    }
    return *this;
}
//Move Constructor 
WareHouse::WareHouse(WareHouse &&other): isOpen(other.isOpen), actionsLog(std::move(other.actionsLog)),
      volunteers(std::move(other.volunteers)), pendingOrders(std::move(other.pendingOrders)),
      inProcessOrders(std::move(other.inProcessOrders)),
      completedOrders(std::move(other.completedOrders)),
      customers(std::move(other.customers)), customerCounter(other.customerCounter),
      volunteerCounter(other.volunteerCounter), orderCounter(other.orderCounter),
      defaultV(other.defaultV), defaultC(other.defaultC), defaultO(other.defaultO) {
    other.isOpen = false;
    other.customerCounter = 0;
    other.volunteerCounter = 0;
    other.orderCounter = 0;
    other.defaultV = nullptr;
    other.defaultC = nullptr;
    other.defaultO = nullptr;
}

//Move Assignment Operator
WareHouse &WareHouse::operator=(WareHouse &&other) {
    isOpen = other.isOpen;
    customerCounter = other.customerCounter;
    volunteerCounter = other.volunteerCounter;
    orderCounter = other.orderCounter;

    customers.clear();
    for (auto &customer : other.customers) {
        customers.push_back(customer->clone());
    }

    volunteers.clear();
    for (const auto &volunteer : other.volunteers) {
        volunteers.push_back(volunteer->clone());
    }

    pendingOrders.clear();
    for (const auto &order : other.pendingOrders) {
        pendingOrders.push_back(order->clone());
    }

    inProcessOrders.clear();
    for (const auto &order : other.inProcessOrders) {
        inProcessOrders.push_back(order->clone());
    }

    completedOrders.clear();
    for (const auto &order : other.completedOrders) {
        completedOrders.push_back(order->clone());
    }
    
    actionsLog.clear();
    for (auto & action : other.actionsLog) {
        actionsLog.push_back(action->clone());
    }
    defaultV = other.defaultV->clone();
    defaultC = other.defaultC->clone();
    defaultO = other.defaultO->clone();
    other.customerCounter = 0;
    other.volunteerCounter = 0;
    other.orderCounter = 0;
    other.defaultV = nullptr;
    other.defaultC = nullptr;
    other.defaultO = nullptr;
    other.isOpen = false;
    return *this;
}
//destructor
WareHouse::~WareHouse() {
    for (auto &customer : customers) {
        delete customer;
    }
    
    for (auto &volunteer : volunteers) {
        delete volunteer;
    }

    for (auto &order : pendingOrders) {
        delete order;
    }
    
    for (auto &order : inProcessOrders) {
        delete order;
    }

    for (auto &order : completedOrders) {
        delete order;
    }
    for (auto &action : actionsLog) {
        delete action;
    }
    
    actionsLog.clear();
    completedOrders.clear();
    inProcessOrders.clear();
    pendingOrders.clear(); 
    volunteers.clear();
    customers.clear();
    delete defaultV;
    delete defaultC;
    delete defaultO;
}

void WareHouse::start()
{
    open();
    std::cout << "Warehouse is open! \n";

    while (isOpen)
    {
        std::string input;
        std::getline(std::cin, input);  // Use getline to read the entire line

        std::istringstream iss(input);
        std::string firstWord;
        std::string rest;

        iss >> firstWord;

        if (firstWord == "step")
        {
            int arg;
            iss >> arg;
            SimulateStep *step = new SimulateStep(arg);
            step->act(*this);
        }
        else if (firstWord == "order")
        {
            int arg;
            iss >> arg;
            AddOrder *order = new AddOrder(arg);
            order->act(*this);
        }
        else if (firstWord == "orderStatus")
        {
            std::string restOfLine;
            std::getline(iss, restOfLine);  // Read the entire line after the command

            // Use a new stringstream to extract the integer from the rest of the line
            std::istringstream restIss(restOfLine);
            int arg;
            restIss >> arg;

            PrintOrderStatus *orderStatus = new PrintOrderStatus(arg);
            orderStatus->act(*this);
        }
        else if (firstWord == "customerStatus")
        {
            int arg;
            iss >> arg;
            PrintCustomerStatus *customerStatus = new PrintCustomerStatus(arg);
            customerStatus->act(*this);
        }
        else if (firstWord == "volunteerStatus")
        {
            int arg;
            iss >> arg;
            PrintVolunteerStatus *volunteerStatus = new PrintVolunteerStatus(arg);
            volunteerStatus->act(*this);
        }
        else if (firstWord == "log")
        {
            PrintActionsLog *log = new PrintActionsLog();
            log->act(*this);
        }
        else if (firstWord == "customer")
        {
            string name;
            string type;
            int dist, max;
            iss >> name >> type >> dist >> max;
            AddCustomer *customer = new AddCustomer(name, type, dist, max);
            customer->act(*this);
        }
        else if (firstWord == "backup")
        {
            BackupWareHouse *backup = new BackupWareHouse();
            backup->act(*this);
        }
        else if (firstWord == "restore")
        {
            RestoreWareHouse *restore = new RestoreWareHouse();
            restore->act(*this);
        }
        else if (firstWord == "close")
        {
            Close *close = new Close();
            close->act(*this);
            return;
        }
    }
}


void WareHouse::addOrder(Order* order){
    pendingOrders.push_back(order);
    orderCounter++;
}
void WareHouse::addAction(BaseAction* action){
    actionsLog.push_back(action);
}
Customer & WareHouse::getCustomer(int customerId) const{
    if(customerId<customerCounter){
        for (auto customer : customers) {
            if (customer->getId() == customerId) {
                auto& temp = *customer;
                return (temp);
            }
        }
    }
    return *defaultC;
}
Volunteer & WareHouse::getVolunteer(int volunteerId) const{
    if(volunteerId<volunteerCounter){
        for (auto volunteer : volunteers) {
            if (volunteer->getId() == volunteerId) {
                auto& temp = *volunteer;
                return (temp);
                }
        }
    }
    return *defaultV;
}
Order & WareHouse::getOrder(int orderId) const{
    if(orderId<orderCounter){
        for (auto order : pendingOrders) {
            if (order->getId() == orderId) {
                auto& temp = *order;
                return (temp);
                }
        }
        for (auto order : inProcessOrders) {
            if (order->getId() == orderId) {
                auto& temp = *order;
                return (temp);
                }
        }
        for (auto order : completedOrders) {
            if (order->getId() == orderId) {
                auto& temp = *order;
                return (temp);
                }
        }
    }
    return *defaultO;    
}
const vector<BaseAction*> & WareHouse::getActions() const{
    return actionsLog;
}
void WareHouse::close(){
    isOpen=false;  
}
void WareHouse::open(){
    isOpen=true;
}
void WareHouse::addOneCustomer(){
    customerCounter++;
}
void WareHouse::addOneVolunteer(){
    volunteerCounter++;
}
vector<Customer*>& WareHouse::getCustomers(){
    return customers;
}
vector<Order*>& WareHouse::getPending(){
    return pendingOrders;
}
vector<Order*>& WareHouse::getInProcessing(){
    return inProcessOrders;
}
vector<Order*>& WareHouse::getcompleted(){
    return completedOrders;
}
vector<Volunteer*>& WareHouse::getVolunteers(){
    return volunteers;
}
int & WareHouse::getOrderCounter(){
    return orderCounter;
}
int & WareHouse::getCustomerCounter(){
    return customerCounter;
}
int & WareHouse::getVolunteerCounter(){
    return volunteerCounter;
}
int  WareHouse::volunteerForOrder(Order& order){//volunter can take orders 
    for (auto  volunteer : volunteers){
        if(volunteer->canTakeOrder(order)){ 
            volunteer->acceptOrder(order);
            if(order.getStatus()==OrderStatus::PENDING){//if order going to collector
                order.setStatus(OrderStatus::COLLECTING);
                order.setCollectorId(volunteer->getId());
            }
            else{ order.setStatus(OrderStatus::DELIVERING);//if order going for driver
                order.setDriverId(volunteer->getId());
            }
            return 1;
        }
    }
    return -1;
}
void WareHouse::orderfinished(){
 for(auto  volunteer : volunteers){   
    volunteer->step();
    int index=-1;
    for (auto  order : inProcessOrders){
        index++;
        if(order->getId()==volunteer->getCompletedOrderId()){
            if(order->getStatus()==OrderStatus::DELIVERING){;//if driver finshed sent to complete
            order->setStatus(OrderStatus::COMPLETED);
            }
            completedOrders.push_back(order->clone());
            delete inProcessOrders[index];  
            inProcessOrders.erase(inProcessOrders.begin()+index);
            }
        }    
    }
}       
void WareHouse::orderCompleteToPen(){
    int index=-1;
    for (auto  order : completedOrders){
        index ++;
        if(order->getStatus()==OrderStatus::COLLECTING){
            order->setStatus(OrderStatus::DELIVERING);////if collector finshed sent to pending
            pendingOrders.push_back(order->clone());
            delete completedOrders[index];  
            completedOrders.erase(completedOrders.begin()+index);
        }
    }
}  
    
    
    
    
