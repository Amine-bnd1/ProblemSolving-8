#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <cctype>



using namespace std;

const string FILENAME  = "Clients.Txt";
const string FILENAMEU = "Users.Txt";

void ShowMainMenuScreen();
void ShowTransactionsMainMenuScreen();
void ShowManageUserMainManu();
void ShowAccesDeniedMessage();
void Login();

struct stClient {
  string AccountNumber;
  string PinCode;
  string Name;
  string Phone;
  double AccountBalance = 0;
  bool MarkForDelete = false;
};

struct stUser {

  string UserName;
  string Password;
  int Permissions = 0;
  bool MarkForDelete = false;
};

stUser CurrentUser;

enum enMainMenuOptions
{
  enListClient = 1,
  enAddClient = 2,
  enDeleteClient = 3,
  enUpdateClient = 4,
  enFindClient = 5,
  enTrnsactions = 6,
  enManageUsers = 7,
  enLogout = 8

};

enum enTransactionsMainOptions
{
  tDeposit = 1,
  tWithdraw = 2,
  tTotalBalances = 3,
  tMainMenu = 4

};

enum enPermissions
{
  eAll = -1,
  pListClients = 1,
  pAddNewClient = 2,
  pDeleteClient = 4,
  pUpdateClientInfo = 8,
  pFindClient = 16,
  pTransactions = 32,
  pManageUsers = 64
};
bool CheckAccessPermissions(enPermissions);


enum enManageUsersOptions
{
  enListUsers = 1,
  enAddNewUser = 2,
  enDeleteUser = 3,
  enUpdateUser = 4,
  enFindUser = 5,
  enMainMenu = 6

};

vector<string>SplitString(string S1, string Delim){
  vector<string> VstClients;
  size_t pos = 0;
  string sword = "";

  while((pos = S1.find(Delim)) != std::string::npos){
    sword = S1.substr(0, pos);
    if(sword != ""){
      VstClients.push_back(sword);
    }
    S1.erase(0, pos + Delim.length());
  }
  if(!S1.empty()){
    VstClients.push_back(S1);
  }
  return VstClients;
}

stClient ConvertDataLineToRecord(string Line, string Seperator = "#//#"){
  vector<string> VstClients = SplitString(Line, Seperator);
  stClient Client;

  Client.AccountNumber = VstClients[0];
  Client.PinCode = VstClients[1];
  Client.Name = VstClients[2];
  Client.Phone = VstClients[3];
  Client.AccountBalance = stod(VstClients[4]);

  return Client;
}

stUser ConvertUsersLineToRecord(string Line, string Seperator = "#//#" ){
  stUser User;
  vector<string> VstUsers = SplitString(Line, Seperator);

  User.UserName = VstUsers[0];
  User.Password = VstUsers[1];
  User.Permissions = stoi(VstUsers[2]);

  return User;
}

string ConvertClientRecordToLine(stClient Client, string Seperator = "#//#"){

  string stDataRecordLine = "";

  stDataRecordLine += Client.AccountNumber + Seperator;
  stDataRecordLine += Client.PinCode + Seperator;
  stDataRecordLine += Client.Name+ Seperator;
  stDataRecordLine += Client.Phone+ Seperator;
  stDataRecordLine += to_string(Client.AccountBalance);

  return stDataRecordLine;
}

string ConvertUsersRecordToLine(stUser User, string Seperator = "#//#"){

  string stUserLine = "";

  stUserLine += User.UserName + Seperator;
  stUserLine += User.Password + Seperator;
  stUserLine += to_string(User.Permissions);

  return stUserLine;
}

bool IsClientExistsByAccountNumber(string AccountNumber, string FILENAME){
  vector<stClient> VstClients;
  fstream MyFile;
  MyFile.open(FILENAME, ios :: in);

  if(MyFile.is_open()){

    string Line;
    stClient Client;

    while(getline(MyFile, Line)){
      Client = ConvertDataLineToRecord(Line);
      if(Client.AccountNumber == AccountNumber){
        MyFile.close();
        return true;
      }
      VstClients.push_back(Client);
    }
    MyFile.close();
  }
  return false;
}

bool IsUserExistsByUserName(string UserName, string FILENAMEU){

  fstream MyFile;
  MyFile.open(FILENAMEU, ios ::in);

  if(MyFile.is_open()){

    string Line;
    stUser User;

    while(getline(MyFile, Line)){
      User = ConvertUsersLineToRecord(Line);
      if(User.UserName == UserName){
        MyFile.close();
        return true;
      }
    }
    MyFile.close();
  }
  return false;
}

