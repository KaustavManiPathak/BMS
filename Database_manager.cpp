#ifndef UNICODE
#define UNICODE
#endif 

#include<iostream>
#include<iomanip>
#include<vector>
#include<Windows.h>

#include<mysql_connection.h>
#include<mysql_driver.h>
#include<cppconn/exception.h>
#include<cppconn/resultset.h>
#include<cppconn/statement.h>
#include<cppconn/prepared_statement.h>
#include<cppconn/resultset.h>

#include "Database_manager.h"

//convert string to wstring
std::wstring stringTowtsring(std::string str) {
	std::wstring wstr(str.begin(), str.end());
	return wstr;
}

//main database creation function
void dbms() {
	try {
		sql::Driver * driver = sql::mysql::get_mysql_driver_instance();
		sql::Connection* db = driver->connect(hostname, username, pssword);
		sql::Statement* stmt = db->createStatement();
		//Database Created
		stmt->execute("CREATE DATABASE IF NOT EXISTS " + database_name);
		stmt->execute("USE Bank_Mgmt");
		//Accounts Table Created
		stmt->execute("CREATE TABLE IF NOT EXISTS Accounts(Account_No VARCHAR(50) UNIQUE PRIMARY KEY, Account_Type VARCHAR(50), Customer_Name VARCHAR(255), Gender VARCHAR(20), Age VARCHAR(20), Occupation VARCHAR(50), Address VARCHAR(255), Ph_No VARCHAR(50), Account_Balance VARCHAR(50))");
		//Transactions Table Created
		stmt->execute("CREATE TABLE IF NOT EXISTS Transactions(Transaction_Id INT UNIQUE AUTO_INCREMENT PRIMARY KEY, Account_No  VARCHAR(50), Transaction_Type VARCHAR(50), Amount VARCHAR(50), Balance VARCHAR(50))");
		delete stmt;
		delete db;
	}
	catch (sql::SQLException& e) {
		OutputDebugString(e.what());
		OutputDebugString((LPCSTR) e.getSQLStateCStr());
		std::cerr << "SQLEXCEPTION:" << e.what() << std::endl;
		std::cerr << "SQLErrorCode:" << e.getErrorCode() << std::endl;
		std::cerr << "SQLSTATE:" << e.getSQLState() << std::endl;
	}
}

//save account data
void enter_into_accounts(std::string AccountNo, std::string AccountType, std::string CustomerName, 
	std::string Gender, std::string Age, std::string Occupation,
	std::string Address, std::string Phone, std::string Balance, HWND hwnd) {
	try {
		dbms();
		sql::Driver * driver = sql::mysql::get_mysql_driver_instance();
		sql::Connection* db = driver->connect(hostname, username, pssword);
		sql::Statement* stmt = db->createStatement();
		stmt->execute("USE " + database_name);
		sql::PreparedStatement* pst = db->prepareStatement("INSERT INTO Accounts(Account_No, Account_Type, Customer_Name, Gender, Age, Occupation, Address, Ph_No, Account_Balance) VALUES (?,?,?,?,?,?,?,?,?)");
		pst->setString(1, AccountNo);
		pst->setString(2, AccountType);
		pst->setString(3, CustomerName);
		pst->setString(4, Gender);
		pst->setString(5, Age);
		pst->setString(6, Occupation);
		pst->setString(7, Address);
		pst->setString(8, Phone);
		pst->setString(9, Balance);
		pst->execute();
		delete pst;
		delete stmt;
		delete db;
		MessageBox(hwnd, "Successfully Created Account", "SAVE ACCOUNT", MB_OK);
	}
	catch (sql::SQLException& e) {
		MessageBox(hwnd, "Account Creation Failed", "SAVE ACCOUNT", MB_OK);
		std::cerr << "SQLEXCEPTION:" << e.what() << std::endl;
		std::cerr << "SQLSTATE:" << e.getSQLState() << std::endl;
	}
}

