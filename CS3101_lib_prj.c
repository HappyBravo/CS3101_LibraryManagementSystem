#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>


#define MAX_SIZE_USER_NAME 30
#define MAX_SIZE_PASSWORD  20
#define MAX_SIZE_USER_TYPE 6
#define LOGIN_DB "login_info.txt"

#define MAX_BOOK_NAME 50
#define MAX_AUTHOR_NAME 50
#define MAX_USER_NAME 50
#define MAX_USER_ADDRESS 300
#define BOOK_FILE_NAME "books_info.txt"

#define REGISTER_FILE_NAME "register_book.txt"

//
/* - - - - GLOBAL VARIABLES : START - - - - - */
//
typedef struct
{
    int yyyy;
    int mm;
    int dd;
} Date;

typedef struct
{
    char username[MAX_SIZE_USER_NAME];
    char password[MAX_SIZE_PASSWORD];
    char type[MAX_SIZE_USER_TYPE];
    char member[MAX_SIZE_USER_TYPE];
} login_FileHeader;

typedef struct
{
    unsigned int books_id;
    char bookName[MAX_BOOK_NAME];
    char authorName[MAX_AUTHOR_NAME];
    int book_stock;
} BookInfo;

typedef struct
{
    unsigned int books_id;
    char username[MAX_SIZE_USER_NAME];
    Date i_date;
    Date r_date;
} RegisterInfo;

char temp; // used for keeping some memory for "Click here to continue ..."

unsigned char u[MAX_USER_NAME] = {0};
unsigned char u_type[MAX_SIZE_USER_TYPE] = {0}; // for communication between Main Menu and Login
unsigned char u_member[MAX_SIZE_USER_TYPE] = {0};

//
/* - - - - GLOBAL VARIABLES : END - - - - - */
//


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
int isBookPresent(int id)
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
        fscanf(fp, "%u\t%[^\t]%*c%[^\t]%*c\t%d\n", &addBookInfo.books_id,addBookInfo.bookName,addBookInfo.authorName, &addBookInfo.book_stock);
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

// Check if Book is already issued
int isBookIssued(int book)
{
    unsigned int issued = 0;
    RegisterInfo Reg_info = {0};
    FILE *f_checkBookIssued_p = NULL;

    f_checkBookIssued_p = fopen(REGISTER_FILE_NAME, "r");
    if(f_checkBookIssued_p == NULL)
    {
        printf("\n\t\t\tFile is not opened\n");
        exit(1);
    }

    // check Library Register if the user has already borrowed the book previously
    while(!feof(f_checkBookIssued_p))
    {
        fscanf(f_checkBookIssued_p, "%d\t%[^\t]%*c\t%d/%d/%d\t%d/%d/%d\n", &Reg_info.books_id, Reg_info.username, &Reg_info.i_date.dd, &Reg_info.i_date.mm, &Reg_info.i_date.yyyy, &Reg_info.r_date.dd, &Reg_info.r_date.mm, &Reg_info.r_date.yyyy);
        
        //printf("book : %u\nu_id : %c\nIssue : %02d/%02d/%d\nReturn : %02d/%02d/%d\n", Reg_info.books_id, Reg_info.username, Reg_info.i_date.dd, Reg_info.i_date.mm, Reg_info.i_date.yyyy, Reg_info.r_date.dd, Reg_info.r_date.mm, Reg_info.r_date.yyyy);
        
        if((Reg_info.books_id == book) && (strcmp(Reg_info.username, u) == 0))
        {
            // if the user has previously borrowed the book, we will not issue it again.
            issued = 1;
            break;
        }
    }
    fclose(f_checkBookIssued_p);
    return issued;
}

