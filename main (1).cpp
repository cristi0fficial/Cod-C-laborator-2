#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream> 

enum class StudyField {
    MECHANICAL_ENGINEERING = 1,
    SOFTWARE_ENGINEERING = 2,
    FOOD_TECHNOLOGY = 3,
    URBANISM_ARCHITECTURE = 4,
    VETERINARY_MEDICINE = 5
};

class Student {
private:
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string enrollmentDate;
    std::string dateOfBirth;

public:
    Student(std::string first_name, std::string last_name, std::string email, std::string enrollment_date, std::string date_of_birth)
        : firstName(first_name), lastName(last_name), email(email), enrollmentDate(enrollment_date), dateOfBirth(date_of_birth) {}

    std::string getEmail() const {
        return email;
    }
};

class Faculty {
private:
    std::string name;
    std::string abbreviation;
    std::vector<Student> students;
    StudyField studyField;

public:
    Faculty(std::string name, std::string abbreviation, StudyField study_field)
        : name(name), abbreviation(abbreviation), studyField(study_field) {}

    void addStudent(const Student& student) {
        students.push_back(student);
    }

    std::string getName() const {
        return name;
    }

    std::vector<Student> getStudents() const {
        return students;
    }

    StudyField getStudyField() const {
        return studyField;
    }
};

class University {
private:
    std::vector<Faculty> faculties;

public:
    void createFaculty(std::string name, std::string abbreviation, StudyField study_field) {
        Faculty faculty(name, abbreviation, study_field);
        faculties.push_back(faculty);
    }

    bool assignStudentToFaculty(const std::string& student_email, const std::string& faculty_name) {
        for (auto& faculty : faculties) {
            if (faculty.getName() == faculty_name) {
                Student student(student_email, "", "", "", ""); 
                faculty.addStudent(student);
                return true;
            }
        }
        return false;
    }

    std::string searchFacultyByStudentEmail(const std::string& student_email) {
        for (const auto& faculty : faculties) {
            for (const auto& student : faculty.getStudents()) {
                if (student.getEmail() == student_email) {
                    return faculty.getName();
                }
            }
        }
        return "Student not found";
    }

    void displayUniversityFaculties() {
        for (const auto& faculty : faculties) {
            std::cout << faculty.getName() << std::endl;
        }
    }

    void displayAllFacultiesByField(StudyField study_field) {
        for (const auto& faculty : faculties) {
            if (faculty.getStudyField() == study_field) {
                std::cout << faculty.getName() << std::endl;
            }
        }
    }

    void saveState(const std::string& filename) {
        std::ofstream file(filename);
        if (file.is_open()) {
            for (const auto& faculty : faculties) {
                file << faculty.getName() << "," << static_cast<int>(faculty.getStudyField()) << ",";
                for (const auto& student : faculty.getStudents()) {
                    file << student.getEmail() << ",";
                }
                file << std::endl;
            }
            file.close();
            std::cout << "State saved successfully." << std::endl;
        } else {
            std::cerr << "Unable to open file." << std::endl;
        }
    }

    void loadState(const std::string& filename) {
        faculties.clear(); 
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string facultyName, studentEmail;
                int studyFieldInt;
                iss >> facultyName >> studyFieldInt;
                StudyField studyField = static_cast<StudyField>(studyFieldInt);
                createFaculty(facultyName, "", studyField);
                while (std::getline(iss, studentEmail, ',')) {
                    if (!studentEmail.empty()) 
                        assignStudentToFaculty(studentEmail, facultyName);
                }
            }
            file.close();
            std::cout << "State loaded successfully." << std::endl;
        } else {
            std::cerr << "Unable to open file." << std::endl;
        }
    }
};

int main() {
    University tumUniversity;

    while (true) {
        std::cout << "\n1. Create a new faculty" << std::endl;
        std::cout << "2. Create and assign a student to a faculty" << std::endl;
        std::cout << "3. Search what faculty a student belongs to by email" << std::endl;
        std::cout << "4. Display University faculties" << std::endl;
        std::cout << "5. Display all faculties belonging to a field" << std::endl;
        std::cout << "6. Save state" << std::endl;
        std::cout << "7. Load state" << std::endl;
        std::cout << "8. Exit" << std::endl;

        int choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::string name, abbreviation;
                int studyFieldInt;
                std::cout << "Enter faculty name: ";
                std::cin >> name;
                std::cout << "Enter faculty abbreviation: ";
                std::cin >> abbreviation;
                std::cout << "Enter study field (1 - MECHANICAL_ENGINEERING, 2 - SOFTWARE_ENGINEERING, 3 - FOOD_TECHNOLOGY, 4 - URBANISM_ARCHITECTURE, 5 - VETERINARY_MEDICINE): ";
                std::cin >> studyFieldInt;
                StudyField studyField = static_cast<StudyField>(studyFieldInt);
                tumUniversity.createFaculty(name, abbreviation, studyField);
                break;
            }
            case 2: {
                std::string studentEmail, facultyName;
                std::cout << "Enter student email: ";
                std::cin >> studentEmail;
                std::cout << "Enter faculty name to assign the student: ";
                std::cin >> facultyName;
                bool success = tumUniversity.assignStudentToFaculty(studentEmail, facultyName);
                if (success) {
                    std::cout << "Student assigned to faculty successfully." << std::endl;
                } else {
                    std::cout << "Faculty not found." << std::endl;
                }
                break;
            }
            case 3: {
                std::string studentEmail;
                std::cout << "Enter student email: ";
                std::cin >> studentEmail;
                std::string faculty = tumUniversity.searchFacultyByStudentEmail(studentEmail);
                std::cout << "The student belongs to " << faculty << " faculty." << std::endl;
               
                break;
            }
            case 4:
                std::cout << "University faculties:" << std::endl;
                tumUniversity.displayUniversityFaculties();
                break;
            case 5: {
                int studyFieldInt;
                std::cout << "Enter study field (1 - MECHANICAL_ENGINEERING, 2 - SOFTWARE_ENGINEERING, 3 - FOOD_TECHNOLOGY, 4 - URBANISM_ARCHITECTURE, 5 - VETERINARY_MEDICINE): ";
                std::cin >> studyFieldInt;
                StudyField studyField = static_cast<StudyField>(studyFieldInt);
                std::cout << "Faculties under " << studyFieldInt << ":" << std::endl;
                tumUniversity.displayAllFacultiesByField(studyField);
                break;
            }
            case 6: {
                std::string filename;
                std::cout << "Enter filename to save state: ";
                std::cin >> filename;
                tumUniversity.saveState(filename);
                break;
            }
            case 7: {
                std::string filename;
                std::cout << "Enter filename to load state: ";
                std::cin >> filename;
                tumUniversity.loadState(filename);
                break;
            }
            case 8:
                std::cout << "Exiting..." << std::endl;
                return 0;
         
        }
    }

    return 0;
}
