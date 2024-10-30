/**
|-----------------------------------------------------------------------------------------------|
|					UnSafe BANK SYSTEM					|
|					      v1.0.0						|
|					  ThunderDZNGITH					|
|					29/10/2024 - 16h43					|
|-----------------------------------------------------------------------------------------------|
**/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime> 
#include <sstream>
#include <vector>

using namespace std;
/**
	account (struct)
	Représente un compte bancaire avec son id unique, son passcode, et son solde.
**/
struct account {
	long long id; 													// Id unique en 16 chiffres, ex : 8800 0000 1234 5678.
	int passcode; 													// Passcode en 4 chiffres.
	int amount; 													// Solde.
};

/**
	transactionType (enum)
	Énumération du type de transaction
**/
enum transactionType {
	deposit = 1, 													// Dépot.
	withdraw = 2, 													// Retrait.
	transfer = 3													// Transfert.
};

/**
	transaction (struct)
	Représente une transaction bancaire avec son id unique, l'id de l'émetteur, l'id du récepteur, le type de transaction et son montant.
**/
struct transaction {
	long long id;													// Id unique de la transaction en 16 chiffres, ex : 4444 0000 1234 5678.
	long long fromId;												// Id de l'émetteur.
	long long toId;													// Id du récepteur.
	transactionType type;												// Type de transaction (deposit, withdraw, transfer).
	int amount;													// Montant.
};

unsigned short int choice = 0;												// Choix du menu.
bool doExit = false;

account actualAccount; 													// Compte local actuel.

const string ACCOUNTDATABASE("ACCOUNTDATABASE.txt");									// Base de données des comptes.
const string TRANSACTIONDATABASE("TRANSACTIONDATABASE.txt");								// Base de données des transactions.

/**
	menuChoice (enum)
	Énumération des actions du menu.
**/
enum menuChoice {
    createAccount = 1,													// Créer un compte.
	connectAccount =2,												// Connecter un compte.
    dropMoney = 3,													// Déposer de l'argent.
    takeMoney = 4,													// Retirer de l'argent.
    transferMoney = 5,													// Transferer de l'argent vers un autre compte.
    checkAmount = 6,													// Vérifier le solde du compte.
    checkHistory = 7,													// Vérifier l'histroique des transactions du compte.
    closeAccount = 8,													// Fermer un compte.
    close = 9														// Terminer le programme.
};

/**
	saveAccount (void)
	Paramètre : acc (account)
	Enregistrer les paramètres d'un compte dans la base de données.
**/
void saveAccount(account acc) {
    std::ifstream iFile(ACCOUNTDATABASE);
    std::ofstream oFile("temp.txt");
    std::string line;
    bool found = false;
	
    std::vector<std::string> lines;											// Vecteur pour stocker les lignes du fichier.

    while (getline(iFile, line)) {    											// Lire toutes les lignes du fichier existant.
        lines.push_back(line);
    }

    iFile.close();

    for (size_t i = 0; i < lines.size(); ++i) {										// Parcourir les lignes et vérifier si l'ID existe déjà.
        if (lines[i] == std::to_string(acc.id)) {
															// Écraser les trois lignes si l'ID est trouvé.
            oFile << acc.id << std::endl;
            oFile << acc.id << "+P+" << acc.passcode << std::endl;
            oFile << acc.id << "+A+" << acc.amount << std::endl;
            found = true;
            i += 2; 													// Sauter les deux lignes suivantes qui correspondent à l'ancienne entrée.
        } else {
            oFile << lines[i] << std::endl;
        }
    }

    if (!found) { 													// Si l'ID n'est pas trouvé, ajouter les informations à la fin.
        oFile << acc.id << std::endl;
        oFile << acc.id << "+P+" << acc.passcode << std::endl;
        oFile << acc.id << "+A+" << acc.amount << std::endl;
    }

    oFile.close();

															// Remplacer le fichier original par le fichier temporaire.
    remove("ACCOUNTDATABASE.txt");
    rename("temp.txt", "ACCOUNTDATABASE.txt");
}

/**
	displayMenu (void)
	Paramètre:
	Afficher le menu de navigation.
**/
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

/**
	generateBankId (long long)
	Paramètre:
	Génère un ID bancaire unique sous le format 88XXXXXXXXXXXXXX.
**/
long long generateBankId() {
    long long base = 8800000000000000;
    long long randomPart = rand() % 100000000000000;
    return base + randomPart;
}

