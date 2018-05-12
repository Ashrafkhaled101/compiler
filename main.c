#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"


// A linked list node
struct Node
{
    char data[10];
    char type[10];
    int line;
    int size;
    struct Node *next;
    struct Node *prev;

};

struct Node* list = NULL; //list Iterator.
int listSize = 0;
struct Node* s = NULL; // stack with linked-list
//FIFO List (QUEUE!)
#define QueueElement char*

typedef struct{
    int capacity;
    int size;
    QueueElement e;
    struct list_head list;
} Queue;

// FILO linked-list (Stack)
/**
 * Type for individual stack entry
 */
struct stack_entry {
  char *data;
  struct stack_entry *next;
};

/**
 * Type for stack instance
 */
struct stack_t
{
  struct stack_entry *head;
  size_t stackSize;  // not strictly necessary, but
                     // useful for logging
};


//SOME SICK FUNCTIONS ..
int IsValidNumber(char * string);
int IsValidNumberOrLetter(char c);
void append(struct Node** head_ref, char* new_data , char* new_data2 , int line);
void push(struct Node** head_ref, char* new_data , char* new_data2 ,int line);

// Parser.
int PROG();
int PROG_NAME();
int STMT_LIST();
int STMT();
int READ();
int WRITE();
int ID_LIST();
int ASSIGN();
int EXP();
//int TERM();
int FACTOR();
int DEC_LIST();

// Parsing Errors Logs.
char result[10],msg[1000],place[12];
void ErrorLog(char*result , char*msg, char*place);

// Debug
void token();

//Queue
Queue * initQueue(int max);
QueueElement front(Queue *Q);
QueueElement tail(Queue* Q);
void dequeue(Queue *Q);
void enqueue(Queue *Q, QueueElement element);


//SATCK
struct stack_t *newStack(void);
char *copyString(char *str);
void pushs(struct stack_t *theStack, char *value);
char *top(struct stack_t *theStack);
void pop(struct stack_t *theStack);
void clear (struct stack_t *theStack);
void destroyStack(struct stack_t **theStack);

// FIFO
Queue* q;
// FILO
//struct stack_t *s;


// Queue counter
int LISTCOUNT = 0;
//STACK counter
int STACKCOUNTER = 0;
// program counter
int LOCCTR = 0;
// contains the variable which is in the A ATM .
char rn[20];



