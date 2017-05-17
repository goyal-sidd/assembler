#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
char binInst[100][17],ir[17];
int pc=0;
int reg[16]={0},flag[16]={0},hours=-1,minutes=-1,curHour,curMinute,connected[16],stepCount=0,numberOfNotification=0,pulseRate=72;
char pass[5];

void showDetails()
{
    int i;
    printf("general purpose register status-->\n");
    for(i=0;i<16;i++)
    {
        printf("R%d: %d  ",i,reg[i]);
    }
    printf("\n");

    printf("flag register status-->\n");
    for(i=0;i<14;i++)
    {
        printf("F%d: %d  ",i,flag[i]);
    }
    printf("\n");


    printf("connected device status-->\n");
    printf("connection status is %d\n",connected[0]);

    for(i=1;i<16;i++)
    {
        printf("D%d: %d  ",i,connected[i]);
    }
    printf("\n");

    printf("Current time is %d:%d\n",curHour,curMinute);
    printf("Alarm time is %d:%d\n",hours,minutes);
    printf("Your step count is %d\n",stepCount);
    printf("Your pulse rate is %d\n",pulseRate);
    printf("Your have got %d notifications\n",numberOfNotification);


}


void setAlarm(int hour,int minute,int type1,int type2)
{
  	flag[8]=1;//seting flag of alarm as active
  	if(type1==1&&type2==1)
    {
        hours=reg[hour];
        minutes=reg[minute];
  	}
  	else if(type1==1&&type2==2)
  	{
        hours=reg[hour];
        minutes=minute;
  	}
  	else if(type1==2&&type2==1)
  	{
        hours=hour;
        minutes=reg[minute];
  	}
  	else if(type1==2&&type2==2)
  	{
        hours=hour;
   		minutes = minute;
  	}
}

void smartAlarm(int device,int hour,int minute,int type1,int type2,int type3)
{
  	if(type1==1)
    {
        flag[reg[device]]=1;
  	}

  	else if(type1==2)
    {
        flag[device]=1;
  	}
  	setAlarm(hour,minute,type2,type3);
}


void checkAlarm()
{
  	if(curHour==hours&&curMinute==minutes)
    {
        flag[9]=1;
        flag[8]=0;
  	}
    curMinute+=10;
    if(curMinute>=60)
    {
        curHour++;
        curMinute-=60;
        if(curHour>=24)
        {
            curHour=0;
        }
    }
}

void setTime()
{
	curHour=5;
	curMinute=10;
}


void showTime()
{
	printf("Time is %d:%d\n",curHour,curMinute);
}


void connect(int device,int type1)
{
  	connected[0]=1;
  	if(type1==1)
    {
        connected[reg[device]]=1;
  	}
  	else if(type1==2)
  	{
        connected[device]=1;
  	}
}


void setPassword()
{
   pass[0]='1';
   pass[1]='3';
   pass[2]='\0';
}
void checkPassword(int password,int type1)
{
    char p[5];

    if(type1==1)
    {
        password=reg[password];
    }

    itoa(password,p,10);
	if(strcmp(p,pass)==0)
    {
        flag[10]=1;
  	}
  	else
  	{
        flag[10]=0;
  	}
}



void stepCouter(int finish,int type1)
{
	int i;
	stepCount=0;
  	if(type1==1)
    {
        for(i=0;i<reg[finish];i++)
        {
            stepCount++;
  		}
	}
	else if(type1==2)
	{
		for(i=0;i<finish;i++)
		{
            stepCount++;
  		}
	}
}

void heartBeat()
{
	int r=rand()%25;
	pulseRate=r+70;
}



void checkNotif()
{
  	int a=rand()%2;
  	if(a==1)
    {
        numberOfNotification++;
  	}
  	if(numberOfNotification>0)
  	{
    		flag[11]=1;
  	}
}

void showNotification()
{
  	printf("You have %d Notifications\n",numberOfNotification);
  	flag[11]=0;
  	numberOfNotification=0;
}


void move(int source ,int destination,int type1)
{
  	if(type1==2)
    {
        reg[destination]=source;
  	}
  	else if(type1==1)
  	{
        reg[destination]=reg[source];
  	}
}

