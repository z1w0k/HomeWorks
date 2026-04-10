#include "ScienceStudent.h"
#include <iostream>
#include <algorithm>

ScienceStudent::ScienceStudent(const std::string& n, int id, int c,
                               int labs, const std::vector<std::string>& skills,
                               bool stats)
    : Student(n, id, c), labResearches(labs), labSkills(skills), knowsStatistics(stats) {}

bool ScienceStudent::canWriteScientificPaper(const PaperRequirements& req) const {
    // Естественникам нужны: лабораторные исследования, статистика, знание языка
    bool hasRequiredSkill = false;
    if (req.requiredLanguage == "English" || req.requiredLanguage == "Latin") {
        hasRequiredSkill = std::find(labSkills.begin(), labSkills.end(), "ScientificWriting") 
                           != labSkills.end();
    }
    
    return (labResearches >= 2 || !req.needLabResearch) &&
           knowsStatistics &&
           hasRequiredSkill &&
           (course >= 3);
}

void ScienceStudent::displayInfo() const {
    std::cout << "[Естественно-научный] " << name << " (ID: " << studentId << ", " << course << " курс)\n";
    std::cout << "  Лабораторных исследований: " << labResearches << "\n";
    std::cout << "  Навыки: ";
    for (const auto& skill : labSkills) std::cout << skill << " ";
    std::cout << "\n";
}
