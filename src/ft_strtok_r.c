//#include <minishell.h>
#include <stdio.h>
#include <string.h>

char	*my_strtok(char *str, const char *delim, char **save_ptr)
{
	char	*start;
	char	*end;

	if (str != NULL)
		*save_ptr = str;
	else if (*save_ptr == NULL || **save_ptr == '\0')
		return (NULL);
	start = *save_ptr;
	while (*start != '\0' && strchr(delim, *start) != NULL)
		start++;
	if (*start == '\0')
		return (*save_ptr = NULL, NULL);
	end = start;
	while (*end != '\0' && strchr(delim, *end) == NULL)
		end++;
	if (*end != '\0')
	{
		*end = '\0';
		*save_ptr = end + 1;
	}
	else
		*save_ptr = NULL;
	return (start);
}

//int main()
//{
//	char str[] = "Hola,** mundo   este* * es*un,ejemplo";
//	const char delim[] = "    *,";
//	char *save_ptr;
//	char *token = my_strtok(str, delim, &save_ptr);

//	while (token != NULL)
//	{
//		printf("%s\n", token);
//		token = my_strtok(NULL, delim, &save_ptr);
//	}
//	return 0;
//}
