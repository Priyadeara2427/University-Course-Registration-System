#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CORE_COURSES 1000
#define MAX_ELEC_COURSES 1000
#define MAX_BACKLOG_COURSES 100
#define MAX__COURSES 100
#define MAX_WAITLIST 100

#define maxname_length 50
#define maxpassword_length 50
#define max_students 100
#define filename "students.txt"
#define file_core "Core_details.bin"
#define file_elec "Elective_details.bin"
#define filestudent "Students_details.bin"
#define file_waitlist "Waitlist.bin"
#define file_final "Allotment.bin"

struct core_courses
{
	char course_name[10][40];
	char course_code[10][10];
	char faculty_names[10][5][20];
}CSE[8],IT[8],ECE[8],EEE[8],MECH[8];
	
struct elective_courses
{
	char elective_course_name[10][40];
	char elective_course_code[10][10];
	char elective_faculty_names[10][5][20];
}cse[8],ece[8],it[8],eee[8],mech[8];

struct core_selectedcourses{
    char course_code[10];
    char course_name[100];
    int student_id;
    char faculty_name[30];
}core_courses[MAX_CORE_COURSES],core_courses_read[MAX_CORE_COURSES];

struct elective_selectedcourses {
    char course_code[10];
    char course_name[100];
    int student_id;
    char faculty_name[30];
}elec_courses[MAX_ELEC_COURSES], elec_courses_read[MAX_ELEC_COURSES];

struct backlog_courses{
    char course_code[10];
    char course_name[50];
    int student_id;
    char faculty_name[30];
    char course_type[10];
};

typedef struct
{
    char username[50];
    char password[50];
    int student_id;
    char name[50];
    int no_selected_courses;
    char allotment_status[3];
    char waitlist[3];
} StudentDetails;


typedef struct 
{
    char username[maxname_length];
    char password[maxpassword_length];
} Student;

#define MAX_ALLOTMENT 1000

typedef struct 
{
    int sem;
    char department[10];
    char course_code[10];
    int strength;
    int waitlist;
    char faculty[50];
    int section;
    int credit;
    char course_name[100];  
    int available_seats;
}Course;

typedef struct 
{
    int student_id;
    char course_code[10];
    char faculty[50];
    float CGPA;
    int HasBacklog;
    char course_type[10];
    char name[50];

} Waitlist;


typedef struct
{
    int student_id;
    char student_name[50];
    char course_code[10];
    char course_name[100];
    char faculty[50];
    char status[50];
} final_details;


#include <stdio.h>
#include <string.h>

#define MAX_PREREQUISITES 20
#define MAX_COMPLETED_COURSES 100


struct prerequisite {
    char course_code[10];
    char prerequisite_code[10];
};

struct completed_course {
    char course_code[10];
    int student_id;
};

struct prerequisite prerequisites[MAX_PREREQUISITES];
struct completed_course completed_courses[MAX_COMPLETED_COURSES];


//Function Prototypes.
void loadStudents();
void saveStudents();
int findStudent(const char *username);
void changePassword(int studentIndex);
int login();
void Successfull_Login(int *Student_Id);
int store_core_courses();
int store_elective_courses();
void View_Courses();
void course_registration(int *Student_Id);
void add_courses(int *Student_Id);
int core_registration(int num_core_courses, int *Student_Id);
int elective_registration(int num_elec_courses, int *Student_Id);
int backlog_courses(int num_courses, int *Student_Id);
int days_gap_calculation(int start_date, int start_month, int start_year, int current_date,int  current_month, int current_year);
int Check_Eligibility(void);
void Drop_Course(int *Student_Id);
void Display_SelectedCourses(int *Student_Id);
void Display_Waitlist(void);
void Course_WiseAvailability(void);
void Allotment_Process(int *Student_Id);
void WriteStudentDetails(void);
void Course_Allotment(const char *faculty, const char *course_code, char type, int *Student_Id);
void readCoursesFromFile(void);
void writeCoursesToFile(void);
void writeWaitlist(int *Student_Id, const char *faculty, const char *course_code, char type);
void Display_waitlist_result(int *Student_Id);
void manageWaitlist(void);
int checkWaitlistStatus(int *Student_Id, const char *course_code, const char *faculty);
void Display_Allotment(void);
int has_completed_prerequisite(int *Student_Id, char *course_code);
int check_prerequisites(int *Student_Id, char *course_code);
void add_completed_course(char *course_code, int *Student_Id);
void prerequisite_add(void);

Student students[max_students];
int studentCount =0;

int main() 
{

    loadStudents();
    int studentIndex =login();
    //WriteStudentDetails();
    if (studentIndex !=-1) 
    {
        int Std_id;
        StudentDetails studentid;
        FILE *userfile = fopen(filestudent, "rb");
        while (fread(&studentid, sizeof(StudentDetails), 1, userfile) == 1)
        {
            if (strcmp(students[studentIndex].username, studentid.username)==0)
            {
                printf("Welcome, %s!\n",studentid.name);
                Std_id = studentid.student_id;
                break;
            }
        }
        int *Student_Id = &Std_id;
        // Write course records to the file
        //writeCoursesToFile();
        
        Successfull_Login(Student_Id);
    } 
    else 
    {
        printf("Login failed.\n");
    }

    return 0;
}

void loadStudents()
{
    FILE *file =fopen(filename, "r");  
    if (file ==NULL) {
        printf("Error opening file.\n");
        return;
    }

    fscanf(file, "%d", &studentCount); 
    for (int i =0; i < studentCount; i++)
    {
        fscanf(file,"%49s %49s",students[i].username,students[i].password);
    }
    fclose(file);
}

void saveStudents() 
{
    FILE *file = fopen(filename, "w"); 
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    fprintf(file, "%d\n", studentCount); 
    for (int i = 0; i < studentCount; i++) 
    {
        fprintf(file, "%s %s\n",students[i].username,students[i].password);
    }
    fclose(file);
}

int findStudent(const char *username)
{
    for (int i = 0;i < studentCount;i++) 
    {
        if (strcmp(students[i].username, username) ==0)
        {
            return i;
        }
    }
    return -1;
}

void changePassword(int studentIndex)
{
    char newPassword[maxpassword_length];
    printf("Enter new password: ");
    scanf("%s", newPassword);

    strcpy(students[studentIndex].password,newPassword);
    saveStudents();
    printf("Password changed successfully.\n");
}

int login()
{
    char username[maxname_length];
    char password[maxpassword_length];
    int attempts =3;

    while (attempts > 0) {
        printf("Enter username: ");
        scanf("%s",username);
        int studentIndex =findStudent(username);

        if (studentIndex ==-1) {
            printf("Username not found.\n");
            attempts--;
            continue;
        }

        printf("Enter password: ");
        scanf("%s",password);

        if (strcmp(students[studentIndex].password,password) ==0) 
        {
            printf("Login successful.\n");
            return studentIndex;
        } 
        else 
        {
            printf("Incorrect password.\n");
            attempts--;
            if (attempts ==0)
            {
                printf("Forgot password?(yes/no): ");
                char choice[4];
                scanf("%s",choice);
                if (strcmp(choice, "yes") == 0) {
                    changePassword(studentIndex);
                    return studentIndex;
                }
            }
        }
    }

    return -1;
}


void Successfull_Login(int *Student_Id)
{
    int flag = 1;
    while (flag==1)
    {
        printf("\n--------------WELCOME TO UNIVERSITY COURSE REGISTRATION-----------------\n\n");
        printf("\t1. View Courses\n");
        printf("\t2. Check Eligibility Criteria\n");
        printf("\t3. Course Registration.\n");
        printf("\t4. Add Extra Course.\n");
        printf("\t5. Drop Courses.\n");
        printf("\t6. Display my Selected Courses.\n");
        printf("\t7. Display Waitlist.\n");
        printf("\t8. Course Wise Availability and Waitlist\n");
        printf("\t9. Course Allotment Status.\n");
        printf("\t10. Display my Waitlist result after course registartion closes.\n");
        printf("\t11. Display Complete Allotment details.\n");
        printf("\t12. Logout\n");
    
        int choice;
        printf("\nEnter your Choice:  ");
        scanf("%d",&choice);
        getchar();
        int add_ch, no_register; 

        switch (choice)
        {
            case 1:
                View_Courses();
                break;
            
            case 2: // Checking the Eligibility Criteria.
                flag = Check_Eligibility();
                if (flag==1)
                {
                    printf("You are Eligible to proceed for Course Registration.\n");
                }
                else
                {
                    printf("Sorry! You are not eligible to proceed for Course Registration.\n");
                    flag=1;
                }
                break;

            case 3: // Initial Course Registration.
                course_registration(Student_Id);
                break;
            
            
            case 4: // Adding of Courses.
                add_courses(Student_Id); 
                break;

            case 5: // Droping of Courses.
                Drop_Course(Student_Id);
                break;

            case 6: // Displaying Selected Courses.
                Display_SelectedCourses(Student_Id);
                break;

            case 7:
                Display_Waitlist();
                break;

            case 8:
                Course_WiseAvailability();
                break;

            case 9:
                Allotment_Process(Student_Id);
                break;
            
            case 10:
                Display_waitlist_result(Student_Id); 
                break;

            case 11:
                Display_Allotment();
                break;

            case 12:
                printf("Thank you for Course Registration. Happy Journey Ahead. Best luck :) \n");
                flag = 0;
                break;
        }
    }
}

void Course_WiseAvailability(void)
{
    FILE *fp_core = fopen(file_core, "rb");
    if (fp_core== NULL) {
        perror("Error opening file for reading");
        exit(EXIT_FAILURE);
    }

    int sem;
    char dept[10];
    printf("Enter the semester: ");
    scanf("%d", &sem);
    printf("Enter the Department: ");
    scanf("%s", dept);

    Course course_core;
    printf("Core Course Details:\n");
    printf("%-10s %-40s %-20s %-10s %s\n", "Code", "Name", "Faculty", "Seats", "Waitlist");

    while (fread(&course_core, sizeof(Course), 1, fp_core) == 1) 
    {
        if (course_core.sem==sem && strcmp(course_core.department, dept)==0)
            printf("%-10s %-40s %-20s %-10d  %d\n", course_core.course_code, course_core.course_name, course_core.faculty, course_core.available_seats, course_core.waitlist);
    }
    
    fclose(fp_core);

    FILE *fp_elec = fopen(file_elec, "rb");
    if (fp_elec== NULL) {
        perror("Error opening file for reading");
        exit(EXIT_FAILURE);
    }

    Course course_elec;
    printf("\n Elective Course Details:\n");

    printf("%-10s %-40s %-20s %-10s %s\n", "Code", "Name", "Faculty", "Seats", "Waitlist");

    while (fread(&course_elec, sizeof(Course), 1, fp_elec) == 1) 
    {
        if (course_elec.sem==sem && strcmp(course_elec.department, dept)==0)
            printf("%-10s %-40s %-20s %-10d  %d\n", course_elec.course_code, course_elec.course_name, course_elec.faculty, course_elec.available_seats, course_elec.waitlist);
    }

    fclose(fp_elec);
}