int main()
{
    q = initQueue(265); // queue
    //s = newStack(); // stack

    FILE *inputf,*schemaf,*tokensf;
    inputf      = fopen("sum.txt","r");
    schemaf     = fopen("schema.txt","r");
    tokensf     = fopen("tokens.txt","w");
    remove("log.txt"); // remove the previous log file
    FILE *logf = fopen("log.txt","a");
    fprintf(logf,"[#Line]\t[Parse-Stage]\t\tResult\t\tInformations\n\n");
    fclose(logf);

    char label[10],name[10],keyword[10],tokentype[10];
    int lines = 1;

    fscanf(inputf,"%s %s\n",label,name);
    if(strcmp(label,"PROGRAM") == 0){
        fprintf(tokensf,"Line\tToken type\tToken specifier\n");
        //printf("Line\tToken type\tToken specifier\n");

        fprintf(tokensf,"%d\t1\t\t%s\n1\t22\t\t%s\n",lines,label,name);
        //printf("%d\t1\t\t%s\n1\t22\t\t%s\n",lines,label,name);

        append(&list,label,"1",lines);
        append(&list,name,"22",lines);


        lines++;
    }

    char c , token[10];
    int i=0,Iskeyword=0;
    while(fscanf(inputf,"%c",&c) != EOF){

        if(c != '\n' && c != ',' && c !=' ' && c!='\t' && c!='(' && c!= ')' && c!=';'&& c!='+'&& c!='-'&& c!='*'&& c!='/'&& c!=':'){
                token[i++] = c;
        }else{
            char temp = '\0';
            if(token[0] != '\0'){

                token[i] = '\0';
                //printf("%s\n",token);

                rewind(schemaf);
                fscanf(schemaf,"%s %s",keyword,tokentype);
                while(strcmp(keyword,"EXIT") != 0){
                    if(strcmp(token,keyword) == 0){
                        fprintf(tokensf,"%d\t%s\t\t%s\n",lines,tokentype,keyword);
                        //printf("%d\t%s\t\t%s\n",lines,tokentype,keyword);
                        append(&list,keyword,tokentype,lines);

                        Iskeyword = 1;
                        break;
                    }
                    fscanf(schemaf,"%s %s",keyword,tokentype);
                }
                if(Iskeyword == 0){ // maybe it's a number or id
                    // check whatever it's a number
                    if(IsValidNumber(token) == 1){
                        // int
                        fprintf(tokensf,"%d\t23\t\t#%s\n",lines,token);
                        //printf("%d\t23\t\t#%s\n",lines,token);
                        append(&list,token,"23",lines);


                    }else{
                        // id
                        fprintf(tokensf,"%d\t22\t\t%s\n",lines,token);
                        //printf("%d\t22\t\t%s\n",lines,token);
                        append(&list,token,"22",lines);
                    }

                }
                Iskeyword = 0;
            }

            if(c == ';' || c == ',' || c == ')' || c == '('|| c == '+'|| c == '-'|| c == '*'|| c == '/'|| c == ':'){
                token[0] = '\0';
                i=0;
                if(c == ':'){
                    token[i++] = c;
                    if(fscanf(inputf,"%c",&c) != EOF){
                        if(c == '='){
                            token[i++] = c;
                            token[i] = '\0';
                        }else{
                            temp = c;
                            token[i] = '\0';
                        }
                    }
                }else{
                    token[i++] = c;
                    token[i] = '\0';
                }

                //printf("%s\n",token);

                rewind(schemaf);
                fscanf(schemaf,"%s %s",keyword,tokentype);
                while(strcmp(keyword,"EXIT") != 0){
                    if(strcmp(token,keyword) == 0){
                        fprintf(tokensf,"%d\t%s\t\t%s\n",lines,tokentype,keyword);
                        //printf("%d\t%s\t\t%s\n",lines,tokentype,keyword);
                        append(&list,token,tokentype,lines);

                        break;
                    }
                    fscanf(schemaf,"%s %s",keyword,tokentype);
                }

            }

            token[0] = '\0';
            i=0;

            if(IsValidNumberOrLetter(temp)   == 1){
                token[i++] = temp;
            }

        }

        if(c == '\n')
            lines ++;

    }
    //printf("END.");
    //printf("\nNumber of lines : %d\n",lines);
    fprintf(tokensf,"%d\t5\t\tEND.\n",lines);
    //printf("%d\t5\t\tEND.\n",lines);
    append(&list,"END.","5",lines);

    // Parser

    if(PROG() == 1){
        printf("\nParsed successfully ...");
    }else{
        printf("\nFailed While Parsing ... ");
    }

    return 0;
}

int IsValidNumberOrLetter(char c){

    if (isalpha(c) || isdigit(c)) {
        return 1;
    } else {
        return 0;
    }

}

int IsValidNumber(char * string){
    int i =0;
   for(i = 0; i < strlen( string ); i ++)
   {
      //ASCII value of 0 = 48, 9 = 57. So if value is outside of numeric range then fail
      //Checking for negative sign "-" could be added: ASCII value 45.
      if (string[i] < 48 || string[i] > 57)
         return 0;
   }
   return 1;
}

/* Given a reference (pointer to pointer) to the head
   of a DLL and an int, appends a new node at the end  */
void append(struct Node** head_ref, char* new_data , char* new_data2 ,int line){
    /* 1. allocate node */
    struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));

    struct Node *last = *head_ref;  /* used in step 5*/

    /* 2. put in the data  */
    //new_node->data  = new_data;
    strcpy(new_node->data,new_data);
    strcpy(new_node->type,new_data2);
    new_node->line = line;
    listSize++;
    /* 3. This new node is going to be the last node, so
          make next of it as NULL*/
    new_node->next = NULL;

    /* 4. If the Linked List is empty, then make the new
          node as head */
    if (*head_ref == NULL)
    {
        new_node->prev = NULL;
        *head_ref = new_node;
        return;
    }

    /* 5. Else traverse till the last node */
    while (last->next != NULL)
        last = last->next;

    /* 6. Change the next of last node */
    last->next = new_node;

    /* 7. Make last node as previous of new node */
    new_node->prev = last;

    return;
}

