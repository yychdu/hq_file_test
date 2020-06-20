#define _CRT_SECURE_NO_WARNINGS 1
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
//if 后面要加空格 参数前面加空格
//有返回值的都要检测 fread fwrite;
//能不能用goto
typedef struct student{
    int stunum;
    char name[20];
    float score;//浮点数比较 fabs(a-b)<=1E-6
} stu;

void swapstu(stu *a , stu *b)
{
    stu temp = *a;
    *a = *b;
    *b = temp;
}

//分治法把数组分成两份
int patition(stu *a, int left,int right) {
    int j = left;    //用来遍历数组
    int i = j - 1;    //用来指向大于基准元素的位置
    //从左到右遍历数组，把大于等于基准元素的放到左边，小于基准元素的放到右边
    for (; j < right; ++j) {
        if (a[j].score >= a[right].score)
            swapstu(&a[j], &a[++i]);
    }
    //把基准元素放到中间
    swapstu(&a[right], &a[++i]);
    //返回数组中间位置
    return i;
}

//快速排序
void quickSort(stu *a,int left,int right) {
    if (left>=right)
        return;
    int mid = patition(a,left,right);
    quickSort(a, left, mid - 1);
    quickSort(a, mid + 1, right);
}



int InputStudents(const char *bin_name)
{
    int num=0, k=0;
     int stunum;
    char name[20];
    float score;//浮点数比较 fabs(a-b)<=1E-6
    FILE *fp = fopen(bin_name, "wb+");
    if (fp==NULL)
    {
        printf("open file err");
        return -1;
    }
    // printf("input student num:\n");
    // scanf("%d", &num);//num=0 ? num=负数？
    
    //   for (int i = 0; i < num;i++)
    // {
    //     printf("请输入第%d位学生的信息", i + 1);
    //     scanf("%d%s%f",&arr[i].stunum,arr[i].name,&arr[i].score);//用scanf判断num  ???怎么停 输入格式检测  猜想(ret=scanf())!=EOF?
    // }
    stu *arr = (stu *)malloc(num * sizeof(stu));  //内存要释放
    printf("input student data:\n");
    while(1)
    {
        k = scanf("%d%s%f", &stunum, name, &score);
        printf("k=%d\n", k);
        if(k==EOF)
            break;
        if  (k!=3)
        {
            printf("格式输入错误:\n");
            return -1;
        }
        else
        {
            num++;
            stu *ptr=realloc(arr, num * sizeof(stu));//检验
            if(!ptr)
            {
                printf("realloc err");
                return -1;
            }
            arr = ptr;
            arr[num-1].stunum = stunum;
            memcpy(arr[num-1].name, name, 20);
            arr[num-1].score = score;
        }
    }

   
    fwrite(arr, sizeof(stu), num, fp);//return value judgement
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

    if (fp==NULL&&fp2!=NULL)//fp2打开没有关闭
    {
        fclose(fp2);
        printf("open file err");
        return -1;
    }
    if (fp2==NULL&&fp!=NULL)//fp打开没有关闭
    {
        fclose(fp);
        printf("open file err");
        return -1;
    }
    if(fp==NULL&&fp2==NULL)
    {
         printf("open file err");
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    if(sizeof(stu)<=0)
    {
        fclose(fp);
        fclose(fp2);
        printf("stu ERR");
        return -1;
    }
    int num = ftell(fp) / sizeof(stu);
    if(num<=0)
    {
        fclose(fp);
        fclose(fp2);
        printf("num ERR");
        return -1;
    }
    rewind(fp);
    printf("student num is:%d\n", num);
    stu *stud = (stu *)malloc(num * sizeof(stu));
    fread(stud, sizeof(stu), num, fp);//return value judgement
    for ( i = 0; i < num;i++)
    {
        printf("stunum:%d name:%s score:%f \n", stud[i].stunum, stud[i].name, stud[i].score);
        sum += stud[i].score;
    }
    printf("avr=%.1lf", sum / num);//num=0 ? num=负数？
//用快速排序
    quickSort(stud, 0, num - 1);
    fprintf(fp2, "%8s%8s%8s\n", "stunum", "name", "score");
    for ( i = 0; i < num;i++)
    {
        //格式
        
        fprintf(fp2,"%8d%8s%8.1f\n", stud[i].stunum, stud[i].name, stud[i].score);
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