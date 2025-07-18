#pragma once
#ifndef ACCOUNTS_H
#define ACCOUNTS_H

/**
Currently used for Loan Accounts only
Holds the loan amount, loan rate for each class 
Calculates and return EMI based on the values above.
**/


class Customer {
protected:
	int loan_amount;
	int loan_rate;
	int EMI;
public:
	virtual int getLoanAmount() { return loan_amount; }
	virtual int getEMI() { return EMI; }
};

class Retired : public Customer {
public:
	Retired() { loan_amount = 100000; loan_rate = 108;  EMI = ((loan_amount * loan_rate) / 2400);
	}
};

class Student : public Customer {
public:
	Student() {
		loan_amount = 50000; loan_rate = 108; EMI = ((loan_amount * loan_rate) / 1200);
	}
};
class Employed : public Customer {
public:
	Employed() {
		loan_amount = 250000; loan_rate = 120; EMI = ((loan_amount * loan_rate)/3600);
	}
};
inline int returnloanamount(Customer& cust) {
	return cust.getLoanAmount();
}
inline int returnEMI(Customer& cust) {
	return cust.getEMI();
}

#endif // !ACCOUNTS_H
