#include <stdio.h>
#include <time.h>

/*************************************************
 * 
 * This program is based on the assumption 
 * that the returning period is less than 1 month.
 * 
 **************************************************/

typedef struct
{
    int yyyy;
    int mm;
    int dd;
} Date;
 
Date cal_date_diff(Date s, Date e)
{
    printf("\nStart : %d, %d, %d", s.dd, s.mm, s.yyyy); // checking s
    printf("\nEnd : %d, %d, %d", e.dd, e.mm, e.yyyy); // checking e

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

    printf("\nToday's date: %02i/%02i/%i\n", start.mm, start.dd, start.yyyy); // checking

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
    printf("\nReturn's date: %02i/%02i/%i\n", end.mm, end.dd, end.yyyy);
    
    return end;
}


int main()
{
    Date start_date;
    int issue_period = 7; // should be less than 28 or 29
    time_t t;
    time(&t);
    struct tm *now = localtime(&t);

    // initializing current date
    start_date.mm = now->tm_mon + 1;
    start_date.dd = now->tm_mday;
    start_date.yyyy = now->tm_year + 1900;

    //start_date.yyyy = 2001; // inputting custom year for testing
    //start_date.mm = 1; // inputting custom month for testing
    //start_date.dd = 30; // inputting custom date for testing

    Date end_date = returning_date(start_date, issue_period);    
    Date d_date = {0};

    d_date = cal_date_diff(start_date, end_date);
    printf("\nDiffer : %d , %d , %d", d_date.dd, d_date.mm, d_date.yyyy);
    
    /*
    * If differnce > returning period, then the one is subject to penalty.
    */

    return 0;
}