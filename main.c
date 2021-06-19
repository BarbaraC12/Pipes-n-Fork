#include <errno.h>
#include <fcntl.h>   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* PART 1 HOW WORK fork() FUNCTION
int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	int i = fork();
	int j = -1;
	if(i != 0)
		j = fork();
	printf("hello id fork1= %d, id fork2= %d\n", i, j);	// end test 3 re-fork only parent fork
	// fork();
	// fork();						// 2 fork print 4 'Hello' in shell
	// fork();
	// fork();						// 4 fork print 16 'Hello' in shell
	// printf("hello\n");			// end test 2 multi-fork without condition
	// int i = fork();
	// printf("Hello from %d, process: ", i);
	// if(i == 0)
	// 	printf("Child\n");
	// else
	// 	printf("Main\n");

	// int j = fork();
	// printf("Hello 2 from %d, process: ", j);
	// if(j == 0)
	// 	printf("Child %d\n", i);
	// else
	// 	printf("Main %d\n", i);		// end test 1 fork in fork
	return 0;
}
*/

/* PART 2 WAIT FUNCTION
int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	int id = fork();
	int n;
	if (id == 0)
		n = 1;
	else{
		n = 6;
		wait(NULL);			// Parent will stand for end of child execution 
	}
	int i;
	for (i = n; i < n + 5; i++){
		printf("%d ", i);
		fflush(stdout);
	}
	if (id != 0)
		printf("end of line \n");
	return 0;
}
*/

/* PART 3 VISUALIZE FORK 
int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	int id = fork();	// Separate program in 2 who will run in the same time
	int n;
	if (id == 0)		// Child process id always 0
		n = 1;
	else{				// Parent process id
		n = 6;
		wait(NULL);			// Parent will stand for end of child execution 
	}
	int i;
	for (i = n; i < n + 5; i++){
		printf("%d ", i);
		fflush(stdout);
	}
	if (id != 0)
		printf("\n");
	return 0;
}
*/

/* PART 4 PROCESS ID
int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	int id = fork();

	if (id == 0){
		sleep(2);
	}
		printf("CurrentID: %d, ParentID: %d ChildID: %d.\n", getpid(), getppid(), id);		// return the ID of calling process
	// if (id != 0){
	// 	wait(NULL);
	// }
	int res = wait(NULL);
	if (res == -1)
		printf("Not anymore children to wait for ");
	else
		printf("| %d finished execution\n", res);
	//kill(id, SIGTERM);
	return 0;
}
*/

/* PART 5 CALL fork MULTIPLE TIME
int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	int id1 = fork();
	int id2 = fork();
	if (id1 == 0) {
		if (id2 == 0)
			printf("I'm process y: ParentID- %d, ChildID- %d\n", id1, id2);
		else
			printf("I'm process x: ParentID- %d, ChildID- %d\n", id1, id2);
	}
	else {
		if (id2 == 0)
			printf("I'm process z: ParentID- %d, ChildID- %d\n", id1, id2);
		else 
			printf("I'm parent process: ParentID- %d, ChildID- %d\n", id1, id2);
	}
	while (wait(NULL) != -1 || errno != ECHILD)
		printf("Wait for children to finish...\n");
	return 0;
}
*/

/* PART 6 communicate btw process (PIPE)
int	main(int argc, char **argv, char **envp)
{
	int		fds[2];	//fd[0] - read & fd[1] - write
	pid_t	cpid;
	int		x, y;

	(void)envp;
	(void)argv;
	if (argc != 5 && !BONUS)
		exit(EXIT_SUCCESS);
	if (pipe(fds) == -1){
		perror("pipe");
		exit(EXIT_FAILURE);
    }
	cpid = fork();
		if (cpid == -1){
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (cpid == 0) {			// fils
		close(fds[0]);
		printf("Type a number to send to parent: ");
		scanf("%d", &x);
		write(fds[1], &x, sizeof(int));
		close(fds[1]);
	}
	else {						// le pere ecrit argv[1] dans le tube  
		close(fds[1]);
		read(fds[0], &y, sizeof(int));
		close(fds[0]);
		printf("From child : %d \n", y);
	}
	return (0);
}
*/

/* PART 7 PRATICAL USE FOR FORK AND PIPE
int	main(int argc, char **argv, char **envp)
{
	int		fds[2];	//fd[0] - read & fd[1] - write
	pid_t	cpid;
	int		x, y;
	int		arr[] = {1, 5, 2, 6, 1, 3};
	int		size_arr = sizeof(arr) / sizeof(int);

	(void)envp;
	(void)argv;
	if (argc != 5 && !BONUS)
		exit(EXIT_SUCCESS);
	if (pipe(fds) == -1){
		perror("pipe");
		exit(EXIT_FAILURE);
    }
	cpid = fork();
		if (cpid == -1){
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (cpid == 0) {			// fils
		x = 0;
		y = size_arr / 2;
	}
	else {						// le pere ecrit argv[1] dans le tube  
		x = size_arr / 2;
		y = size_arr;
	}
	int sum = 0;
	int i;
	for (i = x; i < y; i++){
		sum += arr[i];
	}
	printf("Calculate partial sum: %d \n", sum);

	if(cpid == 0){
		close(fds[0]);
		write(fds[1], &sum, sizeof(sum));
		close(fds[1]);
	}
	else{
		int	child_sum;
		close(fds[1]);
		read(fds[0], &child_sum, sizeof(child_sum));  // will read and attribute child sum from fd[0]
		close(fds[0]);

		int	total = sum + child_sum;
		printf("Total sum of array is: %d\n", total);;
		wait(NULL);
	}

	return (0);
}
*/

/* PART 8 INTRODUCTION TO FIFO (PIPES)
*/
/* PART 8 INTRODUCTION TO FIFO (PIPES)
*/
int	main(int argc, char **argv){
	(void)argc;
	(void)argv;
	if (mkfifo("fifo1", 0777) == -1) {
		if (errno != EEXIST) {
			printf("Can't create file\n");
			return 1;
		}
	}
	return 0;
}
