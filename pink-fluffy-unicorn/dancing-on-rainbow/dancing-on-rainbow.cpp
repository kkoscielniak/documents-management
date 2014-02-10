// dancing-on-rainbow.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std; 

enum ConfidenceLevel
{
	PUBLIC,
	CONFIDENTIAL,
	STRICTLY_CONFIDENTIAL
};

class Document
{
private:
	vector<string> automaticDisposals;
	
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
		for (vector<string>::size_type i = 0; i != automaticDisposals.size(); i++)
		{
			if (automaticDisposals[i].compare(user) == 0)
			{
				return true;
			}
			return false;
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
		cout << doc->name;
		documents.push_back(*doc);
		ListDocuments();
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
public:
	string username;
	string password;
	ConfidenceLevel permissions;
	vector <DocumentCopy> copies;

	User(string username, string password, ConfidenceLevel level)
	{
		this->username = username;
		this->password = password;
		this->permissions = level;
	}

	bool RequestCopy(Document *doc)
	{
		DocumentCopy *copy = new DocumentCopy(doc);
		if (this->permissions >= copy->confidenceLevel)
		{
			copies.push_back(copy);
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
				this->RequestCopy(&DocumentManager::Instance().documents[i]);
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

	// Document *doc = new Document("Alicja", "Erwin Koczy", PUBLIC);
	/*DocumentManager::Instance().AddDocument("Alicja", "Erwin Koczy", PUBLIC);
	DocumentManager::Instance().ListDocuments();
*/
	User *u = new User("Erwin", "Korzy", CONFIDENTIAL);
/*
	u->RequestCopy(doc);
	Document *doc2 = new Document("Erwinia", "Alojz", PUBLIC);
	u->RequestCopy(doc2);
	u->ListCopies();
	u->ReturnCopy("Erwinia");
	u->ListCopies();

	system("PAUSE");*/

	DocumentManager::Instance().AddDocument("Alicja", "Erwin Korzy", PUBLIC);
	DocumentManager::Instance().GetDocument("Alicja").AddAutomaticDisposal("Erwin");
	DocumentManager::Instance().AddDocument("Erwinia", "Alojz Korzy", STRICTLY_CONFIDENTIAL);

	u->GetAutomaticDisposals();

	char control = '!';
	while (control != 'q')
	{
		system("CLS");
		
		cout << "Select an option (q to quit)." << endl;
		cout << "1. Request a copy of document." << endl;
		cout << "2. List the copies of document." << endl;
		cout << "3. Return the copy of document." << endl;

		cin >> control;

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
				cout << "Couldn't request a copy of document." << endl;
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
		case '5':
			system("cls");

			system("PAUSE");
			break;
		default:
			break;
		}
	}
	delete u;

	return 0;
}