int ReadPermissionsToSet(){

  int Permissions = 0;
  char Answer = 'n';

  cout << "\nDo you want to give Full Access? y/n? ";
  cin >> Answer;
  if (toupper(Answer) == 'Y')
  {
    return -1;
  }
  cout << "\nDo you want to give Access To : \n";

  cout << "\nShow Client List? y/n? ";
  cin >> Answer;
  if (toupper(Answer) == 'Y'){
    Permissions += enPermissions::pListClients;
  }

  cout << "\nAdd New Client? y/n? ";
  cin >> Answer;
  if (toupper(Answer) == 'Y'){
    Permissions += enPermissions::pAddNewClient;
  }

  cout << "\nDelete Client? y/n? ";
  cin >> Answer;
  if(Answer == 'Y' || Answer == 'y'){
    Permissions += enPermissions::pDeleteClient;
  }

  cout << "\nUpdate Client's Info? y/n? ";
  cin >> Answer;
  if(toupper(Answer) == 'Y'){
    Permissions += enPermissions::pUpdateClientInfo;
  }

  cout << "\nFind Client? y/n? ";
  cin >> Answer;
  if(toupper(Answer) == 'Y'){
    Permissions += enPermissions::pFindClient;
  }

  cout << "\nTransactions? y/n? ";
  cin >> Answer;
  if(Answer == 'Y' || Answer == 'y'){
    Permissions += enPermissions::pTransactions;
  }

  cout << "\nManage Users? y/n? ";
  cin >> Answer;
  if(toupper(Answer) == 'Y'){
    Permissions += enPermissions::pManageUsers;
  }
  return Permissions;
}

stClient ReadNewClient(){

  stClient Client;

  cout << "\nPlease Enter Account Number? ";
  getline(cin >> ws, Client.AccountNumber);

  while(IsClientExistsByAccountNumber(Client.AccountNumber, FILENAME))
  {
    cout << "\nClient with Account Number [" << Client.AccountNumber << "] is already exist, Enter another Account Number? ";
    getline(cin >> ws, Client.AccountNumber);
  }

  cout << "\nPlease Enter PinCode? ";
  getline(cin >> ws, Client.PinCode);

  cout << "\nPlease Enter Client's Name? ";
  getline(cin >> ws, Client.Name);

  cout << "\nPlease Enter Phone Number? ";
  getline(cin >> ws, Client.Phone);

  cout << "\nPlease Enter Account Balance? ";
  cin >> Client.AccountBalance;

  return Client;
}

stUser ReadNewUser(){

  stUser User;

  cout << "\nPlease Enter User Name? ";
  getline(cin >> ws, User.UserName);

  while(IsUserExistsByUserName(User.UserName,FILENAMEU)){
    cout << "\nUser with User Name [" << User.UserName << "] is already exist. Please Enter another User Name? ";
    getline(cin >> ws, User.UserName);
  }

  cout << "\nPlease Enter Password? ";
  getline(cin >> ws, User.Password);

  User.Permissions = ReadPermissionsToSet();

  return User;
}

vector<stClient> LoadClientsDataFromFile(string FILENAME){

  vector<stClient> VstClients;

  fstream MyFile;
  MyFile.open(FILENAME, ios :: in);

  if(MyFile.is_open()){
    string Line;
    stClient Client;

    while(getline(MyFile, Line)){
      Client = ConvertDataLineToRecord(Line);
      VstClients.push_back(Client);
    }
    MyFile.close();
  }
  return VstClients;
}

vector<stUser> LoadUsersDataFromFile(string FILENAMEU){
  vector<stUser> VstUsers;
  fstream MyFile;
  MyFile.open(FILENAMEU, ios ::in);

  if(MyFile.is_open()){
    string Line;
    stUser User;

    while(getline(MyFile,Line)){
      User = ConvertUsersLineToRecord(Line);
      VstUsers.push_back(User);
    }
    MyFile.close();
  }
  return VstUsers;
}

void PrintClientRecordLine(stClient Client){

  cout << "| " << setw(15) << left << Client.AccountNumber;
  cout << "| " << setw(10) << left << Client.PinCode;
  cout << "| " << setw(40) << left << Client.Name;
  cout << "| " << setw(12) << left << Client.Phone;
  cout << "| " << setw(12) << left << Client.AccountBalance;
  cout << "| ";
}

