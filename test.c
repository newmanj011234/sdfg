#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LOCATIONS 100

// Structure to store information about each bait placement
struct BaitPlacement {
    char location[20];          // Location name
    int identifier;             // Identifier for the bait placement
    int foodLevel;              // Current food level
    int lastFoodLevel;          // Last recorded food level
    struct BaitPlacement *next; // Pointer to next placement in the list
};

// Structure to store cumulative food taken for each location
struct CumulativeFood {
    char location[20];  // Location name
    int totalFoodTaken; // Cumulative food taken
};

struct CumulativeFood cumulativeFoods[MAX_LOCATIONS];
int numLocations = 0;

// Function prototypes
struct BaitPlacement *findOrCreatePlacement(struct BaitPlacement **placements, char *location, int identifier);
void updateFoodTaken(struct BaitPlacement *placement, int foodLevel);
void printCumulativeFood(struct BaitPlacement *placements[], int count);

int main() {
    struct BaitPlacement *placements[MAX_LOCATIONS] = {0}; // Array of pointers to bait placements
    char location[20];
    int identifier, foodLevel;
    int count = 0;

    while (scanf("%s %d %d", location, &identifier, &foodLevel) == 3) {
        if (strcmp(location, "END") == 0 && identifier == -1 && foodLevel == -1) {
        break; // Termination condition
        }

        struct BaitPlacement *placement = findOrCreatePlacement(placements, location, identifier);
        updateFoodTaken(placement, foodLevel);
    }

    printCumulativeFood(placements, count);

    // Free allocated memory
    for (int i = 0; i < count; ++i) {
        struct BaitPlacement *current = placements[i];
    while (current != NULL) {
      struct BaitPlacement *temp = current;
      current = current->next;
      free(temp);
    }
    }

    return 0;
}

// Finds an existing placement or creates a new one if not found
struct BaitPlacement *findOrCreatePlacement(struct BaitPlacement **placements, char *location, int identifier) {
    for (int i = 0; i < MAX_LOCATIONS; i++) {
        struct BaitPlacement *current = placements[i];
        while (current != NULL) {
            if (strcmp(current->location, location) == 0 && current->identifier == identifier) {
                return current; // Found existing placement
            }
        current = current->next;
        }
    }

    // Create new placement
    struct BaitPlacement *newPlacement = (struct BaitPlacement *)malloc(sizeof(struct BaitPlacement));
    strcpy(newPlacement->location, location);
    newPlacement->identifier = identifier;
    newPlacement->foodLevel = -1; // Initialize with invalid value
    newPlacement->lastFoodLevel = -1;
    newPlacement->next = NULL;

    // Find a non-occupied slot for the new placement
    for (int i = 0; i < MAX_LOCATIONS; i++) {
        if (placements[i] == NULL) {
            placements[i] = newPlacement;
            break;
        }
    }

    return newPlacement;
}

// Updates the food taken for a placement and prints the information
void updateFoodTaken(struct BaitPlacement *placement, int foodLevel) {
    if (placement->lastFoodLevel == -1) {
        printf("New placement.\n");
    }
    else {
        int foodTaken = placement->lastFoodLevel - foodLevel;
        if (foodTaken < 0) {
            // If food level increased, assume it's a refill, and no food was taken
            foodTaken = 0;
        }
        printf("%d\n", foodTaken);

        // Update the cumulative total for the location
        int found = 0;
        for (int i = 0; i < numLocations; i++) {
            if (strcmp(cumulativeFoods[i].location, placement->location) == 0) {
                cumulativeFoods[i].totalFoodTaken += foodTaken;
                found = 1;
            break;
            }
        }
        if (!found) {
            strcpy(cumulativeFoods[numLocations].location, placement->location);
            cumulativeFoods[numLocations].totalFoodTaken = foodTaken;
            numLocations++;
        }
    }
    placement->lastFoodLevel =
        foodLevel; // Update last food level regardless of the condition
}

// Prints the cumulative food taken for each location
void printCumulativeFood() {
    for (int i = 0; i < numLocations; i++) {
        printf(" %s %d\n", cumulativeFoods[i].location, cumulativeFoods[i].totalFoodTaken);
    }
}
