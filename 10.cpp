#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <cctype>
#include <limits>
#include <map>
#include <conio.h>
#include <algorithm>
using namespace std;

class CourseGrade
{
public:
    string course;
    string grade;
    CourseGrade() : course("X"), grade("X") {}
};

class Student
{
public:
    string name;
    string id;
    vector<CourseGrade> student_courses;
};

class Lecturer
{
public:
    string name;
    string lecturer_id;
    vector<string> lecturer_courses;
};

class Parents
{
    string name;
    string parents_id;
    string parents_child;
};

//-----------------------------  SIGN IN - LOG IN - DELETE ACCOUNTS  -----------------------------
map<string, string> studentAccounts;
map<string, string> lecturerAccounts;
map<string, string> parentsAccounts;
map<string, string> departmentAccounts;
map<string, vector<string>> lecturerCourses;

const string departmentfilename = "department_accounts.txt";
const string studentfilename = "student_accounts.txt";
const string parentsfilename = "parents_accounts.txt";
const string lecturerfilename = "lecturer_accounts.txt";
const string studentlistfilename = "studentlist.txt";
const string lecturerlistfilename = "lecturerlist.txt";
const string parentslistfilename = "parentslist.txt";

void Dodge(const string &filename)
{
    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            cout << line << endl;
        }
        file.close();
    }
    else
    {
        cout << "Unable to open file: " << filename << endl;
    }
}

void clear()
{
    cout << "\033[2J\033[1;1H";
}

string getPassword()
{
    string password;
    char ch;
    ch = _getch();
    while (ch != 13)
    { // character 13 is enter
        if (ch == 8)
        { // ASCII value of BACKSPACE
            if (password.length() > 0)
            {
                cout << "\b \b";     // move cursor back, overwrite with space, move cursor back again
                password.pop_back(); // remove last character from password
            }
        }
        else
        {
            password.push_back(ch);
            cout << '*';
        }
        ch = _getch();
    }
    return password;
}

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

void registerAccount(const string &filename, const string &listfilename, map<string, string> &accounts)
{
    string username, password, user_name;
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
        cout << "Enter new user's full name: ";
        cin.ignore();
        getline(cin, user_name);

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
        ofstream personalFile(listfilename, ios::app);
        if (personalFile.is_open())
        {
            personalFile << user_name << "\n"
                         << username << "\n\n";
            personalFile.close();
        }
    }
    else
    {
        cerr << "Username already exists. Please choose a different username.\n";
    }
}