void PrintClientTotalBalances(stClient Client){

  cout << "| " << setw(15) << left << Client.AccountNumber;
  cout << "| " << setw(40) << left << Client.Name;
  cout << "| " << setw(12) << left << Client.AccountBalance;
  cout << "| ";

}

void PrintUserRecordLine(stUser User){

  cout << "| " << setw(15) << left << User.UserName;
  cout << "| " << setw(40) << left << User.Password;
  cout << "| " << setw(15) << left << User.Permissions;
  cout << "| ";
}

void ShowAllClientsScreen(){

  if(!CheckAccessPermissions(enPermissions::pListClients)){
    ShowAccesDeniedMessage();
    return;
  }

  vector<stClient> VstClients = LoadClientsDataFromFile(FILENAME);
  stClient Client;

  cout << "\n\t\t\t\t\tClient List (" << VstClients.size() << ") Client(s). ";

  cout << "\n__________________________________________________________________________________________________\n";
  cout << "| " << left << setw(15) << "Account Number ";
  cout << "| " << left << setw(10) << "Pin Code ";
  cout << "| " << left << setw(40) << "Client's Name ";
  cout << "| " << left << setw(12) << "Phone Number ";
  cout << "| " << left << setw(12) << "Balance ";
  cout << "| ";
  cout << "\n__________________________________________________________________________________________________\n";


  if(VstClients.size() == 0){
    cout << "\nNo Client Available In The System. ";
  }

  else {
    for(stClient Client : VstClients){
      PrintClientRecordLine(Client);
      cout << endl;
    }
  }
  cout << "\n__________________________________________________________________________________________________";
  
}

void ShowAllUsersList() {

  vector<stUser> VstUsers = LoadUsersDataFromFile(FILENAMEU);

  cout << "\n\t\t\t\t\tUsers List (" << VstUsers.size() << ") User(s). ";

  cout << "\n__________________________________________________________________________________________________\n";
  cout << "| " << left << setw(20) << "User Name ";
  cout << "| " << left << setw(30) << "Password  ";
  cout << "| " << left << setw(20) << "Permissions ";
  cout << "\n__________________________________________________________________________________________________\n";

  if(VstUsers.size() == 0){
    cout << "\nNo Users Available In The System. ";
  }

  else {
    for(stUser User : VstUsers){
      PrintUserRecordLine(User);
      cout << endl;
    }
  }
  cout << "\n__________________________________________________________________________________________________\n";
}

void ShowAllClientTotalBalances(){

  vector<stClient> VstClients = LoadClientsDataFromFile(FILENAME);

  cout << "\n\t\t\tBalances List (" << VstClients.size() << ") Client(s). ";

  cout << "\n________________________________________________________________________________________________\n";
  cout << "| " << left << setw(15) << "Account Number ";
  cout << "| " << left << setw(40) << "Client's Name ";
  cout << "| " << left << setw(12) << "Balance ";
  cout << "| ";
  cout << "\n________________________________________________________________________________________________\n";

  double TotalBalances = 0;

  if (VstClients.size() == 0)
  {
    cout << "\nNo Clients Available In The System. ";
  }

  else {
    for(stClient Client : VstClients){
      PrintClientTotalBalances(Client);
      TotalBalances += Client.AccountBalance;
      cout << endl;
    }
  }
  cout << "\n________________________________________________________________________________________________\n";

  cout << "\n\t\t\tTotal Balances = " << TotalBalances;
}


void PrintClientCard(stClient Client){

  cout << "\nThe Following are the Client details : ";

  cout << "\n---------------------------------------------";
  cout << "\nAccount Number   : " << Client.AccountNumber;
  cout << "\nPin Code         : " << Client.PinCode;
  cout << "\nClient's Name    : " << Client.Name;
  cout << "\nPhone            : " << Client.Phone;
  cout << "\nAccount Balanace : " << Client.AccountBalance;
  cout << "\n---------------------------------------------";

}

void PrintUserCard(stUser User){

  cout << "\nThe Following are the User details : ";
  cout << "\n---------------------------------------------";
  cout << "\nUser Name      : " << User.UserName;
  cout << "\nPassword       : " << User.Password;
  cout << "\nPermissions    : " << User.Permissions;
  cout << "\n---------------------------------------------";

}


