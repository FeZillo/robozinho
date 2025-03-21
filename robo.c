#include <stdio.h>
#include <stdlib.h>

#define MAX 100

// Estrutura para a pilha (movimentos do robô)
typedef struct {
    int x[MAX];
    int y[MAX];
    int topo;
} Pilha;

// Estrutura para a fila (chaves coletadas)
typedef struct {
    int chaves[MAX];
    int frente, tras;
} Fila;

// Inicialização da pilha
void inicializaPilha(Pilha *p) {
    p->topo = -1;
}

// Verifica se a pilha está vazia
int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

// Empilha um movimento
void empilha(Pilha *p, int x, int y) {
    if (p->topo < MAX - 1) {
        p->topo++;
        p->x[p->topo] = x;
        p->y[p->topo] = y;
    }
}

// Desempilha um movimento
void desempilha(Pilha *p, int *x, int *y) {
    if (!pilhaVazia(p)) {
        *x = p->x[p->topo];
        *y = p->y[p->topo];
        p->topo--;
    }
}

// Inicialização da fila
void inicializaFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return f->frente > f->tras;
}

// Enfileira uma chave
void enfileira(Fila *f, int chave) {
    if (f->tras < MAX - 1) {
        f->tras++;
        f->chaves[f->tras] = chave;
    }
}

// Desenfileira uma chave
int desenfileira(Fila *f) {
    if (!filaVazia(f)) {
        return f->chaves[f->frente++];
    }
    return -1;
}

// Representação do labirinto (M = parede, K = chave, D = porta, E = saída, . = caminho livre)
char labirinto[5][5] = {
    {'.', 'K', 'M', 'D', '.'},
    {'.', 'M', '.', 'M', 'E'},
    {'.', '.', 'K', 'M', '.'},
    {'M', 'M', '.', '.', '.'},
    {'.', '.', '.', 'M', 'D'}
};

// Tamanho do labirinto
int largura = 5, altura = 5;

// Função para encontrar o caminho até a saída
void explorarLabirinto(int startX, int startY) {
    Pilha movimentos;
    Fila chaves;
    inicializaPilha(&movimentos);
    inicializaFila(&chaves);

    int x = startX, y = startY;
    empilha(&movimentos, x, y);

    while (!pilhaVazia(&movimentos)) {
        // Posição atual
        x = movimentos.x[movimentos.topo];
        y = movimentos.y[movimentos.topo];

        if (labirinto[x][y] == 'E') {
            printf("Saída encontrada em (%d, %d)!\n", x, y);
            return;
        }

        // Marca a posição como visitada
        labirinto[x][y] = 'V';

        // Movimentos possíveis: direita, baixo, esquerda, cima
        int movimentosX[] = {0, 1, 0, -1};
        int movimentosY[] = {1, 0, -1, 0};
        int movido = 0;

        for (int i = 0; i < 4; i++) {
            int novoX = x + movimentosX[i];
            int novoY = y + movimentosY[i];

            // Verifica se está dentro dos limites e não é uma parede
            if (novoX >= 0 && novoX < altura && novoY >= 0 && novoY < largura && labirinto[novoX][novoY] != 'M' && labirinto[novoX][novoY] != 'V') {
                // Se for uma chave, coletamos
                if (labirinto[novoX][novoY] == 'K') {
                    enfileira(&chaves, 1);
                    printf("Chave coletada em (%d, %d)\n", novoX, novoY);
                }
                // Se for uma porta, verificamos se há chaves disponíveis
                if (labirinto[novoX][novoY] == 'D') {
                    if (!filaVazia(&chaves)) {
                        desenfileira(&chaves);
                        printf("Porta aberta em (%d, %d)\n", novoX, novoY);
                    } else {
                        continue; // Não pode passar sem chave
                    }
                }
                
                // Move para a nova posição
                empilha(&movimentos, novoX, novoY);
                movido = 1;
                break;
            }
        }
        
        // Se não conseguimos mover, voltamos
        if (!movido) {
            desempilha(&movimentos, &x, &y);
            printf("Voltando para (%d, %d)\n", x, y);
        }
    }

    printf("Sem saída encontrada!\n");
}

int main() {
    explorarLabirinto(0, 0); // Começamos na posição (0,0)
    return 0;
}
