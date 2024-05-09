#include <minishell.h>

#include <stdio.h>
#include <string.h>

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