/* Given a reference (pointer to pointer) to the head of a list
   and an int, inserts a new node on the front of the list. */
void push(struct Node** head_ref, char* new_data , char* new_data2 ,int line)
{
    /* 1. allocate node */
    struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));

    /* 2. put in the data  */
    strcpy(new_node->data,new_data);
    strcpy(new_node->type,new_data2);
    new_node->line = line;

    /* 3. Make next of new node as head and previous as NULL */
    new_node->next = (*head_ref);
    new_node->prev = NULL;

    /* 4. change prev of head node to new node */
    if((*head_ref) !=  NULL)
      (*head_ref)->prev = new_node ;

    /* 5. move the head to point to the new node */
    (*head_ref)    = new_node;
}


 // ---------------------------------- Parser - Implementation -------------------------------------

 // EXAMPLE :- READ(SUM,X,B,C)
 // NOTE :-
 //         22  id
 //         14  ,
int ID_LIST(){

    int FOUND = 0;
    if(strcmp(list->type,"22")==0){
        FOUND = 1;
        enqueue(q,list->data);
        LISTCOUNT ++;
        list = list->next;
        //token();
        while((strcmp(list->type,"14")==0) && FOUND == 1){
            list = list->next;
            //token();
            if(strcmp(list->type,"22")==0){
                enqueue(q,list->data);
                LISTCOUNT ++;
                list=list->next;
                //token();
            }else{
                FOUND = 0;
                break;
            }
        }
    }

    strcpy(place,"ID_LIST()");

    if(FOUND == 1){
        strcpy(msg,"The program has passed this parsing stage successfully!");
        strcpy(result,"SUCCESS");
        ErrorLog(result,msg,place);

        //code generation

        return 1;
    }else{
        strcpy(msg,"Format Error - The Program ID List either missing or unrecognized.");
        strcpy(result,"FAILURE");
        ErrorLog(result,msg,place);
        return 0;
    }

}


 // EXAMPLE :- READ(SUM,X,B,C)
 // NOTE :-
 //         22  id
 //         14  ,
int DEC_LIST(){

    int FOUND = 0;
    if(strcmp(list->type,"22")==0){
        FOUND = 1;
        enqueue(q,list->data);
        LISTCOUNT ++;
        list = list->next;
        //token();
        while((strcmp(list->type,"14")==0) && FOUND == 1){
            list = list->next;
            //token();
            if(strcmp(list->type,"22")==0){
                enqueue(q,list->data);
                LISTCOUNT ++;
                list=list->next;
                //token();
            }else{
                FOUND = 0;
                break;
            }
        }
    }

    strcpy(place,"DEC_LIST()");

    if(FOUND == 1){
        strcpy(msg,"The program has passed this parsing stage successfully!");
        strcpy(result,"SUCCESS");
        ErrorLog(result,msg,place);

        //code generation
        while(LISTCOUNT > 0){
            printf("%s\tRESW\t1\n",front(q));
            dequeue(q);
            LISTCOUNT --;
            LOCCTR+=3;
        }
        LISTCOUNT = 0;
        return 1;
    }else{
        strcpy(msg,"Format Error - The Program DEC List either missing or unrecognized.");
        strcpy(result,"FAILURE");
        ErrorLog(result,msg,place);
        return 0;
    }

}

 // NOTE :-
 //         8   READ
 //         20  (
 //         21  )

int READ(){

    int FOUND = 0;

    if(strcmp(list->type,"8")==0){
        list = list->next;
        //token();
        if(strcmp(list->type,"20")==0){
            list = list->next;
            token();
            if(ID_LIST() == 1){
                if(strcmp(list->type,"21")==0){
                    FOUND = 1;
                    list = list->next;
                    token();
                }
            }
        }
    }

    strcpy(place,"READ()");

    if(FOUND == 1){
        strcpy(msg,"The program has passed this parsing stage successfully!");
        strcpy(result,"SUCCESS");
        ErrorLog(result,msg,place);

        // code generation of read
        printf("\t+JUSB\tXREAD\n");
        printf("\tWORD\t%d\n",LISTCOUNT);
        while(LISTCOUNT > 0){
            printf("\tWORD\t%s\n",front(q));
            dequeue(q);
            LISTCOUNT --;
        }
        LISTCOUNT = 0;


        return 1;
    }else{
        strcpy(msg,"Format Error - Unrecognized READ() ,[-READ(In1,In2,...)]");
        strcpy(result,"FAILURE");
        ErrorLog(result,msg,place);
        return 0;
    }
}

 // NOTE :-
 //         9   WRITE
 //         20  (
 //         21  )