//search account 
std::vector<std::wstring> find_account_in_db(std::string AccountNo) {
	std::vector<std::wstring> SerAcc;
	try {
		dbms();
		sql::Driver * driver = sql::mysql::get_mysql_driver_instance();
		sql::Connection* db = driver->connect(hostname, username, pssword);
		sql::Statement* stmt = db->createStatement();
		stmt->execute("USE " + database_name);
		sql::PreparedStatement* pst = db->prepareStatement("SELECT * FROM Accounts WHERE Account_No = ?");
		pst->setString(1, AccountNo);
		
		sql::ResultSet* res = pst->executeQuery();
	
		if (res->next()) {
			SerAcc.push_back(stringTowtsring(res->getString(2))); //account type 0
			SerAcc.push_back(stringTowtsring(res->getString(3))); // customer name 1
			SerAcc.push_back(stringTowtsring(res->getString(4))); //gender 2
			SerAcc.push_back(stringTowtsring(res->getString(5))); // age 3
			SerAcc.push_back(stringTowtsring(res->getString(6))); //occupation 4
			SerAcc.push_back(stringTowtsring(res->getString(7))); // address 5
			SerAcc.push_back(stringTowtsring(res->getString(8))); //phone 6
			SerAcc.push_back(stringTowtsring(res->getString(9))); //balance 7
			
		}
		delete stmt;
		delete pst;
		delete res;
		delete db;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQLEXCEPTION:" << e.what() << std::endl;
		std::cerr << "SQLSTATE:" << e.getSQLState() << std::endl;
	}
	return SerAcc;
}

//delete account
void delete_account(std::string AccountNo, HWND hwnd) {
	try {
		dbms();
		sql::Driver * driver = sql::mysql::get_mysql_driver_instance();
		sql::Connection* db = driver->connect(hostname, username, pssword);
		sql::Statement* stmt = db->createStatement();
		stmt->execute("USE " + database_name);
		sql::PreparedStatement* pst = db->prepareStatement("DELETE FROM Accounts WHERE Account_No = ?");
		pst->setString(1, AccountNo);
		pst->execute();
		sql::PreparedStatement* pst2 = db->prepareStatement("DELETE FROM Transactions WHERE Account_No = ?");
		pst2->setString(1, AccountNo);
		pst2->execute();
		delete pst;
		delete pst2;
		delete stmt;
		delete db;
		MessageBox(hwnd, "Successfully Deleted Account", "DELETE ACCOUNT", MB_OK);
	}
	catch (sql::SQLException& e) {
		OutputDebugString(e.what());
		MessageBox(hwnd, "Delete Account Unsuccessful", "DELETE ACCOUNT", MB_OK);
		std::cerr << "SQLEXCEPTION:" << e.what() << std::endl;
		std::cerr << "SQLSTATE:" << e.getSQLState() << std::endl;
	}
}

//Create transactions
void transactions_in_db(std::string AccountNo, std::string TranscType, std::string Amount, HWND hwnd) {
	try {
		dbms();

		std::string Bal = calculate_balance( AccountNo, TranscType, Amount); //to update balance

		sql::Driver * driver = sql::mysql::get_mysql_driver_instance();
		sql::Connection* db = driver->connect(hostname, username, pssword );
		sql::Statement* stmt = db->createStatement();
		stmt->execute("USE " + database_name);
		sql::PreparedStatement* pst = db->prepareStatement("INSERT INTO Transactions(Account_No, Transaction_Type, Amount, Balance) VALUES (?,?,?,?)");
		pst->setString(1, AccountNo);
		pst->setString(2, TranscType);
		pst->setString(3, Amount);
		pst->setString(4, Bal);
		pst->execute();
		delete pst;
		delete stmt;
		delete db;
		MessageBox(hwnd, "Transaction Successful", "PERFORM TRANSACTION", MB_OK);
	}
	catch (sql::SQLException& e) {
		MessageBox(hwnd, "Transaction Unsuccessful", "PERFORM TRANSACTION", MB_OK);
		OutputDebugString(e.what());
		std::cerr << "SQLEXCEPTION:" << e.what() << std::endl;
		std::cerr << "SQLSTATE:" << e.getSQLState() << std::endl;
	}

}

