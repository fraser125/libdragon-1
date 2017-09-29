#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	if (argc < 4)
	{
		fprintf(stderr, "Usage: %s intput_filename output_filename array_name\n", argv[0]);
		return -1;
	}

	FILE *f_input = fopen(argv[1], "rb");
	if (f_input == NULL) {
		fprintf(stderr, "%s: can't open %s for reading\n", argv[0], argv[1]);
		return -1;
	}

	// Get the file length
	fseek(f_input, 0, SEEK_END);
	const unsigned int file_size = ftell(f_input);
	fseek(f_input, 0, SEEK_SET);
	
	char *buf = (char *)malloc(file_size);
	if (buf == NULL)
		return -1;

	fread(buf, file_size, 1, f_input);
	fclose(f_input);

	FILE *f_output = fopen(argv[2], "w");
	if (f_output == NULL)
	{
		fprintf(stderr, "%s: can't open %s for writing\n", argv[0], argv[1]);
		return -1;
	}
	char * ident = argv[3];

	unsigned int need_comma = 0;

	fprintf(f_output, "#ifndef __BIN2C_%s\n", ident);
	fprintf(f_output, "#define __BIN2C_%s\n", ident);
	fprintf(f_output, "\n");
	fprintf(f_output, "const int %s_length = %i;\n", ident, file_size);
	fprintf(f_output, "\n");
	fprintf(f_output, "const char %s[%i] = {", ident, file_size);
	for (unsigned int i = 0; i < file_size; ++i)
	{
		if (need_comma) fprintf(f_output, ", ");
		else need_comma = 1;
		if ((i % 11) == 0) fprintf(f_output, "\n\t");
		fprintf(f_output, "0x%.2x", buf[i] & 0xff);
	}
	fprintf(f_output, "\n};\n\n");
	fprintf(f_output, "\n");
	fprintf(f_output, "#endif // __BIN2C_%s", ident);

	fclose(f_output);

	return 0;
}
