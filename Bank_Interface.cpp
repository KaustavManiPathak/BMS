#ifndef UNICODE
#define UNICODE
#endif 

#include<iostream>
#include<Windows.h>
#include<CommCtrl.h>
#include <string>
#include <codecvt>

#include "Bank_Interface.h"
#include"Database_manager.h"
#include "Accounts.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	WNDCLASS wc = { };
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx(
		0,CLASS_NAME, L"Bank Management System", WS_OVERLAPPEDWINDOW , CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,NULL, hInstance,  NULL );

	if (hwnd == NULL) {	return 0;}

	ShowWindow(hwnd, nCmdShow);

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg); 
	}
	return 0;
}

//fuction to extract data from an editcontrol and return an std::string of that data [DO NOT DELETE OR CHANGE]
std::string get_data(HWND hedit) {
	int textLength = GetWindowTextLength(hedit);
	if (textLength == 0) {
		return "";
	}
	std::wstring wstrText(textLength + 1, L'\0');
	GetWindowText(hedit, &wstrText[0], textLength + 1);
	wstrText.resize(textLength);

	std::string strText;
	int requiredSize = WideCharToMultiByte(CP_UTF8, 0, wstrText.c_str(), -1, NULL, 0, NULL, NULL);
	if (requiredSize > 0) {
		strText.resize(requiredSize - 1);
		WideCharToMultiByte(CP_UTF8, 0, wstrText.c_str(), -1, &strText[0], requiredSize, NULL, NULL);
	}
	return strText;
	
}

