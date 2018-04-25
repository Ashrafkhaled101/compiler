#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// A linked list node
struct Node
{
    char data[10];
    char type[10];
    struct Node *next;
    struct Node *prev;
};

struct Node* list = NULL; //list Iterator.

int IsValidNumber(char * string);
int IsValidNumberOrLetter(char c);
void append(struct Node** head_ref, char* new_data , char* new_data2);

// Parser.
int READ();
int ID_LIST();
int ASSIGN();
int EXP();
int TERM();
int FACTOR();

int main()
{

    FILE *inputf,*schemaf,*grammarf,*tokensf;
    inputf      = fopen("input2.txt","r");
    schemaf     = fopen("schema.txt","r");
    grammarf    = fopen("grammar.txt","r");
    tokensf     = fopen("tokens.txt","w");

    char label[10],name[10],keyword[10],tokentype[10];
    int lines = 1;

    fscanf(inputf,"%s %s\n",label,name);
    if(strcmp(label,"PROGRAM") == 0){
        fprintf(tokensf,"Line\tToken type\tToken specifier\n");
        printf("Line\tToken type\tToken specifier\n");

        fprintf(tokensf,"%d\t1\t\t%s\n1\t22\t\t%s\n",lines,label,name);
        printf("%d\t1\t\t%s\n1\t22\t\t%s\n",lines,label,name);

        append(&list,label,"1");
        append(&list,name,"22");


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
                        printf("%d\t%s\t\t%s\n",lines,tokentype,keyword);
                        append(&list,keyword,tokentype);

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
                        printf("%d\t23\t\t#%s\n",lines,token);
                        append(&list,token,"23");


                    }else{
                        // id
                        fprintf(tokensf,"%d\t22\t\t%s\n",lines,token);
                        printf("%d\t22\t\t%s\n",lines,token);
                        append(&list,token,"22");
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
                        printf("%d\t%s\t\t%s\n",lines,tokentype,keyword);
                        append(&list,token,tokentype);

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
    printf("%d\t5\t\tEND.\n",lines);
    append(&list,"END.","5");
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
void append(struct Node** head_ref, char* new_data , char* new_data2){
    /* 1. allocate node */
    struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));

    struct Node *last = *head_ref;  /* used in step 5*/

    /* 2. put in the data  */
    //new_node->data  = new_data;
    strcpy(new_node->data,new_data);
    strcpy(new_node->type,new_data2);

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


 // ---------------------------------- Parser - Implementation -------------------------------------

 // EXAMPLE :- READ(SUM,X,B,C)
 // NOTE :-
 //         22  id
 //         14  ,
int ID_LIST(){

    int FOUND = 0;
    if(strcmp(list->type,"22")==0){
        FOUND = 1;
        list = list->next;

        while((strcmp(list->type,"14")==0) && FOUND == 1){
            list = list->next;
            if(strcmp(list->type,"22")==0){
                list=list->next;
            }else{
                FOUND = 0;
            }
        }
    }

    if(FOUND == 1){
        return 1;
    }else{
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
        if(strcmp(list->type,"20")==0){
            list = list->next;
            if(ID_LIST() == 1){
                if(strcmp(list->type,"21")==0){
                    FOUND = 1;
                    list = list->next;
                }
            }
        }
    }

    if(FOUND == 1){
        return 1;
    }else{
        return 0;
    }

}

 // NOTE :-
 //         8   READ
 //         20  (
 //         21  )

int WRITE(){

    int FOUND = 0;

    if(strcmp(list->type,"8")==0){
        list = list->next;
        if(strcmp(list->type,"20")==0){
            list = list->next;
            if(ID_LIST() == 1){
                if(strcmp(list->type,"21")==0){
                    FOUND = 1;
                    list = list->next;
                }
            }
        }
    }

    if(FOUND == 1){
        return 1;
    }else{
        return 0;
    }

}

 // NOTE :-
 //         22  id
 //         15  =

int ASSIGN(){

    int FOUND = 0;

    if(strcmp(list->type,"22")==0){
        list = list->next;
        if(strcmp(list->type,"15")==0){
            list = list->next;

            if(EXP() == 1){
                FOUND = 1;
            }
        }

    }

    if(FOUND == 1){
        return 1;
    }else{
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
            if(TERM() == 0){
                FOUND = 0;
            }
        }
    }

    if(FOUND == 1){
        return 1;
    }else{
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
            if(FACTOR() == 0){
                FOUND = 0;
            }
        }
    }

    if(FOUND == 1){
        return 1;
    }else{
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

    }else{

        if(strcmp(list->type,"20")==0){
            list= list->next;
            if(EXP() == 1){
                if(strcmp(list->type,"21")==0){
                    FOUND = 1;
                    list = list->next;
                }

            }
        }
    }

    if(FOUND == 1){
        return 1;
    }else{
        return 0;
    }





}