int WRITE(){

    int FOUND = 0;

    if(strcmp(list->type,"9")==0){
        list = list->next;
        token();
        if(strcmp(list->type,"20")==0){
            list = list->next;
            token();
            if(ID_LIST() == 1){
                if(strcmp(list->type,"21")==0){
                    FOUND = 1;
                    list = list->next;
                    token();
                }
            }
        }
    }

    strcpy(place,"WRITE()");

    if(FOUND == 1){
        strcpy(msg,"The program has passed this parsing stage successfully!");
        strcpy(result,"SUCCESS");
        ErrorLog(result,msg,place);
        // code generation of read
        printf("\t+JUSB\tXWRITE\n");
        printf("\tWORD\t%d\n",LISTCOUNT);
        while(LISTCOUNT > 0){
            printf("\tWORD\t%s\n",front(q));
            dequeue(q);
            LISTCOUNT --;
        }
        LISTCOUNT = 0;

        return 1;
    }else{
        strcpy(msg,"Format Error - Unrecognized READ() ,[-WRITE(In1,In2,...)]");
        strcpy(result,"FAILURE");
        ErrorLog(result,msg,place);
        return 0;
    }

}

 // NOTE :-
 //         22  id
 //         15  =

/*
int ASSIGN(){

    int FOUND = 0;
    char id[20];
    if(strcmp(list->type,"22")==0){
        strcpy(id,list->data);
        list = list->next;
        token();
        if(strcmp(list->type,"15")==0){
            list = list->next;
            token();

            if(EXP() == 1){
                FOUND = 1;
            }
        }

    }

    strcpy(place,"ASSIGN()");

    if(FOUND == 1){
        strcpy(msg,"The program has passed this parsing stage successfully!");
        strcpy(result,"SUCCESS");
        ErrorLog(result,msg,place);


        // code generation
        printf("\tSTA\t%s\n",id );
        return 1;
    }else{
        strcpy(msg,"Format Error - Unrecognized assign operation , [-<assign> ::=  id := <exp>]");
        strcpy(result,"FAILURE");
        ErrorLog(result,msg,place);
        return 0;
    }



}

 // NOTE :-
 //         16  +
 //         17  -

int EXP(){

    int FOUND = 0;

    if(TERM() == 1){
        FOUND = 1;
        while(((strcmp(list->type,"16")==0) || (strcmp(list->type,"17")==0))&& FOUND == 1){
            list = list->next;
            token();
            if(TERM() == 0){
                FOUND = 0;
            }
        }
    }

    strcpy(place,"EXP() ");

    if(FOUND == 1){
        strcpy(msg,"The program has passed this parsing stage successfully!");
        strcpy(result,"SUCCESS");
        ErrorLog(result,msg,place);
        return 1;
    }else{
        strcpy(msg,"Format Error - Unrecognized Expression , [-<exp> ::= <factor> + <factor> | <factor> * factor>]");
        strcpy(result,"FAILURE");
        ErrorLog(result,msg,place);
        return 0;
    }


}

 // NOTE :-
 //         18  *
 //         20  /

int TERM(){

    int FOUND = 0;

    if(FACTOR() == 1){
        FOUND = 1;
        while(((strcmp(list->type,"18")==0) || (strcmp(list->type,"19")==0))&& FOUND == 1){
            list = list->next;
            token();
            if(FACTOR() == 0){
                FOUND = 0;
            }
        }
    }

    strcpy(place,"TERM()");

    if(FOUND == 1){
        strcpy(msg,"The program has passed this parsing stage successfully!");
        strcpy(result,"SUCCESS");
        ErrorLog(result,msg,place);
        return 1;
    }else{
        strcpy(msg,"Format Error - Unrecognized Term , [-<exp> ::= <factor> * <factor> | <factor> / factor>]");
        strcpy(result,"FAILURE");
        ErrorLog(result,msg,place);
        return 0;
    }

}

 // NOTE :-
 //         22  id
 //         23  int
 //         20  (
 //         21  )

int FACTOR(){

    int FOUND = 0 ;
    if ((strcmp(list->type,"22")==0) || (strcmp(list->type,"23")==0)){
        FOUND = 1;
        list = list->next;
        token();

    }else{

        if(strcmp(list->type,"20")==0){
            list= list->next;
            token();
            if(EXP() == 1){
                if(strcmp(list->type,"21")==0){
                    FOUND = 1;
                    list = list->next;
                    token();
                }

            }
        }
    }

    strcpy(place,"FACTOR()");

    if(FOUND == 1){
        strcpy(msg,"The program has passed this parsing stage successfully!");
        strcpy(result,"SUCCESS");
        ErrorLog(result,msg,place);
        return 1;
    }else{
        strcpy(msg,"Format Error - Unrecognized Factor , [-<factor> ::= id | ( <exp> )]");
        strcpy(result,"FAILURE");
        ErrorLog(result,msg,place);
        return 0;
    }





}
*/


 // NOTE :-
 //         22  id
 //         15  =


