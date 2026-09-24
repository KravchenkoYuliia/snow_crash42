#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int	main() {

	int		position = 0;

	int		fd = open( "./token", O_RDONLY );
	if ( fd < 0 ) {
		write( STDERR_FILENO, "Error: can't open the file\n", strlen( "Error: can't open the file\n" ) );
		exit( EXIT_FAILURE );
	}

	char	buffer[30];
	bzero( buffer, 30 );
	
	ssize_t		bytes_nb = read( fd, buffer, 26 );
	if ( bytes_nb < 0 )	{
		write( STDERR_FILENO, "Error: can't read the file\n", strlen( "Error: can't read the file\n" ) );
		exit( EXIT_FAILURE );
	}
	printf( "buffer is %s\n", buffer );
	char	result[30];
	bzero( result, 30 );


	while ( buffer[position] ) {

		result[position] = buffer[position] - position;
		printf( "Current char is %c\n", buffer[position] );
		position++;

	}
	
	printf( "Result is %s\n", result );

}