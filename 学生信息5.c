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
} stu;

int mycomp(stu a, stu b)
{
    if (fabs(a.score-b.score) <= 1E-14 )
        return  (a.stunum < b.stunum);
    else 
        return (a.score > b.score);

}


void sort(stu *a, int left, int right)
{
    if (left >= right)
        return ;
    int i = left;
    int j = right;
    stu key = a[left];
     
    while (i < j)                               
    {
        // while(i < j &&  ((fabs(key.score-a[j].score) <= 1E-14 && key.stunum < a[j].stunum)
        //  || (key.score > a[j].score && fabs(key.score-a[j].score) > 1E-14)  ) ) //函数判断
        while (i < j && mycomp(key,a[j]))
            j--;
        a[i] = a[j];
        // while(i < j && (( fabs(key.score-a[i].score) <= 1E-14 && key.stunum > a[i].stunum)
        // || (key.score < a[i].score && fabs(key.score-a[i].score) > 1E-14 ) ))
         while (i < j && !mycomp(key,a[i]))//反过来
            i++;         
         a[j] = a[i];
    }  
    a[i] = key;
    sort(a, left, i - 1);
    sort(a, i + 1, right);    
    //left = i+1 循环                   
}


int InputStudents(const char *bin_name)
{
    int num = 0;
    stu temp;
    FILE *fp = fopen(bin_name, "wb+");
    if (fp == NULL)
    {
        printf("open file err");
        return -1;
    }  
    while (1)//参数简化
    {      
        printf("input student data:\n");
        if (scanf("%d%9s%lf", &temp.stunum, temp.name, &temp.score) != 3)//并到一起
            break;
        else if (fwrite(&temp, sizeof(stu), 1, fp) != 1)
            break;
        else num++;
    }
    fclose(fp);
    return num;
}

int OutputStudents(const char *bin_name, const char *txt_name)
{
    double sum = 0;
    int i,num;
    FILE *fp = fopen(bin_name, "rb");//垃圾文件  单出口化
    FILE *fp2;
    if(fp == NULL)
    {
         printf("open file bin err");
         return -1;
    }
    else
    {
        fp2 = fopen(txt_name, "w+");
        if(fp2 == NULL)
        {
            fclose(fp);
            printf("open file txt err");
            return -1;
        }

    }

    fseek(fp, 0, SEEK_END);
    num = ftell(fp) / sizeof(stu);
    rewind(fp);
    stu *stud = (stu *)malloc(num * sizeof(stu));//分配失败  直接跳出
    if (stud)
    {
        if (fread(stud, sizeof(stu), num, fp) == num)//推进
        {
            sort(stud, 0, num - 1);
            fprintf(fp2, "%8s\t%9s\t%8s\n", "stunum", "name", "score");
            for ( i = 0; i < num;i++)
            {
                sum += stud[i].score;
                fprintf(fp2,"%8d\t%9s\t%8.1f\n", stud[i].stunum, stud[i].name, stud[i].score);
            }
            printf("avr=%.1lf\n", sum / num);
        }
        else
            printf("read err");
        free(stud);             
    }
    else
        printf("malloc err");
    fclose(fp);
    fclose(fp2);
    
    return num;
}

int main() {
    char filename[4]="stu",txt_name[4]="txt";
    InputStudents(filename);
    OutputStudents(filename, txt_name);
    return 0;
}


