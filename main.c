#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Room {
    char* information;
    char* gun;
    char* score;
    struct Room* left;
    struct Room* right;
    struct Room* up;
    struct Room* down;
};

struct Gamer {
    struct Room* currentPosition;
    int totalScores;
    char* gunList;
};

struct Room* createRoom(char* information, char* gun, char* score) {
    struct Room* newRoom = (struct Room*)malloc(sizeof(struct Room));
    newRoom->information = strdup(information);
    newRoom->gun = gun ? strdup(gun) : NULL;
    newRoom->score = score ? strdup(score) : NULL;
    newRoom->left = NULL;
    newRoom->right = NULL;
    newRoom->up = NULL;
    newRoom->down = NULL;
    return newRoom;
}

void connectRooms(struct Room* room1, struct Room* room2, char direction) {
    switch (direction) {
        case 'U':
            room1->up = room2;
            room2->down = room1;
            break;
        case 'D':
            room1->down = room2;
            room2->up = room1;
            break;
        case 'L':
            room1->left = room2;
            room2->right = room1;
            break;
        case 'R':
            room1->right = room2;
            room2->left = room1;
            break;
        default:
            printf("Invalid direction!\n");
    }
}

void appendToGamerList(struct Gamer* gamer, struct Room* room) {
    if (room->gun) {
        if (gamer->gunList == NULL) {
            gamer->gunList = strdup(room->gun);
        } else {
            gamer->gunList = realloc(gamer->gunList, strlen(gamer->gunList) + strlen(room->gun) + 2);
            strcat(gamer->gunList, "\t");
            strcat(gamer->gunList, room->gun);
        }
    }
    if (room->score) {
        gamer->totalScores += atoi(room->score);
    }
}

void navigateRooms(struct Gamer* gamer) {
    if (!gamer || !gamer->currentPosition) {
        printf("Gamer or current position is not initialized!\n");
        return;
    }

    char command;
    while (true) {
        printf("\nYou are in the room: %s\n", gamer->currentPosition->information);
        printf("Your Current Total Score: %d\n", gamer->totalScores);
        printf("Navigate: (U)p, (D)own, (L)eft, (R)ight, (E)xit\n");
        scanf(" %c", &command);

        struct Room* nextRoom = NULL;
        switch (command) {
            case 'U': nextRoom = gamer->currentPosition->up; break;
            case 'D': nextRoom = gamer->currentPosition->down; break;
            case 'L': nextRoom = gamer->currentPosition->left; break;
            case 'R': nextRoom = gamer->currentPosition->right; break;
            case 'E':
                printf("Exiting navigation.\n");
                return;
            default:
                printf("Invalid command!\n");
                continue;
        }

        if (nextRoom) {
            appendToGamerList(gamer, nextRoom);
            gamer->currentPosition = nextRoom;
            printf("Moved to the next room: %s\n", nextRoom->information);
        } else {
            printf("No room in that direction!\n");
        }
    }
}

int main() {
    struct Room* startingRoom = createRoom("This is the Starting Room.", "Pistol", "10");
    struct Room* room1 = createRoom("This is the Room A.", "Shotgun", "20");
    struct Room* room2 = createRoom("This is the Room B.", "Keyblade", "15");
    struct Room* room3 = createRoom("This is the Room C.", "Rifle", "50");

    connectRooms(startingRoom, room1, 'R');
    connectRooms(startingRoom, room2, 'D');
    connectRooms(room1, room3, 'D');

    struct Gamer* gamer = (struct Gamer*)malloc(sizeof(struct Gamer));
    gamer->currentPosition = startingRoom;
    gamer->totalScores = 0;
    gamer->gunList = NULL;

    printf("Welcome to the Room Navigation System!\n");
    navigateRooms(gamer);

    free(gamer->gunList);
    free(gamer);

    return 0;
}
