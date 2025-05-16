#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct bib {
    char title[200];
    char author[200];
    char publisher[200];
    char isbn[200];
    int year;
    char series[200];
    char edition[200];
    char volume[200];
    char url[200];
}; // структура для файла bib

int sort_bib(struct bib a, struct bib b) { // функция для сортировки карточек по авторам, названию и изданию
    int auth = strcmp(a.author, b.author);
    int titl = strcmp(a.title, b.title);
    int edit = strcmp(a.edition, b.edition);
    if (auth == 0) {
        if (titl == 0) {
            if (edit < 0) return 1;
            else return 0;
        }
        else {
            if (titl < 0) return 1;
            else return 0;
        }
    }
	else {
        if (auth < 0) return 1;
        else return 0;
    }
}

struct tnode
{
   struct bib card;
   struct tnode *lchild, *rchild;
}; // структура для корней бинарного древа

struct tnode *tree_insert(struct tnode *p, struct bib val) {
    if (p == NULL) {
        p = (struct tnode *) malloc(sizeof(struct tnode)); /* insert the new node as root node*/
        if (p == NULL) printf("Cannot allocate\n");  exit(0);
        p -> card = val;
        p -> lchild = p -> rchild = NULL;
        return p;
    }
    if (sort_bib(p -> card, val)) {
        p -> lchild = tree_insert (p -> lchild, val);
        return p;
    }
    else {
        p -> rchild = tree_insert (p -> rchild, val);
        return p;
    }
    return p;
}

void tree_print_file(struct tnode *p, FILE *f) {
    if (p != NULL) {
        tree_print_file(p -> lchild, f);
        fprintf(f, "title: %s\n",p->card.title);
        fprintf(f, "author: %s\n",p->card.author);
        fprintf(f, "publisher: %s\n",p->card.publisher);
        fprintf(f, "isbn: %s\n",p->card.isbn);
        fprintf(f, "year: %d\n",p->card.year);
        fprintf(f, "series: %s\n",p->card.series);
        fprintf(f, "edition: %s\n",p->card.edition);
        fprintf(f, "volume: %s\n",p->card.volume);
        fprintf(f, "url: %s\n",p->card.url);
        fprintf(f, "-------------------------\n");
        tree_print_file(p -> rchild, f);
    }
}

struct tnode *tree = NULL; // дерево для карточек

int main(int argc, char *argv[])
{
    FILE *F;
    FILE *OUT = fopen("output.txt", "a");
    char str[1024], text[200];
    struct bib bib_card;
    int i;
    while (--argc) // идем по списку файлов
    {
        F = fopen(argv[argc],"r"); // открываем очередной файл
        printf("Обрабатывается файл %s\n",argv[argc]);
        if (!F) {
            printf("Ошибка открытия файла %s\n",argv[argc]);
            exit (1);
        }
        while (fgets(str, 1024, F) != NULL) {
            if (str[15] == '{') {
                i = 16;
                while (str[i] != '}') {
                    text[i - 16] = str[i];
                    i++;
                }
                switch (str[3]) {
                    case 't':
                        strcpy(bib_card.title, text);
                        break;
                    case 'a':
                        strcpy(bib_card.author, text);
                        break;
                    case 'p':
                        strcpy(bib_card.publisher, text);
                        break;
                    case 'i':
                        strcpy(bib_card.isbn, text);
                    break;
                    case 'y':
                        bib_card.year = atoi(text);
                        break;
                    case 's':
                        strcpy(bib_card.series, text);
                        break;
                    case 'e':
                        strcpy(bib_card.edition, text);
                        break;
                    case 'v':
                        strcpy(bib_card.volume, text);
                        break;
                    case 'u':
                        strcpy(bib_card.url, text);
                        break;
                    default:
                        break;
                }
                memset(text, '\0', sizeof(text));
            }
        }
        tree = tree_insert(tree, bib_card);
    }
    fprintf(OUT, "-------------------------\n");
    tree_print_file(tree, OUT);
    fclose(F);
    fclose(OUT); // закрытие всех файлов
    return 0; 
}
