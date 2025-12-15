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

struct User
{
    int id;
    struct Product p;
    float total;
    float payment;
};

struct Product p;
struct uProduct up;
struct User u;

int found = 0;
int payment_done = 0;

pthread_mutex_t lock;
sem_t sem;

void setColor();
void mainMenu();
void adminLog();
void settingAdminLogInfo();
void userLog();
void inputPassword();
void adminView();
void userView();
void addProduct();
void* inputProductThread();
void* addProductThread();
void updateProduct();
void* inputUpdateThread();
void* addUpdateThread();
void searchProduct();
void* inputSearchProduct();
void* viewSearchProduct();
void deleteProduct();
void* inputDeleteProduct();
void* addDeleteProduct();
void purchaseProduct();
void payment();
void* updateInventory(void* args);
void* storeSalesProduct();

HANDLE h;

FILE *logfile;
FILE *userlog;
FILE *inventory;
FILE *purchase;
FILE *temp;
FILE *userpurchase;
FILE *usertemp;
FILE *sales;

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
    if(!inventory)
    {
        printf("No product found\n");
        return NULL;
    }

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

    if(!inventory)
    {
        printf("No product found\n");
        return NULL;
    }

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
        printf("No product found\n");
        return NULL;
    }

    temp = fopen("temp.txt","w");
    if(!temp)
    {
        printf("A simple error occured\n");
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
    
    return NULL;
}

void viewProduct() {
    inventory = fopen("inventory.txt", "r");

    if(!inventory)
    {
        printf("No product found\n");
        return NULL;
    }

    printf("\nID\tName\tQty\tPrice\n");
    while (fscanf(inventory, "%d %s %d %f",&p.id, p.name, &p.quantity, &p.price) != EOF) 
    {
        printf("%d\t%s\t%d\t%.2f\n",p.id, p.name, p.quantity, p.price);
    }
    fclose(inventory);
}

void searchProduct()
{
    pthread_t t1,t2;

    pthread_create(&t1,NULL,inputSearchProduct,NULL);
    pthread_create(&t2,NULL,viewSearchProduct,NULL);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);

    return;
}
void* inputSearchProduct(void* args)
{
    printf("\nEnter productID to search : ");
    scanf("%d",&up.id);

    sem_post(&sem);
    return NULL;
}
void* viewSearchProduct(void* args) 
{
    sem_wait(&sem);
    inventory = fopen("inventory.txt", "r");

    if(!inventory)
    {
        printf("No product found\n");
        return NULL;
    }

    while (fscanf(inventory, "%d %s %d %f",&p.id, p.name, &p.quantity, &p.price) != EOF) {
        if (p.id == up.id) {
            printf("Product-Found: %s Quantity:%d Price:%.2f\n",p.name, p.quantity, p.price);
            pthread_mutex_lock(&lock);
            found = 1;
            pthread_mutex_unlock(&lock);
            break;
        }
    }

    if (found == 0)
    {
        printf("Product not found!\n");
    }

    fclose(inventory);
    return NULL;
}

void deleteProduct() 
{
    pthread_t t1,t2;

    pthread_create(&t1,NULL,inputDeleteProduct,NULL);
    pthread_create(&t2,NULL,addDeleteProduct,NULL);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    
    printf("Product Deleted!\n");

    return;
}

void* inputDeleteProduct(void* args)
{
    printf("\nEnter ID to delete: ");
    scanf("%d", &up.id);

    sem_post(&sem);

    return NULL;
}

void* addDeleteProduct(void* args)
{
    sem_wait(&sem);
    pthread_mutex_lock(&lock);

    inventory = fopen("inventory.txt", "r");

    if(!inventory)
    {
        printf("No product found\n");
        return NULL;
    }

    temp = fopen("temp.txt", "w");

    if(!temp)
    {
        printf("A simple error occured\n");
        return NULL;
    }

    while (fscanf(inventory, "%d %s %d %f",&p.id, p.name, &p.quantity, &p.price) != EOF) {
        if (p.id != up.id)
        {
            fprintf(temp, "%d %s %d %.2f\n",p.id, p.name, p.quantity, p.price);
        }
    }

    fclose(inventory);
    fclose(temp);

    remove("inventory.txt");
    rename("temp.txt", "inventory.txt");

    pthread_mutex_unlock(&lock);

    return NULL;
}

