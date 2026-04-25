#include <iostream>
#include <vector>
#include <memory>
#include <limits>
#include <algorithm>

#include "Faculty.h"
#include "MexMatFaculty.h"
#include "HumanitiesFaculty.h"
#include "VmikFaculty.h"
#include "DoubleFacultyStudent.h"
#include "StudentInfo.h"
#include "Exam.h"

using namespace std;

Exam createDefaultExam() {
    vector<string> topics = {"Алгоритмы", "Структуры данных", "Математика"};
    vector<string> languages = {"C++", "Python", "C"};
    
    return Exam("Итоговая аттестация", 100, 180, topics,
                true, true, 12, 2000, true, true, languages);
}

void runPolymorphismTest(const vector<unique_ptr<Faculty>>& faculties, 
                          const StudentInfo& student, 
                          const Exam& exam) {
    cout << "Проверка студента на сдачу экзамена:\n";
    student.display();
    cout << "\n" << exam.name << ":\n";
    exam.display();
    
    for (const auto& faculty : faculties) {
        cout << "\n" << faculty->getFacultyName() << ":\n";
        cout << "  Формат: " << faculty->getExamFormat(exam) << "\n";
        ExamResult result = faculty->takeExam(student, exam);
        result.display();
    }
}

StudentInfo createMathStudent(int id) {
    string name;
    int course, hours, solved;
    double gpa;
    bool laptop, calculator;
    vector<string> skills;
    
    cout << "\nВвод данных студента МЕХМАТА:\n";
    cout << "Имя: ";
    getline(cin, name);
    
    cout << "Курс (1-4): ";
    cin >> course;
    while (course < 1 || course > 4) {
        cout << "Курс должен быть от 1 до 4. Повторите ввод: ";
        cin >> course;
    }
    
    cout << "Средний балл (0.0-5.0): ";
    cin >> gpa;
    while (gpa < 0 || gpa > 5) {
        cout << "Средний балл должен быть от 0 до 5. Повторите ввод: ";
        cin >> gpa;
    }
    
    cout << "Часов учебы в неделю: ";
    cin >> hours;
    
    cout << "Есть ноутбук (1-да/0-нет): ";
    cin >> laptop;
    
    cout << "Использует калькулятор (1-да/0-нет): ";
    cin >> calculator;
    
    cout << "Сколько задач решил (0-12): ";
    cin >> solved;
    while (solved < 0 || solved > 12) {
        cout << "Количество задач должно быть от 0 до 12. Повторите ввод: ";
        cin >> solved;
    }
    
    cin.ignore();
    
    cout << "\nПолезные навыки для мехмата: Топология, Дифгем\n";
    cout << "Навыки (через пробел, 'end' для завершения): ";
    string skill;
    while (cin >> skill && skill != "end") {
        skills.push_back(skill);
    }
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    return StudentInfo(name, id, course, gpa, skills, hours, laptop, calculator,
                       solved, 0, false, false, 1);
}

StudentInfo createHumanitiesStudent(int id) {
    string name;
    int course, hours, essayLength;
    double gpa;
    bool laptop, calculator, presentation;
    vector<string> skills;
    
    cout << "\nВвод данных студента ГУМАНИТАРНОГО ФАКУЛЬТЕТА:\n";
    cout << "Имя: ";
    getline(cin, name);
    
    cout << "Курс (1-4): ";
    cin >> course;
    while (course < 1 || course > 4) {
        cout << "Курс должен быть от 1 до 4. Повторите ввод: ";
        cin >> course;
    }
    
    cout << "Средний балл (0.0-5.0): ";
    cin >> gpa;
    while (gpa < 0 || gpa > 5) {
        cout << "Средний балл должен быть от 0 до 5. Повторите ввод: ";
        cin >> gpa;
    }
    
    cout << "Часов учебы в неделю: ";
    cin >> hours;
    
    cout << "Есть ноутбук (1-да/0-нет): ";
    cin >> laptop;
    
    cout << "Использует калькулятор (1-да/0-нет): ";
    cin >> calculator;
    
    cout << "Сколько слов написал в эссе: ";
    cin >> essayLength;
    while (essayLength < 0) {
        cout << "Количество слов не может быть отрицательным. Повторите ввод: ";
        cin >> essayLength;
    }
    
    cout << "Сделал презентацию (1-да/0-нет): ";
    cin >> presentation;
    
    cin.ignore();
    
    cout << "\nПолезные навыки для гуманитариев: Письмо, Литература\n";
    cout << "Навыки (через пробел, 'end' для завершения): ";
    string skill;
    while (cin >> skill && skill != "end") {
        skills.push_back(skill);
    }
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    return StudentInfo(name, id, course, gpa, skills, hours, laptop, calculator,
                       0, essayLength, presentation, false, 2);
}