int Acc_index;
// account management window
LRESULT CALLBACK FormWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg) {
	case WM_DESTROY:
		hwndForm = NULL;
		return 0;
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case EDIT_COMBO:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				Acc_index = SendMessage(lv_Atype, CB_GETCURSEL, 0, 0);
			}
		case EDIT_OCC: {
			int oindex = SendMessage(lv_Occ, CB_GETCURSEL, 0, 0);
			std::wstring bal;
			if (Acc_index == 1) {
				if (oindex == 0) {
					Employed emp;
					bal = stringTowtsring(std::to_string(returnloanamount(emp)));
					SetWindowText(eAcc_Bal, bal.c_str());
				}
				else if (oindex == 1) {
					Retired re;
					bal = stringTowtsring(std::to_string(returnloanamount(re)));
					SetWindowText(eAcc_Bal, bal.c_str());
				}
				else if (oindex == 2) {
					Student stu;
					bal = stringTowtsring(std::to_string(returnloanamount(stu)));
					SetWindowText(eAcc_Bal, bal.c_str());
				}
			}
			else {
				bal = stringTowtsring(std::to_string(0));
				SetWindowText(eAcc_Bal, bal.c_str());
			}
		}

		}
		switch (wParam) {
		case SEARCH_CUSTOMER: { 
			std::vector<std::wstring> Saccount;
			std::wstring AccountType[3] = { L"Current Account" , L"Loan Account", L"Savings Account" };
			std::wstring Occuption[3] = { L"Employed", L"Retired",L"Student" };

			SendMessage(hwndRadio1, BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(hwndRadio2, BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(hwndRadio3, BM_SETCHECK, BST_UNCHECKED, 0);



			std::string AccountNN = get_data(eA_No);
			Saccount = find_account_in_db(AccountNN); //function in datatbasemanager
			if (!Saccount.empty()) {
				SetWindowText(eCname, Saccount[1].c_str());
				SetWindowText(eCage, Saccount[3].c_str());
				SetWindowText(eCaddress, Saccount[5].c_str());
				SetWindowText(ePH, Saccount[6].c_str());
				SetWindowText(eAcc_Bal, Saccount[7].c_str());
				for (int i = 0; i < 3; i++) {
					if (Saccount[0] == AccountType[i]) {
						SendMessage(lv_Atype, CB_SETCURSEL, i,0);
					}
					if (Saccount[4] == Occuption[i]) {
						SendMessage(lv_Occ, CB_SETCURSEL, i,0);
					}
				}
				if (Saccount[2] == L"Male") {
					SendMessage(hwndRadio1, BM_SETCHECK, BST_CHECKED, 0);
				}
				else if (Saccount[2] == L"Female") {
					SendMessage(hwndRadio2, BM_SETCHECK, BST_CHECKED, 0);

				}
				else if (Saccount[2] == L"Others") {
					SendMessage(hwndRadio3, BM_SETCHECK, BST_CHECKED, 0);
				}
				
			}
			break;
			}

		case ADD_CUSTOMER: {
			std::string AccountType[3] = { "Current Account" , "Loan Account", "Savings Account" };
			std::string Occuption[3] = { "Employed", "Retired","Student" };
			int Aindex = SendMessage(lv_Atype, CB_GETCURSEL, 0, 0);
			int Oindex = SendMessage(lv_Occ, CB_GETCURSEL, 0, 0);
			std::string gender;

			if ((SendMessage(hwndRadio1, BM_GETCHECK, 0, 0) == BST_CHECKED)) {
				gender = "Male";
			}
			else if ((SendMessage(hwndRadio2, BM_GETCHECK, 0, 0) == BST_CHECKED)) {
				gender = "Female";
			}
			else if ((SendMessage(hwndRadio3, BM_GETCHECK, 0, 0) == BST_CHECKED)) {
				gender = "Others";
			}

			enter_into_accounts(get_data(eA_No), AccountType[Aindex], get_data(eCname), gender, get_data(eCage), Occuption[Oindex], get_data(eCaddress), get_data(ePH), get_data(eAcc_Bal), hwnd); // function in database manager

			break;
			}

		case DELETE_CUSTOMER: {
			
			delete_account(get_data(eA_No),hwnd);

			break; 
			}
		}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void CustomerForm(HWND hwnd) {
	if (hwndForm == NULL) {
		WNDCLASS wcForm = { };
		if (!wcForm.lpszClassName) {
			wcForm.lpfnWndProc = FormWindowProc;
			wcForm.hInstance = GetModuleHandle(NULL);
			wcForm.lpszClassName = L"FormWindowClass";

			RegisterClass(&wcForm);
		}

		hwndForm = CreateWindowEx(0, L"FormWindowClass", L"ACCOUNT MANAGEMENT", WS_CHILD | WS_VISIBLE, 10, 10, 800, 500, hwnd, NULL, GetModuleHandle(NULL), NULL);

		//ACCOUNT NO
		static HWND textAA = CreateWindowEx(0, L"Static", L"*ACCOUNT NO. :", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 10, 300, 20, hwndForm, NULL, NULL, NULL);
		eA_No = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 300, 10, 250, 20, hwndForm, (HMENU)EDIT_TEXT, NULL, NULL);

		//ACCOUNT TYPE
		static HWND text2A = CreateWindowEx(0, L"Static", L"*ACCOUNT TYPE :", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 40, 300, 20, hwndForm, NULL, NULL, NULL);
		lv_Atype = CreateWindowEx(0, L"combobox", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | BS_CENTER, 300, 40, 250, 150, hwndForm, (HMENU)EDIT_COMBO, NULL, NULL);
		SendMessage(lv_Atype, CB_ADDSTRING, 0, (LPARAM)L"CURRENT ACCOUNT");
		SendMessage(lv_Atype, CB_ADDSTRING, 0, (LPARAM)L"LOAN ACCOUNT");
		SendMessage(lv_Atype, CB_ADDSTRING, 0, (LPARAM)L"SAVINGS ACCOUNT");
		SendMessage(lv_Atype, CB_SETCURSEL, 2, 0);

		//CUSTOMER NAME
		static HWND text2B = CreateWindowEx(0, L"Static", L"*CUSTOMER NAME :", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 70, 300, 20, hwndForm, NULL, NULL, NULL);
		eCname = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 300, 70, 250, 20, hwndForm, (HMENU)EDIT_TEXT, NULL, NULL);

		//CUSTOMER GENDER
		static HWND text = CreateWindowEx(0, L"Static", L"CUSTOMER GENDER :", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 100, 300, 20, hwndForm, NULL, NULL, NULL);
		hwndRadio1 = CreateWindowEx(0, L"BUTTON", L"MALE", WS_GROUP | WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_CENTER, 320, 100, 50, 20, hwndForm, (HMENU)ID_RAdio1, NULL, NULL);
		hwndRadio2 = CreateWindowEx(0, L"BUTTON", L"FEMALE", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_CENTER, 380, 100, 80, 20, hwndForm, (HMENU)ID_RAdio2, NULL, NULL);
		hwndRadio3 = CreateWindowEx(0, L"BUTTON", L"OTHERS", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_CENTER, 460, 100, 80, 20, hwndForm, (HMENU)ID_RAdio3, NULL, NULL);

		//CUSTOMER AGE
		static HWND text5 = CreateWindowEx(0, L"Static", L"*CUSTOMER AGE :", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 130, 300, 20, hwndForm, NULL, NULL, NULL);
		eCage = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 300, 130, 250, 20, hwndForm, (HMENU)EDIT_TEXT, NULL, NULL);
		SendMessage(eCage, EM_LIMITTEXT,2,0);
		//CUSTOMER OCCUPATION
		static HWND text6 = CreateWindowEx(0, L"Static", L"*CUSTOMER OCCUPATION :", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 160, 300, 20, hwndForm, NULL, NULL, NULL);
		lv_Occ = CreateWindowEx(0, L"combobox", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | BS_CENTER, 300, 160, 250, 150, hwndForm, (HMENU)EDIT_OCC, NULL, NULL);
		SendMessage(lv_Occ, CB_ADDSTRING, 0, (LPARAM)L"EMPLOYED");
		SendMessage(lv_Occ, CB_ADDSTRING, 0, (LPARAM)L"RETIRED");
		SendMessage(lv_Occ, CB_ADDSTRING, 0, (LPARAM)L"STUDENT");
		SendMessage(lv_Occ, CB_SETCURSEL, 0, 0);

		//CUSTOMER ADDRESS
		static HWND text7 = CreateWindowEx(0, L"Static", L"*CUSTOMER ADDRESS :", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 190, 300, 20, hwndForm, NULL, NULL, NULL);
		eCaddress = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 300, 190, 250, 20, hwndForm, (HMENU)EDIT_TEXT, NULL, NULL);

		//CUSTOMER PH NO
		static HWND text8 = CreateWindowEx(0, L"Static", L"*CUSTOMER CONTACT NO. :", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 220, 600, 20, hwndForm, NULL, NULL, NULL);
		ePH = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 300, 220, 250, 20, hwndForm, (HMENU)EDIT_TEXT, NULL, NULL);
		SendMessage(ePH, EM_LIMITTEXT, 10, 0);
		//ACCOUNT BALANCE
		static HWND text9 = CreateWindowEx(0, L"Static", L"ACCOUNT BALANCE :", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 250, 600, 20, hwndForm, NULL, NULL, NULL);
		eAcc_Bal = CreateWindowEx(0, L"EDIT", L"0", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER | ES_READONLY, 300, 250, 250, 20, hwndForm, (HMENU)EDIT_TEXT, NULL, NULL);

		Button2 = CreateWindowEx(0, L"Button", L"SEARCH ACCOUNT", WS_VISIBLE | WS_CHILD, 560, 10, 150, 20, hwndForm, (HMENU)SEARCH_CUSTOMER, NULL, NULL);
		Button3 = CreateWindowEx(0, L"Button", L"SAVE ACCOUNT", WS_VISIBLE | WS_CHILD, 300, 280, 250, 20, hwndForm, (HMENU)ADD_CUSTOMER, NULL, NULL);
		Button4 = CreateWindowEx(0, L"Button", L"DELETE ACCOUNT", WS_VISIBLE | WS_CHILD, 300, 310, 250, 20, hwndForm, (HMENU)DELETE_CUSTOMER, NULL, NULL);

	}
	ShowWindow(hwndlistView, SW_HIDE);
	ShowWindow(hwndBal, SW_HIDE);
	ShowWindow(hwndTransc, SW_HIDE);
	ShowWindow(hwndForm, SW_SHOW);
}

