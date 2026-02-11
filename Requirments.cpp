#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <cctype>

using namespace std;

const string FILENAME = "Clients.Txt";

struct stClient{
  string AccountNumber;
  string PinCode;
  string Name;
  string Phone;
  double Balance;
  bool MarkForDelete = false;
};

stClient CurrentClient;

enum enMainMenu
{
  enQuickWithdraw = 1,
  enNoramlQuickWithdraw = 2,
  enDeposit = 3,
  enCheckBalance = 4,
  enLogout = 5

};

void ShowQuickWithdrawMenu();
void GoBackToMainMenu();
void PerformNormalWithdrawMenu();
void ShowmMainMenuScreen();
void Login();

vector<string>SplitString(string S1, string Delim){
  vector<string> VstClients;
  short pos = 0;
  string word;

  while((pos = S1.find(Delim))!= std::string::npos){
    word = S1.substr(0, pos);
    if(!word.empty()){
      VstClients.push_back(word);
    }
    S1.erase(0, pos + Delim.length());
  }
  if(!S1.empty()){
    VstClients.push_back(S1);
  }
  return VstClients;
}

stClient ConvertClientDataLineToRecord(string Line, string Seperator = "#//#"){

  vector<string> VstClient = SplitString(Line, Seperator);
  stClient Client;

  Client.AccountNumber = VstClient[0];
  Client.PinCode = VstClient[1];
  Client.Name = VstClient[2];
  Client.Phone = VstClient[3];
  Client.Balance = stod(VstClient[4]);

  return Client;
}

string ConvertClientRecordToLine(stClient Client, string Seperator = "#//#"){
  string stRecordDataline = "";

  stRecordDataline += Client.AccountNumber + Seperator;
  stRecordDataline += Client.PinCode + Seperator;
  stRecordDataline += Client.Name + Seperator;
  stRecordDataline += Client.Phone + Seperator;
  stRecordDataline += to_string(Client.Balance);

  return stRecordDataline;
}

vector<stClient> LoadClientDataFromFile(string FILENAME){

  vector<stClient> VstClients;
  fstream MyFile;
  MyFile.open(FILENAME, ios ::in);

  if(MyFile.is_open()){

    string Line;
    stClient Client;

    while(getline(MyFile, Line)){
      Client = ConvertClientDataLineToRecord(Line);
      VstClients.push_back(Client);
    }
    MyFile.close();
  }
  return VstClients;
}

