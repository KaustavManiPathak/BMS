#pragma once
#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include<vector>

//for database connection
std::string hostname = "localhost"; //localhost
std::string username = "root"; //root
std::string pssword = "****"; //password or null
std::string database_name = "Bank_Mgmt"; //name of the database


//Rowdata definition
struct RowData {
	std::wstring AccountNo;
	std::wstring AccountType; 
	std::wstring CustomerName;
	std::wstring Gender; 
	std::wstring Age; 
	std::wstring Occupation;
	std::wstring Address; 
	std::wstring Phone;
	std::wstring Balance;
};

//function declarations
std::wstring stringTowtsring(std::string str);

void dbms();

void enter_into_accounts(std::string AccountNo, std::string AccountType, std::string CustomerName,
	std::string Gender, std::string Age, std::string Occupation,
	std::string Address, std::string Phone, std::string Balance, HWND hwnd);

std::vector<std::wstring> find_account_in_db(std::string AccountNo);

void delete_account(std::string AccountNo, HWND hwnd);

std::vector<RowData> show_transactions(std::string AccountNo);

std::string calculate_balance(std::string AccountNo, std::string TranscType, std::string Amount);

void transactions_in_db(std::string AccountNo, std::string TranscType, std::string Amount, HWND hwmd);

std::vector<RowData> show_database();

#endif // !DATABASE_MANAGER_H
