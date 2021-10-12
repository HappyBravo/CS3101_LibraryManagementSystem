#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE_USER_NAME 30
#define MAX_SIZE_PASSWORD  20
#define MAX_SIZE_USER_TYPE 6
#define LOGIN_DB "login_info.txt"
#define FILE_HEADER_SIZE sizeof(login_FileHeader)

typedef struct
{
    char username[MAX_SIZE_USER_NAME];
    char password[MAX_SIZE_PASSWORD];
    char type[MAX_SIZE_USER_TYPE];
} login_FileHeader;


char temp;
unsigned char u_type[MAX_SIZE_USER_TYPE] = {};


//
/* - - - - Header of the page : START - - - - - */
//

//
// - - - - DO NOT SEPARATE - - - -
// printMessageCenter() AND headMessage()
// (just for the sake of simplicity)
// - - - - - - - - - - - - - - - -
//
void printMessageCenter(const char* message)
{
    int len = 0;
    int pos = 0;
    
    //calculate how many space need to print
    len = (70 - strlen(message))/2;
    printf("\t\t\t"); // initial padding

    for(pos=0; pos<len; pos++)
    {
        printf(" "); // print space
    }

    printf("%s",message); // print message
}

void headMessage(const char *message)
{
    system("cls");
    printf("\t\t\t######################################################################");
    printf("\n\t\t\t############                                              ############");
    printf("\n\t\t\t############           Library management System          ############");
    printf("\n\t\t\t############                                              ############");
    printf("\n\t\t\t######################################################################");
    printf("\n\t\t\t----------------------------------------------------------------------\n");
    printMessageCenter(message);
    printf("\n\t\t\t----------------------------------------------------------------------");
}
//
/* - - - - Header of the page : END - - - - - */
//



//
/* - - - - First Page of the project : START - - - - - */
//
void welcomeMessage()
{
    printf("\n\n\n\n");
    printf("\n\t\t\t\t  **-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**");
    printf("\n\t\t\t\t        = = = = = = = = = = = = = = = = = = = = = = =");
    printf("\n\t\t\t\t        =                 WELCOME                   =");
    printf("\n\t\t\t\t        = = = = = = = = = = = = = = = = = = = = = = =");
    printf("\n\t\t\t\t  **-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**\n");
    printf("\n\n\n\t\t\t\t Enter any key to continue.....");
    //getch();
    scanf("%c", &temp);
}
//
/* - - - - First Page of the project : END - - - - - */
//


//
/* - - - - Main Menu page : START - - - - - */
//
void menu()
{
    int choice = 0;
    int check = strcmp(u_type, "admin"); // check if user is admin or not
    do
    {
        headMessage("MAIN MENU");
        printf("\n\n\n\t\t\t1.Search Books");
        printf("\n\t\t\t2.View Books");
        printf("\n\t\t\t3.Borrow Book");
        printf("\n\t\t\t4.Return Book");

        if(!(check))
        {
            // only admin can do these operations
            printf("\n\t\t\t5.Add Book");
            printf("\n\t\t\t6.Delete Book");
            printf("\n\t\t\t7.Update Book");
        }
        printf("\n\t\t\t0.Exit");
        printf("\n\n\n\t\t\tEnter choice => ");
        scanf("%d",&choice);

        if(check)
        {
            if(choice>4)
                choice = 10; // simply making choice = 10 which is not in switch cases.
        }

        switch(choice)
        {
        case 1:
            printf("searchbook");
            //searchBooks();
            break;
        case 2:
            printf("viewbooks");
            //viewBooks();
            break;
        case 3:
            printf("borrowbooks");
            //borrowBooks();
            break;
        case 4:
            printf("returnbooks");
            //returnBooks();
            break;
        case 5:
            printf("addbooks");
            //addBookInDataBase();
            break;
        case 6:
            printf("deleteboks");
            //deleteBooks();
            break;
        case 7:
            printf("updatebooks");
            //updateBookInfo();
            break;
        case 0:
            printf("\n\n\n\t\t\t\tThank you!!!\n\n\n\n\n");
            exit(0);
            break;
        default:
            printf("\n\n\n\t\t\tINVALID INPUT!!! Try again...");
            fflush(stdin);
            scanf("%c", &temp);
        }                                            //Switch Ended
    }
    while(choice!=0);   //Loop Ended
}
//
/* - - - - Main Menu page : END - - - - - */
//


//
/* - - - - Login-password page : START - - - - - */
//
void login()
{
    unsigned char userName[MAX_SIZE_USER_NAME] = {};
    unsigned char password[MAX_SIZE_PASSWORD] = {};
    
    int L = 1;
    login_FileHeader login_info = {0};
    FILE *fp = NULL;
    int flag = 0;

    do
    {
        headMessage("Login");
        printf("\n\n\n\t\t\t\tUsername: ");
        scanf("%s", userName);
        
        printf("\n\t\t\t\tPassword: ");
        scanf("%s", password);
   
        //printf("Entered U %s\n",userName);
        //printf("Entered P %s\n", password);
        flag = 0;
        fp = fopen(LOGIN_DB,"r");
        if(fp == NULL)
            {
                printf("File is not opened\n");
                exit(1);
            }

        while(!feof(fp))
        {
            fscanf(fp, "%s %s %s", login_info.username, login_info.password, login_info.type);
            //printf("\nU %s\n", login_info.username);
            //printf("\nP %s\n", login_info.password);
            //printf("%d", strcmp(userName, login_info.username));
            //printf("%d", strcmp(password, login_info.password));

            if((!strcmp(userName,login_info.username)) && (!strcmp(password, login_info.password)))
            {
                printf("Login Success");
                strcpy(u_type, login_info.type);
                L = 0;
                flag = 1;
                menu();
                break;
            }
        }

        if(!(flag))
            {
                printf("\n\t\t\t\tLogin Failed");
                printf("\n\t\t\t\tTry Again\n");
                printf("\n\n\n\t\t\t\t Enter any key to try again.....");
                scanf("%c", &temp);
                scanf("%c", &temp);
                L++;
            }
        fclose(fp);
    }
    while(L != 0 && L <= 3);

    if(L>3)
    {
        headMessage("Login Failed");
        printf("\n\t\t\t\tSorry, Unknown User.");
        printf("\n\n\n\t\t\t\t Enter any key to continue.....");
        fflush(stdin);
        //getch();
        scanf("%c",&temp);
        system("cls");
    }
}
//
/* - - - - Login-password page : END - - - - - */
//


int main()
{
    welcomeMessage();
    login();
    return 0;
}