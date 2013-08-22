#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;
	char buf[80] = "";
	int bytes;

	if((fd = open("/dev/encoder", O_RDONLY)) < 0)
	{
		perror("failed to open /dev/encoder");
		exit(-1);
	}
	
	bytes = read(fd, buf, 10);
	if(bytes > 0)
	{
		buf[bytes] = '\0';
		printf("content: %s\n", buf);	
	}	

	close(fd);
	
	return 0;
}
