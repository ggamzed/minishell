#include "../minishell.h"

char	*ft_handle_exit_status(t_shell *shell)
{
	return (ft_itoa(shell->exit_status, shell));
}

static char *ft_expand_tilde(const char *value, t_shell *shell)
{
    char    *base;
    char    *suffix;
    char    *expanded;
    char    *temp;

    if (!value || value[0] != '~')
        return (NULL);

    if (value[1] != '\0' && value[1] != '/' && value[1] != '+' && value[1] != '-')
        return (NULL);

    if (value[1] == '+' || value[1] == '-')
    {
        if (value[1] == '+')
            base = ft_get_env_value("PWD", shell->env_list);
        else
            base = ft_get_env_value("OLDPWD", shell->env_list);
        suffix = (char *)(value + 2);
    } 
    else
    {
        base = ft_get_env_value("HOME", shell->env_list);
        suffix = (char *)(value + 1);
    }
    if (!base)
        return (ft_strdup(value, shell));
    if (suffix[0] == '\0')
        return (ft_strdup(base, shell));
    if (suffix[0] == '/')
        return (ft_strjoin(base, suffix, shell));
    temp = ft_strjoin(base, "/", shell);
    expanded = ft_strjoin(temp, suffix, shell);
    return (expanded);
}

char *ft_expand_word_variables(char *str, t_shell *shell)
{
    char *result;
    char *temp;
    char *new_result;
    int i;

	if (ft_strcmp(str, "$") == 0)
        return (ft_strdup("", shell));
    result = ft_strdup("", shell);
    if (!result)
        return (NULL);
    i = 0;
    while (str[i])
    {
        if (str[i] == '$' && str[i + 1] == '?')
        {
            temp = ft_handle_exit_status(shell);
            new_result = ft_strjoin(result, temp, shell);
            result = new_result;
            i += 2;
        }
        else if (str[i] == '$' && (ft_isalpha(str[i + 1]) || str[i + 1] == '_'))
        {
            temp = ft_extract_and_expand_var(str, &i, shell);
            new_result = ft_strjoin(result, temp, shell);
            result = new_result;
        }
       	else
		{
    		if (str[i] == '\\' && str[i + 1] != '\0')
    		{
        		temp = ft_malloc(2, shell);
        		temp[0] = str[i + 1];
        		temp[1] = '\0';
        		new_result = ft_strjoin(result, temp, shell);
        		result = new_result;
        		i += 2;
    		}
    		else
    		{
        		temp = ft_malloc(2, shell);
        		temp[0] = str[i];
        		temp[1] = '\0';
        		new_result = ft_strjoin(result, temp, shell);
        		result = new_result;
        		i++;
    		}
		}
    }
    return (result);
}

static char	*ft_expand_token_value(char *value, t_token_type type, t_shell *shell, int is_heredoc_delimiter)
{
	int	i;
	char *tilde;

	i = 0;
	if (!value)
		return (NULL);
	if (is_heredoc_delimiter)
		return (ft_strdup(value, shell));
	if (type == VARIABLE)
		return (ft_extract_and_expand_var(value, &i, shell));
	else if (type == EXIT_STATUS)
		return (ft_handle_exit_status(shell));
	else if (type == DOUBLE_QUOTED_STRING)
		return (ft_expand_double_quoted(value, shell));
	else if (type == SINGLE_QUOTED_STRING)
		return (ft_strdup(value, shell));
	else if (type == WORD)
	{
    	tilde = ft_expand_tilde(value, shell);
    	if (tilde)
        	return (tilde);
    	return (ft_expand_word_variables(value, shell));
	}
	else
		return (ft_strdup(value, shell));
}

static int	count_args(t_token *args)
{
	t_token *current;
	int	count;

	count = 0;
	current = args;
	while (current)
	{
		current = current->next;
		count++;
	}
	return (count);
}

