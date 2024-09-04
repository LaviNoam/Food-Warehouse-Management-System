#include "../include/Volunteer.h"

Volunteer::Volunteer (int id, const string &name):completedOrderId(NO_ORDER),activeOrderId(NO_ORDER),id(id),name(name){
}
int Volunteer::getId()const{
    return id;
}
const string &Volunteer::getName() const{
    return name;
}
int Volunteer::getActiveOrderId() const{
    return  activeOrderId;
}
int Volunteer::getCompletedOrderId() const{
    return completedOrderId;
}
bool Volunteer::isBusy() const{// Signal whether the volunteer is currently processing an order  
    return getActiveOrderId() !=NO_ORDER;
}  
Volunteer::~Volunteer(){
}

CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown):Volunteer(id,name),coolDown(coolDown),timeLeft(0){
}
CollectorVolunteer* CollectorVolunteer::clone() const {
return new CollectorVolunteer(*this);
}
void CollectorVolunteer::step() {
    if(decreaseCoolDown()){
        Volunteer::completedOrderId = activeOrderId;
        Volunteer::activeOrderId = NO_ORDER;
    }
}
int CollectorVolunteer::getCoolDown() const{
    return coolDown;
}
int CollectorVolunteer::getTimeLeft() const{
    return timeLeft;
}
bool CollectorVolunteer::decreaseCoolDown(){//Decrease timeLeft by 1,return true if timeLeft=0,false otherwise
    timeLeft--;
    if(timeLeft<0){
        timeLeft=0;
    }
    return(timeLeft==0);
}
bool CollectorVolunteer::hasOrdersLeft() const {
    return true;
}
bool CollectorVolunteer::canTakeOrder(const Order &order) const {
    return((timeLeft==0 )&& (order.getStatus()==OrderStatus::PENDING) && (!isBusy()));
}
void CollectorVolunteer::acceptOrder(const Order &order) {
    if(canTakeOrder(order)){
        timeLeft=coolDown;
        activeOrderId= order.getId();
    }
}
string CollectorVolunteer::toString() const {
    if(isBusy()){
        if(getTimeLeft()!=0){return
            "Volunteer ID :"+ std::to_string(Volunteer::getId()) +"\n"+
            " isBusy: " +  "true" +"\n"+
            " OrderId: " + std::to_string(getActiveOrderId()) +"\n"+
            " timeleft: " + std::to_string(getTimeLeft()) +"\n"+
            " ordersLeft: " + "No Limit" + "\n";
        }
        else return "Volunteer ID :"+ std::to_string(Volunteer::getId()) +"\n"+
            " isBusy: " +  "true" +"\n"+
            " OrderId: " + std::to_string(getActiveOrderId()) +"\n"+
            " timeleft: " + "None" +"\n"+
            " ordersLeft: " + "No Limit" + "\n";
    }
    return "Volunteer ID :"+ std::to_string(Volunteer::getId()) +"\n"+
    " isBusy: " +  "false" +"\n"+
    " OrderId: " + "None" +"\n"+
    " timeleft: " + "None"+"\n"+
    " ordersLeft: " + "No Limit" + "\n";   
}


LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string &name, int coolDown ,int maxOrders):
CollectorVolunteer(id,name,coolDown),maxOrders(maxOrders),ordersLeft(maxOrders){
}
LimitedCollectorVolunteer* LimitedCollectorVolunteer::clone() const {
    return new LimitedCollectorVolunteer(*this);
}
bool LimitedCollectorVolunteer::hasOrdersLeft() const {
    return (ordersLeft!=0);
}
bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const { 
    return(CollectorVolunteer::canTakeOrder(order) && hasOrdersLeft());
}
void LimitedCollectorVolunteer::acceptOrder(const Order &order) { 
    if(canTakeOrder(order)){
        CollectorVolunteer::acceptOrder(order);
        ordersLeft--;
    }
}
int LimitedCollectorVolunteer::getMaxOrders() const{
    return maxOrders;
}
int LimitedCollectorVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
}
string LimitedCollectorVolunteer::toString() const {
    if(isBusy()){
        if(getTimeLeft()!=0){
            return "Volunteer ID :"+ std::to_string(Volunteer::getId()) +"\n"+
            " isBusy: " +  "true" +"\n"+
            " OrderId: " + std::to_string(getActiveOrderId()) +"\n"+
            " timeleft: " + std::to_string(getTimeLeft()) +"\n"+
            " ordersLeft: " + std::to_string(getNumOrdersLeft()) + "\n";
        }
        else return "Volunteer ID :"+ std::to_string(Volunteer::getId()) +"\n"+
            " isBusy: " +  "true" +"\n"+
            " OrderId: " + std::to_string(getActiveOrderId()) +"\n"+
            " timeleft: " + "None" +"\n"+
            " ordersLeft: " + std::to_string(getNumOrdersLeft()) + "\n";
    }
    return "Volunteer ID :"+ std::to_string(Volunteer::getId()) +"\n"+
    " isBusy: " + "false" +"\n"+
    " OrderId: " + "None" +"\n"+
    " timeleft: " + "None" +"\n"+
    " ordersLeft: " + std::to_string(getNumOrdersLeft()) + "\n";       
}

