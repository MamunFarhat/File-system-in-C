#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

// ---------------------------------------------------
// 1. STRUCTURE
// ---------------------------------------------------
struct Student {
    int roll;
    char name[100];
};

void writeData(const char *filename);
void readData(const char *file1, const char *file2);
void sortData(const char *filename);
void swap(struct Student *a, struct Student *b); 
void transData(const char *srcFile, char *destFile);
void createFile(char *destFile);
void delFile(char *file1, char *file2);
void delData(const char *file1, const char *file2);
void delperData(const char *file1, const char *file2);

// ---------------------------------------------------
// 2. FUNCTION IMPLEMENTATIONS
// ---------------------------------------------------
// Made swapping the universal function for sorting
void swap(struct Student *a, struct Student *b) {
    struct Student temp = *a;
    *a = *b;
    *b = temp;
}

void writeData(const char *filename) {
    // [Safety Check] Ensure filename is not empty
    if (strlen(filename) == 0) {
        printf("Error: No filename selected.\n");
        return;
    }

    FILE *fp = fopen(filename, "a");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    int r;
    char na[100];

    // [Integer Protection] Loop until valid integer is entered
    while (1) {
        printf("Enter roll no: ");
        if(scanf("%d", &r) != 1) {
            printf("Invalid input! Only enter integer values.\n");
            while(getchar() != '\n');
        } else {
            getchar(); // Consume newline after valid integer
            break;     // Exit loop when valid integer is entered
        }
    }

    printf("Enter name: ");
    fgets(na, sizeof(na), stdin);
    na[strcspn(na, "\n")] = 0; 

    fprintf(fp, "Roll no: %d\tName: %s\n", r, na);
    printf("Data written successfully to %s.\n", filename);
    
    fclose(fp);
}

void readData(const char *file1, const char *file2) {
    char choiceName[100];
    FILE *fp;
    char c;

    printf("Available files: %s", file1);
    // Check strlen to see if file2 actually exists (is not empty) / ***filename is string***
    if (strlen(file2) > 0) {
        printf(", %s", file2);
    } 

    printf("\nEnter name of the file to view: ");
    scanf("%s", choiceName);

    fp = fopen(choiceName, "r");
    
    if (fp == NULL) {
        printf("Error: Could not open file '%s'.\n", choiceName);
        return;
    }

    printf("\n--- File Contents ---\n");
    c = fgetc(fp);
    while (c != EOF) {
        printf("%c", c);
        c = fgetc(fp);
    }
    printf("\n---------------------\n");
    
    fclose(fp);
}

void sortData(const char *filename) {
    if (strlen(filename) == 0) {
        printf("Error: No filename detected.\n");
        return;
    }

    struct Student students[100];
    int count = 0;
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("File not found or empty.\n");
        return;
    }

    while (fscanf(fp, "Roll no: %d\tName: %[^\n]\n", &students[count].roll, students[count].name) != EOF) {
        count++;
    }
    fclose(fp); 

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (students[j].roll > students[j+1].roll) {
                swap(&students[j], &students[j+1]);
            }
        }
    }

    fp = fopen(filename, "w"); 
    if (fp == NULL) {
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "Roll no: %d\tName: %s\n", students[i].roll, students[i].name);
    }
    fclose(fp);
    printf("File sorted successfully by Roll No.\n");
}

void createFile(char *destFile) {
    FILE *fp;
    // Use a temp variable to avoid corrupting data if fopen fails
    char tempName[100];
    
    printf("Enter the name for the new file (e.g., backup.txt): ");
    scanf("%s", tempName); 

    fp = fopen(tempName, "w"); 
    if (fp != NULL) {
        printf("File '%s' created successfully.\n", tempName);
        fclose(fp);
        strcpy(destFile, tempName); // copy the file-name string from the temp to the main variable
    } else {
        printf("Error creating file.\n");
    }
}