void purchaseProduct()
{
    pthread_t t1,t2,t3;
    found = 0;
    payment_done = 0;

    u.id = rand()%5 + 1;
    
    printf("Enter productID to make purchase: ");
    scanf("%d",&u.p.id);

    printf("\nEnter quantity : ");
    scanf("%d",&u.p.quantity);

    pthread_mutex_lock(&lock);
    inventory = fopen("inventory.txt","r");
    
    if(!inventory)
    {
        printf("No product found\n");
        return NULL;
    }

    while (fscanf(inventory, "%d %s %d %f",&p.id, p.name, &p.quantity, &p.price) != EOF) 
    {
        if (p.id == u.p.id && p.quantity >= u.p.quantity) {
            found = 1;
            strcpy(u.p.name, p.name);
            u.total = u.p.quantity * p.price;
            break;
        }
    }
    
    fclose(inventory);
    pthread_mutex_unlock(&lock);

    if (!found) {
        printf("Insufficient stock or product not found!\n");
        return;
    }

    pthread_mutex_lock(&lock);
    userpurchase = fopen("userpurchase.txt","a");

    if(!userpurchase)
    {
        printf("No product is sold\n");
        return NULL;
    }

    fprintf(userpurchase, "%d %s %d %.2f\n", u.id, u.p.name, u.p.quantity, u.total);

    fclose(userpurchase);
    pthread_mutex_unlock(&lock);

    payment();

    if (!payment_done) {
        printf("Payment not completed. Purchase terminated.\n");
        return;
    }
    pthread_create(&t2,NULL,updateInventory,NULL);
    pthread_create(&t3,NULL,storeSalesProduct,NULL);

    pthread_join(t2,NULL);
    pthread_join(t3,NULL);

    return;
}

void payment()
{
    printf("\nDo you want to pay now or later on? y/n : ");

    char ch;
    scanf(" %c",&ch);

    if(ch == 'y')
    {
        int uid;

        printf("\nEnter your userID: ");
        scanf("%d",&uid);

        u.payment = 0;
        
        pthread_mutex_lock(&lock);
        userpurchase = fopen("userpurchase.txt","r");

        if(!userpurchase)
        {
            printf("No product is sold\n");
            pthread_mutex_unlock(&lock);
            return;
        }

        while(fscanf(userpurchase, "%d %s %d %f",&u.id,u.p.name,&u.p.quantity,&u.total) != EOF)
        {
            if(uid == u.id)
            {
                u.payment += u.total;
            }
        }

        rewind(userpurchase);

        printf("Payment amount is %.2f\n",u.payment);
        sleep(1);
        printf("Payment is processing...\n");
        sleep(1);
        printf("Payment successful...\n");

        payment_done = 1;

        usertemp = fopen("usertemp.txt","w");
        
        if(!usertemp)
        {
            printf("A simple error occured\n");
            return NULL;
        }

        while(fscanf(userpurchase, "%d %s %d %f",&u.id,u.p.name,&u.p.quantity,&u.total) != EOF)
        {
            if(uid != u.id)
            {
                fprintf(usertemp, "%d %s %d %.2f\n", u.id, u.p.name, u.p.quantity, u.total);
            }
        }

        fclose(userpurchase);
        fclose(usertemp);

        remove("userpurchase.txt");
        rename("usertemp.txt","userpurchase.txt");

        pthread_mutex_unlock(&lock);
        return;
    }
    else if(ch == 'n')
    {
        printf("Your payment is pending...\nPurchasing is terminated");
        adminView();
        return;
    }
}

void* updateInventory(void* args)
{
    pthread_mutex_lock(&lock);
    if (!payment_done) 
    {
        pthread_mutex_unlock(&lock);
        pthread_exit(NULL);
    }
    
    inventory = fopen("inventory.txt", "r");
    temp = fopen("temp.txt", "w");
    
    while (fscanf(inventory, "%d %s %d %f",&p.id, p.name, &p.quantity, &p.price) != EOF) {
        if (p.id == u.p.id)
        {
            p.quantity -= u.p.quantity;
        }

        fprintf(temp, "%d %s %d %.2f\n", p.id, p.name, p.quantity, p.price);
    }

    fclose(inventory);
    fclose(temp);

    remove("inventory.txt");
    rename("temp.txt", "inventory.txt");

    return NULL;
}

void* storeSalesProduct(void* arg) {

    pthread_mutex_lock(&lock);
    if (!payment_done) 
    {
        pthread_mutex_unlock(&lock);
        pthread_exit(NULL);
    }

    sales = fopen("sales.txt", "a");
    fprintf(sales, "%d %s %d %.2f\n",u.id, u.p.name, u.p.quantity, u.payment);
    fclose(sales);

    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}

