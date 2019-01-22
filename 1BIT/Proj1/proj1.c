#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_LENGTH_OF_INST 2008 //int_char+2*separator+2*MAX_LENGTH_OF_LINE+\n
#define MAX_LENGTH_OF_LINE 1002 //1000char+\n+\0  

void find_pattern_or_replacement (char *pattern, char *saved_inst, int start){
    int counter = 0;
    while (saved_inst[start] != '\n') {
        pattern[counter] = saved_inst[start];
        counter++;
        start++;
    }
    pattern[counter]= '\0';
}

void check_actln_ended (char *act_line, bool *actln_ended){
    *actln_ended = true;
    if (act_line[strlen(act_line)-1] != '\n'){
        *actln_ended = false;
    }
}

int _goto (char saved_inst[][2008] , int i, int last_i){
    //gX
    //cX pattern
    if(saved_inst[i][1]<='9' && saved_inst[i][1]>='1'){
        char temp_inst[strlen(saved_inst[i])];
        char *end_of_int;
        int skipto = 0;
        memset(temp_inst,'\0',strlen(temp_inst));
        strcpy (temp_inst,saved_inst[i]);
        //load how many instructions to skip
        skipto = strtol ( &saved_inst[i][1], &end_of_int , 10 );
        if (*end_of_int == '\n' || (*end_of_int == ' ' && *end_of_int+1 != '\n')){
            if (skipto != 0){     
                int old_i = i;
                //load index to go to
                i = skipto -1;
                //going back? (cycle detection)
                if (old_i >= i){
                    //same inst.
                    int temp_i= i;
                    if (old_i == temp_i){
                        fprintf(stderr, "Cycle detected\n");
                        return -1;
                    }
                    else{
                        while ((saved_inst[temp_i][0] != 'i' && saved_inst[temp_i][0] != 'd' && saved_inst[temp_i][0] != 'n') && old_i != temp_i){
                                temp_i++;
                        }
                        if (old_i == temp_i){
                            fprintf(stderr, "Cycle detected\n");
                            return -1;
                        }
                    }
                }
                if (i> last_i-1){
                    fprintf(stderr, "Instruction- %cX: invalid format\n",saved_inst[i][0]);
                    return -1;
                }
            }
            //invalid format g0/c0
            else{
                fprintf(stderr, "Instruction- %cX: invalid format\n",saved_inst[i][0]);
                return -1;            
            }
        }
        //invalid format g/c>number>something
        else{
            fprintf(stderr, "Instruction- %cX: invalid format\n",saved_inst[i][0]);
            return -1;            
        }
    }
    //invalid format g/c>something_other_than_number 
    else{
        fprintf(stderr, "Instruction- %cX: invalid format\n",saved_inst[i][0]);
        return -1;    
    }
    return i;
}

char *str_replace (char *pattern , char *replace , char *act_line, char first_char_inst, int* i, bool *edited){

    char  *p = NULL , *old = NULL , new_act_line[1001] ;
    int c = 0 , pattern_size;     
    pattern_size = strlen(pattern);     
    //count how many occurences
    p = strstr(act_line , pattern);
    while (p != NULL){
        c++;
        p = strstr(p + pattern_size , pattern);
    }
    if (c==0){
      return act_line;
    }
    else{
        *edited= true;
        //s overflow
        if (first_char_inst== 's'){
            if (strlen(act_line) - strlen(pattern) + strlen(replace) >1001){
                fprintf(stderr, "Instruction- %c: edited line exceeded limit\n",first_char_inst);
                *i= -1;
                return NULL;
            }
        }
        //S overflow
        if (first_char_inst== 'S'){
            if (strlen(act_line) - (c * strlen(pattern)) + (c * strlen(replace)) >1001){
                fprintf(stderr, "Instruction- %c: edited line exceeded limit\n",first_char_inst);
                *i=  -1;
                return NULL;
            }
        }
         //final size
        c = ( strlen(replace) - pattern_size )*c + strlen(act_line);         
        //set it to blank
        memset(new_act_line ,'\0',sizeof(new_act_line));     
        //start position
        old = act_line;
        p = strstr(act_line , pattern);
        do {
            //move ahead and copy some text from original act_line , from a certain position
            strncpy(new_act_line + strlen(new_act_line) , old , p - old);         
            //move ahead and copy the replacement text
            strcpy(new_act_line + strlen(new_act_line) , replace);         
            //new start position after this pattern match
            old = p + pattern_size;
            p = strstr(p + pattern_size , pattern);
        } while (p != NULL &&  first_char_inst == 'S');  
        //copy the part after the last pattern match
        strcpy(new_act_line + strlen(new_act_line) , old); 
        memset(act_line ,'\0',strlen (act_line) ); 
        strcpy(act_line , new_act_line);     
        return act_line;
    }
}

