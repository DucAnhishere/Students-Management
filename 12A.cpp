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

class Person
{
private:
    string name;
    string id;
    string dob; // dd/mm/yyyy
    string sdt;

public:
    // Constructor
    Person(const string &name, const string &id, const string &sdt, const string &dob) : name(name), id(id), sdt(sdt), dob(dob) {}

    // Getters
    string getName() const { return name; }
    string getId() const { return id; }
    string getSdt() const { return sdt; }
    string getDob() const { return dob; }

    // Setters
    void setName(const string &newName) { name = newName; }
    void setId(const string &newId) { id = newId; }
    void setSdt(const string &newSdt) { sdt = newSdt; }
    void setDob(const string &newDob) { dob = newDob; }
};

class CourseGrade
{
public:
    string course;
    double grade;
    CourseGrade() : course("X"), grade(0.0) {}
};

class Student : public Person
{
private:
    vector<CourseGrade> student_courses;

public:
    // Constructor
    Student() : Person("", "", "", "") {}

    // Getters
    vector<CourseGrade> getCourses() const { return student_courses; }

    // Setters
    void setCourses(const vector<CourseGrade> &newStudentCourses) { student_courses = newStudentCourses; }

    // Add a CourseGrade to student_courses
    void addCourse(const CourseGrade &courseGrade) { student_courses.push_back(courseGrade); }

    void updateGrade(const string &course, double newGrade)
    {
        for (CourseGrade &courseGrade : student_courses)
        {
            if (courseGrade.course == course)
            {
                courseGrade.grade = newGrade;
                break;
            }
        }
    }
};

class Lecturer : public Person
{
private:
    vector<string> lecturer_courses;

public:
    // Constructor
    Lecturer() : Person("", "", "", "") {}

    // Getters
    const vector<string> &getCourses() const { return lecturer_courses; }

    // Setters
    void setCourses(const vector<string> &newCourses) { lecturer_courses = newCourses; }

    // Add a course to courses
    void addCourse(const string &course) { lecturer_courses.push_back(course); }
};

