

/*
 PROJECT: COORDINATE BASED DRONE SIMULATOR
 TURBO C COMPATIBL
*/

#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <string.h>

#define GRID 10
#define SENSORS 5

/* EXPERIMENT 8: STRUCTURE */

struct Sensor
{
    int id;
    int reading;
    int status;
};

/* EXPERIMENT 2: ARITHMETIC */

int absoluteValue(int n)
{
    if (n < 0)
        return -n;
    else
        return n;
}

/* EXPERIMENT 5: FUNCTION AND ARRAY */

float calculateAverage(int a[], int n)
{
    int i, sum = 0;

    for (i = 0; i < n; i++)
        sum = sum + a[i];

    return (float)sum / n;
}

/* EXPERIMENT 4: PWM USING FOR LOOP */

void showPWM(int duty)
{
    int i;

    printf("\nMotor PWM: ");

    for (i = 1; i <= 10; i++)
    {
        if (i <= duty)
            printf("1");
        else
            printf("0");
    }

    printf("\n");
}

/* DISPLAY DRONE MAP */

void displayMap(char map[11][11], int x, int y)
{
    int i, j;

    printf("\n          DRONE MAP\n\n");

    for (i = GRID; i >= 1; i--)
    {
        printf("%2d | ", i);

        for (j = 1; j <= GRID; j++)
        {
            if (j == x && i == y)
                printf(" D ");
            else
                printf(" %c ", map[i][j]);
        }

        printf("\n");
    }

    printf("    ");

    for (j = 1; j <= GRID; j++)
        printf("---");

    printf("\n     ");

    for (j = 1; j <= GRID; j++)
        printf("%2d ", j);

    printf("\n\n");

    printf("S = START   F = FINISH\n");
    printf("p=PATH     D = DRONE\n");
}

/* DISPLAY FLIGHT MOVEMENT */

void showFlight(char map[11][11],
                int x, int y,
                int tx, int ty,
                int steps, int battery,
                char direction)
{
    clrscr();

    printf("====================================\n");
    printf("       AUTONOMOUS DRONE\n");
    printf("====================================\n");

    printf("STATUS      : FLYING\n");
    printf("POSITION    : (%d,%d)\n", x, y);
    printf("DESTINATION : (%d,%d)\n", tx, ty);

    /* EXPERIMENT 6: SWITCH CASE */

    printf("DIRECTION   : ");

    switch (direction)
    {
        case 'R':
            printf("RIGHT\n");
            break;

        case 'L':
            printf("LEFT\n");
            break;

        case 'U':
            printf("UP\n");
            break;

        case 'D':
            printf("DOWN\n");
            break;
    }

    printf("STEPS       : %d\n", steps);
    printf("BATTERY     : %d%%\n", battery);

    displayMap(map, x, y);

    delay(900);
}

/* MAIN FUNCTION */

