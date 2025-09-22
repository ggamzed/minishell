#include "../minishell.h"

// // Heredoc delimiter için bitişik token'ları birleştiren fonksiyon
// static char	*ft_join_heredoc_delimiter(t_token **current, t_shell *shell)
// {
//     char	*delimiter;
//     t_token	*temp;

//     if (!*current)
//         return (NULL);
    
//     // İlk token'ı başlangıç olarak al
//     delimiter = ft_strdup((*current)->value, shell);
//     if (!delimiter)
//         return (NULL);
    
//     temp = *current;
//     // SONRAKİ token'ın space_flag'ini kontrol et!
//     while (temp->next && temp->next->space_flag == 0)  // ✅ DOĞRU!
//     {
//         temp = temp->next;
//         delimiter = ft_strjoin_free(delimiter, temp->value, shell);
//         if (!delimiter)
//             return (NULL);
//     }
    
//     *current = temp;
//     return (delimiter);
// }

// int	ft_in_parser_handle_redirect_in(t_cmd *cmd, t_token **current, t_shell *shell)
// {
// 	*current = (*current)->next;
// 	if (*current && ((*current)->type == WORD ||
// 				(*current)->type == SINGLE_QUOTED_STRING ||
// 				(*current)->type == DOUBLE_QUOTED_STRING))
// 	{
// 		cmd->input_file = ft_strdup((*current)->value, shell);
// 		cmd->input_type = (*current)->type;
// 		*current = (*current)->next;
// 	}
// 	return (1);
// }


// int ft_in_parser_handle_redirect_out(t_cmd *cmd, t_token **current, t_shell *shell)
// {
//     char **new_files;
//     int *new_modes;
    
//     *current = (*current)->next;
//     if (*current && ((*current)->type == WORD ||
//             (*current)->type == SINGLE_QUOTED_STRING ||
//             (*current)->type == DOUBLE_QUOTED_STRING))
//     {
//         // Son dosyayı cmd->output_file'a kaydet (mevcut davranış)
//         cmd->output_file = ft_strdup((*current)->value, shell);
//         cmd->output_type = (*current)->type;
//         cmd->append_mode = 0;
        
//         // Tüm dosyaları all_output_files dizisine ekle
//         new_files = ft_malloc(sizeof(char *) * (cmd->output_count + 1), shell);
//         new_modes = ft_malloc(sizeof(int) * (cmd->output_count + 1), shell);
        
//         if (!new_files || !new_modes)
//             return (0);
            
//         // Eski dosyaları kopyala
//         for (int i = 0; i < cmd->output_count; i++)
//         {
//             new_files[i] = cmd->all_output_files[i];
//             new_modes[i] = cmd->all_append_modes[i];
//         }
        
//         // Yeni dosyayı ekle
//         new_files[cmd->output_count] = ft_strdup((*current)->value, shell);
//         new_modes[cmd->output_count] = 0; // Normal redirect
        
//         cmd->all_output_files = new_files;
//         cmd->all_append_modes = new_modes;
//         cmd->output_count++;
        
//         *current = (*current)->next;
//     }
//     return (1);
// }

// int ft_in_parser_handle_redirect_append(t_cmd *cmd, t_token **current, t_shell *shell)
// {
//     char **new_files;
//     int *new_modes;
    
//     *current = (*current)->next;
//     if (*current && ((*current)->type == WORD ||
//                 (*current)->type == SINGLE_QUOTED_STRING ||
//                 (*current)->type == DOUBLE_QUOTED_STRING))
//     {
//         // Son dosyayı cmd->output_file'a kaydet (mevcut davranış)
//         cmd->output_file = ft_strdup((*current)->value, shell);
//         cmd->output_type = (*current)->type;
//         cmd->append_mode = 1;
        
//         // Tüm dosyaları all_output_files dizisine ekle
//         new_files = ft_malloc(sizeof(char *) * (cmd->output_count + 1), shell);
//         new_modes = ft_malloc(sizeof(int) * (cmd->output_count + 1), shell);
        
//         if (!new_files || !new_modes)
//             return (0);
            
//         // Eski dosyaları kopyala
//         for (int i = 0; i < cmd->output_count; i++)
//         {
//             new_files[i] = cmd->all_output_files[i];
//             new_modes[i] = cmd->all_append_modes[i];
//         }
        
//         // Yeni dosyayı ekle
//         new_files[cmd->output_count] = ft_strdup((*current)->value, shell);
//         new_modes[cmd->output_count] = 1; // Append mode
        
