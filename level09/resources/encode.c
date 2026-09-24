#include <stdio.h>
#include <stdlib.h>

int	main() {

	int		position = 0;
	char	input[10];
	input[0] = '1';
	input[1] = '2';
	input[2] = '3';
	input[3] = '4';
	input[4] = '5';
	input[5] = '6';
	input[6] = '7';

	input[7] = '\0';

	printf( "Input is %s\n", input );
	char	result[10];

	while ( input[position] ) {

		result[position] = input[position] + position;
		printf( "Current char is %c\n", input[position] );
		position++;
	}
	printf( "Result is %s\n", result );

}