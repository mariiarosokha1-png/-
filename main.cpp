#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int MAX_STUDENTS = 100;
const string FILENAME = "students.txt";

struct Student {
    string fullName;
    string recordBookNum;
    string group;
    string specialty;
    int course;
    string studyForm;   // "budget" або "contract"
    string position;    // "student", "headman", "deputy"
    double scholarship;
};

Student students[MAX_STUDENTS];
int studentCount = 0;

void inputStudent() {
    if (studentCount >= MAX_STUDENTS) {
        cout << "Досягнуто максимум!" << endl; return;
    }
    char addMore = 'y';
    while (addMore == 'y' || addMore == 'Y') {
        Student s;
        cin.ignore();
        cout << "ПІБ студента: ";
        getline(cin, s.fullName);
        cout << "Номер залікової книжки: ";
        getline(cin, s.recordBookNum);
        cout << "Назва групи: ";
        getline(cin, s.group);
        cout << "Спеціальність: ";
        getline(cin, s.specialty);
        cout << "Курс (1-6): ";
        cin >> s.course;
        cin.ignore();
        cout << "Форма навчання (budget/contract): ";
        getline(cin, s.studyForm);
        cout << "Посада (student/headman/deputy): ";
        getline(cin, s.position);
        if (s.studyForm == "budget") {
            cout << "Розмір стипендії: ";
            cin >> s.scholarship;
        } else { s.scholarship = 0; }
        students[studentCount++] = s;
        cout << "Додати ще? (y/n): ";
        cin >> addMore;
    }
}

void saveToFile() {
    ofstream file(FILENAME);
    if (!file) { cout << "Помилка відкриття файлу!" << endl; return; }
    file << studentCount << "\n";
    for (int i = 0; i < studentCount; i++) {
        file << students[i].fullName << "\n";
        file << students[i].recordBookNum << "\n";
        file << students[i].group << "\n";
        file << students[i].specialty << "\n";
        file << students[i].course << "\n";
        file << students[i].studyForm << "\n";
        file << students[i].position << "\n";
        file << students[i].scholarship << "\n";
    }
    file.close();
    cout << "Збережено у " << FILENAME << endl;
}

void loadFromFile() {
    ifstream file(FILENAME);
    if (!file) { cout << "Файл не знайдено!" << endl; return; }
    file >> studentCount; file.ignore();
    for (int i = 0; i < studentCount; i++) {
        getline(file, students[i].fullName);
        getline(file, students[i].recordBookNum);
        getline(file, students[i].group);
        getline(file, students[i].specialty);
        file >> students[i].course; file.ignore();
        getline(file, students[i].studyForm);
        getline(file, students[i].position);
        file >> students[i].scholarship; file.ignore();
    }
    file.close();
    cout << "Завантажено " << studentCount << " записів." << endl;
}

void displayStudent(const Student& s) {
    cout << "ПІБ: " << s.fullName << endl;
    cout << "Залік. книжка: " << s.recordBookNum << endl;
    cout << "Група: " << s.group << ", Спец.: " << s.specialty << endl;
    cout << "Курс: " << s.course << ", Форма: " << s.studyForm << endl;
    cout << "Посада: " << s.position;
    cout << ", Стипендія: " << s.scholarship << " грн" << endl;
    cout << "---" << endl;
}

void displayAll() {
    if (studentCount == 0) { cout << "Немає даних." << endl; return; }
    for (int i = 0; i < studentCount; i++) displayStudent(students[i]);
}

void findByScholarship() {
    int course; double threshold;
    cout << "Введіть курс: "; cin >> course;
    cout << "Мін. стипендія: "; cin >> threshold;
    bool found = false;
    for (int i = 0; i < studentCount; i++) {
        if (students[i].course == course &&
            students[i].studyForm == "budget" &&
            students[i].scholarship > threshold) {
            displayStudent(students[i]); found = true;
        }
    }
    if (!found) cout << "Студентів не знайдено." << endl;
}

void findContractByGroup() {
    string group; cin.ignore();
    cout << "Введіть групу: "; getline(cin, group);
    bool found = false;
    for (int i = 0; i < studentCount; i++) {
        if (students[i].group == group &&
            students[i].studyForm == "contract") {
            displayStudent(students[i]); found = true;
        }
    }
    if (!found) cout << "Студентів не знайдено." << endl;
}

void findStudent() {
    cout << "1 - за ПІБ, 2 - за номером зал. книжки: ";
    int choice; cin >> choice; cin.ignore();
    string query;
    if (choice == 1) { cout << "ПІБ: "; getline(cin, query); }
    else { cout << "Номер: "; getline(cin, query); }
    bool found = false;
    for (int i = 0; i < studentCount; i++) {
        bool match = (choice == 1) ?
            (students[i].fullName.find(query) != string::npos) :
            (students[i].recordBookNum == query);
        if (match) { displayStudent(students[i]); found = true; }
    }
    if (!found) cout << "Студента не знайдено." << endl;
}

void countBudgetNoScholarship() {
    int count = 0;
    for (int i = 0; i < studentCount; i++)
        if (students[i].studyForm == "budget" &&
            students[i].scholarship == 0) count++;
    cout << "Бюджетників без стипендії: " << count << endl;
}

void showMenu() {
    cout << "\n=== ОБЛІК АКАДЕМІЧНОЇ ГРУПИ ===\n";
    cout << "1. Ввести дані про студентів\n";
    cout << "2. Зберегти дані у файл\n";
    cout << "3. Завантажити дані з файлу\n";
    cout << "4. Показати всіх студентів\n";
    cout << "5. Студенти Х курсу з підвищеною стипендією\n";
    cout << "6. Студенти Х групи на контракті\n";
    cout << "7. Пошук студента\n";
    cout << "8. Кількість бюджетників без стипендії\n";
    cout << "0. Вихід\n";
    cout << "Ваш вибір: ";
}

int main() {
    int choice;
    do {
        showMenu();
        cin >> choice;
        switch (choice) {
            case 1: inputStudent(); break;
            case 2: saveToFile(); break;
            case 3: loadFromFile(); break;
            case 4: displayAll(); break;
            case 5: findByScholarship(); break;
            case 6: findContractByGroup(); break;
            case 7: findStudent(); break;
            case 8: countBudgetNoScholarship(); break;
            case 0: cout << "До побачення!" << endl; break;
            default: cout << "Невірний вибір." << endl;
        }
    } while (choice != 0);
    return 0;
}