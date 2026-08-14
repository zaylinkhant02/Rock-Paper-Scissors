#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROCK 1
#define PAPER 2
#define SCISSORS 3

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int selectDifficulty() {
    int difficulty = 0;
    int result;

    while (1) {
        printf("\nSelect Difficulty Level:\n");
        printf("1. Easy (Pure Random)\n");
        printf("2. Medium (Mixed Counter AI)\n");
        printf("3. Hard (Pattern & Frequency AI)\n");
        printf("Enter choice (1-3): ");

        result = scanf("%d", &difficulty);

        if (result == EOF) {
            return 1;
        }

        if (result != 1) {
            printf("Invalid input. Please enter a number between 1 and 3.\n");
            clearInputBuffer();
            continue;
        }

        clearInputBuffer();

        if (difficulty >= 1 && difficulty <= 3) {
            return difficulty;
        }

        printf("Invalid selection. Choice out of range.\n");
    }
}

int getUserChoice() {
    int choice = 0;
    int result;

    while (1) {
        printf("\nAll options:\n");
        printf("1. Rock\n");
        printf("2. Paper\n");
        printf("3. Scissors\n");
        printf("4. Get Score\n");
        printf("5. Quit\n");
        printf("Enter your choice (1-5): ");

        result = scanf("%d", &choice);

        if (result == EOF) {
            return 5;
        }

        if (result != 1) {
            printf("Invalid input. Please enter an integer from 1 to 5.\n");
            clearInputBuffer();
            continue;
        }

        clearInputBuffer();

        if (choice >= 1 && choice <= 5) {
            return choice;
        }

        printf("Invalid choice! Number must be between 1 and 5.\n");
    }
}

int getComputerChoice(int difficulty, const int history[3], int lastUserMove) {
    if (difficulty == 1) {
        return (rand() % 3) + 1;
    }

    if (difficulty == 2) {
        if (lastUserMove >= 1 && lastUserMove <= 3 && (rand() % 2 == 0)) {
            return (lastUserMove % 3) + 1;
        }
        return (rand() % 3) + 1;
    }

    int mostPlayedIndex = 0;
    for (int i = 1; i < 3; i++) {
        if (history[i] > history[mostPlayedIndex]) {
            mostPlayedIndex = i;
        }
    }

    int predictedMove = mostPlayedIndex + 1;

    if ((rand() % 100) < 80) {
        return (predictedMove % 3) + 1;
    }

    return (rand() % 3) + 1;
}

void showChoices(int userChoice, int computerChoice) {
    const char *options[] = {"", "Rock", "Paper", "Scissors"};
    printf("\nYou picked: %s.", options[userChoice]);
    printf("\nThe computer picked: %s.\n", options[computerChoice]);
}

void getResult(int userChoice, int computerChoice, int *w, int *l, int *t, int *streak, int *maxStreak) {
    if (userChoice == computerChoice) {
        printf("\nIt's a tie.\n");
        (*t)++;
        *streak = 0;
    }
    else if ((userChoice == ROCK && computerChoice == SCISSORS) || 
             (userChoice == PAPER && computerChoice == ROCK) || 
             (userChoice == SCISSORS && computerChoice == PAPER)) {
        printf("\nYou win!\n");
        (*w)++;
        (*streak)++;
        if (*streak > *maxStreak) {
            *maxStreak = *streak;
        }
    }
    else {
        printf("\nYou lose. Try again!\n");
        (*l)++;
        *streak = 0;
    }
}

void showScore(int *w, int *l, int *t, int *maxStreak) {
    int totalGames = *w + *l + *t;
    double winRate = (totalGames > 0) ? ((double)*w / totalGames) * 100.0 : 0.0;

    printf("\nCurrent Score:\n");
    printf("Wins: %d | Losses: %d | Ties: %d\n", *w, *l, *t);
    printf("Win Rate: %.1f%%\n", winRate);
    printf("Highest Win Streak: %d\n\n", *maxStreak);
}

int game(int *w, int *l, int *t, int *streak, int *maxStreak, int difficulty, int history[3], int *lastUserMove) {
    int userChoice = getUserChoice();

    if (userChoice <= 3) {
        history[userChoice - 1]++;
        int computerChoice = getComputerChoice(difficulty, history, *lastUserMove);
        *lastUserMove = userChoice;

        showChoices(userChoice, computerChoice);
        getResult(userChoice, computerChoice, w, l, t, streak, maxStreak);
    }
    else if (userChoice == 4) {
        showScore(w, l, t, maxStreak);
    }
    else if (userChoice == 5) {
        return 1;
    }

    return 0;
}

int main() {
    int wins = 0, losses = 0, ties = 0;
    int streak = 0, maxStreak = 0;
    int history[3] = {0, 0, 0};
    int lastUserMove = 0;
    int stop = 0;

    srand((unsigned int)time(NULL));

    printf("=========================\n");
    printf(" Rock Paper Scissors Game\n");
    printf("=========================\n");

    int difficulty = selectDifficulty();

    do {
        stop = game(&wins, &losses, &ties, &streak, &maxStreak, difficulty, history, &lastUserMove);
    } while (stop == 0);

    printf("\nFinal Totals:");
    showScore(&wins, &losses, &ties, &maxStreak);
    printf("The game has stopped running. Goodbye!\n");

    return 0;
}