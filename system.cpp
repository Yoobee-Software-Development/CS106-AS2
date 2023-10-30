#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <stdlib.h>
#include <cstdlib>
using namespace std;



// Structure to store user information.
struct User {
	string username;
	string password;
	string type; // "admin" or "user"
};

// Define Functions
void loginuser();
void modifyBook(string title);
void adminmenu(string username);
void viewaccounts();
void usermenu(string username);
void createuser();
void exitprogram();
int main();

// Structure to store book information.
struct Book {
	string title;
	string author;
	int year;
};

// Function to add a book.
void addBook() {
	Book newBook;
	cin.ignore();  // To clear the newline character in the buffer
	cout << "Enter book title: ";
	getline(cin, newBook.title);
	cout << "Enter book author: ";
	getline(cin, newBook.author);
	cout << "Enter book year: ";
	cin >> newBook.year;
	cin.ignore();  // Add this line

	ofstream booksfile("Books.txt", ios::app);
	booksfile << newBook.title << " | " << newBook.author << " | " << newBook.year << endl;
	booksfile.close();

	cout << "Book added successfully!" << endl;
}

// Function to view the books.
void viewBooks() {
	int choice;
	do {
		system("clear"); // Clear Screen
		cout << "Books:\n";
		string line;
		ifstream booksfile("Books.txt");
		while (getline(booksfile, line)) {
			stringstream ss(line);
			Book currentBook;
			getline(ss, currentBook.title, '|');
			getline(ss, currentBook.author, '|');
			ss >> currentBook.year;
			// Trim leading and trailing spaces
			currentBook.title = currentBook.title.substr(currentBook.title.find_first_not_of(' '));
			currentBook.title = currentBook.title.substr(0, currentBook.title.find_last_not_of(' ') + 1);
			currentBook.author = currentBook.author.substr(currentBook.author.find_first_not_of(' '));
			currentBook.author = currentBook.author.substr(0, currentBook.author.find_last_not_of(' ') + 1);
			cout << "Title: " << currentBook.title << ", Author: " << currentBook.author << ", Year: " << currentBook.year << endl;
		}
		booksfile.close();

		cout << "\n1. Add Book" << endl;
		cout << "2. Modify Book" << endl;
		cout << "3. Return to Admin Menu" << endl;
		cout << "Enter your choice: ";
		cin >> choice;

		if (choice == 1) {
			addBook();
			cout << "Book added successfully.\n";
		}
		else if (choice == 2) {
			string title;
			cout << "Enter the title of the book to modify: ";
			cin.ignore();
			getline(cin, title);
			modifyBook(title);
			cout << "Book modified successfully.\n";
		}
	} while (choice != 3);

	system("clear"); //Clear Screen
}


// Function to modify a book.
void modifyBook(string title) {
	vector<Book> books;
	Book currentBook;
	ifstream booksfile("Books.txt");
	while (booksfile >> currentBook.title >> currentBook.author >> currentBook.year) {
		if (currentBook.title != title) {
			books.push_back(currentBook);
		}
		else {
			cout << "Enter new author: ";
			cin >> currentBook.author;
			cout << "Enter new year: ";
			cin >> currentBook.year;
			books.push_back(currentBook);
		}
	}
	booksfile.close();

	ofstream booksfileout("Books.txt");
	for (const auto& book : books) {
		booksfileout << book.title << " " << book.author << " " << book.year << endl;
	}
	booksfileout.close();
}

void createuser() {

	system("clear"); // clear screen

	User newuser;
	cout << "Enter Username: ";
	cin >> newuser.username;
	cout << "Enter Password: ";
	cin >> newuser.password;
	cout << "Enter User Type (admin or user): ";
	cin >> newuser.type;

	ofstream usersfile("Accounts.txt", ios::app);
	usersfile << newuser.username << " " << newuser.password << " " << newuser.type << endl;
	usersfile.close();

	cout << "Account creation Successful!" << endl;

	main();
}
// Function to delete a account by specifying the username.
void deleteaccount(string username) {
	vector<User> users;
	User currentuser;
	ifstream usersfile("Accounts.txt");
	while (usersfile >> currentuser.username >> currentuser.password >> currentuser.type) {
		if (currentuser.username != username) {
			users.push_back(currentuser);
		}
	}
	usersfile.close();

	ofstream usersfileout("Accounts.txt");
	for (const auto& user : users) {
		usersfileout << user.username << " " << user.password << " " << user.type << endl;
	}
	usersfileout.close();
}

