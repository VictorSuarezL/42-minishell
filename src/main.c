#include <minishell.h>

#include <stdio.h>
#include <string.h>

// char	*my_strtok(char *str, const char *delim, char **save_ptr)
// {
// 	char	*start;
// 	char	*end;

// 	if (str != NULL)
// 		*save_ptr = str;
// 	else if (*save_ptr == NULL || **save_ptr == '\0')
// 		return (NULL);
// 	start = *save_ptr;
// 	while (*start != '\0' && ft_strchr(delim, *start) != NULL)
// 		start++;
// 	if (*start == '\0')
// 		return (*save_ptr = NULL, NULL);
// 	end = start;
// 	while (*end != '\0' && ft_strchr(delim, *end) == NULL)
// 		end++;
// 	if (*end != '\0')
// 	{
// 		*end = '\0';
// 		*save_ptr = end + 1;
// 	}
// 	else
// 		*save_ptr = NULL;
// 	return (start);
// }

int main()
{
	char str[] = "Hola,** mundo   este* * es*un,ejemplo de ft_strtok_r";
	const char delim[] = "    *,";
	char *save_ptr;
	char *token = ft_strtok_r(str, delim, &save_ptr);

	while (token != NULL)
	{
		printf("%s\n", token);
		token = ft_strtok_r(NULL, delim, &save_ptr);
	}

    char strb[] = "Hola,mundo,este,es,un,ejemplo";
    const char delimb[] = ",";
    char *tokenb = ft_strtok(strb, delimb);

	while (tokenb != NULL)
	{
		printf("%s\n", tokenb);
		tokenb = ft_strtok(NULL, delimb);
	}
}