//all accounts
void view_all_accounts(HWND hwnd) {
	std::vector<RowData> all_accounts;
	all_accounts = show_database();
	hwndlistView = CreateWindowEx(0, WC_LISTVIEW, L"", WS_CHILD | WS_VISIBLE | LVS_REPORT, 10, 10, 400, 600, hwnd, NULL, NULL, NULL);
	LVCOLUMN column;
	column.mask = LVCF_TEXT | LVCF_WIDTH;
	column.pszText = (LPWSTR)L"Customer Name";
	column.cx = 100;
	ListView_InsertColumn(hwndlistView, 0, &column);
	column.pszText = (LPWSTR)L"Account Type";
	column.cx = 100;
	ListView_InsertColumn(hwndlistView, 0, &column);
	column.pszText = (LPWSTR)L"Account No.";
	column.cx = 100;
	ListView_InsertColumn(hwndlistView, 0, &column);

	for (int i = 0; i < all_accounts.size(); i++) {

		LVITEM item;
		item.mask = LVIF_TEXT;
		item.pszText = const_cast<LPWSTR>(all_accounts[i].AccountNo.c_str());
		item.iItem = i;
		item.iSubItem = 0;
		
		ListView_InsertItem(hwndlistView, &item);
		ListView_SetItemText(hwndlistView, i, 1, const_cast<LPWSTR>(all_accounts[i].AccountType.c_str()));
		ListView_SetItemText(hwndlistView, i, 2, const_cast<LPWSTR>(all_accounts[i].CustomerName.c_str()));
	}
	ShowWindow(hwndlistView, SW_SHOW);
	ShowWindow(hwndBal, SW_HIDE);
	ShowWindow(hwndForm, SW_HIDE);
	ShowWindow(hwndTransc, SW_HIDE);
	
}

