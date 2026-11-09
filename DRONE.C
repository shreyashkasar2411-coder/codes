
/*
 PROJECT: COORDINATE BASED DRONE SIMULATOR
 Based on C Programming Assignments 1 to 9
*/

#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <string.h>

#define GRID 10
#define MAXPOINTS 5
#define SENSORS 5

struct Sensor
{
    int id;
    int reading;
    int status;
};

int absoluteValue(int number)
{
    if (number < 0)
        return -number;
    return number;
}

float calculateAverage(int readings[], int count)
{
    int i, total = 0;

    for (i = 0; i < count; i++)
        total = total + readings[i];

    return (float)total / count;
}

void displayPWM(int duty)
{
    int i;

    printf("\nPWM SIGNAL: ");

    for (i = 1; i <= 10; i++)
    {
        if (i <= duty)
            printf("1");
        else
            printf("0");
    }

    printf("\n");
}

void displayMap(char map[GRID+1][GRID+1],
                int droneX, int droneY, int showDrone)
{
    int row, col;

    printf("\n          DRONE PATH MAP\n\n");

    for (row = GRID; row >= 1; row--)
    {
        printf("%2d | ", row);

        for (col = 1; col <= GRID; col++)
        {
            if (showDrone && col == droneX &&
                row == droneY)
                printf(" D ");
            else
                printf(" %c ", map[row][col]);
        }

        printf("\n");
    }

    printf("   ");

    for (col = 1; col <= GRID; col++)
        printf("---");

    printf("\n    ");

    for (col = 1; col <= GRID; col++)
        printf("%2d ", col);

    printf("\n\n");
    printf("S=START W=WAYPOINT F=FINISH\n");
    printf("*=PATH D=DRONE\n");
}

void showDroneStep(char map[GRID+1][GRID+1],
                   int x, int y, int tx, int ty,
                   int steps, int battery, char direction)
{
    clrscr();

    printf("====================================\n");
    printf("       DRONE PATH SIMULATOR\n");
    printf("====================================\n");

    printf("STATUS      : FLYING\n");
    printf("POSITION    : (%d,%d)\n", x, y);
    printf("TARGET      : (%d,%d)\n", tx, ty);
    printf("DIRECTION   : ");

    if (direction == 'R')
        printf("RIGHT\n");
    else if (direction == 'L')
        printf("LEFT\n");
    else if (direction == 'U')
        printf("UP\n");
    else
        printf("DOWN\n");

    printf("STEPS       : %d\n", steps);
    printf("BATTERY     : %d%%\n", battery);

    displayMap(map, x, y, 1);

    delay(300);
}

