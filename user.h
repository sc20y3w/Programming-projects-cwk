#ifndef __USERS__H__
#define __USERS__H__
#define MAX_PEOPLE 1000

struct user
{
    char account[20];
    char passwd[20];
    int bk[10];
    int bn1;
};
struct users
{
    struct user us[MAX_PEOPLE];
    int pu;
};

#endif