LRESULT CALLBACK BalWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_DESTROY:
		return 0;
		break;
	case WM_COMMAND:
		switch (wParam) {
		case SEARCH_CUSTOMER_BALANCE:
			std::string AccountNo = get_data(eBA_No);
			std::vector<RowData> all_accounts;
			all_accounts = show_transactions(AccountNo);
			ListView_DeleteAllItems(hwndlistView2);

			for (int i = 0; i < all_accounts.size(); i++) {
				LVITEM item;
				item.mask = LVIF_TEXT;
				item.pszText = const_cast<LPWSTR>(all_accounts[i].Balance.c_str());
				item.iItem = i;
				item.iSubItem = 0;
				
				ListView_InsertItem(hwndlistView2, &item);
				ListView_SetItemText(hwndlistView2, i, 1, const_cast<LPWSTR>(all_accounts[i].CustomerName.c_str()));
				ListView_SetItemText(hwndlistView2, i, 2, const_cast<LPWSTR>(all_accounts[i].AccountType.c_str()));
			}
			break;
		}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//balance enquiry
void bal_enquiry(HWND hwnd) {
	if (hwndBal == NULL) {
		WNDCLASS wcBal = { };
		if (!wcBal.lpszClassName) {
			wcBal.lpfnWndProc = BalWindowProc;
			wcBal.hInstance = GetModuleHandle(NULL);
			wcBal.lpszClassName = L"BalWindowClass";

			RegisterClass(&wcBal);
		}

		hwndBal = CreateWindowEx(0, L"BalWindowClass", L"Balance ", WS_CHILD | WS_VISIBLE, 10, 10, 800, 500, hwnd, NULL, GetModuleHandle(NULL), NULL);
		static HWND textAA = CreateWindowEx(0, L"Static", L"*ACCOUNT NO. :", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 10, 300, 20, hwndBal, NULL, NULL, NULL);
		eBA_No = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 300, 10, 250, 20, hwndBal, (HMENU)EDIT_TEXT, NULL, NULL);
		Button2 = CreateWindowEx(0, L"Button", L"SEARCH ACCOUNT", WS_VISIBLE | WS_CHILD, 560, 10, 150, 20, hwndBal, (HMENU)SEARCH_CUSTOMER_BALANCE, NULL, NULL);
		
		eBAl_show = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER| ES_READONLY, 300, 40, 250, 20, hwndBal, (HMENU)EDIT_TEXT, NULL, NULL);
		
		
	
		hwndlistView2 = CreateWindowEx(0, WC_LISTVIEW, L"", WS_CHILD | WS_VISIBLE | LVS_REPORT, 10, 70, 400, 600, hwndBal, NULL, NULL, NULL);
		LVCOLUMN column;
		column.mask = LVCF_TEXT | LVCF_WIDTH;
		column.pszText = (LPWSTR)L"Transaction Type";
		column.cx = 100;
		ListView_InsertColumn(hwndlistView2, 0, &column);
		column.pszText = (LPWSTR)L"Amount";
		column.cx = 100;
		ListView_InsertColumn(hwndlistView2, 0, &column);
		column.pszText = (LPWSTR)L"Balance";
		column.cx = 100;
		ListView_InsertColumn(hwndlistView2, 0, &column);


	}
	ShowWindow(hwndlistView, SW_HIDE);
	ShowWindow(hwndBal, SW_SHOW);
	ShowWindow(hwndForm, SW_HIDE);
	ShowWindow(hwndTransc, SW_HIDE);
}



std::wstring MAccountTYPE;
int LA;

