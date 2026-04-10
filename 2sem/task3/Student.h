#ifndef STUDENT_H
#define STUDENT_H
#include <string>
#include "PaperRequirements.h"

class Student {
protected:
    std::string name;
    int studentId;
    int course;
    
public:
    Student(const std::string& n, int id, int c) : name(n), studentId(id), course(c) {}
    virtual ~Student() {}
    
    // Чистая виртуальная функция - у каждого факультета своя логика
    virtual bool canWriteScientificPaper(const PaperRequirements& req) const = 0;
    virtual void displayInfo() const = 0;
    
    // Геттеры
    std::string getName() const { return name; }
    int getStudentId() const { return studentId; }
    int getCourse() const { return course; }
};

#endif