int ASSIGN(){

    int FOUND = 0;
    char id[20]; // save variable result into it

    if(strcmp(list->type,"22")==0){
        strcpy(id,list->data);
        list = list->next;
        token();
        if(strcmp(list->type,"15")==0){
            list = list->next;
            token();

            if(EXP() == 1){
                FOUND = 1;
            }
        }

    }

    strcpy(place,"ASSIGN()");

    if(FOUND == 1){
        strcpy(msg,"The program has passed this parsing stage successfully!");
        strcpy(result,"SUCCESS");
        ErrorLog(result,msg,place);


        // code generation
        strcpy(rn,"null"); // reset rn
        while(STACKCOUNTER > 0){

            if(strcmp(rn,"null") == 0){
                if(strcmp(s->type,"22")==0 || strcmp(s->type,"23")==0 ){
                    strcpy(rn,s->data); // set rn
                    printf("\tLDA\t%s\n",s->data); // LDA
                }
                STACKCOUNTER --;
            }else if((strcmp(rn,"null")!=0)&&(strcmp(s->type,"16")==0)){
                STACKCOUNTER--;
                s=s->next;
                printf("\tADD\t%s\n",s->data);

            }else if((strcmp(rn,"null")!=0)&& strcmp(s->type,"18")==0){
                STACKCOUNTER--;
                s=s->next;
                printf("\tMULL\t%s\n",s->data);

            }

        }
        STACKCOUNTER = 0;
        printf("\tSTA\t%s\n",id );
        return 1;
    }else{
        strcpy(msg,"Format Error - Unrecognized assign operation , [-<assign> ::=  id := <exp>]");
        strcpy(result,"FAILURE");
        ErrorLog(result,msg,place);
        return 0;
    }



}


 // NOTE :-
 //         16  +
 //         18  *

int EXP(){

    int FOUND = 0;

    if(FACTOR() == 1){
        FOUND = 1;
        while(((strcmp(list->type,"16")==0) || (strcmp(list->type,"18")==0))&& FOUND == 1){
            push(&s,list->data,list->type,list->line);
            STACKCOUNTER++;
            list = list->next;
            //token();
            if(FACTOR() == 0){
                FOUND = 0;
            }
        }
    }

    strcpy(place,"EXP() ");

    if(FOUND == 1){
        strcpy(msg,"The program has passed this parsing stage successfully!");
        strcpy(result,"SUCCESS");
        ErrorLog(result,msg,place);
        return 1;
    }else{
        strcpy(msg,"Format Error - Unrecognized Expression , [-<exp> ::= <factor> + <factor> | <factor> * factor>]");
        strcpy(result,"FAILURE");
        ErrorLog(result,msg,place);
        return 0;
    }


}
// NOTE :-
// 22   id
// 23   int
// 20   (
// 21   )

