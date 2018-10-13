/** Exercise 01: Calendar manager
 * Simple C program to practice with pointers to pointers
 * Copyright (C) RedFantom 2017
 * Available under GNU GPLv3 as in LICENSE.md
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define DAYS_IN_WEEK 7
#define MAX_NOTE_LENGTH 100

/**Type definitions
 * This makes sure that typing enum and struct all the time is no
 * longer necessary.
*/
typedef struct Note Note;
typedef enum Day Day;
/** Function definitions
 *
 * These make sure that the order of the functions doesn't matter as the
 * functions are always defined before they are called in the code.
*/
// Note operations
char* day_to_string(Day day);
int get_amount_notes(Note* first_note);
Note* get_last_note(Note* note);
// Memory management
Note* create_note(char* text, Day day);
void free_note(Note* note);
// Printing notes
void print_note(Note* note);
void print_notes(Note* first_note);
void print_notes_by_day(Note** notes_by_day);
// Adding notes
Note* add_note(Note* first_note, Note* note_to_add);
Note** add_note_user(Note** notes_by_day);
// Deleting notes
Note** delete_note_user(Note** notes_by_day);
Note* delete_note_by_text(Note* first_note, char* text);
Note* delete_note_by_keyword(Note* first_note, char* keyword);
Note* delete_note_by_index(Note* first_note, int index);
Note* delete_note(Note* first_note, Note* to_delete);
// Finding notes
Note* find_by_keyword(Note** notes_by_day, char* keyword);
Note* find_by_text(Note** notes_by_day, char* text);
Note* find_by_index(Note** notes_by_day, Day day, int index);
Note* find_by_index_user(Note** notes_by_day);
Note* find_user(Note** notes_by_day);
// Serialization
Note** read_from_file(Note** notes_by_day);
void save_to_file(Note** notes_by_day);
// Miscellaneous
void print_help();
void print_find_help();
Day read_day();
int main();

enum Day{
    /*
    This enum contains a Day Of Week
    */
    SUNDAY = 1,
    MONDAY = 2,
    TUESDAY = 3,
    WEDNESDAY = 4,
    THURSDAY = 5,
    FRIDAY = 6,
    SATURDAY = 7
};

struct Note {
    /*
    This struct can store a single note
    */
    char* text;
    Day day;
    Note* next;
    Note* prev;
};

char* day_to_string(Day day) {
    /*
    Return a printable string for a Day

    :param Day: Either enum Day, or compatible int 1-7
    :return: String with the name of the day
    */
    switch (day) {
        case 1: return "Sunday";
        case 2: return "Monday";
        case 3: return "Tuesday";
        case 4: return "Wednesday";
        case 5: return "Thursday";
        case 6: return "Friday";
        case 7: return "Saturday";
    }
}

/* Memory management */

Note* create_note(char* text, Day day) {
    /*
    Allocate memory for a Note and return a pointer

    :param text: The text that is to be stored in the note
    :return: A Pointer to the new Note
    */
    Note* note = (Note*) malloc(sizeof(Note));
    note->text = (char*) malloc(sizeof(char) * strlen(text) + 1);
    day += 1;
    note->day = day;
    strcpy(note->text, text);
    return note;
}

void free_note(Note* note) {
    /*
    Free the memory for a note

    :param note: A Note pointer to free the memory of
    :return: void
    */
    free(note->text);
    free(note);
}

/* Note operations */

int get_amount_notes(Note* first_note) {
    /*
    Get the amount of notes in a linked list

    :param first_note: First item in the linked list to determine the length of
    :return: amount of items in list, int
    */
    if (first_note == NULL) return 0;
    Note* note = first_note;
    int amount = 0;
    while (true) {
        amount += 1;
        if (note->next == NULL) break;
        note = note->next;
    }
    return amount;
}

int get_amount_notes_by_day(Note** notes) {
    /*
    Get the amount of notes of a notes database

    :param notes: notes database
    :return: amount of items in the database
    */
    int total = 0;
    int i = 0;
    for (i; i < DAYS_IN_WEEK; i++) {
        total += get_amount_notes(notes[i]);
    }
    return total;
}

