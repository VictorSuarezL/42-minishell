//#include <minishell.h>
#include <stdio.h>
#include <string.h>

char	*my_strtok(char *str, const char *delim)
{
	static char	*ptr = NULL;
	char		*start;
	char		*end;

	if (str != NULL)
		ptr = str;
	else if (ptr == NULL)
		return (NULL);
	start = ptr;
	while (*start != '\0' && strchr(delim, *start) != NULL)
		start++;
	if (*start == '\0')
		return (ptr = NULL, NULL);
	end = start;
	while (*end != '\0' && strchr(delim, *end) == NULL)
		end++;
	if (*end != '\0')
	{
		*end = '\0';
		ptr = end + 1;
	}
	else
		ptr = NULL;
	return (start);
}

//int main()
//{
//    char str[] = "Hola,mundo,este,es,un,ejemplo";
//    const char delim[] = ",";
//    char *token = my_strtok(str, delim);

//	while (token != NULL)
//	{
//		printf("%s\n", token);
//		token = my_strtok(NULL, delim);
//	}
//	return 0;
//}