#include <stdio.h>

#include "sequence.h"


#define PRTSEQ( seq ) \
	printf( \
			"Length of seq: %zu\t Capacity of seq: %zu\n", \
			Sequence_length((seq)), \
			Sequence_capacity((seq)) \
		)


int
main(void)
{
	Sequence seq = Sequence_new(sizeof(int), 8);
	printf("Created Sequence:\n");
	PRTSEQ(seq);

	int data[] = {451, 42, 420};
	Sequence_append(seq, data, 3);
	printf("Appended to Sequence:\n");
	PRTSEQ(seq);

	Sequence_shrink(seq);
	printf("Shrunk Sequence:\n");
	PRTSEQ(seq);

	int new_value = 4;
	Sequence_add(seq, &new_value);
	printf("Added item to Sequence:\n");
	PRTSEQ(seq);

	/* Print out contents of Sequence */
	printf("Sequence contents: ");
	for (size_t i = 0; i < Sequence_length(seq); i++)
		printf("%d ", *(int*)Sequence_at(seq, i));
	printf("\n");

	Sequence_free(seq);
	return 0;
}