DriverVolunteer::DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep):Volunteer(id,name),maxDistance(maxDistance),
distancePerStep(distancePerStep),distanceLeft(maxDistance){
}
DriverVolunteer* DriverVolunteer::clone() const {
    return new DriverVolunteer(*this);
}
int DriverVolunteer::getDistanceLeft() const{
    return distanceLeft;
}
int DriverVolunteer::getMaxDistance() const{
    return maxDistance;
}
int DriverVolunteer::getDistancePerStep() const{
    return distancePerStep;
}
bool DriverVolunteer::decreaseDistanceLeft(){ //Decrease distanceLeft by distancePerStep,return true if distanceLeft<=0,false otherwise
    distanceLeft=distanceLeft-distancePerStep;
    return(distanceLeft<=0);
}
bool DriverVolunteer::hasOrdersLeft() const {
    return true;
}
bool DriverVolunteer::canTakeOrder(const Order &order) const {// Signal if the volunteer is not busy and the order is within the maxDistance
    return !isBusy() && order.getDistance() <= maxDistance && order.getStatus()!=OrderStatus::PENDING;
}
void DriverVolunteer::acceptOrder(const Order &order) { // Assign distanceLeft to order's distance
    if(canTakeOrder(order)){
        distanceLeft=order.getDistance();
        Volunteer::activeOrderId= order.getId();
    }
}
void DriverVolunteer::step() { // Decrease distanceLeft by distancePerStep
    if(decreaseDistanceLeft()){
        distanceLeft=maxDistance;
        Volunteer::completedOrderId = activeOrderId;
        Volunteer::activeOrderId = NO_ORDER;
    }
}
string DriverVolunteer::toString() const {
    if(isBusy()){
        if(getDistanceLeft()!=0){
            return "Volunteer ID :"+ std::to_string(Volunteer::getId()) +"\n"+
            " isBusy: " +  "true" +"\n"+
            " OrderId: " + std::to_string(getActiveOrderId()) +"\n"+
            " Distance Left: " + std::to_string(getDistanceLeft()) +"\n"+
            " ordersLeft: " + "No Limit" + "\n";
    }
        else
            return "Volunteer ID :"+ std::to_string(Volunteer::getId()) +"\n"+
            " isBusy: " +  "true" +"\n"+
            " OrderId: " + std::to_string(getActiveOrderId()) +"\n"+
            " Distance Left: " + "None" +"\n"+
            " ordersLeft: " + "No Limit" + "\n";
    }
    return "Volunteer ID :"+ std::to_string(Volunteer::getId()) +"\n"+
    " isBusy: " + "false" +"\n"+
    " OrderId: " + "None" +"\n"+
    " Distance Left: " + "None" +"\n"+
    " ordersLeft: " + "No Limit" + "\n";   
}


LimitedDriverVolunteer::LimitedDriverVolunteer (int id, const string &name, int maxDistance, int distancePerStep,int maxOrders):
DriverVolunteer(id,name,maxDistance,distancePerStep),maxOrders(maxOrders),ordersLeft(maxOrders){
}
LimitedDriverVolunteer* LimitedDriverVolunteer::clone() const {
    return new LimitedDriverVolunteer(*this);
}
int LimitedDriverVolunteer::getMaxOrders() const{
    return maxOrders;
}
int LimitedDriverVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
}
bool LimitedDriverVolunteer::hasOrdersLeft() const {
    return(ordersLeft!=0);
}
bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const { // Signal if the volunteer is not busy, the order is within the maxDistance.
    return DriverVolunteer::canTakeOrder(order) && hasOrdersLeft();
}
void LimitedDriverVolunteer::acceptOrder(const Order &order) { // Assign distanceLeft to order's distance and decrease ordersLeft
    if(canTakeOrder(order)){
        ordersLeft--;
        DriverVolunteer::acceptOrder(order);
    }
}
string LimitedDriverVolunteer::toString() const {
    if(isBusy())
        {if(getDistanceLeft()!=0){
            return "Volunteer ID :"+ std::to_string(Volunteer::getId()) +"\n"+
            " isBusy: " + "true" +"\n"+
            " OrderId: " + std::to_string(getActiveOrderId()) +"\n"+
            " Distance Left: " + std::to_string(getDistanceLeft()) +"\n"+
            " ordersLeft: " + std::to_string(getNumOrdersLeft())+"\n";
    }
        else
            return "Volunteer ID :"+ std::to_string(Volunteer::getId()) +"\n"+
            " isBusy: " + "true" +"\n"+
            " OrderId: " + std::to_string(getActiveOrderId()) +"\n"+
            " Distance Left: None"+"\n"+
            " ordersLeft: " + std::to_string(getNumOrdersLeft())+"\n";
    }
        return "Volunteer ID :"+ std::to_string(Volunteer::getId()) +"\n"+
        " isBusy: " + "false" +"\n"+
        " OrderId: " + "None" +"\n"+
        " Distance Left: None"+"\n"+
        " ordersLeft: " + std::to_string(getNumOrdersLeft())+"\n";   
}

