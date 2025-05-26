#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct field {
    char type;
    int discovered;
    int crossroad;
}; // структура для поля лабиринта, содержащая данные о типе поля, исследовано ли оно и является ли оно перекёстком.

int N, M;

int tupichok(struct field** lab_map, int x, int y) { // функция проверки тупика, когда мы никуда не можем пойти
    switch (lab_map[x][y].type) {
        case '0':
            return lab_map[x - 1][y].discovered * lab_map[x][y + 1].discovered * lab_map[x + 1][y].discovered * lab_map[x][y - 1].discovered;
        case '1':
            return lab_map[x][y + 1].discovered * lab_map[x + 1][y].discovered * lab_map[x][y - 1].discovered;
        case '2':
            return lab_map[x - 1][y].discovered * lab_map[x + 1][y].discovered * lab_map[x][y - 1].discovered;
        case '3':
            return lab_map[x + 1][y].discovered * lab_map[x][y - 1].discovered;
        case '4':
            return lab_map[x - 1][y].discovered * lab_map[x][y + 1].discovered * lab_map[x][y - 1].discovered;
        case '5':
            return lab_map[x][y - 1].discovered * lab_map[x][y + 1].discovered;
        case '6':
            return lab_map[x - 1][y].discovered * lab_map[x][y - 1].discovered;
        case '7': 
            return lab_map[x][y - 1].discovered;
        case '8':
            return lab_map[x - 1][y].discovered * lab_map[x][y + 1].discovered * lab_map[x + 1][y].discovered;
        case '9':
            return lab_map[x][y + 1].discovered * lab_map[x + 1][y].discovered;
        case 'A':
            return lab_map[x - 1][y].discovered * lab_map[x + 1][y].discovered;
        case 'B':
            return lab_map[x + 1][y].discovered;
        case 'C':
            return lab_map[x - 1][y].discovered * lab_map[x][y + 1].discovered;
        case 'D':
            return lab_map[x][y + 1].discovered;
        case 'E':
            return lab_map[x - 1][y].discovered;
        case 'F':
        default: return 1;
    }
}

int discover_way(struct field** lab_map, int x, int y, int way_steps[][2], int *i) { // функция для исследования прямого пути
    int x_n = x, y_n = y;
    while ((x_n != M - 1 || y_n != N - 1) & !lab_map[x_n][y_n].crossroad) { // пока не доходим до финиша или до перекрёстка
        way_steps[*i][0] = x_n;
        way_steps[*i][1] = y_n;
        *i += 1;
        lab_map[x_n][y_n].discovered = 1;
        if (!tupichok(lab_map, x_n, y_n)) { // если мы не в тупике, то продолжаем движение
            if ((lab_map[x_n][y_n].type == '6' || lab_map[x_n][y_n].type == 'A' || lab_map[x_n][y_n].type == 'C' || lab_map[x_n][y_n].type == 'E') && !lab_map[x_n - 1][y_n].discovered) {
                x_n--;
            }
            else if ((lab_map[x_n][y_n].type == '5' || lab_map[x_n][y_n].type == '9' || lab_map[x_n][y_n].type == 'C' || lab_map[x_n][y_n].type == 'D') && !lab_map[x_n][y_n + 1].discovered) {
                y_n++;
            }
            else if ((lab_map[x_n][y_n].type == '3' || lab_map[x_n][y_n].type == '9' || lab_map[x_n][y_n].type == 'A' || lab_map[x_n][y_n].type == 'B') && !lab_map[x_n + 1][y_n].discovered) {
                x_n++;
            }
            else if ((lab_map[x_n][y_n].type == '3' || lab_map[x_n][y_n].type == '5' || lab_map[x_n][y_n].type == '6' || lab_map[x_n][y_n].type == '7') && !lab_map[x_n][y_n - 1].discovered) {
                y_n--;
            }
        }
        else if (x_n + y_n == 1) return -2;
        else return -1; // возвращаем значение -1 как сигнал того, что путь ведёт в тупик
    }
    if (x_n == M - 1 && y_n == N - 1) {
        way_steps[*i][0] = x_n;
        way_steps[*i][1] = y_n;
        *i += 1;
        return 1; // если добираемся до финиша, то возвращаем 1
    }
    else if (lab_map[x_n][y_n].crossroad) {
        way_steps[*i][0] = x_n;
        way_steps[*i][1] = y_n;
        *i += 1;
        lab_map[x_n][y_n].discovered = 1;
        return 0; // если мы наткнули на перекрёсток, то возвращаем сигнал 0
    }
}

