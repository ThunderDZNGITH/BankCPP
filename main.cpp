/**
|---------------------------------------------------------------------------------------------------|
|											UnSafe BANK SYSTEM										|
|												  v1.0.0											|
|											  ThunderDZNGITH										|
|											29/10/2024 - 16h43										|
|---------------------------------------------------------------------------------------------------|
/**

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime> 
#include <sstream>
#include <vector>

using namespace std;

struct account {
	long long id;
	int passcode;
	int amount;
};

enum transactionType {
	deposit = 1,
	withdraw = 2,
	transfer = 3
};

struct transaction {
	long long id;
	long long fromId;
	long long toId;
	transactionType type;
	int amount;
};

unsigned short int choice = 0;
bool doExit = false;

account actualAccount;

const string ACCOUNTDATABASE("ACCOUNTDATABASE.txt");
const string TRANSACTIONDATABASE("TRANSACTIONDATABASE.txt");

enum menuChoice {
    createAccount = 1,
	connectAccount =2,
    dropMoney = 3,
    takeMoney = 4,
    transferMoney = 5,
    checkAmount = 6,
    checkHistory = 7,
    closeAccount = 8,
    close = 9
};

void saveAccount(account acc) {
    std::ifstream iFile(ACCOUNTDATABASE);
    std::ofstream oFile("temp.txt");
    std::string line;
    bool found = false;

    // Vecteur pour stocker les lignes du fichier
    std::vector<std::string> lines;

    // Lire toutes les lignes du fichier existant
    while (getline(iFile, line)) {
        lines.push_back(line);
    }

    iFile.close();

    // Parcourir les lignes et vérifier si l'ID existe déjà
    for (size_t i = 0; i < lines.size(); ++i) {
        if (lines[i] == std::to_string(acc.id)) {
            // Écraser les trois lignes si l'ID est trouvé
            oFile << acc.id << std::endl;
            oFile << acc.id << "+P+" << acc.passcode << std::endl;
            oFile << acc.id << "+A+" << acc.amount << std::endl;
            found = true;
            i += 2; // Sauter les deux lignes suivantes qui correspondent à l'ancienne entrée
        } else {
            oFile << lines[i] << std::endl;
        }
    }

    // Si l'ID n'est pas trouvé, ajouter les informations à la fin
    if (!found) {
        oFile << acc.id << std::endl;
        oFile << acc.id << "+P+" << acc.passcode << std::endl;
        oFile << acc.id << "+A+" << acc.amount << std::endl;
    }

    oFile.close();

    // Remplacer le fichier original par le fichier temporaire
    remove("ACCOUNTDATABASE.txt");
    rename("temp.txt", "ACCOUNTDATABASE.txt");
}

void displayMenu() {
	cout << "Votre compte est actuellement le : " << actualAccount.id << endl;
	cout << "Vous avez : " << actualAccount.amount << "€" << endl;
    cout << "---------- MENU ----------" << endl;
    cout << "1. Créer un compte" << endl;
	cout << "2. Connecter un compte" << endl;
    cout << "3. Déposer de l'argent" << endl;
    cout << "4. Retirer de l'argent" << endl;
    cout << "5. Transférer de l'argent" << endl;
    cout << "6. Consulter le solde" << endl;
    cout << "7. Consulter l'historique des transactions" << endl;
    cout << "8. Fermer un compte" << endl;
    cout << "9. Quitter" << endl;
    cout << "--------------------------" << endl;
    cout << "" << endl;
    cout << "Quel est votre choix ?" << endl;
}

long long generateBankId() {
    long long base = 8800000000000000;
    long long randomPart = rand() % 100000000000000;
    return base + randomPart;
}

long long generateTransactionId() {
    long long base = 4444000000000000;
    long long randomPart = rand() % 100000000000000;
    return base + randomPart;
}

void createAccountFunc() {
	int passcode;
	int amount;
	account acc;
	cout << "Quel est votre passcode ?" << endl;
	cin >> passcode;
	acc.passcode = passcode;
	cout << "Quel est votre solde de départ ?" << endl;
	cin >> amount;
	acc.amount = amount;
	
	srand(time(0));
	acc.id = generateBankId();
	
	saveAccount(acc);
	cout << "Votre compte est le : " << acc.id << " | Avec le code : " << passcode << endl;
}

void connectAccountFunc() {
	long long id = 0;
	string passcode;
	
	bool accountFind = false;
	bool passcodeCorrect = false;
	
	ifstream iFile;
	iFile.open(ACCOUNTDATABASE);
	
	cout << "Quel est l'id de votre compte ?" << endl;
	cin >> id;
	
	string lineid;
	string linepasscode;
	while(getline(iFile, lineid)) {
		if(lineid.find(id)) {
			accountFind = true;
			cout << "Compte trouvé ! " << endl;
			break;
		} else {
			cout << "Compte introuvable !" << endl;	
		}
	}
	iFile.close();
	iFile.open(ACCOUNTDATABASE);
	if(accountFind) {
		cout << "Quel est votre passcode ?" << endl;
		cin >> passcode;
		while(getline(iFile, linepasscode)) {
			if(linepasscode == to_string(id)+"+P+"+passcode) {
				passcodeCorrect = true;
				break;
			}
		}
	}
	if(passcodeCorrect) {
		actualAccount.id = id;
		cout << "Vous êtes connécté au compte : " << actualAccount.id << endl;
		actualAccount.passcode = stoi(passcode);
		
		string lineAmount = to_string(id)+"+A+";
		unsigned int currentAmount = 0;
	
		string line;
		while(getline(iFile, line)) {
			if(line.find(lineAmount) == 0) {
				size_t pos = line.find_last_of('+');
				if (pos != string::npos) {
					string number = line.substr(pos + 1);
					currentAmount = stoi(number);
					actualAccount.amount = currentAmount;
				}
				break;
			}
		}
	} else {
		cout << "Passcode incorrect." << endl;	
	}
		iFile.close();
}

void generateTransaction(transaction tra) {
	ofstream oFile(TRANSACTIONDATABASE, ios::app);
	
	srand(time(0));
	tra.id = generateTransactionId();
	
	string type;
	switch(tra.type){
		case 1:
			type = "deposit";
			break;
		case 2:
			type = "withdrawal";
			break;
		case 3:
			type = "transfer";
			break;
	}
	
	oFile << tra.id << " | From : " << tra.fromId << " To : " << tra.toId << " | " << type << " | " << tra.amount  << "€" << endl;
	
	oFile.close();
}

void addMoney(int amount, account acc) {
	ifstream iFile(ACCOUNTDATABASE);
	
	string lineAmount = to_string(acc.id)+"+A+";
	unsigned int currentAmount = 0;
	
	string line;
	while(getline(iFile, line)) {
		if(line.find(lineAmount) == 0) {
			size_t pos = line.find_last_of('+');
    		if (pos != string::npos) {
        		string number = line.substr(pos + 1);
				currentAmount = stoi(number);
				acc.amount = currentAmount+amount;
			}
			break;
		}
	}
	
	saveAccount(acc);
}

void subMoney(int amount, account acc) {
	ifstream iFile(ACCOUNTDATABASE);
	
	string lineAmount = to_string(acc.id)+"+A+";
	unsigned int currentAmount = 0;
	
	string line;
	while(getline(iFile, line)) {
		if(line.find(lineAmount) == 0) {
			size_t pos = line.find_last_of('+');
    		if (pos != string::npos) {
        		string number = line.substr(pos + 1);
				currentAmount = stoi(number);
				acc.amount = currentAmount-amount;
			}
			break;
		}
	}
	
	saveAccount(acc);
}

void dropMoneyFunc() {
	transaction tra;
	
	int amount = 0;
	if(actualAccount.id == 0) {
		cout << "Connectez vous à un compte d'abord." << endl;	
	} else {
		cout << "Combien voulez vous déposer ?" << endl;
		cin >> amount;
		addMoney(amount, actualAccount);
		actualAccount.amount=actualAccount.amount+amount;
		cout << "Votre compte à été créditer de " << amount << "€, vous avez donc " << actualAccount.amount << "€" << endl;
	}
	tra.fromId = actualAccount.id;
	tra.toId = actualAccount.id;
	tra.type = deposit;
	tra.amount = amount;
	generateTransaction(tra);
}

void takeMoneyFunc() {
	transaction tra;
	
	int amount = 0;
	if(actualAccount.id == 0) {
		cout << "Connectez vous à un compte d'abord." << endl;	
	} else {
		cout << "Combien voulez vous retirer ?" << endl;
		cin >> amount;
		subMoney(amount, actualAccount);
		actualAccount.amount=actualAccount.amount-amount;
		cout << "Vous avez bien retirer " << amount << "€, vous avez donc " << actualAccount.amount << "€" << endl;
	}
	tra.fromId = actualAccount.id;
	tra.toId = actualAccount.id;
	tra.type = withdraw;
	tra.amount = amount;
	generateTransaction(tra);
}

void checkAmountFunc() {
	if(actualAccount.id == 0) {
		cout << "Connectez vous à un compte d'abord." << endl;
	} else {
		cout << "Vous avez actuellement " << actualAccount.amount << "€" << endl;	
	}
}

account getAccountFromId(long long id) {
	account acc;
	
	ifstream iFile(ACCOUNTDATABASE);
	
	acc.id = id;
	
	string linePasscode = to_string(id)+"+P+";
		unsigned int currentPasscode = 0;
	
		string line;
		while(getline(iFile, line)) {
			if(line.find(linePasscode) == 0) {
				size_t pos = line.find_last_of('+');
				if (pos != string::npos) {
					string number = line.substr(pos + 1);
					currentPasscode = stoi(number);
					acc.passcode = currentPasscode;
				}
				break;
			}
		}
	
	string lineAmount = to_string(id)+"+A+";
		unsigned int currentAmount = 0;
	
		string line1;
		while(getline(iFile, line1)) {
			if(line1.find(lineAmount) == 0) {
				size_t pos = line1.find_last_of('+');
				if (pos != string::npos) {
					string number = line1.substr(pos + 1);
					currentAmount = stoi(number);
					acc.amount = currentAmount;
				}
				break;
			}
		}
	
	return acc;	
}

void transferMoneyFunc() {
	transaction tra;
	long long toId;
	int amount;
	if(actualAccount.id == 0) {
		cout << "Connectez vous à un compte d'abord." << endl;	
	} else {
		cout << "Vers quel compte voulez-vous transferer l'argent ?" << endl;
		cin >> toId;
		cout << "De combien est le transfert ?" << endl;
		cin >> amount;
		
		actualAccount.amount=actualAccount.amount-amount;
		addMoney(amount, getAccountFromId(toId));
		subMoney(amount, actualAccount);
	}
	
	tra.fromId = actualAccount.id;
	tra.toId = toId;
	tra.type = transfer;
	tra.amount = amount;
	generateTransaction(tra);
	
}

void getChoice() {
	cin >> choice;
	switch(choice) {
		case createAccount:
			createAccountFunc();
			break;

		case connectAccount:
			connectAccountFunc();
			break;
			
		case dropMoney:
			dropMoneyFunc();
			break;

		case takeMoney:
			takeMoneyFunc();
			break;

		case transferMoney:
			transferMoneyFunc();
			break;

		case checkAmount:
			checkAmountFunc();
			break;

		case checkHistory:
			break;

		case closeAccount:
			break;

		case close:
			doExit = true;
			break;

		default:
			cerr << "Choix incorrect." << endl;
	}
}

int main()
{
	while(!doExit) {
		  /**#ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif**/
		displayMenu();
		getChoice();
	}
	return 0;
}