void transData(const char *srcFile, char *destFile) {
    char helper, ch;
    FILE *fpSrc, *fpDest;

    if (strlen(destFile) == 0) {
        printf("No destination file detected!\n");
        printf("Do you want to create it now? (y/n): ");
        scanf(" %c", &helper);
        if (helper == 'y') {
            createFile(destFile);
        } else {
            return;
        }
    }

    fpSrc = fopen(srcFile, "r");
    fpDest = fopen(destFile, "w");

    

    if (fpSrc == NULL || fpDest == NULL) {
        printf("Error opening files for transfer.\n");
        if(fpSrc) fclose(fpSrc);
        if(fpDest) fclose(fpDest);
        return;
    }

    printf("Do you want to change the case of the data in new file? (y/n): ");
    scanf(" %c", &helper);

    ch = fgetc(fpSrc);
    while (ch != EOF) {
        if (helper == 'y') {
            if (isupper(ch)) {
                ch = tolower(ch);
            } else if (islower(ch)) {
                ch = toupper(ch);
            } 
        }
        
        fputc(ch, fpDest);
        ch = fgetc(fpSrc);
    }

    printf("Data transferred from %s to %s successfully (Case Swapped).\n", srcFile, destFile);
    
    fclose(fpSrc);
    fclose(fpDest);
}

// Function to delete a file and clear its name from memory
void delFile(char *file1, char *file2) {
    char inputName[100]; // Temporary storage for user input

    printf("Available files: %s", file1);
    if (strlen(file2) > 0) {
        printf(", %s", file2);
    }
    
    printf("\nEnter the filename you want to delete: ");
    scanf("%s", inputName); // Read into TEMP variable, don't overwrite file1 yet!

    // Check if the file removed successfully from the disk
    if (remove(inputName) == 0) {
        printf("File '%s' deleted successfully.\n", inputName);
        
        // Logic to "Forget" the file name in program
        //compares if the entered file name is equal to the file1 or file2 '== 0' means equal
        if (strcmp(inputName, file1) == 0) { 
            strcpy(file1, ""); // Clear file1 string "" means a empty string
        } 
        else if (strcmp(inputName, file2) == 0) {
            strcpy(file2, ""); // Clear file2 string "" means a empty string
        }
    } else {
        printf("Error: File not found or could not be deleted.\n");
    }
}
void delData(const char *file1, const char *file2) {
    FILE *fp = NULL;
    char choiceName[100];
    
    printf("Available files: %s", file1);
    if (strlen(file2) > 0) {
        printf(", %s", file2);
    }
    
    printf("\nEnter the filename you want to delete data: ");
    scanf("%s", choiceName);
    
    fp = fopen(choiceName, "w");
    if (fp == NULL) {
        printf("Error: Could not open file '%s'.\n", choiceName);
        return;
    }
    fclose(fp);
    printf("\033[1;92mData deleted successfully from file '%s'.\033[0m\n", choiceName);
}
void delperData(const char *file1, const char *file2){
    FILE *fp = NULL;
    char choiceName[100];
    int count = 0;
    int rollToDelete; // separate variable for roll number to delete
    int found = 0; // Flag to check if student exists
    struct Student students[100];

    printf("Available files: %s", file1);
    if (strlen(file2) > 0) {
        printf(", %s", file2);
    }
    
    printf("\nEnter the filename you want to delete data from: ");
    scanf("%s", choiceName);
    
    fp = fopen(choiceName, "r");
    if (fp == NULL) {
        printf("Error: Could not open file '%s'.\n", choiceName);
        return;
    }
    
    // 1. file is written as "Roll no: %d\tName: %s\n" but was reading as "%d %s"
    // Changed to match the actual file format written by writeData() function
    while(fscanf(fp, "Roll no: %d\tName: %[^\n]\n", &students[count].roll, students[count].name) != EOF){
        count++;
    }
    fclose(fp);

    // 2. Check if file is empty before proceeding
    if (count == 0) {
        printf("No data found in file.\n");
        return;
    }

    printf("Which data you want to delete?\n");
    for(int i = 0; i < count; i++){
        printf("Roll no: %d\tName: %s\n", students[i].roll, students[i].name);
    }

    printf("Enter the roll no of the student you want to delete: ");
    scanf("%d", &rollToDelete); // 3. Use a separate variable instead of students[count].roll

    // 4. shift all elements after the deleted one
    for(int i = 0; i < count; i++){
        if(students[i].roll == rollToDelete){
            found = 1; // Mark if found the student
            // Shift all remaining students one position to the left
            for(int j = i; j < count - 1; j++){
                students[j].roll = students[j+1].roll;
                // 5. Can't copy the strings directly so, strcpy()
                strcpy(students[j].name, students[j+1].name);
            }
            count--; // Reduce total count after deletion
            break; // Exit after finding and deleting the student
        }
    }

    // 6. Validate if the roll number was actually found
    if (!found) {
        printf("Error: Student with roll no %d not found.\n", rollToDelete);
        return;
    }

    fp = fopen(choiceName, "w");
    if (fp == NULL) {
        printf("Error: Could not open file '%s'.\n", choiceName);
        return;
    }
    
    // 7. Write back in the same format as writeData() function 
    for(int i = 0; i < count; i++){ // Changed loop condition from count-1 to count
        fprintf(fp, "Roll no: %d\tName: %s\n", students[i].roll, students[i].name);
    }
    fclose(fp);
    printf("\033[1;92mData deleted successfully from file '%s'.\033[0m\n", choiceName);
}