int edit (char *act_line, char saved_inst[100][MAX_LENGTH_OF_INST], int i, int last_i, bool *actln_ended,bool *edited) {
    char content[strlen(saved_inst[i])-1];
    if (saved_inst[i][0]== 'i'|| saved_inst[i][0]== 'b' || saved_inst[i][0]== 'a'){
            //fill string:content with CONTENT
        int start=1;
        find_pattern_or_replacement (&content[0], saved_inst[i], start);
    }
    if (saved_inst[i][0]== 'i'){
        printf ("%s\n",content);
        i++;
    }
    if (saved_inst[i][0]== 'b' || saved_inst[i][0]== 'a'){
        char temp_act_line[1001]="";
            //strlen= length of string without \n
        if( strlen(content) + strlen(act_line) <= 1000){
            //fill temporary constant
            strcpy(temp_act_line, act_line);
        }
        else{
            fprintf(stderr, "Error:\nInstruction file\n Line %d\nInstruction- %c: length of line exceeded limit\n",i,saved_inst[i][0]);
            return -1;
        }

        if (saved_inst[i][0]== 'b'){
            //load: CONTENT into act_line
            memset(act_line,'\0',strlen(act_line));
            sprintf(act_line, "%s%s", content, temp_act_line);
            *edited= true;
            i++;
        }
        else if (saved_inst[i][0]== 'a'){
            memset(act_line,'\0',strlen(act_line));
            bool newln_ended=0;
            //check line ending- does it end with \n?
            if (temp_act_line[strlen(temp_act_line)-1]=='\n'){
                newln_ended = true;
            }
            //delete newline in temp
            if (newln_ended == true){
                temp_act_line[strlen(temp_act_line)-1]='\0';
            }
                //add content
            sprintf(act_line,"%s%s",temp_act_line, content);
            //if line started with newln, return it to the end
            if (newln_ended == true){
                 act_line[strlen(act_line)+1]='\0';
                act_line[strlen(act_line)]='\n';
            }
            *edited= true;
            i++;
        }
    }

    else if (saved_inst[i][0]== 'r'){
        if(saved_inst[i][1] == '\n'){
            //check line ending- does it end with \n?
            if (act_line[strlen(act_line)-1]=='\n'){
                *actln_ended = false;
                //delete newline in temp
                act_line[strlen(act_line)-1]='\0';
                *edited= true;
            }
            i++;
        }
        else{
            fprintf(stderr, "Instruction- r: invalid format\n");
            return -1;
        }
    }
    else if (saved_inst[i][0]== 's' || saved_inst[i][0]== 'S'){
        if (saved_inst[i][1] != '\n'){
            char separator = saved_inst[i][1];
            char pattern[1001], replace[1001];
            memset(pattern,'\0',strlen(pattern));
            int  j = 2, counter = 0;
            //put to function
            while (saved_inst[i][j] != separator && saved_inst[i][j] != '\n') {
                pattern[counter] = saved_inst[i][j];
                counter++;
                j++;
            }

            if (saved_inst[i][j] == '\n'){
                fprintf(stderr, "Instruction- %c: invalid format- second separator undeclared\n",saved_inst[i][0]);
                return -1;
            }
            j++;        
            find_pattern_or_replacement (&replace[0], saved_inst[i], j);
            act_line= str_replace(pattern, replace, act_line, saved_inst[i][0],&i,edited);
            i++;
        }
        else { 
            fprintf(stderr, "Instruction- %c: invalid format- separator undeclared\n",saved_inst[i][0]);
            return -1;
        }
    }
    else if (saved_inst[i][0]== 'g'){
        i= _goto(saved_inst, i, last_i);
    }
    else if (saved_inst[i][0]== 'c'){
        char pattern[1001], *end_of_int;
        strtol ( &saved_inst[i][1], &end_of_int , 10 );
        //difference between addresses + ' '
        int start = end_of_int - &saved_inst[i][0] +1;
        find_pattern_or_replacement (&pattern[0], saved_inst[i], start);
        if ((strstr(act_line, pattern)) != NULL){
            i= _goto (saved_inst, i, last_i);
        }
        else{
            //next instr.
            i++;
            return i;
        }
    }
    else if (saved_inst[i][0]== 'e'){
        //put \n as last char of active line(from stdin) 
        act_line[strlen(act_line)+1]='\0';
        act_line[strlen(act_line)]='\n';
        *edited= true;
        i++;
    }
    else if (saved_inst[i][0]== 'f'){ 
        char pattern[1001];
        int  start_at= 1;
        find_pattern_or_replacement (&pattern[0], saved_inst[i], start_at);
        while(strstr(act_line , pattern) == NULL && act_line[0] != '\0'){;
            //print or delete !?
            printf ("%s",act_line);
            fgets(act_line, MAX_LENGTH_OF_LINE , stdin);
        }
        i++;
    }
    return i;
}

