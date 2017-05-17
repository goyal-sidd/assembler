d//correct label..program countter

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<ctype.h>

// hash table nodes..created using chaining
struct Opcode
{
        char name[20];
        char code[35];
        struct Opcode *next;
};

struct Symbol
{
        char name[20];
        int addr;
        struct Symbol *next;
};


struct OpTable
{
        char name[20];
        int type[3];
        int addr;
        struct OpTable *next;
};

typedef struct Opcode Opcode;
typedef struct Symbol Symbol;
typedef struct OpTable OpTable;
Opcode* hash_table[13] = {NULL};
Opcode* register_table[30] = {NULL};

Symbol* sym_table[13]={NULL};

OpTable* op_table[13] = {NULL};

/***********************************************************************************
                UTILITY FUNCTIONS
***********************************************************************************/

//hashes the given string
int hashing(char code[])
{
    int sum=0,i;
    for(i=0;code[i]!='\0';i++)
        sum+=code[i];

    return sum%13;

}

//returns register number or label number
//eg. returns 15 for name[]=R15 or 2 for
//name[]=L2:
int getNumber(char name[])
{
    int i;
    int ind=0,pro=1,temp=0;//=name[strlen(name)-1]-'0';
    for(i=strlen(name)-1;(name[i]>='0' && name[i]<='9') || (name[i]==':');i--)
    {
        if(name[i]==':')
            continue;
        temp=(name[i]-'0')*pro;
        ind+=temp;
        pro*=10;
    }
    return ind;
}

//function to reverse a string
void strr(char *s)
{
    int i,j,l=strlen(s);
    char c;
    for(i=0,j=l-1;i<j;i++,j--)
    {
        c=s[i];
        s[i]=s[j];
        s[j]=c;
    }
}

//converts integer to binary number of length 'len'
void convertAddrToBinary(int num,int len,char *mem)
{
    int i;
    for(i=0;i<len;i++)
    {
        mem[i]=num%2+'0';
        num>>=1;
    }
    mem[i]='\0';
    strr(mem);
}


//secondPass utility
//finds length of operators
int getBinDigitCount(char temp[],int leen)
{
    int kk;
    if(16-leen == 12 && strcmp(temp,"ALA")!=0)
        kk=4;
    else if(16-leen == 12 && strcmp(temp,"ALA")==0)
        kk=6;
    else if(16-leen == 10)
        kk=5;
    else if(16-leen == 6)
        kk=6;
    return kk;
}

//secondPass utility
//removes first element of the string
void shift(char tt[])
{
    int jj=1;
    while(tt[jj]!='\0')
        {tt[jj-1]=tt[jj];jj++;}
    tt[jj-1]='\0';
}

//secondPass utility
//prints leading zeros if needed
void printLeadinZero(char temp[],int leen,FILE* write)
{
    if(16-leen == 12 && strcmp(temp,"ALA")!=0)
        ;
    else if(16-leen == 12 && strcmp(temp,"ALA")==0)
        fprintf(write,"%s","00");
    else if(16-leen == 10)
        fprintf(write,"%s","0");
    else if(16-leen == 6)
        fprintf(write,"%s","00");
}


//firstPass utility
//returns type of operator
//R -> 1, # ->2 , @->3 , only integer->4 , $->5
void getOperandType(char s[],int type[])
{
    int i=0,c=0,j=0;
    while(s[i]!='\0')
    {
        if(s[i]==' ')
        {
            if(s[i+1]=='R')
                type[j]=1;

            else if(s[i+1]=='#')
                type[j]=2;

            else if(s[i+1]=='@')
                type[j]=3;

            else if(s[i+1]>='0' && s[i+1]<='9')
                type[j]=4;
            else if(s[i+1]=='$')
                type[j]=5;
            //else
              //  *flag=0;
            j++;
        }
        i++;
    }
}

/**^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/


/***********************************************************************************
 OPCODES FUNCTIONS
***********************************************************************************/

void insertIntoHashTable(Opcode* Node,int ind)
{
    if(hash_table[ind] == NULL)
    {
        hash_table[ind] = Node;
        Node->next = NULL;
    }
    else
    {
        Opcode* temp = hash_table[ind]; //confusion/.......
        Node->next = temp->next;
        temp->next = Node;
    }
}


void makeHashTable(Opcode *Node)
{
    int i=hashing(Node->name);
    insertIntoHashTable(Node,i);
}

void getOpcodes()
{
    FILE *read_opcode;
    read_opcode = fopen("opcode.txt","r+");  //opcode.txt contains a list of opcodes followed by their binary codes
    if (read_opcode == NULL)
        printf("FILE OPENING PROBLEM");
    else
    {

        char opcode[100];
        char binary_code[100];
        while(fscanf(read_opcode,"%s %s",opcode,binary_code)!=EOF)
        {
           Opcode* Node = malloc(sizeof(Opcode));
           strcpy(Node->name,opcode);
           strcpy(Node->code,binary_code);
           makeHashTable(Node);

           //printf("%s %s\n",Node->name,Node->code);
        }
    }
    fclose(read_opcode);
}

