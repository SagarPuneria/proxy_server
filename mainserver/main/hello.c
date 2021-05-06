#include<stdio.h>
int main()
{
	float t[2][3] = { { 1.1,1.2,1.3},{2.1,2.2,2.3}};
	printf("address at table=%p\n",t);
	printf("address at (table+1)=%p\n",t+1);
	printf("address at *(table+1)=%p\n",*(t+1));
	printf("address at (*(table+1)+1)=%p\n",*(t+1)+1);
	printf("address at (*(table)+1)=%p\n",*t+1);
	printf("value at *(*(table+1) +1)=%f\n",*(*(t+1)+1));
	printf("value at *(*(table)+1)=%f\n",*(*t+1));
	printf("value at *(*(table+1))=%f\n",*(*(t+1)));
	printf("value at *(*(table)+1) +1=%f\n",*(*t+1)+1);
}