//transaction 
LRESULT CALLBACK TranscWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_DESTROY:
		return 0;
		break;
	case WM_COMMAND:
		switch (wParam)
		{
		case SEARCH_CUSTOMER_ACC:{
				std::vector<std::wstring> Saccount;
				std::wstring MAccountOCC;
				std::string AccountNN = get_data(eAcc_No); 
				Saccount = find_account_in_db(AccountNN); 
				SendMessage(lB_transc, CB_DELETESTRING, 0, (LPARAM)L"PAY EMI");
				SendMessage(lB_transc, CB_DELETESTRING, 0, (LPARAM)L"ACCOUNT TRANSFER");
				SendMessage(lB_transc, CB_DELETESTRING, 0, (LPARAM)L"DEPOSIT");
				SendMessage(lB_transc, CB_DELETESTRING, 0, (LPARAM)L"WITHDRAWL");
				
				if (!Saccount.empty()) {
					MAccountTYPE = Saccount[0];
					MAccountOCC = Saccount[4];
					SetWindowText(eATy, MAccountTYPE.c_str());	
				}
				if (MAccountTYPE == L"Loan Account") {
					std::wstring LAmount;
					if (MAccountOCC == L"Retired") {
						Retired retire;
						LA = returnEMI(retire);
					}
					else if (MAccountOCC == L"Student") {
						Student stu;
						LA = returnEMI(stu);
					}
					else if (MAccountOCC == L"Employed") {
						Employed Emp;
						LA = returnEMI(Emp);
					}
					LAmount = stringTowtsring(std::to_string(LA));
					OutputDebugString(LAmount.c_str());
					SetWindowText(eAmnt, LAmount.c_str());
					SendMessage(eAmnt, ES_READONLY, 0, 0);

					SendMessage(lB_transc, CB_ADDSTRING, 0, (LPARAM)L"PAY EMI");
					SendMessage(lB_transc, CB_SETCURSEL, 0, 0);
				}
				else {
					
					SendMessage(lB_transc, CB_ADDSTRING, 0, (LPARAM)L"ACCOUNT TRANSFER");
					SendMessage(lB_transc, CB_ADDSTRING, 0, (LPARAM)L"DEPOSIT");
					SendMessage(lB_transc, CB_ADDSTRING, 0, (LPARAM)L"WITHDRAWL");
					SendMessage(lB_transc, CB_SETCURSEL, 1, 0);
				}
				
				break;
			}						
		case PERFORM_TRANSACTION: {
			std::string Account1 = get_data(eAcc_No);

			if (MAccountTYPE == L"Loan Account") {
				int Aindex = SendMessage(lB_transc, CB_GETCURSEL, 0, 0);
				std::string TranscType = "EMI";
				transactions_in_db(Account1, TranscType, std::to_string(LA), hwnd);
			}
			else {
				std::string Amount = get_data(eAmnt);
				int Aindex = SendMessage(lB_transc, CB_GETCURSEL, 0, 0);
				switch (Aindex) {
					case 0: {
						std::string Account2 = get_data(eA_No_2);
						std::string TranscType1 = "WITHDRAWL";
						std::string TranscType2 = "DEPOSIT";
						transactions_in_db(Account1, TranscType1, Amount,hwnd);
						transactions_in_db(Account2, TranscType2, Amount, NULL);

						break; 
					}
					case 1: {
						std::string TranscType3 = "DEPOSIT";
						transactions_in_db(Account1, TranscType3, Amount, hwnd);
						break; 
					}
					case 2: {
						std::string TranscType4 = "WITHDRAWL";
						transactions_in_db(Account1, TranscType4, Amount, hwnd);
						break; 
					}
				}
			}
			break;
		}
		case SEARCH_RECEIVER_ACC: {
			std::vector<std::wstring> Saccount;
			std::string AccountNN = get_data(eA_No_2);
			Saccount = find_account_in_db(AccountNN);
			
			if (!Saccount.empty()) {
				SetWindowText(eAcc_type_r, Saccount[0].c_str());
			}
			break;
		}
		}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void transaction(HWND hwnd) {
	if (hwndTransc == NULL) {
		WNDCLASS wcTransc = { };
		if (!wcTransc.lpszClassName) {
			wcTransc.lpfnWndProc = TranscWindowProc;
			wcTransc.hInstance = GetModuleHandle(NULL);
			wcTransc.lpszClassName = L"TranscWindowClass";

			RegisterClass(&wcTransc);

		}
		hwndTransc = CreateWindowEx(0, L"TranscWindowClass", L"Transaction MANAGEMENT", WS_CHILD | WS_VISIBLE, 10, 10, 800, 500, hwnd, NULL, GetModuleHandle(NULL), NULL);

		static HWND teAA = CreateWindowEx(0, L"Static", L"*ACCOUNT NO. :", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 10, 300, 20, hwndTransc, NULL, NULL, NULL);
		eAcc_No = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 300, 10, 250, 20, hwndTransc, (HMENU)EDIT_TEXT, NULL, NULL);
		Button2 = CreateWindowEx(0, L"Button", L"SEARCH ACCOUNT", WS_VISIBLE | WS_CHILD, 560, 10, 150, 20, hwndTransc, (HMENU)SEARCH_CUSTOMER_ACC, NULL, NULL);

		static HWND text51 = CreateWindowEx(0, L"Static", L"ACCOUNT TYPE :", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 40, 300, 20, hwndTransc, NULL, NULL, NULL);
		eATy = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, 300, 40, 250, 20, hwndTransc, (HMENU)EDIT_TEXT, NULL, NULL);

		static HWND te2A = CreateWindowEx(0, L"Static", L"*TRANSACTION TYPE :", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 70, 300, 20, hwndTransc, NULL, NULL, NULL);
		lB_transc = CreateWindowEx(0, L"combobox", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | BS_CENTER, 300, 70, 250, 150, hwndTransc, (HMENU)EDIT_TEXT, NULL, NULL);
		

		static HWND tt2B = CreateWindowEx(0, L"Static", L"*AMOUNT :", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 100, 300, 20, hwndTransc, NULL, NULL, NULL);
		eAmnt = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 300, 100, 250, 20, hwndTransc, (HMENU)EDIT_TEXT, NULL, NULL);

		static HWND AccountN2 = CreateWindowEx(0, L"Static", L"*ACCOUNT NO. :", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 130, 300, 20, hwndTransc, NULL, NULL, NULL);
		eA_No_2 = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 300, 130, 250, 20, hwndTransc, (HMENU)EDIT_TEXT, NULL, NULL);
		Button2 = CreateWindowEx(0, L"Button", L"SEARCH TO ACCOUNT", WS_VISIBLE | WS_CHILD, 560, 130, 150, 20, hwndTransc, (HMENU)SEARCH_RECEIVER_ACC, NULL, NULL);
		
		static HWND AccountTYPEEE = CreateWindowEx(0, L"Static", L"ACCOUNT TYPE :", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 160, 300, 20, hwndTransc, NULL, NULL, NULL);
		eAcc_type_r = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, 300, 160, 250, 20, hwndTransc, (HMENU)EDIT_TEXT, NULL, NULL);


		CreateWindowEx(0, L"Button", L"PERFORM TRANSACTION", WS_VISIBLE | WS_CHILD, 300, 200, 250, 50, hwndTransc, (HMENU)PERFORM_TRANSACTION, NULL, NULL);

	}
	ShowWindow(hwndlistView, SW_HIDE);
	ShowWindow(hwndBal, SW_HIDE);
	ShowWindow(hwndTransc, SW_SHOW);
	ShowWindow(hwndForm, SW_HIDE);

}


