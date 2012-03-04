# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <queue>
# include <stack>

using namespace std;

class board
{
	int a[3][3]; //3*3 matrix to represent the board.
	int blank_space; //position of blank space (0 - 8).
	int depth; //depth of the node in the search tree - used for iterative deepening.
	
	board* parent;

	public:

	board() //goal state - default constructor
	{
		int i,j,count=0;
		for(i=0;i<3;i++)
		{
			for(j=0;j<3;j++)
			{
				if (i==1 && j==1)
					a[i][j] = 0;
				else
					a[i][j] = ++count;
			}
		}
		blank_space = 4;
	}

	board (board* b) //Constructor to copy from a previous board.
	{
		int pos;
		for (pos=0;pos<=8;pos++)
			a[pos/3][pos%3] = b->a[pos/3][pos%3];
		blank_space = b->blank_space;
	}


	
	int return_pos (int pos) //function to return value at location pos.
	{
		return a[pos/3][pos%3];
	}

	int bs ()  //function to return blank space location.
	{
		return this->blank_space;
	}

	void set_parent(board* par)
	{
		this->parent = par;
	}

	board* get_parent()
	{
		return this->parent;
	}

	void set_depth(int dep)
	{
		this->depth = dep;
	}

	int get_depth ()
	{
		return this->depth;
	}	
	
	void swap(int blank, int pos)
	{
		if (blank != blank_space)
		{
			printf("Fault in generating swap function\nExiting...\n");
			exit(0);
		}

		int temp;
		a[blank/3][blank%3] = a[pos/3][pos%3];
		a[pos/3][pos%3] = 0;

		blank_space = pos;
	}

	void print()
	{
		int i,j;		
		for(i=0;i<3;i++)
		{
			for(j=0;j<3;j++)
			{
				printf("%d\t",a[i][j]);
			}
			printf("\n");
		}
		printf("\n\n");	
	}

	bool isGoal () //Checks if current state is Goal State.
	{
		int i,j,count=0;
		;

		for(i=0;i<3;i++)
		{
			for(j=0;j<3;j++)
			{
				if (i==1 && j==1)
				{
					if (a[i][j] !=	0)
						return false;
				}
			
				else
				{
					count++;
					if (a[i][j] != count)
						return false;
				}
			}
		}
		
		return true;
	}

	bool isEqual (board* b) //Checks if current board is same as board b.
	{		
		int i,j;
		
		for(i=0;i<3;i++)
		{
			for(j=0;j<3;j++)
			{
				if (a[i][j] != b->a[i][j])
					return false;
			}
		}
		return true;
	}

	void initial_state (int shuffle, bool show_shuffle) //function that generates a start state shuffled 'shuffle' times from goal state.
	{
		int no_shuffle;
		srand(time(NULL)); //initializing the seed for randomize function.
		int random_no;

		for(no_shuffle=0; no_shuffle<shuffle; no_shuffle++)
		{
			if (blank_space == 4)
			{
				random_no = rand() % 4; //Since there are four places for blank to move. 

				if (random_no == 0)
					swap (blank_space,1);
				else if (random_no == 1)
					swap (blank_space,3);
				else if (random_no == 2)
					swap (blank_space,5);
				else
					swap (blank_space,7);
			}
			
			else if (blank_space == 0 || blank_space == 2 || blank_space == 6 || blank_space == 8)
			{
				random_no = rand() % 2;
				
				if (random_no == 0)
				{
					if (blank_space == 0 || blank_space == 2)
						swap (blank_space, 1);
					else
						swap (blank_space, 7);
				}
				else
				{
					if (blank_space == 0 || blank_space == 6)
						swap (blank_space, 3);
					else
						swap (blank_space, 5);
				}
			}

			else
			{	
				random_no = rand() % 3;
				
				if(random_no == 0)
					swap (blank_space,4);
				else if (random_no == 1)
				{
					if (blank_space == 1 || blank_space == 3)
						swap(blank_space,0);
					else
						swap (blank_space, 8);
				}
				else
				{
					if (blank_space == 1 || blank_space == 5)
						swap (blank_space, 2);
					else
						swap (blank_space, 6);
				}
			}
			if (show_shuffle)
				this->print();
		}
		printf("\nInitial State\n");
		this->print();

	}

};


