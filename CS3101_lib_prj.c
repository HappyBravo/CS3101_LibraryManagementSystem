#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_SIZE_USER_NAME 30
#define MAX_SIZE_PASSWORD  20
#define MAX_SIZE_USER_TYPE 6
#define LOGIN_DB "login_info.txt"
//#define FILE_HEADER_SIZE sizeof(login_FileHeader)

#define MAX_BOOK_NAME 50
#define MAX_AUTHOR_NAME 50
#define MAX_USER_NAME 50
#define MAX_USER_ADDRESS 300
#define BOOK_FILE_NAME "books_info.txt"
//#define BOOK_FILE_HEADER_SIZE sizeof("books_info.txt")

/*
// MAY BE USED IN BORROW / RETURN FUNCTION
typedef struct
{
    int yyyy;
    int mm;
    int dd;
} Date;
*/

typedef struct
{
    char username[MAX_SIZE_USER_NAME];
    char password[MAX_SIZE_PASSWORD];
    char type[MAX_SIZE_USER_TYPE];
} login_FileHeader;

typedef struct
{
    unsigned int books_id;
    char bookName[MAX_BOOK_NAME];
    char authorName[MAX_AUTHOR_NAME];
} BookInfo;

char temp; // used for keeping some memory for "Click here to continue ..."
unsigned char u_type[MAX_SIZE_USER_TYPE] = {0}; // for communication between Main Menu and Login


//
/* - - - - Header of the page : START - - - - - */
//

//
// - - - - DO NOT SEPARATE - - - -
// printMessageCenter() AND headMessage()
// ( just for the sake of simplicity )
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


/* - - - - - - - - - - - - - - - - - - - - - -*/
/* - - - - - HELPING FUNCTIONS : START - - - -*/
/* - - - - - - - - - - - - - - - - - - - - - -*/

// - These functionns are used in main functions - //
// - mainly for checking and validating if - - - - //
// - an entry is correct or not. - - - - - - - - - //
// - There may be some other stuffs too. - - - - - //

// Checking valid Entry
int isNameValid(const char *name)
{
    // - - - - - - - - - - - - - - - - - - - - - - - - //
    // Checks if Entered Name is valid or not.
    // accepts alphabets and blank spaces " " only.
    // - - - - - - - - - - - - - - - - - - - - - - - - //
    int validName = 1;
    int len = 0;
    int i = 0;
    len = strlen(name);
    for(i = 0; i < len ; i++)
    {
        if( (!(isalpha(name[i])) && (name[i] != ' ')) && (name[i] != '\n'))
        {
            validName = 0;
            break;
        }
    }
    return validName;
}

// header for Add Book Page
void headerAddBookPage()
    {
        // used only in Add Book Page
        headMessage("ADD NEW BOOK");
        printf("\n");
        printMessageCenter("ENTER BOOK DETAILS BELOW:");
        printf("\n\t\t\t----------------------------------------------------------------------\n");
    }

// Check if book is already present
int isBookPresent(const int id)
{
    int present = 0;
    char bookName[MAX_BOOK_NAME] = {0};
    BookInfo addBookInfo = {0};
    FILE *fp = NULL;
    int status = 0;

    fp = fopen(BOOK_FILE_NAME,"r");
    if(fp == NULL)
    {
        printf("\n\t\t\tFile is not opened\n");
        exit(1);
    }
    while(!feof(fp))
    {
        fscanf(fp, "%u\n%[^\n]%*c%[^\n]%*c\n", &addBookInfo.books_id,addBookInfo.bookName,addBookInfo.authorName );
        //printf("\n- - - - \nID - %d,\nName - %s,\nAuthor - %s\n- - - - ", addBookInfo.books_id,addBookInfo.bookName, addBookInfo.authorName);
        
        if(addBookInfo.books_id == id)
        {
            present = 1;
            break;
        }
    }
    fclose(fp);
    
    return present;
}

/* - - - - - - - - - - - - - - - - - - - - - -*/
/* - - - - - HELPING FUNCTIONS : END - - - - -*/
/* - - - - - - - - - - - - - - - - - - - - - -*/




/* - - - - - - - - - - - - - - - - - - - - - -*/
/* - - - - - MAIN FUNCTIONS : START - - - - - */
/* - - - - - - - - - - - - - - - - - - - - - -*/

