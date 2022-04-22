#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>
#include<windows.h>
#define LEN 10
#define YEAR 3
#define FILENUM 20
#define FILENAMELEN 50

int amount;
char opensource[FILENAMELEN];
int fnum=0;					


struct data{
	 int i;         
	 int QuestionNum[YEAR][LEN];
	 
}tempdata[FILENUM];

struct leadmodel{
	int i;
	char lead[FILENUM][FILENAMELEN]; 
}leaddata;

long ReadQuestion(FILE*,long,int);
void ComfirmAmount(FILE*fp);
void MakePaper(FILE*);     
long LocateOffset(FILE*,int);
int FoundSame(int);
void CreateNew(void);
void RewriteData(void);
void InitializeData(void);  
int GetData(void);
void PrintData(void);
char *s_gets(char *st,int n);

void AddNewLibrary(void);
void InitializeLeadFile(void);
void PrintLead(void);
void DeleteLibrary(void);
void RenameLibrary(void);

int main(void){
	FILE *fp,*finished,*fdata,*flead;
	long locate;
	int ch;
	int choice=-1;
	int ret=0;

  A:if((flead=fopen("leadfile","r"))==NULL){
		printf("Con't open the leadfile.");
		exit(1);
	}
	fread(&leaddata,sizeof(leadmodel),1,flead);
	fclose(flead); 
	
	printf("Welcome!\n");
	while(ret!=1){
		printf("Choose a library as a open source(press 0 to creat a new library):\n");
		PrintLead();
		if((ret=scanf("%d",&fnum))!=1) {
			printf("Error!Input again!\n\n\n");
			while(getchar()!='\n') continue;
		}
		}
		
	ret=0;
	
	if(fnum==0){
	 	AddNewLibrary();
	 	goto A;
	}
	else fnum--;
	strncpy(opensource,leaddata.lead[fnum],FILENAMELEN);

MAINB:	
    if((fp=fopen(opensource,"r"))==NULL){
		printf("Con't open the file %s.",opensource);
		system("pause");
		exit(1);
	}
	ComfirmAmount(fp);
	

	system("COLOR F");      
	printf("\n\nYou have opened the library file %s\n",opensource);
	printf("\n1,Make paper\n\n2,Calculate amount of question\n");
	printf("3,InitializeData\n4,Create new question\n\n5,Check datafile\n\n");
	printf("6,Add/rename/delete a library file\n7,InitializeLeadFile\n8,Print library list\n\n"); 
	printf("9,Change open sources\n0,Exit\n\n\n");
	scanf("%d",&ch);
	while(getchar()!='\n') continue;
	
	switch(ch)
	{
		case 1:MakePaper(fp);
				break;
		case 2:printf("There are %d questions.\n",amount);
				break;
		case 3:InitializeData();
				break;
		case 4:fclose(fp);
				CreateNew();
		        break;
		case 5:PrintData();
			break;
		case 6: 
			C:printf("\n1 to add a library\n2 to rename a library.\n\n3 to \
delete a library\nother number to quit.");
			scanf("%d",&choice);
			if(choice==1) AddNewLibrary();
				else if(choice==2) RenameLibrary();
		    		else if(choice==3) DeleteLibrary();
			else printf("Input error!\n");
			fclose(fp);
			fclose(flead);
			printf("\n\n\n\n");
			goto A;
		case 7:InitializeLeadFile();
				break;
		case 8:PrintLead();
				break;
		case 9: goto A;
		case 0:exit(0);
				
		default:printf("Wrong!");
			
	}
	fclose(fp);
	fclose(flead);
	system("pause");
	printf("\n\n\n\n");
	goto MAINB;
	return 0;
}
void MakePaper(FILE*fp){
	int i,n;
	char ans[5];
	long offset;
	extern int fnum;
	struct data *p=&tempdata[fnum];
	int ret;

	
	if((ret=GetData())==1)
		printf("Data get successfully.\n");
	else {
		printf("Data get fail.\nYou can try initializing data\n");
		return;
	}
	if(amount==0){
		printf("It is empty!\n");
		return ;
	}
	
	for(i=0;i<LEN;i++){
		do{

			printf("\nFinding rand question...\n");
			srand((unsigned)time(0));
			n=rand()%amount+1;      	 
			printf("rand question  %d found.\n",n);
			Sleep(1000);
		}while(FoundSame(n));
		printf("locating...\n");
		offset=LocateOffset(fp,n);
		printf("located.\n");
		
		printf("Reading question...");
		ReadQuestion(fp,offset,i+1);
		tempdata[fnum].QuestionNum[(tempdata[fnum].i)%YEAR][i]=n;
	}
	
	printf("Do you want to save?(y/n)");
	scanf("%5s",ans);
	if(ans[0]=='y'){
		p->i=(p->i)+1;
		printf("i=%d",tempdata[fnum].i);
		RewriteData();    
		printf("Save secessfully");
	}
	else GetData();
}

