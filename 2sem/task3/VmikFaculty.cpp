#include "VmikFaculty.h"
#include <algorithm>

VmikFaculty::VmikFaculty() : Faculty("Факультет вычислительной математики и кибернетики", 75) {}

ExamResult VmikFaculty::takeExam(const StudentInfo& student, const Exam& exam) const {
    int finalScore = 0;
    
    if (exam.requiresLiveCoding) {
        if (student.canLiveCode) {
            finalScore = exam.maxScore;
        } else {
            finalScore = exam.maxScore / 2;
        }
    } else {
        finalScore = exam.maxScore / 2;
    }
    
    bool hasProgrammingSkill = false;
    for (const auto& lang : exam.allowedLanguages) {
        if (std::find(student.skills.begin(), student.skills.end(), lang) != student.skills.end()) {
            hasProgrammingSkill = true;
            break;
        }
    }
    
    if (hasProgrammingSkill) {
        finalScore += 10;
    }
    
    if (student.hasLaptop) finalScore += 5;
    if (!student.hasLaptop) finalScore -= 5;
    
    finalScore += student.course * 2;
    
    finalScore += static_cast<int>(student.gpa * 3);
    
    if (student.usesCalculator) finalScore += 5;
    
    if (student.facultyType == 3) {
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

std::string VmikFaculty::getExamFormat(const Exam& exam) const {
    std::string langs;
    for (size_t i = 0; i < exam.allowedLanguages.size() && i < 2; i++) {
        langs += exam.allowedLanguages[i];
        if (i < 1 && exam.allowedLanguages.size() > 1) langs += ", ";
    }
    return "Живое кодирование. Разрешённые языки: " + langs + ". Калькуляторы разрешены.";
}

void VmikFaculty::displayInfo() const {
    Faculty::displayInfo();
}