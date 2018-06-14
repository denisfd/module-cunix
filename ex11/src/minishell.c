#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

char **g_envv;

void	exit_shell(void) {
	free(g_envv);
	write(1, "\n", 1);
	exit(0);
}

void	init_envv(int args, char **argv, char **envv) {
	int		i;
	(void)args;
	(void)argv;
	g_envv = (char **)malloc(sizeof(char *) * (strlen(*envv) + 1));
	i = -1;
	while (envv[++i]) {
		if (!(g_envv[i] = strdup(envv[i])))
			exit_shell();
	}
}

static void	 get_input(char **input) {
	int		ret;
	char	buf;
	int		i;
	int		count;

	*input = (char *)malloc(sizeof(char) + 1);
	count = 1;
	i = 0;
	while ((ret = read(0, &buf, 1)) && buf != '\n') {
		*(*input + i++) = buf;
		*input = realloc(*input,count + 1);
		count++;
	}
	*(*input + i) = '\0';
	if (!ret) {
		free(*input);
		exit_shell();
	}
	if ((strchr(*input, '$') != NULL) || (strchr(*input, '~') != NULL)) {}
//	  	*input = parse_input(*input);
}

void	display_msg(void) {
	char	*cwd;
	char	buff[4096 + 1];
	char	*parsed_cwd;

	cwd = getcwd(buff, 4096);
  write(1,cwd,strlen(cwd));
	free(parsed_cwd);
  write(1, " $ ",strlen(" $ "));
}

void	signal_handler(int signo) {
	if (signo == SIGINT) {
		puts("\n");
  	display_msg();
		signal(SIGINT, signal_handler);
	}
}


int main(int argc, char **argv, char **envv) {
  char	*input;
  int		ret;
  char	**commands;
  init_envv(argc,argv,envv);
  while(1) {
    display_msg();
    signal(SIGINT, signal_handler);
    get_input(&input);
  }
  free(g_envv);
  return 0;
}
