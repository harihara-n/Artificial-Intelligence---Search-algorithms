/* Heuristic Search Algorithm for the Stacking Problem */

# include <stdio.h>
# include <stdlib.h>
# include <list>
# include <algorithm>
# include <time.h>	
# include <queue>

# define NO_STACKS 3

using namespace std;

int factorial (int i)
{
	if(i==1)
		return 1;
	return i*factorial(i-1);
}		


struct state
{

	private:

		state* parent;
		int total_score;
		int g_score; 
				

	public:
		list<int> stack[NO_STACKS];     //stacks in a single state.		
		state()
		{
		}

		state (state* pr)
		{
			this->parent = pr;
		}

			
		state (int no_blocks, state*pr)
		{
						
			list<int> block_list;
			list<int>::iterator it;
			int i,random_block,j;

			for(i=1;i<=no_blocks;i++)
				block_list.push_back(i);

			srand(time(NULL));
		
			for(i=0;i<no_blocks;i++)
			{	
				random_block = rand()%((int)block_list.size());
				it=block_list.begin();j=0;
				while(j<random_block)
				{	
					it++;j++;
				}	
				stack[rand()%NO_STACKS].push_back(*it);
				block_list.erase(it);					
			}

			this->parent=pr;

			this->calculate_score();
		}

		void set_parent (state* sp)
		{
			this->parent = sp;
		}

		state* get_parent ()
		{
			return this->parent;
		}

		list<int> get_stack (int stack_no)
		{
			if (stack_no < 0 || stack_no > NO_STACKS)
			{
				printf("stack no requested is wrong %d", stack_no);
				exit(1);
			}			
			return stack[stack_no];
		}	

		void init_stacks(state* pr)
		{
			int i;
			for(i=0;i<NO_STACKS;i++)
				stack[i].assign(pr->stack[i].begin(),pr->stack[i].end());

			//printf("DONE");
			calculate_score();
		} 

		void calculate_score () //Sets score for this particular state.
		{
			
			int no_of_stacks_used=0;
			int i,first_val,second_val;
			list<int>::iterator it;

			this->total_score = 0;
			if (!(this->get_parent()))
				this->g_score = 0;
			else
				this->g_score = ((this->get_parent())->get_gscore())+1;

			//for(i=0;i<NO_STACKS; i++)
			//{
			//	if (!stack[i].empty())
			//		no_of_stacks_used ++;
			//}

			(this->total_score) += (no_of_stacks_used-1);	

			for(i=0;i<NO_STACKS;i++)
			{
				if (stack[i].empty())
					continue;;
			
				it = stack[i].begin();
				
				do
				{
					first_val = *it;
					it++;
					if (it == stack[i].end())
						break;	
					second_val= *it;
					if (!(first_val == second_val-1))
						(this->total_score) ++;
				}
				while (1);				
			}

			this->total_score += this->g_score;	
			
		}			

		int get_score()
		{
			return this->total_score;
		}

		int get_gscore()
		{
			return this->g_score;
		}

		bool isGoal ()
		{

			int no_empty_stacks;
			int i,filled_stack;
			list<int>::iterator it;


			for(i=0;i<NO_STACKS;i++)
			{
				if(!stack[i].empty())
				{
					no_empty_stacks++;
					if (no_empty_stacks > 1)
						return false;
					filled_stack = i;
				}
			}

			for(it=stack[filled_stack].begin(),i=1;it!=stack[filled_stack].end();i++,it++)
			{
				if(!((*it) == i))
					return false;
			}
		
			return true;
		}

		bool isSame(state* s)
		{
			int i;
			list<int>::iterator it1,it2;
			
			for(i=0;i<NO_STACKS;i++)
			{
				if (stack[i].size() != s->stack[i].size())
					return false;
				for(it1=stack[i].begin(),it2=(s->stack[i]).begin();it1!=stack[i].end();it1++,it2++)
				{
					if((*it1) != (*it2))
						return false;
				}
			}
			
			return true;
		}
			

		void move (int from_stack, int to_stack, int no_blocks)  //Move no_blocks from from_stack to to_stack
		{

			if (from_stack > NO_STACKS - 1  || from_stack < 0 || to_stack > NO_STACKS -1 || to_stack < 0)
			{
				printf("Invalid stack number arguments\n");
				exit(0);
			}

			if (no_blocks > ((int)stack[from_stack].size())) 
			{
				printf("Invalid stack size argument\n");
				exit(0);
			}
			
			list<int> :: iterator it;
			int count = 0;

			for(it= stack[from_stack].begin(); count < ( (int)stack[from_stack].size() - no_blocks) ; count++, it++);

			stack[to_stack].splice(stack[to_stack].end(), stack[from_stack], it, stack[from_stack].end()); //transfer the last no_blocks from from_stack to to_stack.
			this->calculate_score();
			return;
		}

		void display ()
		{
			int i; list<int>:: iterator it;
			
			for(i=0; i < NO_STACKS ; i++)
			{
				it = stack[i].begin();
				printf("\nStack %d\n",i+1);

				while (it !=stack[i].end())
				{
					printf("%d\n",*it);
					it++;
				}
			}

			printf("\nTotal Score is %d\n", this->total_score);
		}
};


struct compare
{
	bool operator()(state* s1, state* s2)
	{
		return (s1->get_score())>(s2->get_score());
	}
};

int main()
{
	int blocks;	
	int i,j,k;	
		
	printf("Enter the number of blocks\n");
	scanf("%d", &blocks);

	const int no_blocks = blocks;	

	printf("Starting state\n");
	state* original_state = new state(no_blocks,NULL);
	
	original_state->display();
	
	//state* test = new state(original_state);
	//test->init_stacks(original_state);
	//test->display();
	//return 1;		
			
	state* temp_state = original_state;
	state* least_temp_state;
	state* ch_temp;
	list<int>::iterator it;

	priority_queue <state*, vector<state*>, compare> pq;
	
	int no_steps = 0;
	bool is_same;
	
	while(!(temp_state->isGoal()))
	{
		no_steps++;		
		int least_cost = NO_STACKS+10;
		
		for(i=0;i<NO_STACKS;i++)
		{	
			for(j=1;j<=((temp_state->stack[i]).size());j++)
			{	
				for(k=0;k<NO_STACKS;k++)
				{	
					if (i == k)
						continue;					
					state* tstate = new state(temp_state);
					tstate->init_stacks(temp_state);					
					
					tstate->move(i,k,j);
					//printf("SCORE: %d",tstate->get_score());
					ch_temp = tstate->get_parent();
					is_same = false;
					while(ch_temp)
					{
						if (tstate->isSame(ch_temp))
						{
							is_same = true;
							break;
						}
						
						ch_temp = ch_temp->get_parent();
					}
					if (is_same)
						continue;
					pq.push(tstate);									
				}
			}
		}
		temp_state =  pq.top();
		pq.pop();
		
	}

	if ((temp_state->get_stack(NO_STACKS - 1)).empty()) //If true - not in rightmost stack
		no_steps++;	
		

	printf("\nGoal State reached in %d\n", no_steps);

	return 1;		

}					
			


			
			 
			
					
	
			

	
	 

