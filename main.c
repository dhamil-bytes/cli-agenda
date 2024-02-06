#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

char* lowerStr(char*);
int listTasks(FILE*);


// List of commands
const char *commands[] = {"list", "add", "archive", "exit"};
int cal[] = {31, 28, 31, 30, 31, 30,31, 31, 30, 31, 30, 31};


// Deletes line from specified file
int deleteLine(int target) {
    int cpyT = target;
    int fond = 0;
    char line[250];

    // Base
    if (target == 0 || target < 0) {
        printf("No such task exists");
        return 0;
    }

    FILE* f = fopen("tasks.txt", "r");

    // Reading line by line
    while (fgets(line, sizeof(line), f)) {
        // Line found - task stored
        if (target == 0) {
            // Indicating task exists
            fond = 1;
            // Breaks
            break;
        }
        target--;
    }
    // Doesn't find anything
    if (fond != 1) {
        // Returns
        printf("No such task exists\n");
        fclose(f);
        return 0;
    }

    // Getting number of lines from file
    fseek(f, 0L, SEEK_END);
    int nLines = ((ftell(f))/40);
    // Going back to the beginning
    rewind(f);

    // Open a new file in write mode
    int curr = 0;
    char* car = "";
    // Beginning of file
    long whereRd = ftell(f);
    FILE* pt = fopen("temp.txt", "w");
    FILE* archive = fopen("archive.txt", "a");
    // Checking if archive is empty
    long arxE = ftell(archive)/40;


    while (curr < nLines) {
        // Reading from tasks.txt
        if (fread(&line, sizeof(char), 39, f) == 0) {
            // Reached EOF
            break;
        }
        car = strchr(line, '\n');
        if (car != NULL) {
            *strchr(line, '\n') = '\0';
        }
        // Don't do anything if on target line
        if (curr != cpyT) {
            // Adding a new line character if the current line of the file is not the last line of the file
            if (curr + 1 < nLines) {
                strcat(line, "\n");
                printf("adding to file -> [%s]", line);
                fwrite(line, sizeof(char), 40, pt);
                // Going backwards
//                fseek(pt, whereRd, SEEK_SET);
            }
            else {
                // Does not add new line if last line of file
                fwrite(line, sizeof(char), 39, pt);
            }
        }
        // If target is found, do not add to temp file. Add to archive
        else {
            // If the file is not empty, add the end of the file with a leading new line character
            if (arxE != 0) {
                char* aLine = "\n";
                strcat(aLine, line);
                printf("adding to archive -> [%s]", aLine);
                fwrite(aLine, sizeof(char), 40, archive);
            }
            // In the event the archive is empty, add line to file plainly
            else {
                // Adds target to archive
                fwrite(line, sizeof(char), 39, archive);
            }
        }
        curr++;
        // Changing file start position
        whereRd += 41;
        fseek(f, whereRd, SEEK_SET);
    }
    fclose(f);
    fclose(archive);
    fclose(pt);

    return 1;
}

// Returns string that shows how long you have until you need to submit your assignment
char* timeTill(struct tm due, struct tm today) {
    // Nweeks Mdays
    // Adjusting struct tm today
    today.tm_year = today.tm_year + 1900;
    today.tm_mon = today.tm_mon + 1;
    // Due (yr) - today (yr)
    // Convert to days
    long w1 = (due.tm_year%4==0 ? due.tm_year*365 : due.tm_year*364);
    long w2 = (today.tm_year%4==0 ? today.tm_year*365 : today.tm_year*364);


    // Calculating doy
    w1 += due.tm_mday;
    if (due.tm_year%4==0) {
        cal[1] += 1;
    }
    // Calculating doy with respect to leap year
    for (int i = 0; i < due.tm_mon - 1; i++) {
        w1 += cal[i];
    }
    // Current day of the year n/365
    if (due.tm_year%4==0) {
        cal[1] -= 1;
    }

    w2 += today.tm_mday;
    if (today.tm_year%4==0) {
        cal[1] += 1;
    }
    // Calculating doy with respect to leap year
    for (int i = 0; i < today.tm_mon - 1; i++) {
        w2 += cal[i];
    }
    // Current day of the year n/365
    if (today.tm_year%4==0) {
        cal[1] -= 1;
    }

    int weks = 0;
    int dys = w1-w2;
    // Converting to weeks
    if ((w1-w2)/7) {
        weks += dys/7;
        dys -= (dys/7)*7;
    }

    char* temp = (char*)malloc(sizeof(char));
    // Printing
    // 0 0
    if ((dys==0) && (weks==0)) {
        return "today!";
    }
    // 1 0
    else if ((dys>0) && (weks==0)) {
        sprintf(temp, "%dd", dys);
    }
    // 0 1
    else if ((dys==0) && (weks>0)) {
        sprintf(temp, "%dw", weks);
    }
    // -1 -1
    else if ((dys < 0) || (weks < 0)) {
        return "late!";
    }
    // 1 1
    else {
        sprintf(temp,"%dw %dd",weks, dys);
    }
    return temp;
}