void CreateNew(void){
	char question[200];
	char option[4][20];
	char key[20];
	int op;
	FILE*fp;
	extern char opensource[FILENAMELEN];
	
	if((fp=fopen(opensource,"a+"))==NULL){
		printf("con't open the file");
		return ;
	}
	
	while(getchar()!='\n') continue;
	printf("Input question(less than 100 words):\n");
	s_gets(question,200);
	printf("Input options:(less than 10 words)\n ");
	for(op=0;op<4;op++){
		printf(" \r%c.",'A'+op);
		s_gets(option[op],20);
	}
	printf("Input the key:(less than 10 words)\nthe key is");
	s_gets(key,20);
	
	fprintf(fp,"\n$\n%s\n",question);
	for(op=0;op<4;op++) fprintf(fp,"%c.%s\n",'A'+op,option[op]);
	fprintf(fp,"*the key is %s\n*\n#",key);
	
	fclose(fp);
	
	return ;
}
long LocateOffset(FILE*fp,int n){
	int i=0;
	int offset=0;
	char ch;
	
	while(i<n){
		fseek(fp,offset,SEEK_SET);

			if(getc(fp)=='$'){
			i++;
		}
		offset++;	
	}
	return offset+2;	
}

int FoundSame(int n){
	int year=0,num=0;
	
	for(year=0;year<YEAR;year++){
		for(num=0;num<LEN;num++){
			if(tempdata[fnum].QuestionNum[year][num]==n) return 1; 
		}															
	}
	return 0;													
}

long ReadQuestion(FILE* fp,long begin,int qnum){ 
	FILE *ff,*fkey;                        
	char ch;
	int key=0;     

	
	fseek(fp,begin,SEEK_SET);			 
	if((ff=fopen("FinishedPaper.doc","a"))==NULL){
		printf("Con't open the file FinishedPaper.");
		return -1;
	}
	if((fkey=fopen("FinishedPaperKey.doc","a"))==NULL){
		printf("Con't open the file FinishedPaperKey.");
		return -1;
	}

	fprintf(ff,"%d,",qnum);
	fprintf(fkey,"%d,",qnum);

	while((ch=getc(fp))!='#') { 		
		if(ch=='*') key^=1; 			
		if(ch!='*')putc(ch,key ? fkey:ff);   
		begin+=1;
		fseek(fp,begin,SEEK_SET);		
	}
	fclose(ff);
	fclose(fkey);
	
	return begin+1;
}

void ComfirmAmount(FILE*fp){		
	unsigned long i=0;
	int num=0;
	char word;
	
	while(fseek(fp,i,SEEK_SET)==0&&(word=getc(fp))!=EOF){
		if(word=='$') num++;
		i++;
	}

	amount=num;
}

