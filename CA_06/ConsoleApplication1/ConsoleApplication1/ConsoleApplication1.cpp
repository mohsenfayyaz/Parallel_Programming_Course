// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <time.h>
#include <Windows.h>

#define N  12

int solutions;

void print_names()
{
    printf("Mohsen Fayyaz - Mahdi Hadiloo\n");
    printf("  810196650   -   810196587 \n");
}

int put(int Queens[], int row, int column)
{
    int i;
    for (i = 0; i < row; i++) {
        if (Queens[i] == column || abs(Queens[i] - column) == (row - i))
            return -1;
    }
    Queens[row] = column;
    if (row == N - 1) {
        #pragma omp atomic
        solutions++;
    }
    else {
        for (i = 0; i < N; i++) { // increment row
            put(Queens, row + 1, i);
        }
    }
    return 0;
}


void solve(int Queens[]) {
    int i;
    #pragma omp parallel for
    for (i = 0; i < N; i++) {
        int Q[N];
        // int Q = new int[n];
        put(Q, 0, i);
    }
}



int main()
{
    int Queens[N];
    DWORD t0 = 0, tf = 0, t0s = 0, tfs = 0;

    t0 = timeGetTime();
    solve(Queens);
    tf = timeGetTime();
    print_names();
    printf("# solutions %d time: %f \n", solutions, difftime(tf, t0));
    
    // getchar();
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
