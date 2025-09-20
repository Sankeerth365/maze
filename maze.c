#include <stdio.h>
#include <stdlib.h>
#include "maze.h"
/*
*Partner: anara32
*Intro Paragraph: This program implements a maze-solving system using a depth first search. It 
* defines a maze_t structure to store the grid, dimensions, start/end positions>
*The createMaze functions reads a maze from a file, dynamically allocates memory and identifies the start and end positions
*DestroyMaze ensures proper memory deallocation. The printMaze function displays the maze in readable format. 
*solveMazeDFs, employs a recursive DFS approach, exploring all four directions (right, down,left, up) to find 
* a path from 'S' to 'E'. It marks explored paths with * and backtracked paths with ~, providing a visual representation of the solution.
*This approach efficiently finds a valid path in a maze while properly handling memory
*/
/*
 * createMaze -- Creates and fills a maze structure from the given file
 * INPUTS:       fileName - character array containing the name of the maze file
 * OUTPUTS:      None 
 * RETURN:       A filled maze structure that represents the contents of the input file
 * SIDE EFFECTS: Allocates memory for the maze structure and its cells
 */
maze_t * createMaze(char * fileName)
{
    FILE *fp = fopen(fileName, "r");
    if (fp == NULL) {
        return NULL;
    }

    // Allocate maze structure
    maze_t *maze = (maze_t *)malloc(sizeof(maze_t));
    if (maze == NULL) {
        fclose(fp);
        return NULL;
    }

    // Read width and height from file
    if (fscanf(fp, "%d %d\n", &maze->width, &maze->height) != 2) {
        free(maze);
        fclose(fp);
        return NULL;
    }

    // Allocate memory for cell rows
    maze->cells = (char **)malloc(maze->height * sizeof(char *));
    if (maze->cells == NULL) {
        free(maze);
        fclose(fp);
        return NULL;
    }

    // Initialize cells and find start/end positions
    for (int i = 0; i < maze->height; i++) {
        maze->cells[i] = (char *)malloc(maze->width * sizeof(char));
        if (maze->cells[i] == NULL) {
            // Clean up previously allocated rows
            for (int j = 0; j < i; j++) {
                free(maze->cells[j]);
            }
            free(maze->cells);
            free(maze);
            fclose(fp);
            return NULL;
        }

        // Read each row of the maze
        for (int j = 0; j < maze->width; j++) {
            maze->cells[i][j] = fgetc(fp);
            
            // Check for start position
            if (maze->cells[i][j] == 'S') {
                maze->startColumn = j;
                maze->startRow = i;
            }
            // Check for end position
            else if (maze->cells[i][j] == 'E') {
                maze->endColumn = j;
                maze->endRow = i;
            }
        }
        // Skip newline character
        fgetc(fp);
    }

    fclose(fp);
    return maze;
}

/*
 * destroyMaze -- Frees all memory associated with the maze structure
 * INPUTS:        maze -- pointer to maze structure to be freed
 * OUTPUTS:       None
 * RETURN:        None
 * SIDE EFFECTS:  All memory that has been allocated for the maze is freed
 */
void destroyMaze(maze_t * maze)
{
    if (maze == NULL) return;

    // Free each row of cells
    for (int i = 0; i < maze->height; i++) {
        free(maze->cells[i]);
    }

    // Free the array of row pointers
    free(maze->cells);

    // Free the maze structure itself
    free(maze);
}

/*
 * printMaze --  Prints out the maze in a human readable format
 * INPUTS:       maze -- pointer to maze structure to print
 * OUTPUTS:      None
 * RETURN:       None
 * SIDE EFFECTS: Prints the maze to the console
 */
void printMaze(maze_t * maze)
{
    if (maze == NULL) return;

    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            printf("%c", maze->cells[i][j]);
        }
        printf("\n");
    }
}

/*
 * solveMazeDFS -- Recursively solves the maze using depth first search
 * INPUTS:        maze -- pointer to maze structure
 *                col -- current column position
 *                row -- current row position
 * OUTPUTS:       Marks cells as part of solution path or visited
 * RETURN:        1 if maze is solved, 0 if unsolvable
 * SIDE EFFECTS:  Modifies maze cells to show solution path and visited cells
 */
 int solveMazeDFS(maze_t * maze, int col, int row) {
    // Check bounds
    if (col < 0 || col >= maze->width || row < 0 || row >= maze->height) {
        return 0;
    }

    // Get current cell value
    char current = maze->cells[row][col];

    // Check if we've reached the end
    if (current == 'E') {
        return 1;
    }

    // Check if current cell is valid (not wall or already part of path/visited)
    if (current != ' ' && current != 'S') {
        return 0;
    }

    // Mark as part of potential solution path (except start position)
    if (current != 'S') {
        maze->cells[row][col] = '*';
    }

    // Try all four directions in optimized order
    // Try moving right first (often more efficient for left-to-right mazes)
    if (solveMazeDFS(maze, col + 1, row) ||  // right
        solveMazeDFS(maze, col, row + 1) ||  // down
        solveMazeDFS(maze, col - 1, row) ||  // left
        solveMazeDFS(maze, col, row - 1)) {  // up
        return 1;
    }

    // If no direction worked, backtrack and mark as visited
    if (current != 'S') {
        maze->cells[row][col] = '~';
    }

    return 0;
}



