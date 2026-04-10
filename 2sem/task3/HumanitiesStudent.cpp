#include "HumanitiesStudent.h"
#include <iostream>
#include <algorithm>

HumanitiesStudent::HumanitiesStudent(const std::string& n, int id, int c,
                                      const std::vector<std::string>& langs,
                                      int pubs, bool cert)
    : Student(n, id, c), knownLanguages(langs), journalPublications(pubs), 
      hasPhilologyCertificate(cert) {}

bool HumanitiesStudent::canWriteScientificPaper(const PaperRequirements& req) const {
    // Гуманитариям нужны: публикации в журналах, знание языка, не нужны конференции и лаборатории
    bool knowsLanguage = std::find(knownLanguages.begin(), knownLanguages.end(), 
                                   req.requiredLanguage) != knownLanguages.end();
    
    return (journalPublications >= req.minPublications) && 
           knowsLanguage &&
           (hasPhilologyCertificate || course >= 3); // сертификат или старший курс
}

void HumanitiesStudent::displayInfo() const {
    std::cout << "[Гуманитарий] " << name << " (ID: " << studentId << ", " << course << " курс)\n";
    std::cout << "  Публикаций в журналах: " << journalPublications << "\n";
    std::cout << "  Языки: ";
    for (const auto& lang : knownLanguages) std::cout << lang << " ";
    std::cout << "\n";
}