// Adds a task to task list
void addTask(FILE* f) {
    char tName[50] = "";
    char tDue[50] = "";
    char tClass[50] = "";
    char temp[100] = "";
//    int day, month, year = 0;

    // Format strings
    char line[250] = "";
    char *timedue;

    printf("--- Class ---\n");
    printf("\tEnter course number and section [cs###-###]: ");
    scanf("%s", temp);
    sprintf(tClass, "cs%s", temp);
    strcpy(temp, "");

    printf("--- Assignment ---\n");
    printf("\tName of assignment: ");
    scanf("%s", tName);

    printf("--- Due Date --- \n");
    printf("\tEnter due date [mm-dd-yyyy]:");
    scanf("%s", tDue);
//    sscanf(tDue, "%d-%d-%d", &month, &day, &year);

    // Time till due

    sprintf(line, "%-12s|%-12s|%-12s|", tClass, tName, tDue);

    // open
    f = fopen("tasks.txt", "r");
    // File does not exist
    if (f == NULL) {
        fclose(f);
        f = fopen("tasks.txt", "w");
        fprintf(f, "%-12s|%-12s|%-12s|", "Course", "Assignment", "Date Due");
    }
    fclose(f);

    f = fopen("tasks.txt", "a");
    fprintf(f, "\n%s", line);

    fclose(f);

}

// Turning a string to lowercase
char* lowerStr(char* inp) {
    char* cpy = inp;
    for(;*cpy;++cpy) {
        *cpy = tolower((unsigned char) *cpy);
    }
    return inp;
}

// Lists tasks to be done
int listTasks(FILE* f) {
    char str[1000];
    char* newlin = "";
    char* date = (char*)malloc(sizeof(char)*200);
    int lin = 0;
    char* token;
    int day = 0; int month = 0; int yr = 0;
    time_t t;
    time(&t);
    struct tm currDate = *localtime(&t);

    f = fopen("tasks.txt", "r");

    // Opens file
    if (!f) {
        // Do nothing for now
        printf("No tasks yet!\n");
        fclose(f);
        return 0;
    }
    else {
        // Goes thru file
        while(fgets(str, 999, f) != NULL) {
            lin++;
            // Getting rid of newline character found by fgets
            newlin = strchr(str, '\n');
            if (newlin != NULL) {
                *strchr(str, '\n') = '\0';
            }
            // Prints each line of the .txt file DB
            if (lin == 1) {
                printf("%-6s|%-6s|%s", "ID", "Due", str);
            }
            else {
                // Getting due date from string
                strncpy(date, str, sizeof(char)*200);
                // Breaking apart by '|' char
                token = strtok(date, "|");
                token = strtok(NULL, "|");
                token = strtok(NULL, "|");

                // Making cpy
                strcpy(date, token);
                // Storing value
                sscanf(date, "%d-%d-%d", &month, &day, &yr);
                struct tm dueDate = {59,59,23,day, month, yr, 0, 0, -1};
                // Dynamic due date tick
                printf("\n %-6d|%-6s|%s", lin-1, timeTill(dueDate, currDate), str);
            }
        }
        if (lin == 0) {
            // Closes file if nothing new
            printf("No tasks yet!\n");
            fclose(f);
            free(newlin);
            free(date);
            return 0;
        }
        printf("\n");
    }

    fclose(f);
    free(newlin);
    return 1;
}

// Pops a task from the task list and moves it to the archive
void archive(FILE* f) {
    char* tsk = (char*)malloc(sizeof(char)*12);
    int id = 0;

    // Calls list to display tasks
    if (listTasks(f) == 0) {
        // No tasks
        printf("Nothing to archive!\n");
        free(tsk);
        return;
    }

    printf("What task do you want to archive? [Enter ID #]: ");
    scanf("%s", tsk);
    id = atoi(tsk);
    // Opening archive
    f = fopen("archive.txt", "r");
    // File does not exist, creating
    if (f == NULL) {
        fclose(f);
        f = fopen("archive.txt", "w");
        fclose(f);
    }
    else {
        // Just creating a new file.
        // No need to open it
        fclose(f);
    }

    // Checking input
    f = fopen("tasks.txt", "r");
    // Finds amt of lines in file
    fseek(f, 0L, SEEK_END);
    long maxTsk = ftell(f)/40;
    if ((id >= maxTsk) || id <= 0) {
        printf("lines in task file %ld", maxTsk);
        printf("No such task exists\n");
        fclose(f);
        return;
    }
    // Closes f if the input is valid
    fclose(f);


    // Creates a new file without the target line.
    if (deleteLine(id) != 0) {
        // Removing "tasks.txt" file
        remove("tasks.txt");
        // Renaming "temp.txt" to "tasks.txt"
        rename("temp.txt", "tasks.txt");
        printf("\n");
    }

}

int operations(FILE* f) {
    char inp[1000];

    // Taking input
    printf("What would you like to do?\n");
    printf("\tList\n");
    printf("\tAdd\n");
    printf("\tArchive\n");
    printf("\tExit\n");
    printf("Enter command: ");
    scanf("%s", inp);

    // Turning string to lowercase
    lowerStr(inp);

    // Calling commands
    if (strcmp(inp, *(commands + 0)) == 0) {
        // Lists current items in tasks.txt
        listTasks(f);
        return 1;
    }
    else if (strcmp(inp, *(commands + 1)) == 0) {
        // Add something to agenda
        addTask(f);
        return 1;
    }
    else if (strcmp(inp, *(commands + 2)) == 0) {
        // Archive something
        archive(f);
        return 1;
    }
    else if (strcmp(inp, *(commands + 3)) == 0) {
        printf("Goodbye!\n");
        return 0;
    }
    else {
        printf("Invalid command\n");
    }

    return 2;
}

int main() {
    FILE *f = fopen("tasks.txt", "r");
    fclose(f);
    int running = 1;
    while (running != 0) {
        running = operations(f);
    }
    return 0;
}
