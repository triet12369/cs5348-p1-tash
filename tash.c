#include <stdio.h>
#include "process.h"

#define EXIT_CODE_SUCCESS 0
#define EXIT_CODE_ERROR 1
#define TASH_PROMPT "tash> "

int
tashLoop()
{
	int exitCode = EXIT_CODE_SUCCESS;
	int shouldRun = 1;
	char argsLine[20];
	
	while(shouldRun) {
		printf(TASH_PROMPT);
		// read inputs
		// should replace with readLine function later
		scanf("%s", argsLine);
		printf("\nDEBUG: argsLine is %s\n", argsLine);
	}

	return exitCode;
}

int
main()
{
	printf("Welcome to Tash!\n");
	processHello();

	// actual tash main logic
	int exitCode;
	exitCode = tashLoop();
	return exitCode;
}