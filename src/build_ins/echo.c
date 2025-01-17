/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 10:08:35 by mohamibr          #+#    #+#             */
/*   Updated: 2024/12/26 11:11:39 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static bool	check_n(t_token **token)
{
	bool	n;
	int		i;

	n = false;
	while (*token)
	{
		if ((*token)->tokens[0] == '-' && (*token)->tokens[1] == 'n')
		{
			i = 1;
			while ((*token)->tokens[++i] == 'n')
				;
			if ((*token)->tokens[i] != '\0')
				break ;
			n = true;
			*token = (*token)->next;
		}
		else
			break ;
	}
	return (n);
}

void	echo_loop(int *first, t_token *token, t_env_cpy *env_list)
{
	char	*output;

	if (!*first)
		ft_printf(" ");
	if (token->qoute_type == '\'')
		output = ft_strdup(token->tokens);
	else
		output = expand_token_if_variable(token->tokens, env_list);
	ft_printf("%s", output);
	*first = 0;
	free(output);
}

void	check_echo(t_token *token, t_env_cpy *env_list)
{
	bool	n;
	int		first;

	if (!token->next)
	{
		ft_printf("\n");
		env_list->last_exit_status = 0;
		return ;
	}
	token = token->next;
	n = check_n(&token);
	first = 1;
	while (token)
	{
		echo_loop(&first, token, env_list);
		token = token->next;
	}
	if (!n)
		ft_printf("\n");
	env_list->last_exit_status = 0;
}
