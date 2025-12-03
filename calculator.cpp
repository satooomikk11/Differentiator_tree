#include <stdio.h>
#include <math.h>

const char* s = "10*(30+20*10)+13$";

int GetN();
int GetP();
int GetT();
int GetE();
int GetG();

int GetN()
{
    int val = 0;
    while ('0' <= *s && *s <= '9')
    {
        val = val * 10 + (*s - '0');
        s++;
    }
    return val;
}

int GetP()
{
    if (*s == '(')
    {
        s++;
        int val = GetE();
        s++;
        return val;
    }
    else
    {
        return GetN();
    }
}

int GetT()
{
    int val = GetP();
    while(*s == '*' || *s == '/')
    {
        char op = *s;
        s++;
        int val2 = GetP();
        if (op == '*')
        {
            val *= val2;
        }
        else
        {
            val /= val2;
        }
    }
    return val;
}

int GetE()
{
    int val = GetT();
    while(*s == '+' || *s == '-')
    {
        char op = *s;
        s++;
        int val2 = GetT();
        if (op == '+')
        {
            val += val2;
        }
        else
        {
            val -= val2;
        }
    }
    return val;
}

int GetG()
{
    int val = GetE();
    if (*s != '$')
    {
        s++;
        return -1; //поменять на SyntaxError
    }
    printf("%d\n", val);
    return val;
}

int main()
{
    GetG();
    return 0;
}