#include <stdio.h>

int reg[16]={0},flag[16]={0},hours,minutes,curHour,curMinute,connected[16],stepCount,numberOfNotification=0;
//char password[4]={'1','2','3','4'};
void smartAlarm(int device,int hour,int minute,int type1,int type2,int type3 ){
  	if(type1==1){
  	  	flag[reg[device]]=1;

  	}
  	else if(type1==2){
   		 flag[device]=1;
  	}
  	setAlarm(hour,minute,type2,type3);
}
void setAlarm(int hour,int minute,int type1,int type2){
  	flag[8]=1;//seting flag of alarm as active
  	if(type1==1&&type2==1){
    		hours=reg[hour];
    		minutes=reg[minute];
  	}
  	else if(type1==1&&type2==2){
    		hours=reg[hour];
    		minutes=minute;
  	}
  	else if(type1==2&&type2==1){
    		hours=hour;
    		minutes=reg[minute];
  	}
  	else if(type1==2&&type2==2){
    		hours=hour;
   		minutes = minute;
  	}
}
void checkAlarm(){
  	if(curHour==hours&&curMinute==minutes){
      		flag[9]=1;
      		flag[8]=0;
  	}
  	else{
    		curMinute+=10;
		if(curMinute>=60){
			curHour++;
			curMinute-=60;
			if(curHour>=24){
				curHour=0;
			}
		}
	  }
}
void setTime(){
	curHour=5;
	curMinute=0;
}
void showTime(){
	printf("Time is %d:%d\n",curHour,curMinute);
}
void connect(int device,int type1){
  	connected[0]=1;
  	if(type1==1){
  	 	 connected[reg[device]]=1;
  	}
  	else if(type2==2){
  	    	connected[device]=1;
  	}

}


void setPassword(char pass[]){
  //password= generatPassword(pass);
  strcpy(password,pass);
}
void checkPassword(int pass){
  //char check = generatPassword(pass);

	if(strcmp(password,pass)==0){
    		flag[10]=1;
  	}
  	else{
    		flag[10]=0;
  	}
}
void stepCouter(int finish,int type1){
	int i;
	stepCount=0;
  	if(type1==1){
		for(i=0;i<reg[finish];i++){
    			stepCount++;
  		}
	}
	else if(type1==2){
		for(i=0;i<finish;i++){
    			stepCount++;
  		}
	}
}

void heartBeat(){

}
void checkNotif(){
  	int a=rand()%2;
  	if(a==1){
    		numberOfNotification++;
  	}
  	if(numberOfNotification>0){
    		flag[11]=1;
  	}

}
void showNotification(){
  	printf("You have %d Notifications\n",numberOfNotification);
  	flag[11]=0;
  	numberOfNotification=0;
}


void move(int source ,int destination,int type1){
  	if(type1==2){
    		reg[destination]=source;
  	}
  	else if(type1==1){
    		reg[destination]=reg[source];
  	}

}
void add(int source,int destination,int type1){
  	if(type1==2){
    		reg[destination]=source+reg[destination];
  	}
  	else if(type1==1){
    		reg[destination]=reg[source]+reg[destination];
  	}

}
void sub(int source,int destination,int type1){
  	if(type1==2){
    		reg[destination]=source-reg[destination];
  	}
  	else if(type1==1){
    		reg[destination]=reg[source]-reg[destination];
  	}

}
void mul(int source,int destination,int type1){
  	if(type1==2){
    		reg[destination]=source*reg[destination];
  	}
  	else if(type1==1){
    		reg[destination]=reg[source]*reg[destination];
  	}

}
void div(int source,int destination,int type1){
  	if(type1==2){
    		reg[destination]=source/reg[destination];
  	}
  	else if(type1==1){
    		reg[destination]=reg[source]/reg[destination];
  	}

}
void cmp(int source,int destination,int type1,int type2){
  	if(type1==2&&type2==2){
    		if(source>destination)
      			return;
    		else if(source==destination)
      			flag[12]=1;
    		else
        		flag[13]=1;
  	}
  	else if(type1==1&&type2==2){
    		if(reg[source]>destination)
      			return;
    		else if(reg[source]==destination)
      			flag[12]=1;
    		else
        		flag[13]=1;
  	}
  	else if(type1==2&&type2==1){
  		if(source>reg[destination])
  			return;
    		else if(source==reg[destination])
      			flag[0]=1;
    		else
        		flag[1]=1;
  	}
  	else if(type1==1&&type2==1){
    		if(reg[source]>reg[destination])
      			return;
    		else if(reg[source]==reg[destination])
      			flag[0]=1;
    		else
        		flag[1]=1;
  	}

}

void ld(int source,int destination,int type1){
  	if(type1==2){
    		regs[destination]=source;
  	}
  	else{
    		regs[destination]=regs[source];
  	}

}
void st(int source,int destination,int type2){
  	if(type2==2){
    		destination=regs[source];
  	}
  	else{
  	  	regs[destination]=regs[source];
 	 }

}

void halt(){
  	exit(0);
}