bool FindClientByAccountNumber(string AccountNumber, vector<stClient>& VstClients, stClient& OutClient){

  for(stClient& C : VstClients){
    if(C.AccountNumber == AccountNumber){
      OutClient = C;
      return true;
    }
  }
  return false;
}

bool FindUserByUserName(string UserName, vector<stUser>& VstUsers, stUser& OutUser){

  for(stUser U: VstUsers){
    if(U.UserName == UserName){
      OutUser = U;
      return true;
    }
  }
  return false;
}

bool FindUserByUserNameAndPassword(string UserName, string Password, stUser & User){

  vector<stUser> VstUsers = LoadUsersDataFromFile(FILENAMEU);

  for(stUser U : VstUsers){
    if(U.UserName == UserName && U.Password == Password){
      User = U;
      return true;
    }
  }
  return false;
}

stClient ChangeClientRecord(string AccountNumber){

  stClient Client;

  Client.AccountNumber = AccountNumber;

  cout << "\nPlease Enter Pin Code? ";
  getline(cin >> ws, Client.PinCode);

  cout << "\nPlease Enter Client's Name? ";
  getline(cin >> ws, Client.Name);

  cout << "\nPlease Enter Phone Number? ";
  getline(cin >> ws, Client.Phone);

  cout << "\nPlease Enter Account Balance? ";
  cin >> Client.AccountBalance;

  return Client;
}

stUser ChangeUserRecord(string UserName){

  stUser User;

  User.UserName = UserName;

  cout << "\nPlease Enter Password? ";
  getline(cin >> ws, User.Password);

  User.Permissions = ReadPermissionsToSet();

  return User;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<stClient>& VstClients){

  for(stClient& C : VstClients){
    if(C.AccountNumber == AccountNumber){
      C.MarkForDelete = true;
      return true;
    }
  }
  return false;
}

bool MarkUserForDeleteByUserName(string UserName, vector<stUser>& VstUsers){

  for(stUser& U: VstUsers){
    if(U.UserName == UserName){
      U.MarkForDelete = true;
      return true;
    }
  }
  return false;
}

vector<stClient> SaveClientDataToFile(string FILENAME, vector<stClient> VstClients){
  fstream MyFile;
  MyFile.open(FILENAME, ios :: out);

  if(MyFile.is_open()){

    string DataLine;

    for (stClient C : VstClients)
    {
      if(C.MarkForDelete == false){
        DataLine = ConvertClientRecordToLine(C);
        MyFile << DataLine << endl;
      }
    }
    MyFile.close();
  }
  return VstClients;
}

vector<stUser> SaveUserDataToFile(string FILENAMEU, vector<stUser>VstUsers){

  fstream MyFile;
  MyFile.open(FILENAMEU, ios ::out);

  if(MyFile.is_open()){

    string DataLines;

    for(stUser U: VstUsers){
      if(U.MarkForDelete == false){
        DataLines = ConvertUsersRecordToLine(U);
        MyFile << DataLines << endl;
      }
    }
    MyFile.close();
  }
  return VstUsers;
}

void AddDataLineToFile(string FILENAME, string stDataLine){
  fstream MyFile;
  MyFile.open(FILENAME, ios ::out | ios ::app);

  if(MyFile.is_open()){
    MyFile << stDataLine << endl;
    MyFile.close();
  }
}

void AddUserDataLineToFile(string FILENAMEU, string stDataLine){
  fstream MyFile;
  MyFile.open(FILENAMEU, ios ::out | ios ::app);

  if(MyFile.is_open()){

    MyFile << stDataLine << endl;
    MyFile.close();
  }
}

void AddNewClient(){
  stClient Client;
  Client = ReadNewClient();
  AddDataLineToFile(FILENAME, ConvertClientRecordToLine(Client));
}

void AddNewUser() {
  stUser User;
  User = ReadNewUser();
  AddUserDataLineToFile(FILENAMEU, ConvertUsersRecordToLine(User));
}

void AddNewClients(){
  stClient Client;
  char AddMore = 'n';

  do
  {
    cout << "\nAdding New Client :\n\n";

    AddNewClient();
    cout << "\nClient Added Successfully. Do you want to add more Clients? Y/N? ";
    cin >> AddMore;
  } while (toupper(AddMore) == 'Y');
}