//calculate account balance
std::string calculate_balance(std::string AccountNo, std::string TranscType, std::string Amount) {
	try {
		std::string Bal; 
		sql::Driver * driver = sql::mysql::get_mysql_driver_instance();
		sql::Connection* db = driver->connect(hostname, username, pssword);
		sql::Statement* stmt = db->createStatement();
		stmt->execute("USE " + database_name);
		
		sql::PreparedStatement* pst1 = db->prepareStatement("SELECT Account_Balance FROM Accounts WHERE Account_No = ?");
		pst1->setString(1, AccountNo);
		sql::ResultSet* res = pst1->executeQuery();

		if (res->next()) {
			Bal = res->getString(1);
		}
		int BALC = std::stoi(Bal);
		int AMT = std::stoi(Amount);
		
		Bal = "";

		if (TranscType == "DEPOSIT") {
			BALC += AMT;
		}
		else {
			BALC -= AMT;
		}
		Bal = std::to_string(BALC);
	
		sql::PreparedStatement* pst = db->prepareStatement("UPDATE Accounts SET Account_Balance = ? WHERE Account_No = ?");
		pst->setString(1, Bal);
		pst->setString(2, AccountNo);
		pst->execute();
		
		delete pst1;
		delete res;
		delete pst;
		delete stmt;
		delete db;
		
		return Bal;
	}
	catch (sql::SQLException& e) {
		OutputDebugString(e.what());
		std::cerr << "SQLEXCEPTION:" << e.what() << std::endl;
		std::cerr << "SQLSTATE:" << e.getSQLState() << std::endl;
	}
}

//show all transactions from account no.
std::vector<RowData> show_transactions(std::string AccountNo) {
	std::vector<RowData>TD;
	try {
		dbms();
		sql::Driver * driver = sql::mysql::get_mysql_driver_instance();
		sql::Connection* db = driver->connect(hostname, username, pssword);
		sql::Statement* stmt = db->createStatement();
		stmt->execute("USE " + database_name);
		sql::PreparedStatement* pst = db->prepareStatement("SELECT * FROM Transactions WHERE Account_No = ?");
		pst->setString(1, AccountNo);
		sql::ResultSet* res = pst->executeQuery();
		

		while (res->next()) {
			RowData row;
			row.AccountType = stringTowtsring(res->getString(3)); //transaction type
			row.CustomerName = stringTowtsring(res->getString(4)); // amount
			row.Balance = stringTowtsring(res->getString(5)); //balance
			TD.push_back(row);
		}
		delete stmt;
		delete res;
		delete db;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQLEXCEPTION:" << e.what() << std::endl;
		std::cerr << "SQLSTATE:" << e.getSQLState() << std::endl;
	}
	return TD;
}

//show all accounts
std::vector<RowData> show_database() {
	std::vector<RowData> Data;
	try {
		dbms();
		sql::Driver * driver = sql::mysql::get_mysql_driver_instance();
		sql::Connection* db = driver->connect(hostname, username, pssword);
		sql::Statement* stmt = db->createStatement();
		stmt->execute("USE " + database_name);
		sql::ResultSet* res = stmt->executeQuery("SELECT * FROM Accounts");

		while (res->next()) {
			RowData row;
			row.AccountNo = stringTowtsring(res->getString(1));
			row.AccountType = stringTowtsring(res->getString(2));
			row.CustomerName = stringTowtsring(res->getString(3));
			Data.push_back(row);
		}
		delete stmt;
		delete res;
		delete db;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQLEXCEPTION:" << e.what() << std::endl;
		std::cerr << "SQLSTATE:" << e.getSQLState() << std::endl;
	}
	return Data;
}