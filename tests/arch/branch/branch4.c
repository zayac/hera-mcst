#define MAX_I 100
int arr[MAX_I];

int main()
{
	int i = 0;
	do
	{
		arr[i] = i+1;
		i++;
	}while (i< MAX_I && arr[i-1] != 0);

	return (arr[6]-7);
}
