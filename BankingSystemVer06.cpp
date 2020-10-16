/*
	------------------------------------------------
	BankingSystemVer06
	edited by Hyeok
	------------------------------------------------

	1. Account Ŭ������ ����ϴ� 2���� Ŭ���� �߰� 
		- NormalAccount : ���뿹�ݰ���
			=> �������� ��� 
		- HighCreditAccount : �ſ�ŷڰ���
			=> ���뿹�ݰ��¿� ���� �����������
			=> �� �ſ����� A,B,C�� ������ ���°����� �� ������ ��� 
			=> A,B,C ��޺��� ���� �⺻ ������ 7,4,2% ������ �߰� ���� 
		- �Աݽ� ���ڰ� ���ݿ� �������� ������ ����
	2. ��� ���¿� ���� ������ ����
		- ���� ���� �������� �ʱ� �ԱݵǴ� �ݾ׿� ���ؼ��� ���ڸ� ������� �ʴ´�
		- ���� ���� ���� ������ �Աݰ����� ��ĥ ������ ���ڰ� �߻��ϴ� ������ ���� 
		- ������ ���������� �߻��ϴ� �Ҽ��� ������ �ݾ��� �����Ѵ�.	
*/

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>

using namespace std;
const int NAME_LEN = 20;

enum { MAKE = 1, DEPOSIT, WITHDRAW, INQUIRE, EXIT };

class Account
{
protected:
	int accID;      // ���¹�ȣ
	int balance;    // �ܾ�
	char * cusName; // ���̸�
public:
	Account(int ID, int money, char * name)
		: accID(ID), balance(money)
	{
		cusName = new char[strlen(name) + 1];
		strcpy(cusName, name);
	}

	Account(const Account & ref)			// ��������� 
		: accID(ref.accID), balance(ref.balance)
	{
		cusName = new char[strlen(ref.cusName) + 1];
		strcpy(cusName, ref.cusName);
	}

	void Deposit(int money)
	{
		balance += money;
	}

	int Withdraw(int money)    // ��ݾ� ��ȯ, ���� �� 0 ��ȯ
	{
		if (balance < money)
			return 0;

		balance -= money;
		return money;
	}

	void ShowAccInfo() const
	{
		cout << "����ID: " << accID << endl;
		cout << "��  ��: " << cusName << endl;
		cout << "��  ��: " << balance << endl;
	}

	virtual double GetRate() const {				// NormalAccount, HighCreditAccount Class ���� ����ϱ� ���� �θ�Ŭ������ virtual function ����
		return 0;									// ���빫
	}

	int GetAccID() const { return accID; }
	int GetBalance() const { return balance; }
	~Account()
	{
		delete[]cusName;
	}
};

// ���뿹�ݰ���
class NormalAccount : public Account {
protected:
	double rate = 0.0;
public:
	NormalAccount(int ID, int money, char * name, double rate)
		: Account(ID, money, name), rate(rate)
	{
		cusName = new char[strlen(name) + 1];
		strcpy(cusName, name);
	}

	virtual double GetRate() const { return rate; }
};

// �ſ�ŷڰ���
class HighCreditAccount : public NormalAccount {
private:
	int grade = 0;	// �ſ��� 
public:
	HighCreditAccount(int ID, int money, char * name, double rate, char myGrade)
		: NormalAccount(ID, money, name, rate), grade(myGrade)
	{
		cusName = new char[strlen(name) + 1];
		strcpy(cusName, name);
	}

	virtual double GetRate() const 
	{
		double tot = 0.0;
		switch (grade) {				// �ſ� ��޿� ���� ���� ���� ���� 
		case 1:
			tot = rate + 0.07;
			break;
		case 2:
			tot = rate + 0.04;
			break;
		case 3:
			tot = rate + 0.02;
			break;
		}
		return tot; 
	}
};
/*
��Ʈ�� Ŭ���� :
	���α׷� ��ü�� ��� ��� => ����� ������ ����
	��Ʈ�� Ŭ���������� ���α׷� ��ü�� ��ɰ� �帧 �ľǰ���
*/
class AccountHandler
{
private:
	Account * accArr[100];	// ��ü���⿡���� ���������� �����ϹǷ� ������ ������� �ʴ´�.
	int accNum;				// ���� �������� ����Ǿ��ִ� �������� ��Ʈ�� Ŭ������ ����Լ��� ������

public:
	AccountHandler();
	void ShowMenu(void) const;
	void MakeAccount(void);
	void DepositMoney(void);
	void WithdrawMoney(void);
	void ShowAllAccInfo(void) const;
	
	~AccountHandler();
};

