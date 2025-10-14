/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranhaia- <ranhaia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:50:06 by ranhaia-          #+#    #+#             */
/*   Updated: 2025/10/14 15:44:44 by ranhaia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int	main(void)
{
	int	id = fork();
	int	n;
	if (id == 0)
		n = 1;
	else
		n = 6;
	if (id != 0)
		wait(NULL);
	for (int i = n; i < n + 5; i++)
	{
		printf("%d ", i);
	}
	if (id != 0)
		printf("\n");
	// printf("oi: %d\n", id);
	return (0);
}
