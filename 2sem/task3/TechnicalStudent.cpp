#include "TechnicalStudent.h"
#include <iostream>
#include <algorithm>

TechnicalStudent::TechnicalStudent(const std::string& n, int id, int c,
                                   int conferences, const std::vector<std::string>& progLangs,
                                   bool math)
    : Student(n, id, c), conferenceParticipations(conferences), 
      programmingLanguages(progLangs), knowsHigherMath(math) {}

bool TechnicalStudent::canWriteScientificPaper(const PaperRequirements& req) const {
    // Технарям нужны: конференции, язык программирования как "язык статьи" - необычно, но осмысленно
    bool knowsRequiredLang = std::find(programmingLanguages.begin(), 
                                        programmingLanguages.end(), 
                                        req.requiredLanguage) != programmingLanguages.end();
    
    return (conferenceParticipations >= 1 || req.needConferenceParticipation) &&
           knowsRequiredLang &&
           knowsHigherMath &&
           (course >= 2); // со 2 курса
}

void TechnicalStudent::displayInfo() const {
    std::cout << "[Технический] " << name << " (ID: " << studentId << ", " << course << " курс)\n";
    std::cout << "  Участий в конференциях: " << conferenceParticipations << "\n";
    std::cout << "  Языки программирования: ";
    for (const auto& lang : programmingLanguages) std::cout << lang << " ";
    std::cout << "\n";
}
