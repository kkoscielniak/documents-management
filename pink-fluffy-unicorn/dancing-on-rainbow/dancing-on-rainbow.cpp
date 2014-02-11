// dancing-on-rainbow.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>
#include <istream>
#include <vector>
#include <ctime>

using namespace std; 

enum ConfidenceLevel
{
	PUBLIC,
	CONFIDENTIAL,
	STRICTLY_CONFIDENTIAL
};

enum AccessType
{
	AUTOMATIC_DISPOSAL,
	BY_REQUEST
};

struct AccessInfo
{
	string user;
	AccessType type;
};

class Document
{
private:
	vector<string> automaticDisposals;
	vector<AccessInfo> accessingUsers;
	
public:
	string name;
	string author;
	ConfidenceLevel confidenceLevel;

	Document(string name, string author, ConfidenceLevel confidenceLevel)
	{
		this->name = name;
		this->author = author;
		this->confidenceLevel = confidenceLevel;
	}
	void AddAutomaticDisposal(string user)
	{
		automaticDisposals.push_back(user);
	}
	bool IsAutomaticallyDisposableFor(string user)
	{
		cout << "Erwin"; system("PAUSE");
		for (vector<string>::size_type i = 0; i != automaticDisposals.size(); i++)
		{
			if (automaticDisposals[i].compare(user) == 0)
			{
				cout << this->name << "Disposable for " << user;
				system("PAUSE");
				return true;
			}
			return false;
		}
	}
	void AddAccessingUser(string username, AccessType type)
	{
		AccessInfo info;
		info.user = username;
		info.type = type;
		this->accessingUsers.push_back(info);
	}
	void ListAccessingUsers()
	{
		for (vector<AccessInfo>::size_type i = 0; i != accessingUsers.size(); i++)
		{
			cout << accessingUsers[i].user;
			if (accessingUsers[i].type == AUTOMATIC_DISPOSAL)
				cout << " (automatically)." << endl;
			else
				cout << " (by request)." << endl;
		}
	}
	~Document(){};
};

class DocumentManager
{
private:
	DocumentManager(){};
	DocumentManager(DocumentManager const&);
	void operator=(DocumentManager const&);
	~DocumentManager(){};
public:
	vector<Document> documents;

