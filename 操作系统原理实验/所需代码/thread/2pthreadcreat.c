#include<stdio.h>

int main()
{
    int n;
    int i,j;
    scanf("%d",&n);
     for(i=1;i< n/2+2;i++)
     {
         for(j=1;j< n/2+2-i;j++)  //�Ǻ�ǰ��2���ո� 
             printf("  ");
         for(j=1;j< 2*i;j++)    // �Ǻ�+1���ո�
             printf("* ");
             printf("\n");
     }
    for( i=n/2; i>0; i-- )
{
    for(j=1;j< n/2+2-i;j++)  //�Ǻ�ǰ��2���ո� 
       printf("  ");
    for(j=1;j< 2*i;j++)    // �Ǻ�+1���ո�
       printf("* ");
    printf("\n");
}
    return 0;
}

