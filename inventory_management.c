#include<stdio.h>
#include<Windows.h>
#include<string.h>
#include<conio.h>

void settingAdminLogInfo();
void inputPassword(char *password, int size);
void adminLog();
void setColor(WORD color);
void userLog();
void mainMenu();
HANDLE h;

FILE *logfile;
FILE *userlog;

void setColor(WORD color)
{
    SetConsoleTextAttribute(h,color);
}

void adminLog()
{
    int tracking = 2;
    while(tracking--)
    {
        setColor(FOREGROUND_GREEN);

        printf("\nEnter admin username : ");
        char username[20];
        fgets(username,sizeof(username),stdin);
        username[strcspn(username,"\n")] = '\0';

        printf("\nEnter admin password : ");
        char password[20];
    
        inputPassword(password,sizeof(password));

        logfile = fopen("login.txt","r");

        if(logfile == NULL)
        {
            printf("\nFailed to open file...\n");
            mainMenu();
            return;
        }

        char usernameR[20];
        char passwordR[20];

        fgets(usernameR,sizeof(usernameR),logfile);
        usernameR[strcspn(usernameR,"\n")] = '\0';

        fgets(passwordR,sizeof(passwordR),logfile);
        passwordR[strcspn(passwordR,"\n")] = '\0';

        fclose(logfile);

        if(strcmp(username,usernameR) == 0 && strcmp(password, passwordR) == 0)
        {
            setColor(FOREGROUND_BLUE);
            printf("\nlog in successful");
            Sleep(1000);
            return;
        }
        else
        {
            setColor(FOREGROUND_RED | FOREGROUND_GREEN);
            printf("\nLog in failed... You have %d more chance... \nTry again.\n",tracking);
            Sleep(1000);        
        }
    }
    setColor(FOREGROUND_RED);
    printf("\nOOPS!!!!!...\nYou crossed your try limit... \nNow you are kicking off on your butt , go to sleep!!!\n");
    
    Sleep(1000);
    setColor(FOREGROUND_GREEN);
    printf("\nYou wanna change log info? y/n : ");

    char ch;

    scanf("%c",&ch);
    getchar();

    if(ch == 'y')
    {
        settingAdminLogInfo();
        printf("\nLog in information updated successfully...\n");
        Sleep(1500);
        mainMenu();
    }
    else
    {
        Sleep(1500);
        mainMenu();
        return;
    }
}

void inputPassword(char *password, int size)
{
    int i = 0;
    char ch;

    while (1)
    {
        ch = getch();   

        if (ch == 13)   
        {
            password[i] = '\0';
            printf("\n");
            break;
        }
        else if (ch == 8)  
        {
            if (i > 0)
            {
                i--;
                printf("\b \b");
            }
        }
        else
        {
            if (i < size - 1)
            {
                password[i++] = ch;
                printf("*");
            }
        }
    }
}


void settingAdminLogInfo()
{
    logfile = fopen("login.txt","w");

    if(logfile == NULL)
    {
        printf("Failed to open file");
        mainMenu();
        return;
    }

    char username[20];
    char password[10];
    printf("Enter new username : ");
    fgets(username,sizeof(username),stdin);
    username[strcspn(username,"\n")] = '\0';

    printf("Enter new password : ");

    inputPassword(password,sizeof(password));

    fprintf(logfile,"%s\n%s\n",username,password);
    fclose(logfile);
}

void userLog()
{
    int tracking = 2;
    while(tracking--)
    {
        setColor(FOREGROUND_GREEN);

        userlog = fopen("userlog.txt","r");
        if(userlog == NULL)
        {
            printf("Failed to open userlog file.\n");
            mainMenu();
            return;
        }
        printf("\nEnter your username: ");
        char username[20];
        fgets(username,sizeof(username),stdin);
        username[strcspn(username,"\n")] = '\0';

        char password[20];
        printf("Enter your password : ");
        inputPassword(password,sizeof(password));
    
        char usernameR[20];
        char passwordR[20];

        fgets(usernameR,sizeof(usernameR),userlog);
        usernameR[strcspn(usernameR,"\n")] = '\0';
    
        fgets(passwordR,sizeof(passwordR),userlog);
        passwordR[strcspn(passwordR,"\n")] = '\0';
    
        fclose(userlog);
    
        if(strcmp(username,usernameR) == 0 && strcmp(password, passwordR) == 0)
        {
            setColor(FOREGROUND_BLUE);
            printf("\nlog in successful");
            Sleep(1000);
            return;
        }
        else
        {
            setColor(FOREGROUND_RED | FOREGROUND_GREEN);
            printf("\nLog in failed... You have %d more chance... \nTry again.\n",tracking);
            Sleep(1000);
        }
    }
    setColor(FOREGROUND_RED);
    printf("\nOOPS!!!!!...\nYou crossed your try limit... \nNow you are kicking off on your butt , go to sleep!!!\n");
    Sleep(1000);
    mainMenu();
}

void mainMenu()
{
    setColor(FOREGROUND_GREEN);
    printf("\n1-- admin login\n");
    printf("2-- user login\n");
    printf("0-- Exit\n");
    
    printf("\nEnter your choice: ");
    int ch;
    scanf("%d",&ch);
    fflush(stdin);
    
    switch (ch)
    {
    case 1:
        adminLog();
        break;
    case 2:
        userLog();
        break;
    case 0:
        break;
    default:
        break;
    }

}
int main()
{   
    h = GetStdHandle(STD_OUTPUT_HANDLE);
    setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    mainMenu();
    
    return 0;
}