int store_core_courses(void)
{	
	FILE *file = fopen("core_courses.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }

    char line[10000];
	char token[8][40]; 
	int index= 0,semester,i;
	char index_check[]="1";
    while (fgets(line, sizeof(line), file)) {
        int token_count = 0;
        for (i = 0; i < 7; i++) {
        	token[i][0] = '\0';
    	}
    	
        char *tok = strtok(line, ",");
        while (tok != NULL){
            strcpy(token[token_count], tok); 
            token_count++;
            tok = strtok(NULL, ",");
		}
		
		if(strcmp(index_check,token[1])==0){
		}
		
		else{
			strcpy(index_check,token[1]);
			index=0;
		}
		if (strcmp(token[0],"CSE")==0){
			semester=atoi(token[1]);
				strcpy(CSE[semester-1].course_code[index],token[2]);
				strcpy(CSE[semester-1].course_name[index],token[3]);
				strcpy(CSE[semester-1].faculty_names[index][0],token[4]);
				strcpy(CSE[semester-1].faculty_names[index][1],token[5]);
				strcpy(CSE[semester-1].faculty_names[index][2],token[6]);
				index++;
			}
		else if(strcmp(token[0],"IT")==0){
			semester=atoi(token[1]);
				strcpy(IT[semester-1].course_code[index],token[2]);
				strcpy(IT[semester-1].course_name[index],token[3]);
				strcpy(IT[semester-1].faculty_names[index][0],token[4]);
				strcpy(IT[semester-1].faculty_names[index][1],token[5]);
				strcpy(IT[semester-1].faculty_names[index][2],token[6]);
				index++;	
			}
		else if(strcmp(token[0],"MECH")==0){
			semester=atoi(token[1]);
				strcpy(MECH[semester-1].course_code[index],token[2]);
				strcpy(MECH[semester-1].course_name[index],token[3]);
				strcpy(MECH[semester-1].faculty_names[index][0],token[4]);
				strcpy(MECH[semester-1].faculty_names[index][1],token[5]);
				strcpy(MECH[semester-1].faculty_names[index][2],token[6]);
				index++;	
			}
		else if(strcmp(token[0],"ECE")==0){
			semester=atoi(token[1]);
				strcpy(ECE[semester-1].course_code[index],token[2]);
				strcpy(ECE[semester-1].course_name[index],token[3]);
				strcpy(ECE[semester-1].faculty_names[index][0],token[4]);
				strcpy(ECE[semester-1].faculty_names[index][1],token[5]);
				strcpy(ECE[semester-1].faculty_names[index][2],token[6]);
				index++;	
			}
		else if(strcmp(token[0],"EEE")==0){
			semester=atoi(token[1]);
				strcpy(EEE[semester-1].course_code[index],token[2]);
				strcpy(EEE[semester-1].course_name[index],token[3]);
				strcpy(EEE[semester-1].faculty_names[index][0],token[4]);
				strcpy(EEE[semester-1].faculty_names[index][1],token[5]);
				strcpy(EEE[semester-1].faculty_names[index][2],token[6]);
				index++;	
			}
		}

    
    fclose(file);
	
	
}

int store_elective_courses(void){
	
	FILE *file_e = fopen("elective_courses.txt", "r");
    if (file_e == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }

    char line_e[10000];
	char token_e[8][40]; 
	int index_e= 0,semester_e,i;
	char index_check_e[]="1";
    while (fgets(line_e, sizeof(line_e), file_e)) {
        int token_count_e = 0;
        for (i = 0; i < 7; i++) {
        	token_e[i][0] = '\0';
    	}
    	
        char *tok_e = strtok(line_e, ",");
        while (tok_e != NULL){
            strcpy(token_e[token_count_e], tok_e); 
            token_count_e++;
            tok_e = strtok(NULL, ",");
		}
		
		if(strcmp(index_check_e,token_e[1])==0){
		}
		
		else{
			strcpy(index_check_e,token_e[1]);
			index_e=0;
		}
		if (strcmp(token_e[0],"CSE")==0){
			semester_e=atoi(token_e[1]);
				strcpy(cse[semester_e-1].elective_course_code[index_e],token_e[2]);
				strcpy(cse[semester_e-1].elective_course_name[index_e],token_e[3]);
				strcpy(cse[semester_e-1].elective_faculty_names[index_e][0],token_e[4]);
				strcpy(cse[semester_e-1].elective_faculty_names[index_e][1],token_e[5]);
				strcpy(cse[semester_e-1].elective_faculty_names[index_e][2],token_e[6]);
				index_e++;
			}
		else if(strcmp(token_e[0],"IT")==0){
			semester_e=atoi(token_e[1]);
				strcpy(it[semester_e-1].elective_course_code[index_e],token_e[2]);
				strcpy(it[semester_e-1].elective_course_name[index_e],token_e[3]);
				strcpy(it[semester_e-1].elective_faculty_names[index_e][0],token_e[4]);
				strcpy(it[semester_e-1].elective_faculty_names[index_e][1],token_e[5]);
				strcpy(it[semester_e-1].elective_faculty_names[index_e][2],token_e[6]);
				index_e++;	
			}
		else if(strcmp(token_e[0],"MECH")==0){
			semester_e=atoi(token_e[1]);
				strcpy(mech[semester_e-1].elective_course_code[index_e],token_e[2]);
				strcpy(mech[semester_e-1].elective_course_name[index_e],token_e[3]);
				strcpy(mech[semester_e-1].elective_faculty_names[index_e][0],token_e[4]);
				strcpy(mech[semester_e-1].elective_faculty_names[index_e][1],token_e[5]);
				strcpy(mech[semester_e-1].elective_faculty_names[index_e][2],token_e[6]);
				index_e++;	
			}
		else if(strcmp(token_e[0],"ECE")==0){
			semester_e=atoi(token_e[1]);
				strcpy(ece[semester_e-1].elective_course_code[index_e],token_e[2]);
				strcpy(ece[semester_e-1].elective_course_name[index_e],token_e[3]);
				strcpy(ece[semester_e-1].elective_faculty_names[index_e][0],token_e[4]);
				strcpy(ece[semester_e-1].elective_faculty_names[index_e][1],token_e[5]);
				strcpy(ece[semester_e-1].elective_faculty_names[index_e][2],token_e[6]);
				index_e++;	
			}
		else if(strcmp(token_e[0],"EEE")==0){
			semester_e=atoi(token_e[1]);
				strcpy(eee[semester_e-1].elective_course_code[index_e],token_e[2]);
				strcpy(eee[semester_e-1].elective_course_name[index_e],token_e[3]);
				strcpy(eee[semester_e-1].elective_faculty_names[index_e][0],token_e[4]);
				strcpy(eee[semester_e-1].elective_faculty_names[index_e][1],token_e[5]);
				strcpy(eee[semester_e-1].elective_faculty_names[index_e][2],token_e[6]);
				index_e++;	
			}
		}    
    fclose(file_e);

	
}
void View_Courses(void)
{
	store_core_courses();
	store_elective_courses();
	int semesterI,i,j;
	char department[10];
    char search_faculty[20] ;
	
	printf("Enter your semester number (1,2,3,4,5,6,7,8): ");
    scanf("%d", &semesterI);
    if(semesterI>8 || semesterI<1){
    	printf("\nPlease enter a valid semester detail.\n");
	}
    
    printf("Enter your department (CSE/ IT/ ECE/ EEE/ MECH): ");
    scanf(" %s", department); // Include space to handle spaces in department names

	if(strcmp(department,"CSE")==0){
			printf("\nCore Courses Available are:\n\n");
			for(i=0;i<6;i++){
			printf("%s %s \n",CSE[semesterI-1].course_code[i],CSE[semesterI-1].course_name[i]);
			puts("The faculties availble are:"); 
			for(j=0;j<3;j++){
				puts(CSE[semesterI-1].faculty_names[i][j]);
			}
		}
		
			printf("Elective Courses Available are:\n\n");
			for( i=0;i<3;i++){
			printf("%s %s \n",cse[semesterI-1].elective_course_code[i],cse[semesterI-1].elective_course_name[i]);
			puts("The faculties availble are:");
			for( j=0;j<2;j++){
				puts(cse[semesterI-1].elective_faculty_names[i][j]);
			}
		}
	}
	
	else if(strcmp(department,"ECE")==0) {
			printf("\nCore Courses Available are:\n\n");
			for( i=0;i<6;i++){
			printf("%s %s\n",ECE[semesterI-1].course_code[i],ECE[semesterI-1].course_name[i]);
			puts("The faculties availble are:");
			for( j=0;j<3;j++){
				puts(ECE[semesterI-1].faculty_names[i][j]);
			}
		}
		
			printf("Elective Courses Available are:\n\n");
			for( i=0;i<3;i++){
			printf("%s %s\n",ece[semesterI-1].elective_course_code[i],ece[semesterI-1].elective_course_name[i]);
			puts("The faculties availble are:");
			for( j=0;j<2;j++){
				puts(ece[semesterI-1].elective_faculty_names[i][j]);
			}
		}
	}
			
	else if(strcmp(department,"IT")==0) {
			printf("\nCore Courses Available are:\n\n");
			for( i=0;i<6;i++){
			printf("%s %s \n",IT[semesterI-1].course_code[i],IT[semesterI-1].course_name[i]);
			puts("The faculties availble are:");
			for( j=0;j<3;j++){
				puts(IT[semesterI-1].faculty_names[i][j]);
			}
		}
		
			printf("Elective Courses Available are:\n\n");
			for( i=0;i<3;i++){
			printf("%s %s\n",it[semesterI-1].elective_course_code[i],it[semesterI-1].elective_course_name[i]);
			puts("The faculties availble are:");
			for( j=0;j<2;j++){
				puts(it[semesterI-1].elective_faculty_names[i][j]);
			}
		}
	}
	
	else if(strcmp(department,"EEE")==0) {
			printf("\nCore Courses Available are:\n\n");
			for( i=0;i<6;i++){
			printf("%s %s \n",EEE[semesterI-1].course_code[i],EEE[semesterI-1].course_name[i]);
			puts("The faculties availble are:");
			for( j=0;j<3;j++){
				puts(EEE[semesterI-1].faculty_names[i][j]);
			}
		}
		
			printf("Elective Courses Available are:\n\n");
			for( i=0;i<3;i++){
			printf("%s %s\n",eee[semesterI-1].elective_course_code[i],eee[semesterI-1].elective_course_name[i]);
			puts("The faculties availble are:");
			for( j=0;j<2;j++){
				puts(eee[semesterI-1].elective_faculty_names[i][j]);
			}
		}
	}
	
	else if(strcmp(department,"MECH")==0) {
			printf("\nCore Courses Available are:\n\n");
			for( i=0;i<6;i++){
			printf("%s %s \n",MECH[semesterI-1].course_code[i],MECH[semesterI-1].course_name[i]);
			puts("The faculties availble are:");
			for( j=0;j<3;j++){
				puts(MECH[semesterI-1].faculty_names[i][j]);
			}
		}
		
			printf("Elective Courses Available are:\n\n");
			for( i=0;i<3;i++){
			printf("%s %s\n",mech[semesterI-1].elective_course_code[i],mech[semesterI-1].elective_course_name[i]);
			puts("The faculties availble are:");
			for( j=0;j<2;j++){
				puts(mech[semesterI-1].elective_faculty_names[i][j]);
			}
		}
	}
	else{
		printf("\nPlease enter a valid department.\n");
	}
	printf("\nEnter faculty name to search course: ");
    fgets(search_faculty, sizeof(search_faculty), stdin);
	if (fgets(search_faculty, sizeof(search_faculty), stdin) != NULL) {
        size_t len = strlen(search_faculty);
        if (len > 0 && search_faculty[len - 1] == '\n') {
            search_faculty[len - 1] = '\0';
        }
    } else {
        printf("Error reading input.");
    }
	
	struct core_courses (*selected_department)[8];
	struct elective_courses (*elective_selected_department)[8];

    if (strcmp(department, "CSE") == 0) {
        selected_department = &CSE;
        elective_selected_department = &cse;
    } else if (strcmp(department, "IT") == 0) {
        selected_department = &IT;
        elective_selected_department=&it;
    } else if (strcmp(department, "ECE") == 0) {
        selected_department = &ECE;
        elective_selected_department=&ece;
    } else if (strcmp(department, "EEE") == 0) {
        selected_department = &EEE;
        elective_selected_department=&eee;
    } else if (strcmp(department, "MECH") == 0) {
        selected_department = &MECH;
        elective_selected_department=&mech;
	}
	
	int u,k,l,flag=0;
	for(u=0;u<8;u++){
		for(k=0;k<6;k++){
			for(l=0;l<3;l++){
				if(strcmp(search_faculty,(*selected_department)[u].faculty_names[k][l]) == 0){
		 				printf("Core Course :%s %s \n",(*selected_department)[u].course_code[k] , (*selected_department)[u].course_name[k]);
		 				flag=1;
		 		}
		 		if(strcmp(search_faculty,(*elective_selected_department)[u].elective_faculty_names[k][l]) == 0){
		 				printf("Elective Course :%s %s \n",(*elective_selected_department)[u].elective_course_code[k] , (*elective_selected_department)[u].elective_course_name[k]);
		 				flag=1;
		 		}
			}	
		}	
	}
	if(flag==0)
		printf("Not Found!");

}


void course_registration(int *Student_Id)
{
    StudentDetails reg;
    long position;
    FILE *userfile = fopen(filestudent, "r+b");

    int no_courses, no_core_courses, backlog_no, no_elec_courses ;
    char sem_result;
    printf("Do you have any arear (Y/N): ");
    scanf("%c", &sem_result);

    if (sem_result=='Y'){
        printf("Number of backlog courses: ");
        scanf("%d",&backlog_no);
        getchar();
        int backlog = backlog_courses(backlog_no,Student_Id);
    }
    else{
        printf("\nExcellent, Keep going like this!\n\n");
    }

    printf("\nEnter the total number of courses to register (Min 4 and Max 6): ");
    scanf("%d", &no_courses);
    getchar();
    
    int no_already;
    char status[3], waitlist[3];
    while (fread(&reg, sizeof(StudentDetails), 1, userfile) == 1)
    {
        if (reg.student_id == *Student_Id)
        {
            no_already = reg.no_selected_courses;
            strcpy(status, reg.allotment_status);
            strcpy(waitlist, reg.waitlist);

        }
    }
    if (no_already+no_courses <=6 && strcmp(status, "YES")!=0 && strcmp(waitlist, "YES")!=0)
    {
        if (no_courses<=6 && no_courses>=3)
        {
            printf("You are good to go for choice filling :)\n\n");
        }
        else if (no_courses<4)
        {
            printf("Minimum requirement of courses is 4. So, Enter number of courses again: ");
            scanf("%d", &no_courses);
            getchar();
        }
        else if (no_courses>6){
            printf("You have reached maximum limit to register for courses! \n\n");
        }
        printf("\nEnter the total number of core courses to register: ");
        scanf("%d", &no_core_courses);
        getchar();

        no_elec_courses = no_courses - no_core_courses;
        
        int last_sem;
        char c_code[10];
        printf("Number of completed courses in last semester: ");
        scanf("%d",&last_sem);
        getchar();

        strcpy(prerequisites[0].course_code, "CS2101");
        strcpy(prerequisites[0].prerequisite_code, "CS1101");

        strcpy(prerequisites[1].course_code, "CS2104");
        strcpy(prerequisites[1].prerequisite_code, "CS1102");

        strcpy(prerequisites[2].course_code, "CS3105");
        strcpy(prerequisites[2].prerequisite_code, "CS1104");

        strcpy(prerequisites[3].course_code, "CS4104");
        strcpy(prerequisites[3].prerequisite_code, "CS3102");

        strcpy(prerequisites[4].course_code, "CS5101");
        strcpy(prerequisites[4].prerequisite_code, "CS3105");

        strcpy(prerequisites[5].course_code, "CS5105");
        strcpy(prerequisites[5].prerequisite_code, "CS4104");

        strcpy(prerequisites[6].course_code, "IT3102");
        strcpy(prerequisites[6].prerequisite_code, "IT2103");

        strcpy(prerequisites[7].course_code, "IT2104");
        strcpy(prerequisites[7].prerequisite_code, "IT1105");

        strcpy(prerequisites[8].course_code, "IT4104");
        strcpy(prerequisites[8].prerequisite_code, "IT3102");

        strcpy(prerequisites[9].course_code, "IT5105");
        strcpy(prerequisites[9].prerequisite_code, "IT4104");

        strcpy(prerequisites[10].course_code, "ME2103");
        strcpy(prerequisites[10].prerequisite_code, "ME1105");

        strcpy(prerequisites[11].course_code, "ME3102");
        strcpy(prerequisites[11].prerequisite_code, "ME2104");

        strcpy(prerequisites[12].course_code, "ME5102");
        strcpy(prerequisites[12].prerequisite_code, "ME4103");

        strcpy(prerequisites[13].course_code, "EC2104");
        strcpy(prerequisites[13].prerequisite_code, "EC1104");

        strcpy(prerequisites[14].course_code, "EC3104");
        strcpy(prerequisites[14].prerequisite_code, "EC2104");

        strcpy(prerequisites[15].course_code, "EC4102");
        strcpy(prerequisites[15].prerequisite_code, "EC3103");

        strcpy(prerequisites[16].course_code, "EE2103");
        strcpy(prerequisites[16].prerequisite_code, "EE1106");

        strcpy(prerequisites[17].course_code, "EE3102");
        strcpy(prerequisites[17].prerequisite_code, "EE2106");

        strcpy(prerequisites[18].course_code, "EE4103");
        strcpy(prerequisites[18].prerequisite_code, "EE3106");

        for (int i=0; i<last_sem; i++)
        {
            printf("\nEnter Course code for completed course %d : ",i+1);
            fgets(c_code, sizeof(c_code), stdin);
            c_code[strcspn(c_code, "\n")] = '\0'; // Remove newline character
            add_completed_course(c_code, Student_Id);
        }
        core_registration(no_core_courses,Student_Id);
        elective_registration(no_elec_courses,Student_Id);  
        printf("\nWish you best of luck for course allotment ! :)\n\n"); 
    
        fseek(userfile, 0, SEEK_SET);
        while (fread(&reg, sizeof(StudentDetails), 1, userfile) == 1)
        {
            if (reg.student_id == *Student_Id)
            {
                reg.no_selected_courses += no_courses;
                position = ftell(userfile) - sizeof(StudentDetails);  // Get the position to rewrite the course
                fseek(userfile, position, SEEK_SET);
                fwrite(&reg, sizeof(StudentDetails), 1, userfile);  // R
                break;
            }
        }
    }
    else if (strcmp(status, "YES")==0)
    {
        printf("\nCourses has already been alloted to you! :) \n");
    }
    else if (strcmp(waitlist, "YES")==0)
    {
        printf("\nCourse is in waitlist. Better wait for the result! \n");
    }
    else
        printf("\nAlready reached the maximum limit for course registration. \n");
    fclose(userfile);
} 


void add_courses(int *Student_Id) {
    int num_courses = 1;
    StudentDetails reg;
    long position;
    FILE *userfile = fopen(filestudent, "r+b");

    if (userfile == NULL) {
        perror("Error opening file");
        return;
    }

    int no_already = 0;
    char status[4], waitlist[4]; // Adjusted sizes to fit "YES" or "NO" with null terminator
    int found = 0;
    while (fread(&reg, sizeof(StudentDetails), 1, userfile) == 1) {
        if (reg.student_id == *Student_Id) {
            no_already = reg.no_selected_courses;
            strcpy(status, reg.allotment_status);
            strcpy(waitlist, reg.waitlist);
            found = 1;
            break;  // Exit the loop once the student is found
        }
    }

    if (!found) {
        printf("Student ID not found.\n");
        fclose(userfile);
        return;
    }

    if ((no_already + num_courses) <= 6 && (strcmp(status, "YES") != 0) && (strcmp(waitlist, "YES") != 0)) {
        int course_type;
        printf("Enter the course Type (1 for Core or 2 for Elective): ");
        scanf("%d", &course_type);
        getchar();
        int eligible = Check_Eligibility();
        if (eligible == 1) {
            if (course_type == 1)
                core_registration(num_courses, Student_Id);
            else
                elective_registration(num_courses, Student_Id);
        }

        rewind(userfile);  // Reset file pointer to the beginning
        while (fread(&reg, sizeof(StudentDetails), 1, userfile) == 1) {
            if (reg.student_id == *Student_Id) {
                reg.no_selected_courses += 1;
                position = ftell(userfile) - sizeof(StudentDetails);  // Get the position to rewrite the course
                fseek(userfile, position, SEEK_SET);
                fwrite(&reg, sizeof(StudentDetails), 1, userfile);
                break;
            }
        }
        fclose(userfile);
    } else if (strcmp(status, "YES") == 0) {
        printf("\nCourses have already been allotted to you! :)\n");
        fclose(userfile);
    } else if (strcmp(waitlist, "YES") == 0) {
        printf("\nCourse is in waitlist. Better wait for the result!\n");
        fclose(userfile);
    } else {
        printf("\nAlready reached the maximum limit for course registration.\n");
        fclose(userfile);
    }
}


int core_registration(int num_core_courses, int *Student_Id) 
{
    // Input course details
    
    FILE *selected_core = fopen("core_selected.bin", "ab+");
    if (selected_core == NULL) {
        printf("Error opening file.\n");
        return 1;
    }
    int record_count = fread(core_courses, sizeof(struct core_selectedcourses), MAX_CORE_COURSES, selected_core);
    for (int i = 0; i < num_core_courses; i++)
    {
        printf("Enter details for Core course %d:\n", i + 1);
        printf("Core Course code: ");
        fgets(core_courses[i].course_code, sizeof(core_courses[i].course_code), stdin);
        core_courses[i].course_code[strcspn(core_courses[i].course_code, "\n")] = '\0'; // Remove newline character

        if (!check_prerequisites(Student_Id, core_courses[i].course_code)) {
            printf("Cannot register for %s due to unmet prerequisites.\n", core_courses[i].course_code);
            printf("\nChoose another course---\n");
            i--;
            continue;
        }

        printf("Core Course name: ");
        fgets(core_courses[i].course_name, sizeof(core_courses[i].course_name), stdin);
        core_courses[i].course_name[strcspn(core_courses[i].course_name, "\n")] = '\0'; // Remove newline character
        //printf("Student ID: ");
        core_courses[i].student_id = *Student_Id;
        //getchar(); // Consume newline character
        printf("Faculty name: ");
        fgets(core_courses[i].faculty_name, sizeof(core_courses[i].faculty_name), stdin);
        core_courses[i].faculty_name[strcspn(core_courses[i].faculty_name, "\n")] = '\0'; // Remove newline character

        printf("Course details of %s written to file successfully.\n", core_courses[i].course_code);

    }

    // Write course details to file
    

    fwrite(&core_courses, sizeof(struct core_selectedcourses), num_core_courses, selected_core);
    fclose(selected_core);
    
    return 0;
}

int elective_registration(int num_elec_courses, int *Student_Id) {
    // Input course details
    FILE *selected_elective = fopen("elective_selected.bin", "ab+");
    if (selected_elective == NULL) {
        printf("Error opening file.\n");
        return 1;
    }
    int record_count = fread(elec_courses, sizeof(struct elective_selectedcourses), MAX_ELEC_COURSES, selected_elective);

    for (int i = 0; i < num_elec_courses; i++) {
        printf("\nEnter details for Elective course %d:\n", i+1);
        printf("Elective Course code: ");
        fgets(elec_courses[i].course_code, sizeof(elec_courses[i].course_code), stdin);
        elec_courses[i].course_code[strcspn(elec_courses[i].course_code, "\n")] = '\0'; // Remove newline character
        printf("Elective Course name: ");
        fgets(elec_courses[i].course_name, sizeof(elec_courses[i].course_name), stdin);
        elec_courses[i].course_name[strcspn(elec_courses[i].course_name, "\n")] = '\0'; // Remove newline character
        //printf("Student ID: ");
        elec_courses[i].student_id = *Student_Id;
        //getchar(); // Consume newline character
        printf("Faculty name: ");
        fgets(elec_courses[i].faculty_name, sizeof(elec_courses[i].faculty_name), stdin);
        elec_courses[i].faculty_name[strcspn(elec_courses[i].faculty_name, "\n")] = '\0'; // Remove newline character
    }

    // Write course details to file

    fwrite(&elec_courses, sizeof(struct core_selectedcourses), num_elec_courses, selected_elective);
    fclose(selected_elective);
    printf("Course details written to file successfully.\n");
    return 0;
}

int backlog_courses(int num_courses, int *Student_Id)
{
    struct backlog_courses backlogs[MAX_BACKLOG_COURSES];
    // Input course details
    for (int i = 0; i < num_courses; i++)
    {
        printf("Enter details for Core course %d:\n", i+1);
        printf("Course code: ");
        fgets(backlogs[i].course_code, sizeof(backlogs[i].course_code), stdin);
        backlogs[i].course_code[strcspn(backlogs[i].course_code, "\n")] = '\0'; // Remove newline character
        printf("Course name: ");
        fgets(backlogs[i].course_name, sizeof(backlogs[i].course_name), stdin);
        backlogs[i].course_name[strcspn(backlogs[i].course_name, "\n")] = '\0'; // Remove newline character
        //printf("Student ID: ");
        backlogs[i].student_id = *Student_Id;
        //getchar(); // Consume newline character
        printf("Faculty name: ");
        fgets(backlogs[i].faculty_name, sizeof(backlogs[i].faculty_name), stdin);
        backlogs[i].faculty_name[strcspn(backlogs[i].faculty_name, "\n")] = '\0'; // Remove newline character
        printf("Course type: ");
        fgets(backlogs[i].course_type, sizeof(backlogs[i].course_type), stdin);
        backlogs[i].course_type[strcspn(backlogs[i].course_type, "\n")] = '\0'; // Remove newline character
    }

    // Write course details to file
    FILE *backlog_course = fopen("backlog_courses.bin", "ab");
    if (backlog_course == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }
    fwrite(&backlogs, sizeof(struct backlog_courses), num_courses, backlog_course);

    fclose(backlog_course);
    printf("Course details written to file successfully.\n");
    return 0;
}

int days_gap_calculation(int start_date, int start_month, int start_year, int current_date,int  current_month, int current_year)
{
    int year_diff, days_diff, month_diff, days_calc;
    year_diff = current_year - start_year;
    if (year_diff==0){
        month_diff = current_month - start_month;
        condition: if (month_diff<=1)
        {
            if (month_diff==0){
                days_calc = current_date - start_date;
                return days_calc;
            }
            else{
                if (start_month==1 || start_month==3 || start_month==5 || start_month==7 || start_month==8 || start_month==10 || start_month==12){
                    start_date = 31 - start_date;
                    days_calc = start_date + current_date; 
                    return days_calc; 
                }
                else if (start_month==2){
                        if (start_year%4==0){
                            if (start_year%100==0 && start_year%400==0){
                                start_date = 29 - start_date;
                                days_calc = start_date + current_date; 
                                return days_calc;
                            }
                            else{
                                start_date = 28 - start_date;
                                days_calc = start_date + current_date;
                                return days_calc; 
                            }
                        }
                        else{
                            start_date = 31 - start_date;
                            days_calc = start_date + current_date;
                            return days_calc;
                        } 
                }
                else {
                    start_date = 30 - start_date;
                    days_calc = start_date + current_date; 
                    return days_calc;
                }
            }
        }
        else 
            return 20;
    }
    
    else if (year_diff==1){
        month_diff = 12 - start_month + current_month;
        goto condition;
    }
    else
        return 20;
}

int Check_Eligibility(void)
{
    int start_date, start_month, start_year, current_date, current_month, current_year;
    printf("Enter the starting month, date of semester (MM DD YYYY): ");
    scanf("%d %d %d", &start_date, &start_month, &start_year);
    getchar();
    printf("Enter the current month, date of registration i.e today's (MM DD YYYY): ");
    scanf("%d %d %d", &current_date, &current_month, &current_year);
    getchar();
    int days_calc = days_gap_calculation(start_date, start_month, start_year, current_date, current_month, current_year );
    if (days_calc<=14)
        return 1;
    else
        return 0;
}

void Drop_Course(int *Student_Id)
{

    StudentDetails reg;
    long position;
    FILE *userfile = fopen(filestudent, "r+b");

    if (userfile == NULL) 
    {
        perror("Error opening file");
        return;
    }

    int no_already = 0;
    char status[4], waitlist[4]; // Adjusted sizes to fit "YES" or "NO" with null terminator
    int found = 0;
    while (fread(&reg, sizeof(StudentDetails), 1, userfile) == 1) 
    {
        if (reg.student_id == *Student_Id) {
            no_already = reg.no_selected_courses;
            strcpy(status, reg.allotment_status);
            strcpy(waitlist, reg.waitlist);
            found = 1;
            break;  // Exit the loop once the student is found
        }
    }

    if (!found) 
    {
        printf("Student ID not found.\n");
        fclose(userfile);
        return;
    }
 
    
        int num_courses;
        printf("Enter Number of Courses to Drop (Minimum number of courses should be 4): ");
        scanf("%d", &num_courses);

        int eligible = Check_Eligibility();
        if (eligible==1 && (no_already - num_courses) >= 4)
        {
            int course_type;
            printf("Enter the course Type (1 for Core and 2 for Elective: ");
            scanf("%d", &course_type);
            getchar();
            if (course_type == 1 )
            {
                char c_code[10], faculty[50];
                int std_id;
                printf("\nEnter the Course Code which want to drop: ");
                fgets(c_code, sizeof(c_code), stdin);
                c_code[strcspn(c_code, "\n")] = '\0';
                printf("\nEnter the Faculty name which want to drop: ");
                fgets(faculty, sizeof(faculty), stdin);
                faculty[strcspn(faculty, "\n")] = '\0';  
                //printf("Enter the student id: ");
                std_id = *Student_Id;
                FILE *selected_core_read = fopen("core_selected.bin", "rb");
                
                int record_count = fread(core_courses, sizeof(struct core_selectedcourses), MAX_CORE_COURSES , selected_core_read);
                
                
                int found = 0;

                FILE *fp_core = fopen(file_core, "r+b");
                    if (fp_core == NULL) {
                        perror("Error opening core courses file for reading");
                        exit(EXIT_FAILURE);
                    }


                for (int i = 0; i < record_count; i++) 
                {
                    if (strcmp(core_courses[i].course_code, c_code) == 0 && core_courses[i].student_id == std_id ) {
                        found = 1;
                        for (int j = i; j < record_count-1; j++) {
                            core_courses[j] = core_courses[j+1];
                        }
                        (record_count)--;
                        printf("Course deleted successfully.\n");

                        Course course;
                        while (fread(&course, sizeof(Course), 1, fp_core) == 1) 
                        {
                            if ( strcmp(course.faculty, faculty) == 0 && strcmp(course.course_code, c_code) == 0) 
                            {
                                course.available_seats += 1;
                                position = ftell(fp_core) - sizeof(Course);
                                fseek(fp_core, position, SEEK_SET);
                                fwrite(&course, sizeof(Course), 1, fp_core);
                                found = 1;
                                printf("\n");
                                break; 
                            }
                        }
                        rewind(userfile);  // Reset file pointer to the beginning
                        while (fread(&reg, sizeof(StudentDetails), 1, userfile) == 1)
                        {
                            if (reg.student_id == *Student_Id) 
                            {
                                reg.no_selected_courses -= 1;
                                position = ftell(userfile) - sizeof(StudentDetails);  // Get the position to rewrite the course
                                fseek(userfile, position, SEEK_SET);
                                fwrite(&reg, sizeof(StudentDetails), 1, userfile);
                                break;
                            }
                        }

                        break;
                    }
                }
                fclose(selected_core_read);
                FILE *selected_core = fopen("core_selected.bin", "wb");
                fwrite(core_courses, sizeof(struct core_selectedcourses ), record_count, selected_core);
                fclose(selected_core);
                fclose(fp_core);

                FILE *final_fp = fopen(file_final, "rb");
                final_details final[100];
                int record_count1 = fread(final, sizeof(final_details), 100 , final_fp);
                for (int i = 0; i < record_count1; i++) 
                {
                    if (strcmp(final[i].course_code, c_code) == 0 && final[i].student_id == std_id && strcmp(final[i].faculty, faculty)==0 ) {
                        found = 1;
                        for (int j = i; j < record_count1-1; j++) {
                            final[j] = final[j+1];
                        }
                        (record_count1)--;
                        printf("Allotment deleted successfully.\n");

                        break;
                    }
                }
                fclose(final_fp);
                FILE *fp_final = fopen(file_final, "wb");
                fwrite(final, sizeof(final_details), record_count1, fp_final);
                fclose(fp_final);

            }
            else 
            {
            
                char c_code[10], std_id, faculty[50];
                printf("\nEnter the Course Code which want to drop: ");
                fgets(c_code, sizeof(c_code), stdin);
                c_code[strcspn(c_code, "\n")] = '\0';
                printf("\nEnter the Faculty name which want to drop: ");
                fgets(faculty, sizeof(faculty), stdin);
                faculty[strcspn(faculty, "\n")] = '\0'; 

                //printf("Enter the student id: ");
                std_id = *Student_Id;
                FILE *selected_elective_read = fopen("elective_selected.bin", "rb");
                int record_count = fread(elec_courses, sizeof(struct elective_selectedcourses), MAX_CORE_COURSES , selected_elective_read);
                int found = 0;

                    FILE *fp_elec = fopen(file_elec, "r+b");
                    if (fp_elec == NULL) {
                        perror("Error opening elective courses file for reading");
                        exit(EXIT_FAILURE);
                    }

                for (int i = 0; i < record_count; i++) 
                {
                    if (strcmp(elec_courses[i].course_code, c_code) == 0 && elec_courses[i].student_id == std_id ) {
                        found = 1;
                        for (int j = i; j < record_count-1; j++) {
                            elec_courses[j] = elec_courses[j+1];
                        }
                        (record_count)--;
                        printf("Course deleted successfully.\n");

                        Course course;
                        while (fread(&course, sizeof(Course), 1, fp_elec) == 1) 
                        {
                            if ( strcmp(course.faculty, faculty) == 0 && strcmp(course.course_code, c_code) == 0) 
                            {
                                course.available_seats += 1;
                                position = ftell(fp_elec) - sizeof(Course);
                                fseek(fp_elec, position, SEEK_SET);
                                fwrite(&course, sizeof(Course), 1, fp_elec);
                                found = 1;
                                printf("\n");
                                break; 
                            }
                        }

                        rewind(userfile);  // Reset file pointer to the beginning
                        while (fread(&reg, sizeof(StudentDetails), 1, userfile) == 1)
                        {
                            if (reg.student_id == *Student_Id) 
                            {
                                reg.no_selected_courses -= 1;
                                position = ftell(userfile) - sizeof(StudentDetails);  // Get the position to rewrite the course
                                fseek(userfile, position, SEEK_SET);
                                fwrite(&reg, sizeof(StudentDetails), 1, userfile);
                                break;
                            }
                        }
                    }
                }
                fclose(selected_elective_read);
                FILE *selected_elective = fopen("elective_selected.bin", "wb");
                fwrite(elec_courses, sizeof(struct elective_selectedcourses ), record_count, selected_elective);
                fclose(selected_elective);
                fwrite(elec_courses, sizeof(struct elective_selectedcourses ), record_count, selected_elective);
                fclose(selected_elective);
                fclose(fp_elec);
                FILE *final_fp = fopen(file_final, "rb");
                final_details final[100];
                int record_count1 = fread(final, sizeof(final_details), 100 , final_fp);
                for (int i = 0; i < record_count1; i++) 
                {
                    if (strcmp(final[i].course_code, c_code) == 0 && final[i].student_id == std_id && strcmp(final[i].faculty, faculty)==0 ) {
                        found = 1;
                        for (int j = i; j < record_count1-1; j++) {
                            final[j] = final[j+1];
                        }
                        (record_count1)--;
                        printf("Allotment deleted successfully.\n");

                        break;
                    }
                }
                fclose(final_fp);
                FILE *fp_final = fopen(file_final, "wb");
                fwrite(final, sizeof(final_details), record_count1, fp_final);
                fclose(fp_final);
            }
        }
        else 
        {
            printf("\nSorry, You can not drop the course..\n\n");
        }
        fclose(userfile);
    
        fclose(userfile);
    }


void Display_SelectedCourses(int *Student_Id)
{
    FILE *selected_core = fopen("core_selected.bin", "rb");
    FILE *selected_elective = fopen("elective_selected.bin", "rb");
    int record_countcore = fread(&core_courses_read, sizeof(struct core_selectedcourses), MAX_CORE_COURSES, selected_core);
    printf("Selected Core Course details are: \n");
    int j=1;
    for (int i=0; i<record_countcore; i++)
    {
        if (core_courses_read[i].student_id == *Student_Id)
        {
            printf("\n============= Core %d ============> \n",j++);
            printf("\tCourse Code: %s \n", core_courses_read[i].course_code);
            printf("\tCourse Name: %s \n", core_courses_read[i].course_name);
            printf("\tStudent ID: %d \n", core_courses_read[i].student_id);
            printf("\tFaculty Name: %s\n", core_courses_read[i].faculty_name);
        }
    }
    
    int record_countelec = fread(&elec_courses_read, sizeof(struct elective_selectedcourses), MAX_ELEC_COURSES, selected_elective);
    printf("\nSelected Elective Course details are \n");
    j=1;
    for (int i=0; i<record_countelec; i++)
    {
        if (elec_courses_read[i].student_id == *Student_Id)
        {
            printf("\n============= Elective %d ============> \n",j++);
            printf("\tCourse Code: %s \n", elec_courses_read[i].course_code);
            printf("\tCourse Name: %s \n", elec_courses_read[i].course_name);
            printf("\tStudent ID: %d \n", elec_courses_read[i].student_id);
            printf("\tFaculty Name: %s \n", elec_courses_read[i].faculty_name);
        }

    }
    fclose(selected_core);
    fclose(selected_elective);
}

void writeCoursesToFile(void) {
    FILE *fp_core = fopen(file_core, "wb");
    if (fp_core == NULL) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }

    FILE *fp_elec= fopen(file_elec, "wb");
    if (fp_elec == NULL) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }

    // Sample Core Courses records
    Course core_courses[]= {1,"CSE","CS1101",30,0,"Mahipal Sharma",1,4,"Mathematics I",26,
1,"CSE","CS1101",30,0,"Rohit Sharma",2,4,"Mathematics I",7,
1,"CSE","CS1101",30,0,"Abishek Bannerjee",3,4,"Mathematics I",21,
1,"CSE","CS1102",30,0,"Nisha Desai",1,3,"Physics I",16,
1,"CSE","CS1102",30,0,"Nidhi Singh",2,3,"Physics I",7,
1,"CSE","CS1102",30,0,"Aarav Patel",3,3,"Physics I",28,
1,"CSE","CS1103",30,0,"Arjun Kumar",1,3,"Chemistry",17,
1,"CSE","CS1103",30,0,"Kavya Menon",2,3,"Chemistry",14,
1,"CSE","CS1103",30,0,"Akshay Nair",3,3,"Chemistry",5,
1,"CSE","CS1104",30,0,"Aishwarya Rao",1,3.5,"Introduction to Computing",20,
1,"CSE","CS1104",30,0,"Sanjay Pillai",2,3.5,"Introduction to Computing",21,
1,"CSE","CS1104",30,0,"Ananya Krishnan",3.5,4,"Introduction to Computing",10,
1,"CSE","CS1105",30,0,"Rahul Reddy",1,3,"Engineering Graphics",10,
1,"CSE","CS1105",30,0,"Meera Iyer",2,3,"Engineering Graphics",14,
1,"CSE","CS1105",30,0,"Vikram Chandrasekhar",3,3,"Engineering Graphics",29,
1,"CSE","CS1106",30,0,"Nandini Menon",1,4,"Basic Electrical Engineering",30,
1,"CSE","CS1106",30,0,"Surya Srinivasan",2,4,"Basic Electrical Engineering",29,
1,"CSE","CS1106",30,0,"Sneha Prasad",3,4,"Basic Electrical Engineering",6,
2,"CSE","CS2101",30,0,"Mahipal Sharma",1,4,"Mathematics II",6,
2,"CSE","CS2101",30,0,"Rohit Sharma",2,4,"Mathematics II",13,
2,"CSE","CS2101",30,0,"Abishek Bannerjee",3,4,"Mathematics II",28,
2,"CSE","CS2102",30,0,"Priya Ranganathan",1,3,"Physics II",0,
2,"CSE","CS2102",30,0,"Vivek Balakrishnan",2,3,"Physics II",14,
2,"CSE","CS2102",30,0,"Malati Subramaniyan",3,3,"Physics II",4,
2,"CSE","CS2103",30,0,"Lakshmi Venkatesh",1,4,"Engineering Mechanics",25,
2,"CSE","CS2103",30,0,"Ravi Kumar",2,4,"Engineering Mechanics",0,
2,"CSE","CS2103",30,0,"Swathi Raman",3,4,"Engineering Mechanics",7,
2,"CSE","CS2104",30,0,"Vinay Patel",1,3,"Thermodynamics",16,
2,"CSE","CS2104",30,0,"Shruti Natarajan",2,3,"Thermodynamics",0,
2,"CSE","CS2104",30,0,"Rajesh Menon",3,3,"Thermodynamics",21,
2,"CSE","CS2105",30,0,"Anusha Suresh",1,3,"Basic Electronics Engineering",5,
2,"CSE","CS2105",30,0,"Manoj Krishnamurthy",2,3,"Basic Electronics Engineering",15,
2,"CSE","CS2105",30,0,"Vandana Sharma",3,3,"Basic Electronics Engineering",11,
2,"CSE","CS2106",30,0,"Harish Raghavan",1,4,"Introduction to Data science",10,
2,"CSE","CS2106",30,0,"Deepa Nair",2,4,"Introduction to Data science",9,
2,"CSE","CS2106",30,0,"Siddharth Rao",3,4,"Introduction to Data science",9,
3,"CSE","CS3101",30,0,"Mahipal Sharma",1,4,"Mathematics III",30,
3,"CSE","CS3101",30,0,"Rohit Sharma",2,4,"Mathematics III",10,
3,"CSE","CS3101",30,0,"Abishek Bannerjee",3,4,"Mathematics III",9,
3,"CSE","CS3102",30,0,"Ganesh Iyer",1,4,"Object-Oriented Programming with Java",0,
3,"CSE","CS3102",30,0,"Lavanya Venugopal",2,4,"Object-Oriented Programming with Java",10,
3,"CSE","CS3102",30,0,"Sathish Menon",3,4,"Object-Oriented Programming with Java",24,
3,"CSE","CS3103",30,0,"Pooja Naidu",1,4,"Computer Organization and Architecture",13,
3,"CSE","CS3103",30,0,"Arvind Chandran",2,4,"Computer Organization and Architecture",2,
3,"CSE","CS3103",30,0,"Anjali Soman",3,4,"Computer Organization and Architecture",24,
3,"CSE","CS3104",30,0,"Rajesh Pillai",1,3,"Database Management Systems",12,
3,"CSE","CS3104",30,0,"Vaishnavi Srinivas",2,3,"Database Management Systems",21,
3,"CSE","CS3104",30,0,"Aditya Singh",3,3,"Database Management Systems",16,
3,"CSE","CS3105",30,0,"Revathi Venkataraman",1,3,"Data Communication and Computer Networks",11,
3,"CSE","CS3105",30,0,"Rakesh Kumar",2,3,"Data Communication and Computer Networks",7,
3,"CSE","CS3105",30,0,"Archana Krishnan",3,3,"Data Communication and Computer Networks",6,
3,"CSE","CS3106",30,0,"Ashwin Iyer",1,3,"Operating Systems",7,
3,"CSE","CS3106",30,0,"Kalyani Nair",2,3,"Operating Systems",6,
3,"CSE","CS3106",30,0,"Karthick Ramachandran",3,3,"Operating Systems",6,
4,"CSE","CS4101",30,0,"Mahipal Sharma",1,4,"Mathematics IV",8,
4,"CSE","CS4101",30,0,"Rohit Sharma",2,4,"Mathematics IV",3,
4,"CSE","CS4101",30,0,"Abishek Bannerjee",3,4,"Mathematics IV",8,
4,"CSE","CS4102",30,0,"Pranav Rajan",1,3,"Analysis and Design of Algorithms",12,
4,"CSE","CS4102",30,0,"Divya Gupta",2,3,"Analysis and Design of Algorithms",21,
4,"CSE","CS4102",30,0,"Karthik Iyer",3,3,"Analysis and Design of Algorithms",7,
4,"CSE","CS4103",30,0,"Kala Krishnan",1,3,"Microprocessors and Microcontrollers",30,
4,"CSE","CS4103",30,0,"Praveen Kumar",2,3,"Microprocessors and Microcontrollers",24,
4,"CSE","CS4103",30,0,"Bhavana Rajagopal",3,3,"Microprocessors and Microcontrollers",5,
4,"CSE","CS4104",30,0,"Ramya Nandakumar",1,4,"Software Engineering",29,
4,"CSE","CS4104",30,0,"Prakash Venkat",2,4,"Software Engineering",5,
4,"CSE","CS4104",30,0,"Gayathri Krishnamoorthy",3,4,"Software Engineering",21,
4,"CSE","CS4105",30,0,"Vignesh Balaji",1,3,"Computer Graphics",0,
4,"CSE","CS4105",30,0,"Keerthi Mohan",2,3,"Computer Graphics",29,
4,"CSE","CS4105",30,0,"Madhu Ramanathan",3,3,"Computer Graphics",6,
4,"CSE","CS4106",30,0,"Sharon Bincy",1,4,"Unix and Shell Programming",9,
4,"CSE","CS4106",30,0,"Shalini Krishnan",2,4,"Unix and Shell Programming",1,
4,"CSE","CS4106",30,0,"Aravind Menon",3,4,"Unix and Shell Programming",7,
5,"CSE","CS5101",30,0,"Ashwin Iyer",1,4,"Computer Networks",27,
5,"CSE","CS5101",30,0,"Kalyani Nair",2,4,"Computer Networks",21,
5,"CSE","CS5101",30,0,"Karthick Ramachandran",3,4,"Computer Networks",25,
5,"CSE","CS5102",30,0,"Sangeetha Subramanian",1,3,"Compiler Design",23,
5,"CSE","CS5102",30,0,"Naveen Kumar",2,3,"Compiler Design",1,
5,"CSE","CS5102",30,0,"Malini Sridharan",3,3,"Compiler Design",8,
5,"CSE","CS5103",30,0,"Manjunath Prasad",1,4,"Artificial Intelligence",28,
5,"CSE","CS5103",30,0,"Sowmya Iyengar",2,4,"Artificial Intelligence",19,
5,"CSE","CS5103",30,0,"Vinod Rajagopalan",3,4,"Artificial Intelligence",9,
5,"CSE","CS5104",30,0,"Jayashree Sundaram",1,3,"Web Technologies",16,
5,"CSE","CS5104",30,0,"Kiran Rao",2,3,"Web Technologies",12,
5,"CSE","CS5104",30,0,"Anuradha Suresh",3,3,"Web Technologies",18,
5,"CSE","CS5105",30,0,"Deepak Venugopalan",1,2.5,"Software Testing and Quality Assurance",20,
5,"CSE","CS5105",30,0,"Indira Iyer",2,2.5,"Software Testing and Quality Assurance",19,
5,"CSE","CS5105",30,0,"Sunil Krishnan",3,2.5,"Software Testing and Quality Assurance",15,
5,"CSE","CS5106",30,0,"Asha Anand",1,3,"Cryptography and Network Security",17,
5,"CSE","CS5106",30,0,"Pradeep Menon",2,3,"Cryptography and Network Security",22,
5,"CSE","CS5106",30,0,"Meenakshi Nair",3,3,"Cryptography and Network Security",4,
6,"CSE","CS6101",30,0,"Shankar Chandrasekar",1,3,"Distributed Systems",5,
6,"CSE","CS6101",30,0,"Hemalatha Ramaswamy",2,3,"Distributed Systems",21,
6,"CSE","CS6101",30,0,"Krishna Iyengar",3,3,"Distributed Systems",5,
6,"CSE","CS6102",30,0,"Ramya Nandakumar",1,4,"Cloud Computing",12,
6,"CSE","CS6102",30,0,"Prakash Venkat",2,4,"Cloud Computing",20,
6,"CSE","CS6102",30,0,"Gayathri Krishnamoorthy",3,4,"Cloud Computing",4,
6,"CSE","CS6103",30,0,"Revathi Venkataraman",1,4,"Machine Learning",18,
6,"CSE","CS6103",30,0,"Rakesh Kumar",2,4,"Machine Learning",18,
6,"CSE","CS6103",30,0,"Archana Krishnan",3,4,"Machine Learning",18,
6,"CSE","CS6104",30,0,"Satish Kumar",1,3,"Big Data Analytics",1,
6,"CSE","CS6104",30,0,"Mamatha Natarajan",2,3,"Big Data Analytics",30,
6,"CSE","CS6104",30,0,"Vijay Sharma",3,3,"Big Data Analytics",9,
6,"CSE","CS6105",30,0,"Saranya Krishnan",1,3.5,"Internet of Things",11,
6,"CSE","CS6105",30,0,"Anil Kumar",2,3.5,"Internet of Things",22,
6,"CSE","CS6105",30,0,"Sujatha Menon",3,3.5,"Internet of Things",26,
6,"CSE","CS6106",30,0,"Manu Nair",1,3,"Mobile Computing",12,
6,"CSE","CS6106",30,0,"Uma Srinivasan",2,3,"Mobile Computing",7,
6,"CSE","CS6106",30,0,"Ramesh Varma",3,3,"Mobile Computing",2,
7,"CSE","CS7101",30,0,"Ananya Krishnan",1,4,"Cyber Security",25,
7,"CSE","CS7101",30,0,"Vivek Balakrishnan",2,4,"Cyber Security",4,
7,"CSE","CS7101",30,0,"Sneha Prasad",3,4,"Cyber Security",27,
7,"CSE","CS7102",30,0,"Karthik Iyer",1,4,"Natural Language Processing",25,
7,"CSE","CS7102",30,0,"Priya Ranganathan",2,4,"Natural Language Processing",21,
7,"CSE","CS7102",30,0,"Surya Srinivasan",3,4,"Natural Language Processing",28,
7,"CSE","CS7103",30,0,"Arjun Kumar",1,3,"Blockchain Technology",21,
7,"CSE","CS7103",30,0,"Kavya Menon",2,3,"Blockchain Technology",10,
7,"CSE","CS7103",30,0,"Sanjay Pillai",3,3,"Blockchain Technology",15,
7,"CSE","CS7104",30,0,"Nandini Menon",1,4,"Open Elective III",16,
7,"CSE","CS7104",30,0,"Vikram Chandrasekhar",2,4,"Open Elective III",3,
7,"CSE","CS7104",30,0,"Meera Iyer",3,4,"Open Elective III",2,
7,"CSE","CS7105",30,0,"Rahul Reddy",1,4.5,"Project Work Phase I",6,
7,"CSE","CS7105",30,0,"Aishwarya Rao",2,4.5,"Project Work Phase I",27,
7,"CSE","CS7105",30,0,"Vinay Patel",3,4.5,"Project Work Phase I",0,
7,"CSE","CS7106",30,0,"Jayalakshmi Iyer",1,4,"Open Source Systems",17,
7,"CSE","CS7106",30,0,"Arun Venkatesh",2,4,"Open Source Systems",29,
7,"CSE","CS7106",30,0,"Sumathi Krishnamurthy",3,4,"Open Source Systems",24,
8,"CSE","CS8101",30,0,"Lakshmi Venkatesh",1,4.5,"Project Work Phase II",3,
8,"CSE","CS8101",30,0,"Ravi Kumar",2,4.5,"Project Work Phase II",19,
8,"CSE","CS8101",30,0,"Swathi Raman",3,4.5,"Project Work Phase II",18,
8,"CSE","CS8102",30,0,"Ganesh Iyer",1,4,"Industrial Training",16,
8,"CSE","CS8102",30,0,"Lavanya Venugopal",2,4,"Industrial Training",12,
8,"CSE","CS8102",30,0,"Sathish Menon",3,4,"Industrial Training",28,
8,"CSE","CS8103",30,0,"Pooja Naidu",1,2,"Entrepreneurship Development Programme",24,
8,"CSE","CS8103",30,0,"Arvind Chandran",2,2,"Entrepreneurship Development Programme",11,
8,"CSE","CS8103",30,0,"Anjali Soman",3,2,"Entrepreneurship Development Programme",0,
8,"CSE","CS8104",30,0,"Rajesh Menon",1,2,"Comprehensive Viva Voce",16,
8,"CSE","CS8104",30,0,"Anusha Suresh",2,2,"Comprehensive Viva Voce",15,
8,"CSE","CS8104",30,0,"Manoj Krishnamurthy",3,2,"Comprehensive Viva Voce",11,
8,"CSE","CS8105",30,0,"Dinesh Iyengar",1,2,"Industrial Management",24,
8,"CSE","CS8105",30,0,"Jyothi Raghavan",2,2,"Industrial Management",3,
8,"CSE","CS8105",30,0,"Mohan Kumar",3,2,"Industrial Management",10,
8,"CSE","CS8106",30,0,"Deepika Menon",1,2,"Open Source Systems",4,
8,"CSE","CS8106",30,0,"Ganapathy Narayanan",2,2,"Open Source Systems",1,
8,"CSE","CS8106",30,0,"Shanthi Subramanian",3,2,"Open Source Systems",15,
1,"IT","IT1101",30,0,"Suresh Kumar",1,4,"Mathematics I",12,
1,"IT","IT1101",30,0,"Deepika Sharma",2,4,"Mathematics I",20,
1,"IT","IT1101",30,0,"Rajesh Singh",3,4,"Mathematics I",15,
1,"IT","IT1102",30,0,"Anita Patel",1,3,"Physics I",8,
1,"IT","IT1102",30,0,"Vivek Gupta",2,3,"Physics I",20,
1,"IT","IT1102",30,0,"Priya Shah",3,3,"Physics I",7,
1,"IT","IT1103",30,0,"Rahul Mehta",1,3,"Chemistry",26,
1,"IT","IT1103",30,0,"Kavita Singh",2,3,"Chemistry",4,
1,"IT","IT1103",30,0,"Amit Sharma",3,3,"Chemistry",3,
1,"IT","IT1104",30,0,"Asha Gupta",1,3,"Introduction to Computing",6,
1,"IT","IT1104",30,0,"Rajesh Verma",2,3,"Introduction to Computing",9,
1,"IT","IT1104",30,0,"Sunita Dubey",3,3,"Introduction to Computing",10,
1,"IT","IT1105",30,0,"Manoj Kumar",1,3,"Engineering Graphics",12,
1,"IT","IT1105",30,0,"Meera Devi",2,3,"Engineering Graphics",13,
1,"IT","IT1105",30,0,"Ramesh Gupta",3,3,"Engineering Graphics",3,
1,"IT","IT1106",30,0,"Shalini Singh",1,3,"Basic Electrical Engineering",3,
1,"IT","IT1106",30,0,"Sandeep Kumar",2,3,"Basic Electrical Engineering",24,
1,"IT","IT1106",30,0,"Neeta Sharma",3,3,"Basic Electrical Engineering",6,
2,"IT","IT2101",30,0,"Ravi Kumar",1,4,"Mathematics II",27,
2,"IT","IT2101",30,0,"Anjali Gupta",2,4,"Mathematics II",0,
2,"IT","IT2101",30,0,"Vikas Sharma",3,4,"Mathematics II",22,
2,"IT","IT2102",30,0,"Anjali Verma",1,3,"Physics II",20,
2,"IT","IT2102",30,0,"Amit Singh",2,3,"Physics II",11,
2,"IT","IT2102",30,0,"Neelam Patel",3,3,"Physics II",24,
2,"IT","IT2103",30,0,"Deepak Sharma",1,4,"Data Structures",1,
2,"IT","IT2103",30,0,"Anita Verma",2,4,"Data Structures",21,
2,"IT","IT2103",30,0,"Rajesh Kumar",3,4,"Data Structures",29,
2,"IT","IT2104",30,0,"Rajesh Verma",1,3,"Digital Logic Design",17,
2,"IT","IT2104",30,0,"Deepika Singh",2,3,"Digital Logic Design",20,
2,"IT","IT2104",30,0,"Vivek Gupta",3,3,"Digital Logic Design",14,
2,"IT","IT2105",30,0,"Ritu Sharma",1,3,"Basic Electronics Engineering",3,
2,"IT","IT2105",30,0,"Ajay Kumar",2,3,"Basic Electronics Engineering",25,
2,"IT","IT2105",30,0,"Vidya Patel",3,3,"Basic Electronics Engineering",20,
2,"IT","IT2106",30,0,"Vikram Singh",1,1,"Environmental Studies",4,
2,"IT","IT2106",30,0,"Meena Patel",2,1,"Environmental Studies",5,
2,"IT","IT2106",30,0,"Rajesh Verma",3,1,"Environmental Studies",15,
3,"IT","IT3101",30,0,"Sanjay Kumar",1,4,"Mathematics III",11,
3,"IT","IT3101",30,0,"Anita Gupta",2,4,"Mathematics III",20,
3,"IT","IT3101",30,0,"Rajesh Verma",3,4,"Mathematics III",5,
3,"IT","IT3102",30,0,"Anita Sharma",1,4.5,"Object-Oriented Programming with Java",13,
3,"IT","IT3102",30,0,"Ajay Singh",2,4.5,"Object-Oriented Programming with Java",4,
3,"IT","IT3102",30,0,"Vivek Patel",3,4.5,"Object-Oriented Programming with Java",12,
3,"IT","IT3103",30,0,"Deepak Kumar",1,4.5,"Computer Organization and Architecture",1,
3,"IT","IT3103",30,0,"Sneha Singh",2,4.5,"Computer Organization and Architecture",29,
3,"IT","IT3103",30,0,"Amit Patel",3,4.5,"Computer Organization and Architecture",28,
3,"IT","IT3104",30,0,"Ajay Sharma",1,4,"Discrete Mathematics",30,
3,"IT","IT3104",30,0,"Anjali Verma",2,4,"Discrete Mathematics",22,
3,"IT","IT3104",30,0,"Rajesh Kumar",3,4,"Discrete Mathematics",15,
3,"IT","IT3105",30,0,"Neelam Patel",1,3,"Database Management Systems",16,
3,"IT","IT3105",30,0,"Anil Singh",2,3,"Database Management Systems",2,
3,"IT","IT3105",30,0,"Deepika Sharma",3,3,"Database Management Systems",13,
3,"IT","IT3106",30,0,"Vikas Kumar",1,3,"Data Communication and Computer Networks",11,
3,"IT","IT3106",30,0,"Anita Verma",2,3,"Data Communication and Computer Networks",25,
3,"IT","IT3106",30,0,"Anil Singh",3,3,"Data Communication and Computer Networks",8,
4,"IT","IT4101",30,0,"Rajesh Singh",1,4,"Mathematics IV",22,
4,"IT","IT4101",30,0,"Deepika Gupta",2,4,"Mathematics IV",14,
4,"IT","IT4101",30,0,"Suresh Kumar",3,4,"Mathematics IV",2,
4,"IT","IT4102",30,0,"Anjali Sharma",1,3,"Analysis and Design of Algorithms",2,
4,"IT","IT4102",30,0,"Vivek Singh",2,3,"Analysis and Design of Algorithms",24,
4,"IT","IT4102",30,0,"Anil Patel",3,3,"Analysis and Design of Algorithms",5,
4,"IT","IT4103",30,0,"Neelam Patel",1,3,"Microprocessors and Microcontrollers",10,
4,"IT","IT4103",30,0,"Amit Verma",2,3,"Microprocessors and Microcontrollers",26,
4,"IT","IT4103",30,0,"Rajesh Kumar",3,3,"Microprocessors and Microcontrollers",18,
4,"IT","IT4104",30,0,"Anil Kumar",1,3.5,"Software Engineering",18,
4,"IT","IT4104",30,0,"Anita Singh",2,3.5,"Software Engineering",17,
4,"IT","IT4104",30,0,"Deepak Sharma",3,3.5,"Software Engineering",17,
4,"IT","IT4105",30,0,"Rajesh Verma",1,3,"Computer Graphics",19,
4,"IT","IT4105",30,0,"Anjali Gupta",2,3,"Computer Graphics",14,
4,"IT","IT4105",30,0,"Ajay Singh",3,3,"Computer Graphics",22,
4,"IT","IT4106",30,0,"Anita Patel",1,3,"Theory of Computation",14,
4,"IT","IT4106",30,0,"Vivek Kumar",2,3,"Theory of Computation",18,
4,"IT","IT4106",30,0,"Deepika Singh",3,3,"Theory of Computation",4,
5,"IT","IT5101",30,0,"Vivek Sharma",1,3.5,"Computer Networks",18,
5,"IT","IT5101",30,0,"Anjali Singh",2,3.5,"Computer Networks",6,
5,"IT","IT5101",30,0,"Rajesh Patel",3,3.5,"Computer Networks",17,
5,"IT","IT5102",30,0,"Deepika Singh",1,3,"Compiler Design",30,
5,"IT","IT5102",30,0,"Rajesh Kumar",2,3,"Compiler Design",20,
5,"IT","IT5102",30,0,"Anil Patel",3,3,"Compiler Design",12,
5,"IT","IT5103",30,0,"Anjali Gupta",1,4,"Artificial Intelligence",2,
5,"IT","IT5103",30,0,"Anil Kumar",2,4,"Artificial Intelligence",6,
5,"IT","IT5103",30,0,"Anita Singh",3,4,"Artificial Intelligence",5,
5,"IT","IT5104",30,0,"Rajesh Verma",1,3.5,"Web Technologies",27,
5,"IT","IT5104",30,0,"Vivek Singh",2,3.5,"Web Technologies",5,
5,"IT","IT5104",30,0,"Anjali Patel",3,3.5,"Web Technologies",27,
5,"IT","IT5105",30,0,"Suresh Kumar",1,3,"Software Testing and Quality Assurance",5,
5,"IT","IT5105",30,0,"Deepika Gupta",2,3,"Software Testing and Quality Assurance",13,
5,"IT","IT5105",30,0,"Vivek Sharma",3,3,"Software Testing and Quality Assurance",17,
5,"IT","IT5106",30,0,"Anjali Singh",1,2.5,"Cryptography and Network Security",12,
5,"IT","IT5106",30,0,"Anil Patel",2,2.5,"Cryptography and Network Security",1,
5,"IT","IT5106",30,0,"Rajesh Kumar",3,2.5,"Cryptography and Network Security",15,
6,"IT","IT6101",30,0,"Anita Patel",1,3,"Distributed Systems",13,
6,"IT","IT6101",30,0,"Rajesh Kumar",2,3,"Distributed Systems",24,
6,"IT","IT6101",30,0,"Vivek Singh",3,3,"Distributed Systems",5,
6,"IT","IT6102",30,0,"Deepika Singh",1,3,"Cloud Computing",3,
6,"IT","IT6102",30,0,"Anil Patel",2,3,"Cloud Computing",4,
6,"IT","IT6102",30,0,"Anjali Gupta",3,3,"Cloud Computing",1,
6,"IT","IT6103",30,0,"Anil Kumar",1,4,"Machine Learning",28,
6,"IT","IT6103",30,0,"Anita Singh",2,4,"Machine Learning",20,
6,"IT","IT6103",30,0,"Anjali Verma",3,4,"Machine Learning",25,
6,"IT","IT6104",30,0,"Vivek Sharma",1,3,"Big Data Analytics",5,
6,"IT","IT6104",30,0,"Anjali Patel",2,3,"Big Data Analytics",19,
6,"IT","IT6104",30,0,"Rajesh Verma",3,3,"Big Data Analytics",21,
6,"IT","IT6105",30,0,"Rajesh Kumar",1,3.5,"Internet of Things",2,
6,"IT","IT6105",30,0,"Deepika Gupta",2,3.5,"Internet of Things",0,
6,"IT","IT6105",30,0,"Anil Patel",3,3.5,"Internet of Things",11,
6,"IT","IT6106",30,0,"Anjali Singh",1,3,"Mobile Computing",7,
6,"IT","IT6106",30,0,"Rajesh Verma",2,3,"Mobile Computing",13,
7,"IT","IT7101",30,0,"Deepika Singh",1,3,"Cyber Security",27,
7,"IT","IT7101",30,0,"Anil Patel",2,3,"Cyber Security",29,
7,"IT","IT7101",30,0,"Anjali Gupta",3,3,"Cyber Security",29,
7,"IT","IT7102",30,0,"Anita Patel",1,3.5,"Natural Language Processing",20,
7,"IT","IT7102",30,0,"Vivek Sharma",2,3.5,"Natural Language Processing",6,
7,"IT","IT7102",30,0,"Rajesh Kumar",3,3.5,"Natural Language Processing",24,
7,"IT","IT7103",30,0,"Rajesh Verma",1,3,"Blockchain Technology",21,
7,"IT","IT7103",30,0,"Anjali Singh",2,3,"Blockchain Technology",9,
7,"IT","IT7103",30,0,"Anil Patel",3,3,"Blockchain Technology",5,
7,"IT","IT7104",30,0,"Anita Singh",1,1,"Open Elective III",2,
7,"IT","IT7104",30,0,"Vivek Sharma",2,1,"Open Elective III",18,
7,"IT","IT7104",30,0,"Deepika Gupta",3,1,"Open Elective III",11,
7,"IT","IT7105",30,0,"Anjali Patel",1,4.5,"Project Work Phase I",18,
7,"IT","IT7105",30,0,"Anil Kumar",2,4.5,"Project Work Phase I",20,
7,"IT","IT7105",30,0,"Rajesh Verma",3,4.5,"Project Work Phase I",30,
8,"IT","IT8101",30,0,"Vivek Sharma",1,4.5,"Project Work Phase II",24,
8,"IT","IT8101",30,0,"Anjali Gupta",2,4.5,"Project Work Phase II",18,
8,"IT","IT8101",30,0,"Rajesh Kumar",3,4.5,"Project Work Phase II",20,
8,"IT","IT8102",30,0,"Rajesh Verma",1,2,"Industrial Training / In-Plant Training",29,
8,"IT","IT8102",30,0,"Deepika Singh",2,2,"Industrial Training / In-Plant Training",13,
8,"IT","IT8102",30,0,"Anil Patel",3,2,"Industrial Training / In-Plant Training",9,
8,"IT","IT8103",30,0,"Anita Patel",1,2,"Entrepreneurship Development Programme",7,
8,"IT","IT8103",30,0,"Vivek Sharma",2,2,"Entrepreneurship Development Programme",25,
8,"IT","IT8103",30,0,"Anjali Gupta",3,2,"Entrepreneurship Development Programme",21,
8,"IT","IT8104",30,0,"Anil Kumar",1,1,"Comprehensive Viva Voce",12,
8,"IT","IT8104",30,0,"Rajesh Verma",2,1,"Comprehensive Viva Voce",7,
8,"IT","IT8104",30,0,"Deepika Singh",3,1,"Comprehensive Viva Voce",11,
8,"IT","IT8105",30,0,"Raghu Menon",1,2,"Communication Skills",29,
8,"IT","IT8105",30,0,"Janani Nair",2,2,"Communication Skills",10,
8,"IT","IT8105",30,0,"Vijayakumar Venkataraman",3,2,"Communication Skills",0,
8,"IT","IT8105",30,0,"Yamuna Nair",1,2,"Open Source Systems",21,
8,"IT","IT8105",30,0,"Subramanian Iyer",2,2,"Open Source Systems",7,
8,"IT","IT8105",30,0,"Geetha Menon",3,2,"Open Source Systems",18,
1,"ECE","EC1101",30,0,"Krishna Murthy",1,4,"Mathematics I",28,
1,"ECE","EC1101",30,0,"Rajeshwari Naidu",2,4,"Mathematics I",2,
1,"ECE","EC1101",30,0,"Siddharth Rajan",3,4,"Mathematics I",3,
1,"ECE","EC1102",30,0,"Ranjith Kumar",1,3,"Physics I",20,
1,"ECE","EC1102",30,0,"Divya Reddy",2,3,"Physics I",15,
1,"ECE","EC1102",30,0,"Arjun Menon",3,3,"Physics I",11,
1,"ECE","EC1103",30,0,"Shalini Raghavan",1,3,"Chemistry",8,
1,"ECE","EC1103",30,0,"Vivek Nair",2,3,"Chemistry",19,
1,"ECE","EC1103",30,0,"Meenakshi Rao",3,3,"Chemistry",20,
1,"ECE","EC1104",30,0,"Deepa Balan",1,4,"Introduction to Electronics",28,
1,"ECE","EC1104",30,0,"Ajay Menon",2,4,"Introduction to Electronics",5,
1,"ECE","EC1104",30,0,"Nandini Iyer",3,4,"Introduction to Electronics",20,
1,"ECE","EC1105",30,0,"Rajesh Kumar",1,3,"Engineering Graphics",4,
1,"ECE","EC1105",30,0,"Anusha Srinivasan",2,3,"Engineering Graphics",13,
1,"ECE","EC1105",30,0,"Suresh Nair",3,3,"Engineering Graphics",5,
1,"ECE","EC1106",30,0,"Meera Menon",1,3,"Basic Electrical Engineering",3,
1,"ECE","EC1106",30,0,"Siddharth Raghavan",2,3,"Basic Electrical Engineering",17,
1,"ECE","EC1106",30,0,"Akshay Reddy",3,3,"Basic Electrical Engineering",6,
2,"ECE","EC2101",30,0,"Divya Menon",1,4,"Mathematics II",16,
2,"ECE","EC2101",30,0,"Rajesh Iyer",2,4,"Mathematics II",5,
2,"ECE","EC2101",30,0,"Anjali Raghavan",3,4,"Mathematics II",23,
2,"ECE","EC2102",30,0,"Arjun Nair",1,3,"Physics II",0,
2,"ECE","EC2102",30,0,"Sneha Reddy",2,3,"Physics II",18,
2,"ECE","EC2102",30,0,"Vijay Kumar",3,3,"Physics II",26,
2,"ECE","EC2103",30,0,"Vivek Menon",1,3,"Network Analysis and Synthesis",27,
2,"ECE","EC2103",30,0,"Deepika Rao",2,3,"Network Analysis and Synthesis",1,
2,"ECE","EC2103",30,0,"Rahul Krishnan",3,3,"Network Analysis and Synthesis",11,
2,"ECE","EC2104",30,0,"Anusha Naidu",1,3.5,"Electronic Devices and Circuits",22,
2,"ECE","EC2104",30,0,"Ajay Menon",2,3.5,"Electronic Devices and Circuits",17,
2,"ECE","EC2104",30,0,"Meenakshi Rajan",3,3.5,"Electronic Devices and Circuits",21,
2,"ECE","EC2105",30,0,"Siddharth Iyer",1,3,"Basic Civil and Mechanical Engineering",26,
2,"ECE","EC2105",30,0,"Shalini Nair",2,3,"Basic Civil and Mechanical Engineering",2,
2,"ECE","EC2105",30,0,"Krishna Menon",3,3,"Basic Civil and Mechanical Engineering",17,
2,"ECE","EC2106",30,0,"Rajesh Menon",1,1,"Environmental Studies",18,
2,"ECE","EC2106",30,0,"Divya Kumar",2,1,"Environmental Studies",28,
2,"ECE","EC2106",30,0,"Arjun Reddy",3,1,"Environmental Studies",28,
3,"ECE","EC3101",30,0,"Divya Menon",1,4,"Mathematics III",0,
3,"ECE","EC3101",30,0,"Rajesh Iyer",2,4,"Mathematics III",4,
3,"ECE","EC3101",30,0,"Anjali Raghavan",3,4,"Mathematics III",26,
3,"ECE","EC3102",30,0,"Arjun Nair",1,3,"Signals and Systems",23,
3,"ECE","EC3102",30,0,"Sneha Reddy",2,3,"Signals and Systems",4,
3,"ECE","EC3102",30,0,"Vijay Kumar",3,3,"Signals and Systems",30,
3,"ECE","EC3103",30,0,"Vivek Menon",1,3,"Analog Circuits",24,
3,"ECE","EC3103",30,0,"Deepika Rao",2,3,"Analog Circuits",23,
3,"ECE","EC3103",30,0,"Rahul Krishnan",3,3,"Analog Circuits",11,
3,"ECE","EC3104",30,0,"Anusha Naidu",1,3,"Digital Electronics",22,
3,"ECE","EC3104",30,0,"Ajay Menon",2,3,"Digital Electronics",22,
3,"ECE","EC3104",30,0,"Meenakshi Rajan",3,3,"Digital Electronics",22,
3,"ECE","EC3105",30,0,"Siddharth Iyer",1,3,"Electromagnetic Fields",1,
3,"ECE","EC3105",30,0,"Shalini Nair",2,3,"Electromagnetic Fields",20,
3,"ECE","EC3105",30,0,"Krishna Menon",3,3,"Electromagnetic Fields",15,
3,"ECE","EC3106",30,0,"Rajesh Menon",1,4,"Semiconductor Devices",23,
3,"ECE","EC3106",30,0,"Divya Kumar",2,4,"Semiconductor Devices",21,
3,"ECE","EC3106",30,0,"Arjun Reddy",3,4,"Semiconductor Devices",28,
4,"ECE","EC4101",30,0,"Krishna Murthy",1,4,"Mathematics IV",30,
4,"ECE","EC4101",30,0,"Rajeshwari Naidu",2,4,"Mathematics IV",1,
4,"ECE","EC4101",30,0,"Siddharth Rajan",3,4,"Mathematics IV",9,
4,"ECE","EC4102",30,0,"Ranjith Kumar",1,3,"Linear Integrated Circuits",15,
4,"ECE","EC4102",30,0,"Divya Reddy",2,3,"Linear Integrated Circuits",20,
4,"ECE","EC4102",30,0,"Arjun Menon",3,3,"Linear Integrated Circuits",28,
4,"ECE","EC4103",30,0,"Shalini Raghavan",1,3.5,"Communication Theory",15,
4,"ECE","EC4103",30,0,"Vivek Nair",2,3.5,"Communication Theory",24,
4,"ECE","EC4103",30,0,"Meenakshi Rao",3,3.5,"Communication Theory",25,
4,"ECE","EC4104",30,0,"Deepa Balan",1,4,"Microprocessors and Microcontrollers",13,
4,"ECE","EC4104",30,0,"Ajay Menon",2,4,"Microprocessors and Microcontrollers",24,
4,"ECE","EC4104",30,0,"Nandini Iyer",3,4,"Microprocessors and Microcontrollers",19,
4,"ECE","EC4105",30,0,"Rajesh Kumar",1,3.5,"Control Systems",22,
4,"ECE","EC4105",30,0,"Anusha Srinivasan",2,3.5,"Control Systems",23,
4,"ECE","EC4105",30,0,"Suresh Nair",3,3.5,"Control Systems",12,
4,"ECE","EC4106",30,0,"Anjali Nair",1,4,"Digital Signal Processing",18,
4,"ECE","EC4106",30,0,"Vivek Krishnan",2,4,"Digital Signal Processing",18,
4,"ECE","EC4106",30,0,"Deepak Menon",3,4,"Digital Signal Processing",14,
5,"ECE","EC5102",30,0,"Arjun Nair",1,3.5,"VLSI Design",1,
5,"ECE","EC5102",30,0,"Sneha Reddy",2,3.5,"VLSI Design",20,
5,"ECE","EC5102",30,0,"Vijay Kumar",3,3.5,"VLSI Design",21,
5,"ECE","EC5103",30,0,"Vivek Menon",1,3,"Electronic Instrumentation",9,
5,"ECE","EC5103",30,0,"Deepika Rao",2,3,"Electronic Instrumentation",29,
5,"ECE","EC5103",30,0,"Rahul Krishnan",3,3,"Electronic Instrumentation",18,
5,"ECE","EC5104",30,0,"Anusha Naidu",1,3,"Wireless Communication",14,
5,"ECE","EC5104",30,0,"Ajay Menon",2,3,"Wireless Communication",1,
5,"ECE","EC5104",30,0,"Meenakshi Rajan",3,3,"Wireless Communication",10,
5,"ECE","EC5105",30,0,"Siddharth Iyer",1,3,"Optical Communication",10,
5,"ECE","EC5105",30,0,"Shalini Nair",2,3,"Optical Communication",21,
5,"ECE","EC5105",30,0,"Krishna Menon",3,3,"Optical Communication",15,
5,"ECE","EC5106",30,0,"Rajesh Menon",1,4,"Embedded Systems",0,
5,"ECE","EC5106",30,0,"Divya Kumar",2,4,"Embedded Systems",24,
5,"ECE","EC5106",30,0,"Arjun Reddy",3,4,"Embedded Systems",21,
6,"ECE","EC6101",30,0,"Krishna Murthy",1,3.5,"Digital Communication",11,
6,"ECE","EC6101",30,0,"Rajeshwari Naidu",2,3.5,"Digital Communication",2,
6,"ECE","EC6101",30,0,"Siddharth Rajan",3,3.5,"Digital Communication",13,
6,"ECE","EC6102",30,0,"Ranjith Kumar",1,3,"Radar and Navigational Aids",13,
6,"ECE","EC6102",30,0,"Divya Reddy",2,3,"Radar and Navigational Aids",24,
6,"ECE","EC6102",30,0,"Arjun Menon",3,3,"Radar and Navigational Aids",3,
6,"ECE","EC6103",30,0,"Shalini Raghavan",1,3,"Audio and Video Systems",28,
6,"ECE","EC6103",30,0,"Vivek Nair",2,3,"Audio and Video Systems",28,
6,"ECE","EC6103",30,0,"Meenakshi Rao",3,3,"Audio and Video Systems",18,
6,"ECE","EC6104",30,0,"Deepa Balan",1,3,"Satellite Communication",13,
6,"ECE","EC6104",30,0,"Ajay Menon",2,3,"Satellite Communication",24,
6,"ECE","EC6104",30,0,"Nandini Iyer",3,3,"Satellite Communication",7,
6,"ECE","EC6105",30,0,"Rajesh Kumar",1,3.5,"Robotics and Automation",28,
6,"ECE","EC6105",30,0,"Anusha Srinivasan",2,3.5,"Robotics and Automation",19,
6,"ECE","EC6105",30,0,"Suresh Nair",3,3.5,"Robotics and Automation",25,
6,"ECE","EC6106",30,0,"Meera Menon",1,3,"Biomedical Instrumentation",13,
6,"ECE","EC6106",30,0,"Siddharth Raghavan",2,3,"Biomedical Instrumentation",13,
6,"ECE","EC6106",30,0,"Akshay Reddy",3,3,"Biomedical Instrumentation",16,
7,"ECE","EC7101",30,0,"Divya Menon",1,3,"Internet of Things",1,
7,"ECE","EC7101",30,0,"Rajesh Iyer",2,3,"Internet of Things",0,
7,"ECE","EC7101",30,0,"Anjali Raghavan",3,3,"Internet of Things",22,
7,"ECE","EC7102",30,0,"Arjun Nair",1,3,"Artificial Intelligence in Electronics",2,
7,"ECE","EC7102",30,0,"Sneha Reddy",2,3,"Artificial Intelligence in Electronics",22,
7,"ECE","EC7102",30,0,"Vijay Kumar",3,3,"Artificial Intelligence in Electronics",5,
7,"ECE","EC7103",30,0,"Vivek Menon",1,3,"Nanoelectronics",10,
7,"ECE","EC7103",30,0,"Deepika Rao",2,3,"Nanoelectronics",29,
7,"ECE","EC7103",30,0,"Rahul Krishnan",3,3,"Nanoelectronics",19,
7,"ECE","EC7104",30,0,"Anjali Kumar",1,2,"Industrial Management",29,
7,"ECE","EC7104",30,0,"Deepak Menon",2,2,"Industrial Management",15,
7,"ECE","EC7104",30,0,"Rajesh Nair",3,2,"Industrial Management",15,
7,"ECE","EC7105",30,0,"Siddharth Iyer",1,4.5,"Project Work Phase I",16,
7,"ECE","EC7105",30,0,"Shalini Nair",2,4.5,"Project Work Phase I",18,
7,"ECE","EC7105",30,0,"Krishna Menon",3,4.5,"Project Work Phase I",17,
7,"ECE","EC7106",30,0,"Meera Menon",1,3,"Antennas and Wave Propagation",15,
7,"ECE","EC7106",30,0,"Siddharth Raghavan",2,3,"Antennas and Wave Propagation",16,
7,"ECE","EC7106",30,0,"Akshay Reddy",3,3,"Antennas and Wave Propagation",6,
8,"ECE","EC8101",30,0,"Rajesh Menon",1,4.5,"Project Work Phase II",26,
8,"ECE","EC8101",30,0,"Divya Kumar",2,4.5,"Project Work Phase II",4,
8,"ECE","EC8101",30,0,"Arjun Reddy",3,4.5,"Project Work Phase II",13,
8,"ECE","EC8102",30,0,"Anjali Kumar",1,2,"Industrial Training / In-Plant Training",30,
8,"ECE","EC8102",30,0,"Deepak Menon",2,2,"Industrial Training / In-Plant Training",20,
8,"ECE","EC8102",30,0,"Rajesh Nair",3,2,"Industrial Training / In-Plant Training",17,
8,"ECE","EC8103",30,0,"Karthik Naidu",1,2,"Entrepreneurship Development Programme",26,
8,"ECE","EC8103",30,0,"Shweta Kumar",2,2,"Entrepreneurship Development Programme",16,
8,"ECE","EC8103",30,0,"Prakash Menon",3,2,"Entrepreneurship Development Programme",18,
8,"ECE","EC8104",30,0,"Krishna Murthy",1,2,"Comprehensive Viva Voce",23,
8,"ECE","EC8104",30,0,"Rajeshwari Naidu",2,2,"Comprehensive Viva Voce",29,
8,"ECE","EC8104",30,0,"Siddharth Rajan",3,2,"Comprehensive Viva Voce",1,
8,"ECE","EC8105",30,0,"Anjali Kumar",1,2,"Pulse and Digital Circuits",29,
8,"ECE","EC8105",30,0,"Deepak Menon",2,2,"Pulse and Digital Circuits",14,
8,"ECE","EC8105",30,0,"Rajesh Nair",3,2,"Pulse and Digital Circuits",22,
8,"ECE","EC8106",30,0,"Anjali Kumar",1,2,"Technical Communication",19,
8,"ECE","EC8106",30,0,"Deepak Menon",2,2,"Technical Communication",12,
8,"ECE","EC8106",30,0,"Rajesh Nair",3,2,"Technical Communication",24,
1,"EEE","EE1101",30,0,"Krishna Murthy",1,4,"Mathematics I",16,
1,"EEE","EE1101",30,0,"Rajeshwari Naidu",2,4,"Mathematics I",23,
1,"EEE","EE1101",30,0,"Siddharth Rajan",3,4,"Mathematics I",12,
1,"EEE","EE1102",30,0,"Ranjith Kumar",1,3,"Physics I",0,
1,"EEE","EE1102",30,0,"Divya Reddy",2,3,"Physics I",2,
1,"EEE","EE1102",30,0,"Arjun Menon",3,3,"Physics I",5,
1,"EEE","EE1103",30,0,"Shalini Raghavan",1,3,"Chemistry",1,
1,"EEE","EE1103",30,0,"Vivek Nair",2,3,"Chemistry",24,
1,"EEE","EE1103",30,0,"Meenakshi Rao",3,3,"Chemistry",22,
1,"EEE","EE1104",30,0,"Deepa Balan",1,3,"Introduction to Electrical Engineering",4,
1,"EEE","EE1104",30,0,"Ajay Menon",2,3,"Introduction to Electrical Engineering",6,
1,"EEE","EE1104",30,0,"Nandini Iyer",3,3,"Introduction to Electrical Engineering",29,
1,"EEE","EE1105",30,0,"Rajesh Kumar",1,3,"Engineering Graphics",11,
1,"EEE","EE1105",30,0,"Anusha Srinivasan",2,3,"Engineering Graphics",21,
1,"EEE","EE1105",30,0,"Suresh Nair",3,3,"Engineering Graphics",14,
1,"EEE","EE1106",30,0,"Meera Menon",1,3,"Basic Electronics Engineering",1,
1,"EEE","EE1106",30,0,"Siddharth Raghavan",2,3,"Basic Electronics Engineering",23,
1,"EEE","EE1106",30,0,"Akshay Reddy",3,3,"Basic Electronics Engineering",18,
2,"EEE","EE2101",30,0,"Divya Menon",1,4,"Mathematics II",15,
2,"EEE","EE2101",30,0,"Rajesh Iyer",2,4,"Mathematics II",24,
2,"EEE","EE2101",30,0,"Anjali Raghavan",3,4,"Mathematics II",9,
2,"EEE","EE2102",30,0,"Arjun Nair",1,3,"Physics II",0,
2,"EEE","EE2102",30,0,"Sneha Reddy",2,3,"Physics II",1,
2,"EEE","EE2102",30,0,"Vijay Kumar",3,3,"Physics II",18,
2,"EEE","EE2103",30,0,"Vivek Menon",1,4,"Electrical Circuits Analysis",16,
2,"EEE","EE2103",30,0,"Deepika Rao",2,4,"Electrical Circuits Analysis",11,
2,"EEE","EE2103",30,0,"Rahul Krishnan",3,4,"Electrical Circuits Analysis",23,
2,"EEE","EE2104",30,0,"Anusha Naidu",1,3,"Electromagnetic Theory",20,
2,"EEE","EE2104",30,0,"Ajay Menon",2,3,"Electromagnetic Theory",11,
2,"EEE","EE2104",30,0,"Meenakshi Rajan",3,3,"Electromagnetic Theory",21,
2,"EEE","EE2105",30,0,"Siddharth Iyer",1,3,"Basic Civil and Mechanical Engineering",1,
2,"EEE","EE2105",30,0,"Shalini Nair",2,3,"Basic Civil and Mechanical Engineering",2,
2,"EEE","EE2105",30,0,"Krishna Menon",3,3,"Basic Civil and Mechanical Engineering",28,
2,"EEE","EE2106",30,0,"Anjali Kumar",1,3.5,"Electrical Materials",14,
2,"EEE","EE2106",30,0,"Deepak Menon",2,3.5,"Electrical Materials",15,
2,"EEE","EE2106",30,0,"Rajesh Nair",3,3.5,"Electrical Materials",9,
3,"EEE","EE3101",30,0,"Divya Menon",1,4,"Mathematics III",6,
3,"EEE","EE3101",30,0,"Rajesh Iyer",2,4,"Mathematics III",25,
3,"EEE","EE3101",30,0,"Anjali Raghavan",3,4,"Mathematics III",17,
3,"EEE","EE3102",30,0,"Arjun Nair",1,3.5,"Electrical Machines I",7,
3,"EEE","EE3102",30,0,"Sneha Reddy",2,3.5,"Electrical Machines I",10,
3,"EEE","EE3102",30,0,"Vijay Kumar",3,3.5,"Electrical Machines I",18,
3,"EEE","EE3103",30,0,"Vivek Menon",1,3,"Analog Electronic Circuits",23,
3,"EEE","EE3103",30,0,"Deepika Rao",2,3,"Analog Electronic Circuits",20,
3,"EEE","EE3103",30,0,"Rahul Krishnan",3,3,"Analog Electronic Circuits",26,
3,"EEE","EE3104",30,0,"Anusha Naidu",1,3,"Control Systems",6,
3,"EEE","EE3104",30,0,"Ajay Menon",2,3,"Control Systems",12,
3,"EEE","EE3104",30,0,"Meenakshi Rajan",3,3,"Control Systems",27,
3,"EEE","EE3105",30,0,"Siddharth Iyer",1,3,"Network Theory",13,
3,"EEE","EE3105",30,0,"Shalini Nair",2,3,"Network Theory",20,
3,"EEE","EE3105",30,0,"Krishna Menon",3,3,"Network Theory",29,
3,"EEE","EE3106",30,0,"Rajesh Menon",1,3,"Measurement and Instrumentation",22,
3,"EEE","EE3106",30,0,"Divya Kumar",2,3,"Measurement and Instrumentation",13,
3,"EEE","EE3106",30,0,"Arjun Reddy",3,3,"Measurement and Instrumentation",15,
4,"EEE","EE4101",30,0,"Krishna Murthy",1,4,"Mathematics IV",26,
4,"EEE","EE4101",30,0,"Rajeshwari Naidu",2,4,"Mathematics IV",10,
4,"EEE","EE4101",30,0,"Siddharth Rajan",3,4,"Mathematics IV",20,
4,"EEE","EE4102",30,0,"Ranjith Kumar",1,3.5,"Electrical Machines II",30,
4,"EEE","EE4102",30,0,"Divya Reddy",2,3.5,"Electrical Machines II",20,
4,"EEE","EE4102",30,0,"Arjun Menon",3,3.5,"Electrical Machines II",0,
4,"EEE","EE4103",30,0,"Shalini Raghavan",1,3,"Power Electronics",2,
4,"EEE","EE4103",30,0,"Vivek Nair",2,3,"Power Electronics",6,
4,"EEE","EE4103",30,0,"Meenakshi Rao",3,3,"Power Electronics",11,
4,"EEE","EE4104",30,0,"Deepa Balan",1,4,"Digital Electronics",26,
4,"EEE","EE4104",30,0,"Ajay Menon",2,4,"Digital Electronics",23,
4,"EEE","EE4104",30,0,"Nandini Iyer",3,4,"Digital Electronics",2,
4,"EEE","EE4105",30,0,"Rajesh Kumar",1,3,"Power System Analysis",14,
4,"EEE","EE4105",30,0,"Anusha Srinivasan",2,3,"Power System Analysis",30,
4,"EEE","EE4105",30,0,"Suresh Nair",3,3,"Power System Analysis",24,
4,"EEE","EE4106",30,0,"Meera Menon",1,4,"Microprocessors and Microcontrollers",21,
4,"EEE","EE4106",30,0,"Siddharth Raghavan",2,4,"Microprocessors and Microcontrollers",6,
4,"EEE","EE4106",30,0,"Akshay Reddy",3,4,"Microprocessors and Microcontrollers",15,
5,"EEE","EE5101",30,0,"Divya Menon",1,3.5,"Transmission and Distribution",1,
5,"EEE","EE5101",30,0,"Rajesh Iyer",2,3.5,"Transmission and Distribution",14,
5,"EEE","EE5101",30,0,"Anjali Raghavan",3,3.5,"Transmission and Distribution",0,
5,"EEE","EE5102",30,0,"Arjun Nair",1,2.5,"Switchgear and Protection",21,
5,"EEE","EE5102",30,0,"Sneha Reddy",2,2.5,"Switchgear and Protection",30,
5,"EEE","EE5102",30,0,"Vijay Kumar",3,2.5,"Switchgear and Protection",22,
5,"EEE","EE5103",30,0,"Vivek Menon",1,2.5,"Renewable Energy Sources",25,
5,"EEE","EE5103",30,0,"Deepika Rao",2,2.5,"Renewable Energy Sources",13,
5,"EEE","EE5103",30,0,"Rahul Krishnan",3,2.5,"Renewable Energy",9,
5,"EEE","EE5104",30,0,"Anusha Naidu",1,3,"Electrical Machine Design",9,
5,"EEE","EE5104",30,0,"Ajay Menon",2,3,"Electrical Machine Design",19,
5,"EEE","EE5104",30,0,"Meenakshi Rajan",3,3,"Electrical Machine Design",19,
5,"EEE","EE5105",30,0,"Siddharth Iyer",1,3,"High Voltage Engineering",9,
5,"EEE","EE5105",30,0,"Shalini Nair",2,3,"High Voltage Engineering",12,
5,"EEE","EE5105",30,0,"Krishna Menon",3,3,"High Voltage Engineering",27,
5,"EEE","EE5106",30,0,"Rajesh Menon",1,3,"Industrial Drives and Control",6,
5,"EEE","EE5106",30,0,"Divya Kumar",2,3,"Industrial Drives and Control",7,
5,"EEE","EE5106",30,0,"Arjun Reddy",3,3,"Industrial Drives and Control",19,
5,"EEE","EE5108",30,0,"Karthik Naidu",1,3.5,"Mini Project",18,
5,"EEE","EE5108",30,0,"Shweta Kumar",2,3.5,"Mini Project",9,
5,"EEE","EE5108",30,0,"Prakash Menon",3,3.5,"Mini Project",1,
6,"EEE","EE6101",30,0,"Krishna Murthy",1,3,"Power System Operation and Control",0,
6,"EEE","EE6101",30,0,"Rajeshwari Naidu",2,3,"Power System Operation and Control",6,
6,"EEE","EE6101",30,0,"Siddharth Rajan",3,3,"Power System Operation and Control",6,
6,"EEE","EE6102",30,0,"Ranjith Kumar",1,3,"Power System Protection",10,
6,"EEE","EE6102",30,0,"Divya Reddy",2,3,"Power System Protection",30,
6,"EEE","EE6102",30,0,"Arjun Menon",3,3,"Power System Protection",17,
6,"EEE","EE6103",30,0,"Shalini Raghavan",1,3,"Utilization of Electrical Energy",8,
6,"EEE","EE6103",30,0,"Vivek Nair",2,3,"Utilization of Electrical Energy",16,
6,"EEE","EE6103",30,0,"Meenakshi Rao",3,3,"Utilization of Electrical Energy",15,
6,"EEE","EE6104",30,0,"Deepa Balan",1,3,"Power System Planning",5,
6,"EEE","EE6104",30,0,"Ajay Menon",2,3,"Power System Planning",20,
6,"EEE","EE6104",30,0,"Nandini Iyer",3,3,"Power System Planning",14,
6,"EEE","EE6105",30,0,"Rajesh Kumar",1,3.5,"Electric Traction",4,
6,"EEE","EE6105",30,0,"Anusha Srinivasan",2,3.5,"Electric Traction",3,
6,"EEE","EE6105",30,0,"Suresh Nair",3,3.5,"Electric Traction",30,
6,"EEE","EE6106",30,0,"Meera Menon",1,3,"FACTS and HVDC Transmission",4,
6,"EEE","EE6106",30,0,"Siddharth Raghavan",2,3,"FACTS and HVDC Transmission",0,
6,"EEE","EE6106",30,0,"Akshay Reddy",3,3,"FACTS and HVDC Transmission",6,
7,"EEE","EE7101",30,0,"Divya Menon",1,4,"Power System Dynamics",2,
7,"EEE","EE7101",30,0,"Rajesh Iyer",2,4,"Power System Dynamics",11,
7,"EEE","EE7101",30,0,"Anjali Raghavan",3,4,"Power System Dynamics",18,
7,"EEE","EE7102",30,0,"Arjun Nair",1,3,"Smart Grid Technology",21,
7,"EEE","EE7102",30,0,"Sneha Reddy",2,3,"Smart Grid Technology",22,
7,"EEE","EE7102",30,0,"Vijay Kumar",3,3,"Smart Grid Technology",7,
7,"EEE","EE7103",30,0,"Vivek Menon",1,2.5,"Renewable Energy Integration",13,
7,"EEE","EE7103",30,0,"Deepika Rao",2,2.5,"Renewable Energy Integration",1,
7,"EEE","EE7103",30,0,"Rahul Krishnan",3,2.5,"Renewable Energy Integration",11,
7,"EEE","EE7104",30,0,"Anjali Kumar",1,2,"Industrial Management",23,
7,"EEE","EE7104",30,0,"Deepak Menon",2,2,"Industrial Management",1,
7,"EEE","EE7104",30,0,"Rajesh Nair",3,2,"Industrial Management",8,
7,"EEE","EE7105",30,0,"Anjali Nair",1,4.5,"Project Work Phase",19,
7,"EEE","EE7105",30,0,"Vivek Krishnan",2,4.5,"Project Work Phase",5,
7,"EEE","EE7105",30,0,"Deepak Menon",3,4.5,"Project Work Phase",1,
7,"EEE","EE7106",30,0,"Karthik Naidu",1,2,"Communication Skills",19,
7,"EEE","EE7106",30,0,"Shweta Kumar",2,2,"Communication Skills",0,
7,"EEE","EE7106",30,0,"Prakash Menon",3,2,"Communication Skills",21,
8,"EEE","EE8101",30,0,"Rajesh Menon",1,4.5,"Project Work Phase II",3,
8,"EEE","EE8101",30,0,"Divya Kumar",2,4.5,"Project Work Phase II",1,
8,"EEE","EE8101",30,0,"Arjun Reddy",3,4.5,"Project Work Phase II",22,
8,"EEE","EE8102",30,0,"Anjali Kumar",1,2,"Industrial Training / In-Plant Training",20,
8,"EEE","EE8102",30,0,"Deepak Menon",2,2,"Industrial Training / In-Plant Training",4,
8,"EEE","EE8102",30,0,"Rajesh Nair",3,2,"Industrial Training / In-Plant Training",14,
8,"EEE","EE8103",30,0,"Karthik Naidu",1,2,"Entrepreneurship Development Programme",1,
8,"EEE","EE8103",30,0,"Shweta Kumar",2,2,"Entrepreneurship Development Programme",27,
8,"EEE","EE8103",30,0,"Prakash Menon",3,2,"Entrepreneurship Development Programme",16,
8,"EEE","EE8104",30,0,"Krishna Murthy",1,2,"Comprehensive Viva Voce",20,
8,"EEE","EE8104",30,0,"Rajeshwari Naidu",2,2,"Comprehensive Viva Voce",15,
8,"EEE","EE8104",30,0,"Siddharth Rajan",3,2,"Comprehensive Viva Voce",25,
8,"EEE","EE8105",30,0,"Anjali Nair",1,2,"Power Generation Techniques",20,
8,"EEE","EE8105",30,0,"Vivek Krishnan",2,2,"Power Generation Techniques",12,
8,"EEE","EE8105",30,0,"Deepak Menon",3,2,"Power Generation Techniques",30,
8,"EEE","EE8106",30,0,"Anjali Kumar",1,2,"Technical Communication",1,
8,"EEE","EE8106",30,0,"Deepak Menon",2,2,"Technical Communication",21,
8,"EEE","EE8106",30,0,"Rajesh Nair",3,2,"Technical Communication",22,
1,"MECH","ME1101",30,0,"Krishna Murthy",1,4,"Mathematics I",13,
1,"MECH","ME1101",30,0,"Rajeshwari Naidu",2,4,"Mathematics I",8,
1,"MECH","ME1101",30,0,"Siddharth Rajan",3,4,"Mathematics I",8,
1,"MECH","ME1102",30,0,"Ranjith Kumar",1,3,"Physics I",2,
1,"MECH","ME1102",30,0,"Divya Reddy",2,3,"Physics I",13,
1,"MECH","ME1102",30,0,"Arjun Menon",3,3,"Physics I",19,
1,"MECH","ME1103",30,0,"Shalini Raghavan",1,3,"Chemistry",0,
1,"MECH","ME1103",30,0,"Vivek Nair",2,3,"Chemistry",28,
1,"MECH","ME1103",30,0,"Meenakshi Rao",3,3,"Chemistry",28,
1,"MECH","ME1104",30,0,"Deepa Balan",1,3,"Introduction to Computing",1,
1,"MECH","ME1104",30,0,"Ajay Menon",2,3,"Introduction to Computing",14,
1,"MECH","ME1104",30,0,"Nandini Iyer",3,3,"Introduction to Computing",23,
1,"MECH","ME1105",30,0,"Rajesh Kumar",1,3,"Engineering Graphics",25,
1,"MECH","ME1105",30,0,"Anusha Srinivasan",2,3,"Engineering Graphics",29,
1,"MECH","ME1105",30,0,"Suresh Nair",3,3,"Engineering Graphics",15,
1,"MECH","ME1106",30,0,"Meera Menon",1,3,"Basic Electrical Engineering",6,
1,"MECH","ME1106",30,0,"Siddharth Raghavan",2,3,"Basic Electrical Engineering",22,
1,"MECH","ME1106",30,0,"Akshay Reddy",3,3,"Basic Electrical Engineering",15,
2,"MECH","ME2101",30,0,"Divya Menon",1,4,"Mathematics II",0,
2,"MECH","ME2101",30,0,"Rajesh Iyer",2,4,"Mathematics II",22,
2,"MECH","ME2101",30,0,"Anjali Raghavan",3,4,"Mathematics II",9,
2,"MECH","ME2102",30,0,"Arjun Nair",1,3,"Physics II",24,
2,"MECH","ME2102",30,0,"Sneha Reddy",2,3,"Physics II",3,
2,"MECH","ME2102",30,0,"Vijay Kumar",3,3,"Physics II",0,
2,"MECH","ME2103",30,0,"Vivek Menon",1,4,"Engineering Mechanics",28,
2,"MECH","ME2103",30,0,"Deepika Rao",2,4,"Engineering Mechanics",27,
2,"MECH","ME2103",30,0,"Rahul Krishnan",3,4,"Engineering Mechanics",13,
2,"MECH","ME2104",30,0,"Anusha Naidu",1,3.5,"Thermodynamics",12,
2,"MECH","ME2104",30,0,"Ajay Menon",2,3.5,"Thermodynamics",25,
2,"MECH","ME2104",30,0,"Meenakshi Rajan",3,3.5,"Thermodynamics",27,
2,"MECH","ME2105",30,0,"Siddharth Iyer",1,3,"Material Science",13,
2,"MECH","ME2105",30,0,"Shalini Nair",2,3,"Material Science",18,
2,"MECH","ME2105",30,0,"Krishna Menon",3,3,"Material Science",7,
2,"MECH","ME2106",30,0,"Rajesh Menon",1,3,"Basic Electronics Engineering",23,
2,"MECH","ME2106",30,0,"Divya Kumar",2,3,"Basic Electronics Engineering",3,
2,"MECH","ME2106",30,0,"Arjun Reddy",3,3,"Basic Electronics Engineering",16,
3,"MECH","ME3101",30,0,"Anjali Menon",1,4,"Mathematics III",24,
3,"MECH","ME3101",30,0,"Krishna Kumar",2,4,"Mathematics III",16,
3,"MECH","ME3101",30,0,"Shalini Iyer",3,4,"Mathematics III",11,
3,"MECH","ME3102",30,0,"Rahul Nair",1,3.5,"Fluid Mechanics",24,
3,"MECH","ME3102",30,0,"Divya Menon",2,3.5,"Fluid Mechanics",28,
3,"MECH","ME3102",30,0,"Arjun Iyer",3,3.5,"Fluid Mechanics",20,
3,"MECH","ME3103",30,0,"Meenakshi Kumar",1,3,"Mechanics of Solids",28,
3,"MECH","ME3103",30,0,"Vivek Naidu",2,3,"Mechanics of Solids",2,
3,"MECH","ME3103",30,0,"Siddharth Menon",3,3,"Mechanics of Solids",3,
3,"MECH","ME3104",30,0,"Suresh Menon",1,3,"Manufacturing Processes",15,
3,"MECH","ME3104",30,0,"Deepa Reddy",2,3,"Manufacturing Processes",22,
3,"MECH","ME3104",30,0,"Rajesh Iyer",3,3,"Manufacturing Processes",23,
3,"MECH","ME3105",30,0,"Anusha Kumar",1,3.5,"Electrical Machines",21,
3,"MECH","ME3105",30,0,"Ajay Nair",2,3.5,"Electrical Machines",19,
3,"MECH","ME3105",30,0,"Meera Iyer",3,3.5,"Electrical Machines",12,
3,"MECH","ME3106",30,0,"Vijay Menon",1,3,"Introduction to Programming",5,
3,"MECH","ME3106",30,0,"Divya Nair",2,3,"Introduction to Programming",22,
3,"MECH","ME3106",30,0,"Arjun Kumar",3,3,"Introduction to Programming",20,
4,"MECH","ME4101",30,0,"Arjun Iyer",1,4,"Mathematics IV",20,
4,"MECH","ME4101",30,0,"Vivek Kumar",2,4,"Mathematics IV",25,
4,"MECH","ME4101",30,0,"Meenakshi Menon",3,4,"Mathematics IV",18,
4,"MECH","ME4102",30,0,"Rajesh Naidu",1,3,"Heat and Mass Transfer",26,
4,"MECH","ME4102",30,0,"Siddharth Reddy",2,3,"Heat and Mass Transfer",21,
4,"MECH","ME4102",30,0,"Divya Kumar",3,3,"Heat and Mass Transfer",16,
4,"MECH","ME4103",30,0,"Shalini Kumar",1,3,"Kinematics of Machinery",2,
4,"MECH","ME4103",30,0,"Anjali Naidu",2,3,"Kinematics of Machinery",11,
4,"MECH","ME4103",30,0,"Rahul Reddy",3,3,"Kinematics of Machinery",27,
4,"MECH","ME4104",30,0,"Arjun Menon",1,3.5,"Manufacturing Technology",3,
4,"MECH","ME4104",30,0,"Vivek Nair",2,3.5,"Manufacturing Technology",26,
4,"MECH","ME4104",30,0,"Meera Menon",3,3.5,"Manufacturing Technology",0,
4,"MECH","ME4105",30,0,"Divya Iyer",1,3,"Dynamics of Machinery",16,
4,"MECH","ME4105",30,0,"Rajesh Kumar",2,3,"Dynamics of Machinery",16,
4,"MECH","ME4105",30,0,"Anusha Menon",3,3,"Dynamics of Machinery",10,
4,"MECH","ME4106",30,0,"Siddharth Menon",1,4,"Electrical and Electronics Measurements",9,
4,"MECH","ME4106",30,0,"Deepa Reddy",2,4,"Electrical and Electronics Measurements",12,
4,"MECH","ME4106",30,0,"Suresh Kumar",3,4,"Electrical and Electronics Measurements",26,
5,"MECH","ME5101",30,0,"Deepak Nair",1,3,"Finite Element Analysis",2,
5,"MECH","ME5101",30,0,"Anusha Kumar",2,3,"Finite Element Analysis",24,
5,"MECH","ME5101",30,0,"Rajesh Iyer",3,3,"Finite Element Analysis",10,
5,"MECH","ME5102",30,0,"Anjali Menon",1,2.5,"Refrigeration and Air Conditioning",1,
5,"MECH","ME5102",30,0,"Arjun Reddy",2,2.5,"Refrigeration and Air Conditioning",13,
5,"MECH","ME5102",30,0,"Divya Menon",3,2.5,"Refrigeration and Air Conditioning",2,
5,"MECH","ME5103",30,0,"Rahul Kumar",1,3,"Turbo Machines",13,
5,"MECH","ME5103",30,0,"Meera Reddy",2,3,"Turbo Machines",7,
5,"MECH","ME5103",30,0,"Siddharth Menon",3,3,"Turbo Machines",9,
5,"MECH","ME5104",30,0,"Shalini Naidu",1,3,"Machine Tools and Operations",6,
5,"MECH","ME5104",30,0,"Vivek Kumar",2,3,"Machine Tools and Operations",20,
5,"MECH","ME5104",30,0,"Meenakshi Menon",3,3,"Machine Tools and Operations",25,
5,"MECH","ME5105",30,0,"Arjun Menon",1,4,"Automobile Engineering",30,
5,"MECH","ME5105",30,0,"Divya Nair",2,4,"Automobile Engineering",12,
5,"MECH","ME5105",30,0,"Rajesh Reddy",3,4,"Automobile Engineering",27,
5,"MECH","ME5106",30,0,"Anusha Kumar",1,2.5,"Power Plant Engineering",20,
5,"MECH","ME5106",30,0,"Arjun Kumar",2,2.5,"Power Plant Engineering",24,
5,"MECH","ME5106",30,0,"Deepa Reddy",3,2.5,"Power Plant Engineering",25,
6,"MECH","ME6101",30,0,"Meera Menon",1,3,"Design of Machine Elements",22,
6,"MECH","ME6101",30,0,"Arjun Kumar",2,3,"Design of Machine Elements",27,
6,"MECH","ME6101",30,0,"Siddharth Menon",3,3,"Design of Machine Elements",15,
6,"MECH","ME6102",30,0,"Divya Nair",1,2.5,"Operations Research",28,
6,"MECH","ME6102",30,0,"Anusha Kumar",2,2.5,"Operations Research",18,
6,"MECH","ME6102",30,0,"Rajesh Reddy",3,2.5,"Operations Research",20,
6,"MECH","ME6103",30,0,"Rajesh Naidu",1,3,"Robotics",6,
6,"MECH","ME6103",30,0,"Meenakshi Menon",2,3,"Robotics",13,
6,"MECH","ME6103",30,0,"Shalini Kumar",3,3,"Robotics",12,
6,"MECH","ME6104",30,0,"Arjun Menon",1,3.5,"Mechatronics",25,
6,"MECH","ME6104",30,0,"Divya Nair",2,3.5,"Mechatronics",4,
6,"MECH","ME6104",30,0,"Anusha Kumar",3,3.5,"Mechatronics",16,
6,"MECH","ME6105",30,0,"Rajesh Reddy",1,3,"CAD/CAM/CIM",9,
6,"MECH","ME6105",30,0,"Rajesh Naidu",2,3,"CAD/CAM/CIM",15,
6,"MECH","ME6105",30,0,"Meera Menon",3,3,"CAD/CAM/CIM",14,
6,"MECH","ME6106",30,0,"Anjali Kumar",1,2.5,"Total Quality Management",26,
6,"MECH","ME6106",30,0,"Deepak Nair",2,2.5,"Total Quality Management",9,
6,"MECH","ME6106",30,0,"Arjun Kumar",3,2.5,"Total Quality Management",20,
7,"MECH","ME7101",30,0,"Siddharth Menon",1,4,"Finite Element Analysis and Computational Fluid Dynamics Lab",6,
7,"MECH","ME7101",30,0,"Rajesh Reddy",2,4,"Finite Element Analysis and Computational Fluid Dynamics Lab",18,
7,"MECH","ME7101",30,0,"Meera Menon4",3,4,"Finite Element Analysis and Computational Fluid Dynamics Lab",10,
7,"MECH","ME7102",30,0,"Divya Nair",1,4.5,"Project Work Phase I",27,
7,"MECH","ME7102",30,0,"Arjun Kumar",2,4.5,"Project Work Phase I",28,
7,"MECH","ME7102",30,0,"Anusha Menon",3,4.5,"Project Work Phase I",4,
7,"MECH","ME7103",30,0,"Anjali Kumar",1,2,"Industrial Engineering",16,
7,"MECH","ME7103",30,0,"Krishna Reddy",2,2,"Industrial Engineering",4,
7,"MECH","ME7103",30,0,"Shweta Menon",3,2,"Industrial Engineering",3,
7,"MECH","ME7104",30,0,"Krishna Menon",1,1.5,"Managerial Economics and Financial Analysis",19,
7,"MECH","ME7104",30,0,"Shalini Nair",2,1.5,"Managerial Economics and Financial Analysis",8,
7,"MECH","ME7104",30,0,"Arjun Kumar",3,1.5,"Managerial Economics and Financial Analysis",23,
7,"MECH","ME7105",30,0,"Rajesh Naidu",1,3,"Control Engineering",3,
7,"MECH","ME7105",30,0,"Shalini Menon",2,3,"Control Engineering",8,
7,"MECH","ME7105",30,0,"Anjali Kumar",3,3,"Control Engineering",24,
7,"MECH","ME7106",30,0,"Arjun Reddy",1,2,"Seminar",29,
7,"MECH","ME7106",30,0,"Divya Nair",2,2,"Seminar",17,
7,"MECH","ME7106",30,0,"Meera Menon",3,2,"Seminar",21,
8,"MECH","ME8101",30,0,"Anusha Menon",1,4.5,"Project Work Phase II",23,
8,"MECH","ME8101",30,0,"Arjun Kumar",2,4.5,"Project Work Phase II",8,
8,"MECH","ME8101",30,0,"Divya Nair",3,4.5,"Project Work Phase II",13,
8,"MECH","ME8102",30,0,"Rajesh Reddy",1,2,"Industrial Training / In-Plant Training",16,
8,"MECH","ME8102",30,0,"Siddharth Menon",2,2,"Industrial Training / In-Plant Training",4,
8,"MECH","ME8102",30,0,"Meera Menon",3,2,"Industrial Training / In-Plant Training",2,
8,"MECH","ME8103",30,0,"Divya Nair",1,2,"Entrepreneurship Development Programme",16,
8,"MECH","ME8103",30,0,"Anusha Menon",2,2,"Entrepreneurship Development Programme",4,
8,"MECH","ME8103",30,0,"Arjun Kumar",3,2,"Entrepreneurship Development Programme",18,
8,"MECH","ME8104",30,0,"Meera Menon",1,2,"Comprehensive Viva Voce",19,
8,"MECH","ME8104",30,0,"Rajesh Reddy",2,2,"Comprehensive Viva Voce",11,
8,"MECH","ME8104",30,0,"Siddharth Menon",3,2,"Comprehensive Viva Voce",4,
8,"MECH","ME8105",30,0,"Anjali Nair",1,2,"Workshop Practice",21,
8,"MECH","ME8105",30,0,"Vivek Krishnan",2,2,"Workshop Practice",29,
8,"MECH","ME8105",30,0,"Deepak Menon",3,2,"Workshop Practice",14,
8,"MECH","ME8106",30,0,"Karthik Naidu",1,2,"Communication Skills",13,
8,"MECH","ME8106",30,0,"Shweta Kumar",2,2,"Communication Skills",29,
8,"MECH","ME8106",30,0,"Prakash Menon",3,2,"Communication Skills",17,

};

    // Sample elective course records
    Course elec_courses[] = {1,"MECH","ME1201",30,0,"Shweta Menon",2,1,"Introduction to Management",6,
1,"MECH","ME1201",30,0,"Deepak Reddy",2,2,"Introduction to Management",10,
1,"MECH","ME1202",30,0,"Krishna Nair",2,1,"Soft Skills Development",24,
1,"MECH","ME1202",30,0,"Shalini Menon",2,2,"Soft Skills Development",29,
1,"MECH","ME1203",30,0,"Anjali Nair",3,1,"History of Tamils",19,
1,"MECH","ME1203",30,0,"Vivek Krishnan",3,2,"History of Tamils",10,
2,"MECH","ME2201",30,0,"Anjali Reddy",3,1,"Film Appreciation",20,
2,"MECH","ME2201",30,0,"Siddharth Iyer",3,2,"Film Appreciation",26,
2,"MECH","ME2202",30,0,"Sakshi V.S",2,1,"Human resource and Development",12,
2,"MECH","ME2202",30,0,"Priyamani",2,2,"Human resource and Development",23,
2,"MECH","ME2203",30,0,"Divya Kumar",3,1,"Human Psycology",19,
2,"MECH","ME2203",30,0,"Arjun Reddy",2,2,"Human Psycology",23,
3,"MECH","ME3201",30,0,"Sneha Reddy",3,1,"Project Management",24,
3,"MECH","ME3201",30,0,"Vijay Kumar",3,2,"Project Management",27,
3,"MECH","ME3202",30,0,"Rahul Krishnan",3,1,"Intellectual Property Rights",17,
3,"MECH","ME3202",30,0,"Arjun Nair",2,2,"Intellectual Property Rights",18,
3,"MECH","ME3203",30,0,"Vivek Krishnan",2,1,"Professional Ethics and Human Values",0,
3,"MECH","ME3203",30,0,"Deepak Menon",2,2,"Professional Ethics and Human Values",24,
4,"MECH","ME4201",30,0,"Meenakshi Rajan",3,1,"Innovation and Creativity",0,
4,"MECH","ME4201",30,0,"Deepa Balan",3,2,"Innovation and Creativity",27,
4,"MECH","ME4202",30,0,"Prakash Menon",2,1,"Disaster Management and Preparedness",16,
4,"MECH","ME4202",30,0,"R.K.Rahulram",2,1,"Tamil and Technology",14,
4,"MECH","ME4202",30,0,"Divya Menon",2,2,"Disaster Management and Preparedness",26,
4,"MECH","ME4202",30,0,"Siddhart Kumar",3,2,"Tamil and Technology",15,
5,"MECH","ME5201",30,0,"Arjun Reddy",2,1,"Supply Chain Management",25,
5,"MECH","ME5201",30,0,"Karthik Naidu",3,2,"Supply Chain Management",15,
5,"MECH","ME5202",30,0,"Shweta Kumar",2,1,"Financial Management",25,
5,"MECH","ME5202",30,0,"Siddharth Rajan",2,2,"Financial Management",27,
5,"MECH","ME5203",30,0,"Divya Kumar",3,1,"Entrepreneurship Development",15,
5,"MECH","ME5203",30,0,"Rajesh Menon",3,2,"Entrepreneurship Development",0,
6,"MECH","ME6201",30,0,"Anjali Raghavan",3,1,"Operations Management",22,
6,"MECH","ME6201",30,0,"Rajeshwari Naidu",2,2,"Operations Management",26,
6,"MECH","ME6202",30,0,"Vivek Nair",3,1,"Total Quality Management",27,
6,"MECH","ME6202",30,0,"Anusha Naidu",2,2,"Total Quality Management",1,
6,"MECH","ME6203",30,0,"Shweta Kumar",2,1,"Engineering Economics",19,
6,"MECH","ME6203",30,0,"Prakash Menon",3,2,"Engineering Economics",27,
7,"MECH","ME7201",30,0,"Deepak Menon",2,1,"Negotiation and Conflict Management",22,
7,"MECH","ME7201",30,0,"Shalini Raghavan",2,2,"Negotiation and Conflict Management",25,
7,"MECH","ME7202",30,0,"Meera Menon",2,1,"Sustainable Development and Corporate Social Responsibility (CSR)",25,
7,"MECH","ME7202",30,0,"Shweta Menon",2,2,"Sustainable Development and Corporate Social Responsibility (CSR)",25,
7,"MECH","ME7203",30,0,"Saravanan Chandrasekhar",2,1,"Entrepreneurship Development Programme",25,
7,"MECH","ME7203",30,0,"Sangeeta Ramanathan",3,2,"Entrepreneurship Development Programme",26,
8,"MECH","ME3203",30,0,"Rahul Krishnan",2,1,"Mechanical Measurements and Metrology",15,
8,"MECH","ME3203",30,0,"Arjun Nair",3,2,"Mechanical Measurements and Metrology",16,
8,"MECH","ME8201",30,0,"Ranjith Iyer",2,1,"Risk Management",19,
8,"MECH","ME8201",30,0,"Arjun Nair",2,2,"Risk Management",20,
8,"MECH","ME8202",30,0,"Anjali Nair",3,1,"Change Management",0,
8,"MECH","ME8202",30,0,"Deepak Reddy",3,2,"Change Management",17,
1,"CSE","CS1201",30,0,"Rajeshwari Naidu",1,2,"Introduction to Humanities and Social Sciences",25,
1,"CSE","CS1201",30,0,"Shalini Menon",2,2,"Introduction to Humanities and Social Sciences",24,
1,"CSE","CS1202",30,0,"Anjali Reddy",1,3,"Soft Skills Development",15,
1,"CSE","CS1202",30,0,"Deepak Menon",2,3,"Soft Skills Development",17,
1,"CSE","CS1203",30,0,"Anjali Nair",1,3,"History of Tamils",0,
1,"CSE","CS1203",30,0,"Vivek Krishnan",2,3,"History of Tamils",26,
2,"CSE","CS2201",30,0,"Siddharth Iyer",1,2,"Basics of Psychology",14,
2,"CSE","CS2201",30,0,"Divya Kumar",2,2,"Basics of Psychology",24,
2,"CSE","CS2202",30,0,"Arjun Nair",1,2,"Professional Ethics and Values",0,
2,"CSE","CS2202",30,0,"Anjali Menon",2,2,"Professional Ethics and Values",15,
2,"CSE","CS2203",30,0,"Anjali Reddy",1,3,"Film Appreciation",25,
2,"CSE","CS2203",30,0,"Siddharth Iyer",2,3,"Film Appreciation",16,
3,"CSE","CS3201",30,0,"Vivek Krishnan",1,2,"Technical Writing and Communication Skills",23,
3,"CSE","CS3201",30,0,"Shweta Menon",2,2,"Technical Writing and Communication Skills",27,
3,"CSE","CS3202",30,0,"Rahul Krishnan",1,3,"Foreign Language (French, German, Spanish)",29,
3,"CSE","CS3202",30,0,"Meera Menon",2,3,"Foreign Language (French, German, Spanish)",20,
3,"CSE","CS3202",30,0,"Sakshi V.S",1,3,"Human resource and Development",16,
3,"CSE","CS3202",30,0,"Priyamani",2,3,"Human resource and Development",26,
4,"CSE","CS4201",30,0,"Shweta Kumar",1,3,"Introduction to Sociology",0,
4,"CSE","CS4201",30,0,"Anusha Naidu",2,3,"Introduction to Sociology",11,
4,"CSE","CS4202",30,0,"Krishna Menon",1,3,"Disaster Management and Preparedness",23,
4,"CSE","CS4202",30,0,"Arjun Reddy",2,3,"Disaster Management and Preparedness",22,
4,"CSE","CS4203",30,0,"Meenakshi Rajan",1,3,"Innovation and Creativity",25,
4,"CSE","CS4203",30,0,"Deepa Balan",2,3,"Innovation and Creativity",22,
5,"CSE","CS5201",30,0,"Deepa Balan",1,3,"Financial Management",21,
5,"CSE","CS5201",30,0,"Rajesh Menon",2,3,"Financial Management",13,
5,"CSE","CS5202",30,0,"Shalini Raghavan",1,2,"Innovation Management",15,
5,"CSE","CS5202",30,0,"Divya Menon",2,2,"Innovation Management",26,
5,"CSE","CS5203",30,0,"R.K.Rahulram",1,2,"Tamil and Technology",24,
5,"CSE","CS5203",30,0,"Siddhart Kumar",2,2,"Tamil and Technology",15,
5,"CSE","CS5204",30,0,"Shweta Kumar",1,3,"Engineering Economics",0,
5,"CSE","CS5204",30,0,"Prakash Menon",2,3,"Engineering Economics",7,
6,"CSE","CS6201",30,0,"Anjali Raghavan",1,3,"Operations Management",23,
6,"CSE","CS6201",30,0,"Vivek Nair",2,3,"Operations Management",23,
6,"CSE","CS6202",30,0,"Anjali Kumar",1,3,"Marketing Management",26,
6,"CSE","CS6202",30,0,"Prakash Menon",2,3,"Marketing Management",26,
6,"CSE","CS6203",30,0,"Divya Kumar",1,2,"Entrepreneurship Development",0,
6,"CSE","CS6203",30,0,"Rajesh Menon",2,2,"Entrepreneurship Development",18,
7,"CSE","CS7201",30,0,"Sneha Reddy",1,2,"Business Analytics",19,
7,"CSE","CS7201",30,0,"Arjun Menon",2,2,"Business Analytics",21,
7,"CSE","CS7202",30,0,"Meenakshi Rajan",1,3,"Sustainable Development and Corporate Social Responsibility (CSR)",20,
7,"CSE","CS7202",30,0,"Deepak Reddy",2,3,"Sustainable Development and Corporate Social Responsibility (CSR)",29,
7,"CSE","CS7203",30,0,"Saravanan Chandrasekhar",1,3,"Entrepreneurship Development Programme",23,
7,"CSE","CS7203",30,0,"Sangeeta Ramanathan",2,3,"Entrepreneurship Development Programme",26,
8,"CSE","CS8201",30,0,"Shalini Raghavan",1,3,"Technology Management",17,
8,"CSE","CS8201",30,0,"Siddharth Rajan",2,3,"Technology Management",2,
8,"CSE","CS8202",30,0,"Ranjith Iyer",1,2,"Corporate Governance",3,
8,"CSE","CS8202",30,0,"Shweta Kumar",2,2,"Corporate Governance",7,
8,"CSE","CS8203",30,0,"Ranjith Iyer",1,2,"Risk Management",2,
8,"CSE","CS8203",30,0,"Arjun Nair",2,2,"Risk Management",23,
1,"IT","IT1201",30,0,"Rajeshwari Naidu",1,2,"Introduction to Humanities and Social Sciences",12,
1,"IT","IT1201",30,0,"Shalini Menon",2,2,"Introduction to Humanities and Social Sciences",11,
1,"IT","IT1202",30,0,"Anjali Reddy",1,3,"Soft Skills Development",5,
1,"IT","IT1202",30,0,"Deepak Menon",2,3,"Soft Skills Development",7,
1,"IT","IT1203",30,0,"Anjali Nair",1,3,"History of Tamils",2,
1,"IT","IT1203",30,0,"Vivek Krishnan",2,3,"History of Tamils",6,
2,"IT","IT2201",30,0,"Siddharth Iyer",1,2,"Basics of Psychology",0,
2,"IT","IT2201",30,0,"Divya Kumar",2,2,"Basics of Psychology",10,
2,"IT","IT2202",30,0,"Arjun Nair",1,2,"Professional Ethics and Values",23,
2,"IT","IT2202",30,0,"Anjali Menon",2,2,"Professional Ethics and Values",21,
2,"IT","IT2203",30,0,"Anjali Reddy",1,3,"Film Appreciation",14,
2,"IT","IT2203",30,0,"Siddharth Iyer",2,3,"Film Appreciation",15,
3,"IT","IT3201",30,0,"Vivek Krishnan",1,2,"Technical Writing and Communication Skills",22,
3,"IT","IT3201",30,0,"Shweta Menon",2,2,"Technical Writing and Communication Skills",12,
3,"IT","IT3202",30,0,"Rahul Krishnan",1,3,"Foreign Language (French, German, Spanish)",4,
3,"IT","IT3202",30,0,"Meera Menon",2,3,"Foreign Language (French, German, Spanish)",9,
3,"IT","IT3202",30,0,"Sakshi V.S",1,3,"Human resource and Development",10,
3,"IT","IT3202",30,0,"Priyamani",2,3,"Human resource and Development",2,
4,"IT","IT4201",30,0,"Shweta Kumar",1,3,"Introduction to Sociology",5,
4,"IT","IT4201",30,0,"Anusha Naidu",2,3,"Introduction to Sociology",17,
4,"IT","IT4202",30,0,"Krishna Menon",1,3,"Disaster Management and Preparedness",13,
4,"IT","IT4202",30,0,"Arjun Reddy",2,3,"Disaster Management and Preparedness",3,
4,"IT","IT4203",30,0,"Meenakshi Rajan",1,3,"Innovation and Creativity",18,
4,"IT","IT4203",30,0,"Deepa Balan",2,3,"Innovation and Creativity",23,
5,"IT","IT5201",30,0,"Deepa Balan",1,3,"Financial Management",15,
5,"IT","IT5201",30,0,"Rajesh Menon",2,3,"Financial Management",28,
5,"IT","IT5202",30,0,"Shalini Raghavan",1,2,"Innovation Management",12,
5,"IT","IT5202",30,0,"Divya Menon",2,2,"Innovation Management",2,
5,"IT","IT5203",30,0,"R.K.Rahulram",1,2,"Tamil and Technology",4,
5,"IT","IT5203",30,0,"Siddhart Kumar",2,2,"Tamil and Technology",7,
5,"IT","IT5204",30,0,"Shweta Kumar",1,3,"Engineering Economics",23,
5,"IT","IT5204",30,0,"Prakash Menon",2,3, "Engineering Economics",21,
6,"IT","IT6201",30,0,"Anjali Raghavan",1,3,"Operations Management",10,
6,"IT","IT6201",30,0,"Vivek Nair", 2, 3, "Operations Management",23,
6,"IT","IT6202",30,0,"Anjali Kumar",1,3,"Marketing Management",18,
6,"IT","IT6202",30,0,"Prakash Menon",2,3,"Marketing Management",23,
6,"IT","IT6203",30,0,"Divya Kumar",1,2,"Entrepreneurship Development",23,
6,"IT","IT6203",30,0,"Rajesh Menon",2,2,"Entrepreneurship Development",23,
7,"IT","IT7201",30,0,"Sneha Reddy",1,2,"Business Analytics",12,
7,"IT","IT7201",30,0,"Arjun Menon",2,2,"Business Analytics",11,
7,"IT","IT7202",30,0,"Meenakshi Rajan",1,3,"Sustainable Development and Corporate Social Responsibility (ITR)",10,
7,"IT","IT7202",30,0,"Deepak Reddy",2,3,"Sustainable Development and Corporate Social Responsibility (ITR)",11,
7,"IT","IT7203",30,0,"Saravanan Chandrasekhar",1,3,"Entrepreneurship Development Programme",16,
7,"IT","IT7203",30,0,"Sangeeta Ramanathan",2,3,"Entrepreneurship Development Programme",13,
8,"IT","IT8201",30,0,"Shalini Raghavan",1,3,"Technology Management",17,
8,"IT","IT8201",30,0,"Siddharth Rajan",2,3,"Technology Management",2,
8,"IT","IT8202",30,0,"Ranjith Iyer",1,2,"Corporate Governance",23,
8,"IT","IT8202",30,0,"Shweta Kumar",2,2,"Corporate Governance",12,
8,"IT","IT8203",30,0,"Ranjith Iyer",1,2,"Risk Management",11,
8,"IT","IT8203",30,0,"Arjun Nair",2,2,"Risk Management",5,
1,"ECE","EC1201",30,0,"Anjali Nair",1,3,"Introduction to Management",2,
1,"ECE","EC1201",30,0,"Shalini Raghavan",2,3,"Introduction to Management",3,
1,"ECE","EC1202",30,0,"Vivek Krishnan",1,3,"Soft Skills Development",5,
1,"ECE","EC1202",30,0,"Deepak Menon",2,3,"Soft Skills Development",19,
1,"ECE","EC1203",30,0,"Divya Menon",1,2,"Introduction to Electronics",10,
1,"ECE","EC1203",30,0,"Rajesh Iyer",2,2,"Introduction to Electronics",11,
2,"ECE","EC2201",30,0,"Arjun Reddy",1,3,"Engineering Economics",7,
2,"ECE","EC2201",30,0,"Anjali Kumar",2,3,"Engineering Economics",8,
2,"ECE","EC2202",30,0,"Meenakshi Rajan",1,2,"Entrepreneurship Development",12,
2,"ECE","EC2202",30,0,"Siddharth Iyer",2,2,"Entrepreneurship Development",11,
2,"ECE","EC2203",30,0,"Karthik Naidu",1,3,"Professional Ethics and Values",17,
2,"ECE","EC2203",30,0,"Shweta Menon",2,3,"Professional Ethics and Values",19,
3,"ECE","EC3201",30,0,"Ranjith Iyer",1,3,"Project Management",20,
3,"ECE","EC3201",30,0,"Deepa Balan",2,3,"Project Management",21,
3,"ECE","EC3202",30,0,"Anusha Naidu",1,2,"Intellectual Property Rights",23,
3,"ECE","EC3202",30,0,"Rahul Krishnan",2,2,"Intellectual Property Rights",12,
3,"ECE","EC3203",30,0,"Arjun Nair",1,2,"Technical Writing and Communication Skills",17,
3,"ECE","EC3203",30,0,"Shweta Kumar",2,2,"Technical Writing and Communication Skills",18,
4,"ECE","EC4201",30,0,"Divya Kumar",1,3,"Innovation and Creativity",13,
4,"ECE","EC4201",30,0,"Anjali Raghavan",2,3,"Innovation and Creativity",11,
4,"ECE","EC4202",30,0,"Shalini Menon",1,3,"Disaster Management and Preparedness",12,
4,"ECE","EC4202",30,0,"Krishna Menon",2,3,"Disaster Management and Preparedness",19,
4,"ECE","EC4203",30,0,"Vivek Nair",1,2,"Computer Organization and Architecture",8,
4,"ECE","EC4203",30,0,"Deepak Reddy",2,2,"Computer Organization and Architecture",4,
5,"ECE","EC5201",30,0,"Anjali Reddy",1,3,"Supply Chain Management",2,
5,"ECE","EC5201",30,0,"Rajesh Menon",2,3,"Supply Chain Management",8,
5,"ECE","EC5202",30,0,"Meera Menon",1,3,"Financial Management",10,
5,"ECE","EC5202",30,0,"Sneha Reddy",2,3,"Financial Management",12,
5,"ECE","EC5203",30,0,"Arjun Menon",1,3,"Digital Signal Processing",11,
5,"ECE","EC5203",30,0,"Vivek Krishnan",2,3,"Digital Signal Processing",12,
6,"ECE","EC6201",30,0,"Shweta Menon",1,3,"Operations Management",13,
6,"ECE","EC6201",30,0,"Arjun Reddy",2,3,"Operations Management",14,
6,"ECE","EC6202",30,0,"Deepa Balan",1,2,"Total Quality Management",15,
6,"ECE","EC6202",30,0,"Ranjith Iyer",2,2,"Total Quality Management",16,
6,"ECE","EC6203",30,0,"Shalini Raghavan",1,3,"Computer Networks",17,
6,"ECE","EC6203",30,0,"Karthik Naidu",2,3,"Computer Networks",18,
7,"ECE","EC7201",30,0,"Rahul Krishnan",1,3,"Negotiation and Conflict Management",19,
7,"ECE","EC7201",30,0,"Anusha Naidu",2,3,"Negotiation and Conflict Management",20,
7,"ECE","EC7202",30,0,"Siddharth Iyer",1,3,"Sustainable Development and Corporate Social Responsibility (CSR)",21,
7,"ECE","EC7202",30,0,"Anjali Kumar",2,3,"Sustainable Development and Corporate Social Responsibility (CSR)",22,
7,"ECE","EC7203",30,0,"Divya Menon",1,3,"Wireless Communication",24,
7,"ECE","EC7203",30,0,"Arjun Nair",2,3,"Wireless Communication",23,
8,"ECE","EC8201",30,0,"Shweta Kumar",1,2,"Risk Management",25,
8,"ECE","EC8201",30,0,"Meenakshi Rajan",2,2,"Risk Management",26,
8,"ECE","EC8202",30,0,"Shalini Menon",1,3,"Change Management",27,
8,"ECE","EC8202",30,0,"Deepak Menon",2,3,"Change Management",28,
8,"ECE","EC8203",30,0,"Shweta Menon",1,3,"Embedded Systems Design",1,
8,"ECE","EC8203",30,0,"Arjun Reddy",2,3,"Embedded Systems Design",2,
1,"EEE","EE1201",30,0,"Shalini Menon",1,3,"Introduction to Management",3,
1,"EEE","EE1201",30,0,"Deepak Reddy",2,3,"Introduction to Management",4,
1,"EEE","EE1202",30,0,"Anjali Nair",1,3,"Soft Skills Development",5,
1,"EEE","EE1202",30,0,"Vivek Krishnan",2,3,"Soft Skills Development",6,
1,"EEE","EE1203",30,0,"Divya Kumar",1,2,"Introduction to Management",7,
1,"EEE","EE1203",30,0,"Arjun Reddy",2,2,"Introduction to Management",8,
2,"EEE","EE2201",30,0,"Meenakshi Rajan",1,3,"Engineering Economics",9,
2,"EEE","EE2201",30,0,"Shweta Menon",2,3,"Engineering Economics",10,
2,"EEE","EE2202",30,0,"Anusha Naidu",1,3,"Entrepreneurship Development",11,
2,"EEE","EE2202",30,0,"Arjun Menon",2,3,"Entrepreneurship Development",12,
2,"EEE","EE2203",30,0,"Rahul Krishnan",1,3,"Film Appreciation",13,
2,"EEE","EE2203",30,0,"Divya Menon",2,3,"Film Appreciation",14,
3,"EEE","EE3201",30,0,"Siddharth Iyer",1,2,"Project Management",15,
3,"EEE","EE3201",30,0,"Shweta Kumar",2,2,"Project Management",16,
3,"EEE","EE3202",30,0,"Shalini Raghavan",1,2,"Intellectual Property Rights",17,
3,"EEE","EE3202",30,0,"Karthik Naidu",2,2,"Intellectual Property Rights",18,
3,"EEE","EE3203",30,0,"Deepa Balan",1,3,"Human resource and Development",19,
3,"EEE","EE3203",30,0,"Anjali Kumar",2,3,"Human resource and Development",20,
4,"EEE","EE4201",30,0,"Rajesh Menon",1,2,"Innovation and Creativity",12,
4,"EEE","EE4201",30,0,"Anjali Reddy",2,2,"Innovation and Creativity",3,
4,"EEE","EE4202",30,0,"Meera Menon",1,3,"Disaster Management and Preparedness",4,
4,"EEE","EE4202",30,0,"Vivek Nair",2,3,"Disaster Management and Preparedness",5,
4,"EEE","EE4203",30,0,"Krishna Menon",1,3,"Professional Ethics and Values",6,
4,"EEE","EE4203",30,0,"Sneha Reddy",2,3,"Professional Ethics and Values",7,
5,"EEE","EE5201",30,0,"Arjun Nair",1,3,"Supply Chain Management",8,
5,"EEE","EE5201",30,0,"Shalini Menon",2,3,"Supply Chain Management",9,
5,"EEE","EE5202",30,0,"Vivek Krishnan",1,2,"Financial Management",10,
5,"EEE","EE5202",30,0,"Ranjith Iyer",2,2,"Financial Management",11,
5,"EEE","EE5203",30,0,"Deepak Reddy",1,2,"Introduction to Humanities and Social Sciences",12,
5,"EEE","EE5203",30,0,"Anjali Nair",2,2,"Introduction to Humanities and Social Sciences",13,
6,"EEE","EE6201",30,0,"Shweta Menon",1,3,"Operations Management",14,
6,"EEE","EE6201",30,0,"Arjun Reddy",2,3,"Operations Management",15,
6,"EEE","EE6202",30,0,"Anusha Naidu",1,3,"Total Quality Management",17,
6,"EEE","EE6202",30,0,"Divya Kumar",2,3,"Total Quality Management",16,
6,"EEE","EE6203",30,0,"Shweta Kumar",1,3,"Foreign Language (French, German, Spanish)",11,
6,"EEE","EE6203",30,0,"Rahul Krishnan",2,3,"Foreign Language (French, German, Spanish)",11,
7,"EEE","EE7201",30,0,"Karthik Naidu",1,2,"Negotiation and Conflict Management",12,
7,"EEE","EE7201",30,0,"Anjali Kumar",2,2,"Negotiation and Conflict Management",13,
7,"EEE","EE7202",30,0,"Arjun Menon",1,3,"Sustainable Development and Corporate Social Responsibility (CSR)",12,
7,"EEE","EE7202",30,0,"Meenakshi Rajan",2,3,"Sustainable Development and Corporate Social Responsibility (CSR)",13,
7,"EEE","EE7203",30,0,"Shalini Raghavan",1,3,"Renewable Energy Integration",11,
7,"EEE","EE7203",30,0,"Siddharth Iyer",2,3,"Renewable Energy Integration",12,
8,"EEE","EE8201",30,0,"Deepa Balan",1,2,"Risk Management",5,
8,"EEE","EE8201",30,0,"Vivek Nair",2,2,"Risk Management",6,
8,"EEE","EE8202",30,0,"Anjali Reddy",1,3,"Change Management",6,
8,"EEE","EE8202",30,0,"Shalini Menon",2,3,"Change Management",4,
8,"EEE","EE8203",30,0,"Rajesh Menon",1,3,"High Voltage Engineering",7,
8,"EEE","EE8203",30,0,"Divya Menon",2,3,"High Voltage Engineering",6};

    size_t num_core_courses = sizeof(core_courses) / sizeof(core_courses[0]);
    size_t num_elec_courses = sizeof(elec_courses) / sizeof(elec_courses[0]);

    // Write the courses to the file
    if (fwrite(core_courses, sizeof(Course), num_core_courses, fp_core) != num_core_courses) {
        perror("Error writing to file");
        fclose(fp_core);
        exit(EXIT_FAILURE);
    }
    if (fwrite(elec_courses, sizeof(Course), num_elec_courses, fp_elec) != num_elec_courses) {
        perror("Error writing to file");
        fclose(fp_elec);
        exit(EXIT_FAILURE);
    }

    fclose(fp_core);
    fclose(fp_elec);
}

