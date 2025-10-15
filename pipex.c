/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranhaia- <ranhaia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:50:06 by ranhaia-          #+#    #+#             */
/*   Updated: 2025/10/15 18:16:17 by ranhaia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*concat_cmd(char *path, char *cmd)
{
	char	*command;
	int		i;
	int		j;
	int		lencmd;
	int		lenpath;

	lencmd = ft_strlen(cmd);
	lenpath = ft_strlen(path);
	command = ft_calloc(lenpath + lencmd + 2, sizeof(char));
	i = 0;
	j = 0;
	while (i < lenpath)
	{
		command[i] = path[i];
		i++;
	}
	command[i++] = '/';
	while (i < lenpath + lencmd + 1 && cmd[j] != ' ')
		command[i++] = cmd[j++];
	return (command);
}

char	*find_path(char *command, char *envp)
{
	int		i;
	char	**paths;
	char	*pathname;

	i = 0;
	paths = ft_split(envp, ':');
	paths[i] = ft_strtrim(paths[i], "PATH=");
	while (command[i] != ' ')
		i++;
	command[i] = '\0';
	i = 0;
	while (paths[i])
	{
		pathname = concat_cmd(paths[i], command);
		if (access(pathname, X_OK) == 0)
			break ;
		i++;
	}
	return (pathname);
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*path;
	int		pid1;
	int		pid2;
	int		fdinfile;
	int		outfile;
	int		fd[2];
	int		i;

	i = 0;
	while (envp[i])
	{
			if (ft_strncmp(envp[i], "PATH=", 5) == 0)
				break ;
			i++;
	}
	path = find_path(argv[2], envp[i]);
	// printf("path: %s\n", find_path(argv[2], envp[i]));
	if (argc != 5)
		return (1);
	if (pipe(fd) == -1)
		return (1);
	pid1 = fork();
	if (pid1 == 0)
	{
		fdinfile = open(argv[1], O_RDONLY);
		if (fdinfile == -1)
			return (1);
		dup2(fdinfile, 0);
		dup2(fd[1], 1);
		close(fd[0]);
		close(fd[1]);
		close(fdinfile);
		execve(path, argv, envp);
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		dup2(fd[0], 0);
		outfile = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0777);
		if (outfile == -1)
			return (1);
		dup2(outfile, 1);
		close(fd[1]);
		close(fd[0]);
		close(outfile);
		execve(path, argv, envp);
	}
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	close(fd[0]);
	close(fd[1]);
	printf("Success\n");
	return (0);
}
