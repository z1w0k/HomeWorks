#ifndef HUMANITIES_STUDENT_H
#define HUMANITIES_STUDENT_H
#include "Student.h"
#include <vector>

class HumanitiesStudent : public Student {
private:
    std::vector<std::string> knownLanguages;
    int journalPublications;
    bool hasPhilologyCertificate;
    
public:
    HumanitiesStudent(const std::string& name, int id, int course, 
                      const std::vector<std::string>& languages, 
                      int publications, bool certificate);
    
    bool canWriteScientificPaper(const PaperRequirements& req) const override;
    void displayInfo() const override;
};

#endif