vector<stClient> SaveClientDataToFile(string FILENAME, vector<stClient> VstClients){

  fstream MyFile;
  MyFile.open(FILENAME, ios ::out);

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

bool DepositByAccountNumber(string AccountNumber, double Amount, vector<stClient> VstClients){

  char Answer = 'n';

  cout << "\n\nAre you sure do you want to perform this transaction? y/n? ";
  cin >> Answer;

  if(toupper(Answer) == 'Y'){
    for(stClient& C : VstClients){
      if(C.AccountNumber == AccountNumber){
        C.Balance += Amount;
        SaveClientDataToFile(FILENAME, VstClients);
        cout << "\nDone Successfully. New Balance is : " << C.Balance;
        return true;
      }
    }
  }
  return false;
}

double ReadDepositAmount(){

  double Amount;

  cout << "\nPlease Enter a Positive Deposit Amount? ";
  cin >> Amount;

  while(Amount <= 0){
    cout << "\nPlease Enter a Positive Deposit Amount? ";
    cin >> Amount;
  }
  return Amount;
}

void ShowDepositScreen(){

  cout << "\n=======================================";
  cout << "\n\tDeposit Screen ";
  cout << "\n=======================================";

  double Amount = ReadDepositAmount();
  vector<stClient> VstClients = LoadClientDataFromFile(FILENAME);
  if (DepositByAccountNumber(CurrentClient.AccountNumber, Amount, VstClients)) {
    CurrentClient.Balance += Amount;
  }
}

void ShowWithdrawScreen()
{
  char Answer = 'n';
  double Amount = 0;

  cout << "\nPlease Enter a Withdraw Amount? ";
  cin >> Amount;

  while (Amount <= 0 || Amount > CurrentClient.Balance) {
    cout << "\nAmount exceeds the Balance, You can withdraw up to : " << CurrentClient.Balance;
    cout << "\nPlease Enter another Amount? ";
    cin >> Amount;
  }

  vector<stClient> VstClients = LoadClientDataFromFile(FILENAME);
  cout << "\n\nAre you sure do you want to perform this transaction? y/n? ";
  cin >> Answer;

  if(toupper(Answer) == 'Y'){
    if (DepositByAccountNumber(CurrentClient.AccountNumber, Amount * -1, VstClients)) {
      CurrentClient.Balance -= Amount;
      cout << "\nDone Successfully, New Balance is :" << CurrentClient.Balance << endl;
    }
  }
}

int ReadNormalWithdrawAmount(){

  int Amount = 0;
  cout << "\nPlease Enter an Amount Multiple of 5's.";
  cin >> Amount;

  while(Amount % 5 != 0){
    cout << "\nEnter an amount multiple of 5's. ";
    cin >> Amount;
  }
  return Amount;
}

void ShowNormalWithdawScreen(){

  cout << "\n=======================================";
  cout << "\n\tNormal Withdaw Screen ";
  cout << "\n=======================================";
  PerformNormalWithdrawMenu();
}

void PerformNormalWithdrawMenu(){

  int WithrawBalance = ReadNormalWithdrawAmount();

  if(WithrawBalance > CurrentClient.Balance){
    cout << "\nThe Amount Exceeds Your Balance, Make another choice? ";
    cout << "\nPress Any Key To Continue...";
    system("pause>0");
    ShowNormalWithdawScreen();
    return;
  }
  vector<stClient> VstClients = LoadClientDataFromFile(FILENAME);
  if (DepositByAccountNumber(CurrentClient.AccountNumber, WithrawBalance * -1, VstClients)) {
    CurrentClient.Balance -= WithrawBalance;
  }
}
void ShowCheckBalanceScreen(){
  system("cls");
  cout << "\n=======================================";
  cout << "\n\tCheck Balance Screen ";
  cout << "\n=======================================";
  cout << "\nYour Balance is " << CurrentClient.Balance;

  GoBackToMainMenu();
}
void GoBackToMainMenu(){

  system("cls");
  cout << "\nPlease Press any key to Go Back To Main Menu... ";
  system("pause > 0");
  ShowmMainMenuScreen();
}

short ReadQuickWithdrawOptions(){

  short qchoice = 0;
  while(qchoice < 1 || qchoice > 9)
 { 
  cout << "\nChoose what to Withdraw from [1] to [9] ";
  cin >> qchoice;
 }
  return qchoice;
}

short ReadMainMenuOptions(){
  short mchoice = 0;
  cout << "\nChoose what do you want to do? [1 to 5]? ";
  cin >> mchoice;
  return mchoice;
}
short GetQuickWithdrawAmount(short QuickWithdrawOptions){

  switch (QuickWithdrawOptions)
  {
    case 1:
      return 20;

    case 2:
      return 50;

    case 3:
      return 100;

    case 4:
      return 200;

    case 5:
      return 400;

    case 6:
      return 600;

    case 7:
      return 800;

    case 8:
      return 1000;

    default:
      return 0;
    }
}

void PerformQuickWithdrawMenueOptions(short QuickWithdrawOptions){

  if(QuickWithdrawOptions == 9)
    return;

  short WithdrawBalance = GetQuickWithdrawAmount(QuickWithdrawOptions);

  if(WithdrawBalance > CurrentClient.Balance){
    cout << "\nThe Amount Exceeds Your Balance, Make another choice? ";
    cout << "\nPress Any Key To Continue...";
    system("pause>0");
    ShowQuickWithdrawMenu();
    return;
  }

  vector<stClient> VstClients = LoadClientDataFromFile(FILENAME);
  if (DepositByAccountNumber(CurrentClient.AccountNumber, WithdrawBalance * -1, VstClients)) {
    CurrentClient.Balance -= WithdrawBalance;
  }
}

void ShowQuickWithdrawMenu(){

  system("cls");
  cout << "\n=========================================";
  cout << "\n\tQuick Withdraw ";
  cout << "\n=========================================";
  cout << "\n\t[1]  20 " << "\t\t[2] 50 ";
  cout << "\n\t[3] 100 " << "\t\t[4] 200 ";
  cout << "\n\t[5] 400 " << "\t\t[6] 600 ";
  cout << "\n\t[7] 800 " << "\t\t[8] 1000 ";
  cout << "\n\t[9] Exit ";
  cout << "\n=========================================";
  cout << "\n\nYour Balance is " << CurrentClient.Balance;

  PerformQuickWithdrawMenueOptions(ReadQuickWithdrawOptions());
}

void PerformMainMenu(enMainMenu MainMenuOptions){

  switch (MainMenuOptions)
  {
    case enMainMenu::enQuickWithdraw:
      system("cls");
      ShowQuickWithdrawMenu();
      GoBackToMainMenu();
      break;

    case enMainMenu::enNoramlQuickWithdraw:
      system("cls");
      ShowNormalWithdawScreen();
      GoBackToMainMenu();
      break;

    case enMainMenu::enDeposit:
      system("cls");
      ShowDepositScreen();
      GoBackToMainMenu();
      break;

    case enMainMenu::enCheckBalance:
      system("cls");
      ShowCheckBalanceScreen();
      break;

    case enMainMenu::enLogout:
      system("cls");
      Login();
      break;
  }
}

void ShowmMainMenuScreen(){

  system("cls");
  cout << "\n=========================================";
  cout << "\n\tATM Main Menu Screen ";
  cout << "\n=========================================";
  cout << "\n\t[1] Quick Withdraw. ";
  cout << "\n\t[2] Normal Withdraw. ";
  cout << "\n\t[3] Deposit. ";
  cout << "\n\t[4] Check Balance. ";
  cout << "\n\t[5] Logout. ";
  cout << "\n=========================================";

  PerformMainMenu((enMainMenu)ReadMainMenuOptions());
}

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string Pincode, stClient& Client){

  vector<stClient> VstClients = LoadClientDataFromFile(FILENAME);

  for (stClient C : VstClients){
    if(C.AccountNumber == AccountNumber && C.PinCode == Pincode){
      Client = C;
      return true;
    }
  }
  return false;
}

bool LoadClientAccountNumberAndPinCode(string AccountNumber, string PinCode){
  stClient User;

  if(FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, User)){
    CurrentClient = User;
    return true;
  } else {
    return false;
  }
}

void Login(){

  bool LoginFaild = false;
  string AccountNumber, PinCode;


  do {
    cout << "\n=====================================";
    cout << "\n\tLogin Screen ";
    cout << "\n=====================================";

    if(LoginFaild)
    {
      cout << "\nInvalid Account Number/Pin Code!\n ";
    }

      cout << "\nPlease Enter Account Number? ";
      cin >> AccountNumber;

      cout << "\nPlease Enter Pin Code? ";
      cin >> PinCode;
      LoginFaild = !LoadClientAccountNumberAndPinCode(AccountNumber, PinCode);
  } while (LoginFaild);
  ShowmMainMenuScreen();
}

int main()
{
  Login();

  system("pause>0");
  return 0;
}