//         cmd->all_output_files = new_files;
//         cmd->all_append_modes = new_modes;
//         cmd->output_count++;
        
//         *current = (*current)->next;
//     }
//     return (1);
// }

// int	ft_in_parser_handle_heredoc(t_cmd *cmd, t_token **current, t_shell *shell)
// {
// 	int temp_fd;
// 	char *joined_delimiter;
// 	t_token_type first_type;

// 	*current = (*current)->next;
// 	if (*current)
// 	{
// 		// İlk token'ın tipini kaydet (expansion kontrolü için)
// 		first_type = (*current)->type;
		
// 		// Bitişik token'ları birleştir
// 		joined_delimiter = ft_join_heredoc_delimiter(current, shell);
// 		if (!joined_delimiter)
// 			return (0);
		
// 		// Her heredoc için input al ve sadece sonuncusunu tut
// 		temp_fd = ft_process_heredoc(joined_delimiter, shell, 
// 			(first_type != SINGLE_QUOTED_STRING && first_type != DOUBLE_QUOTED_STRING));

// 		// Önceki heredoc fd varsa kapat
// 		if (cmd->heredoc_fd != -1)
// 		{
// 			close(cmd->heredoc_fd);
// 		}
// 		// Yeni fd'yi ata
// 		cmd->heredoc_fd = temp_fd;
// 		cmd->heredoc_delimiter = joined_delimiter;
// 		cmd->heredoc_type = first_type;
// 		cmd->heredoc_should_expand = (first_type != SINGLE_QUOTED_STRING && first_type != DOUBLE_QUOTED_STRING);
		
// 		*current = (*current)->next;
// 	}
// 	return (1);
// }






#include "../minishell.h"

// Redirection filename için bitişik token'ları birleştiren fonksiyon
// Redirection filename için bitişik token'ları birleştiren fonksiyon - TAM DÜZELTME
// static char	*ft_join_redirect_filename(t_token **current, t_shell *shell)
// {
//     char	*filename;
//     char	*temp;
//     t_token	*token_ptr;

//     if (!*current)
//         return (NULL);
    
//     //printf("DEBUG: ft_join_redirect_filename starting with '%s'\n", (*current)->value);
    
//     // İlk token'ı başlangıç olarak al
//     filename = ft_strdup((*current)->value, shell);
//     if (!filename)
//         return (NULL);
    
//     token_ptr = *current;
    
//     // SONRAKİ token'ın space_flag'ini kontrol et ve REDIRECTION olmadığından emin ol
//     while (token_ptr->next && 
//            token_ptr->next->space_flag == 0 &&
//            token_ptr->next->type != PIPE &&           // Pipe değil
//            token_ptr->next->type != REDIRECT_IN &&    // Redirection değil
//            token_ptr->next->type != REDIRECT_OUT &&
//            token_ptr->next->type != REDIRECT_APPEND &&
//            token_ptr->next->type != HEREDOC)
//     {
//         token_ptr = token_ptr->next;
//         //printf("DEBUG: ft_join_redirect_filename joining '%s'\n", token_ptr->value);
//         temp = ft_strjoin(filename, token_ptr->value, shell);
//         // filename'i manuel free etmeyin - memory tracker halleder
//         filename = temp;
//         if (!filename)
//             return (NULL);
//     }
    
//     //printf("DEBUG: ft_join_redirect_filename final filename='%s'\n", filename);
//     //printf("DEBUG: ft_join_redirect_filename final token='%s'\n", 
//     //       token_ptr->value ? token_ptr->value : "NULL");
    
//     *current = token_ptr;
//     return (filename);
// }

// Heredoc delimiter için bitişik token'ları birleştiren fonksiyon
static char	*ft_join_heredoc_delimiter(t_token **current, t_shell *shell, int *has_quotes)
{
    char	*delimiter;
    char	*temp;
    t_token	*token_ptr;

    if (!*current)
        return (NULL);
    
    *has_quotes = 0; // Initialize
    
    // İlk token quoted mi kontrol et
    if ((*current)->type == SINGLE_QUOTED_STRING || (*current)->type == DOUBLE_QUOTED_STRING)
        *has_quotes = 1;
    
    delimiter = ft_strdup((*current)->value, shell);
    if (!delimiter)
        return (NULL);
    
    token_ptr = *current;
    while (token_ptr->next && token_ptr->next->space_flag == 0)
    {
        token_ptr = token_ptr->next;
        
        // Herhangi bir token quoted ise has_quotes = 1
        if (token_ptr->type == SINGLE_QUOTED_STRING || token_ptr->type == DOUBLE_QUOTED_STRING)
            *has_quotes = 1;
            
        temp = ft_strjoin(delimiter, token_ptr->value, shell);
        delimiter = temp;
        if (!delimiter)
            return (NULL);
    }
    
    *current = token_ptr;
    return (delimiter);
}
// Mevcut ft_join_redirect_filename fonksiyonunu bu şekilde değiştir:

