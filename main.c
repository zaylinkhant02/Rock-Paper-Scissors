#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROCK 1
#define PAPER 2
#define SCISSORS 3
#define MAX_HISTORY 5

typedef struct {
    int userChoice;
    int computerChoice;
    int outcome;
} MatchRecord;

typedef struct {
    MatchRecord records[MAX_HISTORY];
    int count;
    int index;
} MatchHistory;

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
        printf("4. Get Score & History\n");
        printf("5. Reset Statistics\n");
        printf("6. Quit\n");
        printf("Enter your choice (1-6): ");

        result = scanf("%d", &choice);

        if (result == EOF) {
            return 6;
        }

        if (result != 1) {
            printf("Invalid input. Please enter an integer from 1 to 6.\n");
            clearInputBuffer();
            continue;
        }

        clearInputBuffer();

        if (choice >= 1 && choice <= 6) {
            return choice;
        }

        printf("Invalid choice! Number must be between 1 and 6.\n");
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

int getResult(int userChoice, int computerChoice, int *w, int *l, int *t, int *streak, int *maxStreak) {
    if (userChoice == computerChoice) {
        printf("\nIt's a tie.\n");
        (*t)++;
        *streak = 0;
        return 0;
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
        return 1;
    }
    else {
        printf("\nYou lose. Try again!\n");
        (*l)++;
        *streak = 0;
        return 2;
    }
}

void addRecord(MatchHistory *mh, int userChoice, int computerChoice, int outcome) {
    mh->records[mh->index].userChoice = userChoice;
    mh->records[mh->index].computerChoice = computerChoice;
    mh->records[mh->index].outcome = outcome;
    mh->index = (mh->index + 1) % MAX_HISTORY;
    if (mh->count < MAX_HISTORY) {
        mh->count++;
    }
}

void showScore(int *w, int *l, int *t, int *maxStreak, const MatchHistory *mh) {
    int totalGames = *w + *l + *t;
    double winRate = (totalGames > 0) ? ((double)*w / totalGames) * 100.0 : 0.0;
    const char *options[] = {"", "Rock", "Paper", "Scissors"};
    const char *results[] = {"Tie", "Win", "Loss"};

    printf("\nCurrent Score:\n");
    printf("Wins: %d | Losses: %d | Ties: %d\n", *w, *l, *t);
    printf("Win Rate: %.1f%%\n", winRate);
    printf("Highest Win Streak: %d\n\n", *maxStreak);

    printf("Recent Matches (Last %d):\n", mh->count);
    if (mh->count == 0) {
        printf("No matches played yet.\n\n");
    } else {
        int start = (mh->index - mh->count + MAX_HISTORY) % MAX_HISTORY;
        for (int i = 0; i < mh->count; i++) {
            int pos = (start + i) % MAX_HISTORY;
            printf("%d. You: %s | CPU: %s | Result: %s\n",
                   i + 1,
                   options[mh->records[pos].userChoice],
                   options[mh->records[pos].computerChoice],
                   results[mh->records[pos].outcome]);
        }
        printf("\n");
    }
}

void resetStats(int *w, int *l, int *t, int *streak, int *maxStreak, int history[3], int *lastUserMove, MatchHistory *mh) {
    *w = 0;
    *l = 0;
    *t = 0;
    *streak = 0;
    *maxStreak = 0;
    *lastUserMove = 0;
    history[0] = 0;
    history[1] = 0;
    history[2] = 0;
    mh->count = 0;
    mh->index = 0;
    printf("\nStatistics and history have been reset.\n");
}

int game(int *w, int *l, int *t, int *streak, int *maxStreak, int difficulty, int history[3], int *lastUserMove, MatchHistory *mh) {
    int userChoice = getUserChoice();

    if (userChoice <= 3) {
        history[userChoice - 1]++;
        int computerChoice = getComputerChoice(difficulty, history, *lastUserMove);
        *lastUserMove = userChoice;

        showChoices(userChoice, computerChoice);
        int outcome = getResult(userChoice, computerChoice, w, l, t, streak, maxStreak);
        addRecord(mh, userChoice, computerChoice, outcome);
    }
    else if (userChoice == 4) {
        showScore(w, l, t, maxStreak, mh);
    }
    else if (userChoice == 5) {
        resetStats(w, l, t, streak, maxStreak, history, lastUserMove, mh);
    }
    else if (userChoice == 6) {
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
    MatchHistory matchHistory = { .count = 0, .index = 0 };

    srand((unsigned int)time(NULL));

    printf("=========================\n");
    printf(" Rock Paper Scissors Game\n");
    printf("=========================\n");

    int difficulty = selectDifficulty();

    do {
        stop = game(&wins, &losses, &ties, &streak, &maxStreak, difficulty, history, &lastUserMove, &matchHistory);
    } while (stop == 0);

    printf("\nFinal Totals:");
    showScore(&wins, &losses, &ties, &maxStreak, &matchHistory);
    printf("The game has stopped running. Goodbye!\n");

    return 0;
}