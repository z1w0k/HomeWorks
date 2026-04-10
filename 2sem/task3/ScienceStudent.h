#ifndef SCIENCE_STUDENT_H
#define SCIENCE_STUDENT_H
#include "Student.h"
#include <vector>

class ScienceStudent : public Student {
private:
    int labResearches;
    std::vector<std::string> labSkills;
    bool knowsStatistics;
    
public:
    ScienceStudent(const std::string& name, int id, int course,
                   int labs, const std::vector<std::string>& skills,
                   bool stats);
    
    bool canWriteScientificPaper(const PaperRequirements& req) const override;
    void displayInfo() const override;
};

#endif
