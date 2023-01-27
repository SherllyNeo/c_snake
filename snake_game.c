#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<termios.h>


#define COLS 150
#define ROWS 35
#define LIMIT 1000

int main() {
	//remove cursor
	printf("\e[?25]");


	//disable echo
	struct termios oldt, newt;
	tcgetattr(STDIN_FILENO,&oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO,TCSANOW,&newt);

	int x[LIMIT],y[LIMIT];
	int xdir = 1, ydir =0;
	int appleX = -1, appleY;
	int end_game = 0;
	while (!end_game) {
	//render grid
	    printf("┌");
	    for (int i = 0; i < COLS; i++)
	      printf("─");
	    printf("┐\n");

	    for (int j = 0; j < ROWS; j++) {
	      printf("│");
	      for (int i = 0; i < COLS; i++)
		printf("·");
	      printf("│\n");
	    }

	    printf("└");
	    for (int i = 0; i < COLS; i++)
	      printf("─");
	    printf("┘\n");

	    //move cursor to top
	    printf("\e[%iA",ROWS+2);


	    //init
	    int head = 0;
	    int tail = 0;
	    int gameover = 0;

	    x[head] = COLS / 2;
	    y[head] = ROWS / 2;


	    //game proper
	    while (!end_game && !gameover) {

	      if (appleX < 0) {
		// Create new apple
		appleX = rand() % COLS - 1;
		appleY = rand() % ROWS - 1;

		for (int i = tail; i != head; i = (i + 1) % 1000)
		  if (x[i] == appleX && y[i] == appleY)
		    appleX = -1;

		if (appleX >= 0) {
		  // Draw apple
		  printf("\e[%iB\e[%iC🍎", appleY + 1, appleX + 1);
		  printf("\e[%iF", appleY + 1);
		}
	      }

	      //clear snake tail
	      printf("\e[%iB\e[%iC·", y[tail] + 1, x[tail] + 1);
	      printf("\e[%iF", y[tail] + 1);


	      //logic for apple being eaten
	      if (x[head] == appleX && y[head] == appleY) {
		appleX = -1;
		printf("\a"); // Bell
	      } else
		tail = (tail + 1) % LIMIT;

	      int newhead = (head + 1) % LIMIT;
	      x[newhead] = (x[head] + xdir + COLS) % COLS;
	      y[newhead] = (y[head] + ydir + ROWS) % ROWS;
	      head = newhead;


	      for (int i = tail; i != head; i = (i + 1) % 1000)
		if (x[i] == x[head] && y[i] == y[head])
		  gameover = 1;

	      // Draw head
	      printf("\e[%iB\e[%iC▓", y[head] + 1, x[head] + 1);
	      printf("\e[%iF", y[head] + 1);
	      fflush(stdout);

	      usleep(5 * 1000000 / 60);

	      // Read keyboard
	      struct timeval tv;
	      fd_set fds;
	      tv.tv_sec = 0;
	      tv.tv_usec = 0;

	      FD_ZERO(&fds);
	      FD_SET(STDIN_FILENO, &fds);
	      select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
	      if (FD_ISSET(STDIN_FILENO, &fds)) {
		int ch = getchar();

		 if (ch == 27 || ch == 'q') {
		   end_game = 1;
		 } else if (ch == 'a' && xdir != 1) {
		   //left
		   xdir = -1;
		   ydir = 0;
		 } else if (ch == 'd' && xdir != -1) {
		   //right
		   xdir = 1;
		   ydir = 0;
		 } else if (ch == 's' && ydir != -1) {
		   //down
		   xdir = 0;
		  ydir = 1;
		 } else if (ch == 'w' && ydir != 1) {
		   //up
		   xdir = 0;
		   ydir = -1;
		 }


	      }
	    }

	    if (!end_game) {
	      // Show game over
	      printf("\e[%iB\e[%iC Game Over! ", ROWS / 2, COLS / 2 - 5);
	      printf("\e[%iF", ROWS / 2);
	      fflush(stdout);
	      getchar();
	    }


	}




















	  // Show cursor
	  printf("\e[?25h");

	  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	return 0;
}

