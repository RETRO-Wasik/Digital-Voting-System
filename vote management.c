#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VOTERS 100
#define MAX_CANDIDATES 50

typedef struct {
    char name[50];
    int nid;
    char city[50];
    int hasVoted;
} Voter;

typedef struct {
    char name[50];
    int uid;
    char city[50];
    char symbol[10];
    char party[50];
    int voteCount;
} Candidate;

Voter voters[MAX_VOTERS];
Candidate candidates[MAX_CANDIDATES];
int voterCount = 0, candidateCount = 0;

#define ADMIN_NAME "WASIK"
#define ADMIN_PASS "1122"

void pauseScreen() {
    printf("\nPress Enter to continue...");
    getchar();
}

void clearScreen() {
    system("cls");
}

void loadData() {
    FILE *vf = fopen("voters.txt", "r");
    if (vf != NULL) {
        while (fscanf(vf, " %[^\n]\n%d\n%[^\n]\n%d\n", voters[voterCount].name, &voters[voterCount].nid, voters[voterCount].city, &voters[voterCount].hasVoted) == 4) {
            voterCount++;
        }
        fclose(vf);
    }

    FILE *cf = fopen("candidates.txt", "r");
    if (cf != NULL) {
        while (fscanf(cf, " %[^\n]\n%d\n%[^\n]\n%[^\n]\n%[^\n]\n%d\n", candidates[candidateCount].name, &candidates[candidateCount].uid, candidates[candidateCount].city, candidates[candidateCount].symbol, candidates[candidateCount].party, &candidates[candidateCount].voteCount) == 6) {
            candidateCount++;
        }
        fclose(cf);
    }
}

int authenticateAdmin() {
    char name[50], pass[50];
    clearScreen();
    printf("========== ADMIN LOGIN ==========\n");
    printf("Enter admin username: ");
    scanf(" %[^\n]", name);
    printf("Enter admin password: ");
    scanf(" %[^\n]", pass);

    if (strcmp(name, ADMIN_NAME) == 0 && strcmp(pass, ADMIN_PASS) == 0) {
        printf("Login successful! Welcome, Admin.\n");
        pauseScreen();
        return 1;
    } else {
        printf("Invalid credentials. Access denied.\n");
        pauseScreen();
        return 0;
    }
}

void registerVoter() {
    clearScreen();
    printf("======== REGISTER VOTER ========\n");

    if (voterCount >= MAX_VOTERS) {
        printf("Voter limit reached!\n");
        pauseScreen();
        return;
    }

    Voter v;
    printf("Enter voter's name: ");
    scanf(" %[^\n]", v.name);
    printf("Enter voter's NID (last 4 digits): ");
    scanf("%d", &v.nid);
    printf("Enter voter's division: ");
    scanf(" %[^\n]", v.city);
    v.hasVoted = 0;

    for (int i = 0; i < voterCount; i++) {
        if (voters[i].nid == v.nid) {
            printf("This voter is already registered!\n");
            pauseScreen();
            return;
        }
    }

    voters[voterCount++] = v;
    printf("Voter registered successfully!\n");
    pauseScreen();
}

void registerCandidate() {
    clearScreen();
    printf("======= REGISTER CANDIDATE =======\n");

    if (candidateCount >= MAX_CANDIDATES) {
        printf("Candidate limit reached!\n");
        pauseScreen();
        return;
    }

    Candidate c;
    printf("Enter candidate's name: ");
    scanf(" %[^\n]", c.name);
    printf("Enter candidate's UID: ");
    scanf("%d", &c.uid);
    printf("Enter candidate's division: ");
    scanf(" %[^\n]", c.city);
    printf("Enter candidate's symbol: ");
    scanf(" %[^\n]", c.symbol);
    printf("Enter candidate's party name: ");
    scanf(" %[^\n]", c.party);
    c.voteCount = 0;

    for (int i = 0; i < candidateCount; i++) {
        if (candidates[i].uid == c.uid) {
            printf("Candidate UID already exists!\n");
            pauseScreen();
            return;
        }
    }

    candidates[candidateCount++] = c;
    printf("Candidate registered successfully!\n");
    pauseScreen();
}

