#include "../include/Action.h"
extern WareHouse* backup;

BaseAction::BaseAction():errorMsg("ERROR"),status (ActionStatus::ERROR){

}
ActionStatus BaseAction::getStatus() const{
    return status;
}
void BaseAction::complete(){
    status=ActionStatus::COMPLETED;
}
void BaseAction::error(string errorMsg){
    status=ActionStatus::ERROR;
    std::cout<<"Error: "+ errorMsg;
}
string BaseAction::getErrorMsg() const{
    return errorMsg;
}
BaseAction::~BaseAction(){
}

SimulateStep::SimulateStep(int numOfSteps):numOfSteps(numOfSteps){
}

void SimulateStep::act(WareHouse &wareHouse) {
int times=0;
    while(numOfSteps!=times){
            wareHouse.orderCompleteToPen();
            for (int index =0 ; index < (int) wareHouse.getPending().size() ;index++) {
                    Order order = *(wareHouse.getPending()[index]);
                    if(wareHouse.volunteerForOrder(order)==1){//volunter take order
                        wareHouse.getInProcessing().push_back(order.clone());
                        delete wareHouse.getPending()[index];
                        wareHouse.getPending().erase(wareHouse.getPending().begin()+index);
                        index=-1;
                    }  
            }
            wareHouse.orderfinished();
            int index=-1;
            for(auto  volunteer : wareHouse.getVolunteers()){ 
                index++;
                if(!volunteer->hasOrdersLeft()&& (!volunteer->isBusy())){//if finished and limted delete him
                    delete volunteer;
                    wareHouse.getVolunteers().erase(wareHouse.getVolunteers().begin()+index);
                }                 
            }
        times++;
        }
    complete();
    wareHouse.addAction(this);    
}

std::string SimulateStep::toString() const{
return "SimulateStep " +std::to_string(numOfSteps)+" "+ actionStatusIs(getStatus());
}
SimulateStep* SimulateStep::clone() const{
    return new SimulateStep(*this);
}

AddOrder::AddOrder(int id):customerId(id){
}       
void AddOrder::act(WareHouse &wareHouse){
    if(customerId!=-1){
        if(wareHouse.getCustomer(customerId).canMakeOrder()){
            wareHouse.getCustomer(customerId).addOrder(wareHouse.getOrderCounter());
            wareHouse.addOrder(new Order(wareHouse.getOrderCounter(),wareHouse.getCustomer(customerId).getId(),wareHouse.getCustomer(customerId).getCustomerDistance()));
            complete();
            }
        else error("Cannot place this order \n");
    }
    else error("There is no customer with customerID = "+ std::to_string(customerId));
    wareHouse.addAction(this);
}

string AddOrder::toString() const {
    return "order "+std::to_string(customerId)+" "+ actionStatusIs(getStatus()); 
}
AddOrder* AddOrder::clone() const{
    return new AddOrder(*this);
}

AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders)
:customerName(customerName),customerType(stringType(customerType)),distance(distance),maxOrders(maxOrders){
}
void AddCustomer::act(WareHouse &wareHouse){ 
    if(customerType==CustomerType::Soldier){
        wareHouse.getCustomers().push_back(new SoldierCustomer(wareHouse.getCustomerCounter(),customerName,distance,maxOrders));
    }
    else if(customerType==CustomerType::Civilian){
        wareHouse.getCustomers().push_back(new CivilianCustomer(wareHouse.getCustomerCounter(),customerName,distance,maxOrders));
    }
    wareHouse.addOneCustomer();
    complete();
    wareHouse.addAction(this);
}

AddCustomer* AddCustomer::clone() const{
    return new AddCustomer (*this);
}
string AddCustomer::toString() const{
    std::string customerTypeS;
    if (customerType == CustomerType::Soldier) {
        customerTypeS = "Soldier";
    }
    else customerTypeS = "Civilian";
    return "customer "+ customerName + " "+ customerTypeS + " "+ std::to_string(distance) +" "+ std::to_string(maxOrders)+ actionStatusIs(getStatus());
}

PrintOrderStatus::PrintOrderStatus(int id):orderId(id){
}
void PrintOrderStatus::act(WareHouse &wareHouse){
    if(orderId < wareHouse.getOrderCounter()  &&wareHouse.getOrder(orderId).getId()!=-1){
        std::cout<<wareHouse.getOrder(orderId).toString();
        complete();
    }
    else error("Order doesn't exist \n");
    wareHouse.addAction(this);
}
PrintOrderStatus* PrintOrderStatus::clone() const{
    return new PrintOrderStatus(*this);
}
string PrintOrderStatus::toString() const{
    return "OrderStatus " +std::to_string(orderId) +" "+actionStatusIs(getStatus());
}