char **ft_split_expanded_word(char *word, t_shell *shell)
{
    char **words;
    int count = 0;
    int i = 0, j = 0, start;
    
    while (word[i])
    {
        while (word[i] && ft_is_space(word[i]))
            i++;
        if (word[i])
        {
            count++;
            while (word[i] && !ft_is_space(word[i]))
                i++;
        }
    }
    if (count == 0)
        return (NULL);
    words = ft_malloc(sizeof(char *) * (count + 1), shell);
    if (!words)
        return (NULL);
    i = 0;
    while (word[i] && j < count)
    {
        while (word[i] && ft_is_space(word[i]))
            i++;
        start = i;
        while (word[i] && !ft_is_space(word[i]))
            i++;
        if (i > start)
        {
            words[j] = ft_substr(word, start, i - start, shell);
            j++;
        }
    }
    words[j] = NULL;
    return (words);
}

int should_word_split(t_token *token_group)
{
    t_token *current = token_group;
    
    while (current)
    {
        if (current->type == VARIABLE)
            return (1);
        if (current->next && current->next->space_flag == 0)
            current = current->next;
        else
            break;
    }
    return (0);
}

int get_token_group_length(t_token *start_token)
{
    t_token *current;
    int length;

	length = 1;
    current = start_token;
    while (current->next && current->next->space_flag == 0)
    {
        current = current->next;
        length++;
    }
    return (length);
}

void ft_join_expand_tokens(char ***joined_argv, char **expanded_argv, 
                            t_token *original_tokens, t_shell *shell)
{
    char **final_argv;
    t_token *current;
    char **split_words;
    int final_count = 0;
    int i, j, k;
    
    current = original_tokens;
    i = 0;
    while (current)
    {
        char *merged_word = ft_strdup(expanded_argv[i], shell);
        t_token *token_start = current;
        int group_length = get_token_group_length(current);
        for (int g = 1; g < group_length; g++)
        {
            current = current->next;
            i++;
            char *temp = ft_strjoin(merged_word, expanded_argv[i], shell);
            merged_word = temp;
        }
        if (should_word_split(token_start))
        {
            split_words = ft_split_expanded_word(merged_word, shell);
            if (split_words)
            {
                j = 0;
                while (split_words[j])
                {
                    final_count++;
                    j++;
                }
            }
        }
        else
        {
            final_count++;
        }
        
        i++;
        current = current->next;
    }
    final_argv = ft_malloc(sizeof(char *) * (final_count + 1), shell);
    if (!final_argv)
    {
        *joined_argv = NULL;
        return;
    }
    current = original_tokens;
    i = 0;
    k = 0;
    while (current)
    {
        char *merged_word = ft_strdup(expanded_argv[i], shell);
        t_token *token_start = current;
        int group_length = get_token_group_length(current);
        for (int g = 1; g < group_length; g++)
        {
            current = current->next;
            i++;
            char *temp = ft_strjoin(merged_word, expanded_argv[i], shell);
            merged_word = temp;
        }
        if (should_word_split(token_start))
        {
            split_words = ft_split_expanded_word(merged_word, shell);
            if (split_words)
            {
                j = 0;
                while (split_words[j])
                {
                    final_argv[k] = ft_strdup(split_words[j], shell);
                    k++;
                    j++;
                }
            }
        }
        else
        {
            final_argv[k] = ft_strdup(merged_word, shell);
            k++;
        }
        
        i++;
        current = current->next;
    }
    
    final_argv[k] = NULL;
    *joined_argv = final_argv;
}

char	**ft_expand_tokens(t_token *args, t_shell *shell)
{
	char		**argv;
	char		**join_argv;
	t_token		*current;
	int			i;
	
	argv = ft_malloc(sizeof(char *) * (count_args(args) + 1), shell);
	if (!argv)
		return (NULL);
	current = args;
	i = 0;
	while (current)
	{
		argv[i++] = ft_expand_token_value(current->value, current->type, shell, 0);
		
		current = current->next;
	}
	argv[i] = NULL;
	ft_join_expand_tokens(&join_argv, argv, args, shell);
	return (join_argv);
}