// Search books function
void searchBooks()
{
    int found = 0;
    char bookName[MAX_BOOK_NAME] = {0};
    BookInfo searchBookInfo = {0};
    FILE *fp = NULL;
    int status = 0;

    fp = fopen(BOOK_FILE_NAME,"r");
    if(fp == NULL)
    {
        printf("\n\t\t\tFile is not opened\n");
        exit(1);
    }
    headMessage("SEARCH BOOKS");

    printf("\n\n\t\t\tEnter Book Name to search : ");
    fflush(stdin);
    scanf("%[^\n]%*c", bookName);

    // we will read the file and store the data in some variable
    // and check if any Book Name matches with the entered Book Name
    // if matched, we print the details stored in variable and return to Main Menu.

    while(!feof(fp))
    {
        // -------------------------------------------------//

        /*
        fscanf(fp, "%d\n", &addBookInfo.books_id);
        fscanf(fp, "%[^\n]%*c", addBookInfo.bookName);
        fscanf(fp, "%[^\n]%*c", addBookInfo.authorName);
        */

        // ---------------------------------------------------// 
        
        //
        //We can write the above statements in single line.
        //
        fscanf(fp, "%u\n%[^\n]%*c%[^\n]%*c\n", &searchBookInfo.books_id, searchBookInfo.bookName, searchBookInfo.authorName);

        if(!strcmp(searchBookInfo.bookName, bookName))
        {
            found = 1;
            break;
        }
    }
    if(found)
    {
        printf("\n\t\t\tBook id = %u", searchBookInfo.books_id);
        printf("\n\t\t\tBook name = %s", searchBookInfo.bookName);
        printf("\n\t\t\tBook authorName = %s", searchBookInfo.authorName);
    }
    else
    {
        printf("\n\t\t\tNo Record");
    }

    fclose(fp);
    printf("\n\n\n\t\t\tPress any key to go to main menu.....");
    fflush(stdin);
    scanf("%c", &temp);
    //getchar();
}

// View books function
void viewBooks()
{
    int found = 0;
    BookInfo addBookInfo = {0};
    FILE *fp = NULL;
    int countBook = 1;

    headMessage("VIEW BOOKS DETAILS");
    
    fp = fopen(BOOK_FILE_NAME, "r"); // opened file in read mode
    if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }

    // we will simply read the content of file and print them in formatted order.

    while(!feof(fp))
    {
        fscanf(fp, "%u\n%[^\n]%*c%[^\n]%*c\n", &addBookInfo.books_id,addBookInfo.bookName,addBookInfo.authorName );
        printf("\n\t\t\t%d.)", countBook);
        printf("\n\t\t\t\tBook id = %u", addBookInfo.books_id);
        printf("\n\t\t\t\tBook name = %s", addBookInfo.bookName);
        printf("\n\t\t\t\tBook authorName = %s\n", addBookInfo.authorName);
        found = 1;
        ++countBook;
    }
    fclose(fp);

    if(!found)
    {
        printf("\n\t\t\tNo Record");
    }
    printf("\n\n\t\t\tPress any key to go to main menu.....");
    fflush(stdin);
    scanf("%c", &temp);
}

// Borrow Book (to do)
void borrowBook()
{
    /* work in progress */
}

// Return Book (to do)
void returnBook()
{
    /* work in progress */
}

// Delete function
void deleteBooks()
{
    int found = 0;
    int bookDelete = 0;
    char bookName[MAX_BOOK_NAME] = {0};
    BookInfo delBookInfo = {0};
    FILE *fp = NULL; // original file
    FILE *tmpFp = NULL; // temporary file

    headMessage("DELETE BOOK");
    
    // opening original file
    fp = fopen(BOOK_FILE_NAME,"r");
    if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }

    // making temporary file
    tmpFp = fopen("tmp.txt","w");
    if(tmpFp == NULL)
    {
        fclose(fp);
        printf("File is not opened\n");
        exit(1);
    }

    printf("\n\t\t\tEnter Book ID NO. for delete : ");
    scanf("%d",&bookDelete);

    // we will scan each line and store it
    // then we will write the stored data in new temporary file
    // we will skip to write the book which is to be deleted
    // now delete original file and rename the temporary file with the original one.

    while(!feof(fp))
    {
        fscanf(fp, "%u\n%[^\n]%*c%[^\n]%*c\n", &delBookInfo.books_id, delBookInfo.bookName, delBookInfo.authorName);
        
        if(delBookInfo.books_id != bookDelete)
        {
            fprintf(tmpFp, "%u\n%s\n%s\n", delBookInfo.books_id, delBookInfo.bookName, delBookInfo.authorName);
        }
        else
        {
            found = 1;
        }
    }

    (found)? printf("\n\t\t\tRecord deleted successfully....."):printf("\n\t\t\tRecord not found");
    fclose(fp);
    fclose(tmpFp);
    
    remove(BOOK_FILE_NAME);
    rename("tmp.txt", BOOK_FILE_NAME);

    printf("\n\n\t\t\tPress any key to go to main menu.....");
    fflush(stdin);
    scanf("%c", &temp);
}

