#include <sys/types.h> // for stat(), opendir(), closedir()
#include <sys/stat.h> // for stat()
#include <unistd.h> // for stat(), off_t
#include <dirent.h> // for opendir(), closedir(), readdir ()
#include <stdlib.h> // for malloc()
#include <assert.h> // for assert()
#include <string.h> // for strcmp(), strcpy(), strcat()
#include <stdio.h> // for printf(), fprintf() and stderr

typedef struct list
{
    struct list* next;
    char *file;
} list;

typedef struct dictionary 
{
    struct dictionary* next;
    off_t key;
    list *files;
} dictionary;


// "Create" an empty dictionary
dictionary* create_dictionary(void)
{
    return NULL;
}

// Add a node to the front of the list
void list_prepend(list **head, char *data)
{
    list *newNode = (list*) malloc(sizeof(list));
    assert(newNode);
    newNode->next = *head;
    newNode->file = data;
    *head = newNode;
}

// Returns true if the entry exists
dictionary* find_entry(dictionary *head, int key)
{
    for (dictionary *currentEntry = head; currentEntry != NULL; currentEntry = currentEntry->next)
        if (currentEntry->key == key)
            return currentEntry;
    return NULL;
}

// Add the file to the keyed entry's list of files
// If an entry doesn't exist, prepend an entry with the file
void insert_entry(dictionary **head, int key, char *file)
{
    dictionary *entry = find_entry(*head, key);
    if (entry)
        list_prepend(&(entry->files), file);
    else {
        dictionary* newEntry = (dictionary*) malloc(sizeof(dictionary));
        assert(newEntry);
        newEntry->next = *head;
        newEntry->key = key;
        newEntry->files = NULL;
        list_prepend(&(newEntry->files), file);
        *head = newEntry; // Set new entry as head
    }
}

// Frees all list nodes and srtings
void delete_list(list *head) {
    list *nextNode = NULL;
    for (list *currentNode = head; currentNode != NULL; currentNode = nextNode) {
        nextNode = currentNode->next;
        free(currentNode->file);
        free(currentNode);
    }
}

// Frees each entry and its list of files
void delete_dictionary(dictionary *head)
{
    dictionary *nextEntry = NULL;
    for (dictionary *currentEntry = head; currentEntry != NULL; currentEntry = nextEntry) {
        nextEntry = currentEntry->next;
        delete_list(currentEntry->files);
        free(currentEntry);
    }
}

// Compares two files to determine if they are identical (1) or differ (0)
int compareFiles(char *fname1, char *fname2)
{
    FILE *file1, *file2;
    int ch1, ch2;

    if ((file1  = fopen(fname1, "r")) == NULL) {            // Open the first file
        fprintf(stderr, "failed to open %s\n", fname1);
        return -1;                                              // -1 indicates error opening first file
    }
    if ((file2 = fopen(fname2, "r")) == NULL) {             // Open the second file
        fprintf(stderr, "failed to open %s\n", fname2);
        fclose(file1);
        return -2;                                              // -2 indicates error opening second file
    }

    // While char read from file0 is the same as the char read from file 1
    while ((ch1 = fgetc(file1)) == (ch2 = fgetc(file2)))
        if (ch1 == EOF) {   // If EOF was reached on both files
            fclose(file1);      
            fclose(file2);      
            return 1;           
        }

    fclose(file1);
    fclose(file2);
    return 0;
}

void finddups(dictionary *head)
{
    for (dictionary *currentEntry = head; currentEntry != NULL; currentEntry = currentEntry->next) {
        printf("%ld\n", currentEntry->key);
        for (list *currentNode = currentEntry->files; currentNode != NULL; currentNode = currentNode->next) {
            printf("%s\n", currentNode->file);
        }
    }

    // For every file size dictionary entry
        // For every file of that size
            // Make an empty list
                // Compare the file to the rest of the files in front of it that aren't already marked as copies
                    // If the two files are the same mark them as copies
                        // Put them in a new list
                        // Incrememt the "number of files in group" counter
                    // Print the list following the format in README.md
                

}

// Prepend file path to list of files
void processFile(dictionary **head, char *path, size_t len, struct stat sb)
{
    char *filePath = (char *) malloc(len + 1);      // Allocate character array that will contain file pathname + null terminating character
    assert(filePath);                               // Assert that malloc() succeeded
    strcpy(filePath, path);                         // Copy path to pathName
    insert_entry(head, sb.st_size, filePath);      // Process the file
}

// Look for files in given directory path
// Call recursively when new directory is found
void processDir(dictionary **head, char *path, size_t len)
{
	DIR *dir;
	struct dirent *entry;
    char *filePath;

    if ((dir = opendir(path)) == NULL) {                    // Open the directory
    	fprintf(stderr, "failed to open directory: %s\n", path);    // Print an error message and return on failure
		return;
	}

    while ((entry = readdir(dir)) != NULL) {                                // While there is an entry in the directory to be read
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {    // If it's not . or .. (preventing loops and repeating directories)
            filePath = (char *) malloc(len + strlen(entry->d_name) + 2);        // Allocate character array for current path + '/' + name of entry + null terminating character
            assert(filePath);                                                   // Assert that malloc() succeeded
            strcpy(filePath, path);                                             // Copy the current path to filePath
            if (filePath[len - 1] != '/')                                       // If the file path didn't have '/' at the end already
                strcat(filePath, "/");                                              // Add a '/' to the end
            strcat(filePath, entry->d_name);                                    // Add the current entry's name to the end

            switch (entry->d_type) {                                            // Check current entry's type
                case DT_DIR:                                                        // If it's a directory type
                    processDir(head, filePath, strlen(filePath));                             // Recur on processDir()
                    free(filePath);                                                     // On return, free the directory's filePath since its not a regular file
                    break;
                case DT_REG:                                                        // If it's a regular file type
                    struct stat sb; // sb = stat buffer
                    if (!stat(filePath, &sb))                                           // Retrieve file information
                        insert_entry(head, sb.st_size, filePath);                           // Insert the file's name into the dictionary based on the file's size                                  
                    break;
                default:                                                            // If it's any other type
                    free(filePath);                                                     // Free filePath since it's not a regular file
            }
        }
	}

    closedir(dir);                                                          // Close the directory
}

int main(int argc, char *argv[])
{
    // Initialize empty dictionary to organize files by size
    dictionary *filesBySize = create_dictionary(); 

    struct stat sb; // sb = stat buffer

    if (argc == 1)                                                  // If no program arguments are given
        processDir(&filesBySize, ".", strlen("."));                     // Process the current working directory
    else {                                                          // Otherwise
        for (int i = 1; i < argc; i++) {                                // Iterate through program arguments using i
                if (!stat(argv[i], &sb)) {                                  // If stat doesn't fail
                    if (S_ISDIR(sb.st_mode))                                    // If it's a directory
                        processDir(&filesBySize, argv[i], strlen(argv[i]));         // Process the directory
                    else if (S_ISREG(sb.st_mode)) {                             // If it's a regular file
                        processFile(&filesBySize, argv[i], strlen(argv[i]), sb);    // Process the file
                    }
                }
        }
    }

    finddups(filesBySize); // Identify duplicate files and print them to stdout
    delete_dictionary(filesBySize); // Free all of the memory dynamically allocated by the dictionary

    return 0;
}
