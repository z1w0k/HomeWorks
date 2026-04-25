all: exam_system

exam_system: MexMatFaculty.o HumanitiesFaculty.o VmikFaculty.o DoubleFacultyStudent.o main.o
	g++ -g MexMatFaculty.o HumanitiesFaculty.o VmikFaculty.o DoubleFacultyStudent.o main.o -o exam_system

MexMatFaculty.o: MexMatFaculty.cpp MexMatFaculty.h Faculty.h Exam.h ExamResults.h StudentInfo.h
	g++ -std=c++23 -g -c MexMatFaculty.cpp -o MexMatFaculty.o

HumanitiesFaculty.o: HumanitiesFaculty.cpp HumanitiesFaculty.h Faculty.h Exam.h ExamResults.h StudentInfo.h
	g++ -std=c++23 -g -c HumanitiesFaculty.cpp -o HumanitiesFaculty.o

VmikFaculty.o: VmikFaculty.cpp VmikFaculty.h Faculty.h Exam.h ExamResults.h StudentInfo.h
	g++ -std=c++23 -g -c VmikFaculty.cpp -o VmikFaculty.o

DoubleFacultyStudent.o: DoubleFacultyStudent.cpp DoubleFacultyStudent.h MexMatFaculty.h HumanitiesFaculty.h Faculty.h Exam.h ExamResults.h StudentInfo.h
	g++ -std=c++23 -g -c DoubleFacultyStudent.cpp -o DoubleFacultyStudent.o

main.o: main.cpp Faculty.h MexMatFaculty.h HumanitiesFaculty.h VmikFaculty.h DoubleFacultyStudent.h Exam.h ExamResults.h StudentInfo.h
	g++ -std=c++23 -g -c main.cpp -o main.o

clean:
	rm -f *.o exam_system

run: exam_system
	./exam_system