void add(int source,int destination,int type1)
{
  	if(type1==2)
    {
        reg[destination]=source+reg[destination];
  	}
  	else if(type1==1)
    {
        reg[destination]=reg[source]+reg[destination];
  	}
}

void sub(int source,int destination,int type1)
{
  	if(type1==2)
    {
        reg[destination]=source-reg[destination];
  	}
  	else if(type1==1)
  	{
        reg[destination]=reg[source]-reg[destination];
  	}
}

void mul(int source,int destination,int type1)
{
  	if(type1==2)
    {
        reg[destination]=source*reg[destination];
  	}
  	else if(type1==1)
  	{
        reg[destination]=reg[source]*reg[destination];
  	}
}

void divi(int source,int destination,int type1)
{
  	if(type1==2)
    {
        reg[destination]=source/reg[destination];
  	}
  	else if(type1==1)
  	{
        reg[destination]=reg[source]/reg[destination];
  	}
}

void cmp(int source,int destination,int type1,int type2)
{
    flag[12]=flag[13]=0;
  	if(type1==2&&type2==2)
    {
        if(source>destination)
            return;
        else if(source==destination)
            flag[12]=1;
        else
            flag[13]=1;
  	}

  	else if(type1==1&&type2==2)
    {
        if(reg[source]>destination)
            return;
        else if(reg[source]==destination)
            flag[12]=1;
        else
            flag[13]=1;
  	}

  	else if(type1==2&&type2==1)
    {
        if(source>reg[destination])
            return;
        else if(source==reg[destination])
            flag[12]=1;
        else
            flag[13]=1;
  	}

  	else if(type1==1&&type2==1)
    {
        if(reg[source]>reg[destination])
            return;
        else if(reg[source]==reg[destination])
            flag[12]=1;
        else
            flag[13]=1;
  	}
}


void ld(int source,int destination,int type1)
{
  	if(type1==2)
    {
        reg[destination]=source;
  	}
  	else
  	{
        reg[destination]=reg[source];
  	}
}

void st(int source,int destination,int type2)
{
  	if(type2==2)
    {
        destination=reg[source];
  	}
  	else
  	{
  	  	reg[destination]=reg[source];
    }
}

void halt()
{
  	exit(0);
}

int jump(int addr,char code[])
{

    if(strcmp(code,"JMP")==0)
    {
        return addr/2;
    }
    else if(strcmp(code,"JLT")==0)
    {
        if(flag[13]==1)
        {
            flag[13]=0;
            return addr/2;

        }
    }
    else if(strcmp(code,"JGT")==0)
    {
        if(flag[13]==0)
        {
            return addr/2;
            //flag[13]=0;
        }
    }
    else if(strcmp(code,"JEQ")==0)
    {
        if(flag[12]==1)
        {
            flag[12]=0;
            return addr/2;

        }
    }
    else if(strcmp(code,"JNE")==0)
    {
        if(flag[12]==0)
        {
            return addr/2;
            //flag[12]=0;
        }
    }

    return -1;

}


void calender()
{
	time_t rawtime;
    	struct tm * timeinfo;
    	time ( &rawtime );
	int type_year;
    	timeinfo = localtime ( &rawtime );
	type_year=determineleapyear((timeinfo->tm_year)+1990);
	//printf("Date:- %d\nMonth:- %d\nYear:- %d\n",timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);
	int n;

	//get day number
	n=dayofweek(timeinfo->tm_mday,timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);
	//printf("%d\n",n);

	//make day
	 switch(n)
	   {
		case 1:
		       printf("Monday\n");
		       break;
		case 2:
		       printf("Tuesday\n");
		       break;
		case 3:
		       printf("Wednesday\n");
		       break;
		case 4:
		       printf("Thursday\n");
		       break;
		case 5:
		       printf("Friday\n");
		       break;
		case 6:
		       printf("Saturday\n");
		       break;
		case 0:
		       printf("Sunday\n");
		       break;
	}

	n=(timeinfo->tm_mon+1);
	//to get month name
	  switch(n)
	   {
		case 1:
		       printf("January %d\n",timeinfo->tm_mday);
		       break;
		case 2:
		       printf("February %d\n",timeinfo->tm_mday);
		       break;
		case 3:
		       printf("March %d\n",timeinfo->tm_mday);
		       break;
		case 4:
		       printf("April %d\n",timeinfo->tm_mday);
		       break;
		case 5:
		       printf("May %d\n",timeinfo->tm_mday);
		       break;
		case 6:
		       printf("June %d\n",timeinfo->tm_mday);
		       break;
		case 7:
		       printf("July %d\n",timeinfo->tm_mday);
		       break;
		case 8:
		       printf("August %d\n",timeinfo->tm_mday);
		       break;
		case 9:
		       printf("September %d\n",timeinfo->tm_mday);
		       break;
		case 10:
		       printf("October %d\n",timeinfo->tm_mday);
		       break;
		case 11:
		       printf("November %d\n",timeinfo->tm_mday);
		       break;
		case 12:
		       printf("December %d\n",timeinfo->tm_mday);
		       break;
		default:
		       printf("invalid Month number\nPlease try again ....\n");
		       break;
	   }
	printf("%d\n",timeinfo->tm_year + 1900);

}


