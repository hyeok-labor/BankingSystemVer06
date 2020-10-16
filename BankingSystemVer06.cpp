/*
	------------------------------------------------
	BankingSystemVer06
	edited by Hyeok
	------------------------------------------------

	1. Account 클래스를 상속하는 2개의 클래스 추가 
		- NormalAccount : 보통예금계좌
			=> 이율정보 등록 
		- HighCreditAccount : 신용신뢰계좌
			=> 보통예금계좌와 같이 이율정보등록
			=> 고객 신용등급을 A,B,C로 나누고 개좌개설시 이 정보를 등록 
			=> A,B,C 등급별로 각각 기본 이율에 7,4,2% 이율을 추가 제공 
		- 입금시 이자가 원금에 더해지는 것으로 간주
	2. 모든 계좌에 다음 조건을 적용
		- 계좌 개설 과정에서 초기 입금되는 금액에 대해서는 이자를 계산하지 않는다
		- 계좌 개설 이후 별도의 입금과정을 거칠 때에만 이자가 발생하는 것으로 간주 
		- 이자의 계산과정에서 발생하는 소수점 이하의 금액은 무시한다.	
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
	int accID;      // 계좌번호
	int balance;    // 잔액
	char * cusName; // 고객이름
public:
	Account(int ID, int money, char * name)
		: accID(ID), balance(money)
	{
		cusName = new char[strlen(name) + 1];
		strcpy(cusName, name);
	}

	Account(const Account & ref)			// 복사생성자 
		: accID(ref.accID), balance(ref.balance)
	{
		cusName = new char[strlen(ref.cusName) + 1];
		strcpy(cusName, ref.cusName);
	}

	void Deposit(int money)
	{
		balance += money;
	}

	int Withdraw(int money)    // 출금액 반환, 부족 시 0 반환
	{
		if (balance < money)
			return 0;

		balance -= money;
		return money;
	}

	void ShowAccInfo() const
	{
		cout << "계좌ID: " << accID << endl;
		cout << "이  름: " << cusName << endl;
		cout << "잔  액: " << balance << endl;
	}

	virtual double GetRate() const {				// NormalAccount, HighCreditAccount Class 에서 사용하기 위해 부모클래스에 virtual function 생성
		return 0;									// 내용무
	}

	int GetAccID() const { return accID; }
	int GetBalance() const { return balance; }
	~Account()
	{
		delete[]cusName;
	}
};

// 보통예금계좌
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

// 신용신뢰계좌
class HighCreditAccount : public NormalAccount {
private:
	int grade = 0;	// 신용등급 
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
		switch (grade) {				// 신용 등급에 따라 이율 차등 적용 
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
컨트롤 클래스 :
	프로그램 전체의 기능 담당 => 기능적 성격이 강함
	컨트롤 클래스만으로 프로그램 전체의 기능과 흐름 파악가능
*/
class AccountHandler
{
private:
	Account * accArr[100];	// 객체지향에서는 전역개념이 존재하므로 가급적 사용하지 않는다.
	int accNum;				// 원래 전역으로 선언되어있던 변수들을 컨트롤 클래스의 멤버함수로 지정함

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
		cout << "선택: ";
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
	cout << "1. 계좌개설" << endl;
	cout << "2. 입    금" << endl;
	cout << "3. 출    금" << endl;
	cout << "4. 계좌정보 전체 출력" << endl;
	cout << "5. 프로그램 종료" << endl;
}

void AccountHandler::MakeAccount(void)
{
	int id;

	char name[NAME_LEN];
	int balance;
	double rate;			
	int accChoose;					// 계좌 선택을 위한 변수 선언 
	int gradechoose;				// 신용등급 선택을 위한 변수 선언 
	
	cout << "[계좌 종류 선택]" << endl;
	cout << "1.보통예금계좌   2.신용신뢰계좌" << endl;
	cout << "*****************************" << endl;
	cout << "선택 : "; cin >> accChoose; cout << endl;

	switch (accChoose) {

	case 1:	// 보통예금계좌
		cout << "[보통예금계좌개설]" << endl;
		cout << "계좌ID: ";	cin >> id;
		cout << "이  름: ";	cin >> name;
		cout << "입금액: ";	cin >> balance;
		cout << "이자율: "; cin >> rate;
		cout << endl;

		accArr[accNum++] = new NormalAccount(id, balance, name, rate);
		break;
	case 2:	// 신용신뢰계좌
		cout << "[신용신뢰계좌개설]" << endl;
		cout << "계좌ID: ";	cin >> id;
		cout << "이  름: ";	cin >> name;
		cout << "입금액: ";	cin >> balance;
		cout << "이자율: "; cin >> rate;
		cout << "신용등급(1toA, 2toB, 3toC): "; cin >> gradechoose;
		cout << endl;

		accArr[accNum++] = new HighCreditAccount(id, balance, name, rate, gradechoose);
		break;
	default:
		cout << "Wrong Access....exit";
		break;
	}	
	cout << "계좌 개설 완료...." << endl;
}

void AccountHandler::DepositMoney(void)		
{
	int money;
	int id;
	double interest;			// 이자 
	cout << "[입    금]" << endl;
	cout << "계좌ID: ";	cin >> id;
	cout << "입금액: ";	cin >> money;

	for (int i = 0; i < accNum; i++)
	{
		if (accArr[i]->GetAccID() == id)
		{
			// 1)입금하려는 금액을 입금한다.
			accArr[i]->Deposit(money);

			// 2)전체금액에 대한 이율을 적용해 이자를 입금한다.
			interest = accArr[i]->GetBalance() * accArr[i]->GetRate();
			accArr[i]->Deposit(interest);
			
			cout << "입금완료" << endl << endl;
			return;
		}
	}
	cout << "유효하지 않은 ID 입니다." << endl << endl;
}

void AccountHandler::WithdrawMoney(void)
{
	int money;
	int id;
	cout << "[출    금]" << endl;
	cout << "계좌ID: ";	cin >> id;
	cout << "출금액: ";	cin >> money;

	for (int i = 0; i < accNum; i++)
	{
		if (accArr[i]->GetAccID() == id)
		{
			if (accArr[i]->Withdraw(money) == 0)
			{
				cout << "잔액부족" << endl << endl;
				return;
			}

			cout << "출금완료" << endl << endl;
			return;
		}
	}
	cout << "유효하지 않은 ID 입니다." << endl << endl;
}

void AccountHandler::ShowAllAccInfo(void) const
{
	for (int i = 0; i < accNum; i++)
	{
		accArr[i]->ShowAccInfo();
		cout << endl;
	}
}

// AccountHandler 클래스의 생성자 / 소멸자 
AccountHandler::AccountHandler() :accNum(0) {}	// 생성 ==> accNum = 0 초기화
AccountHandler::~AccountHandler() {
	for (int i = 0; i < accNum; i++) {	// 메인함수에서 accArr 를 delete 해주는 대신
		delete accArr[i];				// 컨트롤 클래스가 소멸 될 때 delete 해줌 
	}
}