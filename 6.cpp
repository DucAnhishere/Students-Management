#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <cctype>
#include <limits>
#include <map>
using namespace std;

class Student
{
public:
    string name;
    string id;
    string course;
    double grade;
};

class Lecturer
{
private:
    string full_name;
    string lecturer_id;
    string course;
};

//-----------------------------  SIGN IN - LOG IN - DELETE ACCOUNTS  -----------------------------
map<string, string> studentAccounts;
map<string, string> lecturerAccounts;
map<string, string> parentsAccounts;
map<string, string> departmentAccounts;

const string departmentfilename = "department_accounts.txt";
const string studentfilename = "student_accounts.txt";
const string parentsfilename = "parents_accounts.txt";
const string lecturerfilename = "lecturer_accounts.txt";

void loadAccountsFromFile(const string &filename, map<string, string> &accounts)
{
    ifstream file(filename);

    // Kiểm tra xem tệp có tồn tại không, nếu không thì tạo mới
    if (!file.is_open())
    {
        ofstream createFile(filename);
        createFile.close();
        file.open(filename);
    }

    if (file.is_open())
    {
        string username, password;
        while (file >> username >> password)
        {
            accounts[username] = password;
        }
        file.close();
    }
    else
    {
        cerr << "Error opening the file: " << filename << "\n";
    }
}

void registerAccount(const string &filename, map<string, string> &accounts)
{
    string username, password;

    cout << "Enter new username(ID): ";
    cin >> username;
    // Kiểm tra xem tệp có tồn tại không, nếu không thì tạo mới
    ifstream file(filename);
    if (!file.is_open())
    {
        ofstream createFile(filename);
        createFile.close();
        file.open(filename);
    }

    // Kiểm tra xem tài khoản đã tồn tại chưa
    if (accounts.find(username) == accounts.end())
    {
        cout << "Enter new password: ";
        cin >> password;

        accounts[username] = password;

        ofstream file(filename, ios::app);
        if (file.is_open())
        {
            file << username << " " << password << "\n";
            file.close();
            cout << "Account registered successfully.\n";
        }
        else
        {
            cerr << "Error opening the file: " << filename << "\n";
        }
    }
    else
    {
        cerr << "Username already exists. Please choose a different username.\n";
    }
}

bool login(const string &username, const string &password, const map<string, string> &accounts)
{
    auto it = accounts.find(username);
    if (it != accounts.end() && it->second == password)
    {
        cout << "-----------------------------" << endl;
        cout << "Login successful!\n";
        return true;
    }
    else
    {
        cout << "-----------------------------" << endl;
        cerr << "Login failed. Invalid username or password.\n";
        return false;
    }
}

void deleteAccounts(const string &filename, const string &usernameToDelete, const string &passwordToDelete, map<string, string> &accounts)
{
    ifstream inFile(filename);
    if (!inFile.is_open())
    {
        cerr << "Error opening the file: " << filename << endl;
        return;
    }
    ofstream tempFile("temp.txt"); // Mở file tạm để ghi
    if (!tempFile.is_open())
    {
        cerr << "Error opening the temporary file." << endl;
        inFile.close();
        return;
    }
    string line; // Duyệt qua từng dòng trong file gốc
    bool usernameFound = false;
    while (getline(inFile, line))
    {
        string username, password;
        istringstream iss(line);

        if (iss >> username >> password)
        {
            if (username != usernameToDelete || password != passwordToDelete)
            {
                tempFile << line << endl;
            }
            else
            {
                accounts.erase(username);
                usernameFound = true;
                cout << "-----------------------------" << endl;
                cout << "Delete successfully!" << endl;
            }
        }
    }
    inFile.close();
    tempFile.close();
    if (usernameFound)
    {
        remove(filename.c_str());
        rename("temp.txt", filename.c_str());
    }
    else
    {
        cerr << "Invalid username or password!" << endl;
        remove("temp.txt");
    }
}

