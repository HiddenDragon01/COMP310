#include <stdlib.h>
#include <stdio.h>

int balances[100];
int numberAttemptedTransactions = 0;

int accountNumberValid(int accountNumber){
	if (accountNumber < 100 &&
		accountNumber >= 0) return 1; //true
	else return 0; //false
}

void deposit(int amount, int accountNumber){
	numberAttemptedTransactions++;
	if (amount > 0 && 
		accountNumberValid(accountNumber)) {
		balances[accountNumber] += amount;
	}
}

int balance (int accountNumber) {
	numberAttemptedTransactions++;

	if (accountNumberValid(accountNumber)){
		return balances[accountNumber];
	} else{
		return -1; //error
	}
}