// View Accounts function.
void viewaccounts() {
	int choice;
	do {

		system("clear"); // Clear Screen
		cout << "All Accounts:\n";
		User currentuser;
		ifstream usersfile("Accounts.txt");
		while (usersfile >> currentuser.username >> currentuser.password >> currentuser.type) {
			cout << "Account Name: " << currentuser.username << ", Type: " << currentuser.type << endl;
		}
		usersfile.close();
		cout << "\n1. Delete Account" << endl;
		cout << "2. Return to Main Menu" << endl;
		cout << "Enter your choice: ";
		cin >> choice;

		if (choice == 1) {
			string username;
			cout << "Enter the username of the account to delete: ";
			cin >> username;
			deleteaccount(username);
			cout << "Account deleted successfully.\n";
		}
	} while (choice != 2);

	system("clear"); //Clear Screen
}


// Exit Program
void exitprogram() {
	cout << "Exiting program. Goodbye!" << endl;
	exit(0);
}

// Login
void loginuser() {   // Login User

	system("clear"); // clear screen
	string username, password;
	int attempts = 0; // Keeps track of attempted log ins.
	bool found = false;
	User currentuser;
	while (attempts < 3 && !found) { // Allows up to 3 login attempts.
		cout << "Enter Username: ";
		cin >> username;
		cout << "Enter Password: ";
		cin >> password;

		if (username.empty() || password.empty()) {
			cout << "Username and password cannot be nil." << endl;
			attempts++;
			continue;
		}

		ifstream usersfile("Accounts.txt");
		while (usersfile >> currentuser.username >> currentuser.password >> currentuser.type) {
			if (currentuser.username == username && currentuser.password == password) {
				found = true;
				break;
			}
		}
		usersfile.close();

		if (!found) {
			cout << "Invalid username or password. You have " << 2 - attempts << " attempts left." << endl;
			attempts++;
		}
	}


	if (found) {
		cout << "Login Successful!" << endl;
		if (currentuser.type == "admin") {
			adminmenu(username);
		}
		else {
			usermenu(username);
		}
	}
	else {
		cout << "Too Many failed login attempts. Please try again later." << endl;
	}

}

// Admin Menu
void adminmenu(string username)
{
	system("clear"); // clear screen
	int choice;
	do {
		cout << "Admin Menu:" << endl;
		cout << "1. View Books" << endl;
		cout << "2. Logout" << endl;
		cout << "3. Exit" << endl;
		cout << "Enter your choice: ";
		cin >> choice;

		switch (choice) {
		case 1:
			viewBooks();
			break;
		case 2:
			main();
			break;
		case 3:
			exitprogram();
			break;
		default:
			cout << "Invalid choice." << endl;
			break;
		}
	} while (choice != 3);
}

// User Menu
void usermenu(string username)
{

	system("clear"); // clear screen
	int choice;
	do {
		cout << "1. Logout" << endl;
		cout << "2. Exit" << endl;
		cout << "Enter your choice: ";
		cin >> choice;

		switch (choice) {
		case 1:
			main();
			break;
		case 2:
			exitprogram();
			break;
		default:
			cout << "Invalid choice." << endl;
			break;
		}
	} while (choice != 3);

}

// Main
int main()
{

	system("clear");  // clear screen
	int choice;
	cout << "1. Login" << endl;
	cout << "2. Register" << endl;
	cout << "3. Exit" << endl;
	cout << "Enter your choice: ";
	cin >> choice;

	if (choice == 1) {
		loginuser();
	}
	else if (choice == 2) {
		createuser();
	}
	else if (choice == 3) {
		exitprogram();
	}
	return 0;
};