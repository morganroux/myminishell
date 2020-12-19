/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkanaev <alkanaev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 17:01:16 by alkanaev          #+#    #+#             */
/*   Updated: 2020/12/19 11:11:31 by alkanaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

/*
** export без аргументов, то он просто выводит список всех переменных окружения, 
**        с приставкой declare -x, а еще он сортирует список в алфавитном порядке, например
```bash
declare -x COLORFGBG="15"
declare -x COLORTERM="truecolor"
declare -x COMMAND_MODE="unix2003"
declare -x HOME="/Users/annaleonova"
```

** export с аргументом без '=', то он просто печатает ее
** export без ключа, но со знаком = добавляет переменную в переменные окружения! 
**        при этом после знака = буду двойные кавычки. например, x= будет в списке.
** export по ключу, который уже существует, меняет значение этого ключа.
** если export аргумент (без равно), то он не добавляет это в переменные окружения 
**        (то есть в env_arr этой переменной нет), но при этом 

** export ZSH=/Users/dbliss/.oh-my-zsh
** export USER=dbliss
** export PWD=
** export OLDPWD (вот это не должно заноситься в env_arr, либо в конце ее удалить надо 
**        (или лучше просто не отображать в функции env_arr! типо распечатал и удалил из массива.)
```bash
declare -x ZSH="/Users/dbliss/.oh-my-zsh"
declare -x USER="dbliss"
declare -x PWD=""
declare -x OLDPWD
```
*/

int		space_checker(char *str)
{
	int i;

	i = 0;
	while (str[i] == ' ' || str[i] == '\t'
        || str[i] == '\n' || str[i] == '\r'
        || str[i] == '\v' || str[i] == '\f')
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

int		env_valid(char *env_arr)
{
	int		i;
	int		len;
	char	**sep;

	i = 0;
    if (env_arr[0] == '\0' || space_checker(env_arr) == 1)
		return (-4);
	if (ft_isdigit(env_arr[0]) == 1)
		return (-1);
	if (env_arr[0] == "=")
		return (-2);
	sep = split_mod(env_arr, "=");
	len = ft_strlen(sep[0]);
	while (sep[0][i])
	{
		if (ft_isalnum(sep[0][i]) == 0 && sep[0][len - 1] != '+')
		{
			arr_cleaner(sep);
			return (-3);
		}
		i++;
	}
	arr_cleaner(sep);
	return (1);
}

void		upd_newenv(t_command *mimi, char *env_update)
{
	char	**sep;
	char	*var;
	char	*val;
	char	*new_env;
	int		ret;

	sep = split_mod(env_update, "+=");
	var = ft_strdup(sep[0]);
	val = ft_strdup(sep[1]);
	arr_cleaner(sep);
	ret = ind_of_envvar(mimi, var);
	if (ret < 0)
		new_env = join_mod(var, "=", val);
	else
		new_env = ft_strjoin(mimi->env_arr[ret], val);
	add_env(mimi, new_env);
	strdel(&var);
	strdel(&val);
}

int		exp_err_case(t_command *mimi, char *cmd)
{
	int		ret;

	ret = env_valid(cmd);
	if (ret < 0)
	{
		ft_putstr_fd("MINISHELL: export: \" ", STDERR_FILENO);
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(" \" : not a valid identifier\n", STDERR_FILENO);
		mimi->ret = 1;
		return (1);
	}
	return (0);
}

/*
// in case of export of "addition value" 
ex:
bash-3.2$ export ok=v1
bash-3.2$ export ok+=v2
bash-3.2$ echo $ok
SO HERE IN THE END OUR ok = v1v2
*/
int		val_adder(char *var) 
{
	int		i;

	i = 0;
	while (var[i])
	{
		if (i != 0 && var[i - 1] == '+' && var[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

void			com_export(t_command *mimi, char **cmd)
{
	int		k;
	int		argc;

	argc = cnt_com_parts(cmd);
	mimi->ret = 0;
	if (argc == 1)
		envvar_pr_sort(mimi);
	else
	{
		k = 1;
		while (k < argc)
		{
			if (exp_err_case(mimi, cmd[k]) == 1)
				k++;
			else
			{
				if (val_adder(cmd[k]) > 0)
					upd_newenv(mimi, cmd[k]);
				else
					add_env(mimi, ft_strdup(cmd[k]));
				k++;
			}
		}
	}
}
