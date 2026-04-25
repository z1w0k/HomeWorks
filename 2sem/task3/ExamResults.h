#ifndef EXAM_RESULTS_H
#define EXAM_RESULTS_H

#include <iostream>
#include <string>

struct ExamResult {
    int score;
    std::string grade;
    bool passed;

    ExamResult(int s = 0, const std::string& g = "", bool p = false)
        : score(s), grade(g), passed(p) {}

    void display() const {
        std::cout << "  Результат: " << score << "\n";
        std::cout << "  Оценка: " << grade << "\n";
        std::cout << "  Статус: " << (passed ? "СДАЛ" : "НЕ СДАЛ") << "\n";
    }
};

#endif