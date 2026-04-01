#include <stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct{
    int day, month, year;
    char note[100];
    int is_recurring; //0 for non recurring & 1 for recurring
}events;


int firstday(int month, int year)
{
    int d = 1;
    if(month<3)
    {
        month+=12;
        year--;
    }
    int k = year%100;
    int j = year/100;
    int result = (d + (13 * (month + 1)) / 5 + k +(k/4) + (j/4) +(5*j)) % 7;
    
    return result;
}

void event_marker(int day, int month, int year)
{
    FILE *fp = fopen("events.dat", "rb");
    if(!fp) return;

    events e;
    int flag =0 ;
    while(fread(&e, sizeof(events), 1, fp))
    {
        if(e.day==day && e.month==month && (e.year==year || e.is_recurring==1))
        {
            printf("*");
            flag = 1;
            fclose(fp);
            return;
        }
    }
    fclose(fp);
}

void amarcalender(int month, int year)
{
    char* Month[] = {"January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"};
    
    int totaldays_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    printf("\n\t %s, %d\t\n", Month[month-1], year);
    printf(" SAT\t SUN\t MON\t TUE\t WED\t THU\t FRI\n");
    for(int i = 0 ; i < firstday(month, year) ; i++)
        printf("\t");
    
    if((year%100!=0 &&year%4==0)||(year%400==0))
        totaldays_month[1]+=1;
        
    for(int i = 1; i <= totaldays_month[month-1]; i++)
    {
        printf("%4d",i);
        event_marker(i, month, year);
        printf("\t");
        if((i + firstday(month, year)) % 7 == 0){
            printf("\n");
        }
    }
    
    printf("\n");
}

void allyear_cal(int year)
{
    for(int i = 1; i<=12; i++)
    {
        amarcalender(i, year);

        printf("\n-----------------------------------------------------\n");
    }
}

void see_event()
{
    int d, m, y;
    char c[20];
    printf("\nEnter the proper date to view event: (dd-mm-yy)");
    scanf("%s",c);

    sscanf(c,"%02d-%02d-%d", &d, &m, &y );
    while(getchar() != '\n');

    events e;

    FILE *fp = fopen("events.dat", "rb");
    if(!fp)
    {
        printf("No Events to be found :( !!!");
        return;
    }
    int flag = 0;
    while(fread(&e, sizeof(events), 1, fp))
    {
        if(e.day==d && e.month==m && (e.year==y || e.is_recurring==1))
        {
            printf("An event is there on %02d-%02d-%d: %s\n", e.day, e.month, e.year, e.note);
            flag = 1;
        }
    }

    fclose(fp);

    if(flag==0)
    {
        printf("No Events Found on this date\n");
    }
}

void make_event()
{
    events e;
    char input[20];
    printf("\nEnter the date details for the event (dd-mm-yy): ");
    scanf("%s", input);

    while (getchar() != '\n'); //clearing buffer

    sscanf(input,"%02d-%02d-%d", &e.day, &e.month, &e.year);
    printf("\nEnter the details: ");

    fgets(e.note, sizeof(e.note), stdin);
    e.note[strcspn(e.note, "\n")]=0;

    printf("Should this event repeat every year? (1 = Yes, 0 = No): ");
    scanf("%d", &e.is_recurring);
    while (getchar() != '\n');

    FILE *fp = fopen("events.dat", "ab+");
    if(!fp)
    {
        printf("Error opening file!\n");
        return;
    }

    fwrite(&e, sizeof(events), 1, fp);
    fclose(fp);
    getchar();
    printf("Event is succesfully added ! :) :) : )");
    
}

int main() 
{
    int year, month;
    char choice;
    while(1)
    {
        
        printf("\n Calender Menu: \n");
        printf("\nEnter A to view calender");
        printf("\nEnter B to view  Event");
        printf("\nEnter C to Add event");
        printf("\nEnter D to view whole calender for a year");
        printf("\nEnter X to Exit");
        printf("\nEnter choice: ");
        scanf("%c", &choice);

        switch(choice)
        {
            
            case 'A':
                printf("Enter the year: ");
                scanf("%5d", &year);
                printf("Enter the month: ");
                scanf("%3d", &month);
            
                if(month < 1 || month > 12 || year < 1){
                    printf("Invalid input :)");
                }
                else{
                    amarcalender(month, year);
                }
                
                break;
            case 'B':
                see_event();
                break;
            case 'C':
                make_event();
                break;
            case 'D':
                printf("Enter the year: ");
                scanf("%5d", &year);
                
                if(year < 1){
                    printf("Invalid !!");
                }
                else{
                    allyear_cal(year);
                }
                break;
            case 'X':
                printf("\nexiting program...\n");
                return 0;
            default:
                printf("\nInvalid choice\n");
            
            }
            getchar(); 
    }
    
    
}