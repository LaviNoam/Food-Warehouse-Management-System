#pragma once
#include <string>
#include <vector>

#include "Order.h"
#include "Customer.h"
#include <iostream>//added
#include "Volunteer.h"//added
#include <fstream>//added
#include <sstream>//added
//#include "Action.h"//added
class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();
        void parseFile(const std::string& filename, std::vector<Customer*>& customers, std::vector<Volunteer*>& volunteers);//added parse function
        vector<Order*> &getPending();//added function
        vector<Order*> &getInProcessing();//added function
        vector<Order*> &getcompleted();//added function
        vector<Volunteer*> &getVolunteers();//added function
        vector<Customer*>& getCustomers();//added function
        WareHouse(const WareHouse &other);//added
        WareHouse &operator=(const WareHouse &other);//added
        WareHouse(WareHouse &&other);//added
        WareHouse & operator=(WareHouse &&other);//added
        ~WareHouse();//added
        void addOneCustomer();//added function
        void addOneVolunteer();//added function
        int& getOrderCounter();//added
        int& getCustomerCounter();//added
        int& getVolunteerCounter();//added
        int volunteerForOrder(Order& order);//added
        void orderfinished();//added
        void orderCompleteToPen();//added
    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        int orderCounter;//added For assigning unique orders IDs
        Volunteer* defaultV;//added
        Customer* defaultC;//added
        Order* defaultO;//added
};