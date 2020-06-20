#define _CRT_SECURE_NO_WARNINGS 1
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
//if 后面要加空格 参数前面加空格

typedef struct student{
    int stunum;
    char name[20];
    int score;
} stu;

void swap(int *a , int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void swapstr(char *a,char *b)
{
    char *temp=(char*)malloc(sizeof(char)*20);//正好二十字节判断
    strcpy(temp, a);
    strcpy(a, b);
    strcpy(b, temp);
    free(temp);
}

void swapstu(stu *a , stu *b)
{
    stu temp = *a;
    *a = *b;
    *b = temp;
}

int InputStudents(const char *bin_name)
{
    int num;
    FILE *fp = fopen(bin_name, "wb+");
    if(fp==NULL)
    {
        printf("open file err");
        return -1;
    }
    printf("input student num:\n");
    scanf("%d", &num);
    stu *arr = (stu *)malloc(num * sizeof(stu));  //内存要释放
      for (int i = 0; i < num;i++)
    {
        printf("请输入第%d位学生的信息", i + 1);
        scanf("%d%s%d",&arr[i].stunum,arr[i].name,&arr[i].score);//用scanf判断num   输入格式检测
    }
    fwrite(arr, sizeof(stu), num, fp);
    free(arr);
    fclose(fp);
    return num;
}

int OutputStudents(const char *bin_name,const char *txt_name)
{
    double sum=0;
     int i, j, temp;
    FILE *fp = fopen(bin_name, "rb");
    FILE *fp2 = fopen(txt_name, "w+");
    if(fp==NULL)//fp2打开没有关闭
    {
        printf("open file err");
        return -1;
    }
    fseek(fp, 0, SEEK_END);
    int num = ftell(fp) / sizeof(stu);
    rewind(fp);
    printf("student num is:%d\n", num);
    stu *stud = (stu *)malloc(num * sizeof(stu));
    fread(stud, sizeof(stu), num, fp);
    for ( i = 0; i < num;i++)
    {
        printf("stunum:%d name:%s score:%d \n", stud[i].stunum, stud[i].name, stud[i].score);
        sum += stud[i].score;
    }
    printf("avr=%.1lf", sum / num);
//用快速排序
    for (i = 0; i < num-1;i++)
    {
        //选择排序
        temp = i;
        j = i + 1;
        for (;j<num;j++)
        {
            if(stud[j].score>stud[temp].score)
                temp = j;
            else if(stud[j].score==stud[temp].score)//浮点数比较问题
            {
                if(stud[j].stunum>stud[temp].stunum)
                    temp = j;
                else
                    continue;
            }
        }
        swapstu(&stud[temp], &stud[i]);
    }
    for ( i = 0; i < num;i++)
    {
        //格式
        fprintf(fp2,"stunum:%d name:%s score:%d \n", stud[i].stunum, stud[i].name, stud[i].score);
    }
    free(stud);
    fclose(fp);
    fclose(fp2);
    return num;
}

int main() {
    char filename[20],txt_name[20];
    printf("请输入文件名:\n");
    scanf("%s", filename);
    InputStudents(filename);
    printf("请输入成绩单文件名:\n");
    scanf("%s", txt_name);
    OutputStudents(filename, txt_name);
    return 0;
}