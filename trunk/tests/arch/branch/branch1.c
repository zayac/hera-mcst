#define MAX_I 100
int glob;

int main()
{
	int a = 0, b = 1, c = 2;
	int res[MAX_I];
	int i;

	for ( i = 0; i < MAX_I; i++)
	{
		if( i< 30)
		{
			res[i] = i+a;
		}else if ( i<60)
		{
			res[i] = i+b;
		}else
		{
			res[i] = i+c;
		}
	}

	return res[glob];
}
