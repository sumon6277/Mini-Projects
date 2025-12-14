#include<stdio.h>
#include<Windows.h>
#include<string.h>
#include<conio.h>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>
#include<stdbool.h>

struct Product
{
    int id;
    char name[20];
    int quantity;
    float price;
};

struct uProduct
{
    int id;
    char name[20];
    float price;
};

struct Product p;
struct uProduct up;

pthread_mutex_t lock;
sem_t sem;

void settingAdminLogInfo();
void inputPassword(char *password, int size);
void adminLog();
void setColor(WORD color);
void userLog();
void mainMenu();
void addProduct();
void viewProduct();
void updateProduct();
void searchProduct();
void removeProduct();
void* inputProductThread();
void* addProductThread();
void updateProduct();
void* inputUpdateThread();
void* addUpdateThread();
void adminView();

HANDLE h;

FILE *logfile;
FILE *userlog;
FILE *inventory;
FILE *purchase;
FILE *temp;

bool updateName;
bool updatePrice;

void addProduct()
{
    pthread_t p1,p2;

    pthread_create(&p1,NULL,inputProductThread,NULL);
    pthread_create(&p2,NULL,addProductThread,NULL);

    pthread_join(p1,NULL);
    pthread_join(p2,NULL);

    printf("Product added into file successful...\n");

}
void* inputProductThread(void* arg)
{
    pthread_mutex_lock(&lock);

    printf("\nEnter productID : ");
    scanf("%d",&p.id);
    getchar();
    printf("\nEnter product name : ");
    fgets(p.name,sizeof(p.name),stdin);
    p.name[strcspn(p.name,"\n")] = '\0';
    printf("\nEnter product quantity : ");
    scanf("%d",&p.quantity);
    printf("\nEnter product price : ");
    scanf("%f",&p.price);

    printf("Input taken successful...\n");

    pthread_mutex_unlock(&lock);
    sem_post(&sem);

}
void* addProductThread(void* arg)
{
    sem_wait(&sem);
    pthread_mutex_lock(&lock);

    inventory = fopen("inventory.txt","a");
    fprintf(inventory,"%d %s %d %.2f\n",p.id,p.name,p.quantity,p.price);
    fclose(inventory);

    pthread_mutex_unlock(&lock);
    printf("Adding into file successful\n");
}

void updateProduct()
{
    pthread_t p1,p2;
    
    pthread_create(&p1,NULL,inputUpdateThread,NULL);

    pthread_create(&p2,NULL,addUpdateThread,NULL);
    pthread_join(p1,NULL);
    pthread_join(p2,NULL);

    printf("Product updated into file successful...\n");

}

void* inputUpdateThread(void* arg)
{
    pthread_mutex_lock(&lock);
    updateName = false;
    updatePrice = false;

    printf("\nEnter productID to be updated : ");
    scanf("%d",&up.id);
    inventory = fopen("inventory.txt","r");

    while(fscanf(inventory,"%d %s %d %f",&p.id,p.name,&p.quantity,&p.price) != EOF)
    {
        if(up.id == p.id)
        {
            printf("You can only update product name and price.\nWhat do you want to update?\n");
            printf("1-- name\n2-- price\n3-- Both\n");
            printf("choose your choice: ");
            int ch;
            scanf("%d",&ch);

            if(ch == 1)
            {
                getchar();
                printf("Enter product new name : ");
                fgets(up.name,sizeof(up.name),stdin);
                up.name[strcspn(up.name,"\n")] = '\0';
                updateName = true;
            }
            else if(ch == 2)
            {
                printf("Enter product new price : ");
                scanf("%f",&up.price);
                updatePrice = true;
            }
            else if(ch == 3)
            {
                getchar();
                printf("Enter product new name : ");
                fgets(up.name,sizeof(up.name),stdin);
                up.name[strcspn(up.name,"\n")] = '\0';
                updateName = true;
                printf("Enter product new price : ");
                scanf("%f",&up.price);
                updatePrice = true;
            }
            else
            {
                printf("You have chosen a wrong choice...\n");
                return NULL;
            }
        }
    }
    fclose(inventory);
    pthread_mutex_unlock(&lock);
    sem_post(&sem);
    
    return NULL;
}

void* addUpdateThread(void* arg)
{
    sem_wait(&sem);
    pthread_mutex_lock(&lock);
    inventory = fopen("inventory.txt","r");
    if(!inventory)
    {
        return NULL;
    }
    temp = fopen("temp.txt","w");
    if(!temp)
    {
        return NULL;
    }
    while(fscanf(inventory,"%d %s %d %f",&p.id,p.name,&p.quantity,&p.price) != EOF)
    {
        if(up.id == p.id)
        {
            if(updateName==true && updatePrice ==false)
            {
                fprintf(temp,"%d %s %d %.2f\n",p.id,up.name,p.quantity,p.price);
            }
            else if(updateName==false && updatePrice ==true)
            {
                fprintf(temp,"%d %s %d %.2f\n",p.id,p.name,p.quantity,up.price);
            }
            else if(updateName==true && updatePrice ==true)
            {
                fprintf(temp,"%d %s %d %.2f\n",p.id,up.name,p.quantity,up.price);
            }
        }
        else
        {
            fprintf(temp,"%d %s %d %.2f\n",p.id,p.name,p.quantity,p.price);
        }
    }

    fclose(inventory);
    fclose(temp);

    remove("inventory.txt");
    rename("temp.txt","inventory.txt");
    pthread_mutex_unlock(&lock);
}
void setColor(WORD color)
{
    SetConsoleTextAttribute(h,color);
}

void adminView()
{
    printf("\nAdmin Pannel\n");
    printf("\n1-- Add product");
    printf("\n2-- Update Product");
    printf("\n0-- Exit");
    printf("\nEnter your choice: ");
    int ch;
    scanf("%d",&ch);
    if(ch == 1)
    {
        addProduct();
        adminView();
    }
    else if(ch == 2)
    {
        updateProduct();
        adminView();
    }
    else if(ch == 0)
    {
        return;
    }
    else
    {
        printf("You have chosen a wrong choice...\n");
        return;
    }
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
            adminView();
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

    pthread_mutex_init(&lock,NULL);
    sem_init(&sem,0,0);

    mainMenu();
    
    pthread_mutex_destroy(&lock);
    sem_destroy(&sem);
    return 0;
}