void printHashTable()
{
    int i=0;
    for(i=0;i<13;i++)
    {

        if(hash_table[i]!=NULL)
        {

            Opcode* temp = hash_table[i];

            while(temp!=NULL)
            {

                printf("NAME:: %s and CODE:: %s\n",temp->name,temp->code);
                temp = temp->next;
            }
        }
    }
}


Opcode* findOpCode(char s[])
{
    int i=hashing(s);
    if(hash_table[i]!=NULL)
    {
        Opcode* temp = hash_table[i];
        while(temp!=NULL)
        {
            if(strcmp(temp->name,s)==0)
                break;
            //printf("NAME:: %s and CODE:: %s\n",temp->name,temp->code);
            temp = temp->next;
        }
        return temp;
    }
    else
        return NULL;
}

/**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
**/


/***********************************************************************************
 REGISTER FUNCTIONS
***********************************************************************************/



void insertIntoRegisterTable(Opcode* Node,int ind)
{
    register_table[ind]=Node;
}

void makeRegisterTable(Opcode* Node)
{
    int i=getNumber(Node->name);
    insertIntoRegisterTable(Node,i);

}

void getRegisterCode()
{
    FILE *reg;
    reg = fopen("register.txt","r+");
    if (reg == NULL)
        printf("FILE OPENING PROBLEM");
    else
    {

        char registers[100];
        char binary_code[100];
        while(fscanf(reg,"%s %s",registers,binary_code)!=EOF)
        {
           Opcode* Node = malloc(sizeof(Opcode));
           strcpy(Node->name,registers);
           strcpy(Node->code,binary_code);
           makeRegisterTable(Node);

           //printf("%s %s\n",Node->name,Node->code);
        }
    }
    fclose(reg);
}

void printRegisterTable()
{
    int i=0;
    for(i=0;i<17;i++)
    {
        if(register_table[i]!=NULL)
        {
            Opcode* temp = register_table[i];
            printf("REGISTER:: %s and CODE:: %s\n",temp->name,temp->code);
        }
    }
}

Opcode* findRegister(char s[])
{
    int i=getNumber(s);
    return register_table[i];
}

/**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
**/




/***********************************************************************************
 SYMBOL FUNCTIONS
***********************************************************************************/

void insertIntoSymbTable(Symbol* Node,int ind)
{
    if(sym_table[ind] == NULL)
    {
        sym_table[ind] = Node;
        Node->next = NULL;
    }
    else
    {
        Symbol* temp = sym_table[ind]; //confusion/.......
        Node->next = temp->next;
        temp->next = Node;
    }
    //printf("%s %d\n",Node->name,Node->addr);
}

void makeSymbTable(Symbol* Node)
{
    int i=hashing(Node->name);
    //printf("%d ",i);
    insertIntoSymbTable(Node,i);

}


Symbol* findLabel(char s[])
{
    //printf("%s ",s);
    int i= hashing(s);

    if(sym_table[i]!=NULL)
    {
        Symbol* temp = sym_table[i];
        while(temp!=NULL)
        {
            if(strcmp(temp->name,s)==0)
                break;
            //printf("NAME:: %s and CODE:: %s\n",temp->name,temp->code);
            temp = temp->next;
        }
        return temp;
    }
    else
        return NULL;
}

/**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
**/



/***********************************************************************************
 OP-TABLE FUNCTIONS
***********************************************************************************/
void insertIntoOpTable(OpTable* Node,int ind)
{

    if(op_table[ind] == NULL)
    {
        op_table[ind] = Node;
        Node->next = NULL;
    }
    else
    {
        OpTable* temp = op_table[ind];
        Node->next = temp->next;
        temp->next = Node;
    }
}

void makeOpTable(OpTable* Node)
{
    int i=hashing(Node->name);

    insertIntoOpTable(Node,i);
}

/**
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
**/


void firstPass(char instructionFile[])
{
    FILE *read=fopen(instructionFile,"r");
    FILE *sym=fopen("symbolTable.txt","w");
    FILE *opTa=fopen("OpTable.txt","w");
    FILE *opTa1=fopen("binaryCod.txt","w");
    if(read==NULL)
    {
        printf("Assembly Code not found\n");
        return;
    }

    char s[100];
    int line_count=0;
    while(fgets(s,100,read)!=NULL)
    {

        s[strlen(s)-1]='\0';
        int i=0,f=0;
        char temp[100];
        while(s[i]!=' ' || s[i]!='\0')
        {
            temp[i]=s[i];
            if(s[i]==':')
                {f=1;break;}
            else if(s[i]==' ')
                break;
            else if(s[i]=='\0')
                break;
            i++;
        }
        //printf("%s ",s);
        if(f==1)
        {
            //printf("%d ",line_count);
            Symbol* Node = malloc(sizeof(Symbol));
            strcpy(Node->name,s);
            Node->addr=(line_count)*2;
            line_count--;
            fprintf(sym,"%s %d\n",Node->name,Node->addr);
            makeSymbTable(Node);
        }
        else
        {

            temp[i]='\0';
            OpTable* Node = malloc(sizeof(OpTable));
            for(i=0;i<3;i++)
                Node->type[i]=-1;
            getOperandType(s,Node->type);
            strcpy(Node->name,temp);
            Node->addr=line_count*2;
            fprintf(opTa,"%s %d %d %d %d\n",Node->name,Node->type[0],Node->type[1],Node->type[2],Node->addr);
            fprintf(opTa1,"%s %d %d %d %d\n",Node->name,Node->type[0],Node->type[1],Node->type[2],Node->addr);
            makeOpTable(Node);
        }
        line_count++;
    }
    fclose(read);
    fclose(sym);
    fclose(opTa);
}