Note* get_last_note(Note* first_note) {
    /*
    Get the last item in a linked list

    :param first_note: First item in linked list to return the last item of
    :return: pointer to last item, or NULL if the list is empty
    */
    if (first_note == NULL) return NULL;
    Note* current = first_note;
    while (true) {
        if (current->next == NULL) break;
        current = current->next;
    }
    return current;
}

/* Printing functions */

void print_note(Note* note) {
    /*
    Perform a NULL check and then print the text of a note

    :param note: The note to print the text of
    :return: void
    */
    if (note == NULL) return;
    printf("%-10s: %s\n", day_to_string(note->day), note->text);
}

void print_notes(Note* first_note) {
    /*
    Pretty print a linked list of notes

    :param first_note: First item in linked list of notes
    :return: void
    */
    if (first_note == NULL) return;
    Note* note = first_note;
    while (true) {
        print_note(note);
        if (note->next == NULL) break;
        note = note->next;
    }
}

void print_notes_by_day(Note** notes_by_day) {
    /*
    Pretty print all the notes for a week

    :param notes_by_day: Pointer to pointers for linked lists (array of linked
                         lists)
    :return: void
    */
    if (notes_by_day == NULL) return;
    int i = 0;
    for (i; i < DAYS_IN_WEEK; i++) {
        if (notes_by_day[i] == NULL) continue;
        print_notes(notes_by_day[i]);
    }
}

/* Adding notes */

Note* add_note(Note* first_note, Note* note_to_add) {
    /*
    Perform NULL checks and then add a note to the end of a linked list

    :param first_note: First note in the linked list to add the new item to
    :param note_to_add: Note to add to the linked list
    :return: pointer to the last item of the linked list
    */
    if (note_to_add == NULL) return first_note;
    if (first_note == NULL) return note_to_add;
    Note* last_note = get_last_note(first_note);
    last_note->next = note_to_add;
    note_to_add->prev = last_note;
    return first_note;
}

Note** add_note_user(Note** notes_by_day) {
    /*
    Get input from the user to add a note to the database

    :param notes_by_day: Pointer to linked list pointers
    :return: Pointer to linked list pointers (unmodified with the current code)
    */
    Day day_of_week = read_day();
    day_of_week -= 1;
    if (!(1 <= day_of_week <= DAYS_IN_WEEK)) {
        printf("Invalid day entered.\n");
        return notes_by_day;
    }
    printf("Note text (%d): ", MAX_NOTE_LENGTH);
    char text[MAX_NOTE_LENGTH];
    scanf(" %100[^\n]", text);
    Note* note = create_note(text, day_of_week);
    Note* first_note = notes_by_day[day_of_week];
    notes_by_day[day_of_week] = add_note(first_note, note);
    return notes_by_day;
}

/* Finding notes */

Note* find_by_index(Note** notes_by_day, Day day, int index) {
    /*
    Returns the index'th item in the linked list of Day day.

    :param notes_by_day: Notes database
    :param day: Day to search in
    :param index: Item to get
    :return: Item or last item when index too large
    */
    Note* first_note = notes_by_day[day - 1];
    if (first_note == NULL) return NULL;
    Note* current = first_note;
    int i = 0;
    for (i; i < index; i++) {
        if (current->next == NULL) return current;
        current = current->next;
    }
    return current;
}

Note* find_by_keyword(Note** notes_by_day, char* keyword) {
    /*
    Search for a note with substring testing. Return the first note
    that is found in the notes database.

    :param notes_by_day: notes database
    :param keyword: string with keyword(s) in it
    */
    int i = 0;
    Note* first_note;
    Note* current = NULL;
    for (i; i < DAYS_IN_WEEK; i++) {
        first_note = notes_by_day[i];
        if (first_note == NULL) continue;
        current = first_note;
        while (true) {
            if (strstr(current->text, keyword) != NULL) return current;
            if (current->next == NULL) break;
            current = current->next;
        }
    }
    return NULL;
}

