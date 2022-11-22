#include <stdio.h>
 
#define MAXCHAR 101  /*��������ַ�������*/
 
int char_to_num(char ch);  /*�����ַ���Ӧ������*/ 
char num_to_char(int num);  /*�������ֶ�Ӧ���ַ�*/
long source_to_decimal(char temp[], int source);  /*������ԭ��ת���ɵ�10������*/
int decimal_to_object(char temp[], long decimal_num, int object);  /*����ת����Ŀ�����ƺ��ַ�����ĳ���*/
void output(char temp[], int length);  /*���ַ����������ӡ*/
 
int main()
{
    int source;  /*�洢ԭ����*/
    int object;  /*�洢Ŀ������*/
    int length;  /*�洢ת����Ŀ�����ƺ��ַ�����ĳ���*/
    long decimal_num;  /*�洢ת���ɵ�10������*/
    char temp[MAXCHAR];  /*�洢��ת������ֵ��ת�������ֵ*/
    int flag = 1;  /*�洢�Ƿ��˳�����ı�־*/
    while(flag)  /*���������flagֵ����ѭ���Ƿ����*/
    {
        
        scanf("%s", temp);
        source =10;
        
        scanf("%d", &object);
       	if (temp<0)
       	{
       		printf("-");
       		temp=-temp;
		   }
        decimal_num = source_to_decimal(temp, source);
        length = decimal_to_object(temp, decimal_num, object);
        output(temp, length);
        flag = 0;
        
    }
 
    return 0;
}
/*���ַ�ת��������*/
int char_to_num(char ch)
{
    if(ch>='0' && ch<='9')
        return ch-'0';  /*�������ַ�ת��������*/
    else
        return ch-'A'+10;  /*����ĸ�ַ�ת��������*/
}
char num_to_char(int num)
{
    if(num>=0 && num<=9)
        return (char)('0'+num-0);  /*��0~9֮�������ת�����ַ�*/
    else
        return (char)('A'+num-10);  /*������10������ת�����ַ�*/
}
long source_to_decimal(char temp[], int source)
{
    long decimal_num = 0;  /*�洢չ��֮��ĺ�*/
    int length;
    int i;
    for( i=0; temp[i]!='\0'; i++ );
    length=i;
    for( i=0; i<=length-1; i++ )  /*�ۼ�*/
        decimal_num = (decimal_num*source) + char_to_num(temp[i]);
    return decimal_num;
}
int decimal_to_object(char temp[], long decimal_num, int object)
{
    int i=0;
    while(decimal_num)
    {
        temp[i] = num_to_char(decimal_num % object);  /*���������ת��Ϊ�ַ�*/
        decimal_num = decimal_num / object;  /*��ʮ���������Ի���*/
        i++;
    }
    temp[i]='\0';
    return i;
}
void output(char temp[], int length)
{
    int i;
    for( i=length-1; i>=0; i--)  /*���temp�����е�ֵ*/
        printf("%c", temp[i]);
    printf("\n");
}
