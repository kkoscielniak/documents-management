/*
Du¿a firma zamierza zakupiæ system informatyczny wspomagaj¹cy proces obiegu dokumentacji.
W tej chwili proces wykorzystuje du¿e iloœci papieru. Wszystkie dokumenty z zewn¹trz jak i wewn¹trz firmy s¹ sk³adowane w archiwum.
Je¿eli jakiœ dzia³ firmy chce zajrzeæ do jakiegoœ dokumentu, to zg³asza siê do archiwum po jego kopiê (orygina³y dokumentów nigdy
nie opuszczaj¹ archiwum). Dla niektórych dokumentów jest zdefiniowana lista dzia³ów firmy, do których kopia jest wysy³ana automatycznie.
Ka¿demu dokumentowi jest przypisywany poziom tajnoœci (jawne, tajne, œciœle tajne) i tylko osoby o odpowiednich uprawnieniach maj¹
dostêp do jego zawartoœci. Dodatkowo dokumenty maj¹ przypisan¹ informacjê o tym kiedy i gdzie zosta³a wys³ana kopia oraz na jakiej
podstawie (automatycznie lub na podstawie wype³nionego wniosku). Kopia dokumentu mo¿e zostaæ przekazana z jednego dzia³u do innego
do dalszego przetwarzania (przy czym na kopii jest to odnotowane). Na koñcu gdy kopia nie jest potrzebna, to wraca do archiwum,
gdzie nastêpuje odnotowanie kto mia³ do niej dostêp i nastêpnie jest niszczona. Firma chce zakupiæ system, który pozwoli zaoszczêdziæ
koszt materia³ów i zmniejszyæ czas dostêpu do ¿¹danego dokumentu. Dokumenty sk³adowane w archiwum powinny znajdowaæ siê w systemie
i ca³y obieg powinien odbywaæ siê w ramach systemu z uwzglêdnieniem praw u¿ytkowników. System powinien umo¿liwiaæ stwierdzenie, kto
i kiedy mia³ kontakt z danym dokumentem lub jego kopi¹, do jakich dokumentów dany u¿ytkownik ma dostêp i wykrywaæ nieudane próby dostêpu
do tajnych i œciœle tajnych dokumentów.

Author: Krystian Koœcielniak
*/
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define DEBUG 1

using namespace std;

enum ConfidenceLevel
{
	PUBLIC,
	CONFIDENTIAL,
	STRICTLY_CONFIDENTIAL
};

enum AccessType
{
	AUTOMATICALLY,
	BY_APPLICATION
};

struct Document
{
	string name;
	string author;
	ConfidenceLevel confidenceLevel;
};

struct DocumentCopy
{
	Document document;
};

// class responsible for maintaining documents
// it is designed using Singleton design pattern
class Doc
{
private:
	Doc(){};
	Doc(Doc const&);
	void operator=(Doc const&);
	~Doc(){};
public:
	vector <Document> documents;

	static Doc& Instance()
	{
		static Doc instance;
		return instance;
	}

	void AddDocument(string name, string author, ConfidenceLevel level)
	{
		Document tmp;
		tmp.author = author;
		tmp.name = name;
		tmp.confidenceLevel = level;
		documents.push_back(tmp);
	}

	void RemoveDocument(string name)
	{
		for (vector<Document>::size_type i = 0; i != documents.size(); i++)
		{
			if (documents[i].name.compare(name) == 0)
			{
				documents.erase(documents.begin() + i);
			}
		}
	}

	Document GetDocument(string name)
	{
		for (vector<Document>::size_type i = 0; i != documents.size(); i++)
		{
			if (documents[i].name.compare(name) == 0)
			{
				return documents[i];
			}
		}
	}

	void ListDocuments()
	{
		for (vector<Document>::size_type i = 0; i != documents.size(); i++)
		{
			cout << documents[i].name << "\t" << documents[i].author << endl;
		}
	}
};

//class DocCopy
//{
//private:
//	DocCopy(){};
//	DocCopy(DocCopy const&);
//	void operator=(DocCopy const&);
//	~DocCopy(){};
//	vector< DocumentCopy > copies;
//
//public:
//	static DocCopy& Instance()
//	{
//		static DocCopy instance;
//		return instance;
//	}
//	void RequestCopy(User u, Document d)
//	{
//		// it is really needed?
//	}
//};

class User
{
public:
	vector <DocumentCopy> copies;
	string username;
	string password;
	ConfidenceLevel permissions;
	bool isAdmin;
	User(string name, ConfidenceLevel level, bool is, string p)
	{
		this->username = name;
		this->permissions = level;
		this->isAdmin = is;
		this->password = p;
	};
	~User(){};

