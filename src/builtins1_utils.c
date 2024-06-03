#include <minishell.h>

// Function to copy old export to new export
void	copy_export(char **new_export, char **export, int count)
{
	for (int i = 0; i < count; i++)
		new_export[i] = export[i];
}