Note* find_by_text(Note** notes_by_day, char* text) {
    /*
    Search for a note by comparing the full contents of its text. Only
    the first result is returned.

    :param notes_by_day: notes database
    :param text: string with full text
    */
    int i = 0;
    Note* first_note;
    Note* current = NULL;
    for (i; i < DAYS_IN_WEEK; i++) {
        first_note = notes_by_day[i];
        if (first_note == NULL) continue;
        current = first_note;
        while (true) {
            if (strcmp(current->text, text) == 0) return current;
            if (current->next == NULL) break;
            current = current->next;
        }
    }
    return NULL;
}

Note* find_by_index_user(Note** notes_by_day) {
    /*
    Perform the user interaction required to perform a search by index.
    The searching is done by find_by_index().

    :param notes_by_day: Notes database
    :return: Result of find_by_index
    */
    Day day = read_day();
    if (!(1 <= day <= DAYS_IN_WEEK)) {
        printf("Invalid day entered. Please try again.\n");
        return find_by_index_user(notes_by_day);
    }
    Note* first_note = notes_by_day[day - 1];
    int length = get_amount_notes(first_note);
    if (length == 0) {
        printf("The linked list of this day is empty.");
        return NULL;
    }
    int index;
    printf("Index (1-%d): ", length);
    scanf(" %d", &index);
    if (!(1 <= index <= length)) {
        printf("Invalid index entered. Please try again.\n");
        return find_by_index_user(notes_by_day);
    }
    return find_by_index(notes_by_day, day, index);
}

Note* find_user(Note** notes_by_day) {
    /*
    Perform the user input required for a search. Some methods
    require user input from other functions. Searching is not
    performed in this function at all.

    :param notes_by_day: Notes database
    :return: result of search function called
    */
    printf("Method (i/k/t/l/c/h): ");
    char command;
    scanf(" %c", &command);
    if (command == 'i') {
        return find_by_index_user(notes_by_day);
    } else if (command == 'k') {
        char keyword[MAX_NOTE_LENGTH];
        printf("Keyword (%d): ", MAX_NOTE_LENGTH);
        scanf(" %100[^\n]s", keyword);
        return find_by_keyword(notes_by_day, keyword);
    } else if (command == 't') {
        char text[MAX_NOTE_LENGTH];
        printf("Text (%d): ", MAX_NOTE_LENGTH);
        scanf(" %100[^\n]s", text);
        return find_by_text(notes_by_day, text);
    } else if (command == 'l') {
        Day day = read_day();
        Note* first_note = notes_by_day[day - 1];
        return get_last_note(first_note);
    } else if (command == 'c') {
        return NULL;
    } else if (command == 'h') {
        print_find_help();
        return NULL;
    } else {
        printf("Invalid command entered. Please try again.\n");
        return find_user(notes_by_day);
    }
}

/* Deleting notes */

Note* delete_note(Note* first_note, Note* to_delete) {
    /*
    Delete a note from a linked list

    :param first_note: First item of linked list
    :param to_delete: Note to delete
    :return: New pointer to first item of linked list
    */
    if (to_delete == NULL) {
        printf("Nothing to delete!\n");
        return first_note;
    }
    printf("Deleting note: %s\n", to_delete->text);
    if (to_delete->prev == NULL) {
        // first_note and to_delete have the same address, so they're the same
        if (first_note->next != NULL) {
            // There is more than one note in the linked list
            first_note->next->prev = NULL;
            Note* new_first_note = first_note->next;
            free_note(first_note);
            return new_first_note;
        } else {
            free_note(first_note);
            return NULL;
        }
    }
    // The note is not the first item
    if (to_delete->next == NULL) {
        // The note is the last item
        to_delete->prev->next = NULL;
        free_note(to_delete);
        return first_note;
    } else {
        // The note is not the last item
        to_delete->prev->next = to_delete->next;
        free_note(to_delete);
        return first_note;
    }
    // Unreachable
}

Note** delete_note_user(Note** notes_by_day) {
    /*
    Offer the user various ways of removing a note and get the user input
    required for that method.

    :param notes_by_day: Notes database
    :return: Notes database
    */
    Note* to_delete = find_user(notes_by_day);
    if (to_delete == NULL) return notes_by_day;
    notes_by_day[to_delete->day - 1] = delete_note(notes_by_day[to_delete->day - 1], to_delete);
    return notes_by_day;
}