PrintCustomerStatus::PrintCustomerStatus(int customerId):customerId(customerId){
}
void PrintCustomerStatus::act(WareHouse &wareHouse){
    if(customerId<wareHouse.getCustomerCounter() && wareHouse.getCustomer(customerId).getId() !=-1){
        std::cout<<"CustomerID: "+std::to_string(customerId)+"\n";
        for(int ordersId : (wareHouse.getCustomer(customerId)).getOrdersIds()){  
            std::cout<<"Orderid: "+ std::to_string(ordersId)+"\n";
            std::cout<<"OrderStatus: " + wareHouse.getOrder(ordersId).statusIs(wareHouse.getOrder(ordersId).getStatus())+ "\n";
        }
        std::cout << "numOrderLeft: " + std::to_string(wareHouse.getCustomer(customerId).getMaxOrders() - wareHouse.getCustomer(customerId).getNumOrders()) + "\n";
        complete();
    }
    else error("customer doesn't exist \n");
    wareHouse.addAction(this);
}

PrintCustomerStatus* PrintCustomerStatus::clone() const{
    return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus::toString() const{
    return "CustomerStatus " + std::to_string(customerId) +" "+ actionStatusIs(getStatus());
}

PrintVolunteerStatus::PrintVolunteerStatus(int id):volunteerId(id){
}
void PrintVolunteerStatus::act(WareHouse &wareHouse){
    if(volunteerId<wareHouse.getVolunteerCounter() && wareHouse.getVolunteer(volunteerId).getId()!=-1){
        std::cout<<wareHouse.getVolunteer(volunteerId).toString();
        complete();
    }
    else error("Volunteer doesn't exist \n" );
    wareHouse.addAction(this);
}
PrintVolunteerStatus* PrintVolunteerStatus::clone() const{
    return new PrintVolunteerStatus (*this);
}
string PrintVolunteerStatus::toString() const {
    return "volunteerStatus "+ std::to_string(volunteerId) +" "+actionStatusIs(getStatus());
}

PrintActionsLog::PrintActionsLog(){
}
void PrintActionsLog::act(WareHouse &wareHouse){
    for(auto action : wareHouse.getActions()){
        std::cout<<((*action).toString())+"\n";
    }
    complete();
    wareHouse.addAction(this);
}
PrintActionsLog* PrintActionsLog::clone() const{
    return new PrintActionsLog (*this);
}
string PrintActionsLog::toString() const{ 
    return "log "+actionStatusIs(getStatus());
}

Close::Close(){
}
void Close::act(WareHouse &wareHouse){ 
    for(int i=0 ;i<wareHouse.getOrderCounter();i++){
      std::cout << "OrderID: " + std::to_string(i) + ", " + "CustomerID: " + std::to_string((wareHouse.getOrder(i)).getCustomerId()) + ", "
      + "Status: " + wareHouse.getOrder(i).statusIs((wareHouse.getOrder(i)).getStatus()) + "\n";
    }
    delete backup;
    backup=NULL;
    complete();
    std::cout << backup;
    wareHouse.addAction(this);
    wareHouse.close();
}
Close* Close::clone() const{
    return new Close(*this);
}
string Close::toString() const{
    return "close "+actionStatusIs(getStatus());
}

BackupWareHouse::BackupWareHouse(){
}
void BackupWareHouse::act(WareHouse &wareHouse) {
    if(backup == NULL){
        backup = new WareHouse(wareHouse);
    }
    else {
        delete backup;
        backup = new WareHouse(wareHouse);
    }
    complete();
    wareHouse.addAction(this);
}

BackupWareHouse* BackupWareHouse::clone() const {
    return new BackupWareHouse(*this);
}
string BackupWareHouse::toString() const {
    return "backup "+actionStatusIs(getStatus());
}

RestoreWareHouse::RestoreWareHouse(){
}
void RestoreWareHouse::act(WareHouse &wareHouse){
    if (backup != 0){
        wareHouse = *backup;
        complete();
    }
    else error("No backup available \n");
    wareHouse.addAction(this);
}

RestoreWareHouse* RestoreWareHouse::clone() const{
    return new RestoreWareHouse(*this);
}
string RestoreWareHouse::toString() const{
    return "restore "+actionStatusIs(getStatus());
}

CustomerType stringType(const string &customerTypeT) {
    if (customerTypeT == "Soldier"||customerTypeT == "soldier"){
        return CustomerType::Soldier;
    }
    return CustomerType::Civilian;
}
string actionStatusIs(ActionStatus status) {
    if (status == ActionStatus::COMPLETED) {
        return "COMPLETED";
    }
    return "ERROR"; 
}