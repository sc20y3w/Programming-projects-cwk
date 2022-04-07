#include "interface.h"

#include "book.h"
#include "user.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

book_node *books;
struct users us;
struct users *users = &us;
char current_user[100];
char root_name[100] = "librarian";   // librarian
char root_passwd[100] = "librarian"; // librarian

void display_all_books()
{
    book_node *p = books->next;
    printf("ID\tTitle\tAuthor\tyear\tcopies\n");
    while (p != NULL)
    {
        printf("%d \t%s \t%s \t%d \t%d\n", p->book.id, p->book.title, p->book.author, p->book.year, p->book.copies);
        p = p->next;
    }
}

void read_users()
{
    int i;
    FILE *fp;
    // read users.txt
    fp = fopen("../users.txt", "r");
    if (fp == NULL)
    {
        exit(0);
    }
    users->pu = -1;
    for (i = 0; !feof(fp); i++)
    {
        fscanf(fp, "%s %s %d", users->us[i].account, users->us[i].passwd, &users->us[i].bn1);
        for (int j = 0; j < users->us[i].bn1; j++)
        {
            fscanf(fp, "%d", &users->us[i].bk[j]);
        }
    }
    users->pu = i - 1;
    // show users.txt
    // for (int i = 0; i <= users->top; i++)
    // {
    //     printf("%s %d\n", users->us[i].account, users->us[i].bn1);
    //     for (int j = 0; j < users->us[i].bn1; j++)
    //     {
    //         printf("%d ", users->us[i].bk[j]);
    //     }
    //     putchar('\n');
    // }
    fclose(fp);
}
void read_books()
{
    int count = -1;
    int j = 0, k = 1;
    char c;
    char tmp[100];
    FILE *fp;
    // read books.txt
    books = init_book_list();
    fp = fopen("../books.txt", "r");
    if (fp == NULL)
    {
        exit(0);
    }
    book_node *p, *pre = books;
    memset(tmp, '\0', 100);
    while (!feof(fp))
    {
        p = (book_node *)malloc(sizeof(book_node));
        do
        {
            c = fgetc(fp);
            if (c != '|' && c != '\n' && !feof(fp))
            {
                tmp[j] = c;
                j++;
            }
            else if (c == '|' || c == '\n' || feof(fp))
            {
                if (k == 5)
                {
                    char copies[10];
                    strcpy(copies, tmp);
                    p->book.copies = atoi(copies);
                }
                else if (k == 4)
                {
                    char year[10];
                    strcpy(year, tmp);
                    p->book.year = atoi(year);
                }
                else if (k == 3)
                {
                    p->book.author = (char *)malloc(sizeof(char) * strlen(tmp));
                    strcpy(p->book.author, tmp);
                }
                else if (k == 2)
                {
                    p->book.title = (char *)malloc(sizeof(char) * strlen(tmp));
                    strcpy(p->book.title, tmp);
                }
                else if (k == 1)
                {
                    char id[10];
                    strcpy(id, tmp);
                    p->book.id = atoi(id);
                }
                j = 0;
                k++;
                memset(tmp, '\0', 100);
            }
        } while (c != '\n' && !feof(fp));
        k = 1;
        p->next = NULL;
        pre->next = p;
        pre = p;
        count++;
    }
    books->top = count;

    // display_all_books();

    fclose(fp);
}

