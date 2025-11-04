#include "storage.h"
#include "error_codes.h"

#include <stdio.h>


int main(int argc, char** argv) {
	int flag = 0;

	setProgramName(argv[0]);
	polishTheConsole();
	readArg(argc, argv);

	flag = startApp(argv[1], (argc > 2 ? argv[2] : NULL));
	if (flag != 0) {
		fprintf(stderr, "%s: Koñczê dzia³anie programu.\n", program_name);
		fprintf(stderr, "Kod b³êdu: %d\n", flag);
		return flag;
	}

	return SUCCESS;
}
