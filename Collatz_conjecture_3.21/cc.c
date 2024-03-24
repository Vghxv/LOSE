#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

int main( int argc, char *argv[] ) {
    int n = -1;
    if ( argc != 2 ) {
        fprintf( stderr, "Usage: %s <number>\n", argv[0] );
        exit(EXIT_FAILURE);
    }
    n = atoi( argv[1] );
    if ( n <= 0 ) {
        fprintf( stderr, "The number must be positive.\n exiting...\n" );
        exit(EXIT_FAILURE);
    }
    pid_t pid;
    pid = fork();
    if ( pid == -1 ) {
        perror( "fork" );
        return 1;
    } 
    else if ( pid == 0 ) {
        while( n != 1 ) {
            printf( "%d ", n );
            if ( n % 2 == 0 ) {
                n /= 2;
            } 
            else {
                n = 3 * n + 1;
            }
        }
    } 
    else {
        wait( NULL );
        puts("1");
    }
    return 0;
}