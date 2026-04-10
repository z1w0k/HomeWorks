#ifndef TECHNICAL_STUDENT_H
#define TECHNICAL_STUDENT_H
#include "Student.h"
#include <vector>

class TechnicalStudent : public Student {
private:
    int conferenceParticipations;
    std::vector<std::string> programmingLanguages;
    bool knowsHigherMath;
    
public:
    TechnicalStudent(const std::string& name, int id, int course,
                     int conferences, const std::vector<std::string>& progLangs,
                     bool math);
    
    bool canWriteScientificPaper(const PaperRequirements& req) const override;
    void displayInfo() const override;
};

#endif
