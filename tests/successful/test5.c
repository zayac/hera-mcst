int main()
{
	int b = 32766;
	int c = 0;
	int i;
	for (i=0;i<32766;i++) c=c+1;
	return c - b;
}  