void AddNewUsers() {
  stUser User;
  char Addmore = 'n';

  do{
    cout << "\nAdding New User :\n\n";

    AddNewUser();
    cout << "\nUser Added Successfully, Do you want to add more Users? y/n? ";
    cin >> Addmore;
  } while (toupper(Addmore) == 'Y');
}

bool DeleteClientByAccountNumber(string AccountNumber, vector<stClient>& VstClients){
  stClient Client;
  char Answer = 'n';

  if (FindClientByAccountNumber(AccountNumber, VstClients, Client))
  {
    PrintClientCard(Client);
    cout << "\n\nAre you sure do you want to delete this Client? y/n? ";
    cin >> Answer;

    if(toupper(Answer) == 'Y'){
      MarkClientForDeleteByAccountNumber(AccountNumber, VstClients);
      SaveClientDataToFile(FILENAME, VstClients);
      VstClients = LoadClientsDataFromFile(FILENAME);
      cout << "\nClient Deleted Successfully. ";
      return true;
    }
  }
  else {
    cout << "\nClient with Account Number [" << AccountNumber << "] is Not Found! ";
    return false;
  }
}

bool DeleteUserByUserName(string UserName, vector<stUser>& VstUsers){

  stUser User;
  char Answer = 'n';

  if(UserName == "Admin"){
      cout << "\n\nYou Cannot Delete This User. ";
      return false;
  }

  if(FindUserByUserName(UserName, VstUsers,User)){
    PrintUserCard(User);

    cout << "\n\nAre you sure do you want to delete this User> y/n? ";
    cin >> Answer;

    if(toupper(Answer) == 'Y'){
      MarkUserForDeleteByUserName(UserName, VstUsers);
      SaveUserDataToFile(FILENAMEU, VstUsers);
      VstUsers = LoadUsersDataFromFile(FILENAMEU);
      cout << "\nUser Deleted Successfully. ";
      return true;
    }
  }
  else 
  {
    cout << "\nClient with Account Number [" << UserName << "] is Not Found! ";
    return false;
  }
}

bool UpdateClientByAccountNumber(string AccountNumber, vector<stClient>& VstClients){

  stClient Client;
  char Answer = 'n';

  if(FindClientByAccountNumber(AccountNumber, VstClients,Client)){
    PrintClientCard(Client);

    cout << "\n\nAre you sure do you want to Update This Client? y/n? ";
    cin >> Answer;

    if(Answer == 'y' || Answer == 'Y'){
      for(stClient& C : VstClients){
        if(C.AccountNumber == AccountNumber)
        {
          C = ChangeClientRecord(AccountNumber);
          break;
        }
      }
      SaveClientDataToFile(FILENAME, VstClients);
      cout << "\nClient Updated Successfully. ";
      return true;
    }
  }
  else {
    cout << "\nClient with Account Number [" << AccountNumber << "] is Not Found! ";
    return false;
  }
}

bool UpdateUserByUserName(string UserName, vector<stUser>& VstUsers){

  stUser User;
  char Answer = 'n';

  if(FindUserByUserName(UserName, VstUsers,User)){
    PrintUserCard(User);

    cout << "\n\nAre you sure do you want to Update this User? y/n? ";
    cin >> Answer;

    if(toupper(Answer) == 'Y'){
      for(stUser& U: VstUsers){
        if(U.UserName == UserName){
          U = ChangeUserRecord(UserName);
          break;
        }
      }
      SaveUserDataToFile(FILENAMEU, VstUsers);
      cout << "\nUser Updated Successfully. ";
      return true;
    }
  }
  else {
    cout << "\nUser with User Name [" << UserName << "] is not Found! ";
    return false;
  }
}

bool DepositByAccountNumber(string AccountNumber, double Amount, vector<stClient>& VstClients){

  stClient Client;
  char Answer = 'n';


  cout << "\n\nAre you sure do you want to perform this transaction? Y/N? ";
  cin >> Answer;

  if(toupper(Answer) == 'Y')
  {
    for (stClient& C : VstClients)
    {
      if (C.AccountNumber == AccountNumber)
      {
        C.AccountBalance += Amount;
        SaveClientDataToFile(FILENAME, VstClients);
        cout << "\nNew Balance : Transactions Done Successfully. Your New Balance is : "
             << C.AccountBalance;
        return true;
      }
    }
    return false;
  }
  return false;
}

