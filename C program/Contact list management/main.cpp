#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <limits>
using namespace std;

#define MAX 1000
#define nameWidth 20
#define genderWidth 10
#define ageWidth 6
#define phoneWidth 15
#define addressWidth 30
    
//显示菜单
void Menu() {
    cout << "Welcome to the contact list management system!" << endl << endl;
    cout << "***********************************" << endl;
    cout << "***** Contact List Management *****" << endl;
    cout << "***** 1. Add contact          *****" << endl;
    cout << "***** 2. Display all contacts *****" << endl;
    cout << "***** 3. Delete contact       *****" << endl;
    cout << "***** 4. Search contact       *****" << endl;
    cout << "***** 5. Modify contacts      *****" << endl;
    cout << "***** 6. Clear all contacts   *****" << endl;
    cout << "***** 0. Exit                 *****" << endl;
    cout << "***********************************" << endl;

}
// Contact 结构体，用于存储单个联系人的信息
struct Contact {
    string name;
    int gender;
    int age;
    string phoneNumber;
    string address;
};
// ContactList 结构体，用于存储联系人列表
struct ContactList {
    struct Contact contacts[MAX];
    int count;
};
// 添加联系人函数
void AddContact (struct ContactList &cL) {
    struct Contact con;
    if(cL.count >= MAX) {
        cout << "Contact list is full!" << endl;
        return;
    }

    getchar();// 清除输入缓冲区中的换行符
    cout << "Enter name(max 20 characters): ";
    while(1) {
        getline(cin, con.name);// 使用getline读取字符串，允许包含空格
        if(con.name.length() == 0) {
            cout << "Name cannot be empty, please enter again: ";
            continue;
        }
        if(con.name.length() > nameWidth) {
            cin.clear(); // Clear the error flag on cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the rest of the line
            cout << "Name is too long, please enter again: ";
        } else {
            break;
        }
    }
    
    cout << "Enter gender: " << "1 -- man" << endl << "2 -- woman" << endl;
    while(1) {
        cin >> con.gender;
        if(cin.fail()) {
            cout << "Invalid input, please enter again: " << endl;
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            continue;
        }
        if(con.gender == 1 || con.gender == 2) {
            break;
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input, please enter again: " << endl;
        }
    }

    cout << "Enter age: ";
    while(1) {
        cin >> con.age;
        if(cin.fail()) {
            cout << "Invalid input, please enter again: " << endl;
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if(con.age < 0 || con.age > 120) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid age, please enter again: " << endl;
        } else {
            break;
        }
    }

    cout << "Enter phone number(max 15 characters): ";
    while(1) {
        getline(cin, con.phoneNumber);
        if(con.phoneNumber.length() == 0) {
            cout << "Phone number cannot be empty, please enter again: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            continue;
        }
        if(con.phoneNumber.length() > phoneWidth) {
            scanf("%*[^\n]");
            cin.clear();
            cout << "Phone number is too long, please enter again: ";
        } else {
            break;
        }
        
        
    }
    
    cout << "Enter address(max 30 characters): ";
    while(1) {
        getline(cin, con.address);
        if(con.address.length() == 0) {
            cout << "Address cannot be empty, please enter again: ";
            continue;
        }
        if(con.address.length() > addressWidth) {
            scanf("%*[^\n]");
            cin.clear();
            cout << "Address is too long, please enter again: ";
        } else {
            break;
        }
    }
    
    cL.contacts[cL.count] = con;
    cout << "Contact added successfully!" << endl;
}
// 显示所有联系人函数
void DisplayContact (struct ContactList cL) {
    if(cL.count == 0) {
        cout << "No contacts available!" << endl;
        return;
    }

    cout << left << setw(nameWidth) << "Name" 
         << setw(genderWidth) << "Gender" 
         << setw(ageWidth) << "Age" 
         << setw(phoneWidth) << "Phone Number" 
         << "Address" << endl;
    cout << string(nameWidth + genderWidth + ageWidth + phoneWidth + addressWidth, '-') << endl;
    
    for (int i = 0; i < cL.count; i++) {
        string gender;
        cL.contacts[i].gender == 1 ? gender = "Man" : gender = "Woman";
         
        cout << left << setw(nameWidth) << cL.contacts[i].name 
            << setw(genderWidth) << gender 
            << setw(ageWidth) << cL.contacts[i].age 
            << setw(phoneWidth) << cL.contacts[i].phoneNumber 
            << cL.contacts[i].address << endl;
    }
    cout << "Total contacts: " << cL.count << endl;
}
// 删除联系人函数
void DeleteContact (struct ContactList &cL) {
    string name;
    cout << "Enter name of contact to delete: ";cin >> name;
    for (int i = 0; i < cL.count; i++) {
        if (cL.contacts[i].name == name) {
            for (int j = i; j < cL.count - 1; j++) {
                cL.contacts[j] = cL.contacts[j + 1];
            }
            cL.count--;
            cout << "Contact deleted successfully!" << endl;
            return;
        }
    }
    cout << "Contact not found!" << endl;
}
// 搜索联系人函数
void SearchContact (struct ContactList cL) {
    string name;
    cout << "Enter name of contact to search: ";cin >> name;
    bool found = false;
    for (int i = 0; i < cL.count; i++) {
        if (cL.contacts[i].name == name) {
            found = true;
            string gender;
            cL.contacts[i].gender == 1 ? gender = "Man" : gender = "Woman";
        
            cout << left << setw(nameWidth) << "Name" 
                << setw(genderWidth) << "Gender" 
                << setw(ageWidth) << "Age" 
                << setw(phoneWidth) << "Phone Number" 
                << "Address" << endl;

            cout << string(nameWidth + genderWidth + ageWidth + phoneWidth + 30, '-') << endl;
   
            cout << left << setw(nameWidth) << cL.contacts[i].name 
                << setw(genderWidth) << gender 
                << setw(ageWidth) << cL.contacts[i].age 
                << setw(phoneWidth) << cL.contacts[i].phoneNumber 
                << cL.contacts[i].address << endl;
        
            return;
        }
    }
    if (!found) 
        cout << "Contact not found!" << endl;
}
// 修改联系人函数
void ModifyContact (struct ContactList &cL) {
    string name;
    cout << "Enter name of contact to modify: ";cin >> name;
    for (int i = 0; i < cL.count; i++) {
        if (cL.contacts[i].name == name) {
            cout << "Enter new name: ";cin >> cL.contacts[i].name;
            
            cout << "Enter gender: " << "1 -- man" << endl << "2 -- woman" << endl;
            while(1) {
                cin >> cL.contacts[i].gender;
                if(cL.contacts[i].gender == 1 || cL.contacts[i].gender == 2)
                    break;
                else
                    cout << "Invalid input, please enter again: " << endl;
                scanf("%*[^\n]"); // Clear the input buffer
                cin.clear(); // Clear the error flag on cin
            }
            
            cout << "Enter age: ";cin >> cL.contacts[i].age;
            cout << "Enter phone number: ";cin >> cL.contacts[i].phoneNumber;
            cout << "Enter address: ";cin >> cL.contacts[i].address;
            
            cout << "Contact modified successfully!" << endl;
            return;
        }
    }
    cout << "Contact not found!" << endl;
}
// 清空所有联系人函数
void ClearContact (struct ContactList &cL) {
    cL.count = 0;
    cout << "All contacts cleared!" << endl;
}
// 加载联系人函数
void LoadContacts(struct ContactList &cL) {
    ifstream inFile("contacts.txt");
    if (!inFile) {
        cout << "No contacts file found, starting with empty contact list." << endl;
        return;
    }
    
    string line;
    cL.count = 0;
    
    while (getline(inFile, line) && cL.count < MAX) {
        stringstream ss(line);
        string token;
        
        if (!getline(ss, token, ',')) break;
        cL.contacts[cL.count].name = token;
        
        if (!getline(ss, token, ',')) break;
        cL.contacts[cL.count].gender = stoi(token);
        
        if (!getline(ss, token, ',')) break;
        cL.contacts[cL.count].age = stoi(token);
        
        if (!getline(ss, token, ',')) break;
        cL.contacts[cL.count].phoneNumber = token;
        
        if (!getline(ss, token, '\n')) break;
        cL.contacts[cL.count].address = token;
        
        cL.count++;
    } 
    
    inFile.close();
    cout << cL.count << " contacts loaded successfully!" << endl;
}
// 保存联系人函数
void SaveContacts(const struct ContactList &cL) {
    ofstream outFile("contacts.txt");
    if (!outFile) {
        cout << "Error: Cannot open file for writing!" << endl;
        return;
    }
    
    for (int i = 0; i < cL.count; i++) {
        outFile << cL.contacts[i].name << ","
                << cL.contacts[i].gender << ","
                << cL.contacts[i].age << ","
                << cL.contacts[i].phoneNumber << ","
                << cL.contacts[i].address << endl;
    }
    
    outFile.close();
    cout << cL.count << " contacts saved successfully!" << endl;
}