class Parents
{
    string name;
    string parents_id;
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
        return Lecturer{};
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

void inputCourse(Student &s)
{
    cout << "Enter the number of courses: ";
    int numCourses;
    inputNumber(numCourses);
    vector<CourseGrade> courses = s.getCourses();
    for (int i = 0; i < numCourses; ++i)
    {
        CourseGrade courseGrade;
        cout << "Enter course #" << i + 1 << ": ";
        getline(cin, courseGrade.course);
        courses.push_back(courseGrade);
    }
    s.setCourses(courses);
}

void inputCourseForLecturer(Lecturer &l)
{
    cout << "Enter the number of courses: ";
    int numCourses;
    inputNumber(numCourses);
    vector<string> courses = l.getCourses();
    for (int i = 0; i < numCourses; ++i)
    {
        string course;
        cout << "Enter course #" << i + 1 << ": ";
        getline(cin, course);
        courses.push_back(course);
    }
    l.setCourses(courses);
}

void showCourse(const Student &s)
{
    cout << "Registered courses and grades: ";
    for (const CourseGrade &course : s.getCourses())
    {
        cout << course.course << " (" << course.grade << "), ";
    }
    cout << endl;
}

void showCourseForLecturer(const Lecturer &l)
{
    cout << "Registered courses: ";
    for (const string &course : l.getCourses())
    {
        cout << course << ", ";
    }
    cout << endl;
}

void show(const Student &s)
{
    cout << "Full name: " << s.getName() << endl;
    cout << "Student id: " << s.getId() << endl;
    cout << "Date of birth: " << s.getDob() << endl;
    cout << "Phone number: " << s.getSdt() << endl;
    cout << "Registered courses and grades: " << endl;
    int i = 1;
    for (const CourseGrade &cg : s.getCourses())
    {
        cout << "- Course " << i << ": ";
        cout << cg.course << " " << endl;
        i++;
    }
    cout << endl;
}

void showList(const vector<Student> &studentlist)
{
    if (studentlist.empty())
    {
        cout << endl
             << "Empty list!" << endl;
        return;
    }

    for (size_t i = 0; i < studentlist.size(); i++)
    {
        cout << endl
             << "Number: " << i + 1 << endl;
        show(studentlist[i]);
    }
}

void showListByCourse(const vector<Student> &studentlist, const Lecturer &lecturer, const string &course)
{
    // Check if the lecturer is teaching the course
    if (find(lecturer.getCourses().begin(), lecturer.getCourses().end(), course) == lecturer.getCourses().end())
    {
        cout << "You are not teaching this course!" << endl;
        return;
    }

    bool found = false;
    // Loop through the student list
    for (const Student &student : studentlist)
    {
        // Loop through the student's courses
        for (const CourseGrade &courseGrade : student.getCourses())
        {
            // If the student is enrolled in the course, print their details
            if (courseGrade.course == course)
            {
                found = true;
                cout << "Student Name: " << student.getName() << ", ID: " << student.getId() << ", Grade in " << course << ": " << courseGrade.grade << endl;
                break;
            }
        }
    }

    if (!found)
    {
        cout << "No students are taking this course." << endl;
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

void showPersonalForLecturer(const vector<Lecturer> &lecturerlist)
{
    if (lecturerlist.size() == 0)
    {
        cout << "Empty list!" << endl;
        return;
    }

    for (const Lecturer &l : lecturerlist)
    {
        cout << "Full name: " << l.getName() << endl;
        cout << "Lecturer id: " << l.getId() << endl;
        cout << "Date of birth: " << l.getDob() << endl;
        cout << "Phone number: " << l.getSdt() << endl;
        cout << "Your teaching courses: ";
        for (const string &course : l.getCourses())
        {
            cout << course << ", ";
        }
        cout << endl;
    }
}

vector<Student> inputPersonalInfor(const vector<Student> &studentlist, string username)
{
    vector<Student> result;
    cout << "           ***" << endl;
    cout << "Your personal information" << endl;
    for (int i = 0; i < studentlist.size(); i++)
    {
        if ((studentlist[i].getId()).find(username) != string::npos)
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

vector<Lecturer> inputPersonalInforForLecturer(const vector<Lecturer> &lecturerlist, string username)
{
    vector<Lecturer> result;
    cout << "___________***___________" << endl;
    cout << "Your personal information" << endl;
    for (int i = 0; i < lecturerlist.size(); i++)
    {
        if ((lecturerlist[i].getId()).find(username) != string::npos)
        {
            result.push_back(lecturerlist[i]);
        }
    }
    if (!result.size())
    {
        cout << "Invalid ID." << endl;
        return lecturerlist;
    }
    showPersonalForLecturer(result);
    return result;
}

void update(vector<Student> &studentlist, string myId)
{
    for (auto &student : studentlist)
    {
        if (student.getId() == myId)
        {
            cout << "Updating your name, current name is: " << student.getName() << endl;
            cout << "Enter new name: ";
            string newName;
            getline(cin, newName);
            student.setName(newName);
            cout << "Updating your date of birth, current date of birth is: " << student.getDob() << endl;
            cout << "Enter new date of birth: ";
            string newDob;
            getline(cin, newDob);
            student.setDob(newDob);
            cout << "Updating your phone number, current phone number is: " << student.getSdt() << endl;
            cout << "Enter new phone number: ";
            string newSdt;
            getline(cin, newSdt);
            student.setSdt(newSdt);
            break;
        }
    }
}

void updateGrade(vector<Student> &studentlist, Lecturer &lecturer)
{
    string course;
    bool validCourse = false;

    do
    {
        cout << "Enter the course to view and update grades: ";
        cin >> course;
        if (find(lecturer.getCourses().begin(), lecturer.getCourses().end(), course) != lecturer.getCourses().end())
        {
            validCourse = true;
        }
        else
        {
            cout << "You are not teaching this course! Please enter a valid course." << endl;
        }
    } while (!validCourse);

    showListByCourse(studentlist, lecturer, course);

    cout << endl
         << "Choose student by ID: ";
    string id;
    cin >> id;

    // Find the student with the given ID
    for (Student &student : studentlist)
    {
        if (student.getId() == id)
        {
            double newGrade;
            cout << "Enter new grade for student " << student.getName() << " (ID: " << student.getId() << ") in course " << course << ": ";
            cin >> newGrade;
            student.updateGrade(course, newGrade);
            return;
        }
    }
    cout << "Student not found!" << endl;
}

void addCourse(vector<Student> &studentlist, string myID)
{
    for (auto &student : studentlist)
    {
        if (student.getId() == myID)
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
        if (lecturer.getId() == myID)
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
        if (lowercase(studentlist[i].getName()).find(keyword) != string::npos)
        {
            result.push_back(studentlist[i]);
        }
    }
    if (!result.size())
    {
        cout << "Can not find any student with this name name." << endl;
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
        if (lowercase(studentlist[i].getId()).find(keyword) != string::npos)
        {
            result.push_back(studentlist[i]);
        }
    }
    if (!result.size())
    {
        cout << "Can not find any student with this ID." << endl;
        return studentlist;
    }
    showList(result);
    return result;
}

void showStudentInCourse(const vector<Student> &studentlist, const string &courseName)
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
        cout << "Name: " << studentlist[i].getName() << endl;
        cout << "ID: " << studentlist[i].getId() << endl;
        cout << "Date of birth: " << studentlist[i].getDob() << endl;
        cout << "Phone number: " << studentlist[i].getSdt() << endl;
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
        for (const CourseGrade &course : student.getCourses())
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
    showStudentInCourse(result, keyword);
    return result;
}

void searchbySdt(const vector<Student> &studentlist)
{
    string keyword;
    cout << "Searching for student by phone number: ";
    getline(cin, keyword);
    keyword = lowercase(keyword);
    for (int i = 0; i < studentlist.size(); i++)
    {
        if (lowercase(studentlist[i].getSdt()).find(keyword) != string::npos)
        {
            cout << "       ***" << endl;
            cout << "Student's information: " << endl;
            show(studentlist[i]);
            return;
        }
    }
    cout << "Can not find any student with this phone number." << endl;
}

void search(const vector<Student> &studentlist)
{
    vector<Student> result = studentlist;
    while (1)
    {
        cout << endl
             << "_________________________" << endl
             << "|                       |" << endl
             << "|   1. Name             |" << endl
             << "|   2. Id               |" << endl
             << "|   3. Course           |" << endl
             << "|   4. Phone number     |" << endl
             << "|   5. Refresh filter   |" << endl
             << "|   0. Exit             |" << endl
             << "|_______________________|" << endl;
        cout << endl
             << "      Search by: ";
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
            searchbySdt(result);
            break;
        case 5:
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
        cout << "______________________________________" << endl;
        cout << "|                                    |" << endl;
        cout << "|   Enter your choice:               |" << endl;
        cout << "|   1. Enter your child's name       |" << endl
             << "|   2. Enter your child's id         |" << endl
             << "|   0. Exit                          |" << endl
             << "|____________________________________|" << endl
             << endl;
        cout << "    Search by: ";
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

void sortAndPrintStudentsInCourse(vector<Student> &studentList, const string &courseName)
{
    // Sort the student list by grade in descending order
    sort(studentList.begin(), studentList.end(), [courseName](const Student &s1, const Student &s2)
         {
        double grade1 = 0.0, grade2 = 0.0;
        for (const CourseGrade &course : s1.getCourses())
        {
            if (course.course == courseName)
            {
                grade1 = course.grade;
                break;
            }
        }
        for (const CourseGrade &course : s2.getCourses())
        {
            if (course.course == courseName)
            {
                grade2 = course.grade;
                break;
            }
        }
        return grade1 > grade2; });

    // Print the sorted list
    for (const Student &student : studentList)
    {
        for (const CourseGrade &course : student.getCourses())
        {
            if (course.course == courseName)
            {
                cout << "Student Name: " << student.getName() << ", ID: " << student.getId() << ", Grade in " << courseName << ": " << course.grade << endl;
                break;
            }
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
            s.setName(line);
            if (getline(save, line))
                s.setId(line);
            if (getline(save, line))
                s.setDob(line);
            if (getline(save, line))
                s.setSdt(line);
            while (getline(save, line) && !line.empty())
            {
                CourseGrade courseGrade;
                istringstream iss(line);
                if (iss >> courseGrade.course >> courseGrade.grade)
                {
                    s.addCourse(courseGrade);
                }
            }
            // Check if the student has at least one information
            if (!s.getName().empty() || !s.getId().empty() || !s.getSdt().empty() || !s.getDob().empty() || !s.getCourses().empty())
            {
                studentlist.push_back(s);
            }
        }
    }
    else
    {
        cout << "Failed to load student list!" << endl;
    }
    save.close();
}

void writeToFile(const vector<Student> &studentlist)
{
    ofstream output("studentlist.txt", ios::trunc); // Open file in truncate mode
    if (output.is_open())
    {
        for (const Student &s : studentlist)
        {
            output << s.getName() << endl;
            output << s.getId() << endl; // Write the student's ID to the file
            output << s.getDob() << endl;
            output << s.getSdt() << endl;
            if (!s.getCourses().empty()) // Check if the student's course list is not empty
            {
                for (const CourseGrade &courseGrade : s.getCourses())
                {
                    output << courseGrade.course << " " << courseGrade.grade << endl;
                }
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

void readFileForLecturer(vector<Lecturer> &lecturerlist)
{
    ifstream save("lecturerlist.txt");
    if (save.is_open())
    {
        string line;
        while (getline(save, line))
        {
            Lecturer l;
            l.setName(line);
            if (getline(save, line))
                l.setId(line);
            if (getline(save, line))
                l.setDob(line);
            if (getline(save, line))
                l.setSdt(line);
            while (getline(save, line) && !line.empty())
            {
                l.addCourse(line);
            }
            // Check if the lecturer has at least one information
            if (!l.getName().empty() || !l.getId().empty() || !l.getSdt().empty() || !l.getDob().empty() || !l.getCourses().empty())
            {
                lecturerlist.push_back(l);
            }
        }
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
            output << l.getName() << endl;
            output << l.getId() << endl; // Write the lecturer's ID to the file
            output << l.getDob() << endl;
            output << l.getSdt() << endl;
            if (!l.getCourses().empty()) // Check if the lecturer's course list is not empty
            {
                for (const string &course : l.getCourses())
                {
                    output << course << endl;
                }
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
    cout << "__________________________________________________" << endl;
    cout << "|                                                |" << endl;
    cout << "|   Select an option:                            |" << endl;
    cout << "|   1. Display students list                     |" << endl;
    cout << "|   2. Search in students list                   |" << endl;
    cout << "|   3. Register a new course for this semester   |" << endl;
    cout << "|   4. Show your personal information            |" << endl;
    cout << "|   5. Update your personal information          |" << endl;
    cout << "|   0. Sign out                                  |" << endl;
    cout << "|________________________________________________|" << endl;
}
void displayLecturerMenu()
{
    cout << endl;
    cout << "______________________________________________" << endl;
    cout << "|                                            |" << endl;
    cout << "|   Select an option:                        |" << endl;
    cout << "|   1. Display student list in your course   |" << endl;
    cout << "|   2. Show your personal information        |" << endl;
    cout << "|   3. Search student in your course         |" << endl;
    cout << "|   4. Update score for your students        |" << endl;
    cout << "|   5. Sort students in a course by grade    |" << endl;
    cout << "|   0. Sign out                              |" << endl;
    cout << "|____________________________________________|" << endl;
}
void displayDepartmentMenu()
{
    cout << endl;
    cout << "__________________________________" << endl;
    cout << "|                                |" << endl;
    cout << "|   Select an option:            |" << endl;
    cout << "|   1. Create new account        |" << endl;
    cout << "|   2. Delete account            |" << endl;
    cout << "|   3. Add course for lecturer   |" << endl;
    cout << "|   0. Sign out                  |" << endl;
    cout << "|________________________________|" << endl;
}
void displayParentsMenu()
{
    cout << endl;
    cout << "________________________________________" << endl;
    cout << "|                                      |" << endl;
    cout << "|   Select an option:                  |" << endl;
    cout << "|   1. View your child's information   |" << endl;
    cout << "|   0. Sign out                        |" << endl;
    cout << "|______________________________________|" << endl;
}

//-----------------------------  HÀM MAIN  -----------------------------
int main()
{
    clear();
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
             << "_____________________" << endl
             << "|                   |" << endl
             << "|   You are:        |" << endl
             << "|   S. Student      |" << endl
             << "|   L. Lecturer     |" << endl
             << "|   P. Parents      |" << endl
             << "|   D. Department   |" << endl
             << "|___________________|" << endl
             << "   Your choice: ";
        cin >> userType;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        clear();
        bool loginSuccess = false;
        if (tolower(userType) == 's') //----------------STUDENTS----------------
        {
            while (attemps < maxAttemps)
            {
                cout << endl
                     << "       Log in " << endl;
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
                    cout << "   Enter your choice: ";
                    cin >> choice;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    clear();
                    switch (choice)
                    {
                    case 1:
                        cout << "_____***_____" << endl;
                        cout << " Student List" << endl;
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
                     << "       Log in  " << endl;
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
                    {
                        cout << "Enter the course to view: ";
                        string course;
                        getline(cin, course);
                        Lecturer *current_lecturer = nullptr;
                        for (Lecturer &lecturer : lecturerlist)
                        {
                            if (lecturer.getId() == username)
                            {
                                current_lecturer = &lecturer;
                                break;
                            }
                        }
                        showListByCourse(studentlist, *current_lecturer, course);
                        break;
                    }
                    case 2:
                        inputPersonalInforForLecturer(lecturerlist, username);
                        break;
                    case 3:
                    {
                        search(studentlist);
                        break;
                    }
                    case 4:
                    {
                        Lecturer *current_lecturer = nullptr;
                        for (Lecturer &lecturer : lecturerlist)
                        {
                            if (lecturer.getId() == username)
                            {
                                current_lecturer = &lecturer;
                                break;
                            }
                        }
                        updateGrade(studentlist, *current_lecturer);
                        writeToFile(studentlist);
                        break;
                    }
                    case 5:
                    {
                        cout << "Enter the course to sort: ";
                        string courseName;
                        getline(cin, courseName);
                        vector<Student> studentsInCourse;
                        for (const Student &student : studentlist)
                        {
                            bool added = false;
                            for (const CourseGrade &course : student.getCourses())
                            {
                                if (course.course == courseName && !added)
                                {
                                    studentsInCourse.push_back(student);
                                    added = true;
                                }
                            }
                        }
                        sortAndPrintStudentsInCourse(studentsInCourse, courseName);
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

        else if (userType == 'P' || userType == 'p') //----------------Parents----------------
        {
            while (attemps < maxAttemps)
            {
                cout << endl
                     << "       Log in " << endl;
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
                     << "       Log in " << endl;
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
                            cout << "Lecturer Name: " << lecturer.getName() << ", ID: " << lecturer.getId() << endl;
                            cout << "Courses: " << endl;
                            for (const string &course : lecturer.getCourses())
                            {
                                cout << "- " << course << endl;
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