string ReadClientAccountNumber(){
  string AccountNumber = "";
  cout << "\nPlease Enter Account Number? ";
  cin >> AccountNumber;
  return AccountNumber;
}

string ReadUserUserName(){
  string UserName = "";
  cout << "\nPlease Enter User Name? ";
  cin >> UserName;
  return UserName;
}

void ShowAccesDeniedMessage() {
  cout << "\n--------------------------------------\n";
  cout << "\nAccess Denied, \nYou Do Not Have Permissions To Do This, \nPlease Contact Your Admin. ";
  cout << "\n--------------------------------------\n";

}
void ShowDeleteClientScreen(){

  if(!CheckAccessPermissions(enPermissions::pDeleteClient)){
    ShowAccesDeniedMessage();
    return;
  }

  cout << "\n---------------------------------------";
  cout << "\n\tDelete Client Screen ";
  cout << "\n---------------------------------------";

  vector<stClient> VstClients = LoadClientsDataFromFile(FILENAME);
  string AccountNumber = ReadClientAccountNumber();

  DeleteClientByAccountNumber(AccountNumber, VstClients);
}

void ShowAddNewClientScreen(){

  if(!CheckAccessPermissions(enPermissions::pAddNewClient)){
    ShowAccesDeniedMessage();
    return;
  }

  cout << "\n---------------------------------------";
  cout << "\n\tAdd New Client Screen ";
  cout << "\n---------------------------------------";

  AddNewClients();
}

void ShowUpdateClientScreen(){

  if(!CheckAccessPermissions(enPermissions::pUpdateClientInfo)){
    ShowAccesDeniedMessage();
    return;
  }

  cout << "\n---------------------------------------";
  cout << "\n\tUpadte Client's Info Screen ";
  cout << "\n---------------------------------------";

  vector<stClient> VstClients = LoadClientsDataFromFile(FILENAME);
  string AccountNumber = ReadClientAccountNumber();

  UpdateClientByAccountNumber(AccountNumber, VstClients);
}

void ShowFindClientScreen(){

  if(!CheckAccessPermissions(enPermissions::pFindClient)){
    ShowAccesDeniedMessage();
    return;
  }

  cout << "\n---------------------------------------";
  cout << "\n\tFind Client Screen ";
  cout << "\n---------------------------------------";

  vector<stClient> VstClients = LoadClientsDataFromFile(FILENAME);
  string AccountNumber = ReadClientAccountNumber();
  stClient Client;

  if(FindClientByAccountNumber(AccountNumber, VstClients,Client)){
    PrintClientCard(Client);
  }
  else {
    cout << "\nClient with Account Number [" << AccountNumber << "] is not Found! ";
  }
}

void ShowDepositScreen(){

  cout << "\n---------------------------------------";
  cout << "\n\tDeposit Screen";
  cout << "\n---------------------------------------";

  vector<stClient> VstClients = LoadClientsDataFromFile(FILENAME);
  string AccountNumber = ReadClientAccountNumber();
  stClient Client;

  while(!FindClientByAccountNumber(AccountNumber,VstClients,Client)){
    cout << "\nClient with Account Number [" << AccountNumber << "] does not exist. Please Enter another Account Number? ";
    AccountNumber = ReadClientAccountNumber();
  }
  PrintClientCard(Client);
  double Amount = 0;
  cout << "\nPlease Enter Deposit Amount? ";
  cin >> Amount;

  DepositByAccountNumber(AccountNumber, Amount, VstClients);
}

void ShowWithdrawScreen(){

  cout << "\n---------------------------------------";
  cout << "\n\tWithdraw Screen";
  cout << "\n---------------------------------------";

  vector<stClient> VstClients = LoadClientsDataFromFile(FILENAME);
  string AccountNumber;
  stClient Client;

  do {
    AccountNumber = ReadClientAccountNumber();
    if(!FindClientByAccountNumber(AccountNumber, VstClients, Client)){
      cout << "\nClient with Account Number [" << AccountNumber << "] does not exist!\n";
    }
  } while(!FindClientByAccountNumber(AccountNumber, VstClients, Client));

  PrintClientCard(Client);
  double Amount = 0;
  cout << "\nPlease Enter Withdraw Amount? ";
  cin >> Amount;

  while (Amount > Client.AccountBalance){
    cout << "\nAmount Exceeds The Account Balance, You can Withdraw up to : " << Client.AccountBalance;
    cout << "\nPlease Enter another Amount? ";
    cin >> Amount;
  }
  DepositByAccountNumber(AccountNumber, Amount * -1, VstClients);
  cout << "\nTransaction Done Successfully. ";
}  