int FACTOR(){

    int FOUND = 0 ;
    if ((strcmp(list->type,"22")==0) || (strcmp(list->type,"23")==0)){
        FOUND = 1;
        push(&s,list->data,list->type,list->line);
        STACKCOUNTER++;
        list = list->next;
        token();

    }else{

        if(strcmp(list->type,"20")==0){
            push(&s,list->data,list->type,list->line);
            STACKCOUNTER++;
            list= list->next;
            //token();
            if(EXP() == 1){
                if(strcmp(list->type,"21")==0){
                    push(&s,list->data,list->type,list->line);
                    STACKCOUNTER++;
                    FOUND = 1;
                    list = list->next;
                    token();
                }

            }
        }
    }

    strcpy(place,"FACTOR()");

    if(FOUND == 1){
        strcpy(msg,"The program has passed this parsing stage successfully!");
        strcpy(result,"SUCCESS");
        ErrorLog(result,msg,place);
        return 1;
    }else{
        strcpy(msg,"Format Error - Unrecognized Factor , [-<factor> ::= id | ( <exp> )]");
        strcpy(result,"FAILURE");
        ErrorLog(result,msg,place);
        return 0;
    }





}

int PROG(){
    int FOUND = 0;

    if((strcmp(list->type,"1")==0)){

        list = list->next;
        token();
        if(PROG_NAME() == 1){
            FOUND = 1;
        }

        if((strcmp(list->type,"2")==0) &&FOUND == 1){
            list = list->next;
            token();
            if(DEC_LIST() == 1){
                FOUND = 1;
            }else{
                FOUND = 0;
            }
        }

        if((strcmp(list->type,"3")==0) &&FOUND == 1){
            list = list->next;
            token();
            if(STMT_LIST() == 1){
                FOUND = 1;
            }else{
                FOUND = 0;
            }
        }

        if((strcmp(list->type,"5")==0) &&FOUND == 1){
            FOUND = 1;
        }else{
            FOUND = 0;
        }

    }

    strcpy(place,"PROG()");

    if(FOUND == 1){
        strcpy(msg,"THE PROGRAM HAS BEEN PARSED SUCCESSFULLY!");
        strcpy(result,"SUCCESS");
        ErrorLog(result,msg,place);

        // code generation
        printf("\tLDL\tRETADR\n");
        printf("\tRSUB\n");
        printf("\tEND\n");

        return 1;
    }else{
        strcpy(msg,"Fatal Error - While Parsing the program.");
        strcpy(result,"FAILURE");
        ErrorLog(result,msg,place);
        return 0;
    }

}

int PROG_NAME(){

    int FOUND = 0 ;
    if ((strcmp(list->type,"22")==0)){
        FOUND = 1;
        list =list->next;
        token();
    }

    strcpy(place,"PROG_NAME()");

    if(FOUND == 1){
        strcpy(msg,"The program has passed this parsing stage successfully!");
        strcpy(result,"SUCCESS");
        ErrorLog(result,msg,place);

        // code generation

        printf("\tSTART\t0\n");
        printf("\tEXTREF XREAD,XWRITE\n");
        printf("\tSTL\tRETADR\n");
        printf("\tJ\tEXAADDR\n");
        printf("RETADR\tRESW\t1\n");
        LOCCTR+=3;

        return 1;
    }else{
        strcpy(msg,"Error - The Program name either missing or unrecognized , [-<prog-name>	::=  id].");
        strcpy(result,"FAILURE");
        ErrorLog(result,msg,place);
        return 0;
    }
}

int STMT_LIST(){
    int FOUND = 0;

    if(STMT() == 1){
        FOUND = 1;

        while((strcmp(list->type,"12")==0) && FOUND == 1){
            list = list->next;
            token();
            if(STMT() != 1){
                FOUND = 0;
                break;
            }
        }
    }

    strcpy(place,"STMT_LIST()");

    if(FOUND == 1){
        strcpy(msg,"The program has passed this parsing stage successfully!");
        strcpy(result,"SUCCESS");
        ErrorLog(result,msg,place);
        return 1;
    }else{
        strcpy(msg,"Format Error - The Program's Statement List is either missing or unrecognized,[-<stmt-list> ::=  <stmt> | <stmt-list> ; <stmt>].");
        strcpy(result,"FAILURE");
        ErrorLog(result,msg,place);
        return 0;
    }

}

int STMT(){

    int FOUND = 0 ;
    if(ASSIGN() || READ() || WRITE()){
        FOUND = 1;
    }

    strcpy(place,"STMT()");

    if(FOUND == 1){
        strcpy(msg,"The program has passed this parsing stage successfully!");
        strcpy(result,"SUCCESS");
        ErrorLog(result,msg,place);
        return 1;
    }else{
        strcpy(msg,"Format Error - The Program's Statement is either missing or unrecognized,[-<stmt> ::=  <assign> | <read>  | <write> | <for>].");
        strcpy(result,"FAILURE");
        ErrorLog(result,msg,place);
        return 0;
    }

}