// update book stock if the book is issued to a user
void book_Issued(int book)
{
    int found = 0;
    BookInfo addBookInfo = {0};

    FILE *fp = NULL; // original file
    FILE *tmpFp = NULL; // temporary file

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

    while(!feof(fp))
        {
            // we will take the necessary details and store them in some varibale
            // then we copy the data from original file to temporary file
            // if the book id in original file matches with the book id to be issued,
            // we reduce the stock by 1 as 1 book is being issued to a user,
            // then, we copy the data stored in the variable to the temporary file
            // after copying, we delete original file, and rename it.

            fscanf(fp, "%u\t%[^\t]%*c%[^\t]%*c\t%d\n", &addBookInfo.books_id,addBookInfo.bookName, addBookInfo.authorName, &addBookInfo.book_stock);

            if(addBookInfo.books_id == book)
            {
                --addBookInfo.book_stock;
                found = 1;
            }

            fprintf(tmpFp, "%u\t%s\t%s\t%d\n", addBookInfo.books_id, addBookInfo.bookName, addBookInfo.authorName, addBookInfo.book_stock);
        }
    if (found)
    {
        //printf("\n\t\t\tRecord updated successfully.....");
        fclose(fp);
        fclose(tmpFp);
        remove(BOOK_FILE_NAME);
        rename("tmp.txt", BOOK_FILE_NAME);
    } 

}

// printing Borrowed Books on the screen - part A
char* showBookName(int id)
{
    BookInfo searchBookInfo = {0};
    FILE *fp = NULL;
    char *name;

    fp = fopen(BOOK_FILE_NAME, "r");
    if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }

    while (!feof(fp))
    {
        fscanf(fp, "%u\t%[^\t]%*c\t%[^\t]%*c\t%d\n", &searchBookInfo.books_id, searchBookInfo.bookName, searchBookInfo.authorName, &searchBookInfo.book_stock);

        if(searchBookInfo.books_id == id)
        {
            fclose(fp);
            name = searchBookInfo.bookName;
            return name;
        }
    }
}

// printing Borrowed Books on the screen - part B
void showBorrowedBooks(int list[], int *count)
//void showBorrowedBooks()
{
    //int count = 0;
    FILE *fp = NULL;
    RegisterInfo Reg_info = {0};

    fp = fopen(REGISTER_FILE_NAME, "r");

    if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }
    while (!feof(fp))
    {
        fscanf(fp, "%d\t%[^\t]%*c\t%d/%d/%d\t%d/%d/%d\n", &Reg_info.books_id, Reg_info.username, &Reg_info.i_date.dd, &Reg_info.i_date.mm, &Reg_info.i_date.yyyy, &Reg_info.r_date.dd, &Reg_info.r_date.mm, &Reg_info.r_date.yyyy);
        
        if(!strcmp(Reg_info.username, u))
        {
            list[*count] = Reg_info.books_id;
            printf("\n\t\t\t%d.\t-\t%d\t-\t%s", (1+(*count)++),Reg_info.books_id, showBookName(Reg_info.books_id));
        }   
    }
    printf("\n\t\t\t----------------------------------------------------------------------\n");
    fclose(fp);
    
}