int main()
{
    char pilot[30], command[10];
    char map[GRID+1][GRID+1];

    int readings[SENSORS];
    int *ptr;

    struct Sensor frontSensor;

    int mode;
    int startX, startY;
    int wayX[MAXPOINTS], wayY[MAXPOINTS];

    int pointCount, i, row, col;
    int x, y, targetX, targetY;
    int oldX, oldY;

    int steps, battery, duty;

    int sensorSum, sensorDifference, sensorProduct;

    float average;
    char direction;

    clrscr();

    printf("====================================\n");
    printf("       DRONE PATH SIMULATOR\n");
    printf("====================================\n");

    /* EXPERIMENT 1: INPUT AND OUTPUT */

    printf("\nEnter Pilot Name: ");
    scanf("%29s", pilot);

    printf("\nWelcome %s!\n", pilot);

    /* EXPERIMENT 6: SWITCH CASE */

    printf("\nSELECT DRONE MODE\n");
    printf("1. IDLE\n");
    printf("2. ACTIVE\n");
    printf("3. ERROR\n");

    printf("\nEnter Mode: ");
    scanf("%d", &mode);

    switch (mode)
    {
        case 1:
            printf("Drone is IDLE.\n");
            getch();
            return 0;

        case 2:
            printf("Drone is ACTIVE.\n");
            break;

        case 3:
            printf("Drone ERROR. Mission stopped.\n");
            getch();
            return 0;

        default:
            printf("Invalid Mode.\n");
            getch();
            return 0;
    }

    /* EXPERIMENT 7: STRING MANIPULATION */

    printf("\nEnter Command START / STOP: ");
    scanf("%9s", command);

    if (strcmp(command, "STOP") == 0)
    {
        printf("Drone Stopped.\n");
        getch();
        return 0;
    }
    else if (strcmp(command, "START") != 0)
    {
        printf("Invalid Command. Use START or STOP.\n");
        getch();
        return 0;
    }

    printf("Mission Started!\n");

    /* ENTER STARTING COORDINATES */

    printf("\nEnter Starting X (1-10): ");
    scanf("%d", &startX);

    printf("Enter Starting Y (1-10): ");
    scanf("%d", &startY);

    if (startX < 1 || startX > GRID ||
        startY < 1 || startY > GRID)
    {
        printf("Invalid Starting Coordinates.\n");
        getch();
        return 0;
    }

    /* ENTER WAYPOINTS */

    printf("\nEnter Number of Waypoints (1-5): ");
    scanf("%d", &pointCount);

    if (pointCount < 1 || pointCount > MAXPOINTS)
    {
        printf("Invalid Number of Waypoints.\n");
        getch();
        return 0;
    }

    for (i = 0; i < pointCount; i++)
    {
        printf("\nEnter Waypoint %d (X Y): ", i+1);
        scanf("%d %d", &wayX[i], &wayY[i]);

        if (wayX[i] < 1 || wayX[i] > GRID ||
            wayY[i] < 1 || wayY[i] > GRID)
        {
            printf("Invalid Waypoint.\n");
            getch();
            return 0;
        }
    }

    /* EXPERIMENT 5: ARRAY AND FUNCTION */

    printf("\nEnter 5 Sensor Readings (0-100):\n");

    for (i = 0; i < SENSORS; i++)
    {
        printf("Sensor %d: ", i+1);
        scanf("%d", &readings[i]);

        if (readings[i] < 0 || readings[i] > 100)
        {
            printf("Invalid Reading.\n");
            getch();
            return 0;
        }
    }

    /* EXPERIMENT 2: ARITHMETIC OPERATIONS */

    sensorSum = readings[0] + readings[1];

    sensorDifference =
        absoluteValue(readings[0] - readings[1]);

    sensorProduct = readings[0] * readings[1];

    printf("\nSENSOR CALCULATIONS\n");

    printf("Sum        : %d\n", sensorSum);
    printf("Difference : %d\n", sensorDifference);
    printf("Product    : %d\n", sensorProduct);

    average = calculateAverage(readings, SENSORS);

    printf("Average    : %.2f\n", average);

    /* EXPERIMENT 9: POINTER AND ARRAY */

    ptr = readings;

    printf("\nSENSOR READINGS USING POINTER\n");

    for (i = 0; i < SENSORS; i++)
    {
        printf("Sensor %d : %d\n",
               i+1, *(ptr+i));
    }

    /* EXPERIMENT 8: STRUCTURE */

    frontSensor.id = 1;
    frontSensor.reading = readings[0];
    frontSensor.status = 1;

    printf("\nFRONT SENSOR INFORMATION\n");

    printf("Sensor ID : %d\n", frontSensor.id);
    printf("Reading   : %d\n", frontSensor.reading);
    printf("Status    : ON\n");

    /* EXPERIMENT 3: IF ELSE */

    printf("\nOBSTACLE DETECTION\n");

    if (frontSensor.reading <= 20)
    {
        printf("Obstacle Detected!\n");
        printf("Drone Flight Stopped.\n");
        getch();
        return 0;
    }
    else
    {
        printf("Path Clear. Drone Can Fly.\n");
    }

    /* EXPERIMENT 4: PWM USING FOR LOOP */

    printf("\nEnter PWM Duty (0-10): ");
    scanf("%d", &duty);

    if (duty < 0 || duty > 10)
    {
        printf("Invalid PWM Duty.\n");
        getch();
        return 0;
    }

    displayPWM(duty);

    /* CREATE DRONE MAP */

    for (row = 1; row <= GRID; row++)
    {
        for (col = 1; col <= GRID; col++)
        {
            map[row][col] = '.';
        }
    }

    map[startY][startX] = 'S';

    for (i = 0; i < pointCount; i++)
    {
        map[wayY[i]][wayX[i]] = 'W';
    }

    map[wayY[pointCount-1]][wayX[pointCount-1]] = 'F';

    x = startX;
    y = startY;

    steps = 0;
    battery = 100;

    clrscr();

    printf("====================================\n");
    printf("       DRONE ROUTE CREATED\n");
    printf("====================================\n");

    printf("Pilot: %s\n", pilot);

    printf("Starting Point: (%d,%d)\n",
           startX, startY);

    for (i = 0; i < pointCount; i++)
    {
        printf("Waypoint %d: (%d,%d)\n",
               i+1, wayX[i], wayY[i]);
    }

    displayMap(map, x, y, 1);

    printf("\nPress any key to START FLIGHT...");
    getch();

    /* AUTOMATIC COORDINATE FOLLOWING */

    for (i = 0; i < pointCount; i++)
    {
        targetX = wayX[i];
        targetY = wayY[i];

        /* MOVE ALONG X AXIS */

        while (x != targetX)
        {
            oldX = x;
            oldY = y;

            if (x < targetX)
            {
                x++;
                direction = 'R';
            }
            else
            {
                x--;
                direction = 'L';
            }

            steps++;

            battery = 100 - (steps * 2);

            if (battery < 0)
                battery = 0;

            map[oldY][oldX] = '*';

            showDroneStep(map, x, y,
                          targetX, targetY,
                          steps, battery, direction);
        }

        /* MOVE ALONG Y AXIS */

        while (y != targetY)
        {
            oldX = x;
            oldY = y;

            if (y < targetY)
            {
                y++;
                direction = 'U';
            }
            else
            {
                y--;
                direction = 'D';
            }

            steps++;

            battery = 100 - (steps * 2);

            if (battery < 0)
                battery = 0;

            map[oldY][oldX] = '*';

            showDroneStep(map, x, y,
                          targetX, targetY,
                          steps, battery, direction);
        }

        map[y][x] = 'W';
    }

    /* FINAL MISSION REPORT */

    map[y][x] = 'F';

    clrscr();

    printf("====================================\n");
    printf("      DRONE MISSION COMPLETED\n");
    printf("====================================\n");

    displayMap(map, x, y, 0);

    printf("\nPilot Name       : %s\n", pilot);
    printf("Final Position   : (%d,%d)\n", x, y);
    printf("Waypoints Reached: %d\n", pointCount);
    printf("Total Grid Steps : %d\n", steps);
    printf("Battery Remaining: %d%%\n", battery);

    printf("\nMission Status : SUCCESSFUL\n");
    printf("Drone Status  : LANDED\n");

    printf("\n====================================\n");
    printf("Press any key to exit.\n");

    getch();

    return 0;
}
