#define _CRT_SECURE_NO_WARNINGS 1
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
//if 后面要加空格 参数前面加空格
//有返回值的都要检测 fread fwrite;
//能不能用goto
//字符串内存越界
//排序问题：成绩相同再来一遍？
//括号后面不加空格 双目运算符两边加空格
typedef struct 
{
    int stunum;
    char name[10];
    double score;//浮点数比较 fabs(a-b)<=1E-14
} stu;

void sort(stu *a, int left, int right)
{
    if(left >= right)
        return ;
    int i = left;
    int j = right;
    stu key = a[left];
     
    while(i < j)                               
    {
        while(i < j && key.score > a[j].score && fabs(key.score-a[j].score) >= 1E-14) //函数判断
            j--;
        while(i < j && fabs(key.score-a[j].score) <= 1E-14 && key.stunum < a[j].stunum)
            j--;
        a[i] = a[j];
        while(i < j && key.score < a[i].score && fabs(key.score-a[i].score) >= 1E-14)
            i++; 
        while(i < j && fabs(key.score-a[i].score) <= 1E-14 && key.stunum > a[i].stunum)
            i++;
         a[j] = a[i];
    }  
    a[i] = key;
    sort(a, left, i - 1);
    sort(a, i + 1, right);                       
}


int InputStudents( const char *bin_name)
{
    int num = 0, k = 0;
    FILE *fp ;
    if ((fp = fopen(bin_name, "wb+")) == NULL)//改
    {
        printf("open file err");
        return -1;
    }
    
    while(1)//参数简化
    {
        stu temp;
        printf("input student data:\n");
        if(scanf("%d%19s%lf", &temp.stunum, temp.name, &temp.score) != 3)
            break;
        else if (fwrite(&temp, sizeof(stu), 1, fp) != 1)
            break;
        else num++;
    }

    fclose(fp);
    rewind(stdin);
    return num;
}

int OutputStudents(const char *bin_name, const char *txt_name)
{
    double sum = 0;
    int i,num;
    FILE *fp = fopen(bin_name, "rb");//垃圾文件
    FILE *fp2 = fopen(txt_name, "w+");

    if (fp == NULL && fp2 != NULL) //fp2打开没有关闭
    {
         fclose(fp2);
         printf("open file bin err");
         return -1;
    }
    if (fp2 == NULL && fp != NULL )//fp打开没有关闭
    {
        fclose(fp);
        printf("open file txt err");
        return -1;
    }
    if (fp == NULL && fp2 == NULL)
    {
         printf("open file err");
        return -1;
    }

    fseek(fp, 0, SEEK_END);
  
    if ((num = ftell(fp) / sizeof(stu)) <= 0)
    {
        fclose(fp);
        fclose(fp2);
        printf("num ERR");
        return -1;
    }
    rewind(fp);
    // printf("student num is:%d\n", num);
    stu *stud = (stu *)malloc(num * sizeof(stu));//分配失败
    if(fread(stud, sizeof(stu), num, fp)!=num)//推进
    
    {
        fclose(fp);
        fclose(fp2);
        printf("read err");
        return -1;
    }       
//用快速排序
    sort(stud, 0, num - 1);
    fprintf(fp2, "%8s\t%8s\t%8s\n", "stunum", "name", "score");
    for ( i = 0; i < num;i++)
    {
        sum += stud[i].score;
        //格式
        fprintf(fp2,"%8d\t%8s\t%8.1f\n", stud[i].stunum, stud[i].name, stud[i].score);
    }
    printf("avr=%.1lf\n", sum / num);//num=0 ? num=负数？
    fclose(fp);
    fclose(fp2);
    free(stud);
    return num;
}

int main() {
    char filename[20]="stu",txt_name[20]="txt";
    InputStudents(filename);
    OutputStudents(filename, txt_name);
    return 0;
}