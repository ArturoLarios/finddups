# finddups

The finddups program is a C program designed to scan a set of files and directories, identify duplicate files, and organize them into groups.
Usage

The program accepts command-line arguments to specify the files and directories to scan. If no arguments are provided, it defaults to the current directory. You can run the program as follows:

./finddups [file_or_directory ...]

## Functionality

### Duplicate Detection

finddups recursively traverses directories, seeking regular files for comparison. It identifies and groups identical files, printing the group details to the standard output. Each group is represented in the following format:

	n 1 pathOfFirstFile
	n 2 pathOfSecondFile
	n 3 pathOfThirdFile
	...
	n n pathOfNthFile

'n' is the number of identical files in the group.
'pathOfXFile' is the file's path.
The order of files within the group is not significant.

### Path Preservation

The program ensures that the path information in the output is preserved. Relative file names remain relative, and absolute file names remain absolute.

### File Types

finddups excludes symbolic links, devices, and other non-regular files during its comparison process. It focuses on comparing regular files.

### Error Handling

If the program encounters a file it cannot read, it prints an error message to stderr but continues examining other files. Similarly, if it encounters a directory it cannot traverse, it prints an error message but continues scanning other directories and files.