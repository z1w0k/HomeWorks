#include "HumanitiesFaculty.h"
#include <algorithm>

HumanitiesFaculty::HumanitiesFaculty() : Faculty("Гуманитарный факультет", 65) {}

ExamResult HumanitiesFaculty::takeExam(const StudentInfo& student, const Exam& exam) const {
    int finalScore = 0;
    
    if (student.essayLength > exam.minEssayLength * 2) {
        finalScore = exam.maxScore;
    } else {
        finalScore = (student.essayLength * exam.maxScore) / (exam.minEssayLength * 2);
    }
    
    bool hasWritingSkill = std::find(student.skills.begin(), student.skills.end(), "Письмо") != student.skills.end() ||
                           std::find(student.skills.begin(), student.skills.end(), "Литература") != student.skills.end();
    
    if (hasWritingSkill) {
        finalScore += 5;
    }
    
    finalScore += student.course * 2;
    
    finalScore += static_cast<int>(student.gpa * 3);
    
    if (exam.requiresPresentation) {
        if (student.madePresentation) {
            finalScore += 15;
        } else {
            finalScore -= 10;
        }
    }
    
    if (student.usesCalculator) {
        finalScore -= 15;
    }
    
    if (student.facultyType == 2) {
        finalScore += 20;
    }
    
    finalScore = std::min(std::max(finalScore, 0), exam.maxScore);
    
    bool passed = finalScore >= passingScore;
    std::string grade;
    if (passed) {
        grade = (finalScore >= 85) ? "Отлично" : ((finalScore >= 70) ? "Хорошо" : "Удовлетворительно");
    } else {
        grade = "Неудовлетворительно";
    }
    
    return ExamResult(finalScore, grade, passed);
}

std::string HumanitiesFaculty::getExamFormat(const Exam& exam) const {
    return "Эссе (минимум " + std::to_string(exam.minEssayLength) + " слов) + презентация. Калькуляторы не нужны.";
}

void HumanitiesFaculty::displayInfo() const {
    Faculty::displayInfo();
}