// Add book in list
void addBook()
{
    int count = 0;
    int status = 0;
    BookInfo addBookInfo = {0};
    FILE *fp = NULL;

    fp = fopen(BOOK_FILE_NAME, "a"); // opened file in append mode for adding
    if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }

    // We will take all entries and store it in some variable.
    // and check if those are valid entries. 
    // then append those values in the file.

    do
    {
        if (count>3)
        {
            break;
        }
        headerAddBookPage();

        printf("\n\t\t\tBook ID NO  = ");
        fflush(stdin);
        scanf("%u", &addBookInfo.books_id);

        // check if Book already exist
        status = isBookPresent(addBookInfo.books_id);

        if(status)
        {
            headerAddBookPage();
            printf("\n\t\t\tBook already exist. Please enter new book.");
            printf("\n\n\t\t\tPress any key to retry.....");
            fflush(stdin);
            scanf("%c", &temp);
            count++;
        }
        else
        {
            count = 0;
        }
    } while (status);
    
    do
    {
        if (count>3)
        {
            break;
        }
        
        printf("\n\t\t\tBook Name  = ");
        fflush(stdin);
        scanf("%[^\n]%*c", addBookInfo.bookName);

        // check if entered name is valid.
        status = isNameValid(addBookInfo.bookName);
        if (!status)
        {
            printf("\n\t\t\tName contain invalid character. Please enter again.....");
            count++;
        }
        else
        {
            count = 0;
        }
    }
    while(!status);

    do
    {
        if (count>3)
        {
            break;
        }
        
        printf("\n\t\t\tAuthor Name  = ");
        fflush(stdin);
        
        scanf("%[^\n]%*c", addBookInfo.authorName);
        
        // check if entered name is valid
        status = isNameValid(addBookInfo.authorName);
        if (!status)
        {
            printf("\n\t\t\tName contain invalid character. Please enter again.");
            count++;
        }
        else
        {
            count = 0;
        }
    }
    while(!status);
    
    if (count<3)
    {
        // count < 3 means that we have entered all values correctly and are valid.
        headerAddBookPage();
        fprintf(fp, "\n%u\n%s\n%s", addBookInfo.books_id, addBookInfo.bookName, addBookInfo.authorName);
        printf("\n\t\t\tRecord added successfully.....");
    }
    else
    {
        // if values are not valid, nothing is added and we simply go out to Main Menu.
        headerAddBookPage();
        printf("\n\t\t\tTry Again.");
    }
    fclose(fp);

    printf("\n\n\t\t\tPress any key to go to main menu.....");
    fflush(stdin);
    scanf("%c", &temp);
}

// Update book information
void updateBook()
{
    int found = 0;
    int status = 0;
    int count = 0;
    BookInfo addBookInfo = {0};
    BookInfo updateBookInfo = {0};
    FILE *fp = NULL; // original file
    FILE *tmpFp = NULL; // temporary file

    headMessage("UPDATE BOOK");

    fp = fopen(BOOK_FILE_NAME,"r"); // opened original file in read mode
    if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }

    tmpFp = fopen("tmp.txt","w"); // made a temporary file in write mode
    if(tmpFp == NULL)
    {
        fclose(fp);
        printf("File is not opened\n");
        exit(1);
    }

    // firstly, we will check if book to be updated exists
    // if it exist, then only we will perform actions, else we will return to Main Menu
    do
    {
        if (count>3)
        {
            break;
        }
        printf("\n\t\t\tEnter Book ID No to update = ");
        fflush(stdin);
        scanf("%u", &updateBookInfo.books_id);

        // check if Book already exist
        status = isBookPresent(updateBookInfo.books_id);
        if(!status)
        {
            printf("\n\t\t\tBook does not exist. Please enter correct Book ID.");
            printf("\n\n\t\t\tPress any key to retry.....");
            fflush(stdin);
            scanf("%c", &temp);
            count++;
            headMessage("UPDATE BOOK");

        }
        else
        {
            count = 0;
        }
    } while (!status);

    if(status)
    {
        // if book exits then we perform following functions.
        // we will take the necessary details and store them in some varibale
        // then we copy the data from original file to temporary file
        // the book id in original file matches with the book id to be updated,
        // we copy the data stored in the variable to the temporary file
        // after copying, we delete original file, and rename it.

        do
        {
            if (count>3)
            {
                break;
            }
            printf("\n\t\t\tBook Name  = ");
            fflush(stdin);
            //fgets(addBookInfo.bookName, MAX_BOOK_NAME, stdin);
            scanf("%[^\n]%*c", updateBookInfo.bookName);
            //printf("= %s\n", addBookInfo.bookName);

            status = isNameValid(updateBookInfo.bookName);
            if (!status)
            {
                printf("\n\t\t\tName contain invalid character. Please enter again.....");
                count++;
            }
            else
            {
                count = 0;
            }
        }while(!status);

        do
        {
            if (count>3)
            {
                break;
            }
            printf("\n\t\t\tAuthor Name  = ");
            fflush(stdin);
            scanf("%[^\n]%*c", updateBookInfo.authorName);

            status = isNameValid(updateBookInfo.authorName);
            if (!status)
            {
                printf("\n\t\t\tName contain invalid character. Please enter again.");
                count++;
            }
            else
            {
                count = 0;
            }
        }while(!status);

        while(!feof(fp))
        {
            fscanf(fp, "%u\n%[^\n]%*c%[^\n]%*c\n", &addBookInfo.books_id,addBookInfo.bookName,addBookInfo.authorName);

            if(addBookInfo.books_id != updateBookInfo.books_id)
            {
                fprintf(tmpFp, "%u\n%s\n%s\n", addBookInfo.books_id, addBookInfo.bookName, addBookInfo.authorName);
            }
            else
            {
                fprintf(tmpFp, "%u\n%s\n%s\n", updateBookInfo.books_id, updateBookInfo.bookName, updateBookInfo.authorName);
                found = 1;
            }
        }      
    }

    headMessage("UPDATE BOOK");
    if (found)
    {
        printf("\n\t\t\tRecord updated successfully.....");
        fclose(fp);
        fclose(tmpFp);
        remove(BOOK_FILE_NAME);
        rename("tmp.txt", BOOK_FILE_NAME);
    }
    else
    {
        printf("\n\t\t\tRecord not found.\n\t\t\tTry Again.");
        fclose(fp);
        fclose(tmpFp);
    }

    printf("\n\n\t\t\tPress any key to go to main menu.....");
    fflush(stdin);
    scanf("%c", &temp);

}

