#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/time.h>

typedef struct word {
    int count;
    char Word[50];
} WORD;

WORD words[100000];

int totalw = 0;

int sort_alpha(const void* a, const void* b) {
	return strcmp(((struct word*)a) -> Word, ((struct word*)b) -> Word);
}

int sort_count(const void* a, const void* b) {
	if (((struct word*)a) -> count == ((struct word*)b) -> count) return strcmp(((struct word*)a) -> Word, ((struct word*)b) -> Word);
	else return ((struct word*)b) -> count - ((struct word*)a) -> count;
}

int main(int ac, char *av[])
{
    FILE *OUT1 = fopen("output1.txt", "a");
    FILE *OUT2 = fopen("output2.txt", "a");
    FILE *IN;
    char string[150];
    char *one_word;
    int i;
    if (!OUT1 || !OUT2)
    {
        printf("Ошибка с инициализацией файловых переменных.\n");
        exit (0);
    }
    if (ac < 2) printf ("Вы забыли указать список файлов!\n"); exit (0);
    while (--ac) // идем по списку файлов
    {
        IN = fopen(av[ac],"r");
        printf("Обрабатывается файл %s\n",av[ac]);
        // открываем очередной файл
        while (fgets(string, 100, IN) != NULL) // сканируем строку файла
        {
            one_word = strtok(string, " ,.!?:;-+=`№^&_*#$()[]{}<>/|\"\\"); // с помощью функции strtok записываем слово
            while (one_word != NULL) {
                if (isdigit(one_word[0])) {
                    one_word = strtok(NULL, " ,.!?:;-+=`№^&_*#$()[]{}<>/|\"\\");
                    continue; // если первый символ слова является цифрой, то переходим к следующему слову
                }
                for (i = 0; i < strlen(one_word); i++) one_word[i] = tolower(one_word[i]); // все заглавные буквы переводим в прописные
                i = 0;
                while(i < totalw) {
                    if (strncmp(words[i].Word, one_word, strlen(words[i].Word) >= strlen(one_word) ? strlen(words[i].Word) : strlen(one_word)) == 0) { // если слово уже встречалось, 
                        words[i].count++; // то его количество прибавляется в соотвествующей структуре
                        break;
                    }
                    else i++; // иначе прибавлем 1 к счётчику
                }
                if (i == totalw) { // если в ходе проверки не было совпадений (i = кол-ву всех слов)
                    words[i].count = 1;
                    strcpy(words[i].Word, one_word);
                    totalw++; // мы даём значения структуре и прибавляем число всех слов
                }
                one_word = strtok(NULL, " ,.!?:;-+=`№^&_*#$()[]{}<>/|\"\\"); // переходим к следующему слову
            }
        }
    }
    for (i = 0; i < totalw; i++) printf("%s %d\n", words[i].Word, words[i].count);
    qsort(words, totalw, sizeof(WORD), sort_alpha); // сортировка по символам
    fprintf(OUT1, "Всего слов: %d.\n", totalw);
    fprintf(OUT1, "Слова в алфавитном порядке:\n");
    for (i = 0; i < totalw; i++) fprintf(OUT1, "%s %d;\n", words[i].Word, words[i].count); // запись отсортированного массива в файл
    qsort(words, totalw, sizeof(WORD), sort_count); // сортировка по количеству встреч слова в тексте
    fprintf(OUT2, "Всего слов: %d.\n", totalw);
    fprintf(OUT2, "Слова по количеству:\n");
    for (i = 0; i < totalw; i++) fprintf(OUT2, "%s %d;\n", words[i].Word, words[i].count); // запись отсортированного массива в файл
    fclose(IN);
    fclose(OUT1);
    fclose(OUT2); // закрытие всех файлов
    return 0; 
}