void token(){
    char data[10] ;
    strcpy(data, list->data);
    //printf("\n%s %d",data,list->line);
}

void ErrorLog(char*result , char*msg, char*place){
    FILE *logf = fopen("log.txt","a");
    fprintf(logf,"[%d]\t[%s]\t\t%s\t\t%s\n",list->line-1,place,result,msg);
    fclose(logf);
}

// --------------------------------------------------- Queue implementation

/* Create a Queue */
Queue * initQueue(int max)
{
    Queue *Q;
    Q = (Queue *)malloc(sizeof(Queue));
    Q->size = 0;
    Q->capacity = max;
    /* initialize the list head. Kernel list method */
    INIT_LIST_HEAD(&Q->list);
    return Q;
}

/* pop out the queue front, and free the element space */
void dequeue(Queue *Q)
{
    Queue* tmp;
    if(Q->size==0){
	printf("Queue is Empty.\n");
	return;
    }else{
	Q->size--;
	tmp = list_entry(Q->list.next, Queue, list);
	list_del(Q->list.next);
	free(tmp);
    }
}

QueueElement front(Queue *Q)
{
    Queue* first_element;
    struct list_head * first;
    if(Q->size==0){
	printf("Queue is Empty\n");
	exit(0);
    }
    /* find the first element first */
    first = Q->list.next;
    /* reconstruct the first structure */
    first_element = list_entry(first, Queue, list);
    return first_element->e;
}

QueueElement tail(Queue *Q)
{
    Queue* last_element;
    struct list_head * last;
    if(Q->size==0){
	printf("Queue is Empty.\n");
	exit(0);
    }
    /* find the last element first */
    last = Q->list.prev;
    /* reconstruct the last structure */
    last_element = list_entry(last, Queue, list);
    return last_element->e;
}

void enqueue(Queue *Q, QueueElement element)
{
    Queue* newQ;
    if(Q->size == Q->capacity){
	printf("Queue is Full. No element added.\n");
    }
    else{
	Q->size++;
	newQ = (Queue*) malloc(sizeof(Queue));
	newQ->e = element;
	/* add to the list tail */
	list_add_tail(&(newQ->list), &(Q->list));
    }
}


// --------------------------------------- STACK implementation


/**
 * Create a new stack instance
 */
struct stack_t *newStack(void)
{
  struct stack_t *stack = malloc(sizeof *stack);
  if (stack)
  {
    stack->head = NULL;
    stack->stackSize = 0;
  }
  return stack;
}

/**
 * Make a copy of the string to be stored (assumes
 * strdup() or similar functionality is not
 * available
 */
char *copyString(char *str)
{
  char *tmp = malloc(strlen(str) + 1);
  if (tmp)
    strcpy(tmp, str);
  return tmp;
}

/**
 * Push a value onto the stack
 */
void pushs(struct stack_t *theStack, char *value)
{
  struct stack_entry *entry = malloc(sizeof *entry);
  if (entry)
  {
    entry->data = copyString(value);
    entry->next = theStack->head;
    theStack->head = entry;
    theStack->stackSize++;
  }
  else
  {
    // handle error here
  }
}

/**
 * Get the value at the top of the stack
 */
char *top(struct stack_t *theStack)
{
  if (theStack && theStack->head)
    return theStack->head->data;
  else
    return NULL;
}

/**
 * Pop the top element from the stack; this deletes both
 * the stack entry and the string it points to
 */
void pop(struct stack_t *theStack)
{
  if (theStack->head != NULL)
  {
    struct stack_entry *tmp = theStack->head;
    theStack->head = theStack->head->next;
    free(tmp->data);
    free(tmp);
    theStack->stackSize--;
  }
}

/**
 * Clear all elements from the stack
 */
void clear (struct stack_t *theStack)
{
  while (theStack->head != NULL)
    pop(theStack);
}

/**
 * Destroy a stack instance
 */
void destroyStack(struct stack_t **theStack)
{
  clear(*theStack);
  free(*theStack);
  *theStack = NULL;
}