	static DocumentManager& Instance()
	{
		static DocumentManager instance;
		return instance;
	}
	void AddDocument(string name, string author, ConfidenceLevel level)
	{
		Document *doc = new Document(name, author, level);
		documents.push_back(*doc);
	}
	void ListDocuments()
	{
		for (vector<Document>::size_type i = 0; i != documents.size(); i++)
		{
			cout << documents[i].name << "\t(by " << documents[i].author << ")" << endl;
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
};

class DocumentCopy : public Document
{
public:
	DocumentCopy(Document *doc) : Document(doc->name, doc->author, doc->confidenceLevel)
	{

	}
};

class User
{
private:
	bool isAdmin;
public:
	string username;
	string password;
	ConfidenceLevel permissions;
	vector <DocumentCopy> copies;

	User(string username, string password, ConfidenceLevel level, bool isAdmin)
	{
		this->username = username;
		this->password = password;
		this->permissions = level;
		this->isAdmin = isAdmin;
	}
	bool IsAdmin()
	{
		return this->isAdmin;
	}

	bool RequestCopy(Document *doc)
	{
		DocumentCopy *copy = new DocumentCopy(doc);
		if (this->permissions >= copy->confidenceLevel)
		{
			copies.push_back(copy);
			doc->AddAccessingUser(this->username, BY_REQUEST);
			return true;
		}
		else
		{
			cout << "You do not have required permissions. ";
			return false;
		}
	}

	bool ReturnCopy(string title)
	{
		for (vector<DocumentCopy>::size_type i = 0; i != copies.size(); i++)
		{
			if (copies[i].name.compare(title) == 0)
			{
				// delete &copies[i];
				copies.erase(copies.begin() + i);
				return true;
			}
		}
		return false;
	}

	bool PassCopy(string title, string username)
	{
		for (vector<DocumentCopy>::size_type i = 0; i != copies.size(); i++)
		{
			if (copies[i].name.compare(title) == 0)
			{
				DocumentCopy *tmp = &copies[i];
				copies.erase(copies.begin() + i);
				return true;
			}
		}
		return false;
	}

	int CopiesCount()
	{
		return copies.size();
	}

	void ListCopies()
	{
		for (vector<DocumentCopy>::size_type i = 0; i != copies.size(); i++)
		{
			cout << copies[i].name << "\t(by " << copies[i].author << ")" << endl;
		}
	}
	void GetAutomaticDisposals()
	{
		for (vector<Document>::size_type i = 0; i != DocumentManager::Instance().documents.size(); i++)
		{
			if (DocumentManager::Instance().documents[i].IsAutomaticallyDisposableFor(this->username))
			{
				if (this->RequestCopy(&DocumentManager::Instance().documents[i]))
				{
					DocumentManager::Instance().documents[i].AddAccessingUser(this->username, AUTOMATIC_DISPOSAL);
				}					
			}
		}
	}
};

class Log
{
private:
	Log(){};
	Log(Log const&);
	void operator=(Log const&);
	~Log(){};
public:
	vector<string> logs;

	static Log& Instance()
	{
		static Log instance;
		return instance;
	}
	void AddToLog(string username, string document)
	{
		time_t rawtime;
		string tmp;
		char buffer[80];
		struct tm * timeinfo;
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(buffer, 80, "%d - %m - %Y %I:%M", timeinfo);
		string str(buffer);
		tmp = username + " tries to access " + document + " @ " + str;
		logs.push_back(tmp);
	}
	void ListLogs()
	{
		for (vector<string>::size_type i = 0; i != logs.size(); i++)
		{
			cout << logs[i] << endl;
		}
	}
	void Serialize()
	{
		ofstream file;
		
		try
		{
			file.open("Log.txt");
			if (!file)
				throw 1;

			for (vector<string>::size_type i = 0; i != logs.size(); i++)
			{
				file << logs[i] << endl;
			};
			file.close();
		}
		catch (int error)
		{
			if (error == 1)
			{
				cout << "Couldn't serialize log." << endl;
				system("PAUSE");
			}
		}
	}
	void Deserialize()
	{
		ifstream file;
		try
		{
			file.open("Log.txt");
			if (!file)
				throw 1;
			
			string line;
			while (getline(file, line))
				logs.push_back(line);

		}
		catch (int error)
		{
			if (error == 1)
			{
				cout << "Couldn't deserialize log. ";
				system("PAUSE");
			}
		}
	}
};

void PrintUnderline()
{
	string s = "=";
	for (int i = 0; i < 80; i++)
	{
		cout << s;
	}
	cout << endl;
}

int main(int argc, _TCHAR* argv[])
{	
	string title;

	Log::Instance().Deserialize();

	User *u = new User("Erwin", "Korzy", CONFIDENTIAL, true);

	DocumentManager::Instance().AddDocument("Alicja", "Erwin Korzy", PUBLIC);
	//DocumentManager::Instance().GetDocument("Alicja").AddAutomaticDisposal("Erwin");
	DocumentManager::Instance().AddDocument("Erwinia", "Alojz Korzy", STRICTLY_CONFIDENTIAL);

	//u->GetAutomaticDisposals();

	char control = '!';
	while (control != 'q')
	{
		system("CLS");
		
		cout << "Select an option (q to quit)." << endl;
		cout << "1. Request a copy of document." << endl;
		cout << "2. List the copies of document." << endl;
		cout << "3. Return the copy of document." << endl;
		cout << "4. Pass the copy further." << endl;

		
		if (u->IsAdmin())
		{
			cout << endl;
			cout << "5. Document information." << endl;
		}
		
		cout << "Option: ";	cin >> control;


		switch (control)
		{
		case '1':
			system("CLS");
			cout << "REQUEST A COPY" << endl;
			PrintUnderline();
			DocumentManager::Instance().ListDocuments();
			cout << endl << "Title: ";
			cin >> title;
			if (u->RequestCopy(&DocumentManager::Instance().GetDocument(title)))
			{
				cout << "Copy of document granted." << endl;
			}
			else
			{
				cout << "This incident will be reported." << endl;
				Log::Instance().AddToLog(u->username, title);
			}
			system("PAUSE");
			break;
		case '2':
			system("CLS");
			cout << "LIST OF COPIES" << endl;
			PrintUnderline();
			if (u->CopiesCount() > 0)
				u->ListCopies();
			else
				cout << "You do not have any copies right now." << endl;
			cout << endl;
			system("PAUSE");
			break;
		case '3':
			system("CLS");
			cout << "RETURN A COPY" << endl << endl;
			u->ListCopies();
			cout << endl << "Title: ";
			cin >> title;
			if (u->ReturnCopy(title))
			{
				cout << "Copy returned to archive. " << endl;
			}
			else
			{
				cout << "Couldn't return the copy to archive." << endl;
			}
			system("PAUSE");
			break;
		case '4':
			system("CLS");

			break;
		case '5':
			system("CLS");
			cout << "DOCUMENT INFO" << endl;
			PrintUnderline();
			DocumentManager::Instance().ListDocuments();
			cout << endl << "Which document? "; cin >> title;
			system("CLS");
			cout << "ACCESSING USERS" << endl;
			PrintUnderline();
			DocumentManager::Instance().GetDocument(title).ListAccessingUsers();
			system("PAUSE");
			break;
		case '6':
			system("cls");
			cout << "SHOW ERROR LOG" << endl;
			PrintUnderline();
			Log::Instance().ListLogs();
			system("PAUSE");
			break;
		default:
			break;
		}
	}

	Log::Instance().Serialize();

	delete u;
	return 0;
}

