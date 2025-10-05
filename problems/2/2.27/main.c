// author: galus
// desc: Write a program using Win32 or POSIX API.
// requirements:
// - Do error checking.
// - Prompt user for name of a source and destination file.
// - Copy contents from src to dst file.
// - Run program and report back system calls. `ptrace`, `ktrace`

#include "fcntl.h"
#include "syslog.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	mode_t mode = S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH;
	char *defaultInput = "input.txt";
	char *defaultOutput = "output.txt";
	char buffer[256];
	char *input = defaultInput;
	char *output = defaultOutput;
	
	printf("Enter input file (default: %s): ", defaultInput);
	if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
		buffer[strcspn(buffer, "\n")] = 0;
		if (strlen(buffer) > 1) {
			printf("Entered %s\n", buffer);
			input = strdup(buffer);
		} else {
			printf("Defaulting to %s\n", defaultInput);
		}
	}

	printf("Enter output file (default: %s): ", defaultOutput);
	if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
		buffer[strcspn(buffer, "\n")] = 0;
		if (strlen(buffer) > 1) {
			printf("Entered %s\n", buffer);
			output = strdup(buffer);
		} else {
			printf("Defaulting to %s\n", defaultOutput);
		}
	}


	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		printf("Program's current directory: %s\n", cwd);
	}
	// open a file for reading
	printf("Reading file: (%s)\n", input);
	int fdr = open(input, O_RDONLY);
	if (-1 == fdr) {
		perror("Cannot open file\n");
		goto cleanup;
	}

	// open a file for writing
	printf("Opening file for writing: (%s)\n", output);
	int fdw = open(output, O_WRONLY, mode);
	if (-1 == fdw) {
		perror("Cannot open file\n");
		goto cleanup;
	}


	// read input
	ssize_t size_read = read(fdr, buffer, 256);
	buffer[size_read] = '\0';
	printf("read: \n==============\n%s==============\n", buffer);

	// write to output
	ssize_t bytes_written = write(fdw, buffer, size_read);
	if (bytes_written != size_read) {
		perror("Error writing to file\n");
		goto cleanup;
	}

	printf("\nDone.\n");

	close(fdw);
	close(fdr);

cleanup:
	if (input != defaultInput) {
		free(input);
	}
	if (output != defaultOutput)
	{
		free(output);
	}

	return EXIT_SUCCESS;
}