template <typename T>
void inputNumber(T &i)
{
    while (1)
    {
        if (cin >> i)
        {
            cin.ignore();
            break;
        }
        else
        {
            cout << "Invalid input. Please enter an integer: ";
            cin.clear();                                         // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cin.ignore();
        }
    }
}

void input(Student &s)
{
    cout << "Name: ";
    getline(cin, s.name);
    cout << "Id: ";
    getline(cin, s.id);
    cout << "Course: ";
    getline(cin, s.course);
    cout << "Grading: ";
    inputNumber(s.grade);
}

void inputGrade(Student &s)
{
    cout << "Grading: ";
    inputNumber(s.grade);
}

void inputCourse(Student &s)
{
    cout << "Course: ";
    getline(cin, s.course);
}

void show(const Student &s)
{
    cout << "Full name: " << s.name << endl;
    cout << "Student id: " << s.id << endl;
    cout << "Registered course: " << s.course << endl;
    cout << "Final grade: " << s.grade << endl;
}
void showList(const vector<Student> &studentlist)
{
    if (studentlist.size() == 0)
    {
        cout << "-----------------------------" << endl;
        cout << "Empty list!" << endl;
        return;
    }

    for (int i = 0; i < studentlist.size(); i++)
    {
        cout << "-----------------------------" << endl;
        cout << "Number: " << i + 1 << endl;
        show(studentlist[i]);
    }
}

void showPersonal(const vector<Student> &studentlist)
{
    if (studentlist.size() == 0)
    {
        cout << "Empty list!" << endl;
        return;
    }

    for (int i = 0; i < studentlist.size(); i++)
    {
        show(studentlist[i]);
    }
}

vector<Student> inputPersonalInfor(const vector<Student> &studentlist, string username)
{
    vector<Student> result;
    cout << "Your personal information" << endl;
    for (int i = 0; i < studentlist.size(); i++)
    {
        if ((studentlist[i].id).find(username) != string::npos)
        {
            result.push_back(studentlist[i]);
        }
    }
    if (!result.size())
    {
        cout << "Invalid ID." << endl;
        return studentlist;
    }
    showPersonal(result);
    return result;
}

void remove(vector<Student> &studentlist)
{
    cout << "Index of student you want to delete: ";
    int n;
    inputNumber(n);
    if (n > 0 && n < studentlist.size() + 1)
    {
        studentlist.erase(studentlist.begin() + n - 1);
        cout << "Student index = " << n << " has been removed." << endl;
    }
    else
        cout << "Invalid index." << endl;
}
void update(vector<Student> &studentlist)
{
    cout << "Choose index: ";
    int choice;
    inputNumber(choice);
    if (choice < 1 || choice > studentlist.size())
    {
        cout << "Invalid index." << endl;
        return;
    }
    else
    {
        cout << "Updating student [" << choice << "] " << studentlist[choice - 1].name << endl;
        input(studentlist[choice - 1]);
    }
}

void updateGrade(vector<Student> &studentlist)
{
    showList(studentlist);
    cout << "Choose index: ";
    int choice;
    inputNumber(choice);
    if (choice < 1 || choice > studentlist.size())
    {
        cout << "Invalid index." << endl;
        return;
    }
    else
    {
        cout << "Updating student [" << choice << "] " << studentlist[choice - 1].name << endl;
        inputGrade(studentlist[choice - 1]);
    }
}

void addCourse(vector<Student> &studentlist)
{
    cout << "Input ordinal number: ";
    int choice;
    inputNumber(choice);
    if (choice < 1 || choice > studentlist.size())
    {
        cout << "Invalid index." << endl;
        return;
    }
    else
    {
        cout << "Adding your course [" << choice << "] " << studentlist[choice - 1].name << endl;
        inputCourse(studentlist[choice - 1]);
    }
}