int GetData(void){
	FILE *fp;
	int ret;
	
	if((fp=fopen("datafile","r"))==NULL){
		printf("con't open the file");
		fclose(fp);
		return 0;
	}
	ret=fread(&tempdata,FILENUM*sizeof(struct data),1,fp);
	
	fclose(fp);
	return 1;

}



void RewriteData(void){
	FILE *fp;
	if((fp=fopen("datafile","w"))==NULL){
		printf("Con't open the file");
		return;
	}

	fwrite(&tempdata,FILENUM*sizeof(struct data),1,fp);

	fclose(fp);
}

void InitializeData(void){
	int year,num,leadnum;
	char ans[5];
	FILE *fp;
	int ret=1;
	int  choice;
	
	printf("1 to initialize all data\n2 to initialize datas of this source\nOther key to quit\n");
	scanf("%d",&choice);
	if(choice==1){
		fp=fopen("datafile","w");
		
		for(leadnum=0;leadnum<leaddata.i;leadnum++){
			struct data data0;
			
			data0.i=0;
				for(year=0;year<YEAR;year++)
				{
					for(num=0;num<LEN&&ret==1;num++)
						data0.QuestionNum[year][num]=0;
				}
				tempdata[leadnum]=data0;					 
			}
		ret=fwrite(&tempdata,sizeof(struct data)*FILENUM,1,fp);
		
		fclose(fp);
		
		return;
	}
	else if(choice!=2){
		return;
	}
	printf("Do you want to initialize datas of this source?It is not restorable!(y/n)");
	scanf("%5s",ans);
	while(getchar()!='\n') continue; 
	

	if(ans[0]=='y'){
		if((fp=fopen("datafile","w"))==NULL){
			printf("Con't open the file\n");
		}
		else{
			
			struct data data0;
			data0.i=0;
			for(year=0;year<YEAR;year++)
			{
				for(num=0;num<LEN&&ret==1;num++)
					data0.QuestionNum[year][num]=0;
			}
			tempdata[fnum]=data0;
			ret=fwrite(&tempdata,sizeof(struct data)*FILENUM,1,fp);
		}
		
		fclose(fp);
		if(ret==1){
		 printf("Initialize data successfully!");
		 GetData();
		}
		else{
			printf("Fail to initialize data");
		}
	}
}
void PrintData(void){
	int year=0,num=0;
	int ret;
	
	ret=GetData();
	if((ret=GetData())==1)
		printf("Data get successfully.\n");
	else {
		printf("Data get fail.\nYou can try initialize data\n");
		return;
	}
	
	printf("You used makepaper %d times,the data is:\n",tempdata[fnum].i);
	for(year=0;year<YEAR;year++){
		for(num=0;num<LEN;num++){
			printf("%d\t",tempdata[fnum].QuestionNum[year][num]);
		}
		putchar('\n');
	}
}
char *s_gets(char*st, int n){
	char *ret_val;				 
	int i=0;				
	
	ret_val=fgets(st,n,stdin);
	if(ret_val)
	{
		while((st[i]!='\n'&&st[i])!='\0')
			i++;
		if(st[i]=='\n') st[i]='\0';
		else
			while(getchar()!='\n') continue;
	}
	return ret_val;
}
void AddNewLibrary(void){		
	FILE *flead,*fp,*fdata;
	int num=leaddata.i;
	
	if(leaddata.i==FILENUM){
		printf("Storage space is full,choose a file to replace it or 0 to exit\n");
		PrintLead();
		printf("0,give up\n");
		scanf("%d",&num);
		while(getchar()!='\n') continue;
		num--;
	}
	
	
	flead=fopen("leadfile","r");
	fread(&leaddata,sizeof(leadmodel),1,flead);
	fclose(flead);
	while(getchar()!='\n') continue;
	printf("Input the question library file name(less than %d words)\
press \"enter\" only to quit:\n",FILENAMELEN-5);
	s_gets(leaddata.lead[num],FILENAMELEN-5);
	if(strlen(leaddata.lead[num])==0){
		printf("Input error!\n");
		return;
	}
	
	strcat(leaddata.lead[num],".txt");		
	printf("%s\n",leaddata.lead[num]);
	
	fp=fopen(leaddata.lead[num],"a");
	fclose(fp);
	
	(leaddata.i)=num+1;

	flead=fopen("leadfile","w");
	fwrite(&leaddata,sizeof(leadmodel),1,flead);
	fclose(flead);
	
}
void InitializeLeadFile(void){				
	int num,len;
	char ans[5];
	FILE *fp,*fdata;
	int ret=1;
	
	while(getchar()!='\n') continue;
	printf("Do you want to initialize lead datas along with past file datas?\
It is not restorable!(y/n)");
	s_gets(ans,5); 
	
	 
	if(ans[0]=='y'){
		if((fp=fopen("leadfile","w"))==NULL){
			printf("Con't open the file\n");
		}
		else{
			struct leadmodel data0;
			data0.i=0;
			for(num=0;num<FILENUM;num++)
			{
				for(len=0;len<FILENAMELEN&&ret==1;len++)
					data0.lead[num][len]=' ';
				}
			ret=fwrite(&data0,sizeof(struct leadmodel),1,fp);
			
			if(ret!=0){
				fclose(fp);
				fp=fopen("leadfile","w");
				fwrite(&leaddata,sizeof(struct leadmodel),1,fp);
				printf("Initializing lead datas fails.\n");
				return;
			}
			fdata=fopen("datafile","w");
		}
		fclose(fp);
		fclose(fdata);
	}
}
void PrintLead(void){
	int count;
	printf("There are %d library.\n",leaddata.i);
	for(count=0;count<leaddata.i;count++){
		printf("%d,%s\n",count+1,leaddata.lead[count]);
	}
}
void DeleteLibrary(void){
	int num=-1,t;
	FILE *lead;
	
	
  B:printf("Choose the library you want to delete.(0 to quit)\n");
	PrintLead();
	scanf("%d",&num);
	if(num==0) return;
	else if(num>leaddata.i||num<0){
		printf("Input wrong!\n");
		while(getchar()!='\n') continue;
		goto B;
	}
	else{
		for(t=num-1;t<(leaddata.i)-1;t++){
			strcpy(leaddata.lead[t],leaddata.lead[t+1]);
			tempdata[t]=tempdata[t+1]; 
		}
		tempdata[t]=tempdata[t+1];
		leaddata.i--;
	}
	lead=fopen("leadfile","w");
	fwrite(&leaddata,sizeof(leadmodel),1,lead);
	RewriteData();
	fclose(lead);
}
void RenameLibrary(void){
	int choice=-1;
	char newfile[FILENAMELEN],tempfile[FILENAMELEN];
	FILE* fnew,*fold,*flead;
	char ch;

ReFB:
	printf("\n\nChoose the file you want to rename.(0 to quit)\n");
	PrintLead();
	scanf("%d",&choice);
	if(choice==0) return;
	else if(choice>leaddata.i||choice<0){
		printf("Input wrong!\n");
		while(getchar()!='\n') continue;
		goto ReFB;
	}
	
	while(getchar()!='\n') continue;	
	printf("Input the new filename(less than 45 words),and press\"entr\" only to quit:\n");
	
	s_gets(newfile,FILENAMELEN);
	if(strlen(newfile)==0) return;
	strcat(newfile,".txt"); 
	
 	fnew=fopen(newfile,"w");
	fold=fopen(leaddata.lead[choice-1],"r");
	while((ch=getc(fold))!=EOF) putc(ch,fnew); 
	
	strcpy(tempfile,leaddata.lead[choice-1]);
	strcpy(leaddata.lead[choice-1],newfile);
	
	flead=fopen("leadfile","w");
	fwrite(&leaddata,sizeof(leadmodel),1,flead);
	
	fclose(fold);
	fclose(fnew);
	fclose(flead);
	
	remove(tempfile);
	return;
}
