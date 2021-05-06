#include<stdio.h>
int main()
{
 int a,b,res1,res2;
 float res3,res4;
 printf("enter the values of a and b\n");
 scanf("%d %d",&a,&b);
 res1 = a+b;
 {
 printf("add is %d",res1);
 }
 res2 = a-b;
 {
 printf("sub is %d",res2);
 }
 res3 = a*b;
 {
 printf("mul is %f",res3);
 }
 res4 = (float)a/b;
 {
 printf("div is %f",res4);
 }
 return 0;
}