void read_file()
{
    read_users();
    read_books();
}
void write_file()
{
    FILE *f;
    // write users to users.txt
    f = fopen("../users.txt", "w+");
    int q;
    for (q = 0; q < users->pu; q++)
    {
        fprintf(f, "%s %s %d\n", users->us[q].account, users->us[q].passwd, users->us[q].bn1);
        for (int j = 0; j < users->us[q].bn1; j++)
        {
            fprintf(f, "%d\n", users->us[q].bk[j]);
        }
    }
    fprintf(f, "%s %s %d\n", users->us[q].account, users->us[q].passwd, users->us[q].bn1);
    int j;
    for (j = 0; j < users->us[q].bn1 - 1; j++)
    {
        fprintf(f, "%d\n", users->us[q].bk[j]);
    }
    fprintf(f, "%d", users->us[q].bk[j]);
    fclose(f);
    // write books to books.txt
    f = fopen("../books.txt", "w+");
    book_node *p = books->next;
    for (int i = 0; i < books->top; i++)
    {
        fprintf(f, "%d|%s|%s|%d|%d\n", p->book.id, p->book.title, p->book.author, p->book.year, p->book.copies);
        p = p->next;
    }
    fprintf(f, "%d|%s|%s|%d|%d", p->book.id, p->book.title, p->book.author, p->book.year, p->book.copies);
    fclose(f);
}
/*error*/
void register_account()
{
    int i;
    char passwd[100];
    char username[100];

    printf("Please enter a username: ");
    scanf("%s", username);
    printf("Please enter a password: ");
    scanf("%s", passwd);
    for (i = 0; i <= users->pu; i++)
    {
        if (strcmp(username, users->us[i].account) == 0 || strcmp(username, root_name) == 0)
        {
            printf("Sorry,registration unsuccessful,the username you entered already exists.\n");
            break;
        }
    }
    if (i > users->pu)
    {
        users->pu++;
        strcpy(users->us[users->pu].account, username);
        strcpy(users->us[users->pu].passwd, passwd);
        users->us[users->pu].bn1 = 0;
        printf("successful\n");
    }
}
void search_all_books()
{
    int choice;
    while (1)
    {

        printf("Please choose an option:\n");
        printf("1) Find books by title\n");
        printf("2) Find books by author\n");
        printf("3) Find books by year\n");
        printf("4) Return to previous menu\n Option: ");
        scanf("%d", &choice);
        getchar();
        book_node *p = books;
        if (choice == 1)
        {
            char title[100];
            printf("Please enter title: ");
            gets(title);
            printf("ID\tTitle\tAuthor\tyear\tcopies\n");
            while (p->next != NULL)
            {
                p = p->next;
                if (p != NULL && strcmp(p->book.title, title) == 0)
                {
                    printf("%d \t%s \t%s \t%d \t%d\n", p->book.id, p->book.title, p->book.author, p->book.year, p->book.copies);
                }
            }
        }
        else if (choice == 3)
        {
            int year;
            printf("Please enter year: ");
            scanf("%d", &year);
            getchar();
            printf("ID\tTitle\tAuthor\tyear\tcopies\n");
            while (p->next != NULL)
            {
                p = p->next;
                if (p != NULL && p->book.year == year)
                {
                    printf("%d \t%s \t%s \t%d \t%d\n", p->book.id, p->book.title, p->book.author, p->book.year, p->book.copies);
                }
            }
        }
        else if (choice == 2)
        {
            printf("Please enter author: ");
            char author[100];
            printf("Please enter author: ");
            gets(author);
            printf("ID\tTitle\tAuthor\tyear\tcopies\n");
            while (p->next != NULL)
            {
                p = p->next;
                if (p != NULL && strcmp(p->book.author, author) == 0)
                {
                    printf("%d \t%s \t%s \t%d \t%d\n", p->book.id, p->book.title, p->book.author, p->book.year, p->book.copies);
                }
            }
        }
        else if (choice == 4)
        {
            break;
        }
        else
        {
        }
    }
}

