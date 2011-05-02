int a,b;

int main()
{
	if (a == b)
		goto LABEL;
	a = -1;

LABEL:
	a += 1;

	return (a-1);
}
