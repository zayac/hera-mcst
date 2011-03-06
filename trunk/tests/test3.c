int main()
{
	int a = 1;
	int b = 5;
	int i;
	for (i = 0; i < b; i++)
	    a += i * 7;
	return a; // a = 1 + 7 * (0 + 1 + 2 + 3 + 4) = 1 + 7 * 10 = 71
}
