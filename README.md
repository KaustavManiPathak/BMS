#  BMS
A simple Bank Management System with a GUI implemented in C++.

##  Overview
This project demonstrates basic banking operations such as Creating Accounts, Performing Transactions, and Checking Balances as handled by a clerk at a bank. 

##  Features
*  Creating Accounts (Savings, Current, Loan)
*  Searching Existing Accounts.
*  Deleting Existing Accounts.
*  Depositing to or Withdrawing funds from Accounts.
*  Transfer funds from one Account to another Account.
*  Paying EMI for Loan Accounts.
*  Loan Amount, Rate, and Tenure are based on Customer Occupation(Student, Employed, Retired) and are auto calculated.
*  Check Account Transactions and Balance.
*  View all existing accounts.

##  Files
*  `Bank_Interface.cpp`: The main entry point of the program. Holds all the UI related code of the application.
*  `Bank_Interface.h`: Header file for Bank_Interface. Holds window handles and function declarations.
*  `Database_manager.cpp`: Holds all Database related code for the application i.e. server connection, database and table creation, and other mysql queries. 
*  `Database_manager.h`: Header file for the Database_Manager. Holds database authentication data, custom struct definition and function declarations
*  `Accounts.h`: Holds the Accounts Class data, functions and inherited child classes.

##  Requirements
* C++ compiler(e.g. GCC)
* Mysql
* Mysql Connector/C++ (for mysqldatabase to c++ connection)
* WINDOWS SDK (Win32Api for Windows GUI)
* Visual Studio (preferred)