StudentInfo createCSStudent(int id) {
    string name;
    int course, hours;
    double gpa;
    bool laptop, calculator, liveCode;
    vector<string> skills;
    
    cout << "\nВвод данных студента ВМК:\n";
    cout << "Имя: ";
    getline(cin, name);
    
    cout << "Курс (1-4): ";
    cin >> course;
    while (course < 1 || course > 4) {
        cout << "Курс должен быть от 1 до 4. Повторите ввод: ";
        cin >> course;
    }
    
    cout << "Средний балл (0.0-5.0): ";
    cin >> gpa;
    while (gpa < 0 || gpa > 5) {
        cout << "Средний балл должен быть от 0 до 5. Повторите ввод: ";
        cin >> gpa;
    }
    
    cout << "Часов учебы в неделю: ";
    cin >> hours;
    
    cout << "Есть ноутбук (1-да/0-нет): ";
    cin >> laptop;
    
    cout << "Использует калькулятор (1-да/0-нет): ";
    cin >> calculator;
    
    cout << "Умеет лайвкодить (1-да/0-нет): ";
    cin >> liveCode;
    
    cin.ignore();
    
    cout << "\nПолезные навыки для ВМиК: C++, Python, Java, JavaScript, Git, Open Source\n";
    cout << "Навыки (через пробел, 'end' для завершения): ";
    string skill;
    while (cin >> skill && skill != "end") {
        skills.push_back(skill);
    }
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    return StudentInfo(name, id, course, gpa, skills, hours, laptop, calculator,
                       0, 0, false, liveCode, 3);
}

StudentInfo createDoubleStudent(int id) {
    string name;
    int course, hours, solved, essayLength;
    double gpa;
    bool laptop, calculator, presentation, liveCode;
    vector<string> skills;
    
    cout << "\nВвод данных МЕЖФАКУЛЬТЕТСКОГО СТУДЕНТА (мехмат+гуманитарный)\n";
    cout << "Имя: ";
    getline(cin, name);
    
    cout << "Курс (1-4): ";
    cin >> course;
    while (course < 1 || course > 4) {
        cout << "Курс должен быть от 1 до 4. Повторите ввод: ";
        cin >> course;
    }
    
    cout << "Средний балл (0.0-5.0): ";
    cin >> gpa;
    while (gpa < 0 || gpa > 5) {
        cout << "Средний балл должен быть от 0 до 5. Повторите ввод: ";
        cin >> gpa;
    }
    
    cout << "Часов учебы в неделю: ";
    cin >> hours;
    
    cout << "Есть ноутбук (1-да/0-нет): ";
    cin >> laptop;
    
    cout << "Использует калькулятор (1-да/0-нет): ";
    cin >> calculator;
    
    cout << "Сколько задач решил (0-12): ";
    cin >> solved;
    while (solved < 0 || solved > 12) {
        cout << "Количество задач должно быть от 0 до 12. Повторите ввод: ";
        cin >> solved;
    }
    
    cout << "Сколько слов написал в эссе: ";
    cin >> essayLength;
    while (essayLength < 0) {
        cout << "Количество слов не может быть отрицательным. Повторите ввод: ";
        cin >> essayLength;
    }
    
    cout << "Сделал презентацию (1-да/0-нет): ";
    cin >> presentation;
    
    cout << "Умеет лайвкодить (1-да/0-нет): ";
    cin >> liveCode;
    
    cin.ignore();
    
    cout << "\nПолезные навыки: математические (Топология, Дифгем) и гуманитарные (Письмо, Литература)\n";
    cout << "Навыки (через пробел, 'end' для завершения): ";
    string skill;
    while (cin >> skill && skill != "end") {
        skills.push_back(skill);
    }
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    return StudentInfo(name, id, course, gpa, skills, hours, laptop, calculator,
                       solved, essayLength, presentation, liveCode, 4);
}

