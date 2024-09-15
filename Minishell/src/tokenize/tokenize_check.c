/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:25:04 by mmachlou          #+#    #+#             */
/*   Updated: 2024/09/15 10:46:29 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	check_type(char *token)
{
	char	*cmd_path;

	if (ft_strncmp(token, "/", 1) == 0
		|| ft_strncmp(token, "./", 2) == 0
		|| ft_strncmp(token, "../", 3) == 0)
	{
		if (access(token, X_OK) == 0)
			return (CMND);
		else
			return (UNKNOWN);
	}
	cmd_path = find_in_path(token);
	if (cmd_path != NULL)
	{
		free(cmd_path);
		return (CMND);
	}
	else if (ft_strcmp(token, "|") == 0)
		return (PIPE);
	else if (ft_strcmp(token, "<") == 0
		|| ft_strcmp(token, ">") == 0
		|| ft_strcmp(token, ">>") == 0)
		return (REDIRECT_OUT);
	else if (ft_strcmp(token, "&") == 0)
		return (UNKNOWN);
	else if (ft_strncmp(token, "$", 1) == 0)
		return (VARIABLE);
	else if (ft_strcmp(token, "'") == 0 || ft_strcmp(token, "\"") == 0)
		return (QUOTE);
	else if (ft_strcmp(token, ";") == 0)
		return (UNKNOWN);
	else if (ft_strcmp(token, "<<") == 0)
		return (HEREDOC);
	return (UNKNOWN);
}

static int	check_for_quotations(char *input)
{
	int	i;
	int	single_quote_open;
	int	double_quote_open;

	i = 0;
	single_quote_open = 0;
	double_quote_open = 0;
	while (input[i])
	{
		if (input[i] == '\\' && !single_quote_open)
			i += 2;
		if (input[i] == '"' && !single_quote_open)
			double_quote_open = !double_quote_open;
		else if (input[i] == '\'' && !double_quote_open)
			single_quote_open = !single_quote_open;
		i++;
	}
	if (single_quote_open || double_quote_open)
		return (0);
	return (1);

}

void	check(char *input, char **env)
{
	t_token	*token;

	token = NULL;
	(void)env;
	tokenize_input(input, &token);
	if (!check_for_quotations(input))
	{
		printf("Syntax error\n");
		free(token);
	}
	else
	{
		// while(token)
		// {
		// 	printf("%s\n", token->tokens);
		// 	token = token->next;
		// }
		if (token)
		{
			if (token->token_type == CMND)
				check_cmnd(input, token, env);
			else if ((ft_strcmp(token->tokens, "cd") == 0))
				ft_cd(token);
			else if (token->token_type == UNKNOWN)
				printf("%s : Command not found\n", token->tokens);
		}
		free_token_list(token);
	}
}