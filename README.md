# 🎓 University-Course-Registration-System
A system designed to simplify course selection for university students during each semester! 📚

# 🌟 Overview
This project enables students to:
* 📝 **Log in securely**.
* 📖 **View available courses**.
* 🎯 **Select their desired courses**.
* 👩‍🏫 **Get seat allotments based on availability**.

# 🛠️ Design Features.
The system is modular, ensuring maintainability and scalability. The core modules are:
* 1️⃣ Login Module
* 2️⃣ View Courses Module
* 3️⃣ Choice Filling Module
* 4️⃣ Seat Allotment Module

# 📂 Modules and Supporting Files

## 🔐 Login Module
Handles student authentication with two key operations:
* Login ✅
* Forgot Password 🔒
Stores user credentials securely in students.txt.

## 📋 View Courses Module
Reads course data from:
* core_courses.txt 🗂️
* elective_courses.txt 📜

Inputs: Semester and department.
Output: List of courses available for registration.

# 🛠️ Choice Filling Module
Allows students to manage their courses with:
* 1️⃣ Checking backlog details 📚 (stored in backlog_courses.bin).
* 2️⃣ Adding up to 6 courses ➕.
* 3️⃣ Dropping courses (within 2 weeks) ➖.

Selected courses are saved in:

* core_selected.bin 📁
* elective_selected.bin 📁

# 🪑 Seat Allotment Module
Allocates seats for courses based on preferences with operations:
* 1️⃣ Core courses allotment 🏫
* 2️⃣ Elective courses allotment 🧑‍🏫
* 3️⃣ Waitlisting (10% of class strength) 🕒

**Files used:**

* Allotment.bin
* Core_details.bin, Elective_details.bin
* Waitlist.bin

Waitlisted students are updated dynamically as seats become available!

# 🖥️ How to Compile and Run
## 🐧 Linux-based Machines
1. Open the terminal.
2. Navigate to the directory where the files are saved.
3. Compile: 
   ```shell
   gcc C_Project.c -o CourseRegistrationSystem
4. Run:
   ```shell
    ./CourseRegistrationSystem

# 💻 In Any C IDE
1. Create a project and add all source .c files and header files.
2. Build and compile the project.
3. Click "Run" to execute the program.

# ⚙️ Compilation Requirements
* Ensure all .c files, header files, and data files (.txt or .bin) are in the same directory.

# 🎯 Features At a Glance

| **Module**              | **Operations**                                                                                                      | **Files Used**                             |
|-------------------------|---------------------------------------------------------------------------------------------------------------------|--------------------------------------------|
| **Login Module**         | - Login<br>- Forgot Password                                                                                        | `students.txt`                             |
| **View Courses Module**  | - Displays courses based on semester and department                                                                 | `core_courses.txt`, `elective_courses.txt` |
| **Choice Filling Module**| - Add courses<br>- Drop courses<br>- Check backlog details                                                           | `backlog_courses.bin`, `core_selected.bin`, `elective_selected.bin` |
| **Seat Allotment Module**| - Core courses allotment<br>- Elective courses allotment<br>- Waitlisting students<br>- Updates seat availability    | `Allotment.bin`, `Core_details.bin`, `Elective_details.bin`, `Waitlist.bin` |


Contributors:
1. Priya Verma
2. Monish Kumar
3. Naveen Agarwal
   
### 🎉 Happy Coding! 🚀
Feel free to contribute or report issues in the issues section! 😊

