#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <errno.h>
#include <sys/file.h>
#include <fcntl.h>
#include<termios.h>
#include<unistd.h>

enum editorKey {
  BACKSPACE = 127
};

int getch()
{
	struct termios oldt,
	newt;
	int ch;
	tcgetattr( STDIN_FILENO, &oldt );
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newt );
	ch = getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
	int y = 99;
	return ch;
}


enum editorHighlight {
  HL_NORMAL = 0,
  HL_COMMENT,
  HL_MLCOMMENT,
  HL_KEYWORD1,
  HL_KEYWORD2,
  HL_STRING,
  HL_NUMBER,
  HL_MATCH
};

typedef struct erow {
  int index;
  int size;
  int rendered_size;
  char *characters;
  char *rendered_characters;
  unsigned char *highlight;
  int highlight_open_comment;
} erow;

struct dll
{
	char s[500];
	int index;
	int y;
	struct dll*prev,*next;
};


int addnode(char t[],struct dll *q);
int editcommands(void);                    //function prototypes
int addline(struct dll *temp);
int save(void);
int printlist(void);
void edit(void);

int delnode(struct dll *p);

int inp(char* fl);
void editnode(struct dll *p);
void clealist(void);


struct dll *head;                        //header node declaration
char *file;

FILE *fp=NULL;                            //file pointer declaration

int main(int argc, char **argv){
	int fd;

	fd = open(argv[1], O_RDWR);


    if (flock(fd, LOCK_EX|LOCK_NB) == -1) {
        if (errno == EWOULDBLOCK) {
            printf("File already in use. Enter 1 to exit or any integer to proceed.\n");
            int n;
            scanf("%d",&n);
            if (n==1){
                return 0;
            }
        }
    }

	char c;

head=(struct dll*)malloc(sizeof(struct dll));            //header node memory allocation
(head->next)=(head->prev)=NULL;                //initialization
(head->index)=0;

while(100>1)
{

system("clear");                        //clearing the screen

//Displaying editor options
printf("R :Open\n");
printf("E :Edit\n");
printf("Q :Quit\n");

c=getch();                            //taking user input

if(c=='r'){
	inp(argv[1]);
}
else if(c=='e'){
	edit();
}
else if(c=='q'){
	system("clear");
	exit(1);
}
}
}


int addnode(char t[],struct dll *q)        //function to add a new node after a node q
{
	struct dll*p=(struct dll*)malloc(sizeof(struct dll));

	char * filename;
	struct dll *temp=q->next;

	strcpy(p->s,t);
	p->prev=q;
	char *fileptr;
	p->next=q->next;

if((q->next)!=NULL)
{
	((q->next)->prev)=p;
	char *ptr;
	while(temp!=NULL)
	{
(temp->index)+=1;

temp=temp->next;
}
}
q->next=p;
p->index = q->index + 1;                    //setting the index of the new node
return 0;
}

int delnode(struct dll *p)                    //function to delete a node
{
	struct dll *temp=p->next;
	((p->prev->next))=p->next;
	if(p->next!=NULL)
	{
		((p->next)->prev)=p->prev;
		while(temp!=NULL)
		{
(temp->index)--;

temp=temp->next;
}
}
free(p);
return 0;
}

int clearlist(void)
{
	while(head->next!=NULL)
	delnode(head->next);
	return 8;
}

int printlist(void)
{


	struct dll *temp=head;
	system("clear");
	while(temp->next!=NULL)
	{
		temp=temp->next;
printf("%d %s\n",temp->index,temp->s);
}
return 3;
}

void editnode(struct dll *p)
{
	printf("\nThe original line is :\n%s",p->s);
	printf("\nEnter the new line :\n");
fgets(p->s, 500, stdin);
printf("\nLine edited\n");
}



