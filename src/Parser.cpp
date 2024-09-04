#include "../include/Parser.h"
#include "../include/WareHouse.h"
#include "../include/Customer.h"
#include "../include/Volunteer.h"
#include <fstream>
#include <sstream>

void Parser::parseConfigurationFile(const std::string& configFilePath, WareHouse& wareHouse) {
    std::ifstream file(configFilePath);
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line[0] != '#') {
            std::istringstream iss(line);
            std::string key;
            iss >> key;

            if (key == "customer"){
                std::string name, customerType;
                int locationDistance, maxOrders;
                iss >> name >> customerType >> locationDistance >> maxOrders;

                if (customerType == "soldier") {
                    wareHouse.getCustomers().push_back(new SoldierCustomer(wareHouse.getCustomerCounter(), name, locationDistance, maxOrders));
                }
                else {
                    wareHouse.getCustomers().push_back(new CivilianCustomer(wareHouse.getCustomerCounter(), name, locationDistance, maxOrders));
                }
                wareHouse.addOneCustomer();
            }

            else if (key == "volunteer"){
                std::string name, volunteerType;
                iss >> name >> volunteerType;
                if (volunteerType == "collector") {
                    int cooldown;
                    iss >> cooldown;
                    wareHouse.getVolunteers().push_back(new CollectorVolunteer(wareHouse.getVolunteerCounter(), name, cooldown));
                }
                else if (volunteerType == "limited_collector") {
                    int cooldown, maxOrders;
                    iss >> cooldown >> maxOrders;
                    wareHouse.getVolunteers().push_back(new LimitedCollectorVolunteer(wareHouse.getVolunteerCounter(), name, cooldown, maxOrders));
                }
                else if (volunteerType == "driver"){
                    int maxDistance, distancePerStep;
                    iss >> maxDistance >> distancePerStep;
                    wareHouse.getVolunteers().push_back(new DriverVolunteer(wareHouse.getVolunteerCounter(), name, maxDistance, distancePerStep));
                }
                else {
                    int maxDistance, distancePerStep, maxOrders;
                    iss >> maxDistance >> distancePerStep >> maxOrders;
                    wareHouse.getVolunteers().push_back(new LimitedDriverVolunteer(wareHouse.getVolunteerCounter(), name, maxDistance, distancePerStep, maxOrders));
                
                }
            wareHouse.addOneVolunteer();
        }
    }
}
}