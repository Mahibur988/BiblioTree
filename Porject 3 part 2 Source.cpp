/**************************************************************
* Programmer Name: Mahibur Mahi
* File Name: Project 3 part 2
* Date of Last Modification: 4/23/2026
* Description of program: This program is about a library management system using a binary search tree (BST). It allows users to view available books, borrow books, and donate books. The program reads book data from a file and organizes it in a BST for efficient searching and management.
* Resources (including people) that helped you: AI (Chatgpt)
*
**************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class Book {
public:
    string title;
    string lowerTitle;
    string firstName;
    string lastName;

    Book* left;
    Book* right;

    Book(string t, string f, string l) {
        title = t;
        firstName = f;
        lastName = l;
        lowerTitle = toLower(t);
        left = nullptr;
        right = nullptr;
    }

	static string toLower(string s) {  // Convert string to lowercase .
        for (char& c : s) {
            c = tolower(c);
        }
        return s;
    }
};

class BST {
private:
    Book* root;

    Book* insert(Book* node, Book* newBook) {   //insert helper fucntion
        if (node == nullptr)
            return newBook;

        if (newBook->lowerTitle < node->lowerTitle)
            node->left = insert(node->left, newBook);
        else
            node->right = insert(node->right, newBook);

        return node;
    }

	Book* findMin(Book* node) {               //find smallest node in the right subtree
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

    Book* remove(Book* node, string key, bool& found) {  //remove helper function
        if (node == nullptr)
            return nullptr;

        if (key < node->lowerTitle)
            node->left = remove(node->left, key, found);

        else if (key > node->lowerTitle)
            node->right = remove(node->right, key, found);

        else {
            found = true;

			if (node->left == nullptr && node->right == nullptr) {  //case 1: no children
                delete node;
                return nullptr;
            }

            if (node->left == nullptr) {           //case 2: only right child
                Book* temp = node->right;
                delete node;
                return temp;
            }

            if (node->right == nullptr) {
                Book* temp = node->left;
                delete node;
                return temp;
            }

			Book* temp = findMin(node->right);     //case 3: two children

            node->title = temp->title;
            node->lowerTitle = temp->lowerTitle;
            node->firstName = temp->firstName;
            node->lastName = temp->lastName;

            node->right = remove(node->right, temp->lowerTitle, found);
        }

        return node;
    }

    void inorder(Book* node, int& count, int limit) { // book displayed in sorter order
        if (node == nullptr || count >= limit)
            return;

        inorder(node->left, count, limit);

        if (count < limit) {
            cout << node->title << " by "
                << node->firstName << " "
                << node->lastName << endl;
            count++;
        }

        inorder(node->right, count, limit);
    }

    void destroy(Book* node) {       //delete all nodes
        if (node == nullptr) return;

        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    BST() {
        root = nullptr;
    }

    ~BST() {
        destroy(root);
    }

    void insert(string title, string first, string last) {
        Book* newBook = new Book(title, first, last);
        root = insert(root, newBook);
    }

    void remove(string title) {
        bool found = false;
        root = remove(root, title, found);

        if (found)
            cout << "Book borrowed successfully!\n";
        else
            cout << "Book not available.\n";
    }

    void display(int n) {
        int count = 0;
        inorder(root, count, n);
    }
};

int main() {
	BST tree;   // tree object created

    int choice;
    cout << "Choose file:\n1. Small\n2. Large\n";
    cin >> choice;
    cin.ignore();

    string filename;

	if (choice == 1)      // choice for small or large file
        filename = "books_small_randomized.txt";
    else
        filename = "books_large_randomized.txt";

    ifstream file;

    try {
        file.open(filename);

        if (!file.is_open()) {
            throw ifstream::failure("Could not open file");
        }

        cout << "Opening file: " << filename << endl;
    }
    catch (ifstream::failure& e) {
        cout << "Error opening file!\n";
        return 1;
    }

    string line;

	while (getline(file, line)) {  // read each line of the file
        stringstream ss(line);

        string title, author;
        getline(ss, title, '\t');
        getline(ss, author);

        string first, last;
        stringstream nameStream(author);
        nameStream >> first >> last;

        tree.insert(title, first, last);
    }

    file.close();

    int option;

    do {
        cout << "\n===== MENU =====\n";
        cout << "1. View books\n";
        cout << "2. Borrow book\n";
        cout << "3. Donate book\n";
        cout << "4. Exit\n";
        cout << "Choice: ";
        cin >> option;
        cin.ignore();

		if (option == 1) {   // view books
            int n;
            cout << "How many books? ";
            cin >> n;
            cin.ignore();
            tree.display(n);
        }

		else if (option == 2) {  //borrow book
            string title;
            cout << "Enter title: ";
            getline(cin, title);

            title = Book::toLower(title);
            tree.remove(title);
        }

		else if (option == 3) {       //donate book
            string title, first, last;

            cout << "Enter title: ";
            getline(cin, title);

            cout << "Enter author's first name: ";
            getline(cin, first);

            cout << "Enter author's last name: ";
            getline(cin, last);

            tree.insert(title, first, last);

            cout << "Thank you for your donation!\n";
            cout << title << " by " << first << " " << last
                << " added successfully!\n";
        }

    } while (option != 4);

    cout << "Goodbye!\n";
    return 0;
}