int main(void)
{
	AccountHandler* AH = new AccountHandler();
	int choice;

	while (1)
	{
		AH->ShowMenu();
		cout << "����: ";
		cin >> choice;
		cout << endl;

		switch (choice)
		{
		case MAKE:
			AH->MakeAccount();
			break;
		case DEPOSIT:
			AH->DepositMoney();
			break;
		case WITHDRAW:
			AH->WithdrawMoney();
			break;
		case INQUIRE:
			AH->ShowAllAccInfo();
			break;
		case EXIT:
			return 0;
		default:
			cout << "Illegal selection.." << endl;
		}
	}

	return 0;
}

void AccountHandler::ShowMenu(void) const
{
	cout << "-----Menu------" << endl;
	cout << "1. ���°���" << endl;
	cout << "2. ��    ��" << endl;
	cout << "3. ��    ��" << endl;
	cout << "4. �������� ��ü ���" << endl;
	cout << "5. ���α׷� ����" << endl;
}

void AccountHandler::MakeAccount(void)
{
	int id;

	char name[NAME_LEN];
	int balance;
	double rate;			
	int accChoose;					// ���� ������ ���� ���� ���� 
	int gradechoose;				// �ſ��� ������ ���� ���� ���� 
	
	cout << "[���� ���� ����]" << endl;
	cout << "1.���뿹�ݰ���   2.�ſ�ŷڰ���" << endl;
	cout << "*****************************" << endl;
	cout << "���� : "; cin >> accChoose; cout << endl;

	switch (accChoose) {

	case 1:	// ���뿹�ݰ���
		cout << "[���뿹�ݰ��°���]" << endl;
		cout << "����ID: ";	cin >> id;
		cout << "��  ��: ";	cin >> name;
		cout << "�Աݾ�: ";	cin >> balance;
		cout << "������: "; cin >> rate;
		cout << endl;

		accArr[accNum++] = new NormalAccount(id, balance, name, rate);
		break;
	case 2:	// �ſ�ŷڰ���
		cout << "[�ſ�ŷڰ��°���]" << endl;
		cout << "����ID: ";	cin >> id;
		cout << "��  ��: ";	cin >> name;
		cout << "�Աݾ�: ";	cin >> balance;
		cout << "������: "; cin >> rate;
		cout << "�ſ���(1toA, 2toB, 3toC): "; cin >> gradechoose;
		cout << endl;

		accArr[accNum++] = new HighCreditAccount(id, balance, name, rate, gradechoose);
		break;
	default:
		cout << "Wrong Access....exit";
		break;
	}	
	cout << "���� ���� �Ϸ�...." << endl;
}

void AccountHandler::DepositMoney(void)		
{
	int money;
	int id;
	double interest;			// ���� 
	cout << "[��    ��]" << endl;
	cout << "����ID: ";	cin >> id;
	cout << "�Աݾ�: ";	cin >> money;

	for (int i = 0; i < accNum; i++)
	{
		if (accArr[i]->GetAccID() == id)
		{
			// 1)�Ա��Ϸ��� �ݾ��� �Ա��Ѵ�.
			accArr[i]->Deposit(money);

			// 2)��ü�ݾ׿� ���� ������ ������ ���ڸ� �Ա��Ѵ�.
			interest = accArr[i]->GetBalance() * accArr[i]->GetRate();
			accArr[i]->Deposit(interest);
			
			cout << "�ԱݿϷ�" << endl << endl;
			return;
		}
	}
	cout << "��ȿ���� ���� ID �Դϴ�." << endl << endl;
}

void AccountHandler::WithdrawMoney(void)
{
	int money;
	int id;
	cout << "[��    ��]" << endl;
	cout << "����ID: ";	cin >> id;
	cout << "��ݾ�: ";	cin >> money;

	for (int i = 0; i < accNum; i++)
	{
		if (accArr[i]->GetAccID() == id)
		{
			if (accArr[i]->Withdraw(money) == 0)
			{
				cout << "�ܾ׺���" << endl << endl;
				return;
			}

			cout << "��ݿϷ�" << endl << endl;
			return;
		}
	}
	cout << "��ȿ���� ���� ID �Դϴ�." << endl << endl;
}

void AccountHandler::ShowAllAccInfo(void) const
{
	for (int i = 0; i < accNum; i++)
	{
		accArr[i]->ShowAccInfo();
		cout << endl;
	}
}

// AccountHandler Ŭ������ ������ / �Ҹ��� 
AccountHandler::AccountHandler() :accNum(0) {}	// ���� ==> accNum = 0 �ʱ�ȭ
AccountHandler::~AccountHandler() {
	for (int i = 0; i < accNum; i++) {	// �����Լ����� accArr �� delete ���ִ� ���
		delete accArr[i];				// ��Ʈ�� Ŭ������ �Ҹ� �� �� delete ���� 
	}
}