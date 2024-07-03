#include <minishell.h>

// Function to copy old export to new export
void	copy_export(char **new_export, char **export, int count)
{
	int i;

	i = 0;
	while (i < count)
	{
		new_export[i] = export[i];
		i++;
	}
}