//main window proc

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	
	case WM_CREATE:
		AddMenu(hwnd);
		static HWND textABA = CreateWindowEx(0, L"Static", L" ", WS_VISIBLE | WS_CHILD | SS_LEFT, 0, 0, 1920, 1080, hwnd, NULL, NULL, NULL);
		break;

	case WM_COMMAND:	
		switch (LOWORD(wParam)) {
		case ACCOUNTS:
			CustomerForm(hwnd);
			break;
		case TRANSACTIONS:
			transaction(hwnd);
			break;
		case ENQUIRY:
			bal_enquiry(hwnd);
			break;

		case VIEW_ALL_ACCOUNTS: {
			view_all_accounts(hwnd);
			break;
		}
		}

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &ps);
	}
	return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//menus

void AddMenu(HWND hwnd) {
	HMENU hmenu = CreateMenu();

	//MAIN MENU
	AppendMenu(hmenu, MF_STRING, ACCOUNTS, L"ACCOUNTS MANAGEMENT");
	AppendMenu(hmenu, MF_STRING , TRANSACTIONS, L"CREATE TRANSACTIONS");
	AppendMenu(hmenu, MF_STRING , ENQUIRY, L"BALANCE ENQUIRY");
	AppendMenu(hmenu, MF_STRING , VIEW_ALL_ACCOUNTS, L"VIEW ALL ACCOUNTS");

	SetMenu(hwnd, hmenu);
}