int main() {
    int choice;
    struct ContactList cL;
    cL.count = 0;

    Menu();

    cout << "Loading contacts from file..." << endl;
    LoadContacts(cL);

    while(1) {
    
        cout << "Enter your choice: ";
        cin >> choice;
        if (cin.fail()) {
            scanf("%*[^\n]"); // Clear the input buffer
            cin.clear(); // Clear the error flag on cin
            cout << "Invalid input, please enter a number." << endl;
            system("pause");
            system("cls"); 
            continue; 
        }
        switch(choice) {
            case 1:// Add contact
                AddContact(cL);
                cL.count++;
                system("pause");
                break;
            case 2:// Display all contacts
                DisplayContact(cL);
                system("pause");
                break;
            case 3:// Delete contact
                DeleteContact(cL);
                system("pause");
                break;
            case 4:// Search contact
                SearchContact(cL);
                system("pause");
                break;
            case 5:// Modify all contacts
                ModifyContact(cL);
                system("pause");
                break;
            case 6:// Clear all contacts
                ClearContact(cL);
                system("pause");
                break;
            case 0:// Exit
                cout << "Saving contacts to file..." << endl;
                SaveContacts(cL);
                cout << "Thank you for using the contact list management system!" << endl;
                cout << "Exiting the program..." << endl;
                system("pause");
                return 0;
            default:
                cout << "Invalid choice, please try again." << endl;
        }
        system("cls"); 
        Menu(); 
    }
    
    system("pause");
    return 0;
}