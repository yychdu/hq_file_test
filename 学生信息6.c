#define _CRT_SECURE_NO_WARNINGS 1
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"

typedef struct 
{
    int stunum;
    char name[10];
    double score;//浮点数比较 fabs(a-b)<=1E-14
} student;

int mycomp(const student *a,const student *b)
{
    if (fabs(a->score - b->score) <= 1E-14)
        return (a->stunum < b->stunum);
    else 
        return (a->score > b->score);
}


void sort(student *a, int left, int right)
{
    int i,j;
    student key;
    while (left < right)
    {
        i = left;
        j = right;
        key = a[left];
        while (i < j)                               
        {
            while (i < j && mycomp(&key, a+j))
                 j--;
            if (i == j)
                break;             
            a[i] = a[j];
            while (i < j && mycomp(a+i, &key))//反过来
                i++;
            if (i != j)         
                a[j] = a[i];
        }  
        a[i] = key;
        sort(a, left, i - 1);
        // sort(a, i + 1, right);
        left = i + 1; //循环 减少压栈
    }    
}


int InputStudents(const char *bin_name)
{
    int result = -1;
    int num = 0;
    student temp;
    FILE *fp = fopen(bin_name, "wb+");
    if (fp == NULL)
        printf("open file err"); 
    else
    {
        while (1)//参数简化
        {      
            printf("input student data:\n");
            if (scanf("%d%9s%lf", &temp.stunum, temp.name, &temp.score) == 3
                    && fwrite(&temp, sizeof(student), 1, fp) == 1)//并到一起
                num++;
            else
                break;
        }
        result = num;
        fclose(fp);
    }  
    return result;
}

int OutputStudents(const char *bin_name, const char *txt_name)
{
    double sum = 0;
    int i,num,result = -1;
    FILE *fp = fopen(bin_name, "rb");//垃圾文件  单出口化
    FILE *fp2;
    if (fp == NULL)
         printf("open file bin err");
    else
    {
        fp2 = fopen(txt_name, "w+");
        if (fp2 == NULL)
            printf("open file txt err");
        else
        {
            fseek(fp, 0, SEEK_END);
            num = ftell(fp) / sizeof(student);
            if(num == 0)
                result = 0;
            else
            {
                rewind(fp);
                student *stud = (student *)malloc(num * sizeof(student));//分配失败  直接跳出
                if (stud == NULL)
                    printf("malloc err"); 
                else
                {
                    if (fread(stud, sizeof(student), num, fp) != num)//推进
                        printf("read err");
                    else 
                    {
                        sort(stud, 0, num - 1);
                        fprintf(fp2, "%8s\t%9s\t%8s\n", "stunum", "name", "score");
                        for (i = 0; i < num;i++)
                        {
                            sum += stud[i].score;
                            fprintf(fp2,"%8d\t%9s\t%8.1f\n", stud[i].stunum, stud[i].name, stud[i].score);
                        }
                        printf("avr=%.1lf\n", sum / num);
                        result = num;
                    }
                    free(stud);
                }
            }            
            fclose(fp2);              
        }
        fclose(fp);
    }
    return result;
}

int main() {
    char filename[4]="stu",txt_name[4]="txt";
    InputStudents(filename);
    OutputStudents(filename, txt_name);
    return 0;
}