/* - - - - - - - - - - - - - - - - - - - - - -*/
/* - - - - - - MAIN FUNCTIONS : END- - - - - -*/
/* - - - - - - - - - - - - - - - - - - - - - -*/



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
            {
                // simply making choice = 10 which is not in switch cases.
                // so that "user" cannot use the operations meant for "admin".
                choice = 10;
            }
        }

        switch(choice)
        {
        case 1:
            printf("searchbook");
            searchBooks();
            break;
        case 2:
            printf("viewbooks");
            viewBooks();
            break;
        case 3:
            printf("borrowbooks");
            //borrowBook();
            break;
        case 4:
            printf("returnbooks");
            //returnBook();
            break;
        case 5:
            printf("addbooks");
            addBook();
            break;
        case 6:
            printf("deleteboks");
            deleteBooks();
            break;
        case 7:
            printf("updatebooks");
            updateBook();
            break;
        case 0:
            printf("\n\n\n\t\t\t\tThank you!!!\n\n\n\n\n");
            exit(1);
            break;
        default:
            printf("\n\n\n\t\t\tINVALID INPUT!!! Try again...");
            fflush(stdin);
            scanf("%c", &temp);
        }
    }
    while(choice != 0);   //if choice = 0, we exit, else we reprint the Main Menu page
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
        headMessage("LOGIN");
        printf("\n\n\n\t\t\t\tUsername: ");
        scanf("%s", userName);
        
        printf("\n\t\t\t\tPassword: ");
        scanf("%s", password);

        flag = 0;
        fp = fopen(LOGIN_DB, "r"); // opened file in read mode
        if(fp == NULL)
            {
                printf("File is not opened\n");
                exit(1);
            }
        // we will read the data in file and check if the data matches with entered data.
        // if matched, we go to Main Menu,
        // else try 3 times after which we close the program.
        while(!feof(fp))
        {
            fscanf(fp, "%s %s %s", login_info.username, login_info.password, login_info.type);

            if((!strcmp(userName,login_info.username)) && (!strcmp(password, login_info.password)))
            {
                headMessage("LOGIN");
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
                headMessage("LOGIN");
                printf("\n\t\t\t\tLogin Failed");
                printf("\n\t\t\t\tTry Again\n");
                printf("\n\n\n\t\t\t\t Enter any key to try again.....");
                fflush(stdin);
                scanf("%c", &temp);
                L++;
            }
        fclose(fp);
    }
    while(L != 0 && L <= 3);

    if(L>3)
    {
        headMessage("LOGIN FAILED");
        printf("\n\t\t\t\tSorry, Unknown User.");
        printf("\n\n\n\t\t\t\t Enter any key to continue.....");
        fflush(stdin);
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