class A{

public:

	int x;

	int foo(){
		x += 73;
		return x;
	}
};

int main(){

	A a;
	A b;
	int x;

	x = a.foo();
	a.x = 125;
	b.x = 976;

	x = a.foo();
	x = b.foo();

	return 5;
}