int main()
{
    int startX, startY;
    int destX, destY;

    int x, y;
    int oldX, oldY;

    int steps, battery;
    int distance, duty;

    int i, j;

    int sensorValues[SENSORS];
    int *ptr;

    struct Sensor frontSensor;

    float average;

    char command[5];
    char direction;

    char map[11][11];

    int mode;

    clrscr();

    /* EXPERIMENT 1: INPUT AND OUTPUT */

    printf("====================================\n");
    printf("       DRONE PATH SIMULATOR\n");
    printf("====================================\n");

    printf("\nEnter Starting Coordinates (X Y): ");
    scanf("%d %d", &startX, &startY);

    /* EXPERIMENT 3: IF ELSE */

    if (startX < 1 || startX > GRID ||
        startY < 1 || startY > GRID)
    {
        printf("\nInvalid Starting Coordinates!");
        getch();
        return 0;
    }

    printf("\nEnter Destination Coordinates (X Y): ");
    scanf("%d %d", &destX, &destY);

    if (destX < 1 || destX > GRID ||
        destY < 1 || destY > GRID)
    {
        printf("\nInvalid Destination Coordinates!");
        getch();
        return 0;
    }

    /* EXPERIMENT 7: STRING COMPARISON */

    printf("\nReady for Takeoff? (Y/N): ");
    scanf("%4s", command);

    if (strcmp(command, "Y") != 0 &&
        strcmp(command, "y") != 0)
    {
        printf("\nDrone Mission Cancelled!");
        getch();
        return 0;
    }

    /* EXPERIMENT 6: SWITCH CASE */

    mode = 2;

    switch (mode)
    {
        case 1:
            printf("\nDrone Idle");
            getch();
            return 0;

        case 2:
            printf("\nDrone Active");
            break;

        default:
            printf("\nDrone Error");
            getch();
            return 0;
    }

    /* EXPERIMENT 8: STRUCTURE */

    sensorValues[0] = 80;
    sensorValues[1] = 75;
    sensorValues[2] = 90;
    sensorValues[3] = 85;
    sensorValues[4] = 70;

    frontSensor.id = 1;
    frontSensor.reading = sensorValues[0];
    frontSensor.status = 1;

    /* EXPERIMENT 9: POINTER */

    ptr = sensorValues;

    /* EXPERIMENT 5: FUNCTION */

    average = calculateAverage(ptr, SENSORS);

    /* AUTOMATIC SENSOR CHECK */

    if (frontSensor.reading <= 20)
    {
        printf("\nObstacle Detected!");
        printf("\nFlight Stopped!");
        getch();
        return 0;
    }
    else
    {
        printf("\nPath Clear!");
    }

    printf("\nAverage Sensor Reading: %.2f", average);

    /* EXPERIMENT 2: COORDINATE CALCULATION */

    distance = absoluteValue(destX - startX)
             + absoluteValue(destY - startY);

    printf("\nTotal Route Distance: %d grid units\n",
           distance);

    /* EXPERIMENT 4: PWM SIMULATION */

    duty = 7;

    showPWM(duty);

    /* CREATE MAP */

    for (i = 1; i <= GRID; i++)
    {
        for (j = 1; j <= GRID; j++)
            map[i][j] = '.';
    }

    map[startY][startX] = 'S';
    map[destY][destX] = 'F';

    x = startX;
    y = startY;

    steps = 0;
    battery = 100;

    clrscr();

    printf("====================================\n");
    printf("       DRONE ROUTE CREATED\n");
    printf("====================================\n");

    printf("\nStarting Point: (%d,%d)", startX, startY);
    printf("\nDestination  : (%d,%d)\n", destX, destY);

    displayMap(map, x, y);

    printf("\nPress any key to TAKE OFF...");
    getch();

    /* AUTOMATIC X AXIS MOVEMENT */

    while (x != destX)
    {
        oldX = x;
        oldY = y;

        if (x < destX)
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

        showFlight(map, x, y,
                   destX, destY,
                   steps, battery, direction);
    }

    /* AUTOMATIC Y AXIS MOVEMENT */

    while (y != destY)
    {
        oldX = x;
        oldY = y;

        if (y < destY)
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

        showFlight(map, x, y,
                   destX, destY,
                   steps, battery, direction);
    }

    /* FINAL MISSION REPORT */

    map[destY][destX] = 'F';

    clrscr();

    printf("====================================\n");
    printf("       MISSION COMPLETED\n");
    printf("====================================\n");

    displayMap(map, x, y);

    printf("\nStarting Point : (%d,%d)", startX, startY);
    printf("\nFinal Position : (%d,%d)", x, y);

    printf("\nTotal Distance : %d grid units", distance);
    printf("\nTotal Steps    : %d", steps);
    printf("\nBattery Left   : %d%%", battery);

    printf("\n\nDRONE STATUS  : LANDED");
    printf("\nMISSION STATUS: SUCCESSFUL");

    printf("\n====================================");
    printf("\nPress any key to exit.");

    getch();

    return 0;
}