// update book stock if the book is returned by a user
void book_Returned(int book)
{
    int found = 0;
    BookInfo addBookInfo = {0};

    FILE *fp = NULL; // original file
    FILE *tmpFp = NULL; // temporary file

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

    while(!feof(fp))
        {
            // we will take the necessary details and store them in some varibale
            // then we copy the data from original file to temporary file
            // if the book id in original file matches with the book id to be RETURNED,
            // we INCREASE the stock by 1 as 1 book is being returned by a user,
            // then, we copy the data stored in the variable to the temporary file
            // after copying, we delete original file, and rename it.

            fscanf(fp, "%u\t%[^\t]%*c%[^\t]%*c\t%d\n", &addBookInfo.books_id,addBookInfo.bookName, addBookInfo.authorName, &addBookInfo.book_stock);

            if(addBookInfo.books_id == book)
            {
                ++addBookInfo.book_stock;
                found = 1;
            }

            fprintf(tmpFp, "%u\t%s\t%s\t%d\n", addBookInfo.books_id, addBookInfo.bookName, addBookInfo.authorName, addBookInfo.book_stock);
        }
    if (found)
    {
        //printf("\n\t\t\tRecord updated successfully.....");
        fclose(fp);
        fclose(tmpFp);
        remove(BOOK_FILE_NAME);
        rename("tmp.txt", BOOK_FILE_NAME);
    } 

}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//- - - - - - START : Some Date related Functions - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// EXTRA FUNTION ... NO NEED OF IT...
Date cal_date_diff(Date s, Date e)
{
    //printf("\nStart : %d, %d, %d", s.dd, s.mm, s.yyyy); // checking s
    //printf("\nEnd : %d, %d, %d", e.dd, e.mm, e.yyyy); // checking e

    Date diff = {0};

    if(e.dd < s.dd)
    {      
        // adding days from february
        if (e.mm == 3)
        {
            //  checking if the year is a leap year
            if ((e.yyyy % 4 == 0 && e.yyyy % 100 != 0) || (e.yyyy % 400 == 0)) 
            {
                e.dd += 29;
            }

            else
            {
                e.dd += 28;
            }                        
        }

        // adding days from April or June or September or November
        else if (e.mm == 5 || e.mm == 7 || e.mm == 10 || e.mm == 12) 
        {
            e.dd += 30; 
        }

        // adding days from Jan or Mar or May or July or Aug or Oct or Dec
        else
        {
            e.dd += 31;
        }

        e.mm = e.mm - 1;
    }

    if (e.mm < s.mm)
    {
        // adding months from previous year
        e.mm += 12;
        e.yyyy -= 1;
    }       

    // finding difference
    diff.dd = e.dd - s.dd;
    diff.mm = e.mm - s.mm;
    diff.yyyy = e.yyyy - s.yyyy;

    return diff;
}
 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

int maxDateOfMonth(int m, int y)
{
    int d_max = 31;

    if(m == 2)
    {
        // - - - - for February - - - -
        d_max = 28;

        if((y%4 == 0 || y%100 == 0) && y%400 != 0)
            d_max = 29;
    }
    else if(m == 4 || m == 6 || m == 9 || m == 11)
    {
        // - - - - for months with 30 days - - - -
        d_max = 30;
    }

    return d_max;
}

Date returning_date(Date start, int period)
{
    Date end;
    //printf("\nToday's date: %02i/%02i/%i\n", start.mm, start.dd, start.yyyy); // checking

    // initializing Returning dates
    end.dd = start.dd + period;
    end.mm = start.mm;
    end.yyyy = start.yyyy;

    int max_date = maxDateOfMonth(start.mm, start.yyyy);
    
    if(end.dd > max_date)
    {
        end.mm += 1;
        end.dd = end.dd - max_date;
    }

    if (end.mm > 12)
    {
        end.mm = 1;
        end.yyyy += 1;
    }
    //printf("\nReturn's date: %02i/%02i/%i\n", end.mm, end.dd, end.yyyy);
    
    return end;
}

