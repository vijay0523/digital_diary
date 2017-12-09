#include <fstream.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <iomanip.h>
#include <process.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <windows.h>

#define ARRAY_SIZE(array)	sizeof(array)/sizeof(array[0])

void border(int x=1, int y=1, int l=80, int b=25, char ch='#')
/*(x,y) is the top left corner point and l and b are the length and breadth of the rectangle*/
{
	for(int p=x; p<x+l; p++)			//creates the top horizontal rule
	{
		gotoxy(p,y);
		cout<<ch;
	}
	if(b==25)
		for(int q=x; q<x+l-1; q++)			//creates the bottom horizontal rule
		{
			gotoxy(q,y+b-1);
			cout<<ch;
		}
	else
		for(int q=x; q<x+l; q++)
		{
			gotoxy(q,y+b-1);
			cout<<ch;
		}
	for(int r=y; r<y+b; r++)			//creates the left vertical rule
	{
		gotoxy(x,r);
		cout<<ch;
	}
	for(int s=y; s<y+b-1; s++)			//creates the right vertical rule
	{
		gotoxy(x+l-1,s);
		cout<<ch;
	}
}

void error_message(char mssg[],int x=27)
{
	clrscr();
	border(15,10,50,7);
	gotoxy(x,12);
	cout<<mssg;
	gotoxy(36,15);
	cout<<"Press any key to continue..";
	getch();
}

void delay(int a=1)
{
	long i;
	if(a==1)
		for(i=0; i<40000000; i++);
	else if(a==2)
		for(i=0; i<60000000; i++);
}

void create_menu(char title[], char list_opt[][25]=NULL, int no_of_elements=0)
{
	clrscr();
	border(1,1,80,25);
	gotoxy(41-strlen(title)/2,3);
	cout<<title;

	int step=6;
	for(int i=0; i<no_of_elements; ++i)
	{
		if(i%2==0)
			{
				border(11,step,25,5);
				gotoxy(13,step+2);
				cout<<list_opt[i];
			}
		if(i%2==1)
			{
				border(46,step,25,5);
				gotoxy(48,step+2);
				cout<<list_opt[i];
				step+=5;
			}
	}

	gotoxy(39,22);
	cout<<"___";
	gotoxy(34,24);
	cout<<"Press backspace to return to previous screen.";

	gotoxy(40,22);
}

void create_list(char list_elements[][25], int no_of_elements, int y_start, float no_of_columns=1)
{
	int longest_element=1;
	for(int i=0; i<no_of_elements; ++i)
		if(longest_element<strlen(list_elements[i]))
			longest_element=strlen(list_elements[i]);

	int y=y_start;
	i = 0;
	for(int h=1; h<=no_of_columns; ++h)
	{
		int y=y_start;
		for(; i<(no_of_elements/no_of_columns)*h; ++i)
		{
			gotoxy(((81/no_of_columns-longest_element)/2)/*centers the list in the column*/+(h-1)*(81/no_of_columns)/*shifts the column*/,y++);
			cout<<list_elements[i];
		}
	}
}