bool no_repeat (board* b) //Compare node b with all its ancestors. If repeat of any ancestor node return false, else return true.
{
	board* temp = b;
		
	while (temp->get_parent())
	{
		temp=temp->get_parent();
			

		if(b->isEqual(temp))
			return false;
	}
	return true;
}


void breadth_search (board* b, int* number_of_tests, int *max_size)   //Breadth First Search Code.
{
	board* temp = b;

	*max_size = 0;	
	*number_of_tests = 0;

	queue<board*> q;
	q.push(temp);
		
	while(!q.empty())
	{
		if (q.size()>(*max_size))
			(*max_size) = q.size();

		(*number_of_tests)++;
		
		if ((*number_of_tests) > 1000000)	//Too many tests performed. Integer becomes negative .Exit.
		{
			printf("Too complex to solve... failing..\n");
			return;
		}
			
		
		if (temp->isGoal())  //Check for Goal status in current node.
		{
			printf("Goal node reached in %d steps and max_size of %d nodes\n\n", *number_of_tests, *max_size);
			return;
		}
				
		if ((temp->bs())%3 != 0)
		{
			board *new_b = new board(temp);
			new_b->swap(new_b->bs(), (new_b->bs()) - 1);
			q.push(new_b);
		}
		
		if ((temp->bs())%3 != 2)
		{
			board *new_b = new board(temp);
			new_b->swap(new_b->bs(),(new_b->bs())+1);
			q.push(new_b);
		}

		if ((temp->bs())/3 != 0)
		{
			board *new_b = new board(temp);
			new_b->swap(new_b->bs(), (new_b->bs())-3);
			q.push(new_b);
		}

		if ((temp->bs())/3 != 2)
		{
			board *new_b = new board(temp);
			new_b->swap(new_b->bs(),(new_b->bs())+3);
			q.push(new_b);
		}
		
		temp = q.front();
		q.pop();
		
	}
}



void depth_search (board* b, int* number_of_tests, int *max_size)
{
	board* temp = b;
	temp->set_parent(NULL); //Set parent of goal node to be NULL.

	*max_size = 0;	
	*number_of_tests = 0;	

	stack<board*> s;
	s.push(temp);
		
	while(!s.empty())
	{
		if (s.size()>(*max_size))
			(*max_size) = s.size();

		(*number_of_tests)++;
		
		if ((*number_of_tests) > 100000) //Too many tests performed. Integer becomes negative .Exit.
                {
                        printf("Too complex to solve... failing..\n");
                        return;
                }
		
		
		if (temp->isGoal())
		{
			printf("Goal node reached in %d steps and max_size of %d nodes\n\n", *number_of_tests, *max_size);
			return;
		}
				
		if ((temp->bs())%3 != 0)
		{
			board *new_b = new board(temp);
			new_b->swap(new_b->bs(), (new_b->bs()) - 1);
			new_b->set_parent(temp);
		
			if (no_repeat(new_b))   //This function checks if temp is not a duplicate board arrangement of any of its ancestors. If we don't check this DFS may go on infinitely. 
				s.push(new_b);
		}
		
		if ((temp->bs())%3 != 2)
		{
			board *new_b = new board(temp);
			new_b->swap(new_b->bs(),(new_b->bs())+1);
			new_b->set_parent(temp);

			if (no_repeat(new_b))
				s.push(new_b);
		}

		if ((temp->bs())/3 != 0)
		{
			board *new_b = new board(temp);
			new_b->swap(new_b->bs(), (new_b->bs())-3);
			new_b->set_parent(temp);

			if (no_repeat(new_b))
				s.push(new_b);
		}

		if ((temp->bs())/3 != 2)
		{
			board *new_b = new board(temp);
			new_b->swap(new_b->bs(),(new_b->bs())+3);
			new_b->set_parent(temp);
			
			if (no_repeat(new_b))
				s.push(new_b);
		}
		
		temp = s.top();
		s.pop();
		
	}
}

