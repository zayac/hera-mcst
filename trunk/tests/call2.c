int i;

int fact()
{
	if ( i == 0)
	{
		return 1;
	}else
	{
		i--;
		return (i+1)*fact();
	}
}

int main()
{
	int res;
	i = 4;
	res = fact( );
	return res;
}
