/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranhaia- <ranhaia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:50:06 by ranhaia-          #+#    #+#             */
/*   Updated: 2025/10/23 21:36:37 by ranhaia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_split(char **line)
{
	int	i;

	i = 0;
	while (line[i] != NULL)
		free(line[i++]);
	free(line);
}

char	*find_path(char *command, char *envp)
{
	int		i;
	char	**paths;
	char	**splitted_command;
	char	*pathname;
	char	*temp;

	paths = ft_split(envp, ':');
	temp = ft_strtrim(paths[0], "PATH=");
	free(paths[0]);
	paths[0] = temp;
	splitted_command = ft_split(command, ' ');
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i++], "/");
		pathname = ft_strjoin(temp, splitted_command[0]);
		free(temp);
		if (access(pathname, X_OK) == 0)
			break ;
		free(pathname);
	}
	free_split(paths);
	free_split(splitted_command);
	return (pathname);
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*path;
	int		pid1;
	int		pid2;
	int		fdinfile;
	int		outfile;
	char	**cmd_args;
	int		pipefd[2];
	int		i;

	if (argc != 5)
		return (1);
	if (pipe(pipefd) == -1)
		return (1);
	fdinfile = open(argv[1], O_RDONLY);
	if (fdinfile < 0)
		exit(1);
	outfile = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0777);
	if (outfile < 0)
		exit(1);
	i = -1;
	while (envp[++i])
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			break ;
	pid1 = fork();
	if (pid1 == 0)
	{
		dup2(fdinfile, 0);
		dup2(pipefd[1], 1);
		close(pipefd[0]);
		close(pipefd[1]);
		if (argv[2][0] == '/')
			path = argv[2];
		else
			path = find_path(argv[2], envp[i]);
		if (access(path, X_OK) != 0)
			exit(127);
		cmd_args = ft_split(argv[2], ' ');
		if (execve(path, cmd_args, envp) == -1)
			free_split(cmd_args);
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		dup2(pipefd[0], 0);
		dup2(outfile, 1);
		close(pipefd[0]);
		close(pipefd[1]);
		if (argv[2][0] == '/')
			path = argv[2];
		else
			path = find_path(argv[3], envp[i]);
		if (access(path, X_OK) != 0)
			exit(127);
		cmd_args = ft_split(argv[3], ' ');
		if (execve(path, cmd_args, envp) == -1)
			free_split(cmd_args);
	}
	close(fdinfile);
	close(outfile);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	printf("Success\n");
	return (0);
}