static char	*ft_join_redirect_filename(t_token **current, t_shell *shell)
{
    char	*filename;
    char	*temp;
    char	*expanded_filename;
    t_token	*token_ptr;
    t_token_type first_type;

    if (!*current)
        return (NULL);
    
    // İlk token'ın tipini kaydet (expansion için)
    first_type = (*current)->type;
    
    // İlk token'ı başlangıç olarak al
    filename = ft_strdup((*current)->value, shell);
    if (!filename)
        return (NULL);
    
    token_ptr = *current;
    
    // SONRAKI token'ın space_flag'ini kontrol et ve REDIRECTION olmadığından emin ol
    while (token_ptr->next && 
           token_ptr->next->space_flag == 0 &&
           token_ptr->next->type != PIPE &&           // Pipe değil
           token_ptr->next->type != REDIRECT_IN &&    // Redirection değil
           token_ptr->next->type != REDIRECT_OUT &&
           token_ptr->next->type != REDIRECT_APPEND &&
           token_ptr->next->type != HEREDOC)
    {
        token_ptr = token_ptr->next;
        temp = ft_strjoin(filename, token_ptr->value, shell);
        filename = temp;
        if (!filename)
            return (NULL);
    }
    
    // *** YENİ: Filename'i expand et ***
    // Basit expansion logic - tam ft_expand_token_value yerine
    if (first_type == VARIABLE)
    {
        int i = 0;
        expanded_filename = ft_extract_and_expand_var(filename, &i, shell);
    }
    else if (first_type == WORD && filename[0] == '~')
    {
        char *home = ft_get_env_value("HOME", shell->env_list);
        if (home && ft_strcmp(filename, "~") == 0)
            expanded_filename = ft_strdup(home, shell);
        else
            expanded_filename = filename;
    }
    else if (first_type == DOUBLE_QUOTED_STRING)
    {
        expanded_filename = ft_expand_double_quoted(filename, shell);
    }
    else
    {
        expanded_filename = filename; // WORD, SINGLE_QUOTED - expansion yok
    }
    
    *current = token_ptr;
    return (expanded_filename);
}

int	ft_in_parser_handle_redirect_in(t_cmd *cmd, t_token **current, t_shell *shell)
{
	char *expanded_filename;
	t_token_type first_type;
	
	//printf("DEBUG REDIRECT_IN: Before - current type=%d, value='%s'\n", 
	       //(*current)->type, (*current)->value ? (*current)->value : "NULL");
	
	*current = (*current)->next; // '<' operatöründen sonraki token'a geç
	
	//printf("DEBUG REDIRECT_IN: After next - current type=%d, value='%s'\n", 
	      // *current ? (*current)->type : -1, 
	       //(*current && (*current)->value) ? (*current)->value : "NULL");
	
	if (*current && ((*current)->type == WORD ||
				(*current)->type == SINGLE_QUOTED_STRING ||
				(*current)->type == DOUBLE_QUOTED_STRING ||
				(*current)->type == VARIABLE))  // VARIABLE kontrol et
	{
		//printf("DEBUG REDIRECT_IN: Processing filename\n");
		first_type = (*current)->type;
		
		// Bitiştik token'ları birleştir VE expand et
		expanded_filename = ft_join_redirect_filename(current, shell);
		if (!expanded_filename)
			return (0);
			
		cmd->input_file = expanded_filename;
		cmd->input_type = first_type;
		
		//printf("DEBUG REDIRECT_IN: Set input_file='%s'\n", cmd->input_file);
		
		*current = (*current)->next;
		
		//printf("DEBUG REDIRECT_IN: Final current type=%d, value='%s'\n", 
		  //     *current ? (*current)->type : -1, 
		    //   (*current && (*current)->value) ? (*current)->value : "NULL");
	}
	// else
	// {
	// 	printf("DEBUG REDIRECT_IN: No valid filename token found\n");
	// }
	return (1);
}

