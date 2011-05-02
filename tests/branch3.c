typedef enum
{
CASE0,
CASE1,
CASE2
} my_enum_t;

my_enum_t a = 1;

int main( )
{
	int res;
	switch( a)
	{
		case CASE0:
		case CASE2:
			res = 1;
			break;
		case CASE1:
			res = 0;
			break;
		default:
			res = -1;
			break;
	}

	return res;
}
