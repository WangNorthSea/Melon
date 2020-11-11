int main(void) {	
	int a = 1;
	int i = 0;

	a += 2;
	a = i * 3;
	for (i = 0; i < 100; i++) {
		a++;
	}

	if (a > 10)
		a <<= 5;
	
	return 0;
}