void iterative_search (board* b, int* number_of_tests, int *max_size)
{
	
	int depth_iter = 0;
	int depth;
	*max_size = 0;	
	*number_of_tests = 0;
	

	while (1)
	{	
	
	depth = 0;	
	board* temp = b;
	temp->set_parent(NULL); //Set parent of goal node to be NULL.
	temp->set_depth(0);

	stack<board*> s;
	
	s.push(temp);
		
		
		while(!s.empty())
		{
			if ((temp->get_depth()) > depth_iter)
			{	
				while ((temp->get_depth() > depth_iter) && s.size() > 0)
				{
					temp = s.top();
					s.pop();
				}
				
			}
			if (s.size() == 0 && temp->get_depth() > depth_iter)
				break;
			
			if (s.size()>(*max_size))
				(*max_size) = s.size();

			(*number_of_tests)++;

			if ((*number_of_tests) > 100000)   //Too many tests performed. Integer becomes negative .Exit.
               		{
                        	printf("Too complex to solve... failing..\n");
                        	return;
               		}

		
			if (temp->isGoal())
			{
				printf("Goal node reached in %d steps and max_size of %d nodes\n\n", *number_of_tests, *max_size);
				return;
			}
				
			if ((temp->bs())%3 != 0)
			{
				board *new_b = new board(temp);
				new_b->swap(new_b->bs(), (new_b->bs()) - 1);
				new_b->set_parent(temp);
				new_b->set_depth(temp->get_depth() + 1);
		
				if (no_repeat(new_b))   //This function checks if temp is not a duplicate board arrangement of any of its ancestors. If we don't check this DFS may go on infinitely. 
					s.push(new_b);
			}
		
			if ((temp->bs())%3 != 2)
			{
				board *new_b = new board(temp);
				new_b->swap(new_b->bs(),(new_b->bs())+1);
				new_b->set_parent(temp);
				new_b->set_depth(temp->get_depth() + 1);

				if (no_repeat(new_b))
					s.push(new_b);
			}

			if ((temp->bs())/3 != 0)
			{
				board *new_b = new board(temp);
				new_b->swap(new_b->bs(), (new_b->bs())-3);
				new_b->set_parent(temp);
				new_b->set_depth(temp->get_depth() + 1);	

				if (no_repeat(new_b))
					s.push(new_b);
			}	
	
			if ((temp->bs())/3 != 2)
			{
				board *new_b = new board(temp);
				new_b->swap(new_b->bs(),(new_b->bs())+3);
				new_b->set_parent(temp);
				new_b->set_depth(temp->get_depth() + 1);
				
				if (no_repeat(new_b))
					s.push(new_b);
			}	
		
			temp = s.top();
			s.pop();
		
		}
		depth_iter++;
		if (s.empty())
			continue;

	}
	

}

	

		
			
int main()
{
	int shuffle; char ch; bool show_shuffle;
	char option;

	int number_of_tests, max_size;
	printf("Enter shuffle to generate initial state...\n");
	scanf("%d",&shuffle);

	while(getchar() != '\n');  //flushing the input buffer.
				

	while(shuffle > 100)
	{
		printf("Shuffle should be <= 100\n");
		scanf("%d", &shuffle);
	}
	

	printf("Do you want to see the shuffle? - Press y to do so\n");
	fflush(stdin);
	scanf("%c",&ch);

	while(getchar() != '\n');

	board *b = new board();
	if (ch == 'y')
		 show_shuffle = true;
	else
		 show_shuffle = false;
	b->initial_state(shuffle, show_shuffle);

	while(1)
	{
		printf("Press 'B' for BFS, 'D' for DFS, 'I' for IDS, 'q' to quit\n");
		scanf("%c",&option);
		while(getchar() != '\n');

		switch (option)
		{
			case 'B': breadth_search (b, &number_of_tests,&max_size);
				  break;
	
			case 'D': depth_search (b,&number_of_tests,&max_size);
				  break;
			
			case 'I': iterative_search (b, &number_of_tests, &max_size);
				  break;
	
			case 'q': return 1;
		
			default: printf("Invalid option\n");
				 break;
		}
	}				

}	

