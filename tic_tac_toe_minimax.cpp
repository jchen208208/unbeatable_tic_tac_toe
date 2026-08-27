#include <iostream>
#include <ctime>
#include <string>

void draw_board(char (&board)[9]);
void player_move(char (&board)[9], char player);
void computer_move(char (&board)[9], char computer);
char check_win(char (&board)[9]);
int minimax(char (&board)[9], char turn, int depth);
int one_turn(char (&board)[9], char player, char computer, int whose_turn);

static char board[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
static char player;
static char computer;
static int count = 0;

int main() {
    bool play_again = false;

    do {
        draw_board(board);
        std::cout << "Enter the symbol you want to play with (X / O): ";
        std::cin >> std::ws >> player;
        computer = player == 'O' ? 'X' : 'O';

        int turn;
        srand(time(0));
        turn = rand() % 2;

        while (one_turn(board, player, computer, turn) != 1) {
            turn++;
        }

        for (int i = 0; i < 9; i++) {
            board[i] = ' ';
        }
        count = 0;

        std::string yes_no;
        std::cout << "Play again? (yes/no) ";
        std::getline(std::cin >> std::ws, yes_no);
        play_again = yes_no == "yes" ? true : false;

    } while(play_again);
    

    return 0;
}

void draw_board(char (&board)[9]) {
    printf(" |%c|%c|%c| \n", board[0], board[1], board[2]);
    printf("---------\n");
    printf(" |%c|%c|%c| \n", board[3], board[4], board[5]);
    printf("---------\n");
    printf(" |%c|%c|%c| \n", board[6], board[7], board[8]);
}

void player_move(char (&board)[9], char player) {
    int spot;

    while (true) {
        std::cout << "Enter a spot to place (1 - 9): ";
        if (std::cin >> spot && spot >= 1 && spot <= 9) {
            if (board[spot - 1] == ' ') {
                board[spot - 1] = player;
                count++;
                break;
            }
            else {
                std::cout << "That spot is already filled. Please enter another spot: " << "\n";
            }
        }
        else {
            std::cout << "Invalid input!\n";
            std::cin.clear();  // if user entered a non-int, cin would throw an error so we want to clear that error status to not corrupt future inputs
            std::cin.ignore(1000, '\n');  // .clear() alone does not get rid fo the chars in the input buffer. 1000 is the max num of chars to discard and it will stop at the new line character for when the user pressed enter
        }
    }

}

void computer_move(char (&board)[9], char computer) {
    int best = -1000000;
    int best_slot = -1;

    for (int i = 0; i < 9; i++) {
        if (board[i] != ' ') {
            continue;
        }

        board[i] = computer;
        int new_best = minimax(board, player, 0);
        if (new_best > best) {
            best = new_best;
            best_slot = i;
        }
        board[i] = ' ';
    }

    board[best_slot] = computer;
    printf("Computer chose spot %i\n", best_slot + 1);
    count++;
}

char check_win(char (&board)[9]) {
    if (board[0] != ' ' && board[0] == board[1] && board[1] == board[2]) {
        return board[0];
    }
    if (board[3] != ' ' && board[3] == board[4] && board[4] == board[5]) {
        return board[3];
    }
    if (board[6] != ' ' && board[6] == board[7] && board[7] == board[8]) {
        return board[6];
    }
    if (board[0] != ' ' && board[0] == board[3] && board[3] == board[6]) {
        return board[0];
    }
    if (board[1] != ' ' && board[1] == board[4] && board[4] == board[7]) {
        return board[1];
    }
    if (board[2] != ' ' && board[2] == board[5] && board[5] == board[8]) {
        return board[2];
    }
    if (board[0] != ' ' && board[0] == board[4] && board[4] == board[8]) {
        return board[0];
    }
    if (board[2] != ' ' && board[2] == board[4] && board[4] == board[6]) {
        return board[2];
    }

    return 0;
}

int minimax(char (&board)[9], char turn, int depth) {
    char winner  = check_win(board);

    if (winner == computer) {
        return 10 - depth;
    }
    else if (winner == player) {
        return depth - 10;
    }
    else {
        int i;
        for (i = 0; i < 9; i++) {
            if (board[i] == ' ') {
                break;
            }
        }
        if (i == 9) {
            return 0;
        }
    }

    int best;  // best is the best possible outcome score (1 to 9 for computer win, -1 to -9 for player win, 0 for tie)

    if (turn == computer) {
        best = -1000000;
        int num_empty_space = 0;
        int empty_spaces[9] = {0};
        int counter = 0;
        for (int i = 0; i < 9; i++) {
            if (board[i] == ' ') {
                num_empty_space++;
                empty_spaces[counter] = i;
                counter++;
            }
        }

        for (int i = 0; i < num_empty_space; i++) {
            board[empty_spaces[i]] = computer;
            int new_best = minimax(board, player, depth + 1);
            if (new_best > best) {
                best = new_best;
            }
            board[empty_spaces[i]] = ' ';
        }
    }

    if (turn == player) {
        best = 1000000;
        int num_empty_space = 0;
        int empty_spaces[9] = {0};
        int counter = 0;
        for (int i = 0; i < 9; i++) {
            if (board[i] == ' ') {
                num_empty_space++;
                empty_spaces[counter] = i;
                counter++;
            }
        }

        for (int i = 0; i < num_empty_space; i++) {
            board[empty_spaces[i]] = player;
            int new_best = minimax(board, computer, depth + 1);
            if (new_best < best) {
                best = new_best;
            }
            board[empty_spaces[i]] = ' ';
        }
    }

    return best;
}

int one_turn(char (&board)[9], char player, char computer, int whose_turn) {
    char turn;

    if (whose_turn % 2 == 1) {
        turn = player;
        player_move(board, player);
    }

    else {
        turn = computer;
        computer_move(board, computer);
    }

    draw_board(board);
    if (check_win(board) == turn) {
        std::string msg = turn == computer ? "You lose!" : "You win!";
        std::cout << msg << "\n";
        return 1;
    }
    if (count == 9) {
        std::cout << "It's a tie!\n";
        return 1;
    }
    
    return 0;
}