void viewSales()
{
    sales = fopen("sales.txt","r");
    if(!sales)
    {
        printf("There is no sales statement for today.\n");
        return;
    }

    printf("UserID\tProductName\tQuantity\tTotal\n");
    while(fscanf(sales, "%d %s %d %f",&u.id, u.p.name, &u.p.quantity, &u.payment) != EOF)
    {
        printf("%d\t%s\t%d\t%.2f\n",u.id,u.p.name,u.p.quantity,u.payment);
    }
    fclose(sales);

    return;
}

void setColor(WORD color)
{
    SetConsoleTextAttribute(h,color);
}

void adminView()
{
    setColor(FOREGROUND_GREEN);

    printf("\nAdmin Pannel\n");
    printf("\n1-- Add product");
    printf("\n2-- Update Product");
    printf("\n3-- View Product");
    printf("\n4-- Search Product");
    printf("\n5-- Delete Product");
    printf("\n6-- Add to Cart");
    printf("\n7-- Per day sale");
    printf("\n0-- Exit");

    int ch;
    printf("\nEnter your choice: ");
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
    else if(ch == 3)
    {
        viewProduct();
        adminView();
    }
    else if(ch == 4)
    {
        searchProduct();
        adminView();
    }
    else if(ch == 5)
    {
        deleteProduct();
        adminView();
    }
    else if(ch == 6)
    {
        purchaseProduct();
        adminView();
    }
    else if(ch == 7)
    {
        viewSales();
        adminView();
    }
    else if(ch == 0)
    {
        mainMenu();
        return;
    }
    else
    {
        printf("You have chosen a wrong choice...\n");
        mainMenu();
        return;
    }
}

void userView()
{
    setColor(FOREGROUND_GREEN);

    printf("\nUser Pannel\n");
    printf("\n1-- View Product");
    printf("\n2-- Search Product");
    printf("\n3-- Add to Cart");
    printf("\n4-- View per day sales: ");
    printf("\n0-- Exit");

    int ch;
    printf("\nEnter your choice: ");
    scanf("%d",&ch);

    if(ch == 1)
    {
        viewProduct();
        userView();
    }
    else if(ch == 2)
    {
        searchProduct();
        userView();
    }
   
    else if(ch == 3)
    {
        purchaseProduct();
        userView();
    }
    else if(ch == 4)
    {
        viewSales();
        userView();
    }
    else if(ch == 0)
    {
        mainMenu();
        return;
    }
    else
    {
        printf("You have chosen a wrong choice...\n");
        mainMenu();
        return;
    }
}

void adminLog()
{
    int tracking = 2;

    while(tracking--)
    {
        setColor(FOREGROUND_GREEN);

        char username[20];
        printf("\nEnter admin username : ");

        fgets(username,sizeof(username),stdin);
        username[strcspn(username,"\n")] = '\0';

        char password[20];
        printf("\nEnter admin password : ");
    
        inputPassword(password,sizeof(password));

        logfile = fopen("login.txt","r");

        if(logfile == NULL)
        {
            printf("\nFailed to open logfile...\n");
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
            sleep(1);
            adminView();
            return;
        }
        else
        {
            setColor(FOREGROUND_RED | FOREGROUND_GREEN);
            printf("\nLog in failed... You have %d more chance... \nTry again.\n",tracking);
            sleep(1);        
        }
    }

    setColor(FOREGROUND_RED);
    printf("\nOOPS!!!!!...\nYou crossed your try limit... \nNow you are kicking off on your butt , go to sleep!!!\n");
    
    sleep(1);
    setColor(FOREGROUND_GREEN);
    printf("\nYou wanna change log info? y/n : ");

    char ch;

    scanf("%c",&ch);
    getchar();

    if(ch == 'y')
    {
        settingAdminLogInfo();
        printf("\nLog in information updated successfully...\n");
        sleep(1.5);
        mainMenu();
    }
    else
    {
        sleep(1.5);
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
        printf("Failed to open logfile");
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
    
    return;
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
        char username[20];
        printf("\nEnter your username: ");

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

            sleep(1);
            userView();
            return;
        }
        else
        {
            setColor(FOREGROUND_RED | FOREGROUND_GREEN);
            printf("\nLog in failed... You have %d more chance... \nTry again.\n",tracking);
            sleep(1);
        }
    }

    setColor(FOREGROUND_RED);
    printf("\nOOPS!!!!!...\nYou crossed your try limit... \nNow you are kicking off on your butt , go to sleep!!!\n");
    sleep(1);
    mainMenu();
}

void mainMenu()
{
    setColor(FOREGROUND_GREEN);

    printf("\n1-- admin login\n");
    printf("2-- user login\n");
    printf("0-- Exit\n");
    
    int ch;
    printf("\nEnter your choice: ");

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