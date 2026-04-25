#ifndef STUDENT_INFO_H
#define STUDENT_INFO_H

#include <iostream>
#include <string>
#include <vector>

struct StudentInfo {
    std::string name;
    int studentId;
    int course;
    double gpa;
    std::vector<std::string> skills;
    int studyHoursPerWeek;
    bool hasLaptop;
    bool usesCalculator;
    
    int solvedProblems;
    int essayLength;
    bool madePresentation;
    bool canLiveCode;
    
    int facultyType;

    StudentInfo(const std::string& n = "", int id = 0, int c = 0, double g = 0.0,
                const std::vector<std::string>& sk = {}, int hours = 0, bool laptop = false, bool calculator = false,
                int solved = 0, int essay = 0, bool presentation = false, bool liveCode = false,
                int faculty = 0)
        : name(n), studentId(id), course(c), gpa(g), skills(sk), 
          studyHoursPerWeek(hours), hasLaptop(laptop), usesCalculator(calculator),
          solvedProblems(solved), essayLength(essay), madePresentation(presentation), canLiveCode(liveCode),
          facultyType(faculty) {}

    void display() const {
        std::cout << "  Имя: " << name << " (ID: " << studentId << ")\n";
        std::cout << "  Курс: " << course << ", GPA: " << gpa << "\n";
        std::cout << "  Часов в неделю: " << studyHoursPerWeek << "\n";
        std::cout << "  Использует калькулятор: " << (usesCalculator ? "да" : "нет") << "\n";
        std::cout << "  Решил задач: " << solvedProblems << "\n";
        std::cout << "  Написал слов в эссе: " << essayLength << "\n";
        std::cout << "  Сделал презентацию: " << (madePresentation ? "да" : "нет") << "\n";
        std::cout << "  Умеет лайвкодить: " << (canLiveCode ? "да" : "нет") << "\n";
        
        std::string facultyName;
        if (facultyType == 1) facultyName = "Мехмат";
        else if (facultyType == 2) facultyName = "Гуманитарный";
        else if (facultyType == 3) facultyName = "ВМиК";
        else if (facultyType == 4) facultyName = "Межфакультетский";
        std::cout << "  Факультет: " << facultyName << "\n";
        
        std::cout << "  Навыки: ";
        for (const auto& skill : skills) {
            std::cout << skill << " ";
        }
        std::cout << "\n";
    }
};

#endif