string lowercase(const string &str)
{
    string result;
    for (char ch : str)
    {
        result += tolower(ch);
    }
    return result;
}

vector<Student> searchbyName(const vector<Student> &studentlist)
{
    vector<Student> result;
    string keyword;
    cout << "Name: ";
    getline(cin, keyword);
    keyword = lowercase(keyword);
    for (int i = 0; i < studentlist.size(); i++)
    {
        if (lowercase(studentlist[i].name).find(keyword) != string::npos)
        {
            result.push_back(studentlist[i]);
        }
    }
    if (!result.size())
    {
        cout << "Invalid student's name." << endl;
        return studentlist;
    }
    showList(result);
    return result;
}

vector<Student> searchbyId(const vector<Student> &studentlist)
{
    vector<Student> result;
    string keyword;
    cout << "Searching for student by Id: ";
    getline(cin, keyword);
    keyword = lowercase(keyword);
    for (int i = 0; i < studentlist.size(); i++)
    {
        if (lowercase(studentlist[i].id).find(keyword) != string::npos)
        {
            result.push_back(studentlist[i]);
        }
    }
    if (!result.size())
    {
        cout << "Invalid ID." << endl;
        return studentlist;
    }
    showList(result);
    return result;
}
vector<Student> searchbyCourse(const vector<Student> &studentlist)
{
    vector<Student> result;
    string keyword;
    cout << "Course: ";
    getline(cin, keyword);
    for (int i = 0; i < studentlist.size(); i++)
    {
        if (studentlist[i].course.find(keyword) != string::npos)
        {
            result.push_back(studentlist[i]);
        }
    }
    if (!result.size())
    {
        cout << "Can't find any student in the course." << endl;
        return studentlist;
    }
    cout << "       ***";
    cout << "Students in class " << keyword << ": " << endl;
    showList(result);
    return result;
}

void search(const vector<Student> &studentlist)
{
    vector<Student> result = studentlist;
    while (1)
    {
        cout << "1. Name" << endl
             << "2. Id" << endl
             << "3. Course" << endl
             << "0. Exit" << endl;
        cout << "Search by: ";
        int choice;
        inputNumber(choice);
        switch (choice)
        {
        case 1:
            result = searchbyName(result);
            break;
        case 2:
            result = searchbyId(result);
            break;
        case 3:
            result = searchbyCourse(result);
            break;
        case 0:
            return;
            break;
        default:
            cout << "Invalid choice." << endl;
        }
    }
}

void searchChild(const vector<Student> &studentlist)
{
    vector<Student> result = studentlist;
    while (1)
    {
        cout << "-----------------------------" << endl;
        cout << "Enter your choice:" << endl;
        cout << "1. View with your child's name" << endl
             << "2. View with your child's id" << endl
             << "0. Exit" << endl;
        cout << "Search by: ";
        int choice;
        inputNumber(choice);
        switch (choice)
        {
        case 1:
            result = searchbyName(result);
            break;
        case 2:
            result = searchbyId(result);
            break;
        case 0:
            return;
            break;
        default:
            cout << "Invalid choice." << endl;
        }
    }
}

void readfile(vector<Student> &studentlist)
{
    ifstream save("studentlist.txt");
    if (save.is_open())
    {
        string line;
        while (getline(save, line))
        {
            Student s;
            s.name = line;
            if (getline(save, line))
                s.id = line;
            if (getline(save, line))
                s.course = line;
            if (getline(save, line))
                s.grade = stod(line);
            studentlist.push_back(s);
        }
        cout << "Student list loaded!" << endl;
    }
    else
        cout << "Failed to load student list!" << endl;
    save.close();
}

