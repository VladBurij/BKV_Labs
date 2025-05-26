#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "labirint_funcs.h"

void labirint_map(FILE *out, int M, int N, int way[][2], int steps) {
    int i, k;
    char map[M - 1][N];
    for (i = 0; i < M; i++) {
        for (k = 0; k < N; k++) map[i][k] = '*';
        map[i][N] = '\0';
    }
    for (i = 0; i < steps; i++) {
        if (i == steps - 1) map[M - 1][N - 1] = 'F';
        else {
            if (way[i + 1][0] == way[i][0] - 1 && way[i + 1][1] == way[i][1]) map[way[i][0]][way[i][1]] = 'A';
            else if (way[i + 1][0] == way[i][0] && way[i + 1][1] == way[i][1] + 1) map[way[i][0]][way[i][1]] = '>';
            else if (way[i + 1][0] == way[i][0] + 1 && way[i + 1][1] == way[i][1]) map[way[i][0]][way[i][1]] = 'V';
            else if (way[i + 1][0] == way[i][0] && way[i + 1][1] == way[i][1] - 1) map[way[i][0]][way[i][1]] = '<';
        }
    }
    fprintf(out, "В самом коротком маршруте: %d шагов\n", steps);
    for (i = 0; i < M; i++) {
        for (k = 0; k < N; k++){
            fprintf(out, "%c", map[i][k]);
        }
        fprintf(out, "\n");
    }
}

int main(char *file_name)
{
    FILE *Lab = fopen(file_name,"r"); // открываем файл;
    FILE *Way = fopen("C:/Users/Vlad/Desktop/prog_C/output.txt", "w"); // создаём файл output
    char f_str[20];
    char c;
    int i = 0, k = 0;
    if (!Lab)
    {
        printf("Ошибка с инициализацией файловых переменных.\n");
        exit (0);
    }
    fscanf(Lab, "%d %d\n", &M, &N); // записываем значения размера лабиринта
    struct field **labirint; // массив лабиринта, содержащий структуры полей
    labirint = (struct field**)malloc(M * sizeof *labirint);
    for (i = 0; i < M; i++) labirint[i] = (struct field*)malloc(N * sizeof *labirint[i]);
    i = 0;
    while (c = fgetc(Lab), c != EOF) { // считываем по одному символу
        if (c == '\n') continue;
        labirint[k][i].type = c; // указываем его в тип поля
        if (labirint[k][i].type == 'F' & k != M & i != N) labirint[k][i].discovered = 1; // если поле замкнутое и не конечное, то оно считается исследованным
        else labirint[k][i].discovered = 0; // поле считается не исследованным, если оно не замкнутое
        if (labirint[k][i].type == '0' || labirint[k][i].type == '1' || labirint[k][i].type == '2' || labirint[k][i].type == '4' || labirint[k][i].type == '8' || (labirint[k][i].type == '9' & k == 0 & i == 0)) labirint[k][i].crossroad = 1;
        else labirint[k][i].crossroad = 0; // поле считается перекрёстком, если из него можно пойти по более чем 2 путями
        if ((i + 1) % N == 0) {
            k++;
            i = 0;
        }
        else i++;
    }
    int way[N*M][2], best_way[N*M][2]; // массивы для сохранения координат полей пути
    int steps = 0, best_steps = N*M; // подсчёт кол-ва шагов
    if (!labirint[0][0].crossroad) { // если начальная точка не является является перекрёстком, 
        switch (discover_way(labirint, 0, 0, way, &steps)) { // то идём по пути пока не наткнёмся на финиш или перекрёсток
            case -1:
                printf("Лабиринт не имеет выхода\n");
                exit(0);
            case 0:
                discover_crossroad(labirint, way[steps - 1][0], way[steps - 1][1], way, &steps, best_way, &best_steps);
                break;
            case 1:
                labirint_map(Way, M, N, way, steps);
                return 0;
        }
    }
    else {
        way[0][0] = 0;
        way[0][1] = 0;
        steps++;
        labirint[0][0].discovered = 1;
        discover_crossroad(labirint, 0, 0, way, &steps, best_way, &best_steps);
    }
    for (i = 0; i < best_steps; i++) printf("%d %d\n", best_way[i][0], best_way[i][1]);
    for (i = 0; i < M; i++) {
        for (k = 0; k < N; k++) {
            printf("%d ", labirint[i][k].discovered);
        }
        printf("\n");
    }
    labirint_map(Way, M, N, best_way, best_steps);
    for (i = 0; i < M; i++) free(labirint[i]);
    free(labirint);
    fclose(Lab);
    fclose(Way); // закрытие всех файлов
}