void discover_crossroad(struct field** lab_map, int x, int y, int way_steps[][2], int *i, int best_steps[][2], int *b_i) { // функция для исследования перекрёстка
    int k;
    if ((lab_map[x][y].type == '0' || lab_map[x][y].type == '2' || lab_map[x][y].type == '4' || lab_map[x][y].type == '8') && !lab_map[x - 1][y].discovered) {
        switch (discover_way(lab_map, x - 1, y, way_steps, i)) {
            case -1:
                while (way_steps[*i - 1][0] != x || way_steps[*i - 1][1] != y) {
                    way_steps[*i - 1][0] = 0;
                    way_steps[*i - 1][1] = 0;
                    *i -= 1;
                }
                break;
            case 0:
                discover_crossroad(lab_map, way_steps[*i - 1][0], way_steps[*i - 1][1], way_steps, i, best_steps, b_i);
                break;
            case 1:
                if (*i < *b_i) {
                    for (k = *b_i; k > *i; k--) {
                        best_steps[k - 1][0] = 0;
                        best_steps[k - 1][1] = 0;
                    }
                    *b_i = *i;
                    for (k = 0; k < *b_i; k++){
                        best_steps[k][0] = way_steps[k][0];
                        best_steps[k][1] = way_steps[k][1];
                    }
                }
                break;
            default:
                break;
        }
        while (way_steps[*i - 1][0] != x || way_steps[*i - 1][1] != y) {
            lab_map[way_steps[*i - 1][0]][way_steps[*i - 1][1]].discovered = 0;
            way_steps[*i - 1][0] = 0;
            way_steps[*i - 1][1] = 0;
            *i -= 1;
        }
    }
    if ((lab_map[x][y].type == '0' || lab_map[x][y].type == '1' || lab_map[x][y].type == '4' || lab_map[x][y].type == '8' || lab_map[x][y].type == '9') && !lab_map[x][y + 1].discovered) {
        switch (discover_way(lab_map, x, y + 1, way_steps, i)) {
            case -1:
                while (way_steps[*i - 1][0] != x || way_steps[*i - 1][1] != y) {
                    way_steps[*i - 1][0] = 0;
                    way_steps[*i - 1][1] = 0;
                    *i -= 1;
                }
                break;
            case 0:
                discover_crossroad(lab_map, way_steps[*i - 1][0], way_steps[*i - 1][1], way_steps, i, best_steps, b_i);
                break;
            case 1:
                if (*i < *b_i) {
                    for (k = *b_i; k > *i; k--) {
                        best_steps[k - 1][0] = 0;
                        best_steps[k - 1][1] = 0;
                    }
                    *b_i = *i;
                    for (k = 0; k < *b_i; k++){
                        best_steps[k][0] = way_steps[k][0];
                        best_steps[k][1] = way_steps[k][1];
                    }
                }
                break;
            default:
                break;
        }
        while (way_steps[*i - 1][0] != x || way_steps[*i - 1][1] != y) {
            lab_map[way_steps[*i - 1][0]][way_steps[*i - 1][1]].discovered = 0;
            way_steps[*i - 1][0] = 0;
            way_steps[*i - 1][1] = 0;
            *i -= 1;
        }
    }
    if ((lab_map[x][y].type == '0' || lab_map[x][y].type == '1' || lab_map[x][y].type == '2' || lab_map[x][y].type == '8' || lab_map[x][y].type == '9') && !lab_map[x + 1][y].discovered) {
        switch (discover_way(lab_map, x + 1, y, way_steps, i)) {
            case -1:
                 while (way_steps[*i - 1][0] != x || way_steps[*i - 1][1] != y) {
                    way_steps[*i - 1][0] = 0;
                    way_steps[*i - 1][1] = 0;
                    *i -= 1;
                }
                break;
            case 0:
                discover_crossroad(lab_map, way_steps[*i - 1][0], way_steps[*i - 1][1], way_steps, i, best_steps, b_i);
                break;
            case 1:
                if (*i < *b_i) {
                    for (k = *b_i; k > *i; k--) {
                        best_steps[k - 1][0] = 0;
                        best_steps[k - 1][1] = 0;
                    }
                    *b_i = *i;
                    for (k = 0; k < *b_i; k++){
                        best_steps[k][0] = way_steps[k][0];
                        best_steps[k][1] = way_steps[k][1];
                    }
                }
                break;
            default:
                break;
        }
        while (way_steps[*i - 1][0] != x || way_steps[*i - 1][1] != y) {
            lab_map[way_steps[*i - 1][0]][way_steps[*i - 1][1]].discovered = 0;
            way_steps[*i - 1][0] = 0;
            way_steps[*i - 1][1] = 0;
            *i -= 1;
        }
    }
    if ((lab_map[x][y].type == '0' || lab_map[x][y].type == '1' || lab_map[x][y].type == '2' || lab_map[x][y].type == '4') && !lab_map[x][y - 1].discovered) {
        switch (discover_way(lab_map, x, y - 1, way_steps, i)) {
            case -1:
                while (way_steps[*i - 1][0] != x || way_steps[*i - 1][1] != y) {
                    way_steps[*i - 1][0] = 0;
                    way_steps[*i - 1][1] = 0;
                    *i -= 1;
                }
                break;
            case 0:
                discover_crossroad(lab_map, way_steps[*i - 1][0], way_steps[*i - 1][1], way_steps, i, best_steps, b_i);
                break;
            case 1:
                if (*i < *b_i) {
                    for (k = *b_i; k > *i; k--) {
                        best_steps[k - 1][0] = 0;
                        best_steps[k - 1][1] = 0;
                    }
                    *b_i = *i;
                    for (k = 0; k < *b_i; k++){
                        best_steps[k][0] = way_steps[k][0];
                        best_steps[k][1] = way_steps[k][1];
                    }
                }
                break;
            default:
                break;
        }
        while (way_steps[*i - 1][0] != x || way_steps[*i - 1][1] != y) {
            lab_map[way_steps[*i - 1][0]][way_steps[*i - 1][1]].discovered = 0;
            way_steps[*i - 1][0] = 0;
            way_steps[*i - 1][1] = 0;
            *i -= 1;
        }
    }
    way_steps[*i - 1][0] = 0;
    way_steps[*i - 1][1] = 0;
    *i -= 1;
    lab_map[x][y].discovered = 0;
    return;
}