	bool RequestCopy(string title)
	{
		DocumentCopy copy;
		copy.document = Doc::Instance().GetDocument(title);
		if (this->permissions >= copy.document.confidenceLevel)
		{
			this->copies.push_back(copy);
			// cout << copies.size();
			return true;
		}
		// cout << copies.size();
		return false;
		
	}

	void ListCopies()
	{
		cout << "Chuj";
		cout << this->copies.size();
		for (vector<DocumentCopy>::size_type i = 0; i != copies.size(); i++)
			cout << "Kurwa cycki ";
	}
};

class Users
{
private:
	Users(){};
	Users(Users const&);
	void operator=(Users const&);
	~Users(){};
public:
	vector<User> users;

	static Users& Instance()
	{
		static Users instance;
		return instance;
	}

	void AddUser(string name, string password, ConfidenceLevel level, bool IsAdmin)
	{
		User *u = new User(name, level, IsAdmin, password);
		this->users.push_back(*u);
	}

	User GetUser(string name)
	{
		for (vector<User>::size_type i = 0; i != users.size(); i++)
		{
			if (users[i].username == name)
				return users[i];
		}
	}
	// checks whether the user exists
	bool Exist(string name)
	{
		for (vector<User>::size_type i = 0; i != users.size(); i++)
		{
			if (users[i].username == name)
				return true;
		}
		return false;
	}
	// checks whether given user is admin
	bool IsAdmin(string name)
	{
		for (vector<User>::size_type i = 0; i != users.size(); i++)
		{
			if (users[i].username == name)
				if (users[i].isAdmin)
					return true;
		}
		return false;
	}

	void ListUsers()
	{
		for (vector<User>::size_type i = 0; i != users.size(); i++)
			cout << users[i].username << endl;
	}

	bool RequestCopy(string username, string title)
	{
		DocumentCopy copy;
		copy.document = Doc::Instance().GetDocument(title);

		for (vector<User>::size_type i = 0; i != users.size(); i++)
		{

			if (this->GetUser(username).permissions >= copy.document.confidenceLevel)
			{
				this->GetUser(username).copies.push_back(copy);
				cout << GetUser(username).copies.size();
				return true;
			}
			cout << GetUser(username).copies.size();
			
		}
		return false;

		
	}
};

int main()
{
	string username = "Erwin";	// current user;
	string password;
	string title;

	Doc::Instance().AddDocument("Raport", "Erwin Koczy", PUBLIC);
	Doc::Instance().AddDocument("Sprawozdanie", "Ministerstwo Magii", STRICTLY_CONFIDENTIAL);

	Users::Instance().AddUser("Erwin", "Korzy", CONFIDENTIAL, true);

	// login
	cout << "Login: ";
	cin >> username;
	if (Users::Instance().Exist(username))
	{
		cout << "Password: ";
		cin >> password;
		if (Users::Instance().GetUser(username).password != password)
		{
			cout << "Invalid password." << endl;
			system("PAUSE");
			return -1;
		}
		// else
		//currentUser = &Users::Instance().GetUser(username);
	}
	else
	{
		cout << "User does not exist." << endl;
		system("PAUSE");
		return -1;
	}

	char control = '!';
	while (control != 'q')
	{
		system("CLS");
		if (Users::Instance().GetUser(username).isAdmin)
			cout << "Hello " << Users::Instance().GetUser(username).username << " (as administrator)." << endl << endl;
		else
			cout << "Hello " << Users::Instance().GetUser(username).username << "." << endl;

		cout << "Select an option (q to quit)." << endl;
		cout << "1. Request a copy of document." << endl;
		cout << "2. List the copies of document." << endl;
		cout << "3. Return the copy of document." << endl;

		if (Users::Instance().GetUser(username).isAdmin)
		{
			cout << "4. Add a document to system." << endl;
			cout << "5. List the documents." << endl;
			cout << "5. Remove the document from system." << endl;
			cout << "6. Show log." << endl;
		}

		cin >> control;

		switch (control)
		{
		case '1':
			system("CLS");

			Doc::Instance().ListDocuments();
			cout << "Which one? Title: ";
			cin >> title;
			if (Users::Instance().RequestCopy(username, title))
			{
				cout << Users::Instance().GetUser(username).copies.size();
				cout << "Copy created." << endl;
			}
			else
			{
				cout << "You do not have access to this document." << endl;
			}

			system("PAUSE");
			break;
		case '2':
			system("CLS");
			cout << Users::Instance().GetUser("Erwin").copies[0].document.name;
			Users::Instance().GetUser("Erwin").ListCopies();

			system("PAUSE");
			break;
		case '5':
			system("cls");

			if (Users::Instance().GetUser(username).isAdmin)
			{
				Doc::Instance().ListDocuments();
			}
			else break;

			system("PAUSE");
			break;
		default:
			break;
		}
	}
	getchar();
	return 0;
}
