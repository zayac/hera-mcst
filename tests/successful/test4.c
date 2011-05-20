int main()
{
	int a = -128;
	int b = 32766;
	int c = 0;
	int i;
	for (i=0;i<256;i++) c=c+a;
	return c - b;
} 