void readCoursesFromFile(void) {
    FILE *fp_core = fopen(file_core, "rb");
    if (fp_core== NULL) {
        perror("Error opening file for reading");
        exit(EXIT_FAILURE);
    }

    FILE *fp_elec = fopen(file_elec, "rb");
    if (fp_elec== NULL) {
        perror("Error opening file for reading");
        exit(EXIT_FAILURE);
    }

    int sem;
    char dept[10];
    printf("Enter the semester: ");
    scanf("%d", &sem);
    printf("Enter the Department: ");
    scanf("%s", dept);

    Course course_core;
    printf("Core Course Details:\n");
    printf("%-10s %-40s %-20s %-10s %s\n", "Code", "Name", "Faculty", "Seats", "Waitlist");

    while (fread(&course_core, sizeof(Course), 1, fp_core) == 1) 
    {
        if (course_core.sem==sem && strcmp(course_core.department, dept)==0)
            printf("%-10s %-40s %-20s %-10d  %d\n", course_core.course_code, course_core.course_name, course_core.faculty, course_core.available_seats, course_core.waitlist);
    }
    
    printf("\n Elective Course Details:\n");
    printf("%-10s %-40s %-20s %-10s %s\n", "Code", "Name", "Faculty", "Seats", "Waitlist");

    Course course_elec;
    while (fread(&course_elec, sizeof(Course), 1, fp_elec) == 1) 
    {
        if (course_elec.sem==sem && strcmp(course_elec.department, dept)==0)
            printf("%-10s %-40s %-20s %-10d  %d\n", course_elec.course_code, course_elec.course_name, course_elec.faculty, course_elec.available_seats, course_elec.waitlist);
    }

    fclose(fp_core);
    fclose(fp_elec);
}


