void sub();
int main()
{
	int a = 1;
    int b = 2;
    int c = sub(b);
    c=c-a;
	return c;
}

int sub(int b)
{
    return b;
}
