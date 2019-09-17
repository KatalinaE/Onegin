#include <stdio.h>
#include <string.h>
#include <assert.h>

const char *TO_READ = "r";
const char *TO_WRITE = "w";

const char END_STR = '\0';
const char END_LINE = '\n';

struct Str{
	char *content; 
	int len; 
};

long long aaaaa;


long fileSize(const char *fileName);

//------------------------------------------------------------
//! Loads the contents of the file into the buffer
//!
//! @param [in]  fileName - name of file
//! @param [out]  buf - buffer
//------------------------------------------------------------
void load(char *buf, const char *fileName);

//------------------------------------------------------------
//! 
//!
//! @param [in]  buf - buffer
//! @param [in]  bufSize - size of buffer
//!	@param [out] pstrs - pointer to array of strings
//! @param [out] strn - quantity of strings
//------------------------------------------------------------
void link(char *buf, long bufSize, struct Str **pstrs, long *strn);
//------------------------------------------------------------
//! 
//!
//! @param [in]  buf - buffer
//! @param [in]  c - symbol to quantity
//! 
//! @return Quantity
//------------------------------------------------------------
long count(char *buf, char c);

//------------------------------------------------------------
//! 
//!
//! @param [in]  strs - array of strings
//! @param [in]  strn - quantity of strings 
//------------------------------------------------------------
void sort(struct Str strs[], int strn);
//------------------------------------------------------------
//! 
//!
//! @param [in]  strs - array of strings 
//! @param [in]  first - index of the first element 
//! @param [in]  last - index of the last element
//------------------------------------------------------------
void quickSort(struct Str strs[], int first, int last);
//------------------------------------------------------------
//! 
//!
//! @param [in]  a - pointer to first string 
//! @param [in]  b - pointer to second string
//! 
//! @return True or false
//------------------------------------------------------------
int good(struct Str *a, struct Str *b);
//------------------------------------------------------------
//! 
//!
//! @param [in]  a - first value
//! @param [in]  b - second value
//! 
//! @return Minimum of a and b
//------------------------------------------------------------
int min(int a, int b);
//------------------------------------------------------------
//! 
//!
//! @param [in]  strs - array of strings
//! @param [in]  pos1 - index of first string
//! @param [in]	 pos2 - index of second string
//------------------------------------------------------------
void swap(struct Str strs[], int pos1, int pos2);

//------------------------------------------------------------
//! 
//!
//! @param [in]  strs - array of strings  
//! @param [in]  strn - quantity of strings
//! @param [in]  fileName - name of file
//------------------------------------------------------------
void save(struct Str strs[], int strn, const char *fileName);

//------------------------------------------------------------
//! 
//!
//! @param [in]  argc - quantity of parameters of command line
//! @param [in]  argv - array of parameters accepted as strings
//! 
//! @return Error number
//!
//! @note If not errors return 0
//------------------------------------------------------------
int main(int argc, char **argv)
{
	if (argc != 3)
		return printf("\nExpected:\n Sort_Onegin [input file name] [output file name]\n"), 1;

	long bufSize = fileSize(argv[1]) + 2;
	assert(bufSize < 4E+8);
	char *buf = (char*)calloc(bufSize, sizeof(char));
	Str *strs = NULL;
	long strn = 0;

	load(buf, argv[1]);
	link(buf, bufSize, &strs, &strn);
	sort(strs, strn);
	save(strs, strn, argv[2]);

	if (strs)
		free(strs);
	if (buf)
		free(buf);

	return 0;
}

long fileSize(const char *fileName)
{
	FILE *in = NULL;
	assert(in = fopen(fileName, TO_READ));

	long first = ftell(in);

	assert(fseek(in, 0, SEEK_END) != -1);

	long last = ftell(in);

	fclose(in);
	return (last - first + 1);
}

void load(char *buf, const char *fileName)
{
	FILE *in = NULL;

	assert(in = fopen(fileName, TO_READ));
	assert(buf != NULL);

	fread(buf, sizeof(char), fileSize(fileName), in);

	*(buf + fileSize(fileName) + 1) = END_STR;

	fclose(in);
}

void link(char *buf, long bufSize, struct Str **pstrs, long *strn)
{
	assert(buf != NULL && pstrs != NULL && strn != NULL);

	*strn = count(buf, END_LINE);
	if (!*strn)
		return;

	*pstrs = (Str*)calloc(*strn, sizeof(Str));

	char *it = buf - 1, *tmp = NULL;
	struct Str *strs = *pstrs;

	for (int i = 0; i < *strn; ++i)
	{
		strs[i].content = ++it;
		tmp = strchr(it, END_LINE);
		strs[i].len = tmp - it;
		it = tmp;
		*it = END_STR;
	}
}

long count(char *buf, char c)
{
	assert(buf != NULL);

	long cnt = 0;
	while (*buf)
	{
		if (*buf == c)
			++cnt;
		++buf;
	}
	return cnt;
}

void sort(struct Str strs[], int strn)
{
	assert(strs != NULL);
	quickSort(strs, 0, strn - 1);
}

void quickSort(struct Str strs[], int first, int last)
{
    if (first < last)
    {
        int med = (first + last + 1) / 2;

        int i = first, j = last;
		Str x = strs[med];

        while (i <= j)
        {
			while (good(&strs[i], &x))
				++i;

			while (good(&x, &strs[j]))
				--j;

			if (i <= j)
			{
				if (good(&strs[j], &strs[i]))
					swap(strs, i, j);

				--j;
				++i;
			}
        }

		quickSort(strs, i, last);
		quickSort(strs, first, j);
    }
}

int good(struct Str *a, struct Str *b)
{
	assert(a != NULL && b != NULL);

	int len = min(a->len, b->len);

	for (int i = 0; i < len; ++i)
	{
		if (a->content[i] < b->content[i])
			return true;
		else if (a->content[i] > b->content[i])
			return false;
	}

	return (a->len < b->len);
}

int min(int a, int b)
{
	return (a < b) ? a : b;
}

void swap(struct Str strs[], int a, int b)
{
	struct Str tmp = strs[a];
	strs[a] = strs[b];
	strs[b] = tmp;
}

void save(struct Str strs[], int strn, const char *fileName)
{
	FILE *out = NULL;
	assert(out = fopen(fileName, TO_WRITE));
	assert(strs != NULL);

	for (int i = 0; i < strn; ++i)
	{
		fprintf(out, strs[i].content);
		if (i != strn - 1)
			fprintf(out, "\n");
	}

	fclose(out);
}
