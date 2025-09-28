/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_in_parser_handle_redirect_output.c              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:20:49 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:20:50 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_add_output_file(t_cmd *cmd, char *filename, int append_mode,
							t_shell *shell)
{
	char	**new_files;
	int		*new_modes;
	int		i;

	new_files = ft_malloc(sizeof(char *) * (cmd->output_count + 1), shell);
	new_modes = ft_malloc(sizeof(int) * (cmd->output_count + 1), shell);
	if (!new_files || !new_modes)
		ft_cleanup_and_exit(shell, 1);
	i = 0;
	while (i < cmd->output_count)
	{
		new_files[i] = cmd->all_output_files[i];
		new_modes[i] = cmd->all_append_modes[i];
		i++;
	}
	new_files[cmd->output_count] = ft_strdup(filename, shell);
	new_modes[cmd->output_count] = append_mode;
	cmd->all_output_files = new_files;
	cmd->all_append_modes = new_modes;
	cmd->output_count++;
	return (1);
}

int	ft_in_parser_handle_redirect_output(t_cmd *cmd, t_token **current,
									t_shell *shell, int append_mode)
{
	char			*filename;
	t_token_type	token_type;

	*current = (*current)->next;
	if (!*current)
		return (0);
	token_type = (*current)->type;
	if (token_type != WORD && token_type != SINGLE_QUOTED_STRING
		&& token_type != DOUBLE_QUOTED_STRING && token_type != VARIABLE)
		return (0);
	filename = ft_join_redirect_filename(current, shell);
	if (!filename)
		return (0);
	cmd->output_file = filename;
	cmd->output_type = token_type;
	cmd->append_mode = append_mode;
	if (!ft_add_output_file(cmd, filename, append_mode, shell))
		return (0);
	*current = (*current)->next;
	return (1);
}