void ShowAddNewUserScreen(){

  cout << "\n---------------------------------------";
  cout << "\n\tAdd New User Screen";
  cout << "\n---------------------------------------";

  AddNewUsers();
}

void ShowDeleteUserScreen(){

  cout << "\n---------------------------------------";
  cout << "\n\tDelete User Screen";
  cout << "\n---------------------------------------";

  vector<stUser> VstUsers = LoadUsersDataFromFile(FILENAMEU);
  string UserName = ReadUserUserName();

  DeleteUserByUserName(UserName, VstUsers);
}

void ShowUpdateUserSceen(){

  cout << "\n---------------------------------------";
  cout << "\n\tUpdate User's Info Screen";
  cout << "\n---------------------------------------";

  vector<stUser> VstUsers = LoadUsersDataFromFile(FILENAMEU);
  string UserName = ReadUserUserName();

  UpdateUserByUserName(UserName, VstUsers);
}

void ShowFindUserScreen(){

  cout << "\n---------------------------------------";
  cout << "\n\tFind User Screen";
  cout << "\n---------------------------------------";

  vector<stUser> VstUsers = LoadUsersDataFromFile(FILENAMEU);
  string UserName = ReadUserUserName();
  stUser User;

  if(FindUserByUserName(UserName,VstUsers,User)){
    PrintUserCard(User);
  }
  else {
    cout << "\nUser with User Name [" << UserName << "] is not Found! ";
  }
}

bool CheckAccessPermissions(enPermissions Permissions){

  if(CurrentUser.Permissions == enPermissions::eAll)
    return true;

  if((Permissions & CurrentUser.Permissions) == Permissions)
    return true;
  
  else
    return false;
}

void ShowEndScreen() {

  cout << "\n---------------------------------------";
  cout << "\n\tProgram Finished (:-";
  cout << "\n---------------------------------------";

}

void GoBackToMainMenu(){

  cout << "\nPress any key to go back to main menu... ";
  system("pause > 0");
  ShowMainMenuScreen();
}

void GoBackToTransactionsMainMenu(){

  cout << "\nPress any key to go back to main Menu... ";
  system("pause > 0");
  ShowTransactionsMainMenuScreen();
}

void GoBackToManageUserManu() {
  cout << "\nPress any key to go back to main menu... ";
  system("pause > 0");
  GoBackToMainMenu();
}


short ReadMainMenuOptions(){
  short choice = 0;
  cout << "\nChoose what do you want to do? [1 to 8]? ";
  cin >> choice;
  return choice;
}

short ReadTransactionMenuOptions(){
  short tchoice = 0;
  cout << "\nChoose what do you want to do? [1 to 4]? ";
  cin >> tchoice;
  return tchoice;
}

short ReadManageUsersOptions() {

  short mchoice = 0;
  cout << "\nChoose what do you want to do? [1 to 6]? ";
  cin >> mchoice;
  return mchoice;
}

void PerformMainMenu(enMainMenuOptions MenuOptions){

  switch (MenuOptions)
  {
    {
      case enMainMenuOptions::enListClient:
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenu();
        break;
      }
      case enMainMenuOptions::enAddClient:
        system("cls");
        ShowAddNewClientScreen();
        GoBackToMainMenu();
        break;

      case enMainMenuOptions::enDeleteClient:
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenu();
        break;  

      case enMainMenuOptions::enUpdateClient:
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenu();
        break;  

      case enMainMenuOptions::enFindClient:
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenu();
        break;  
      
      case enMainMenuOptions::enTrnsactions:
        system("cls");
        ShowTransactionsMainMenuScreen();
        break;  

      case enMainMenuOptions::enManageUsers:
        system("cls");
        ShowManageUserMainManu();
        break;

      case enMainMenuOptions::enLogout:
        system("cls");
        Login();
        break;
      }
}

void PerformTransactionsMenu(enTransactionsMainOptions TrnsactionsMenu){

  switch (TrnsactionsMenu)
  {
  {
    case enTransactionsMainOptions::tDeposit:
      system("cls");
      ShowDepositScreen();
      GoBackToTransactionsMainMenu();
      break;
  }

    case enTransactionsMainOptions::tWithdraw:
      system("cls");
      ShowWithdrawScreen();
      GoBackToTransactionsMainMenu();
      break;

    case enTransactionsMainOptions::tTotalBalances:
      system("cls");
      ShowAllClientTotalBalances();
      GoBackToTransactionsMainMenu();
      break;  

    case enTransactionsMainOptions::tMainMenu:
      ShowMainMenuScreen();
    }
}