Lecturer registerAccountForLecturer(const string &filename, const string &listfilename, map<string, string> &accounts)
{
    string username, password, user_name;
    cout << "Enter new username(ID): ";
    cin >> username;
    // Kiểm tra xem tài khoản đã tồn tại chưa
    if (accounts.find(username) == accounts.end())
    {
        cout << "Enter new password: ";
        cin >> password;
        accounts[username] = password;
        cout << "Enter new user's full name: ";
        cin.ignore();
        getline(cin, user_name);

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

        // Write to the list file
        ofstream listfile(listfilename, ios::app);
        if (listfile.is_open())
        {
            listfile << user_name << "\n"
                     << username << "\n\n"; // username is used as id
            listfile.close();
        }
        else
        {
            cerr << "Error opening the file: " << listfilename << "\n";
        }

        // Return the new Lecturer
        return Lecturer{user_name, username, {}};
    }
    else
    {
        cerr << "Username already exists. Please choose a different username.\n";
        // Return a default Lecturer if registration failed
        return Lecturer{};
    }
}
bool login(const string &username, const string &password, const map<string, string> &accounts)
{
    auto it = accounts.find(username);
    if (it != accounts.end() && it->second == password)
    {
        cout << endl
             << "Login successful!\n";
        return true;
    }
    else
    {
        cout << endl
             << endl
             << "Login failed. Invalid username or password.\n";
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
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
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

void inputGrade(Student &student)
{
    cout << "Enter the course to grade: ";
    string courseToGrade;
    getline(cin, courseToGrade);
    for (CourseGrade &cg : student.student_courses)
    {
        if (cg.course == courseToGrade)
        {
            cout << "Enter grade for student " << student.name << " (ID: " << student.id << ") in course " << courseToGrade << ": ";
            inputNumber(cg.grade);
            return; // Đã tìm thấy và cập nhật điểm, có thể thoát khỏi hàm
        }
    }
    cout << "Course not found for student " << student.name << " in course " << courseToGrade << endl;
}

void inputCourse(Student &s)
{
    cout << "Enter the number of courses: ";
    int numCourses;
    inputNumber(numCourses);
    for (int i = 0; i < numCourses; ++i)
    {
        CourseGrade courseGrade;
        cout << "Enter course #" << i + 1 << ": ";
        getline(cin, courseGrade.course);
        s.student_courses.push_back(courseGrade);
    }
}

void inputCourseForLecturer(Lecturer &l)
{
    cout << "Enter the number of courses: ";
    int numCourses;
    inputNumber(numCourses);
    for (int i = 0; i < numCourses; ++i)
    {
        string course;
        cout << "Enter course #" << i + 1 << ": ";
        getline(cin, course);
        l.lecturer_courses.push_back(course);
    }
}

void showCourse(const Student &s)
{
    cout << "Registered courses and grades: ";
    for (const CourseGrade &course : s.student_courses)
    {
        cout << course.course << " (" << course.grade << "), ";
    }
    cout << endl;
}
void showCourseForLecturer(const Lecturer &l)
{
    cout << "Registered courses: ";
    for (const string &course : l.lecturer_courses)
    {
        cout << course << ", ";
    }
    cout << endl;
}

void show(const Student &s)
{
    cout << "Full name: " << s.name << endl;
    cout << "Student id: " << s.id << endl;
    cout << "Registered courses and grades: ";
    for (const CourseGrade &cg : s.student_courses)
    {
        cout << cg.course << ": " << cg.grade << " ";
    }
    cout << endl;
}

void showList(const vector<Student> &studentlist)
{
    if (studentlist.size() == 0)
    {
        cout << endl
             << "Empty list!" << endl;
        return;
    }

    for (int i = 0; i < studentlist.size(); i++)
    {
        cout << endl
             << "Number: " << i + 1 << endl;
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

    for (const Student &s : studentlist)
    {
        show(s);
    }
}

vector<Student> inputPersonalInfor(const vector<Student> &studentlist, string username)
{
    vector<Student> result;
    cout << "           ***" << endl;
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
void update(vector<Student> &studentlist, string myId)
{
    for (auto &student : studentlist)
    {
        if (student.id == myId)
        {
            cout << "Updating your name, current name is: " << student.name << endl;
            cout << "Enter new name: ";
            getline(cin, student.name);
            break;
        }
    }
}

void updateGrade(vector<Student> &studentlist)
{
    showList(studentlist);
    cout << endl
         << "Choose number: ";
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

void addCourse(vector<Student> &studentlist, string myID)
{
    for (auto &student : studentlist)
    {
        if (student.id == myID)
        {
            cout << "Adding your course, current courses are: ";
            showCourse(student);
            inputCourse(student);
            break;
        }
    }
}
void addCourseForLecturer(vector<Lecturer> &lecturerlist, string myID)
{
    for (auto &lecturer : lecturerlist)
    {
        if (lecturer.lecturer_id == myID)
        {
            cout << "Adding your course, current courses are: ";
            showCourseForLecturer(lecturer);
            inputCourseForLecturer(lecturer);
            break;
        }
    }
}

vector<Student> searchbyName(const vector<Student> &studentlist)
{
    vector<Student> result;
    string keyword;
    cout << "Searching for student by name: ";
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

void showCourseGrades(const vector<Student> &studentlist, const string &courseName)
{
    if (studentlist.size() == 0)
    {
        cout << endl
             << "Empty list!" << endl;
        return;
    }

    for (int i = 0; i < studentlist.size(); i++)
    {
        cout << endl
             << "Number: " << i + 1 << endl;
        cout << "Name: " << studentlist[i].name << endl;
        cout << "ID: " << studentlist[i].id << endl;
        for (const CourseGrade &cg : studentlist[i].student_courses)
        {
            if (cg.course == courseName)
            {
                cout << "Course: " << cg.course << ", Grade: " << cg.grade << endl;
            }
        }
    }
}

vector<Student> searchbyCourse(const vector<Student> &studentlist)
{
    vector<Student> result;
    string keyword;
    cout << "Course: ";
    getline(cin, keyword);
    clear();
    for (const Student &student : studentlist)
    {
        for (const CourseGrade &course : student.student_courses)
        {
            if (course.course == keyword)
            {
                result.push_back(student);
                break; // Break out of the inner loop once a match is found for the current student
            }
        }
    }
    if (!result.size())
    {
        cout << "Can't find any student in the course." << endl;
        return studentlist;
    }
    cout << "       ***" << endl;
    cout << "Students in class " << keyword << ": " << endl;
    showCourseGrades(result, keyword);
    return result;
}

void search(const vector<Student> &studentlist)
{
    vector<Student> result = studentlist;
    while (1)
    {
        cout << endl
             << "1. Name" << endl
             << "2. Id" << endl
             << "3. Course" << endl
             << "4. Refresh filter" << endl
             << "0. Exit" << endl;
        cout << "Search by: ";
        int choice;
        inputNumber(choice);
        clear();
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
        case 4:
            result = studentlist;
            cout << "Refresh successfully!" << endl;
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
        cout << "Enter your choice:" << endl;
        cout << "1. View with your child's name" << endl
             << "2. View with your child's id" << endl
             << "0. Exit" << endl;
        cout << "Search by: ";
        int choice;
        inputNumber(choice);
        clear();
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
            while (getline(save, line) && !line.empty())
            {
                istringstream iss(line);
                CourseGrade courseGrade;
                if (iss >> courseGrade.course >> courseGrade.grade)
                {
                    s.student_courses.push_back(courseGrade);
                }
            }
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
            for (const CourseGrade &course : s.student_courses)
            {
                output << course.course << " " << course.grade << endl;
            }
            output << endl;
        }
        cout << "Data has been saved." << endl;
    }
    else
    {
        cout << "Error opening the file" << endl;
    }
    output.close();
}

void readFileForLecturer(vector<Lecturer> &lecturerlist)
{
    ifstream save("lecturerlist.txt");
    if (save.is_open())
    {
        string line;
        while (getline(save, line))
        {
            Lecturer l;
            l.name = line;
            if (getline(save, line))
                l.lecturer_id = line;
            while (getline(save, line) && !line.empty())
            {
                l.lecturer_courses.push_back(line);
            }
            lecturerlist.push_back(l);
        }
        cout << "Lecturer list loaded!" << endl;
    }
    else
    {
        cout << "Failed to load lecturer list!" << endl;
    }
    save.close();
}

void writeToFileForLecturer(const vector<Lecturer> &lecturerlist)
{
    ofstream output("lecturerlist.txt", ios::trunc); // Open file in truncate mode
    if (output.is_open())
    {
        for (const Lecturer &l : lecturerlist)
        {
            output << l.name << endl;
            output << l.lecturer_id << endl;
            for (const string &course : l.lecturer_courses)
            {
                output << course << endl;
            }
            output << endl;
        }
        cout << "Data has been saved." << endl;
    }
    else
    {
        cout << "Failed to save data!" << endl;
    }
    output.close();
}
//-----------------------------  MENU  -----------------------------
void displayStudentMenu()
{
    cout << endl;
    cout << "Select an option:" << endl;
    cout << "1. Display students list" << endl;
    cout << "2. Search in students list" << endl;
    cout << "3. Register a new course for this semester" << endl;
    cout << "4. Show your personal information" << endl;
    cout << "5. Update your personal information" << endl;
    cout << "0. Sign out" << endl;
}
void displayLecturerMenu()
{
    cout << endl;
    cout << "Select an option:" << endl;
    cout << "1. Display student list in your course: " << endl;
    cout << "2. Search student in your course" << endl;
    cout << "3. Update score for your students" << endl;
    cout << "0. Sign out" << endl;
}
void displayDepartmentMenu()
{
    cout << endl;
    cout << "Select an option:" << endl;
    cout << "1. Create new account: " << endl;
    cout << "2. Delete account: " << endl;
    cout << "3. Add course for lecturer: " << endl;
    cout << "0. Sign out" << endl;
}
void displayParentsMenu()
{
    cout << endl;
    cout << "Select an option:" << endl;
    cout << "1. View your child's information : " << endl;
    cout << "0. Sign out" << endl;
}

//-----------------------------  HÀM MAIN  -----------------------------
int main()
{
    vector<Student> studentlist;
    vector<Lecturer> lecturerlist;
    string username, password;
    const int maxAttemps = 3;
    int attemps = 0;
    string usernameToDelete, passwordToDelete;
    char userTypeToDelete, userTypeToRegister;

    readfile(studentlist);
    readFileForLecturer(lecturerlist);
    while (true)
    {
        char userType;
        Dodge("Dodge.txt");
        cout << endl
             << endl
             << "Enter the type of user (S for Student, L for Lecturer, P for Parents, D for Department): ";
        cin >> userType;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        clear();
        bool loginSuccess = false;
        if (tolower(userType) == 's') //----------------STUDENTS----------------
        {
            while (attemps < maxAttemps)
            {
                cout << endl
                     << "Log in: " << endl;
                loadAccountsFromFile(studentfilename, studentAccounts);
                cout << "Enter your username(ID): ";
                getline(cin, username);
                cout << "Enter your password: ";
                password = getPassword();
                loginSuccess = login(username, password, studentAccounts);
                attemps++;
                if (loginSuccess)
                {
                    attemps = 0;
                    clear();
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
                    clear();
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
                        addCourse(studentlist, username);
                        writeToFile(studentlist);
                        break;
                    case 4:
                        inputPersonalInfor(studentlist, username);
                        break;
                    case 5:
                        update(studentlist, username);
                        writeToFile(studentlist);
                        break;
                    case 0: // Exit the program
                        cout << "Sign out successfully!" << endl;
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
                cout << endl
                     << "Log in: " << endl;
                loadAccountsFromFile(lecturerfilename, lecturerAccounts);
                cout << "Enter your username(ID): ";
                getline(cin, username);
                cout << "Enter your password: ";
                password = getPassword();
                loginSuccess = login(username, password, lecturerAccounts);
                attemps++;
                if (loginSuccess)
                {
                    attemps = 0;
                    clear();
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
                    clear();
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
                cout << endl
                     << "Log in: " << endl;
                loadAccountsFromFile(parentsfilename, parentsAccounts);
                cout << "Enter your username(ID): ";
                getline(cin, username);
                cout << "Enter your password: ";
                password = getPassword();
                loginSuccess = login(username, password, parentsAccounts);
                attemps++;
                if (loginSuccess)
                {
                    attemps = 0;
                    clear();
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
                    clear();
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
                cout << endl
                     << "Log in: " << endl;
                loadAccountsFromFile(departmentfilename, departmentAccounts);
                cout << "Enter your username(ID): ";
                getline(cin, username);
                cout << "Enter your password: ";
                password = getPassword();
                loginSuccess = login(username, password, departmentAccounts);
                attemps++;
                if (loginSuccess)
                {
                    attemps = 0;
                    clear();
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
                    clear();
                    switch (choice)
                    {
                    case 1: // Create new account
                        cout << "Enter the type of user to register(S for Student, L for Lecturer, P for Parents): " << endl;
                        cin >> userTypeToRegister;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        clear();
                        if (tolower(userTypeToRegister) == 's')
                        {
                            registerAccount(studentfilename, studentlistfilename, studentAccounts);
                        }
                        else if (tolower(userTypeToRegister) == 'l')
                        {
                            Lecturer newLecturer = registerAccountForLecturer(lecturerfilename, lecturerlistfilename, lecturerAccounts);
                            lecturerlist.push_back(newLecturer);
                        }
                        else if (tolower(userTypeToRegister) == 'p')
                        {
                            registerAccount(parentsfilename, parentslistfilename, parentsAccounts);
                        }
                        break;
                    case 2: // Delete account
                        cout << "Enter the type of user to delete(S for Student, L for Lecturer, P for Parents): " << endl;
                        cin >> userTypeToDelete;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        clear();
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
                    case 3:
                    {
                        for (const Lecturer &lecturer : lecturerlist)
                        {
                            cout << "Lecturer Name: " << lecturer.name << ", ID: " << lecturer.lecturer_id << endl;
                            cout << "Courses: ";
                            for (const string &course : lecturer.lecturer_courses)
                            {
                                cout << course << ", ";
                            }
                            cout << endl;
                        }
                        string lecturer_id;
                        cout << "Enter the lecturer's username(ID): ";
                        getline(cin, lecturer_id);
                        addCourseForLecturer(lecturerlist, lecturer_id);
                        writeToFileForLecturer(lecturerlist);
                        break;
                    }
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