void add_book()
{
    book_node *p, *pre = books;
    p = (book_node *)malloc(sizeof(book_node));
    char tmp[100];
    printf("Enter the title of the book you want to add: ");
    memset(tmp, '\0', 100);
    gets(tmp);
    p->book.title = (char *)malloc(sizeof(strlen(tmp)));
    strcpy(p->book.title, tmp);
    printf("Enter the author of the book you want to add: ");
    memset(tmp, '\0', 100);
    gets(tmp);
    p->book.author = (char *)malloc(sizeof(strlen(tmp)));
    strcpy(p->book.author, tmp);
    printf("Enter the year that the book you want to add was release: ");
    scanf("%d", &p->book.year);
    getchar();
    printf("Enter the number of copies of the book that you want to add: ");
    scanf("%d", &p->book.copies);
    getchar();
    books->top++;

    p->book.id = books->top + 1;

    while (pre->next != NULL)
    {
        pre = pre->next;
    }
    pre->next = p;
    p->next = NULL;
    printf("Add successfully\n");
    // printf("Sorry,you attempted to add an invalid book,please try again.\n");
}
/*error*/
void remove_book()
{

    int id;
    printf("Enter the ID of the book you want to remove: ");
    scanf("%d", &id);
    getchar();
    book_node *p = books->next, *pre = books;
    while (p != NULL)
    {
        if (p->book.id != id)
        {
            pre = p;
            p = p->next;
        }
        else
        {
            pre->next = p->next;
            free(p);
            // p = pre->next;
            books->top--;
            break;
        }
    }

    printf("Remove successfully\n");
}
void root_manage()
{
    int choice;
    do
    {
        printf("\n(logged in as : %s)", root_name);
        printf("\nPlease choose an option:\n");
        printf("1) Add a book\n");
        printf("2) Remove a book\n");
        printf("3) Search for books\n");
        printf("4) Display all books\n");
        printf("5) Log out\n Option: ");
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
        case 1:
            add_book();
            break;
        case 2:
            remove_book();
            break;
        case 3:
            search_all_books();
            break;
        case 4:
            display_all_books();
            break;
        case 5:
            printf("goodbye");
            break;
        default:
            getchar();
            printf("Sorry, the option you entered was invalid, please try again.\n");
        }
    } while (choice != 5);
}
void borrow_book()
{
    int id;
    printf("Blew is the list of Books you can borrow:\n");
    display_all_books();
    printf("Enter the ID of the book you want to borrow: ");
    scanf("%d", &id);
    getchar();
    int i;
    for (i = 0; i <= users->pu; i++)
    {
        if (strcmp(current_user, users->us[i].account) == 0 && id > 0)
        {
            users->us[i].bk[users->us[i].bn1] = id;
            users->us[i].bn1++;
            break;
        }
    }
    if (i > users->pu)
    {
        printf("The book not be borrowed.\n");
    }
    else
    {
        printf("Borrowed book successfully\n");
    }
}
void return_book()
{
    int id;
    int i, count = 0;
    printf("Blew is the list of Books you are currently borrowing:\n");
    printf("ID\tTitle\tAuthor\tyear\tcopies\n");

    for (int i = 0; i <= users->pu; i++)
    {
        if (strcmp(users->us[i].account, current_user) == 0)
        {
            for (int j = 0; j < users->us[i].bn1; j++)
            {
                book_node *p = books->next;
                while (p != NULL)
                {
                    if (p->book.id == users->us[i].bk[j])
                    {
                        printf("%d \t%s \t%s \t%d \t%d\n", p->book.id, p->book.title, p->book.author, p->book.year, p->book.copies);
                    }
                    p = p->next;
                }
            }
        }
    }

    printf("Enter the ID of the book you want to return: ");
    scanf("%d", &id);
    getchar();
    for (i = 0; i <= users->pu; i++)
    {
        if (strcmp(current_user, users->us[i].account) == 0 && id < books->top + 1)
        {
            for (int j = 0; j < users->us[i].bn1; j++)
            {
                if (users->us[i].bk[j] == id)
                {
                    for (int k = j; k < users->us[i].bn1 - 1; k++)
                    {
                        users->us[i].bk[k] = users->us[i].bk[k + 1];
                    }
                    break;
                }
                count++;
            }
            users->us[i].bn1--;
            break;
        }
    }
    if (count > users->us[i].bn1)
    {
        printf("The book not be returned.\n");
    }
    else
    {
        printf("Returned book successfully\n");
    }
}
void user_manage()
{
    int choice;
    do
    {
        printf("\n(logged in as : %s)", current_user);
        printf("\nPlease choose an option:\n");
        printf("1) Borrow a book\n");
        printf("2) Return a book\n");
        printf("3) Search for books\n");
        printf("4) Display all books\n");
        printf("5) Log out\n Option: ");
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
        case 1:
            borrow_book();
            break;
        case 2:
            return_book();
            break;
        case 3:
            search_all_books();
            break;
        case 4:
            display_all_books();
            break;
        case 5:
            printf("goodbye");
            break;
        default:
            getchar();
            printf("Sorry, the option you entered was invalid, please try again.\n");
        }
    } while (choice != 5);
}
void login_account()
{
    char passwd[100];
    char username[100];
    printf("Please enter a username: ");
    scanf("%s", username);
    printf("Please enter a password: ");
    scanf("%s", passwd);
    strcpy(current_user, username);
    if (strcmp(username, root_name) == 0)
    {
        if (strcmp(passwd, root_passwd) == 0)
        {
            root_manage();
            return;
        }
    }
    else
    {
        for (int i = 0; i <= users->pu; i++)
        {
            if (strcmp(username, users->us[i].account) == 0 && strcmp(passwd, users->us[i].passwd) == 0)
            {
                user_manage();
                return;
            }
        }
    }
    printf("login error\n");
}

void main_menu()
{
    int choice = 5;
    do
    {
        printf("\nPlease choose an option:\n");
        printf("1) Register an account\n");
        printf("2) Login\n");
        printf("3) Search for books\n");
        printf("4) Display all books\n");
        printf("5) Quit\n Option: ");
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
        case 1:
            register_account();
            break;
        case 2:
            login_account();
            break;
        case 3:
            search_all_books();
            break;
        case 4:
            display_all_books();
            break;
        case 5:
            printf("goodbye");
            break;
        default:
            getchar();
            printf("Sorry, the option you entered was invalid, please try again.\n");
        }
    } while (choice != 5);

    return;
}

void run_interface()
{
    read_file();
    main_menu();
    write_file();
    return;
}