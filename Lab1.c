#include <stdio.h>

#define SETBIT(b, k) ((0200 >> (k)) | (b))
#define CLRBIT(b, k) (~(0200 >> (k)) & (b))
#define TESTBIT(b, k) (((0200 >> (k)) & (b)) ? 1 : 0)

#define DEBUG
#undef DEBUG
unsigned char nat[10000000];

main() {
    long N;
	int number, simples[10000];
    printf("Впишите N: ");
    scanf("%ld", &N);

    for (int i = 0; i < N / 8; i++) nat[i] = -1; // прописываем 1 в байтах
	for (int k = 0; k < N % 8; k++) nat[N/8] = SETBIT(nat[N/8], k); // прописываем оставшиеся 1 в битах следующего байта

	int i = 0;
	do {
		for (int k = 0; k < 8; k++) {
			if (i == 0 & k == 0) {
				continue; // самый первый бит (под номером 1) пропускаем
			}
			number = i * 8 + k + 1;
			if (number*number > N) {
				break; // остановка алгоритма если квадрат номера бита больше N
			}
			for (int y = number*number - 1; y < N; y += number) nat[y / 8] = CLRBIT(nat[y / 8], y % 8); // все биты под номерами, кратными текущему номеру записываем 0,
			// таким образом в ряду остаются только простые числа
		}
		i++;
	} while( i < N / 8);
	nat[0] = CLRBIT(nat[0], 0); // 1 не является простым числом, поэтому первый бит всегда будет равен 0

	int n = -1;
	for (i = 0; i < N; i++){
		if (TESTBIT(nat[i/8], i%8)) { // если бит равен 1, то его номер записывается в массив
			n++;
			simples[n] = i + 1;
		}
	}

	for (i = 0; i <= n; i++) printf("%d ", simples[i]);

#ifdef DEBUG
    // показ битового массива
	for (int i = 0; i < (N / 8); i++) {
		for (int k = 0; k < 8; k++) putchar(TESTBIT(nat[i], k) ? '1' : '0');
	}
	for (int k = 0; k < N % 8; k++) putchar(TESTBIT(nat[N/8], k) ? '1' : '0');
#endif
}
