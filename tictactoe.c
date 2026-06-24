#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char board[3][3];
int scores[2] = {0, 0}; // [Player, Computer]
int draws = 0;

void init_board() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = '1' + i * 3 + j;
}

void print_board() {
    printf("\n");
    printf("  +-------+-------+-------+\n");
    for (int i = 0; i < 3; i++) {
        printf("  |       |       |       |\n");
        printf("  |   %c   |   %c   |   %c   |\n",
               board[i][0], board[i][1], board[i][2]);
        printf("  |       |       |       |\n");
        printf("  +-------+-------+-------+\n");
    }
    printf("\n");
}

void print_scoreboard(char *p1) {
    printf("  📊 SCORE — %s: %d  |  Computer: %d  |  Draws: %d\n",
           p1, scores[0], scores[1], draws);
    printf("  ============================================\n");
}

int check_winner(char mark) {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == mark && board[i][1] == mark && board[i][2] == mark) return 1;
        if (board[0][i] == mark && board[1][i] == mark && board[2][i] == mark) return 1;
    }
    if (board[0][0] == mark && board[1][1] == mark && board[2][2] == mark) return 1;
    if (board[0][2] == mark && board[1][1] == mark && board[2][0] == mark) return 1;
    return 0;
}

int is_full() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] != 'X' && board[i][j] != 'O') return 0;
    return 1;
}

// Minimax AI
int minimax(int is_maximizing) {
    if (check_winner('O')) return +10;
    if (check_winner('X')) return -10;
    if (is_full()) return 0;

    if (is_maximizing) {
        int best = -1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] != 'X' && board[i][j] != 'O') {
                    char tmp = board[i][j];
                    board[i][j] = 'O';
                    int score = minimax(0);
                    board[i][j] = tmp;
                    if (score > best) best = score;
                }
            }
        }
        return best;
    } else {
        int best = 1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] != 'X' && board[i][j] != 'O') {
                    char tmp = board[i][j];
                    board[i][j] = 'X';
                    int score = minimax(1);
                    board[i][j] = tmp;
                    if (score < best) best = score;
                }
            }
        }
        return best;
    }
}

void computer_move() {
    int best = -1000, bi = -1, bj = -1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != 'X' && board[i][j] != 'O') {
                char tmp = board[i][j];
                board[i][j] = 'O';
                int score = minimax(0);
                board[i][j] = tmp;
                if (score > best) { best = score; bi = i; bj = j; }
            }
        }
    }
    board[bi][bj] = 'O';
    printf("  🤖 Computer played at position %d\n", bi * 3 + bj + 1);
}

void play_vs_computer(char *player_name) {
    int choice;
    init_board();
    printf("\n  📌 You are X, Computer is O\n");
    printf("  Positions: 1-9 (left to right, top to bottom)\n");

    while (1) {
        print_board();

        // Player turn
        printf("  Your turn (%s), enter position (1-9): ", player_name);
        scanf("%d", &choice);
        if (choice < 1 || choice > 9) { printf("  ❌ Invalid! Enter 1-9\n"); continue; }

        int row = (choice - 1) / 3, col = (choice - 1) % 3;
        if (board[row][col] == 'X' || board[row][col] == 'O') {
            printf("  ❌ Cell already taken!\n"); continue;
        }
        board[row][col] = 'X';

        if (check_winner('X')) {
            print_board();
            printf("  🎉 %s WINS!\n", player_name);
            scores[0]++;
            return;
        }
        if (is_full()) { print_board(); printf("  🤝 It's a DRAW!\n"); draws++; return; }

        // Computer turn
        printf("  🤖 Computer is thinking...\n");
        computer_move();

        if (check_winner('O')) {
            print_board();
            printf("  🤖 Computer WINS! Better luck next time!\n");
            scores[1]++;
            return;
        }
        if (is_full()) { print_board(); printf("  🤝 It's a DRAW!\n"); draws++; return; }
    }
}

void play_two_players(char *p1, char *p2) {
    int choice, current = 0;
    char marks[2] = {'X', 'O'};
    char *names[2] = {p1, p2};
    init_board();

    while (1) {
        print_board();
        printf("  %s's turn (%c), enter position (1-9): ", names[current], marks[current]);
        scanf("%d", &choice);
        if (choice < 1 || choice > 9) { printf("  ❌ Invalid! Enter 1-9\n"); continue; }

        int row = (choice - 1) / 3, col = (choice - 1) % 3;
        if (board[row][col] == 'X' || board[row][col] == 'O') {
            printf("  ❌ Cell already taken!\n"); continue;
        }
        board[row][col] = marks[current];

        if (check_winner(marks[current])) {
            print_board();
            printf("  🎉 %s WINS!\n", names[current]);
            if (current == 0) scores[0]++; else scores[1]++;
            return;
        }
        if (is_full()) { print_board(); printf("  🤝 It's a DRAW!\n"); draws++; return; }
        current = 1 - current;
    }
}

int main() {
    int mode, play_again;
    char p1[50], p2[50];

    system("clear || cls");
    printf("  ╔══════════════════════════════════╗\n");
    printf("  ║      ❌ TIC-TAC-TOE GAME ⭕      ║\n");
    printf("  ╚══════════════════════════════════╝\n\n");

    printf("  Select Mode:\n");
    printf("  1. Player vs Computer (AI)\n");
    printf("  2. Player vs Player\n");
    printf("  Choice: ");
    scanf("%d", &mode);

    printf("\n  Enter Player 1 Name: "); scanf("%s", p1);
    if (mode == 2) { printf("  Enter Player 2 Name: "); scanf("%s", p2); }
    else strcpy(p2, "Computer");

    do {
        system("clear || cls");
        printf("  ╔══════════════════════════════════╗\n");
        printf("  ║      ❌ TIC-TAC-TOE GAME ⭕      ║\n");
        printf("  ╚══════════════════════════════════╝\n");
        print_scoreboard(p1);

        if (mode == 1) play_vs_computer(p1);
        else play_two_players(p1, p2);

        printf("\n  Play again? (1=Yes / 0=No): ");
        scanf("%d", &play_again);
    } while (play_again == 1);

    printf("\n  Final Scores — %s: %d | %s: %d | Draws: %d\n",
           p1, scores[0], p2, scores[1], draws);
    printf("  👋 Thanks for playing!\n\n");
    return 0;
}