// ---------------------------------------------------
// 3. MAIN FUNCTION
// ---------------------------------------------------

int main() {
    int choice;
    char help;
    FILE *fp = NULL;
    
    char filename1[100] = ""; 
    char filename2[100] = ""; 

    printf("Enter the filename you want to work with (e.g. data.txt): ");
    scanf("%s", filename1);

    fp = fopen(filename1, "r");
    if (fp == NULL) {
        printf("File does not exist. Create it? (y/n): ");
        scanf(" %c", &help);
        if (help == 'y') {
            fp = fopen(filename1, "w"); 
            if(fp) fclose(fp);          
        } else {
            exit(0);
        }
    } else {
        fclose(fp); 
    }

    printf("Welcome to the file editor using C!\n");

    do {
        printf("\n\033[1;94m--- Main Menu ---\033[0m\n");
        printf("Main File: %s | Secondary File: %s\n", filename1, filename2);
        printf("1. Add Student\n");
        printf("2. View Students\n");
        printf("3. Sort Students\n");
        printf("4. Create New File\n");
        printf("5. Transfer Data\n");
        printf("6. Delete specific data\n");
        printf("7. Delete Data from files\n");
        printf("8. Delete File\n");
        printf("9. Exit\n");
        printf("\033[1;93mEnter your choice: \033[0m");

        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n'); 
            printf("\033[1;91mInvalid input!\033[0m\n");
            continue;
        }

        switch (choice) {
            case 1:
                writeData(filename1);
                break;

            case 2:
                readData(filename1, filename2);
                break;

            case 3:
                sortData(filename1);
                break;

            case 4:
                createFile(filename2);
                break;
            
            case 5:
                transData(filename1, filename2);
                break;

            case 6:
                delperData(filename1, filename2);
                break;

            case 7:
                delData(filename1, filename2);
                break;

            case 8:
                // Pass both strings so the function can decide which one to clear
                delFile(filename1, filename2);
                
                // If the user deleted the MAIN file, force them to make a new one
                if (strlen(filename1) == 0) {
                    printf("\033[1;91mWARNING: You deleted the main file!\033[0m\n");
                    printf("Please enter a new main filename: ");
                    scanf("%s", filename1);
                    fp = fopen(filename1, "w"); // Create it immediately
                    if(fp) fclose(fp);
                }
                break;

            case 9:
                printf("\033[1;91mGoodbye!\033[0m\n");
                exit(0);
                break;

            default:
                printf("\033[1;91mInvalid choice.\033[0m\n");
        }
    } while (choice != 9);

    return 0;
}