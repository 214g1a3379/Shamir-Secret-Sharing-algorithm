#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct {
    int x;
    double y;
} Point;

double decodeValue(const char *valueStr, int base) {
    return strtol(valueStr, NULL, base);
}

int readPointsFromFile(const char *filename, Point *points, int *n, int *k) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return 0;
    }

    char line[256];
    int count = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "\"n\":")) {
            sscanf(line, " \"n\": %d,", n);
        } else if (strstr(line, "\"k\":")) {
            sscanf(line, " \"k\": %d,", k);
        } else if (strstr(line, "base")) {
            int x, base;
            char valueStr[256];
            sscanf(line, " \"%d\": { \"base\": \"%d\", \"value\": \"%[^\"]\" }", &x, &base, valueStr);
            points[count].x = x;
            points[count].y = decodeValue(valueStr, base);
            count++;
        }
    }
    fclose(file);
    return count;
}

double lagrangeInterpolation(Point *points, int k) {
    double constantTerm = 0;

    for (int i = 0; i < k; i++) {
        double term = points[i].y;

        for (int j = 0; j < k; j++) {
            if (i != j) {
                term *= (0 - points[j].x) / (double)(points[i].x - points[j].x);
            }
        }

        constantTerm += term;
    }

    return round(constantTerm);  
}

int main() {
    const char *filenames[] = {"testcase1.json", "testcase2.json"};
    int numFiles = sizeof(filenames) / sizeof(filenames[0]);

    for (int i = 0; i < numFiles; i++) {
        Point points[10];  
        int n, k;
        int count = readPointsFromFile(filenames[i], points, &n, &k);
        if (count < k) {
            fprintf(stderr, "Insufficient points for k value\n");
            continue;
        }
        double constantTerm = lagrangeInterpolation(points, k);
        printf("The secret for Test Case %d is: %.0f\n", i + 1, constantTerm);
    }

    return 0;
}