void Course_Allotment(const char *faculty, const char *course_code, char type, int *Student_Id) 
{
    FILE *fp_core = fopen(file_core, "r+b");
    if (fp_core == NULL) {
        perror("Error opening core courses file for reading");
        exit(EXIT_FAILURE);
    }

    FILE *fp_elec = fopen(file_elec, "r+b");
    if (fp_elec == NULL) {
        perror("Error opening elective courses file for reading");
        exit(EXIT_FAILURE);
    }

    Course course;
    int found = 0;
    long position;

    if (type == 'C') {
        while (fread(&course, sizeof(Course), 1, fp_core) == 1) {
            if (course.available_seats != 0 && strcmp(course.faculty, faculty) == 0 && strcmp(course.course_code, course_code) == 0) {
                printf("\n========== CORE COURSE ALLOTMENT DETAILS ARE ===========\n\n");
                printf("\tCourse Status: Allotted with preferred faculty.\n\tCode: %s\n\tName: %s\n\tFaculty: %s\n\tAvailable Seats: %d\n\tWaitlist: %d\n\tSection: %d\n\tCredits: %d\n", 
                       course.course_code, course.course_name, course.faculty, course.available_seats, course.waitlist, course.section, course.credit);

                
                char Std_name[50];
                StudentDetails name;
                FILE *userfile = fopen(filestudent, "rb");
                while (fread(&name, sizeof(StudentDetails), 1, userfile) == 1)
                {
                    if (name.student_id == *Student_Id)
                    {
                        strcpy(Std_name, name.name);
                        break;
                    }
                }
                FILE *fp_final = fopen(file_final, "ab");
                final_details final;
                final.student_id = *Student_Id;
                strcpy(final.student_name, Std_name);
                strcpy(final.course_code, course.course_code);
                strcpy(final.course_name, course.course_name);
                strcpy(final.faculty, course.faculty);
                strcpy(final.status, "Confirmed Allotment");

                fwrite(&final, sizeof(final_details), 1, fp_final);
                fclose(fp_final);

                course.available_seats -= 1;
                position = ftell(fp_core) - sizeof(Course);
                fseek(fp_core, position, SEEK_SET);
                fwrite(&course, sizeof(Course), 1, fp_core);
                found = 1;
                printf("\n");
                break;
            }
        }

        if (!found) {
            printf("Course with code %s taught by %s not available.\n", course_code, faculty);
            printf("\nIn different Faculty's section want to enroll (1 for Yes and 0 for No): ");
            int choice;
            scanf("%d", &choice);
            if (choice == 1) {
                fseek(fp_core, 0, SEEK_SET);  // Reset file pointer to beginning
                while (fread(&course, sizeof(Course), 1, fp_core) == 1) {
                    if (course.available_seats != 0 && strcmp(course.course_code, course_code) == 0) {
                        printf("\n========== CORE COURSE ALLOTMENT DETAILS ARE ===========\n");
                        printf("\tCourse Status: Allotted with different faculty.\n\tCode: %s\n\tName: %s\n\tFaculty: %s\n\tAvailable Seats: %d\n\tWaitlist: %d\n\tSection: %d\n\tCredits: %d\n", 
                               course.course_code, course.course_name, course.faculty, course.available_seats, course.waitlist, course.section, course.credit);

                        char Std_name[50];
                        StudentDetails name;
                        FILE *userfile = fopen(filestudent, "rb");
                        while (fread(&name, sizeof(StudentDetails), 1, userfile) == 1)
                        {
                            if (name.student_id == *Student_Id)
                            {
                                strcpy(Std_name, name.name);
                                break;
                            }
                        }
                        FILE *fp_final = fopen(file_final, "ab");
                        final_details final;
                        final.student_id = *Student_Id;
                        strcpy(final.student_name, Std_name);
                        strcpy(final.course_code, course.course_code);
                        strcpy(final.course_name, course.course_name);
                        strcpy(final.faculty, course.faculty);
                        strcpy(final.status, "Confirmed Allotment");

                        fwrite(&final, sizeof(final_details), 1, fp_final);
                        fclose(fp_final);

                        course.available_seats -= 1;
                        position = ftell(fp_core) - sizeof(Course);
                        fseek(fp_core, position, SEEK_SET);
                        fwrite(&course, sizeof(Course), 1, fp_core);
                        found = 1;
                        break;
                    }
                }
            } else {
                readCoursesFromFile();
                printf("\n1. Want to go for Alternate Course registration.\n");
                printf("2. Want to go for Waitlist.\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);
                if (choice == 1) {
                    add_courses(Student_Id);
                } else if (choice == 2) {
                    writeWaitlist(Student_Id, faculty, course_code, 'C');
                }
            }
        }
    } else if (type == 'E') {
        while (fread(&course, sizeof(Course), 1, fp_elec) == 1) {
            if (course.available_seats != 0 && strcmp(course.faculty, faculty) == 0 && strcmp(course.course_code, course_code) == 0) {
                printf("\n========== ELECTIVE COURSE ALLOTMENT DETAILS ARE ===========\n\n");
                printf("\tCourse Status: Allotted with preferred faculty.\n\tCode: %s\n\tName: %s\n\tFaculty: %s\n\tAvailable Seats: %d\n\tWaitlist: %d\n\tSection: %d\n\tCredits: %d\n", 
                       course.course_code, course.course_name, course.faculty, course.available_seats, course.waitlist, course.section, course.credit);

                char Std_name[50];
                StudentDetails name;
                FILE *userfile = fopen(filestudent, "rb");
                while (fread(&name, sizeof(StudentDetails), 1, userfile) == 1)
                {
                    if (name.student_id == *Student_Id)
                    {
                        strcpy(Std_name, name.name);
                        break;
                    }
                }
                FILE *fp_final = fopen(file_final, "ab");
                final_details final;
                final.student_id = *Student_Id;
                strcpy(final.student_name, Std_name);
                strcpy(final.course_code, course.course_code);
                strcpy(final.course_name, course.course_name);
                strcpy(final.faculty, course.faculty);
                strcpy(final.status, "Confirmed Allotment");

                fwrite(&final, sizeof(final_details), 1, fp_final);
                fclose(fp_final);

                course.available_seats -= 1;
                position = ftell(fp_elec) - sizeof(Course);
                fseek(fp_elec, position, SEEK_SET);
                fwrite(&course, sizeof(Course), 1, fp_elec);
                found = 1;
                printf("\n");
                break;
            }
        }

        if (!found) {
            printf("Elective Course with code %s taught by %s not available.\n", course_code, faculty);
            printf("\nIn different Faculty's section want to enroll (1 for Yes and 0 for No): ");
            int choice;
            scanf("%d", &choice);
            if (choice == 1) {
                fseek(fp_elec, 0, SEEK_SET);  // Reset file pointer to beginning
                while (fread(&course, sizeof(Course), 1, fp_elec) == 1) {
                    if (course.available_seats != 0 && strcmp(course.course_code, course_code) == 0) {
                        printf("\n========== ELECTIVE COURSE ALLOTMENT DETAILS ARE ===========\n");
                        printf("\tCourse Status: Allotted with different faculty.\n\tCode: %s\n\tName: %s\n\tFaculty: %s\n\tAvailable Seats: %d\n\tWaitlist: %d\n\tSection: %d\n\tCredits: %d\n", 
                               course.course_code, course.course_name, course.faculty, course.available_seats, course.waitlist, course.section, course.credit);

                        char Std_name[50];
                        StudentDetails name;
                        FILE *userfile = fopen(filestudent, "rb");
                        while (fread(&name, sizeof(StudentDetails), 1, userfile) == 1)
                        {
                            if (name.student_id == *Student_Id)
                            {
                                strcpy(Std_name, name.name);
                                break;
                            }
                        }
                        FILE *fp_final = fopen(file_final, "ab");
                        final_details final;
                        final.student_id = *Student_Id;
                        strcpy(final.student_name, Std_name);
                        strcpy(final.course_code, course.course_code);
                        strcpy(final.course_name, course.course_name);
                        strcpy(final.faculty, course.faculty);
                        strcpy(final.status, "Confirmed Allotment");

                        fwrite(&final, sizeof(final_details), 1, fp_final);
                        fclose(fp_final);

                        course.available_seats -= 1;
                        position = ftell(fp_elec) - sizeof(Course);
                        fseek(fp_elec, position, SEEK_SET);
                        fwrite(&course, sizeof(Course), 1, fp_elec);
                        found = 1;
                        break;
                    }
                }
            } else {
                readCoursesFromFile();
                printf("\n1. Want to go for Alternate Course registration.\n");
                printf("2. Want to go for Waitlist.\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);
                if (choice == 1) {
                    add_courses(Student_Id);
                } 
                else if (choice == 2) 
                {
                    writeWaitlist(Student_Id, faculty, course_code, 'E');
                }
            }
        }
    }

    fclose(fp_core);
    fclose(fp_elec);

    // Update student's allotment status
    if (found) {
        StudentDetails reg;
        FILE *userfile = fopen(filestudent, "r+b");
        if (userfile == NULL) {
            perror("Error opening student file for reading");
            exit(EXIT_FAILURE);

        }

        while (fread(&reg, sizeof(StudentDetails), 1, userfile) == 1) {
            if (reg.student_id == *Student_Id) {
                strcpy(reg.allotment_status, "YES");
                position = ftell(userfile) - sizeof(StudentDetails);
                fseek(userfile, position, SEEK_SET);
                fwrite(&reg, sizeof(StudentDetails), 1, userfile);
                break;
            }
        }
        fclose(userfile);
    }
}


void Allotment_Process(int *Student_Id)
{
        // Search for a specific course by faculty and course code
    char faculty[50];
    char course_code[10];
    
    StudentDetails reg;
    char status[3], waitlist[3];
    FILE *userfile = fopen(filestudent, "r+b");
    while (fread(&reg, sizeof(StudentDetails), 1, userfile) == 1)
    {
        if (reg.student_id == *Student_Id)
        {
            strcpy(status, reg.allotment_status);
            strcpy(waitlist, reg.waitlist);
            break;
        }
    }

    FILE *selected_core = fopen("core_selected.bin", "rb");
    FILE *selected_elective = fopen("elective_selected.bin", "rb");
    int record_countcore = fread(&core_courses_read, sizeof(struct core_selectedcourses), MAX_CORE_COURSES, selected_core);
    for (int i=0; i<record_countcore; i++)
    {
        if (core_courses_read[i].student_id == *Student_Id && strcmp(status, "YES")!=0 && strcmp(waitlist, "YES")!=0)
        {
            strcpy(course_code, core_courses_read[i].course_code);
            strcpy(faculty, core_courses_read[i].faculty_name);
            Course_Allotment(faculty, course_code,'C', Student_Id);
        }
    }
    
    int record_countelec = fread(&elec_courses_read, sizeof(struct elective_selectedcourses), MAX_ELEC_COURSES, selected_elective);
    for (int i=0; i< record_countelec; i++)
    {
        if (elec_courses_read[i].student_id == *Student_Id && strcmp(status, "YES")!=0 && strcmp(waitlist, "YES")!=0)
        {
            strcpy(course_code, elec_courses_read[i].course_code);
            strcpy(faculty, elec_courses_read[i].faculty_name);
            Course_Allotment(faculty, course_code, 'E', Student_Id);
        }
    }
    if (strcmp(status, "YES")==0)
    {
        printf("\nCourses has already been alloted to you! :) \n");
    }
    else if (strcmp(waitlist, "YES")==0)
    {
        printf("\nCourse is in waitlist. Better wait for the result! \n");
    }
    fclose(selected_core);
    fclose(selected_elective);

    
}

void WriteStudentDetails(void)
{
    FILE *fp = fopen(filestudent, "wb");
        if (fp == NULL) {
            perror("Error opening file for writing");
            exit(EXIT_FAILURE);
        }

        // Sample course records
        StudentDetails students[] = {
            "user001","pass001",101,"Lucas Brown",0,"NO","NO",
            "user002","pass002",102,"Olivia Smith",0,"NO","NO",
            "user003","pass003",103,"James Johnson",0,"NO","NO",
            "user004","pass004",104,"Emma Williams",0,"NO","NO",
            "user005","pass005",105,"Liam Jones",0,"NO","NO",
            "user006","pass006",106,"Ava Miller",0,"NO","NO",
            "user007","pass007",107,"Noah Davis",0,"NO","NO",
            "user008","pass008",108,"Isabella Garcia",0,"NO","NO",
            "user009","pass009",109,"Elijah Martinez",0,"NO","NO",
            "user010","pass010",110,"Sophia Hernandez",0,"NO","NO",
            "user011","pass011",111,"Benjamin Wilson",0,"NO","NO",
            "user012","pass012",112,"Mia Anderson",0,"NO","NO",
            "user013","pass013",113,"Lucas Thomas",0,"NO","NO",
            "user014","pass014",114,"Evelyn Taylor",0,"NO","NO",
            "user015","pass015",115,"Logan Moore",0,"NO","NO",
            "user016","pass016",116,"Harper Jackson",0,"NO","NO",
            "user017","pass017",117,"Jackson Martin",0,"NO","NO",
            "user018","pass018",118,"Abigail Lee",0,"NO","NO",
            "user019","pass019",119,"Jacob Perez",0,"NO","NO",
            "user020","pass020",120,"Amelia White",0,"NO","NO",
            "user021","pass021",121,"Ethan Harris",0,"NO","NO",
            "user022","pass022",122,"Charlotte Clark",0,"NO","NO",
            "user023","pass023",123,"Mason Lewis",0,"NO","NO",
            "user024","pass024",124,"Ella Robinson",0,"NO","NO",
            "user025","pass025",125,"Caden Walker",0,"NO","NO",
            "user026","pass026",126,"Aubrey Hall",0,"NO","NO",
            "user027","pass027",127,"Grayson Allen",0,"NO","NO",
            "user028","pass028",128,"Zoe Young",0,"NO","NO",
            "user029","pass029",129,"Carson Hernandez",0,"NO","NO",
            "user030","pass030",130,"Aria King",0,"NO","NO",
            "user031","pass031",131,"Dylan Wright",0,"NO","NO",
            "user032","pass032",132,"Hazel Scott",0,"NO","NO",
            "user033","pass033",133,"Luke Green",0,"NO","NO",
            "user034","pass034",134,"Lillian Adams",0,"NO","NO",
            "user035","pass035",135,"Henry Baker",0,"NO","NO",
            "user036","pass036",136,"Ellie Gonzalez",0,"NO","NO",
            "user037","pass037",137,"Samuel Nelson",0,"NO","NO",
            "user038","pass038",138,"Layla Carter",0,"NO","NO",
            "user039","pass039",139,"David Mitchell",0,"NO","NO",
            "user040","pass040",140,"Aiden Perez",0,"NO","NO",
            "user041","pass041",141,"Nora Roberts",0,"NO","NO",
            "user042","pass042",142,"Wyatt Turner",0,"NO","NO",
            "user043","pass043",143,"Riley Phillips",0,"NO","NO",
            "user044","pass044",144,"John Campbell",0,"NO","NO",
            "user045","pass045",145,"Lucy Parker",0,"NO","NO",
            "user046","pass046",146,"Thomas Evans",0,"NO","NO",
            "user047","pass047",147,"Grace Edwards",0,"NO","NO",
            "user048","pass048",148,"Caleb Collins",0,"NO","NO",
            "user049","pass049",149,"Violet Stewart",0,"NO","NO",
            "user050","pass050",150,"Ryan Sanchez",0,"NO","NO",
            "user051","pass051",151,"Zoey Morris",0,"NO","NO",
            "user052","pass052",152,"Jaxon Rogers",0,"NO","NO",
            "user053","pass053",153,"Stella Reed",0,"NO","NO",
            "user054","pass054",154,"Asher Cook",0,"NO","NO",
            "user055","pass055",155,"Lila Morgan",0,"NO","NO"  };

        size_t num_students = sizeof(students) / sizeof(students[0]);

        // Write the courses to the file
        if (fwrite(students, sizeof(StudentDetails), num_students, fp) != num_students) {
            perror("Error writing to file");
            fclose(fp);
            exit(EXIT_FAILURE);
        }

        fclose(fp);
}


void Display_Waitlist(void)
{
    FILE *fp_wait = fopen(file_waitlist, "rb");
    if (fp_wait == NULL) {
        perror("Error opening file for reading");
        exit(EXIT_FAILURE);
    }
    
    Waitlist waitlists;
    printf("Waitlist Details:\n");
    printf("%-20s %-20s %-20s %-20s %-10s %s\n", "Student Id", "Name", "Course Code", "Faculty", "CGPA", "Backlog");

    while (fread(&waitlists, sizeof(Waitlist), 1, fp_wait) == 1) 
    {
        // Display the HasBacklog as "Yes" or "No"
        const char *backlogStatus = waitlists.HasBacklog ? "Yes" : "No";
        
        printf("%-20d %-20s %-20s %-20s %-10.2f %s\n", 
               waitlists.student_id, 
               waitlists.name, 
               waitlists.course_code, 
               waitlists.faculty, 
               waitlists.CGPA, 
               backlogStatus);
    }
    
    fclose(fp_wait);
}



void writeWaitlist(int *Student_Id, const char *faculty, const char *course_code, char type) {
    StudentDetails reg;
    long position;
    FILE *userfile = fopen(filestudent, "r+b");
    if (userfile == NULL) {
        perror("Error opening student file");
        exit(EXIT_FAILURE);
    }

    char name[50];
    int studentFound = 0;
    while (fread(&reg, sizeof(StudentDetails), 1, userfile) == 1) {
        if (reg.student_id == *Student_Id) {
            strcpy(name, reg.name);
            studentFound = 1;
            break;
        }
    }

    if (!studentFound) {
        printf("Student with ID %d not found.\n", *Student_Id);
        fclose(userfile);
        return;
    }

    FILE *fp_waitlist = fopen(file_waitlist, "a+b");
    if (fp_waitlist == NULL) {
        perror("Error opening waitlist file");
        fclose(userfile);
        exit(EXIT_FAILURE);
    }

    FILE *fp_backlog = fopen("backlog_courses.bin", "w+b");
    if (fp_backlog == NULL) {
        perror("Error opening backlog file");
        fclose(userfile);
        fclose(fp_waitlist);
        exit(EXIT_FAILURE);
    }

    struct backlog_courses backlog;
    int back = 0;
    while (fread(&backlog, sizeof(struct backlog_courses), 1 , fp_backlog) == 1) {
        if (backlog.student_id == *Student_Id) {
            back = 1;
            break;
        }
    }

    float CGPA;
    printf("\nEnter your last semester CGPA: ");
    scanf("%f", &CGPA);

    Waitlist waitlist;
    waitlist.student_id = *Student_Id;
    strncpy(waitlist.name, name, sizeof(waitlist.name) - 1);
    waitlist.name[sizeof(waitlist.name) - 1] = '\0';
    strncpy(waitlist.course_code, course_code, sizeof(waitlist.course_code) - 1);
    waitlist.course_code[sizeof(waitlist.course_code) - 1] = '\0';
    strncpy(waitlist.faculty, faculty, sizeof(waitlist.faculty) - 1);
    waitlist.faculty[sizeof(waitlist.faculty) - 1] = '\0';
    waitlist.CGPA = CGPA;
    waitlist.HasBacklog = back;
    if (type == 'C') {
        strcpy(waitlist.course_type, "Core");
    } else {
        strcpy(waitlist.course_type, "Elective");
    }

    if (fwrite(&waitlist, sizeof(Waitlist), 1, fp_waitlist) != 1) {
        perror("Error writing to waitlist file");
    }
    fclose(fp_waitlist);
    fclose(fp_backlog);

    FILE *fp_core = fopen(file_core, "r+b");
    if (fp_core == NULL) {
        perror("Error opening core courses file for reading");
        fclose(userfile);
        exit(EXIT_FAILURE);
    }

    FILE *fp_elec = fopen(file_elec, "r+b");
    if (fp_elec == NULL) {
        perror("Error opening elective courses file for reading");
        fclose(userfile);
        fclose(fp_core);
        exit(EXIT_FAILURE);
    }

    Course course;
    int found = 0;
    FILE *fp_final = fopen(file_final, "ab");
    if (fp_final == NULL) {
        perror("Error opening final file for writing");
        fclose(userfile);
        fclose(fp_core);
        fclose(fp_elec);
        exit(EXIT_FAILURE);
    }

    if (type == 'C') {
        while (fread(&course, sizeof(Course), 1, fp_core) == 1) {
            if (strcmp(course.faculty, faculty) == 0 && strcmp(course.course_code, course_code) == 0) 
            {
                final_details final;
                final.student_id = *Student_Id;
                strncpy(final.student_name, name, sizeof(final.student_name) - 1);
                final.student_name[sizeof(final.student_name) - 1] = '\0';
                strncpy(final.course_code, course.course_code, sizeof(final.course_code) - 1);
                final.course_code[sizeof(final.course_code) - 1] = '\0';
                strncpy(final.course_name, course.course_name, sizeof(final.course_name) - 1);
                final.course_name[sizeof(final.course_name) - 1] = '\0';
                strncpy(final.faculty, course.faculty, sizeof(final.faculty) - 1);
                final.faculty[sizeof(final.faculty) - 1] = '\0';
                strcpy(final.status, "Waitlist");

                if (fwrite(&final, sizeof(final_details), 1, fp_final) != 1) {
                    perror("Error writing to final file");
                }

                course.waitlist += 1;
                position = ftell(fp_core) - sizeof(Course);
                fseek(fp_core, position, SEEK_SET);
                if (fwrite(&course, sizeof(Course), 1, fp_core) != 1) {
                    perror("Error updating core course");
                }
                found = 1;
                break;
            }
        }
    } else if (type == 'E') {
        while (fread(&course, sizeof(Course), 1, fp_elec) == 1) {
            if (strcmp(course.faculty, faculty) == 0 && strcmp(course.course_code, course_code) == 0) {
                final_details final;
                final.student_id = *Student_Id;
                strncpy(final.student_name, name, sizeof(final.student_name) - 1);
                final.student_name[sizeof(final.student_name) - 1] = '\0';
                strncpy(final.course_code, course.course_code, sizeof(final.course_code) - 1);
                final.course_code[sizeof(final.course_code) - 1] = '\0';
                strncpy(final.course_name, course.course_name, sizeof(final.course_name) - 1);
                final.course_name[sizeof(final.course_name) - 1] = '\0';
                strncpy(final.faculty, course.faculty, sizeof(final.faculty) - 1);
                final.faculty[sizeof(final.faculty) - 1] = '\0';
                strcpy(final.status, "Waitlist");

                if (fwrite(&final, sizeof(final_details), 1, fp_final) != 1) {
                    perror("Error writing to final file");
                }

                course.waitlist += 1;
                position = ftell(fp_elec) - sizeof(Course);
                fseek(fp_elec, position, SEEK_SET);
                if (fwrite(&course, sizeof(Course), 1, fp_elec) != 1) {
                    perror("Error updating elective course");
                }
                found = 1;
                break;
            }
        }
    }

    fclose(fp_final);
    fclose(fp_core);
    fclose(fp_elec);

   
        // Reset file pointer to start for rewriting
        fseek(userfile, 0, SEEK_SET);
        while (fread(&reg, sizeof(StudentDetails), 1, userfile) == 1) {
            if (reg.student_id == *Student_Id) {
                strcpy(reg.waitlist, "YES");
                position = ftell(userfile) - sizeof(StudentDetails);
                fseek(userfile, position, SEEK_SET);
                if (fwrite(&reg, sizeof(StudentDetails), 1, userfile) != 1) {
                    perror("Error updating student details");
                }
                break;
            }
        
    }

    fclose(userfile);
}


void Display_waitlist_result(int *Student_Id)
{
    manageWaitlist();
    char code[10], faculty[50];
    FILE *waitlist = fopen(file_waitlist, "rb");
    if (waitlist == NULL) {
        perror("Error opening waitlist file");
        exit(EXIT_FAILURE);
    }

    Waitlist wait;
    while (fread(&wait, sizeof(Waitlist), 1, waitlist) == 1) 
    {
        if (wait.student_id == *Student_Id)  // Removed misplaced semicolon
        {
            strcpy(code, wait.course_code);
            strcpy(faculty, wait.faculty);
            int status = checkWaitlistStatus(Student_Id, code, faculty);  // Pass the address of Student_Id

            if (status) 
            {
                printf("\nCongratulations! Waitlist Student with id %d, course code %s and faculty %s is confirmed for the course.\n", *Student_Id, code, faculty);
                
                FILE *fp_final = fopen(file_final, "r+b");
                if (fp_final == NULL) {
                    perror("Error opening final file");
                    exit(EXIT_FAILURE);
                }

                final_details final[100];
                int record_count = fread(final, sizeof(final_details), 100, fp_final);
                if (record_count == 0 && ferror(fp_final)) {
                    perror("Error reading final file");
                    fclose(fp_final);
                    exit(EXIT_FAILURE);
                }

                int found = 0;
                for (int i = 0; i < record_count; ++i) {
                    if (final[i].student_id == *Student_Id && strcmp(final[i].course_code, code) == 0 && strcmp(final[i].faculty, faculty) == 0) {
                        strcpy(final[i].status, "Confirmed Allotment");
                        printf("Status Updated successfully.\n");
                        found = 1;

                        // Move the file pointer to the correct position for writing
                        fseek(fp_final, i * sizeof(final_details), SEEK_SET);
                        if (fwrite(&final[i], sizeof(final_details), 1, fp_final) != 1) {
                            perror("Error writing to final file");
                            fclose(fp_final);
                            exit(EXIT_FAILURE);
                        }
                        break;
                    }
                }

                if (!found) {
                    printf("Record not found.\n");
                }

                fclose(fp_final);

                
            }
            else 
            {
                printf("Sorry! Waitlist Student with id %d, course code %s and faculty %s is not confirmed for the course.\n", *Student_Id, code, faculty);
            }
        
        }
    }
    fclose(waitlist);
}


void manageWaitlist(void) 
{
    FILE *fp_waitlist = fopen(file_waitlist, "rb");
    if (!fp_waitlist) {
        perror("Failed to open waitlist file");
        exit(EXIT_FAILURE);
    }

    Waitlist waitlist[100]; // Assuming max 100 waitlist entries for simplicity
    int count = 0;

    // Read all waitlist records
    while (fread(&waitlist[count], sizeof(Waitlist), 1, fp_waitlist) == 1) {
        count++;
    }
    fclose(fp_waitlist);

    // Process each unique course_code and faculty combination
    for (int i = 0; i < count; i++) {
        if (waitlist[i].student_id == -1) continue; // Already processed

        // Collect all entries for this course and faculty
        Waitlist temp[100];
        int temp_count = 0;
        for (int j = 0; j < count; j++) {
            if (strcmp(waitlist[i].course_code, waitlist[j].course_code) == 0 &&
                strcmp(waitlist[i].faculty, waitlist[j].faculty) == 0) {
                temp[temp_count++] = waitlist[j];
                waitlist[j].student_id = -1; // Mark as processed
            }
        }

        // Sort by CGPA (descending) and HasBacklog (ascending)
        for (int m = 0; m < temp_count - 1; m++) {
            for (int n = 0; n < temp_count - m - 1; n++) {
                if (temp[n].CGPA < temp[n + 1].CGPA ||
                    (temp[n].CGPA == temp[n + 1].CGPA && temp[n].HasBacklog > temp[n + 1].HasBacklog)) {
                    Waitlist temp_swap = temp[n];
                    temp[n] = temp[n + 1];
                    temp[n + 1] = temp_swap;
                }
            }
        }

        // Write the top 3 to a new confirmed waitlist file
        FILE *fp_confirmed = fopen("ConfirmedWaitlist.bin", "ab");
        if (!fp_confirmed) {
            perror("Failed to open confirmed waitlist file");
            exit(EXIT_FAILURE);
        }

        for (int k = 0; k < 3 && k < temp_count; k++)
        {
            FILE *fp_core = fopen(file_core, "r+b");
            FILE *fp_elec = fopen(file_elec, "r+b");
            Course course;
            while (fread(&course, sizeof(Course), 1, fp_elec) == 1) 
            {
            if (course.available_seats != 0 && strcmp(course.faculty, temp[k].faculty) == 0 && strcmp(course.course_code, temp[k].course_code) == 0)
                fwrite(&temp[k], sizeof(Waitlist), 1, fp_confirmed);
            }
        fclose(fp_confirmed);
        }
    }
}


int checkWaitlistStatus(int *Student_Id, const char *course_code, const char *faculty) {
    FILE *fp_confirmed = fopen("ConfirmedWaitlist.bin", "rb");
    if (!fp_confirmed) {
        perror("Failed to open confirmed waitlist file");
        exit(EXIT_FAILURE);
    }

    Waitlist waitlist;
    while (fread(&waitlist, sizeof(Waitlist), 1, fp_confirmed) == 1) {
        if (waitlist.student_id == *Student_Id &&
            strcmp(waitlist.course_code, course_code) == 0 &&
            strcmp(waitlist.faculty, faculty) == 0) {
            fclose(fp_confirmed);
            return 1; // Found in the confirmed waitlist
        }
    }
    fclose(fp_confirmed);
    return 0; // Not found
}

void Display_Allotment(void)
{
    FILE *fp_final = fopen(file_final, "rb");
    final_details final;
    printf("Allotment Details:\n");
    printf("%-20s %-20s %-20s %-45s %-30s %s\n", "Student Id", "Name", "Course Code", "Course Name", "Faculty", "Status");

    while (fread(&final, sizeof(final_details), 1, fp_final) == 1) 
    {
        printf("%-20d %-20s %-20s %-45s %-30s %s\n", 
               final.student_id, 
               final.student_name, 
               final.course_code, 
               final.course_name,
               final.faculty,
               final.status); 
    }
    fclose(fp_final);
}

int has_completed_prerequisite(int *Student_Id, char *course_code) {
    FILE *completed_courses = fopen("completed_courses.bin", "rb");
    if (completed_courses == NULL) {
        printf("Error opening completed courses file.\n");
        return 0;
    }

    struct completed_course completed_course;
    while (fread(&completed_course, sizeof(struct completed_course), 1, completed_courses) == 1) {
        if (completed_course.student_id == *Student_Id && strcmp(completed_course.course_code, course_code) == 0) {
            fclose(completed_courses);
            return 1;
        }
    }

    fclose(completed_courses);
    return 0;
}

int check_prerequisites(int *Student_Id, char *course_code) 
{
    for (int i = 0; i < MAX_PREREQUISITES; i++) 
    {
        if (strcmp(prerequisites[i].course_code, course_code) == 0) {
            if (!has_completed_prerequisite(Student_Id, prerequisites[i].prerequisite_code)) {
                printf("Student has not completed the prerequisite: %s\n", prerequisites[i].prerequisite_code);
                return 0;
            }
        }
    }
    return 1;
}


void add_completed_course(char *course_code, int *Student_Id) {
    FILE *completed_file = fopen("completed_courses.bin", "ab+");
    if (completed_file == NULL) {
        printf("Error opening completed courses file.\n");
        return;
    }

    struct completed_course new_course;
    strcpy(new_course.course_code, course_code);
    new_course.student_id = *Student_Id;

    fwrite(&new_course, sizeof(struct completed_course), 1, completed_file);
    fclose(completed_file);

    printf("Completed course added successfully.\n");
}