int crossroads (char saved_inst [100][MAX_LENGTH_OF_INST], int i, char *act_line, int last_i, bool *actln_ended){
       //edit till n/d/q

    char _char= saved_inst[i][0];
    bool edited=false;
    while ((_char=='i' || _char== 'b' || _char== 'a' || _char== 'S' || _char== 'r' || _char== 'f' || _char== 's' || _char== 'g' || _char== 'c' || _char== 'e')
          && act_line != NULL ){
            i= edit (&act_line[0], saved_inst, i, last_i, actln_ended, &edited);
            _char= saved_inst[i][0];
    } //no more editing instructions... for now

    if (saved_inst[i][0]== 'd'){
        if(saved_inst[i][1]== '\n'){
            i++;
            return i;
        }
        else if(saved_inst[i][1]<='9' && saved_inst[i][1]>='1'){
            char temp_inst[strlen(saved_inst[i])];
            char *end_of_int;
            int rep = 0;
            memset(temp_inst,'\0',strlen(temp_inst));
            strcpy (temp_inst,saved_inst[i]);
            //load how many times to repeat
            rep = strtol ( &saved_inst[i][1], &end_of_int , 10 );
            if (*end_of_int == '\n'){
                if (rep != 0){
                    //repeat d (rep times)
                    while(rep > 1 && fgets(act_line, MAX_LENGTH_OF_LINE , stdin) != NULL){
                            rep--;
                    }
                    i++;
                    return i;
                }
            }
            //invalid format d>number>something
            else{
                fprintf(stderr, "Instruction- dN: invalid format\n");
                return -1;            
            }
        }
        //invalid format d>something_other_than_number 
        else{
            fprintf(stderr, "Instruction- dN: invalid format\n");
            return -1;            
        }
    }

    else if (saved_inst[i][0]== 'n'){
        if(saved_inst[i][1]== '\n'){
            check_actln_ended (act_line, actln_ended);
            printf ("%s",act_line);
            i++;
            return i;
        }
        else if(saved_inst[i][1]<='9' && saved_inst[i][1]>='1'){
            char temp_inst[strlen(saved_inst[i])];
            char *end_of_int;
            int rep = 0;
            memset(temp_inst,'\0',strlen(temp_inst));
            strcpy (temp_inst,saved_inst[i]);
            //load how many times to repeat
            rep = strtol ( &saved_inst[i][1], &end_of_int , 10 );
            check_actln_ended (act_line, actln_ended);
            if (rep>=2) {
                *actln_ended = true;
            }
            if (*end_of_int == '\n'){
                if (rep != 0){
                    //repeat n (rep times)
                    printf ("%s",act_line);
                    while(rep > 1 && fgets(act_line, MAX_LENGTH_OF_LINE , stdin) != NULL){
                            check_actln_ended (act_line, actln_ended);
                            printf ("%s",act_line);
                            rep--;
                    }
                    i++;
                    return i;
                }
            }
            //invalid format d>number>something
            else{
                fprintf(stderr, "Instruction- nN: invalid format\n");
                return -1;            
            }
        } 
        //invalid format d>something_other_than_number
        else{
            fprintf(stderr, "Instruction- nN: invalid format\n");
            return -1;            
        }
    }

    else if (saved_inst[i][0]== 'q'){
        if(saved_inst[i][1] == '\n'){
            check_actln_ended (act_line, actln_ended);
            if (edited == true){
                printf ("%s",act_line);
            }            
            return -1;
        }
        else{
            fprintf(stderr, "Instruction- r: invalid format\n");
            return -1;
        }
    }
    
    if (i==last_i){
      if (act_line != NULL){
        printf ("%s",act_line);
    }
      while (fgets(act_line, MAX_LENGTH_OF_LINE , stdin) != NULL){
              printf ("%s",act_line);
      }
      return -1;
    }

    else {
        if (i != -1){
            fprintf(stderr, "Error:\nInstruction file\nLine %d\nInstruction format invalid\n",i+1);
            return -1;
        }
    }
    return i;
}

int main(int argc, char *argv[]){
    if (argc != 2){
        printf("Use: write 1 argument with name of instruction file\n");
        return 0;
    }
    else{
        FILE *subor;
        subor = fopen(argv[1], "r");
        int i=0, last_i=0;
        char act_inst [MAX_LENGTH_OF_INST], saved_inst [100][MAX_LENGTH_OF_INST];
        if (subor != NULL) {

            while ( i <= 101 && fgets( act_inst, MAX_LENGTH_OF_INST-1 , subor) != NULL){
                if(i!=101){
                    strcpy (saved_inst[i], act_inst);
            last_i ++;
                }
                else{
                    fprintf(stderr, "Number of instructions exceeded limit\n");
                    return 1;
                }
                i++;
            }
        }
        else{
            fprintf(stderr, "Couldn't load the file containing instructions\n");
        }
        fclose(subor);
        bool actln_ended=true;
            //load stdin
        char act_line[MAX_LENGTH_OF_LINE];
        i=0;
        while(i != -1  &&  saved_inst[i]!=NULL){
            if (fgets(act_line, MAX_LENGTH_OF_LINE , stdin) != NULL){
                check_actln_ended (act_line, &actln_ended);
                i= crossroads (saved_inst, i, act_line,last_i, &actln_ended);
            }
            else {
                i=-1;
            }
        }
        //if last line didn't end with newln, add it
        if (actln_ended != true){
            printf ("\n");
        }
        return 0;
    }
}