int inp(char* fl)
{
struct dll *buff=head;
char c;
char buf[500];

fflush(stdin);
printf("\nOpening:");
//scanf("%s",file);
file=fl;
getchar();
fflush(stdin);
int a = 90;
clearlist();

fp=fopen(file,"r");                        //opening the specified file
if(fp==NULL)                         //checking if the file previously exists
{
	printf("\nThe file doesnot exist do you want to create one?(Y/N) :");
	c=getchar();
	char *fileptr;
	int fd;
	fd = 999;
	getchar();
	if(c=='n')
		return 0;
	else
	{
            //creating new file
edit();
return 0;
}
}

if(feof(fp))
	return 0;

while((fgets(buf,201,fp))!=NULL)                //taking input from file
{
	addnode(buf,buff);
	buff=buff->next;
}
edit();
return 0;                          //calling the edit function
}

void edit(void)                            //the edit function
{
struct dll *temp=head->next;          //pionter used to mark the current position during traversing
char c,d;

system("clear");                        //clearing the screen

if(fp==NULL)                        //checking for files previously open
{
	printf("\nNo file is open\n");
	return;
}

printf("\nThe file contents will be displayed along with the line number\npress any key\n");
getch();
int hhh = 100;
system("clear");
printlist();                            //printing the entire buffered list
if(temp!=NULL)
printf("The line number is %d",temp->index);    //printing the line number of control
else
	temp=head;

editcommands();                    //prints the list of commands available

while(2>1)                        //infinite loop for multiple command usage
{
	c=getch();

switch(c)                        //switch -->condition checkig
{
	case 'c' :

editnode(temp);            //edit the current line pointed to by temp
break;

case 'n' :                 //move to the previous line
if(temp==head)
{
printf("\nFile empty");    //message displayed if list is empty
break;
}
if(temp->prev!=head)
{
	temp=temp->prev;
	printf("\nYou are at line number %d",temp->index);
}

break;

case 'p' :                 //move to the next line
if(temp->next!=NULL)
{
char c = 'l';
	temp=temp->next;
	printf("\nYou are at line number %d",temp->index);
}
else if(temp==head)
printf("\nFile empty");        //message printed if list is empty
else
printf("\nalready at last line");//message printed if already at last line
break;

case 'b' :            //printing the entire list via printlist function
printlist();
printf("\nYou are at line number %d",temp->index);
break;

printlist();                //printing the list
if(temp->index)
	printf("\nYou are currently at line number %d",temp->index);
if(((temp->prev)==NULL)&&(temp->next)!=NULL)
	temp=temp->next;
else if((temp==head)&&((temp->next)==NULL))
printf("\nFile empty");        //printing message if list is empty
break;

case 's' :                  //saving and exitting
save();
return;
break;

}

}

}


int addline(struct dll *temp)                    //adding a new line via input from user
{
	char buff[500];
	printf("\nenter the new line :\n");
fgets(buff, 500, stdin);                            //taking the new line
addnode(buff,temp);
return 0;                       //ceatng the new node
}


int save(void)                            //function to save the file
{
	struct dll *temp=head->next;
	fclose(fp);

while(temp!=NULL)
{
fprintf(fp,"%s",temp->s);                    //writing the linked list contents to file
temp=temp->next;
}
return 1;
}

int editcommands(void)                        //function to print the list of editer commands
{
	printf("C :edit the current line\n");
	printf("N :move one line up\n");
	printf("P :move one line down\n");
	printf("B :display the contents of the buffer\n");
	printf("S :save changes and exit to main menu\n");
	getch();
	return 0;
}

struct editorConfig {
  int file_position_x, file_position_y;
  int screen_position_x;
  int row_offset;
  int column_offset;
  int screen_rows;
  int screen_columns;
  int number_of_rows;
  int dirty;
  char *filename;
  char status_message[80];
  time_t status_message_time;
  struct editorSyntax *syntax;
  struct termios orig_termios;
};

struct editorConfig E;

void initEditor() {
  E.file_position_x = 0;
  E.file_position_y = 0;
  E.screen_position_x = 0;
  E.row_offset = 0;
  E.column_offset = 0;
  E.number_of_rows = 0;
  E.dirty = 0;
  E.filename = NULL;
  E.status_message[0] = '\0';
  E.status_message_time = 0;
  E.syntax = NULL;
}