void printMenu() {
    cout << "1. Добавить студента мехмата\n";
    cout << "2. Добавить студента гуманитарного факультета\n";
    cout << "3. Добавить студента ВМК\n";
    cout << "4. Добавить межфакультетского студента (мехмат+гуманитарный)\n";
    cout << "5. Показать всех студентов\n";
    cout << "6. Удалить студента по ID\n";
    cout << "7. Запустить тест (все факультеты)\n";
    cout << "8. Выйти\n";
    cout << "Выберите действие: ";
}

int main() {
    vector<unique_ptr<Faculty>> faculties;
    faculties.push_back(make_unique<MexMatFaculty>());
    faculties.push_back(make_unique<HumanitiesFaculty>());
    faculties.push_back(make_unique<VmikFaculty>());
    
    vector<StudentInfo> students;
    int nextId = 1;
    
    Exam currentExam = createDefaultExam();
    
    while (true) {
        printMenu();
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (choice == 8) {
            cout << "До свидания!\n";
            break;
        }
        
        switch (choice) {
            case 1:
                students.push_back(createMathStudent(nextId++));
                cout << "\nСтудент мехмата добавлен! (получит +20 баллов на экзамене по математике)\n";
                break;
                
            case 2:
                students.push_back(createHumanitiesStudent(nextId++));
                cout << "\nСтудент гуманитарного факультета добавлен! (получит +20 баллов на экзамене по гуманитарным предметам)\n";
                break;
                
            case 3:
                students.push_back(createCSStudent(nextId++));
                cout << "\nСтудент ВМК добавлен! (получит +20 баллов на экзамене по программированию)\n";
                break;
                
            case 4: {
                StudentInfo info = createDoubleStudent(nextId++);
                students.push_back(info);
                DoubleFacultyStudent doubleStudent(info);
                doubleStudent.displayInfo();
                cout << "\nМежфакультетский студент добавлен! (сдаёт два экзамена, бонусов не получает)\n";
                break;
            }
            
            case 5:
                if (students.empty()) {
                    cout << "\nНет студентов.\n";
                } else {
                    cout << "\nСПИСОК ВСЕХ СТУДЕНТОВ:\n";
                    for (const auto& s : students) {
                        s.display();
                        cout << "РЕЗУЛЬТАТЫ ЭКЗАМЕНОВ:\n";
                        for (const auto& f : faculties) {
                            ExamResult result = f->takeExam(s, currentExam);
                            cout << "    " << f->getFacultyName() << ": " 
                                 << result.score << " баллов - " 
                                 << (result.passed ? "СДАЛ" : "НЕ СДАЛ") << "\n";
                        }
                    }
                }
                break;
                
            case 6: {
                if (students.empty()) {
                    cout << "\nНет студентов для удаления.\n";
                } else {
                    cout << "\nДоступные студенты:\n";
                    for (const auto& s : students) {
                        cout << "  ID: " << s.studentId << " - " << s.name << "\n";
                    }
                    cout << "Введите ID студента для удаления: ";
                    int id;
                    cin >> id;
                    auto it = remove_if(students.begin(), students.end(),
                        [id](const StudentInfo& s) { return s.studentId == id; });
                    if (it != students.end()) {
                        students.erase(it, students.end());
                        cout << "Студент с ID " << id << " удалён.\n";
                    } else {
                        cout << "Студент с ID " << id << " не найден.\n";
                    }
                }
                break;
            }
            
            case 7:
                if (students.empty()) {
                    cout << "\nНет студентов для тестирования.\n";
                } else {
                    cout << "\nТЕСТ:\n";
                    cout << "Доступные студенты:\n";
                    for (const auto& s : students) {
                        string facultyName;
                        if (s.facultyType == 1) facultyName = "Мехмат";
                        else if (s.facultyType == 2) facultyName = "Гуманитарный";
                        else if (s.facultyType == 3) facultyName = "ВМК";
                        else if (s.facultyType == 4) facultyName = "Межфакультетский";
                        cout << "  ID: " << s.studentId << " - " << s.name << " (" << facultyName << ")\n";
                    }
                    cout << "Выберите ID студента для теста: ";
                    int id;
                    cin >> id;
                    auto it = find_if(students.begin(), students.end(),
                        [id](const StudentInfo& s) { return s.studentId == id; });
                    if (it != students.end()) {
                        runPolymorphismTest(faculties, *it, currentExam);
                    } else {
                        cout << "Студент с ID " << id << " не найден.\n";
                    }
                }
                break;
                
            default:
                cout << "\nНеверный выбор. Пожалуйста, выберите пункт от 1 до 8.\n";
                break;
        }
    }
    
    return 0;
}