/**
	generateTransactionId (long long)
	Paramètre:
	Génère un ID de transaction unique sous le format 4444XXXXXXXXXXXX.
**/
long long generateTransactionId() {
    long long base = 4444000000000000;
    long long randomPart = rand() % 100000000000000;
    return base + randomPart;
}

/**
	createAccountFunc (void)
	Paramètre:
	Prompt de création de compte.
**/
void createAccountFunc() {
	int passcode;
	int amount;
	account acc;
	cout << "Quel est votre passcode ?" << endl;									//	Demande du passcode désirer.
	cin >> passcode;
	acc.passcode = passcode;
	cout << "Quel est votre solde de départ ?" << endl;								//	Demande du solde de départ.
	cin >> amount;
	acc.amount = amount;
	
	srand(time(0));
	acc.id = generateBankId();											//	Génèration d'un ID bancaire unique.
	
	saveAccount(acc);												//	Sauvegarde des paramètres du compte.
	cout << "Votre compte est le : " << acc.id << " | Avec le code : " << passcode << endl;
}

/**
	connectAccountFunc (void)
	Paramètre:
	Prompt de connexion au compte.
**/
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
	while(getline(iFile, lineid)) {											//	Vérification de l'existance du compte.
		if(lineid.find(id)) {
			accountFind = true;
			cout << "Compte trouvé ! " << endl;
			break;
		} else {
			cout << "Compte introuvable !" << endl;	
		}
	}
	
	if(accountFind) {												//	Si le compte est trouvé, vérifier la correspondance du passcode.
		cout << "Quel est votre passcode ?" << endl;
		cin >> passcode;
		while(getline(iFile, linepasscode)) {
			if(linepasscode == to_string(id)+"+P+"+passcode) {
				passcodeCorrect = true;
				break;
			}
		}
	}
	
	if(passcodeCorrect) {												//	Si le passcode est correct, retrouver le solde (amount) du compte.
		actualAccount.id = id;											//	Définir l'id de actualAccount à l'id du compte en cours de connexion.
		cout << "Vous êtes connécté au compte : " << actualAccount.id << endl;
		actualAccount.passcode = stoi(passcode);								//	Définir le passcode de actualAccount au passcode du compte en cours de connexion.
		
		string lineAmount = to_string(id)+"+A+";
		unsigned int currentAmount = 0;
	
		string line;
		while(getline(iFile, line)) {
			if(line.find(lineAmount) == 0) {
				size_t pos = line.find_last_of('+');
				if (pos != string::npos) {
					string number = line.substr(pos + 1);
					currentAmount = stoi(number);
					actualAccount.amount = currentAmount;						//	Définir le solde (amount) de actualAccount au solde du compte en cours de connexion.
				}
				break;
			}
		}
	} else {
		cout << "Passcode incorrect." << endl;	
	}
		iFile.close();
}

