int glob;

void g()
{
   glob++; 
}

void f()
{
   glob = glob - 1;
}
void h()
{
   g( );
   f( );
}

int main()
{
	h();
   return glob;
}
