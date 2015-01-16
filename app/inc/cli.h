#ifndef _CLI_H_
#define _CLI_H_


#define MaxNumberOfCommand (20)
#define MaxNameLength (10)
#define MaxCmdLength (10)
#define MaxParameterLength (10)

#define FEEDBACK_YES 1
#define FEEDBACK_NO 0
#define FEEDBACK_NONE 3

extern int feedback;
extern int A;

typedef struct
{
	char Name[MaxNameLength];
	char Parameter[MaxParameterLength];
	float parameter;
	int NameLength;
	int ParameterLength;
	int StringLength;
}commandType;


typedef struct
{
	int head;
	int tail;
	char Name[MaxNameLength];
	commandType command[MaxNumberOfCommand];
}commandListType;


int cmdlen(char *string);
float string2number( const char *string );

void findCommandInList(char* string);
#endif