/* Saving and opening */

void save_to_file(Note** notes) {
    /*
    Save notes to file

    :param notes: Notes database
    :return: void
    */
    char filename[MAX_NOTE_LENGTH];
    printf("File: ");
    scanf(" %100[^\n]s", filename);
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Failed to open file: '%s'.\n", filename);
        return;
    }
    int i = 0;
    Note* current = NULL;
    for (i; i < DAYS_IN_WEEK; i++) {
        if (notes[i] == NULL) continue;
        current = notes[i];
        while (true) {
            fprintf(file, "%d\n", current->day);
            fprintf(file, "%s\n", current->text);
            if (ferror(file)) {
                printf("Writing to file failed.\n");
                fclose(file);
                return;
            }
            if (current->next == NULL) break;
            current = current->next;
        }
    }
    fclose(file);
}

Note** read_from_file(Note** notes) {
    /*
    Read notes from a file

    :param notes: notes database
    :return: notes database
    */
    char filename[MAX_NOTE_LENGTH];
    printf("File: ");
    scanf(" %100[^\n]s", filename);
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file '%s'.\n", filename);
        return notes;
    }
    int day;
    char text[MAX_NOTE_LENGTH];
    Note* note;
    Note* first;
    int total = 0;
    int read;
    while (!feof(file)) {
        read = 0;
        read += fscanf(file, " %d\n", &day);
        read += fscanf(file, " %100[^\n]s", text);
        if (read != 2) break;
        day -= 1;
        note = create_note(text, day);
        first = notes[day];
        notes[day] = add_note(first, note);
        total += 1;
        read = 0;
    }
    printf("Read %d notes. Now %d notes total.\n", total, get_amount_notes_by_day(notes));
    return notes;
}

/* Miscellaneous */

void print_help() {
    /*
    Print all the commands with their descriptions
    */
    printf("Help for calendar manager:\n");
    printf("a - Add a new note\n");
    printf("d - Delete a note\n");
    printf("p - Print existing notes\n");
    printf("f - Find a note by keyword\n");
    printf("s - Save the current notes to file\n");
    printf("r - Read notes from file\n");
    printf("h - Print this help text\n");
    printf("q - Exit the program\n");
}

void print_find_help() {
    /*
    Print the help for finding files.
    i/k/t/l/c/h
    */
    printf("Help for methods of finding notes.\n");
    printf("i - Find by index\n");
    printf("k - Find by keyword\n");
    printf("t - Find by full text\n");
    printf("l - Get the last item of a day\n");
    printf("c - Cancel this command\n");
    printf("h - Print this help text\n");
}

Day read_day() {
    /*
    Read a day value from the user
    */
    printf("Day of the week (1-%d): ", DAYS_IN_WEEK);
    int day;
    scanf(" %d", &day);
    if (!(1 <= day <= DAYS_IN_WEEK)) {
        printf("Invalid day entered.");
        return read_day();
    }
    return day;
}

int main() {
    /*
    Run a command loop with user input

    :return: 0 if exited normally
    */
    Note** notes_by_day = (Note**) malloc(DAYS_IN_WEEK * sizeof(Note*));
    Note* found = NULL;
    char command;
    printf("Welcome to the Notes manager.\n\n");
    while (true) {
        printf("Command (a/d/p/f/s/r/h/q): ");
        scanf(" %c", &command);
        if (command == 'a') {
            notes_by_day = add_note_user(notes_by_day);
        } else if (command == 'd') {
            notes_by_day = delete_note_user(notes_by_day);
        } else if (command == 'p') {
            print_notes_by_day(notes_by_day);
        } else if (command == 'f') {
            found = find_user(notes_by_day);
            if (found == NULL) continue;
            print_note(found);
        } else if (command == 's') {
            save_to_file(notes_by_day);
        } else if (command == 'r') {
            notes_by_day = read_from_file(notes_by_day);
        } else if (command == 'q') {
            break;
        } else if (command == 'h') {
            print_help();
        } else {
            continue;
        }
    }
    return 0;
}
