#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <limits>

#include "Student.h"
#include "HumanitiesStudent.h"
#include "TechnicalStudent.h"
#include "ScienceStudent.h"
#include "PaperRequirements.h"

using namespace std;

// ТЕСТ: вызов чистой виртуальной функции через указатель на базовый класс
void runPolymorphismTest(const vector<unique_ptr<Student>>& students, const PaperRequirements& req) {
    cout << "\n========== ПОЛИМОРФНЫЙ ТЕСТ ==========\n";
    cout << "Требования к статье:\n";
    cout << "  - Мин. публикаций: " << req.minPublications << "\n";
    cout << "  - Нужны конференции: " << (req.needConferenceParticipation ? "да" : "нет") << "\n";
    cout << "  - Нужны лаб. исследования: " << (req.needLabResearch ? "да" : "нет") << "\n";
    cout << "  - Язык: " << req.requiredLanguage << "\n";
    cout << "----------------------------------------\n";
    
    for (const auto& student : students) {
        cout << student->getName() << " (курс " << student->getCourse() << "): ";
        if (student->canWriteScientificPaper(req)) {
            cout << "МОЖЕТ написать статью ✅\n";
        } else {
            cout << "НЕ МОЖЕТ написать статью ❌\n";
        }
    }
    cout << "========================================\n";
}

void printMenu() {
    cout << "\n=== УПРАВЛЕНИЕ СТУДЕНТАМИ ===\n";
    cout << "1. Добавить студента-гуманитария\n";
    cout << "2. Добавить технического студента\n";
    cout << "3. Добавить студента-естественника\n";
    cout << "4. Показать всех студентов\n";
    cout << "5. Удалить студента по ID\n";
    cout << "6. Запустить тест (полиморфный вызов)\n";
    cout << "7. Выйти\n";
    cout << "Выберите действие: ";
}

int main() {
    vector<unique_ptr<Student>> students;
    int nextId = 1;
    
    // Заранее заданные требования к научной статье
    PaperRequirements articleReqs(2, true, true, "English");
    
    while (true) {
        printMenu();
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (choice == 7) break;
        
        switch (choice) {
            case 1: {
                string name;
                int course, publications;
                vector<string> languages;
                bool certificate;
                
                cout << "Имя: "; getline(cin, name);
                cout << "Курс (1-4): "; cin >> course;
                cout << "Кол-во публикаций в журналах: "; cin >> publications;
                cout << "Есть сертификат филолога? (1-да/0-нет): "; cin >> certificate;
                cin.ignore();
                
                string lang;
                cout << "Изучаемые языки (через пробел, 'end' для завершения): ";
                while (cin >> lang && lang != "end") languages.push_back(lang);
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                students.push_back(make_unique<HumanitiesStudent>(name, nextId++, course, languages, publications, certificate));
                cout << "✅ Гуманитарий добавлен!\n";
                break;
            }
            case 2: {
                string name;
                int course, conferences;
                vector<string> progLangs;
                bool math;
                
                cout << "Имя: "; getline(cin, name);
                cout << "Курс (1-4): "; cin >> course;
                cout << "Участий в конференциях: "; cin >> conferences;
                cout << "Знает высшую математику? (1-да/0-нет): "; cin >> math;
                cin.ignore();
                
                string lang;
                cout << "Языки программирования (через пробел, 'end' для завершения): ";
                while (cin >> lang && lang != "end") progLangs.push_back(lang);
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                students.push_back(make_unique<TechnicalStudent>(name, nextId++, course, conferences, progLangs, math));
                cout << "✅ Технический студент добавлен!\n";
                break;
            }
            case 3: {
                string name;
                int course, labs;
                vector<string> skills;
                bool stats;
                
                cout << "Имя: "; getline(cin, name);
                cout << "Курс (1-4): "; cin >> course;
                cout << "Лабораторных исследований: "; cin >> labs;
                cout << "Знает статистику? (1-да/0-нет): "; cin >> stats;
                cin.ignore();
                
                string skill;
                cout << "Лабораторные навыки (через пробел, 'end' для завершения): ";
                while (cin >> skill && skill != "end") skills.push_back(skill);
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                students.push_back(make_unique<ScienceStudent>(name, nextId++, course, labs, skills, stats));
                cout << "✅ Естественник добавлен!\n";
                break;
            }
            case 4: {
                if (students.empty()) {
                    cout << "Нет студентов.\n";
                } else {
                    cout << "\n=== СПИСОК СТУДЕНТОВ ===\n";
                    for (const auto& s : students) {
                        s->displayInfo();
                        cout << "  Может ли писать статьи: " << (s->canWriteScientificPaper(articleReqs) ? "Да" : "Нет") << "\n\n";
                    }
                }
                break;
            }
            case 5: {
                int id;
                cout << "ID студента для удаления: "; cin >> id;
                auto it = remove_if(students.begin(), students.end(),
                    [id](const unique_ptr<Student>& s) { return s->getStudentId() == id; });
                if (it != students.end()) {
                    students.erase(it, students.end());
                    cout << "✅ Студент удалён.\n";
                } else {
                    cout << "❌ Студент с ID " << id << " не найден.\n";
                }
                break;
            }
            case 6: {
                runPolymorphismTest(students, articleReqs);
                break;
            }
            default:
                cout << "Неверный выбор.\n";
        }
    }
    
    cout << "Программа завершена.\n";
    return 0;
}