Date today_date()
{
    Date start_date;
    time_t t;
    time(&t);
    struct tm *now = localtime(&t);

    // initializing current date
    start_date.mm = now->tm_mon + 1;
    start_date.dd = now->tm_mday;
    start_date.yyyy = now->tm_year + 1900;

    return start_date;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//- - - - - - - END : Some Date related Functions - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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
        fscanf(fp, "%u\t%[^\t]%*c\t%[^\t]%*c\t%d\n", &searchBookInfo.books_id, searchBookInfo.bookName, searchBookInfo.authorName, &searchBookInfo.book_stock);

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
        printf("\n\t\t\tIn Stock = %d", searchBookInfo.book_stock);

        if((searchBookInfo.book_stock<3) && (!(strcmp(u_member, "stud"))))
        {
            printf("\n\n\t\t\t-----------------------------------------");
            printf("\n\t\t\tThis book is not available for borrowing.");
            printf("\n\t\t\t-----------------------------------------");
        }
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
        fscanf(fp, "%u\t%[^\t]%*c\t%[^\t]%*c\t%d\n", &addBookInfo.books_id,addBookInfo.bookName,addBookInfo.authorName, &addBookInfo.book_stock);
        printf("\n\t\t\t%d.)", countBook);
        printf("\n\t\t\t\tBook id = %u", addBookInfo.books_id);
        printf("\n\t\t\t\tBook name = %s", addBookInfo.bookName);
        printf("\n\t\t\t\tBook authorName = %s", addBookInfo.authorName);
        printf("\n\t\t\t\tBooks available = %d\n", addBookInfo.book_stock);
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

// Borrow Book function
void borrowBook()
{
    int count = 0;
    int status = 0;
    int flag = 0;
    char choice = 'N';
    unsigned int bookid = 0;

    Date issue_date, return_date;
    int period = 7; // should be less than 28 or 29
    
    BookInfo bookInfo = {0};
    BookInfo bookReading ={0};
    FILE *f_reg_book = NULL;
    FILE *f_book = NULL; 

    f_reg_book = fopen(REGISTER_FILE_NAME, "a"); // opened file in append mode for adding new issues
    if(f_reg_book == NULL)
    {
        printf("File is not opened\n");
        scanf("%c", temp);
        exit(1);
    }

    do
    {
        if (count>3)
        {
            break;
        }
        headMessage("BORROW BOOKS");

        printf("\n\t\t\tBook ID NO  = ");
        fflush(stdin);
        scanf("%u", &bookid);

        // check if Book exist
        status = isBookPresent(bookid);

        // check if the user has already borrowed that same book before.
        // user can borrow ONLY ONE BOOK PER USER
        status = status + isBookIssued(bookid);

        if (status == 2)
        {
            printf("\n\t\t\tBook ID : %u is already issued to user : %s.", bookid, u);
            break; // if the user has borrowed it earlier, we simply print a message and go to main menu.
        }
        
        if(status)
        {
            f_book = fopen(BOOK_FILE_NAME,"r"); // opened file in read mode to read file content.
            if(f_book == NULL)
            {
                printf("\n\t\t\tFile is not opened\n");
                scanf("%c", temp);
                exit(1);
            }

            // we read the book details and print the details on screen
            // then confirm with the user if he wants to take that particular book or not
            // after confirming,
            // => we update the Library Register,
            // => produce the date for returning the book,
            // => update the book's database as 1 book is taken so book stock will be reduced by 1

            while(!feof(f_book))
            {
                // reading book database content.
                fscanf(f_book, "%u\t%[^\t]%*c\t%[^\t]%*c\t%d\n", &bookReading.books_id, bookReading.bookName, bookReading.authorName, &bookReading.book_stock);
                if(bookReading.books_id == bookid)
                {
                    printf("\n\n\t\t\tBook id = %u", bookReading.books_id);
                    printf("\n\t\t\tBook name = %s", bookReading.bookName);
                    printf("\n\t\t\tBook authorName = %s", bookReading.authorName);
                    printf("\n\t\t\tIn Stock = %d", bookReading.book_stock);
                    flag = 1;

                    // check if the book is less than 3 in quantity and if the user is a student,
                    // if True, the student user is not allowed to borrow the book. 
                    if((bookReading.book_stock<3) && (!(strcmp(u_member, "stud"))))
                    {
                        printf("\n\n\t\t\t-----------------------------------------");
                        printf("\n\t\t\tThis book is not available for borrowing.");
                        printf("\n\t\t\t-----------------------------------------");
                        flag = 0;
                    }
                    fclose(f_book);
                    break;
                }
            }
        }
        else
        {
            // if the entered book is not available in Library
            printf("\n\t\t\tBook not found. Please enter again.....");
            fflush(stdin);
            scanf("%c", &temp);
            count++;
        }
    } while (!status);
    
    if(flag)
    {
        printf("\n\t\t\t----------------------------------------------------------------------\n");
        printf("\n\t\t\tConfirm ? (Y/N) : ");
        fflush(stdin);
        scanf("%c", &choice);
        
        if (toupper(choice) == 'Y')
        {
            issue_date = today_date(); //initializing the issue date

            // if book is available in more number then returning date can be increased.
            if(bookReading.book_stock > 6)
                period = 2*period;
            else if(bookReading.book_stock > 12)
                period = 3*period;
                
            return_date = returning_date(issue_date, period);

            // writing the record in the Library Register.
            fprintf(f_reg_book, "\n%u\t%s\t%02i/%02i/%i\t%02i/%02i/%i", bookReading.books_id, u, issue_date.dd, issue_date.mm, issue_date.yyyy, return_date.dd, return_date.mm, return_date.yyyy);

            // Confirming that the book is issued to user
            printf("\n\t\t\tBook Issued successfully.....");
            printf("\n\n\t\t\tPlease note the returning date.....");
            printf("\n\t\t\tReturning Date\t:\t%02d/%02d/%d", return_date.dd, return_date.mm, return_date.yyyy);
            book_Issued(bookid);
        }

        else if (toupper(choice) == 'N')
        {
            headMessage("BORROW BOOKS");
            printf("\n\t\t\tBook Not Issued.");
        }
        else
        {
            // if values are not valid, nothing is added and we simply go out to Main Menu.
            headMessage("BORROW BOOKS");
            printf("\n\t\t\tTry Again.");
        }
    }

    fclose(f_reg_book);

    printf("\n\n\t\t\tPress any key to go to main menu.....");
    fflush(stdin);
    scanf("%c", &temp);
}

// Return Book function
void returnBook()
{
    int count = 0;
    int status = 0;
    int flag = 0;
    char choice = 'N';
    unsigned int bookid = 0;
    int borrowedbookslist[100];

    RegisterInfo Reg_info = {0};

    FILE *f_reg_book = NULL;
    FILE *tmpFp = NULL;

    headMessage("RETURN BOOKS");

    // we read the Library Register details 
    // and print the book taked by the user
    // Asks user which book he wants to return
    // then confirm with the user if he wants to return or not
    // after confirming,
    // => we update the Library Register,
    // => update the book's database as 1 book is returned so book stock will be increased by 1
    
    showBorrowedBooks(borrowedbookslist, &count);
    
    if(count)
    {
    do
    {
        if (flag>2)
        {
            break;
        }

        printf("\n\t\t\tEnter Book ID to return = ");
        fflush(stdin);
        scanf("%d", &bookid);

        // check if Book exist in the borrowed list.
        for(int i = 0 ; i<count; i++)
        {
            if(bookid == borrowedbookslist[i])
            {
                status = 1;
                break;
            }
        }
        if(!status)
        {
            printf("\n\t\t\tPlease enter correct book id....");
            fflush(stdin);
            scanf("%c", &temp);
            flag++;
        
        }
    }while(!status);
        
    if(status)
    {
        printf("\n\t\t\t----------------------------------------------------------------------\n");
        printf("\n\t\t\tConfirm Return ? (Y/N) : ");
        fflush(stdin);
        scanf("%c", &choice);

        if (toupper(choice) == 'Y')
        {
            f_reg_book = fopen(REGISTER_FILE_NAME, "r"); // opened file in append mode for adding new issues
            if(f_reg_book == NULL)
            {
                printf("File is not opened\n");
                scanf("%c", temp);
                exit(1);
            }

            tmpFp = fopen("tmp.txt", "w");
            if(tmpFp == NULL)
            {
                fclose(f_reg_book);
                printf("File is not opened\n");
                exit(1);
            }
            
            while(!feof(f_reg_book))
            {
                // reading the contents if Library register.
                fscanf(f_reg_book, "%d\t%[^\t]%*c\t%d/%d/%d\t%d/%d/%d\n", &Reg_info.books_id, Reg_info.username, &Reg_info.i_date.dd, &Reg_info.i_date.mm, &Reg_info.i_date.yyyy, &Reg_info.r_date.dd, &Reg_info.r_date.mm, &Reg_info.r_date.yyyy);

                if (bookid == Reg_info.books_id && (strcmp(u, Reg_info.username)==0))
                {
                    // if the username and book id matches, we skip that record
                    continue;
                }
                else
                {
                    fprintf(tmpFp, "%u\t%s\t%02i/%02i/%i\t%02i/%02i/%i\n", Reg_info.books_id, Reg_info.username, Reg_info.i_date.dd, Reg_info.i_date.mm, Reg_info.i_date.yyyy, Reg_info.r_date.dd, Reg_info.r_date.mm, Reg_info.r_date.yyyy);
                }
            } 
            fclose(f_reg_book);
            fclose(tmpFp);
            remove(REGISTER_FILE_NAME);
            rename("tmp.txt", REGISTER_FILE_NAME);

            // after returning a book, the book stock must be increased by 1.
            book_Returned(bookid);

            printf("\n\n\t\t\tBook Returned Successfully.");
        }
        else if (toupper(choice) == 'N')
        {
            headMessage("RETURN BOOKS");
            printf("\n\t\t\tBook Not Returned.");
        }
        else
        {
            // if values are not valid, nothing is added and we simply go out to Main Menu.
            headMessage("BORROW BOOKS");
            printf("\n\t\t\tTry Again.");
        }
    }

    }
    else
    {
        // if the count = 0, means there are no books borrowed by the user.
        printf("\n\t\t\tYou don't have any borrowed book");
    }

    printf("\n\n\t\t\tPress any key to go to main menu.....");
    fflush(stdin);
    scanf("%c", &temp);
}

// Delete Book function
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
        fscanf(fp, "%u\t%[^\t]%*c%[^\t]%*c\t%d\n", &delBookInfo.books_id, delBookInfo.bookName, delBookInfo.authorName, &delBookInfo.book_stock);
        
        if(delBookInfo.books_id != bookDelete)
        {
            fprintf(tmpFp, "%u\t%s\t%s\t%d\n", delBookInfo.books_id, delBookInfo.bookName, delBookInfo.authorName, delBookInfo.book_stock);
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

// Add book funtion
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
            printf("\n\t\t\tEnter number of books in stock : ");
            fflush(stdin);
            scanf("%d", &addBookInfo.book_stock);
            count = 0;
        }
    }
    while(!status);
    
    if (count<3)
    {
        // count < 3 means that we have entered all values correctly and are valid.
        headerAddBookPage();
        fprintf(fp, "%u\t%s\t%s\t%d", addBookInfo.books_id, addBookInfo.bookName, addBookInfo.authorName, addBookInfo.book_stock);
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

// Update book information function
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
        // if book exists then we perform following functions.
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
                printf("\n\t\t\tEnter number of books in stock : ");
                fflush(stdin);
                scanf("%d", &updateBookInfo.book_stock);
                count = 0;
            }
        }while(!status);

        while(!feof(fp))
        {
            fscanf(fp, "%u\t%[^\t]%*c%[^\t]%*c\t%d\n", &addBookInfo.books_id,addBookInfo.bookName,addBookInfo.authorName, &addBookInfo.book_stock);

            if(addBookInfo.books_id != updateBookInfo.books_id)
            {
                fprintf(tmpFp, "%u\t%s\t%s\t%d\n", addBookInfo.books_id, addBookInfo.bookName, addBookInfo.authorName, addBookInfo.book_stock);
            }
            else
            {
                fprintf(tmpFp, "%u\t%s\t%s\t%d\n", updateBookInfo.books_id, updateBookInfo.bookName, updateBookInfo.authorName, updateBookInfo.book_stock);
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
            borrowBook();
            break;
        case 4:
            printf("returnbooks");
            returnBook();
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
            fscanf(fp, "%s %s %s %s", login_info.username, login_info.password, login_info.type, login_info.member);

            if((!strcmp(userName,login_info.username)) && (!strcmp(password, login_info.password)))
            {
                headMessage("LOGIN");
                printf("Login Success");
                strcpy(u, login_info.username);
                strcpy(u_type, login_info.type);
                strcpy(u_member, login_info.member);
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