/**
	generateTransaction (void)
	Paramètre: tra (transaction)
	Génère une trace d'une transaction dans TRANSACTIONDATABASE.
**/
void generateTransaction(transaction tra) {
	ofstream oFile(TRANSACTIONDATABASE, ios::app);
	
	srand(time(0));
	tra.id = generateTransactionId();										//	Génèrer un id de transaction.
	
	string type;
	switch(tra.type){												//	En fonction du type de transaction, inscrire son label (1 -> deposit, 2 -> withdrawal, 3 -> transfer).
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
	
															//	Inscription de la transaction dans la TRANSACTIONDATABASE.
	oFile << tra.id << " | From : " << tra.fromId << " To : " << tra.toId << " | " << type << " | " << tra.amount  << "€" << endl;
	
	oFile.close();
}

/**
	addMoney (void)
	Paramètres: amount (int), acc (account)
	Ajoute 'amount' d'argent sur le compte 'acc'.
**/
void addMoney(int amount, account acc) {
	ifstream iFile(ACCOUNTDATABASE);
	
	string lineAmount = to_string(acc.id)+"+A+";									//	Recherche du solde du compte 'acc'.
	unsigned int currentAmount = 0;
	
	string line;
	while(getline(iFile, line)) {
		if(line.find(lineAmount) == 0) {
			size_t pos = line.find_last_of('+');
    		if (pos != string::npos) {
        		string number = line.substr(pos + 1);
				currentAmount = stoi(number);
				acc.amount = currentAmount+amount;							// Ajouter 'amount' au solde de 'acc'
			}
			break;
		}
	}
	
	saveAccount(acc);												//	Enregistrer les paramètres du compte.
}

/**
	subMoney (void)
	Paramètres: amount (int), acc (account)
	Retire 'amount' d'argent sur le compte 'acc'.
**/
void subMoney(int amount, account acc) {
	ifstream iFile(ACCOUNTDATABASE);
	
	string lineAmount = to_string(acc.id)+"+A+";									//	Recerche du solde du compte 'acc'.
	unsigned int currentAmount = 0;
	
	string line;
	while(getline(iFile, line)) {
		if(line.find(lineAmount) == 0) {
			size_t pos = line.find_last_of('+');
    		if (pos != string::npos) {
        		string number = line.substr(pos + 1);
				currentAmount = stoi(number);
				acc.amount = currentAmount-amount;							//	Retirer 'amount' du solde de 'acc'.
			}
			break;
		}
	}
	
	saveAccount(acc);												//	Enregistrer les paramètres du compte.
}

/**
	dropMoneyFunc (void)
	Paramètre:
	Prompt de dépot d'argent.
**/
void dropMoneyFunc() {
	transaction tra;
	
	int amount = 0;
	if(actualAccount.id == 0) {											//	Si aucun compte connecté : Erreur.
		cout << "Connectez vous à un compte d'abord." << endl;		
	} else {													//	Sinon déposer l'argent.
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
	generateTransaction(tra);											//	Génèrer une transaction.
}

/**
	takeMoneyFunc (void)
	Paramètre:
	Prompt de retrait d'argent.
**/
void takeMoneyFunc() {
	transaction tra;
	
	int amount = 0;
	if(actualAccount.id == 0) {											//	Si aucun compte connecté : Erreur.
		cout << "Connectez vous à un compte d'abord." << endl;	
	} else {													//	Sinon retirer l'argent.
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
	generateTransaction(tra);											//	Génèrer une transaction.
}

/**
	checkAmountFunc (void)
	Paramètre:
	Prompt de vérification du solde.
**/
void checkAmountFunc() {
	if(actualAccount.id == 0) {											//	Si aucun compte connecté : Erreur.
		cout << "Connectez vous à un compte d'abord." << endl;
	} else {													//	Sinon retourner le solde.
		cout << "Vous avez actuellement " << actualAccount.amount << "€" << endl;	
	}
}

/**
	getAccountFromId (account)
	Paramètre: id (long long)
	Retourne un compte depuis son id.
**/
account getAccountFromId(long long id) {
	account acc;
	
	ifstream iFile(ACCOUNTDATABASE);
	
	acc.id = id;
	
	string linePasscode = to_string(id)+"+P+";									//	Recherche du passcode.
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
	
	string lineAmount = to_string(id)+"+A+";									//	Recherche du solde.
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
	
	return acc;													//	Retourne un account.
}

/**
	transferMoneyFunc (void)
	Paramètre:
	Prompt de transfert d'argent.
**/
void transferMoneyFunc() {
	transaction tra;
	long long toId;
	int amount;
	if(actualAccount.id == 0) {											//	Si aucun compte connecté : Erreur.
		cout << "Connectez vous à un compte d'abord." << endl;		
	} else {													//	Sinon transferer l'argent.		
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
	generateTransaction(tra);											//	Génèrer une transaction.
	
}

/**
	getChoice (void)
	Paramètre:
	Fonction de vérification du choix de menu.
**/
void getChoice() {
	cin >> choice;
	switch(choice) {
		case createAccount:											//	Action 1 : Création du compte.
			createAccountFunc();
			break;

		case connectAccount:											//	Action 2 : Connexion au compte.
			connectAccountFunc();
			break;
			
		case dropMoney:												// 	Action 3 : Dépot d'argent.
			dropMoneyFunc();
			break;

		case takeMoney:												//	Action 4 : Retrait d'argent.
			takeMoneyFunc();
			break;

		case transferMoney:											//	Action 5 : Transfert d'argent.
			transferMoneyFunc();
			break;

		case checkAmount:											//	Action 6 : Vérification du solde.
			checkAmountFunc();
			break;

		case checkHistory:											//	Action 7 : Vérification de l'historique des transactions.
			break;

		case closeAccount:											//	Action 8 : Fermeture du compte.
			break;

		case close:												//	Action 9 : Arrêt du programme.
			doExit = true;
			break;

		default:												//	Sinon erreur.
			cerr << "Choix incorrect." << endl;
	}
}

/**
	main (int)
	Paramètre:
	Fonction principale.
**/
int main()
{
	while(!doExit) {
		/**
		
		Nettoyer la console à chaque étape.
		
		#ifdef _WIN32
			system("cls");
		#else
			system("clear");
		#endif
		**/
		
		displayMenu();
		getChoice();
	}
	return 0;
}
