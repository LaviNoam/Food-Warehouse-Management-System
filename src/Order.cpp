#include "../include/Order.h"

Order::Order(int id, int customerId, int distance):
id(id),customerId(customerId),distance(distance),
status(OrderStatus::PENDING),collectorId(NO_VOLUNTEER),
driverId(NO_VOLUNTEER){
} 
int Order::getId() const{
    return id;
}
int Order::getCustomerId() const{
    return customerId;
}
void Order::setStatus(OrderStatus status){
    this->status=status;
}
void Order::setCollectorId(int collectorId){
    this->collectorId=collectorId;
}
void Order::setDriverId(int driverId){
    this->driverId=driverId;
}
int Order::getCollectorId() const{
    return collectorId;
}   
int Order::getDriverId() const{
    return driverId;
}
OrderStatus Order::getStatus() const{
    return status;
}
const string Order::toString() const{
    if(collectorId ==-1){return " Order ID: " + std::to_string(id) + "\n"+
        " Order Status: " + statusIs(status) + "\n"+
        " Customer ID: " + std::to_string(customerId) + "\n"+
        " Collector ID: " + "None" + "\n"+
        " Driver ID: " + "None" + "\n";
    }
    else if (driverId ==-1){return " Order ID: " + std::to_string(id) + "\n"+
    " Order Status: " + statusIs(status) + "\n"+
    " Customer ID: " + std::to_string(customerId) + "\n"+
    " Collector ID: " + std::to_string(collectorId) + "\n"+
    " Driver ID: " + "None" + "\n";
    }
    return  " Order ID: " + std::to_string(id) + "\n"+
    " Order Status: " + statusIs(status) + "\n"+
    " Customer ID: " + std::to_string(customerId) + "\n"+
    " Collector ID: " + std::to_string(collectorId) + "\n"+
    " Driver ID: " + std::to_string(driverId);
    }
//added functions
Order* Order::clone()const{
    return new Order(*this);
}
int Order::getDistance () const{
    return distance;
}
string Order::statusIs (OrderStatus status)const{
    std::string orderStatsus;
    if (status == OrderStatus::PENDING) {
        return orderStatsus  = "PENDING";
    }
    else if(status == OrderStatus::COLLECTING){
        return orderStatsus = "COLLECTING";
    }
    else if(status == OrderStatus::DELIVERING){
        return orderStatsus = "DELIVERING";
    }
    return orderStatsus = "COMPLETED";
}