void castVote() {
    clearScreen();
    printf("========= CAST VOTE =========\n");

    int nid;
    printf("Enter your NID to vote: ");
    scanf("%d", &nid);

    int voterIndex = -1;
    for (int i = 0; i < voterCount; i++) {
        if (voters[i].nid == nid) {
            voterIndex = i;
            break;
        }
    }

    if (voterIndex == -1) {
        printf("Voter not registered!\n");
        pauseScreen();
        return;
    }

    if (voters[voterIndex].hasVoted) {
        printf("You have already voted!\n");
        pauseScreen();
        return;
    }

    printf("\nVoter: %s | Division: %s\n", voters[voterIndex].name, voters[voterIndex].city);
    printf("Candidates in your division:\n");

    int found = 0;
    for (int i = 0; i < candidateCount; i++) {
        if (strcmp(candidates[i].city, voters[voterIndex].city) == 0) {
            printf("UID: %d | Name: %s | Symbol: %s | Party: %s\n",
                   candidates[i].uid, candidates[i].name, candidates[i].symbol, candidates[i].party);
            found = 1;
        }
    }

    if (!found) {
        printf("No candidates found in your division.\n");
        pauseScreen();
        return;
    }

    int uid;
    printf("Enter candidate UID to vote: ");
    scanf("%d", &uid);

    for (int i = 0; i < candidateCount; i++) {
        if (candidates[i].uid == uid &&
            strcmp(candidates[i].city, voters[voterIndex].city) == 0) {
            candidates[i].voteCount++;
            voters[voterIndex].hasVoted = 1;
            printf("Vote casted successfully!\n");
            pauseScreen();
            return;
        }
    }

    printf("Invalid candidate UID.\n");
    pauseScreen();
}

void showResults() {
    clearScreen();
    printf("========= ELECTION RESULTS =========\n");

    if (candidateCount == 0) {
        printf("No candidates registered.\n");
        pauseScreen();
        return;
    }

    printf("%-20s %-6s %-10s %-15s %-8s %-15s\n",
           "Candidate Name", "UID", "Symbol", "Party", "Votes", "Division");
    printf("----------------------------------------------------------------------------\n");

    for (int i = 0; i < candidateCount; i++) {
        printf("%-20s %-6d %-10s %-15s %-8d %-15s\n",
               candidates[i].name, candidates[i].uid, candidates[i].symbol,
               candidates[i].party, candidates[i].voteCount, candidates[i].city);
    }

    pauseScreen();
}

void rulingParty() {
    clearScreen();
    printf("========= RULING PARTY =========\n");

    int maxVotes = 0;
    char ruling[50] = "";

    for (int i = 0; i < candidateCount; i++) {
        int partyVotes = 0;
        for (int j = 0; j < candidateCount; j++) {
            if (strcmp(candidates[i].party, candidates[j].party) == 0) {
                partyVotes += candidates[j].voteCount;
            }
        }

        if (partyVotes > maxVotes) {
            maxVotes = partyVotes;
            strcpy(ruling, candidates[i].party);
        }
    }

    if (strlen(ruling) > 0)
        printf("Ruling Party: %s (Total Votes: %d)\n", ruling, maxVotes);
    else
        printf("No ruling party determined yet.\n");

    pauseScreen();
}

void showCandidateData() {
    clearScreen();
    printf("======== CANDIDATE DATA ========\n");

    if (candidateCount == 0) {
        printf("No candidates registered yet.\n");
        pauseScreen();
        return;
    }

    printf("%-20s %-6s %-10s %-10s %-15s %-8s\n", "Name", "UID", "Division", "Symbol", "Party", "Votes");
    printf("------------------------------------------------------------------------\n");
    for (int i = 0; i < candidateCount; i++) {
        printf("%-20s %-6d %-10s %-10s %-15s %-8d\n", candidates[i].name, candidates[i].uid, candidates[i].city, candidates[i].symbol, candidates[i].party, candidates[i].voteCount);
    }

    pauseScreen();
}

void showVoterData() {
    clearScreen();
    printf("======== VOTER DATA ========\n");

    if (voterCount == 0) {
        printf("No voters registered yet.\n");
        pauseScreen();
        return;
    }

    printf("%-20s %-10s %-15s %-10s\n", "Name", "NID", "Division", "Voted");
    printf("-------------------------------------------------------------\n");
    for (int i = 0; i < voterCount; i++) {
        printf("%-20s %-10d %-15s %-10s\n", voters[i].name, voters[i].nid, voters[i].city, voters[i].hasVoted ? "Yes" : "No");
    }

    pauseScreen();
}

int main() {
    int choice;
    loadData();

    if (!authenticateAdmin())
        return 0;

    do {
        clearScreen();
        printf("======== VOTE MANAGEMENT SYSTEM ========\n");
        printf("1. Register Voter\n");
        printf("2. Register Candidate\n");
        printf("3. Cast Vote\n");
        printf("4. Show Voter Data\n");
        printf("5. Show Candidate Data\n");
        printf("6. Show Results\n");
        printf("7. Show Ruling Party\n");
        printf("8. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar();

        switch(choice) {
            case 1: registerVoter(); break;
            case 2: registerCandidate(); break;
            case 3: castVote(); break;
            case 4: showVoterData(); break;
            case 5: showCandidateData(); break;
            case 6: showResults(); break;
            case 7: rulingParty(); break;
            case 8:
                clearScreen();
                printf("Exiting system. Thank you!\n");
                break;
            default:
                printf("Invalid option. Try again.\n");
                pauseScreen();
        }
    } while (choice != 8);

    return 0;
}