void create_list(char list_elements[][50], int no_of_elements, int y_start, float no_of_columns=1)
{
	int longest_element=1;
	for(int i=0; i<no_of_elements; ++i)
		if(longest_element<strlen(list_elements[i]))
			longest_element=strlen(list_elements[i]);

	int y=y_start;
	i = 0;
	for(int h=1; h<=no_of_columns; ++h)
	{
		int y=y_start;
		for(; i<(no_of_elements/no_of_columns)*h; ++i)
		{
			gotoxy(((81/no_of_columns-longest_element)/2)/*centers the list in the column*/+(h-1)*(81/no_of_columns)/*shifts the column*/,y++);
			cout<<list_elements[i];
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////

char notes_file[30], contacts_file[30];

class USER
{

	private :
		char username[30];
		char password[30];

	public :

		char* ret_user()
		{
			return username;
		}

		char* ret_pass()
		{
			return password;
		}

		int check_user();
		void login(char*);

		USER()
		{}

}	U;

class CONTACTS
{
	private:
			char name[20],addline_1[20],addline_2[20],email_id[30];
			char ph_no[10],mob_no[10],encrypted_text[30],decrypted_text[30];
			char index;
	public:                                      
			void file_edit();
			void input(char q);    //get contacts details by input
			void display();
			void menu();
			void view();
			void createnew();
			void edit(char temp[]);
			void search();
			void deletecontact(char tmp[]);
			void Sort();
			void clrdsp();
			void charfix(char temp[],char letter,int mode);
			void encrypt(char temp[]);
			void decrypt(char temp[]);
}	c,f;

//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////login starts/////////////////////////////////////////////
char* get_pass(char* keyword="check")
{
	void login(char*);

	char pass[30];
	for(int i=0;i<30;i++)
	{
		pass[i]=getch();

		if(i>=1 && pass[i]==8)
		{
			--i;
			gotoxy(40+i,14);
			cout<<' ';
			gotoxy(40+i,14);
			i--;
			continue;
		}

		if(strcmp(keyword,"create")==0)
			if(!(pass[i]>96&&pass[i]<123 || pass[i]>64&&pass[i]<91 || pass[i]>32&&pass[i]<43 || pass[i]>47&&pass[i]<58 || pass[i]==13))
			{
				error_message("Use characters A-Z a-z 0-9 ! \" # $ % & ' ( ) *",17);
				return NULL;
			}

		if(pass[i]==13)
		{
			pass[i]='\0';
			break;
		}

		cout<<'*';
	}
	return pass;
}

int check_username(char user[])		//returns loc of user if it exists, 0 if not
{
	USER U1;
	ifstream infile("dduser.dat", ios::in|ios::binary);
	while(infile.read((char*)&U1, sizeof(USER)))
		if(strcmpi(user, U1.ret_user()))
			return infile.tellg();
	return 0;
}

void USER :: login(char* keyword="user")
{
	char login_menu[][25]={"1. Login", "2. Create", "3. Delete"};
	fstream file("dduser.dat", ios::in|ios::app|ios::binary);
	
	char option;
	int bool=0, loc=0, wu=0, wp=0;		// wu,wp count # of times wrong username,password is entered
	if(strcmp(keyword,"pass")==0)
		goto wrong_password_entered;
	/////////////////////////////////////////////////////////

	start:
	border();
	create_menu("Digital Diary", login_menu, ARRAY_SIZE(login_menu));
	option=getche();
	if(option=='1' || option=='2' || option=='3')
	{	
		enter_username:
		char temp_user[30];
		clrscr();
		border(20,8,40,5);
		if(option=='2')
		{
			gotoxy(21,6);
			cout<<"Username should be less than 30 char";
		}
		gotoxy(30,10);
		cout<<"Username: ";
		gets(temp_user);

		if(option=='2')
			if(!check_username(temp_user))
			{
				strcpy(username,temp_user);
				goto enter_password;
			}
			else
			{
				error_message("Username taken.", 33);
				goto enter_username;
			}
		else
		{
			loc=check_username(temp_user);
			if(loc)
				bool=1;
		}
		if(bool==0)
		{
			error_message("Username is incorrect",29);
			wu++;
			if(wu==2)
				goto start;
			goto enter_username;
		}
		/////////////////////////////////////////////////////////

		enter_password:
		bool=0;
		border(20,12,40,5);
		if(option=='2')
		{
			gotoxy(18,18);
			cout<<"Use characters A-Z a-z 0-9 ! \" # $ % & ' ( ) *";
		}
		gotoxy(30,14);
		cout<<"Password:                    ";
		gotoxy(40,14);
		if(option=='2')
		{
			strcpy(password,get_pass("create"));
			file.write((char*)this, sizeof(USER));
			file.close();
			return;
		}
		else if(option=='3')
		{
			file.seekp(loc, ios::beg);
			while(file.read((char*)&U, sizeof(USER)))
			{
				file.seekp(-2*sizeof(USER), ios::cur);
				file.write((char*)&U, sizeof(USER));
				file.seekp(sizeof(USER), ios::cur);
			}
			strcpy(notes_file, U.ret_user());
			strcpy(contacts_file, U.ret_user());
			strcat(notes_file, "ddn.dat");
			strcat(contacts_file, "ddc.dat");
			remove(notes_file);
			remove(contacts_file);
			error_message("User Deleted.", 34);
			goto start;
		}
		else
		{
			char temp_pass[30];
			strcpy(temp_pass,get_pass());
			while(file.read((char*)&U, sizeof(USER)))
				if(strcmpi(U.ret_user(), temp_user)==0)
					if(strcmp(U.ret_pass(), temp_pass)==0)
						bool=1;
		}
		if(bool==0)
		{
			error_message("Password is incorrect",29);
			wp++;
			if(wp==2)
				goto start;
			goto wrong_password_entered;
		}
		return;

		////////////////////////////////////////////////////////
		wrong_password_entered:

		clrscr();
		border(20,8,40,5);
		gotoxy(30,10);
		cout<<"Username: "<<username;
		goto enter_password;
	}
	goto start;
}
//////////////////////////////////////login ends//////////////////////////////////////////////
/////////////////////////////////////menu starts//////////////////////////////////////////////
void menu()
{
	void world_clock();
	void converter();
	void calculator();
	void notes();
	void horoscope();
	void pacman();

	strcpy(notes_file, U.ret_user());
	strcpy(contacts_file, U.ret_user());
	strcat(notes_file, "ddn.dat");
	strcat(contacts_file, "ddc.dat");
	
	start:
	char main_menu[][25]={"1. Contacts","2. World Clock","3. Calculator","4. Converter","5. Notes","6. Horoscope"};
	create_menu("Digital Diary",main_menu,ARRAY_SIZE(main_menu));
	char option = getch();

	switch(option)
	{
		case '1':
			c.menu();
			break;
		case '2':
			world_clock();
			break;
		case '3':
			calculator();
			break;
		case '4':
			converter();
			break;
		case '5':
			notes();
			break;
		case '6':
			horoscope();
			break;
		case 'g':
			pacman();
			break;
		case 8	:
			return;
		default:
			error_message("Invalid option!",32);
	}
	goto start;
}
/////////////////////////////////////menu ends////////////////////////////////////////////////
////////////////////////////////////contacts starts///////////////////////////////////////////

void CONTACTS::Sort()
{
	int i=0;
	char contact[100][20];
	char tmp[20];
	ifstream file;
	file.open(contacts_file,ios::binary);
	while(file.read((char*)&c,sizeof(c)))
	{
		decrypt(name);
		strcpy(contact[i],decrypted_text);
		i++;
	}

	int n,j;
	n=i;

	for(i=0;i<n-1;i++)
	{
		for(j=i+1;j<n;j++)
		{
			if(strcmpi(contact[i],contact[j])>0)
			{
				strcpy(tmp,contact[i]);
				strcpy(contact[i],contact[j]);
				strcpy(contact[j],tmp);
			}
		}
	}

	cout<<endl;                      //debug info
	for(i=0;i<n;i++)
	{
		encrypt(contact[i]);
		strcpy(contact[i],encrypted_text);
	}
	file.close();
	ifstream file2;
	ofstream file3;
	file3.open("temp.cf",ios::binary);
	file2.open(contacts_file,ios::binary);
	char sorted[100];
	i=0;

	for(int k=0;k<n;k++)
	{
		file2.seekg(0);
		while(file2.read((char*)&c,sizeof(c)))
		{
			if(strcmp(contact[k],name)==0)
			{
				file3.write((char*)&c,sizeof(c));
				i++;
				break;
			}
		}
	}
	cout<<endl;                      //debug info
	for(i=0;i<n;i++)                 //debug info
		cout<<int(sorted[i])<<endl;       //debug info
	remove(contacts_file);
	rename("temp.cf",contacts_file);
	file2.close();
	file3.close();
	file.close();
	clrscr();
}
void CONTACTS :: clrdsp()
{
	gotoxy(38,8);cout<<"                                         ";
	gotoxy(38,9);cout<<"                                         ";
	gotoxy(38,10);cout<<"                                         ";
	gotoxy(38,11);cout<<"                                         ";
	gotoxy(38,12);cout<<"                                         ";
	gotoxy(38,13);cout<<"                                         ";
}
void CONTACTS :: encrypt(char temp[])
{
	char ekeyl[]="gi*dhwp(k{}?|^y$x#fv'%q!ua@. ICFEUSWRVD";
	char ekeyu[]="coenXmZ&):<>+][_`jlorz~s-=@.";
	char dkeyl[]="abcdefghijklmnopqrstuvwxyz@. 0123456789";
	char dkeyu[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ@.";

	char output[100];
	//gets(temp);
	int l=strlen(temp);
	for(int i=0;i<=l-1;i++)
	{
		con:
		for(int j=0;j<=39;j++)
		{
			//cout<<i<<dkeyl[j]<<"."<<temp[i]<<endl<<output<<endl;          //debug info
			if(temp[i]==dkeyl[j])
			{
				output[i]=ekeyl[j];
				i++;
				if(i!=l)
					goto con;
				else
					goto exit;
			}
			if(dkeyu[j]==temp[i])
			{
				//cout<<"here2";                                                //debug info
				output[i]=ekeyu[j];
				i++;
				if(i!=l)
					goto con;
				else
					goto exit;
			}
		}
	}
	exit:
	output[i]='\0';
	//cout<<endl<<output;                                           //debug info
	strcpy(encrypted_text,output);
}

void CONTACTS :: decrypt(char temp[])
{
	char ekeyl[]="gi*dhwp(k{}?|^y$x#fv'%q!ua@. ICFEUSWRVD";
	char ekeyu[]="coenXmZ&):<>+][_`jlorz~s-=@.";
	char dkeyl[]="abcdefghijklmnopqrstuvwxyz@. 0123456789";
	char dkeyu[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ@.";

	char output[100];
	//gets(temp);
	int l=strlen(temp);
	for(int i=0;i<=l-1;i++)
	{
		con:
		for(int j=0;j<=39;j++)
		{
			//cout<<i<<dkeyl[j]<<"."<<temp[i]<<endl<<output<<endl;          //debug info
			if(temp[i]==ekeyl[j])
			{
				//cout<<"here1";                                                //debug info
				output[i]=dkeyl[j];
				i++;
				if(i!=l)
					goto con;
				else
					goto exit;
			}

			if(ekeyu[j]==temp[i])
			{
				//cout<<"here2";                                                //debug info
				output[i]=dkeyu[j];
				i++;
				if(i!=l)
					goto con;
				else
					goto exit;
			}
		}
	}
	exit:
	output[i]='\0';
	//cout<<endl<<output;                                           //debug info
	strcpy(decrypted_text,output);
}

void CONTACTS :: file_edit()
{
	int n;
	char i=0;
	cout<<"Enter number of contacts: ";
	cin>>n;
	ofstream file;
	file.open(contacts_file,ios::binary||ios::out);
	while(i<n)
	{
		c.input(i);
		i=i+1;
		file.write((char*)&c,sizeof(c));
	}
	file.close();

	char a;
	again:
	cout<<"Do You Want to View Contacts:(Y/N)";
	a=getchar();
	if(a=='Y'||a=='y')
		view();
	else if(a=='N'||a=='n')
		goto exit;
	else
	{
		cout<<"Invalid input, Enter again";
		goto again;
	}
	exit:
}

void CONTACTS :: deletecontact(char tmp[])
{
	ifstream file;
	ofstream temp;
	int fail=1;
	temp.open("$$.cf",ios::binary);
	file.open(contacts_file,ios::binary);
	while(file.read((char*)&c,sizeof(c)))
	{
		decrypt(name);
		if(!(strcmpi(decrypted_text,tmp)==0))
		{
			temp.write((char*)&c,sizeof(c));
			continue;
		}
		fail=0;
	}
	temp.close();
	file.close();
	remove(contacts_file);
	rename("$$.cf",contacts_file);
	gotoxy(38,8);
	if(fail==0)
		cout<<"Contact Deleted";
	if(fail!=0)
		cout<<"ERROR 2";
	gotoxy(38,9);
	cout<<"Hit Enter to continue.....";
	char c=getch();
	if(c==13)
	{
		clrscr();
		menu();
	}
}

void CONTACTS :: display()
{
	gotoxy(51,8);cout<<"                         ";
	gotoxy(36,9);cout<<"                         ";
	gotoxy(41,10);cout<<"                         ";
	gotoxy(41,11);cout<<"                         ";
	gotoxy(41,12);cout<<"                         ";
	gotoxy(41,13);cout<<"                         ";     //cleaning screen

	gotoxy(38,8);
	cout<<"Contact Name:";
	decrypt(name);
	puts(decrypted_text);

	gotoxy(38,9);
	cout<<"Phone no:";
	cout<<ph_no<<endl;

	gotoxy(38,10);
	cout<<"Mobile no:";
	puts(mob_no);

	gotoxy(38,11);
	cout<<"Email id:";
	decrypt(email_id);
	puts(decrypted_text);

	gotoxy(38,12);
	cout<<"Address (line 1):";
	decrypt(addline_1);
	puts(decrypted_text);

	gotoxy(38,13);
	cout<<"Address (line 2):";
	decrypt(addline_2);
	puts(decrypted_text);
}
void CONTACTS :: charfix(char temp[],char letter,int mode)
{
	int l=strlen(temp);
	//temp[l]='o';                //just increasing the array size by 1
	memmove(temp+1,temp,l);     //move all characters +1 unit to the right
	temp[0]=letter;             //now replace 1st letter
	temp[l+1]='\0';
	if(mode==1)
		strcpy(name,temp);
	else if (mode==2)
		strcpy(ph_no,temp);
	else if (mode==3)
		strcpy(mob_no,temp);
	else if (mode==4)
		strcpy(email_id,temp);
	else if (mode==5)
		strcpy(addline_1,temp);
	else if (mode==6)
		strcpy(addline_2,temp);
}
void CONTACTS :: input(char q)    //get contacts details by input
{
	index=q;
	char a;
	gotoxy(38,8);
	cout<<"Contact Name:";
	a=getche();
	if(a==13)
		goto next0;
	else
		gets(name);
	charfix(name,a,1);
	encrypt(name);
	strcpy(name,encrypted_text);

	next0:

	gotoxy(38,9);
	cout<<"Phone no:";
	a=getche();
	if(a==13)
		goto next1;
	else
		gets(ph_no);
	charfix(ph_no,a,2);

	next1:

	gotoxy(38,10);
	cout<<"Mobile no:";
	a=getche();
	if(a==13)
		goto next2;
	else
		gets(mob_no);
	charfix(mob_no,a,3);

	next2:

	gotoxy(38,11);
	cout<<"Email id:";
	a=getche();
	if(a==13)
		goto next3;
	else
		gets(email_id);
	charfix(email_id,a,4);
	encrypt(email_id);
	strcpy(email_id,encrypted_text);

	next3:

	gotoxy(38,12);
	cout<<"Address (line 1):";
	a=getche();
	if(a==13)
		goto next4;
	else
		gets(addline_1);
	charfix(addline_1,a,5);
	encrypt(addline_1);
	strcpy(addline_1,encrypted_text);

	next4:

	gotoxy(38,13);
	cout<<"Address (line 2):";
	a=getche();
	if(a==13)
		goto next5;
	else
		gets(addline_2);
	charfix(addline_2,a,6);
	encrypt(addline_2);
	strcpy(addline_2,encrypted_text);

	next5:

}

void CONTACTS :: edit(char tmpp[])
{
	fstream fio;
	ofstream fout;
	fout.open(contacts_file,ios::out||ios::binary);
	int recc=0;
	fio.open(contacts_file,ios::out||ios::in||ios::binary);
	char name1[20];
	strcpy(name1,tmpp);

	while(fio.read((char*)&c,sizeof(c)))   //testing if we can still read
	{
		if(strcmp(name,name1)==0)              //check name
		{
			input(recc+1);
			fout.seekp(recc*sizeof(c),ios::beg);
			fout.write((char*)&c,sizeof(c));
			fout.close();
			break;
		}
		gotoxy(1,1);
		recc++;
	}
	fio.close();
	c.menu();
}

void CONTACTS:: view()
{
	ifstream file;
	file.open(contacts_file,ios::binary);
	file.seekg(0,ios::beg);
	while(file.read((char*)&c,sizeof(c)))
		c.display();cout<<endl;
	file.close();
}


void CONTACTS::createnew()
{
	ifstream file;
	ofstream temp;
	temp.open("$$.cf",ios::binary);
	file.open(contacts_file,ios::binary);
	int recc=0;
	while(file.read((char*)&c,sizeof(c)))
	{
		temp.write((char*)&c,sizeof(c));
		recc++;
	}
	input(recc+1);
	temp.write((char*)&c,sizeof(c));
	temp.close();
	file.close();
	remove(contacts_file);
	rename("$$.cf",contacts_file);
	clrscr();
	ph_no[0]='\0';
	mob_no[0]='\0';
	Sort();
	menu();
}

void CONTACTS :: search()
{
	char tmp[20];
	cout<<"Enter Contact's Name:";
	gets(tmp);
	ifstream file;
	file.open(contacts_file,ios::binary);
	while(file.read((char*)&c,sizeof(c)))
	{
		decrypt(name);
		if(strcmpi(decrypted_text,tmp)==0)
			display();
	}
	file.close();
}

void CONTACTS :: menu()
{
	clrscr();
	gotoxy(0,24);
	cout<<"Use W & S keys to scroll through contacts";
	cout<<endl<<"Hit Enter to select contact. Press BackSpace to exit";
	gotoxy(38,17);
	cout<<"              ";
	gotoxy(38,18);
	cout<<"              ";
	gotoxy(55,17);
	cout<<"              ";
	gotoxy(55,18);
	cout<<"              ";
	gotoxy(30,1);
	cout<<endl<<endl<<endl;

	ifstream file;
	file.open(contacts_file,ios::binary);
	cout<<endl<<endl;
	int n=0,scroll=0;
	cout<<"Contacts List:"<<endl;
	cout<<"+Add New+"<<endl;
	file.seekg(0);
	while(file.read((char*)&c,sizeof(c)))
	{
		n++;
		decrypt(name);
		cout<<decrypted_text<<endl;
		if(n==14)
			break;
	}

	file.close();
	ifstream file1;
	file1.open(contacts_file,ios::binary);
	int y=8,x=20,recc=0;
	gotoxy(x,y);
	cout<<"<--";
	file1.read((char*)&c,sizeof(c));
	c.display();
	n--;

	loop:

	char a=getch();
	if(a!=8)
	{	//return;
		if(a=='w'&&y==8)
		{
			clrdsp();
			gotoxy(x,y);
			cout<<"   ";
			y--;
			gotoxy(x,y);
			cout<<"<--";
		}

	if(a=='s'&&y<(n+9))
	{
		clrdsp();
		gotoxy(x,y);
		cout<<"   ";y++;
		gotoxy(x,y);
		cout<<"<--";
		if(y>8)
			{file1.read((char*)&c,sizeof(c));recc++;}
		c.display();
	}

	if(a=='w'&&y>(n-5)&&y!=7)
	{
		gotoxy(x,y);
		cout<<"   ";y--;
		gotoxy(x,y);
		cout<<"<--";
		file1.seekg(((recc*(sizeof(c)))-sizeof(c)),ios::beg);
		file1.read((char*)&c,sizeof(c));
		c.display();
		recc--;
	}
	if(a=='s'&&y==22)
	{
		clrscr();
		gotoxy(0,6);
		cout<<"Contacts List:"<<endl;
		cout<<"+Add New+"<<endl;
		file.seekg(0);
		while(file.read((char*)&c,sizeof(c)))
		{
		cout<<"rea.";
			n++;
			decrypt(name);
			cout<<decrypted_text<<endl;
		}
		file1.read((char*)&c,sizeof(c));
		recc++;
		c.display();
	}
	if(a==13&&y!=7)
		goto enterkey;
	if(a==13&&y==7)
		{
		createnew();
		}
	goto loop;

	enterkey:

	gotoxy(0,24);
	cout<<"Use A & S keys to choose option                                              ";
	cout<<endl<<"Hit Enter to select option. Press B to go back. Press BackSpace to exit";
	gotoxy(10,y);
	cout<<"   ";
	int y1=18,x1=38;
	gotoxy(38,y1-1);
	cout<<"Edit Contact";
	gotoxy(55,y1-1);
	cout<<"Delete Contact";
	gotoxy(x1,y1);
	cout<<"************";

	loop2:

	char b=getch();
	if(b=='d')
	{
		x1=55;
		gotoxy(38,y1);
		cout<<"               ";
		gotoxy(55,y1);
		cout<<"               ";
		gotoxy(x1,y1);
		cout<<"***************";
	}

	if(b=='a')
	{
		x1=38;
		gotoxy(38,y1);
		cout<<"               ";
		gotoxy(55,y1);
		cout<<"               ";
		gotoxy(x1,y1);
		cout<<"************";
	}
	if(b==13&&x1==38)
	{
		//clrdsp();
		file1.close();
		gotoxy(38,8);
		edit(name);
	}
	if(b==13&&x1==55)
	{
		clrdsp();
		file1.close();
		decrypt(name);
		char name2[20];
		strcpy(name2,decrypted_text);
		deletecontact(name2);
	}
	if(b=='b'||b=='B')
	{
		clrscr();
		menu();
	}
	if(b==8)
		return;

	goto loop2;

	}
}

///////////////////////////////////contacts ends//////////////////////////////////////////////
/////////////////////////////////////converter starts/////////////////////////////////////////

double convert(char enter_unit[], double enter_amt, char valid_units[][25], int no_of_units, double mltip_values[])
{
	for(int i=0; i<no_of_units; ++i)   //If the strcmp is used here it terminates the loop
		if(strcmpi(enter_unit,valid_units[i])==0)
		{
			double ret_amt=enter_amt*mltip_values[i];
			return ret_amt;

		}
	return 0;   //Prevents crashes in case of invalid units
}

void converter_ui(char header[],char unit[]="Unit")
{
	clrscr();
	border(1,1,81,24);
	gotoxy(34,4);
	cout<<header;

	border(10,6,25,5);
	gotoxy(12,7);
	cout<<unit<<": ___";
	gotoxy(12,9);
	cout<<"Amount: ______________";

	gotoxy(39,8);
	cout<<"TO";

	border(45,6,25,5);
	gotoxy(47,7);
	cout<<unit<<": ___";
	gotoxy(47,9);
	cout<<"Amount: ______________";
}

int check_units(char in_unit[], char units_list[][25], int no_of_elements)
{
	for(int i=0; i<no_of_elements; ++i)
		if(strcmpi(in_unit,units_list[i])==0)
			return 0;
	return 1;
}

void converter()
{
	void currency();
	void unit();
	void calculator();

	start:
	char converter_menu[][25]={"1. Currency","2. Unit"};
	create_menu("Converter",converter_menu,ARRAY_SIZE(converter_menu));
	char option = getch();
	
	switch(option)
	{
		case '1':
			currency();
			break;
		case '2':
			unit();
			break;
		case 8	:
			return;
		default :
			error_message("Invalid option!");
	}
	goto start;
}

/////////////////////////////////////////////////
void currency()
{
	start:
	char currencies[][25]={"American Dollar	USD","Bahraini Dinar		BHD","Euro			EUR","Indian Rupee	INR","Japanese Yen	YEN","Saudi Riyal	SAR"};
	char currency_units[][25]={"BHD","INR","SAR","EUR","YEN","USD"};
	double to_usd[]={2.65,0.016,0.27,1.14,0.0088,1};
	double from_usd[]={0.375,64.46,3.75,0.88,112.98,1};
		
	converter_ui("Currency Converter","Currency");

	create_list(currencies,ARRAY_SIZE(currencies),13,2);
	
	gotoxy(22,7);
	char in_cur[4];
	gets(in_cur);
	if(check_units(in_cur, currency_units, ARRAY_SIZE(currency_units)))	//checks if entered currency is valid
	{
		error_message("Invalid currency!",31);
		currency();
	}
	gotoxy(20,9);
	double in_amt;
	cin>>in_amt;
		
	double temp_amt=convert(in_cur,in_amt,currency_units,ARRAY_SIZE(currency_units),to_usd);
	
	gotoxy(57,7);
	char out_cur[4];
	gets(out_cur);
	if(check_units(out_cur, currency_units, ARRAY_SIZE(currency_units)))	//checks if entered currency is valid
	{
		error_message("Invalid currency!",31);
		currency();
	}
	
	double out_amt=convert(out_cur,temp_amt,currency_units,ARRAY_SIZE(currency_units),from_usd);
	
	gotoxy(55,9);
	cout<<out_amt;

	gotoxy(50,23);
	cout<<"Press Backspace to go back..";
	gotoxy(10,23);
	cout<<"Press Enter to use again..";
	
	next:
	char temp=getch();
	if(temp==13)
		goto start;
	else if(temp==8)
		return;
	else 
		goto next;
}
/////////////////////////////////////////////////
void unit()
{
	void mass();
	void length();
	void area();
	void volume();

	start:
	char unit_menu[][25]={"1. Mass","2. Length","3. Area","4. Volume"};
	create_menu("Unit Converter",unit_menu,ARRAY_SIZE(unit_menu));
	char option = getch();

	switch(option)
	{
		case '1':
			mass();
			break;
		case '2':
			length();
			break;
		case '3':
			area();
			break;
		case '4':
			volume();
			break;
		case 8	:
			return;
		default	:
			error_message("Invalid option!",32);
	}
	goto start;
}

void mass()
{
	start:
	char masses[][25]={"Tonne		t","Kilogram	kg","Gram		g","Milligram	mg","Microgram	ug","Imperial Ton		imt","US Ton		ust","Stone		st","Pound		lb","Ounce		oz"};
	char mass_units[][25]={"t","kg","g","mg","ug","imt","ust","st","lb","oz"};
	double to_gram[]={1000000.0,1000.0,1.0,0.001,0.000001,1016000.0,907185.0,6350.29,453.592,28.3495};
	double from_gram[]={0.000001,0.001,1.0,1000.0,1000000.0,0.00000098,0.0000011,0.0001575,0.0022046,0.035274};

	converter_ui("Mass Converter");

	create_list(masses,ARRAY_SIZE(masses),15,2);

	gotoxy(18,7);
	char in_unit[4];
	gets(in_unit);
	if(check_units(in_unit, mass_units, ARRAY_SIZE(mass_units)))	//checks if entered unit is valid
	{
		error_message("Invalid Mass Unit!");
		mass();
	}
	gotoxy(20,9);
	double in_amt;
	cin>>in_amt;
	
	double temp_amt=convert(in_unit,in_amt,mass_units,ARRAY_SIZE(mass_units),to_gram);

	gotoxy(53,7);
	char out_unit[4];
	gets(out_unit);
	if(check_units(out_unit, mass_units, ARRAY_SIZE(mass_units)))
	{
		error_message("Invalid Mass Unit!");
		mass();
	}

	double out_amt=convert(in_unit,in_amt,mass_units,ARRAY_SIZE(mass_units),from_gram);
	
	gotoxy(55,9);
	cout<<out_amt;

	gotoxy(50,23);
	cout<<"Press Backspace to go back..";
	gotoxy(10,23);
	cout<<"Press Enter to use again..";
	
	next:
	char temp=getch();
	if(temp==13)
		goto start;
	else if(temp==8)
		return;
	else
		goto next;
}

void length()
{
	start:
	char lengths[][25]={"Kilometre	km","Metre		m","Centimetre	cm","Millimetre	mm","Nanometre	nm","Mile		mi","Yard		yd","Foot		ft","Inch		in","Nautical mile	nmi"};
	char length_units[][25]={"km","m","cm","mm","nm","mi","yd","ft","in","nmi"};
	double to_metre[]={1000.0,1.0,0.01,0.001,0.000000001,1609.34,0.9144,0.3048,0.0254,1852.0};
	double from_metre[]={0.001,1.0,100.0,1000.0,1000000000.0,0.000621371,1.09361,3.28084,39.3701,0.000539957};

	converter_ui("Length Converter");

	create_list(lengths,ARRAY_SIZE(lengths),15,2);

	gotoxy(18,7);
	char in_unit[4];
	gets(in_unit);
	if(check_units(in_unit, length_units, ARRAY_SIZE(length_units)))
	{
		error_message("Invalid length unit!");
		length();
	}
	gotoxy(20,9);
	double in_amt;
	cin>>in_amt;

	double temp_amt=convert(in_unit,in_amt,length_units,ARRAY_SIZE(length_units),to_metre);
	
	gotoxy(53,7);
	char out_unit[4];
	gets(out_unit);
	if(check_units(out_unit, length_units, ARRAY_SIZE(length_units)))
	{
		error_message("Invalid length unit!");
		length();
	}

	double out_amt=convert(out_unit,temp_amt,length_units,ARRAY_SIZE(length_units),from_metre);
	
	gotoxy(55,9);
	cout<<out_amt;

	gotoxy(50,23);
	cout<<"Press Backspace to go back..";
	gotoxy(10,23);
	cout<<"Press Enter to use again..";
	
	next:
	char temp=getch();
	if(temp==13)
		goto start;
	else if(temp==8)
		return;
	else
		goto next;
}

void area()
{
	start:
	char areas[][25]={"Square kilometre	sq km","Square metre	sq m","Square mile	sq mi","Square yard	sq yd","Square foot	sq ft","Square inch	sq in","Hectare		ha","Acre		ac"};
	char area_units[][25]={"sq km","sq m","sq mi","sq yd","sq ft","sq in","ha","ac"};
	double to_metre_sq[]={1000000.0,1.0,2590000.0,0.836127,0.092903,0.00064516,10000.0,4046.86};
	double from_metre_sq[]={0.000001,1.0,0.0000003861,1.19599,10.7639,1550,0.0001,0.000247105};

	converter_ui("Area Converter");

	create_list(areas,ARRAY_SIZE(areas),15,2);
	
	gotoxy(18,7);
	char in_unit[6];
	gets(in_unit);
	if(check_units(in_unit, area_units, ARRAY_SIZE(area_units)))
	{
		error_message("Invalid area unit!");
		area();
	}
	gotoxy(20,9);
	double in_amt;
	cin>>in_amt;
	
	double temp_amt=convert(in_unit,in_amt,area_units,ARRAY_SIZE(area_units),to_metre_sq);
		
	gotoxy(53,7);
	char out_unit[4];
	gets(out_unit);
	if(check_units(out_unit, area_units, ARRAY_SIZE(area_units)))
	{
		error_message("Invalid area unit!");
		area();
	}

	double out_amt=convert(out_unit,temp_amt,area_units,ARRAY_SIZE(area_units),from_metre_sq);
	
	gotoxy(55,9);
	cout<<out_amt;

	gotoxy(50,23);
	cout<<"Press Backspace to go back..";
	gotoxy(10,23);
	cout<<"Press Enter to use again..";
	
	next:
	char temp=getch();
	if(temp==13)
		goto start;
	else if(temp==8)
		return;
	else
		goto next;
}

void volume()
{
	start:
	char volumes[][50]={"US liquid gallon	US gal","US quart		US qt","US pint		US pt","US fluid ounce	US fl oz","US cup		US cp","Cubic metre		m3","Litre		L","Millilitre		ml","Imperial gallon	im gal","Imperial quart	im qt","Imperial pint	im pt","Imperial fluid ounce    im fl oz","Imperial cup		im cp","Cubic foot		ft3","Cubic inch		in3"};
	char volume_units[][25]={"US gal","US qt","US pt","US fl oz","US cp","m3","L","ml","im gal","im qt","im pt","im fl oz","im cp","ft3","in3"};
	double to_litre[]={3.785411784,0.946352946,0.473176473,0.0295735295625,0.2365882365,1000.0,1.0,0.001,4.54609,1.1365225,0.56826125,0.0284130625,0.284131,28.3168,0.0163871};
	double from_litre[]={0.264172,1.05669,2.11338,33.81405,4.16667,0.001,1.0,1000.0,0.219969,0.879877,1.75975,35.1951,3.51951,0.0353147,61.0237};

	converter_ui("Volume Converter");
	
	create_list(volumes,ARRAY_SIZE(volumes),15,2);
	
	gotoxy(18,7);
	char in_unit[4];
	gets(in_unit);
	if(check_units(in_unit, volume_units, ARRAY_SIZE(volume_units)))
	{
		error_message("Invalid Volume Unit!");
		volume();
	}
	gotoxy(20,9);
	double in_amt;
	cin>>in_amt;
	
	double temp_amt=convert(in_unit,in_amt,volume_units,ARRAY_SIZE(volume_units),to_litre);

	gotoxy(53,7);
	char out_unit[4];
	gets(out_unit);
	if(check_units(out_unit, volume_units, ARRAY_SIZE(volume_units)))
	{
		error_message("Invalid Volume Unit!");
		volume();
	}

	double out_amt=convert(in_unit,in_amt,volume_units,ARRAY_SIZE(volume_units),from_litre);
	
	gotoxy(55,9);
	cout<<out_amt;

	gotoxy(50,23);
	cout<<"Press Backspace to go back..";
	gotoxy(10,23);
	cout<<"Press Enter to use again..";
	next:
	
	char temp=getch();
	if(temp==13)
		goto start;
	else if(temp==8)
		return;
	else
		goto next;
}
/////////////////////////////////////////////converter ends///////////////////////////////////////////////////
////////////////////////////////////////////////calculator starts//////////////////////////////////////////////
void calculator();
class OP_STACK
{
	private:
		char stack[32];
		int TOP;

	public:

		void push (char data)
		{
			if(TOP == 63)
			{
				error_message("OP_STACK Overflow..");
				return;
			}
			TOP++;
			stack[TOP]=data;
		}

		void pop ()
		{
			if(TOP == -1)
			{
				error_message("OP_STACK Underflow..");
				return;
			}
			TOP--;
		}

		void clear()
		{	TOP = -1;	}

		char top()
		{	return stack[TOP];	}

		OP_STACK()
		{	TOP = -1;	}
}	O;

int is_operator (char p)	//return 1 if unary operator, 2 if binary operator, 0 if not 
{
	if (p=='s' || p=='c' || p=='t')
		return 1;
	else if (p=='+' || p=='-' || p=='*' || p=='/' || p=='%' || p=='^')
		return 2;
	else
		return 0;
}


int char_to_num (char p[])
{
	int num=0;
	for(; *p !='\0'; ++p)
	{
		num = 10*num+ (*p-48);
	}
	return num;
}

int precedence(char operator_1, char operator_2)		//returns 1 if operator_1 has higher precedence, 0 if lower precedence
{
	if (operator_1=='s' || operator_1=='c' || operator_1=='t')
	 	return 1;
	else if (operator_2=='s' || operator_2=='c' || operator_2=='t')
		return 0;
	else if ( operator_1 == '^' )
		return 1;
	else if ( operator_2 == '^' )
		return 0;
	else if ( operator_1 == '*' || operator_1 == '/' || operator_1 == '%' )
		return 1;
	else if ( operator_2 == '*' || operator_2 == '/' || operator_2 == '%' )
		return 0;
}

void to_postfix(char infix[], char postfix[])
{
	O.clear();
	O.push('(');

	int i=0;
	for(i=0; *infix != '\0'; ++i, ++infix)
	{
		
		if(*infix == ' ')
		{	}
		else if(isdigit(*infix))
		{
			*postfix=*infix;
			postfix++;
			while(isdigit(*++infix))
			{
				*postfix=*infix;
				postfix++;
			}
			*postfix=' ';
			postfix++;
			infix--;
		}
		else if(*infix == '(')
			O.push('(');

		else if(is_operator(*infix))
		{
			while(is_operator(O.top()) && precedence(O.top(), *infix))
			{
				*postfix = O.top();
				O.pop();
				postfix++;
			}
			O.push(*infix);
		}

		else if(*infix == ')')
		{
			while(O.top() != '(')
			{
				*postfix = O.top();
				O.pop();
				postfix++;
			}
			O.pop();	//discarding left parenthesis
		}
		
		else
		{
			error_message("Invalid input..");
			return;
		}
	}

	while(O.top() != '(')
	{
		*postfix=O.top();
		O.pop();
		postfix++;
	}
	O.pop();	//discard left parenthesis
	*postfix='\0';
}


class NUM_STACK
{
	private:
		struct STACK
		{
			int number;
			STACK *next;
		};
		STACK *TOP ;
		
	public:

		NUM_STACK()
		{
			TOP=NULL;
		}
		void push(int data)
		{
			STACK *P;
			P = new STACK;
			P->number=data;
			P->next = TOP;
			TOP = P;
		}
		
		void pop()
		{
			if(TOP == NULL)
			{
				error_message("Stack Underflow..");
				return;
			}
			STACK *P;
			P = TOP;
			TOP = TOP->next;
			delete P;
		}
	
		int top()
		{
			return TOP->number;
		}
}	N_S;


int calculate ()
{
	start:
	char postfix[256]=" ", infix[256]=" ";
	gotoxy(5,10);
	cout<<">>  ";
	gets(infix);
	to_postfix(infix, postfix);

	for(int i=0; postfix[i] != '\0'; ++i)
	{
		if(postfix[i] == ' ')
		{}
		
		else if(isdigit(postfix[i]))
		{
			int j=0;
			char number[11];
			number[j++]=postfix[i++];
			while(isdigit(postfix[i]) && postfix[i] !=' ')
				number[j++] = postfix[i++];
			
			number[j]='\0';
			N_S.push(char_to_num(number));
			i--;
		}
		
		else if(is_operator(postfix[i]))
		{
			char op =postfix[i];
			int r_operand=N_S.top(), l_operand;
			N_S.pop();
			if (op==2)
			{
				l_operand=N_S.top();
				N_S.pop();
			}

			int ans=0;
			switch(op)
			{
				case '+' :
					ans = l_operand+r_operand;
					N_S.push(ans);
					break;
				case '-' :
					ans = l_operand-r_operand;
					N_S.push(ans);
					break;
				case '*' :
					ans = l_operand*r_operand;
					N_S.push(ans);
					break;
				case '/' :
					ans = l_operand/r_operand;
					N_S.push(ans);
					break;
				case '%' :
					ans = l_operand%r_operand;
					N_S.push(ans);
					break;
				case '^' :
					ans = pow(l_operand, r_operand);
					N_S.push(ans);
					break;
				case 's' :
					ans = sin(r_operand);
					N_S.push(ans);
					break;
				case 'c' :
					ans = cos(r_operand);
					N_S.push(ans);
					break;
				case 't' :
					ans = tan(r_operand);
					N_S.push(ans);
					break;
				default	 :
					error_message("Invalid Operation", 32);
					goto start;
			}
		}
	}
	int result= N_S.top();
	N_S.pop();
	return result;
}

void calculator()
{
	start:
	clrscr();
	border();

	int result = calculate();
	gotoxy(60,11);
	cout<<">> "<<result;
	
	gotoxy(50,24);
	cout<<"Press Backspace to go back..";
	gotoxy(10,24);
	cout<<"Press Enter to use again..";
	
	next:
	char temp=getch();
	if(temp==13)
		goto start;
	else if(temp==8)
		return;
	else
		goto next;
}
///////////////////////////////////////calculator ends///////////////////////////////////////////
//////////////////////////////////////world clock starts////////////////////////////////////////
void world_clock()
{
	void display_time(char city[],int hr,int mn);

	start:
	char cities[][25]={"1. Los Angeles","2. New York","3. Buenos Aires","4. London","5. Paris","6. Riyadh","7. Delhi","8. Beijing","9. Sydney"};

	create_menu("World Clock");
	create_list(cities,ARRAY_SIZE(cities),14,2);
	gotoxy(40,22);
	char option = getch();

	switch(option)
	{
		case '1':
			display_time("Los Angeles",-11,0);
			break;
		case '2':
			display_time("New York",-8,0);
			break;
		case '3':
			display_time("Buenos Aires",-6,0);
			break;
		case '4':
			display_time("London",-3,0);
			break;
		case '5':
			display_time("Paris",-2,0);
			break;
		case '6':
			display_time("Riyadh",0,0);
			break;
		case '7':
			display_time("Delhi",2,30);
			break;
		case '8':
			display_time("Beijing",5,0);
			break;
		case '9':
			display_time("Sydney",7,0);
			break;
		case 8	:
			return;
		default	:
			error_message("Invalid option!",32);
	}
	goto start;
}

void display_time(char city[],int hr,int mn)
{
	get_time:

	time_t rawtime=time(0);		//gets the unix timestamp, ie. number of seconds since 1 Jan 1970, stores it in rawtime
	tm *lt;
	lt=localtime(&rawtime);		//converts rawtime to local time, i.e. Bahrain time and stores in lt

	border(25,7,34,5,'.');
	gotoxy(30,9);
	cout<<city<<" --- "<<(hr+(lt->tm_hour)+(mn+(lt->tm_min))/60)%24<<":"<<(mn+(lt->tm_min))%60<<":"<<(lt->tm_sec)<<' ';
	//cout<<city<<"---"<<lt->tm_hour<<':'<<lt->tm_min<<':'<<lt->tm_sec;
	
	gotoxy(3,24);
	cout<<"Press r to refresh";
	next:
	char temp=getch();
	if(temp=='r'||temp=='R')
		goto get_time;
	else if(temp==8)
		return;
	else
		goto next;
}
/////////////////////////////////////world clock ends///////////////////////////////////////////
//////////////////////////////////////notes starts//////////////////////////////////////////////

class NOTE
{
	public :
		char title[30],body[30];
		int serial_num;
}	N;
int note_number=1;

void notes()
{
	void view_notes();
	void add_notes();
	void delete_notes();
	
	ifstream file(notes_file, ios::in|ios::binary);
	file.seekg(-1*sizeof(NOTE), ios::end);
	file.read((char*)&N, sizeof(NOTE));
	note_number=N.serial_num;

	start:
	char notes_menu[][25]={"1. View Notes","2. Add Note", "3.Delete Notes"};
	create_menu("Notes", notes_menu, ARRAY_SIZE(notes_menu));
	char option = getch();

	switch(option)
	{
		case '1':
			view_notes();
			break;
		case '2':
			add_notes();
			break;
		case '3':
			delete_notes();
			break;
		case 8  :
			return;
		default :
			error_message("Invalid Option!",32);
	}
	goto start;
}

void view_notes()
{
	start:
	char notes_list[][25]={"1.Placeholder"," "," "," "," "," "," "," "," "," "};

	ifstream infile;
	infile.open(notes_file,ios::in|ios::binary);
	if(!infile)
	{
		error_message("Notes file doesn\'t exist");
		return;
	}

	infile.seekg(0,ios::beg);
	for(int i=0; infile.read((char*)&N,sizeof(NOTE)); ++i)
	{
		strcpy(notes_list[i],N.title);
	}

	create_menu("View Notes");
	create_list(notes_list,ARRAY_SIZE(notes_list),6);
	
	get_option:
	gotoxy(40,22);
	char option= getch();
	if(option==8)
		return;
	else if(option >='0' && option <=note_number+48)
	{
		infile.seekg((option-1)*sizeof(NOTE),ios::beg);
		infile.read((char*)&N,sizeof(NOTE));
	}
	else
		goto get_option;

	clrscr();
	gotoxy(30,5);
	cout<<N.title;
	gotoxy(3,7);
	cout<<N.body;
	
	next:
	char temp=getch();
	if(temp==8)
		return;
	else
		goto next;
}

void add_notes()
{
	start:
	clrscr();
	gotoxy(30,5);
	cout<<"Title";				//placeholder
	N.serial_num=note_number;
	N.title[0]=(note_number++)+48;
	
	strcat(N.title, ". ");
	gotoxy(30,5);
	gets(N.title);
	
	gotoxy(3,7);
	cout<<"Body";				//placeholder
	gotoxy(60,24);
	cout<<"Ctrl+S to save..";
	gotoxy(3,7);
	
	for(int i=3; ; ++i)
	{
		N.body[i]=getche();
		if(N.body[i]==19)
		{
			N.body[i] = '\0';
			break;
		}
		else if(N.body[i]==13)
		{
			cout<<'\n';
			N.body[i] = '\n';
		}
	}

	ofstream outfile;
	outfile.open(notes_file ,ios::app|ios::binary);
	outfile.write((char*)&N,sizeof(NOTE));
	outfile.close();
}

void delete_notes()
{
	start:
	char notes_list[][25]={"1.Placeholder","","","","","","","","",""};

	fstream file;
	file.open(notes_file,ios::in|ios::app|ios::binary);
	if(!file)
	{
		error_message("Notes file doesn\'t exist");
		return;
	}

	file.seekg(0,ios::beg);
	for(int i=0; file.read((char*)&N,sizeof(NOTE)); ++i)
	{
		strcpy(notes_list[i],N.title);
	}

	create_menu("Delete Notes");
	create_list(notes_list,ARRAY_SIZE(notes_list),6);
	
	get_option:
	gotoxy(40,22);
	char option= getch();
	if(option==8)
		return;
	else if(option>='0' && option<= note_number+48)
	{
		gotoxy(34,21);
		cout<<"Are you sure?";
		if(getche()=='y')
		{
			file.seekp(option*sizeof(NOTE), ios::beg);
			while(file.read((char*)&N, sizeof(NOTE)))
			{
				file.seekp(-2*sizeof(NOTE), ios::cur);
				file.write((char*)&N, sizeof(NOTE));
				file.seekp(sizeof(NOTE), ios::cur);
			}
			error_message("NOTE Deleted.", 34);
			goto start;
		}

	}
	else
		goto get_option;
}
////////////////////////////////////////notes ends//////////////////////////////////////////////
//////////////////////////////////////horoscope starts//////////////////////////////////////////

class Zodiac
{
	public :
			char s_no;
			char para[256];
}Z;

void horoscope()
{
	start:
	char zodiac_signs[][25]={"1. Aries", "2. Taurus", "3. Gemini", "4. Cancer", "5. Leo", "6. Virgo", "7. Libra", "8. Scorpio", "9. Sagittarius", "A. Capricorn", "B. Aquarius", "C. Pisces"};

	border();
	create_menu("Horoscope");
	create_list(zodiac_signs,ARRAY_SIZE(zodiac_signs),10,2);

	next_1:
	gotoxy(40,22);
	char option = getch();
	if(option==8)
		return;
	else if(option<49 || option>57&&option<97 || option>99)
		goto next_1;

	int bool=0;
	ifstream infile;
	infile.open("ddhoroscope.dat",ios::in|ios::binary);
	while(infile.read((char*)&Z, sizeof(Z)))
		if(Z.s_no==option)
		{
			clrscr();
			gotoxy(4,10);
			cout<<Z.para;
			bool=1;
		}
	if(bool==0)
	{
		error_message("Invalid Option!");
		goto start;
	}
	infile.close();

	gotoxy(34,24);
	cout<<"Press backspace to return to previous screen.";
	next_2:
	if(getch()==8)
		return;
	else
		goto next_2;
}

//////////////////////////////////////horoscope ends////////////////////////////////////////////
/////////////////////////////////////snake game starts//////////////////////////////////////////
int left=0,right=0,up=0,down=0,escape=0;

void check()
{
	if(GetAsyncKeyState(VK_LEFT))
	{
		left=1,right=0,up=0,down=0;
	}
	else if(GetAsyncKeyState(VK_RIGHT))
	{
		left=0,right=1,up=0,down=0;
	}
	else if(GetAsyncKeyState(VK_UP))
	{
		left=0,right=0,up=1,down=0;
	}
	else if(GetAsyncKeyState(VK_DOWN))
	{
		left=0,right=0,up=0,down=1;
	}
	else if(GetAsyncKeyState(VK_ESCAPE))
		escape=1;
}

void pacman()
{
	int x=2,y=2;
	left=0;
	right=0;
	up=0;
	down=1;
	while(!GetAsyncKeyState(VK_ESCAPE))
	{
		while(down)
		{
			check();
			gotoxy(x,y);
			cout<<"*"<<endl;
			gotoxy(x,y-1);
			cout<<" ";
			delay(2);
			if(y==24)
			{
				gotoxy(x,y);
				cout<<" ";
				y=1;
			}
			y++;

			if(left||right||up||escape)
			{
				gotoxy(x,y-1);
				cout<<" ";
				goto out0;
			}
		}

		out0:
		while(up)
		{
			check();
			gotoxy(x,y);
			cout<<"*"<<endl;
			gotoxy(x,y+1);
			cout<<" ";
			delay(2);
			if(y==2)
			{
				gotoxy(x,y);
				cout<<" ";
				y=25;
			}
			y--;
			check();
			if(left||right||down||escape)
			{
				gotoxy(x,y+1);cout<<" ";goto out1;
			}
		}

		out1:
		while(left)
		{
			check();
			gotoxy(x,y);
			cout<<"*"<<endl;
			gotoxy(x+1,y);
			cout<<" ";
			delay();
			if(x==0)
			{
				gotoxy(x,y);
				cout<<" ";
				x=80;
			}
			x--;
			check();
			if(down||right||up||escape)
			{
				gotoxy(x+1,y);
				cout<<" ";
				gotoxy(x,y);
				cout<<" ";
				goto out2;
			}
		}

		out2:
		while(right)
		{
			check();
			gotoxy(x,y);
			cout<<"*"<<endl;
			gotoxy(x-1,y);
			cout<<" ";
			delay();
			if(x==80)
			{
				gotoxy(x,y);
				cout<<" ";
				x=0;
			}
			x++;
			check();
			if(left||down||up||escape)
			{
				gotoxy(x-1,y);
				cout<<" ";
				goto out3;
			}
		}

		out3:
	}
	return;
}
////////////////////////////////////snake game ends///////////////////////////////////////////
void main()
{
	U.login();
	menu();
	clrscr();
}
