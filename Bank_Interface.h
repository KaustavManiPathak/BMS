#pragma once
#ifndef BANK_INTERFACE_H
#define BANK_INTERFACE_H

#define EDIT_TEXT 40
#define SEARCH_CUSTOMER 41
#define ADD_CUSTOMER 42
#define DELETE_CUSTOMER 43
#define SEARCH_CUSTOMER_BALANCE 44
#define SEARCH_CUSTOMER_ACC 45
#define PERFORM_TRANSACTION 46
#define SEARCH_RECEIVER_ACC 47
#define EDIT_COMBO 48
#define EDIT_OCC 49
#define ACCOUNTS 11
#define TRANSACTIONS 12
#define VIEW_ALL_ACCOUNTS 13
#define ENQUIRY 14
#define ID_RAdio1 31
#define ID_RAdio2 32
#define ID_RAdio3 33

//function prototypes
void AddMenu(HWND);
void CustomerForm(HWND);
void view_all_accounts(HWND hwnd);
void transaction(HWND hwnd);

//child window handles
HWND hwndlistView;
HWND hwndlistView2;
HWND hwndForm = NULL;
HWND hwndTransc = NULL;
HWND hwndBal = NULL;
HWND hwndView = NULL;

//child windows button handles
HWND Buttonbal;
HWND Button2;
HWND Button3;
HWND Button4;
HWND Button5;
HWND Button6;
HWND Button7;

//balance window handles
static HWND eBA_No;
static HWND eBAl_show;

//transaction window handles
static HWND lB_transc;
static HWND eA_No_2;
static HWND eAcc_type_r;
static HWND eAcc_No;
static HWND eATy;
static HWND eAmnt;

//Form Window handles
static HWND eA_No;
static HWND eCname;
static HWND eCage;
static HWND eCaddress;
static HWND ePH;
static HWND eAcc_Bal;
static HWND lv_Atype;
static HWND lv_Occ;
static HWND hwndRadio1;
static HWND hwndRadio2;
static HWND hwndRadio3;

//windowprocedures
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FormWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK TranscWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK BalWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif // !BANK_INTERFACE_H
