#define _CRT_SECURE_NO_WARNINGS 1
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#define SIZE 100
//if 后面要加空格 参数前面加空格
//有返回值的都要检测 fread fwrite;
//能不能用goto
//字符串内存越界
//排序问题：成绩相同再来一遍？
typedef struct student{
    int stunum;
    char name[20];
    double score;//浮点数比较 fabs(a-b)<=1E-6
} stu;

void swapstu( stu *a , stu *b)
{
    stu temp = *a;
    *a = *b;
    *b = temp;
}

int mycomp( stu *a, stu *b)
{
    if  (a->score > b->score)
        return 1;
    else if (a->score < b->score)
        return 0;
    else if ( fabs(a->score - b->score )<=1E-6)
    {
        if (a->stunum > b->stunum)
            return 0;
        else if (a->stunum < b->stunum)
            return 1;
    }
    return 0;
}

//分治法把数组分成两份
int patition(stu *a, int left, int right) {
    int j = left;    //用来遍历数组
    int i = j - 1;    //用来指向大于基准元素的位置
    //从左到右遍历数组，把大于等于基准元素的放到左边，小于基准元素的放到右边
    for (; j < right; ++j) {
        // if (a[j].score >= a[right].score)
        if (mycomp( &a[j] , &a[right] ))
            swapstu( &a[j], &a[++i]);
    }
    //把基准元素放到中间
    swapstu(&a[right], &a[++i]);
    //返回数组中间位置
    return i;
}
//快速排序
void quickSort( stu *a, int left, int right) {
    if (left >= right)
        return;
    int mid = patition(a, left , right);
    quickSort(a, left, mid - 1);
    quickSort(a, mid + 1, right);
}

int InputStudents( const char *bin_name)
{
    int num = 0, k = 0, ret_val = 0;
    char buf[SIZE];
    FILE *fp = fopen( bin_name, "wb+");
    if (fp == NULL)
    {
        printf("open file err");
        return -1;
    }
    
    while(1)
    {
        stu temp;
        printf("input student data:\n");
        k = scanf("%d%19s%lf", &temp.stunum, temp.name, &temp.score);
        // printf("k=%d\n", k);
        if  (k != 3)
        {
            printf("格式输入错误!跳出\n");
            break;
        }
        else
        {

            if (snprintf(buf, SIZE , "%d,%s,%f", temp.stunum, temp.name, temp.score) <= 0)
            {
                printf("格式化错误");
                break;
            }
            num++;
            ret_val = fwrite(buf, sizeof(stu)+2, 1, fp);
            //return value judgement
            if(ret_val != 1)
            {
                printf("write ERR");
                break;
            }
            
            
        }
    }

    fclose(fp);
    rewind(stdin);
    return num;
}

int OutputStudents(const char *bin_name, const char *txt_name)
{
  
    
    double sum = 0;
    int i,flag = 0,j,ret_val = 0;
    char buf[SIZE], str[SIZE]="";
    FILE *fp = fopen(bin_name, "rb");
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
    if (sizeof(stu) <= 0)
    {
        fclose(fp);
        fclose(fp2);
        printf("stu ERR");
        return -1;
    }
    int num = ftell(fp) / sizeof(stu);
    if (num <= 0)
    {
        fclose(fp);
        fclose(fp2);
        printf("num ERR");
        return -1;
    }
    rewind(fp);
    printf("student num is:%d\n", num);
    stu *stud = (stu *)malloc(num * sizeof(stu));
    for ( i = 0; i < num; i++)
    {
        memset(str, 0, SIZE);
        ret_val = fread( buf, sizeof(stu)+2, 1, fp);
        if (ret_val != 1)
        {
            fclose(fp);
            fclose(fp2);
            printf("READ ERR!");
            return -1;
        }
        // 提取字符串
        for (j = 0; buf[j]!=','; j++)
        {
            str[j] = buf[j];
        }
        j++;
        // printf("str %s\n", str);
        stud[i].stunum = atoi(str);
        memset(str, 0, SIZE);

        for ( int k=0; buf[j] !=','; j++)
        {
            str[k++] = buf[j];
        }
        
        memcpy(stud[i].name, str, strlen(str)+1);
        // printf("name %s\n", stud[i].name);
        memset(str, 0, SIZE);
        j++;

        for (int k=0; buf[j - 2] != '.'; j++)
        {
            str[k++] = buf[j];
        }
        // printf("str %s\n", str);
        stud[i].score = atof(str);
        // printf("buf %s\n", buf);
        sum += stud[i].score;
    }
    printf("avr=%.1lf\n", sum / num);//num=0 ? num=负数？
//用快速排序
    quickSort(stud, 0, num - 1);
    fprintf(fp2, "%8s\t%8s\t%8s\n", "stunum", "name", "score");
    for ( i = 0; i < num;i++)
    {
        //格式
        fprintf(fp2,"%8d\t%8s\t%8.1f\n", stud[i].stunum, stud[i].name, stud[i].score);
    }
    fclose(fp);
    fclose(fp2);
    free(stud);
    return num;
}

int main() {
    char filename[20],txt_name[20];
    printf("请输入文件名:\n");
    if(scanf("%19s", filename) != 1)
    {
        printf("input file name ERR");
        return -1;
    }
    InputStudents(filename);
    printf("请输入成绩单文件名:\n");
    if(scanf("%19s", txt_name) != 1)
    {
        printf("input txt name ERR");
        return -1;
    }
    OutputStudents(filename, txt_name);
    return 0;
}