#include "book.h"
#include <stdio.h>
#include <stdlib.h>

book_node *init_book_list()
{
    struct book_node *head;
    head = (struct book_node *)malloc(sizeof(struct book_node));
    head->next = NULL;
    return head;
}