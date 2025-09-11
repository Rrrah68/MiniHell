








/* ************************************************************************** */

#include "minishell.h"

t_env	*create_env_node(const char *key, const char *value)
{
	t_env	*new_node;

	new_node = ft_calloc(1, sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	if (!new_node->key)
	{
		free(new_node);
		return (NULL);
	}
	if (value)
		new_node->value = ft_strdup(value);
	else
		new_node->value = NULL;
	if (value && !new_node->value)
	{
		free(new_node->key);
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

static void	parse_env_string(char *env_str, char **key, char **value)
{
	char	*delimiter;
	int		key_len;

	*key = NULL;
	*value = NULL;
	delimiter = ft_strchr(env_str, '=');
	if (delimiter)
	{
		key_len = delimiter - env_str;
		*key = ft_substr(env_str, 0, key_len);
		if (!*key)
			return ;
		*value = ft_strdup(delimiter + 1);
		if (!*value)
		{
			free(*key);
			*key = NULL;
		}
	}
	else
	{
		*key = ft_strdup(env_str);
		*value = NULL;
	}
}

static t_env	*process_env_entry(char *env_str)
{
	char	*key;
	char	*value;
	t_env	*new_node;

	parse_env_string(env_str, &key, &value);
	if (!key)
		return (NULL);
	new_node = create_env_node(key, value);
	free(key);
	if (value)
		free(value);
	return (new_node);
}

t_env	*copy_environment(char **envp)
{
	t_env	*env_list;
	t_env	*new_node;
	int		i;

	if (!envp)
		return (NULL);
	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		new_node = process_env_entry(envp[i]);
		if (new_node)
		{
			new_node->next = env_list;
			env_list = new_node;
		}
		i++;
	}
	return (env_list);
}
