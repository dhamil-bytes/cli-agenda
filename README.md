# cli-agenda

--- UNFINISHED 2/3/24 ---

I created a CLI task tracker/agenda because I have a knack for not writing things down. So, instead of doing my actual classwork right now, I decided to make an application to help me do classwork in the future! 

Application writes and reads from a .txt file that holds a 'task.' Task attributes include course #, due date, and assignment name. Different commands can be input by the user, these being: List, Add, Archive, and Exit (exits the program). Sweet and simple personal project I decided to work on in my free time. Uses time.h library.

# List
Lists tasks to be done in no particular order. Prints out how much time you have until the due date. Uses time.h C library.

# Add
Adds a task to the task list (tasks.txt). Takes in class course-section, assignment name, and due date [int-int-int].

# Archive
Removes a task from the task list and adds it to the archive (archive.txt).

**TO BE DECIDED:
**Ask user if they want to clear the archive
**Automatically clear archive after 7 days

# Exit
Exits the program.
