#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

char* lowerStr(char*);
void listTasks(FILE*);


// List of commands
const char *commands[] = {"list", "add", "archive", "exit"};
int cal[] = {31, 28, 31, 30, 31, 30,31, 31, 30, 31, 30, 31};


// Returns string that shows how long you have until you need to submit your assignment
char* timeTill(struct tm due, struct tm today) {
    // Nweeks Mdays
    // Adjusting struct tm today
    today.tm_year = today.tm_year + 1900;
    today.tm_mon = today.tm_mon + 1;
    // Due (yr) - today (yr)
    // Convert to days
    int w1 = (due.tm_year%4==0 ? due.tm_year*365 : due.tm_year*364);
    int w2 = (today.tm_year%4==0 ? today.tm_year*365 : today.tm_year*364);


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
    int day, month, year = 0;

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
    sscanf(tDue, "%d-%d-%d", &month, &day, &year);



    // Time till due
    time_t t;
    time(&t);
    struct tm currDate = *localtime(&t);
    struct tm dueDate = {59,59,23,day, month, year, 0, 0, -1};

    timedue = timeTill(dueDate, currDate);
    sprintf(line, " %-12s|%-12s|%-12s|", tClass, tName, tDue);

    // Writing to file
    // File does not exist
    int newF = 0;
    if (!fopen("tasks.txt", "w")) {
        // Creating file if it does not exist
        f = fopen("tasks.txt", "w+");
        fclose(f);
        newF = 1;
    }
    // Adding to file
    f = fopen("tasks.txt", "w");
    if (newF == 1) {
        fprintf(f, "%-12s|%-12s|%-12s|", "Course", "Assignment", "Date Due");
    }
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
void listTasks(FILE* f) {
    char str[1000];
    char* newlin = "";
    int lin = 0;

    f = fopen("tasks.txt", "r");

    // Opens file
    if (!f) {
        // Do nothing for now
        printf("No tasks yet!\n");
        return;
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
            printf("%s\n", str);
        }
        if (lin == 0) {
            printf("No tasks yet!\n");
            return;
        }
    }

    fclose(f);
    free(newlin);
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