void secondPass(char instructionFile[])
{
    FILE *read=fopen(instructionFile,"r");
    FILE *write=fopen("binaryCode.txt","w");
    if(read==NULL)
    {
        printf("Assembly Code not found\n");
        return;
    }

    char s[100];
    while(fgets(s,100,read)!=NULL)
    {

        s[strlen(s)-1]='\0';
        //printf("%s ",s);
        int i=0,f=0;
        char temp[100];
        while(s[i]!=' ' || s[i]!='\0')
        {
            temp[i]=s[i];
            if(s[i]==':')
                {f=1;break;}
            else if(s[i]==' ')
                break;
            else if(s[i]=='\0')
                break;
            i++;
        }
        if(f==1)
        {

            //printf("%s ",s);
            Symbol* Node;
            Node=findLabel(s);
            char bin[17];
            convertAddrToBinary(Node->addr,16,bin);
            //fprintf(write,"%s\n",bin);
        }

        else
        {

            temp[i]='\0';
            Opcode* Node;
            Node=findOpCode(temp);
            fprintf(write,"%s",Node->code);
            int leen=strlen(Node->code);
            while(i<strlen(s))
            {
                i++;
                char tt[100];
                int jj=0;
                while(s[i]!=' ')
                {
                    tt[jj]=s[i];
                    i++;jj++;
                }
                tt[jj]='\0';
                if(tt[0]=='#')
                {
                    //printf("%s ",tt);
                    shift(tt);
                    int num=atoi(tt),kk=getBinDigitCount(temp,leen);
                    char bin[17];
                    convertAddrToBinary(num,kk,bin);
                    fprintf(write,"%s",bin);
                }

                else if(tt[0]=='@')
                {
                    shift(tt);
                    jj=1;
                    Opcode* Node;
                    Node = findRegister(tt);
                    printLeadinZero(temp,leen,write);
                    fprintf(write,"%s",Node->code);

                }
                else if(tt[0]=='R')
                {
                    //printf("%s ",tt);
                    Opcode* Node;
                    Node = findRegister(tt);
                    printLeadinZero(temp,leen,write);
                    fprintf(write,"%s",Node->code);
                }
                else if(tt[0]>='0' && tt[0]<='9')
                {
                    //printf("%s ",tt);
                    char bin[17];
                    int num=atoi(tt),kk=getBinDigitCount(temp,leen);
                    convertAddrToBinary(num,kk,bin);
                    fprintf(write,"%s",bin);
                }
                else if(tt[0]=='$')
                {
                    shift(tt);
                    int ln=strlen(tt);
                    tt[ln]=':';
                    tt[ln+1]='\0';

                    //printf("%s %d",tt,strlen(tt));
                    Symbol* Node;
                    Node=findLabel(tt);
                    //printf("%s %d",Node->name,Node->addr);
                    char bin[17];
                    int kk=getBinDigitCount(temp,leen);
                    convertAddrToBinary(Node->addr,kk,bin);
                    fprintf(write,"%s",bin);
                }
            }
             fprintf(write,"\n");

        }
    }
    fclose(read);
    fclose(write);
}


int main()
{
    getOpcodes();
    printf("Hash-table Created Successfully!\n\n");

    printf("\n");
    getRegisterCode();
    printf("Register-table Created Successfully!\n\n");


    char instructionFile[100];


int choice=0;
while(choice!=4)
 {
     printf("**********Menu*************\n");
     printf("Press 1 to Print Opcode Table\n");
     printf("Press 2 to Print Register Table\n");
     printf("Press 3 to Convert Assembly to binary\n");
     printf("Press 4 to EXIT\n");

     scanf("%d",&choice);
     printf("\n");
     switch(choice)
     {
     case 1:
         {
             printHashTable();
             break;
         }
     case 2:
         {
             printRegisterTable();
             break;
         }
     case 3:
         {
           printf("Enter the file name...");
           scanf("%s",instructionFile);
           firstPass(instructionFile);
           secondPass(instructionFile);
           break;
         }

     }
     printf("\n\n");
 }

}
