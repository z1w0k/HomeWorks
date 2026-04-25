#ifndef EXAM_H
#define EXAM_H

#include <string>
#include <iostream>
#include <vector>

struct Exam {
    std::string name;
    int maxScore;
    int durationMinutes;
    std::vector<std::string> topics;
    bool requiresProject;
    bool requiresOralDefense;
    
    int problemCount;
    int minEssayLength;
    bool requiresPresentation;
    bool requiresLiveCoding;
    std::vector<std::string> allowedLanguages;

    Exam(const std::string& n = "", int max = 0, int duration = 0, const std::vector<std::string>& t = {},
        bool project = false, bool oral = false,
        int problems = 10, int essayLen = 1500, bool presentation = true, bool liveCoding = true,
        const std::vector<std::string>& languages = {})
        : name(n), maxScore(max), durationMinutes(duration),
          topics(t), requiresProject(project), requiresOralDefense(oral),
          problemCount(problems), minEssayLength(essayLen), 
          requiresPresentation(presentation), requiresLiveCoding(liveCoding),
          allowedLanguages(languages) {}

    void display() const {
        std::cout << "  Экзамен: " << name << "\n";
        std::cout << "  Максимальный балл: " << maxScore << "\n";
        std::cout << "  Длительность: " << durationMinutes << "\n";
        std::cout << "  Темы: ";
        for (const auto& topic : topics) {
            std::cout << topic << " ";
        }
        std::cout << "\n";
        std::cout << "  Количество задач: " << problemCount << "\n";
        std::cout << "  Мин. длина эссе: " << minEssayLength << "\n";
        std::cout << "  Требуется презентация: " << (requiresPresentation ? "да" : "нет") << "\n";
        std::cout << "  Живое кодирование: " << (requiresLiveCoding ? "да" : "нет") << "\n";
    }
};

#endif