int ft_in_parser_handle_redirect_out(t_cmd *cmd, t_token **current, t_shell *shell)
{
    char **new_files;
    int *new_modes;
    char *joined_filename;
    t_token_type first_type;
    
    *current = (*current)->next;
    if (*current && ((*current)->type == WORD ||
            (*current)->type == SINGLE_QUOTED_STRING ||
            (*current)->type == DOUBLE_QUOTED_STRING))
    {
        first_type = (*current)->type;
        
        // Bitişik token'ları birleştir
        joined_filename = ft_join_redirect_filename(current, shell);
        if (!joined_filename)
            return (0);
        
        // Son dosyayı cmd->output_file'a kaydet (mevcut davranış)
        cmd->output_file = joined_filename;
        cmd->output_type = first_type;
        cmd->append_mode = 0;
        
        // Tüm dosyaları all_output_files dizisine ekle
        new_files = ft_malloc(sizeof(char *) * (cmd->output_count + 1), shell);
        new_modes = ft_malloc(sizeof(int) * (cmd->output_count + 1), shell);
        
        if (!new_files || !new_modes)
            return (0);
            
        // Eski dosyaları kopyala
        for (int i = 0; i < cmd->output_count; i++)
        {
            new_files[i] = cmd->all_output_files[i];
            new_modes[i] = cmd->all_append_modes[i];
        }
        
        // Yeni dosyayı ekle
        new_files[cmd->output_count] = ft_strdup(joined_filename, shell);
        new_modes[cmd->output_count] = 0; // Normal redirect
        
        cmd->all_output_files = new_files;
        cmd->all_append_modes = new_modes;
        cmd->output_count++;
        
        // ft_join_redirect_filename zaten current'i son token'a getiriyor
        *current = (*current)->next;
    }
    return (1);
}

int ft_in_parser_handle_redirect_append(t_cmd *cmd, t_token **current, t_shell *shell)
{
    char **new_files;
    int *new_modes;
    char *joined_filename;
    t_token_type first_type;
    
    *current = (*current)->next;
    if (*current && ((*current)->type == WORD ||
                (*current)->type == SINGLE_QUOTED_STRING ||
                (*current)->type == VARIABLE ||
                (*current)->type == DOUBLE_QUOTED_STRING))
    {
        first_type = (*current)->type;
        
        // Bitişik token'ları birleştir
        joined_filename = ft_join_redirect_filename(current, shell);
        if (!joined_filename)
            return (0);
        
        // Son dosyayı cmd->output_file'a kaydet (mevcut davranış)
        cmd->output_file = joined_filename;
        cmd->output_type = first_type;
        cmd->append_mode = 1;
        
        // Tüm dosyaları all_output_files dizisine ekle
        new_files = ft_malloc(sizeof(char *) * (cmd->output_count + 1), shell);
        new_modes = ft_malloc(sizeof(int) * (cmd->output_count + 1), shell);
        
        if (!new_modes || !new_modes)
            return (0);
            
        // Eski dosyaları kopyala
        for (int i = 0; i < cmd->output_count; i++)
        {
            new_files[i] = cmd->all_output_files[i];
            new_modes[i] = cmd->all_append_modes[i];
        }
        
        // Yeni dosyayı ekle
        new_files[cmd->output_count] = ft_strdup(joined_filename, shell);
        new_modes[cmd->output_count] = 1; // Append mode
        
        cmd->all_output_files = new_files;
        cmd->all_append_modes = new_modes;
        cmd->output_count++;
        
        // ft_join_redirect_filename zaten current'i son token'a getiriyor
        *current = (*current)->next;
    }
    return (1);
}

int	ft_in_parser_handle_heredoc(t_cmd *cmd, t_token **current, t_shell *shell)
{
	int temp_fd;
	char *joined_delimiter;
	t_token_type first_type;
    int has_quotes;

	*current = (*current)->next;
	if (*current)
	{
		// İlk token'ın tipini kaydet (expansion kontrolü için)
		first_type = (*current)->type;
		
		// Bitişik token'ları birleştir
		joined_delimiter = ft_join_heredoc_delimiter(current, shell, &has_quotes);
		if (!joined_delimiter)
			return (0);
		
		// Her heredoc için input al ve sadece sonuncusunu tut
		temp_fd = ft_process_heredoc(joined_delimiter, shell, !has_quotes);

		// Önceki heredoc fd varsa kapat
		if (cmd->heredoc_fd != -1)
		{
			close(cmd->heredoc_fd);
		}
		// Yeni fd'yi ata
		cmd->heredoc_fd = temp_fd;
		cmd->heredoc_delimiter = joined_delimiter;
		cmd->heredoc_type = first_type;
		cmd->heredoc_should_expand = !has_quotes;
		
		// ft_join_heredoc_delimiter zaten current'i son token'a getiriyor
		*current = (*current)->next;
	}
	return (1);
}