int determineleapyear(int year)
{
	if(year% 4 == 0 && year%100 != 0 || year%400 == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int dayofweek(int d, int m, int y)
{
    int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
    y -= m < 3;
    return ( y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
}


//R -> 1, # ->2 , @->3 , only integer->4 , $->5

void getInstruction(char binFile[])
{
    FILE *read=fopen(binFile,"r");
    if(read==NULL)
    {
        printf("Code not found\n");
        return;
    }

    char inst[17];
    int i=0;
    while(fscanf(read,"%s",inst)!=EOF)
    {
        strcpy(binInst[i],inst);
        i++;
    }

    /*int j=i;
    for(i=0;i<j;i++)
    {
        printf("%s\n",binInst[i]);
    }
    */

    fclose(read);

}

int convertBinaryToDecimal(int n)
{
    int decimalNumber = 0, i = 0, remainder;
    while (n!=0)
    {
        remainder = n%10;
        n /= 10;
        decimalNumber += remainder*pow(2,i);
        ++i;
    }
    return decimalNumber;
}

int extractInteger(int st,int end,int i)
{
    char str[20];
    int k;
    for(k=0;st<=end;st++,k++)
        str[k]=binInst[i][st];
    str[k]='\0';
    int decimalNumber = atoi(str);
    decimalNumber = convertBinaryToDecimal(decimalNumber);
    return decimalNumber;

}



void decodeInstruction()
{
    FILE *opTa=fopen("binaryCod.txt","r");

    if(opTa==NULL)
    {
        printf("Code not found\n");
        return;
    }
    //printf("HELLO");
    int opType1[100],opType2[100],opType3[100],op1[100],op2[100],op3[100],addr[100],i=0;
    char code[100][20];
    while(fscanf(opTa,"%s %d %d %d %d",code[i],&opType1[i],&opType2[i],&opType3[i],&addr[i])!=EOF)
    {
        //printf("%s %d %d %d %d\n",code,opType1,opType2,opType3,addr);
        op1[i]=-1;op2[i]=-1;op3[i]=-1;

        if(opType1[i]==-1)
        {
            op1[i]=-1;op2[i]=-1;op3[i]=-1;
        }

        else if(opType2[i]==-1)
        {
            op1[i]=extractInteger(10,15,i);
            //printf("%d\n",op1);
        }

        else if(opType3[i]==-1)
        {
            if(strcmp(code[i],"ALA")==0)
            {
                op1[i]=extractInteger(4,9,i);
                op2[i]=extractInteger(10,15,i);
            }
            else
            {
                op1[i]=extractInteger(6,10,i);
                op2[i]=extractInteger(11,15,i);
            }
            //printf("%d %d\n",op1,op2);
        }

        else
        {
            op1[i]=extractInteger(4,7,i);
            op2[i]=extractInteger(8,11,i);
            op3[i]=extractInteger(12,15,i);
            //printf("%d %d %d\n",op1,op2,op3);
        }

        i++;pc+=2;
    }

    int j=i;
    /*for(i=0;i<j;i++)
    {
        printf("%s %d %d %d %d %d %d\n",code[i],op1[i],op2[i],op3[i],opType1[i],opType2[i],opType3[i]);
    }*/




    int temp;
    i=0;
    while(i<j)
    {
        strcpy(ir,binInst[i]);
        pc=i*2;
        if(strcmp(code[i],"SALA")==0)
        {
            smartAlarm(op1[i],op2[i],op3[i],opType1[i],opType2[i],opType3[i]);

        }
        /*else if(strcmp(code,"CALC")==0)
        {
            setAlarm(op1,op2,op3,opType1,opType2,opType3); // not done
        }*/
        else if(strcmp(code[i],"ALA")==0)
        {
            setAlarm(op1[i],op2[i],opType1[i],opType2[i]);
        }
        else if(strcmp(code[i],"MOV")==0)
        {
            move(op1[i],op2[i],opType1[i]);
        }
        else if(strcmp(code[i],"ADD")==0)
        {
            add(op1[i],op2[i],opType1[i]);
        }
        else if(strcmp(code[i],"SUB")==0)
        {
            sub(op1[i],op2[i],opType1[i]);
        }
        else if(strcmp(code[i],"MUL")==0)
        {
            mul(op1[i],op2[i],opType1[i]);
        }
        else if(strcmp(code[i],"DIV")==0)
        {
            divi(op1[i],op2[i],opType1[i]);
        }
        else if(strcmp(code[i],"CMP")==0)
        {
            cmp(op1[i],op2[i],opType1[i],opType2[i]);
        }
        else if(strcmp(code[i],"LD")==0)
        {
            ld(op1[i],op2[i],opType1[i]);
        }
        else if(strcmp(code[i],"ST")==0)
        {
            st(op1[i],op2[i],opType2[i]);
        }
        else if(strcmp(code[i],"CONN")==0)
        {
            connect(op1[i],opType1[i]);
        }
        else if(strcmp(code[i],"PASS")==0)
        {
            checkPassword(op1[i],opType1[i]);
        }
        else if(strcmp(code[i],"STCO")==0)
        {
            stepCouter(op1[i],opType1[i]);
        }
        else if(strcmp(code[i],"JMP")==0)
        {
            temp=jump(op1[i],code[i]);
            if(temp!=-1)
                i=temp-1;
        }
        else if(strcmp(code[i],"JLT")==0)
        {
            temp=jump(op1[i],code[i]);
            if(temp!=-1)
                i=temp-1;
        }
        else if(strcmp(code[i],"JGT")==0)
        {
            temp=jump(op1[i],code[i]);
            if(temp!=-1)
                i=temp-1;
        }
        else if(strcmp(code[i],"JEQ")==0)
        {
            temp=jump(op1[i],code[i]);
            if(temp!=-1)
                i=temp-1;
        }
        else if(strcmp(code[i],"JNE")==0)
        {
            temp=jump(op1[i],code[i]);
            if(temp!=-1)
                i=temp-1;
        }
        else if(strcmp(code[i],"TIME")==0)
        {
            showTime();
        }
        else if(strcmp(code[i],"HBT")==0)
        {
            heartBeat();
        }
        else if(strcmp(code[i],"CALE")==0)
        {
            calender();
        }
        else if(strcmp(code[i],"NOTI")==0)
        {
            showNotification();
        }
        else if(strcmp(code[i],"HALT")==0)
        {
            halt();
        }

        checkNotif();
        checkAlarm();
        printf("IR STATUS--> %s\n",ir);
        printf("PC STATUS--> %d\n",pc);
        showDetails();
        printf("\n\n");


        i++;
    }

    fclose(opTa);
}

void decode()
{
    char binFile[50];
    printf("input the file name....");
    scanf("%s",binFile);
    getInstruction(binFile);
    decodeInstruction();
}

int main()
{
    printf("FLAGS 0-7 SIGNIFY DEVICE CONNECTED FOR SMART ALARM\n");
    printf("FLAG 8 IS SET WHEN WE SET THE ALARM\n");
    printf("FLAG 9 IS SET WHEN CURRENT TIME IS EQUAL TO THE ALARM TIME\n");
    printf("FLAG 10 IS SET WHEN PASSWORD ENTERD IS CORRECT\n");
    printf("FLAG 11 IS SET WHEN WE RECIEVE A NOTIFICATION\n");
    printf("FLAG 12 IS SET WHEN COMPARE STATEMENT RESULTS IN  EQUALITY\n");
    printf("FLAG 13 IS SET WHEN COMPARE STATEMENT RESULT IS NEGATIVE\n\n\n");



    setPassword();
    setTime();
    decode();
    return 0;
}