void writeToFile(const vector<Student> &studentlist)
{
    ofstream output("studentlist.txt");
    if (output.is_open())
    {
        for (const Student &s : studentlist)
        {
            output << s.name << endl;
            output << s.id << endl;
            output << s.course << endl;
            output << s.grade << endl;
        }
        cout << "Data has been saved." << endl;
    }
    else
    {
        cout << "Error opening the file" << endl;
    }
    output.close();
}
//-----------------------------  MENU  -----------------------------
void displayStudentMenu()
{
    cout << "-----------------------------" << endl;
    cout << "Select an option:" << endl;
    cout << "1. Display students list" << endl;
    cout << "2. Search in students list" << endl;
    cout << "3. Register a new course for this semester" << endl;
    cout << "4. Show your personal information" << endl;
    cout << "0. Sign out" << endl;
}
void displayLecturerMenu()
{
    cout << "-----------------------------" << endl;
    cout << "Select an option:" << endl;
    cout << "1. Display student list: " << endl;
    cout << "2. Search in students list" << endl;
    cout << "3. Update score for your students" << endl;
    cout << "0. Sign out" << endl;
}
void displayDepartmentMenu()
{
    cout << "-----------------------------" << endl;
    cout << "Select an option:" << endl;
    cout << "1. Create new account: " << endl;
    cout << "2. Delete account: " << endl;
    cout << "0. Sign out" << endl;
}
void displayParentsMenu()
{
    cout << "-----------------------------" << endl;
    cout << "Select an option:" << endl;
    cout << "1. View your child's information : " << endl;
    cout << "0. Sign out" << endl;
}