void PerformManageUsersMainMenu(enManageUsersOptions ManageUsers){

  switch (ManageUsers)
  {
  {
    case enManageUsersOptions::enListUsers:
      system("cls");
      ShowAllUsersList();
      GoBackToManageUserManu();
      break;
    }

    case enManageUsersOptions::enAddNewUser:
      system("cls");
      ShowAddNewUserScreen();
      GoBackToManageUserManu();
      break;

    case enManageUsersOptions::enDeleteUser:
      system("cls");
      ShowDeleteUserScreen();
      GoBackToManageUserManu();
      break;  

    case enManageUsersOptions::enUpdateUser:
      system("cls");
      ShowUpdateUserSceen();
      GoBackToManageUserManu();
      break;  

    case enManageUsersOptions::enFindUser:
      system("cls");
      ShowFindUserScreen();
      GoBackToManageUserManu();
      break;  

    case enManageUsersOptions::enMainMenu:
      GoBackToManageUserManu();
      
  }
}

void ShowMainMenuScreen(){

  system("cls");
  cout << "\n=======================================";
  cout << "\n\t  Main Menu Screen ";
  cout << "\n=======================================";
  cout << "\n\t[1] Show Client List. ";
  cout << "\n\t[2] Add New Client. ";
  cout << "\n\t[3] Delete Client. ";
  cout << "\n\t[4] Update Client's Info. ";
  cout << "\n\t[5] Find Client. ";
  cout << "\n\t[6] Transactions. ";
  cout << "\n\t[7] Manage Users. ";
  cout << "\n\t[8] Logout. ";
  cout << "\n=======================================";

  PerformMainMenu((enMainMenuOptions)ReadMainMenuOptions());
}

void ShowTransactionsMainMenuScreen()
{
  if(!CheckAccessPermissions(enPermissions::pTransactions)){
    ShowAccesDeniedMessage();
    GoBackToMainMenu();
    return;
  }
  system("cls");
  cout << "\n=======================================";
  cout << "\n\t  Transactions Menu Screen ";
  cout << "\n=======================================";
  cout << "\n\t[1] Deposit. ";
  cout << "\n\t[2] Withdraw. ";
  cout << "\n\t[3] Total Balances. ";
  cout << "\n\t[4] Main Menu. ";
  cout << "\n=======================================";

  PerformTransactionsMenu((enTransactionsMainOptions)ReadTransactionMenuOptions());
}

void ShowManageUserMainManu(){

  if(!CheckAccessPermissions(enPermissions::pManageUsers)){
    ShowAccesDeniedMessage();
    GoBackToMainMenu();
    return;
  }

  system("cls");
  cout << "\n=======================================";
  cout << "\n\t  Manage Users Menu Screen ";
  cout << "\n=======================================";
  cout << "\n\t[1] List Users. ";
  cout << "\n\t[2] Add New User. ";
  cout << "\n\t[3] Delete User. ";
  cout << "\n\t[4] Update User's Info. ";
  cout << "\n\t[5] Find User. ";
  cout << "\n\t[6] Main Menu. ";
  cout << "\n=======================================";

  PerformManageUsersMainMenu((enManageUsersOptions)ReadManageUsersOptions());
}

bool LoadUserInfo(string UserName, string Password){

  if(FindUserByUserNameAndPassword(UserName, Password, CurrentUser))
    return true;
  else
    return false;
}

void Login(){

  bool LoginFaild = false;
  string UserName, Password;

  do{
    system("cls");
    cout << "\n---------------------------------------";
    cout << "\n\tLogin Screen ";
    cout << "\n---------------------------------------";

    if(LoginFaild){
      cout << "\nInvalid UserName/Password!\n ";
    }

    cout << "\nPlease Enter User Name? ";
    cin >> UserName;

    cout << "\nPlease Enter Password? ";
    cin >> Password;

    LoginFaild = !LoadUserInfo(UserName, Password);

  } while (LoginFaild);

  ShowMainMenuScreen();
}

int main()
{
  Login();

  system("pause>0");
  return 0;
}