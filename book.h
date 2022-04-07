#ifndef __BOOKS__H__
#define __BOOKS__H__
typedef struct book
{
    int id;
    char *title;
    char *author;
    int year;
    int copies;
} book;
typedef struct book_node
{
    int top;
    struct book book;
    struct book_node *next;
} book_node;

book_node *init_book_list();

#endif