//-----------------------------  HÀM MAIN  -----------------------------
int main()
{
    vector<Student> studentlist;
    string username, password;
    int maxAttemps = 3;
    int attemps = 0;
    string usernameToDelete, passwordToDelete;
    char userTypeToDelete, userTypeToRegister;

    readfile(studentlist);
    while (true)
    {
        char userType;
        cout << "Enter the type of user (S for Student, L for Lecturer, P for Parents, D for Department): ";
        cin >> userType;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        bool loginSuccess = false;
        if (tolower(userType) == 's') //----------------STUDENTS----------------
        {
            while (attemps < maxAttemps)
            {
                cout << "-----------------------------" << endl;
                cout << "Log in: " << endl;
                loadAccountsFromFile(studentfilename, studentAccounts);
                cout << "Enter your username(ID): ";
                cin >> username;
                cout << "Enter your password: ";
                cin >> password;
                loginSuccess = login(username, password, studentAccounts);
                attemps++;
                if (loginSuccess)
                {
                    break;
                }
            }
            while (true)
            {
                if (loginSuccess)
                {
                    displayStudentMenu();
                    int choice;
                    cout << "Enter your choice: ";
                    cin >> choice;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    switch (choice)
                    {
                    case 1:
                        cout << "    ***" << endl;
                        cout << "Student List:" << endl;
                        showList(studentlist);
                        break;
                    case 2:
                        search(studentlist);
                        break;
                    case 3:
                        addCourse(studentlist);
                        writeToFile(studentlist);
                        break;
                    case 4:
                        inputPersonalInfor(studentlist, username);
                        break;
                    case 0: // Exit the program
                        return 0;
                    default:
                        cout << "Invalid choice. Please enter a valid option." << endl;
                    }
                }
            }
        }
        else if (userType == 'L' || userType == 'l') //----------------LECTURER----------------
        {
            while (attemps < maxAttemps)
            {
                cout << "-----------------------------" << endl;
                cout << "Log in: " << endl;
                loadAccountsFromFile(lecturerfilename, lecturerAccounts);
                cout << "Enter your username(ID): ";
                cin >> username;
                cout << "Enter your password: ";
                cin >> password;
                loginSuccess = login(username, password, lecturerAccounts);
                attemps++;
                if (loginSuccess)
                {
                    break;
                }
            }
            while (true)
            {
                if (loginSuccess)
                {
                    displayLecturerMenu();
                    int choice;
                    cout << "Enter your choice: ";
                    cin >> choice;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    switch (choice)
                    {
                    case 1:
                        searchbyCourse(studentlist);
                        break;
                    case 2:
                        search(studentlist);
                        break;
                    case 3:
                        updateGrade(studentlist);
                        writeToFile(studentlist);
                        break;
                    case 0: // Exit the program
                        return 0;
                    default:
                        cout << "Invalid choice. Please enter a valid option." << endl;
                    }
                }
            }
        }

        else if (userType == 'P' || userType == 'p') //----------------Parents----------------
        {
            while (attemps < maxAttemps)
            {
                cout << "-----------------------------" << endl;
                cout << "Log in: " << endl;
                loadAccountsFromFile(parentsfilename, parentsAccounts);
                cout << "Enter your username(ID): ";
                cin >> username;
                cout << "Enter your password: ";
                cin >> password;
                loginSuccess = login(username, password, parentsAccounts);
                attemps++;
                if (loginSuccess)
                {
                    break;
                }
            }
            while (true)
            {
                if (loginSuccess)
                {
                    displayParentsMenu();
                    int choice;
                    cout << "Enter your choice: ";
                    cin >> choice;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    switch (choice)
                    {
                    case 1:
                        searchChild(studentlist);
                        break;
                    case 0: // Exit the program
                        return 0;
                    default:
                        cout << "Invalid choice. Please enter a valid option." << endl;
                    }
                }
            }
        }

        else if (userType == 'D' || userType == 'd') //----------------DEPARTMENT----------------
        {
            while (attemps < maxAttemps)
            {
                cout << "-----------------------------" << endl;
                cout << "Log in: " << endl;
                loadAccountsFromFile(departmentfilename, departmentAccounts);
                cout << "Enter your username(ID): ";
                cin >> username;
                cout << "Enter your password: ";
                cin >> password;
                loginSuccess = login(username, password, departmentAccounts);
                attemps++;
                if (loginSuccess)
                {
                    break;
                }
            }
            while (true)
            {
                if (loginSuccess)
                {

                    displayDepartmentMenu();
                    int choice;
                    cout << "Enter your choice: ";
                    cin >> choice;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    switch (choice)
                    {
                    case 1: // Create new account
                        cout << "Enter the type of user to register(S for Student, L for Lecturer, P for Parents): " << endl;
                        cin >> userTypeToRegister;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        if (tolower(userTypeToRegister) == 's')
                        {
                            registerAccount(studentfilename, studentAccounts);
                        }
                        else if (tolower(userTypeToRegister) == 'l')
                        {
                            registerAccount(lecturerfilename, lecturerAccounts);
                        }
                        else if (tolower(userTypeToRegister) == 'p')
                        {
                            registerAccount(parentsfilename, parentsAccounts);
                        }
                        break;
                    case 2: // Delete account
                        cout << "Enter the type of user to delete(S for Student, L for Lecturer, P for Parents): " << endl;
                        cin >> userTypeToDelete;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Enter username to delete: ";
                        cin >> usernameToDelete;
                        cout << "Enter password: ";
                        cin >> passwordToDelete;
                        if (tolower(userTypeToDelete) == 's')
                        {
                            deleteAccounts(studentfilename, usernameToDelete, passwordToDelete, studentAccounts);
                        }
                        else if (tolower(userTypeToDelete) == 'l')
                        {
                            deleteAccounts(lecturerfilename, usernameToDelete, passwordToDelete, lecturerAccounts);
                        }
                        else if (tolower(userTypeToDelete) == 'p')
                        {
                            deleteAccounts(parentsfilename, usernameToDelete, passwordToDelete, parentsAccounts);
                        }
                        break;
                    case 0: // Exit the program
                        return 0;
                    default:
                        cout << "Invalid choice. Please enter a valid option." << endl;
                    }
                }
            }
        }
        else
        {
            cout << "Invalid user type, please enter again!" << endl;
            continue;
        }
        break;
    }
    return 0;
}
