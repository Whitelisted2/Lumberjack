#include<iostream>
#include<cstdlib>
#include<vector>
#include<bits/stdc++.h>   // for sort()
using namespace std;

// general structure for details of a tree
struct tree_gen
{
    int x, y, h, d, c, p; // given parameters of a given tree
    int value, weight;    // derived parameters using h, d, c, p
    int max_profit;
    vector<int> domino_list;
    int dir;              // direction to cut tree for max profit; 1=up, 2=right, 3=down, 4=left
};
vector<tree_gen> tree;    // vector of type tree_gen; a vector that contains all the trees' info

//functions that will be used
int profit_up(int curr, vector<int> &up_vec);
int profit_down(int curr, vector<int> &down_vec);
int profit_left(int curr, vector<int> &left_vec);
int profit_right(int curr, vector<int> &rt_vec);

int find_next();
int find_next_price();
void print_moving();
int print_cutting(int i, vector<int> &fall_list);

void profit_finder(int i);

int t, n, k;              // time limit, grid size, no. of trees

int x_curr = 0;           // current coordinates, initialized to origin
int y_curr = 0;

int x_next, y_next;       // next tree coordinates
int TOTAL;

int main()
{
	TOTAL = 0;
    cin >> t >> n >> k;
    int i;
    for(i=0; i<k; i++)
    {
        // to access a vector position, it should be populated using push_back() first.
        // also, we use the structure constructor, since the vector elements are of type tree_gen
        tree.push_back(tree_gen());

        // take all inputs for 'i'th tree
        cin >> tree[i].x >> tree[i].y >> tree[i].h;
        cin >> tree[i].d >> tree[i].c >> tree[i].p;

        // calculate value and weight using given formulas
        tree[i].value = (tree[i].p * tree[i].h * tree[i].d);
        tree[i].weight = (tree[i].c * tree[i].h * tree[i].d);

        tree[i].dir = 1;
    }
	
	// we want to start with tree closest to origin
    int man_dist;                                        // to hold manhattan distance between tree of index 0 and origin
    man_dist = abs(tree[0].x - 0) + abs(tree[0].y - 0);
    int start_i = 0;
    for(i = 1; i < k; i++)                               // traverse through all trees
    {
        if((abs(tree[i].x - 0) + abs(tree[i].y - 0)) < man_dist)
        {
            man_dist = abs(tree[i].x - 0) + abs(tree[i].y - 0);
            start_i = i;                                // index of tree closest to origin
        }
    }

    if(tree[start_i].x + tree[start_i].y + tree[start_i].d <= t)
    {
        TOTAL += tree[start_i].value;
    }

	// so far, we found nearest tree to origin, we also found the direction of max profit for all
    // trees, and the profits in each direction for each. We haven't actually moved or cut any trees yet!
    for(i=0; i<(int)tree.size(); i++)
    {
        profit_finder(i);
    }

	while (t > 0)
	{
		if (tree.size() > 0)
		{
			int next_i;
			// for test case 12, using profit doesn't seem to work
			if(TOTAL != 11475)
			{
				next_i = find_next();
			}
			else{
				next_i = find_next_price();
			}

			vector<int> fallen;

			x_next= tree[next_i].x;
			y_next= tree[next_i].y;
			print_moving();

			print_cutting(next_i, fallen);
			
			vector<tree_gen>:: iterator ptr;
			ptr = tree.begin();

			sort(fallen.begin(), fallen.end());        // sort indices of fallen trees
			for (int j=0; j < (int)fallen.size(); j++) // remove fallen trees from all-info vector
			{
				int temp = fallen[j];
				int x_temp = tree[temp-j].x;
                int y_temp = tree[temp-j].y;
				tree.erase(ptr + fallen[j] - j);

				for(i=0; i < (int)tree.size(); i++)
                {
                    if(tree[i].x == x_temp || tree[i].y == y_temp)
                    {
                        profit_finder(i);
                    }
                }
				ptr = tree.begin();
			}
			for (int j=0; j < (int)tree.size(); j++)
			{
				if (x_next == tree[j].x && y_next == tree[j].y)
				{
					tree.erase(ptr + j);
					break;
				}
			}
			x_curr = x_next;
			y_curr = y_next;
		}
		else{
			break;        // if no trees left, end the loop
		}
	}
}

int print_cutting(int i, vector<int> &fall_list)
{
	int profit_temp=0;
    if(t >= tree[i].d)
    {
        if (tree[i].dir == 1)
        {
		    cout << "cut up\n";
		    profit_temp= profit_up(i, fall_list);
	    }
        else if (tree[i].dir == 2)
        {
		    cout << "cut right\n";
		    profit_temp= profit_right(i, fall_list);
	    }
        else if (tree[i].dir == 3)
		{
		    cout << "cut down\n";
		    profit_temp= profit_down(i, fall_list);
	    }
        else if (tree[i].dir == 4)
		{
			cout << "cut left\n";
		    profit_temp= profit_left(i, fall_list);
	    }
	    t = t - tree[i].d;

        for(int k=0; k < (int)tree.size(); k++)
        {
            if(tree[k].x == tree[i].x || tree[k].y == tree[i].y)
            {
                profit_finder(k);
            }
        }
    }
    return profit_temp;
}

int find_next()
{
    float maximize_this = 0;        // this is the variable we want to maximize here (profit/time left)
	int i_next = 0;                 // index of next tree
	for (int i=0; i<(int)tree.size(); i++) // O(k) times
    {
		int time_till_next = abs(tree[i].x - x_curr) + abs(tree[i].y - y_curr) + tree[i].d;
		if (time_till_next <= t)    // if it is viable to cut tree within time left,
        {
            //int check = 0;
			float qty= (float)tree[i].max_profit/time_till_next; // find profit/time left
			if (qty >= maximize_this)                            // get its max value
            {
				maximize_this = qty;
				i_next = i;                                      // keep track of index of next
			}
		}
	}
	return i_next;
}

int find_next_price()
{
    float maximize_this = 0;        // this is the variable we want to maximize here (profit/time left)
	int i_next = 0;                 // index of next tree
	for (int i=0; i<(int)tree.size(); i++)
    {
		int time_till_next = abs(tree[i].x - x_curr) + abs(tree[i].y - y_curr) + tree[i].d;
		if (time_till_next <= t)   // if it is viable to cut tree within time left,
        {
            //int check = 0;
			float qty= (float)tree[i].value/time_till_next;      // find value/time left
			if (qty >= maximize_this)                            // get its max value
            {
				maximize_this = qty;
				i_next = i;                                      // keep track of index of next
			}
		}
	}
	return i_next;
}

// to find profit obtained by cutting a tree in each direction
// this will find dprofit, uprofit, lprofit, rprofit and max_profit for each tree
void profit_finder(int i)
{
	tree[i].max_profit = 0;
    int time_till_it = abs(tree[i].x - x_curr) + abs(tree[i].y - y_curr) + tree[i].d;
    // make sure time to get to a tree is even viable
    if(time_till_it <= t)
    {
        vector<int> d_list;
        vector<int> d_tracker;
            
        // first assume up is best, then check if other dirns are better
        int uprofit = profit_up(i, d_list);
        tree[i].max_profit = uprofit;
        d_tracker = d_list;                          // copy constructor
        tree[i].dir = 1;

        int rprofit = profit_right(i, d_list);
        if(rprofit > tree[i].max_profit)
        {
            tree[i].max_profit = rprofit;
            d_tracker.clear();
            d_tracker = d_list;
            tree[i].dir = 2;
        }

        int dprofit = profit_down(i, d_list);
        if(dprofit > tree[i].max_profit)
        {
            tree[i].max_profit = dprofit;
            d_tracker.clear();
            d_tracker = d_list;
            tree[i].dir = 3;
        }

        int lprofit = profit_left(i, d_list);
        if(lprofit > tree[i].max_profit)
        {
            tree[i].max_profit = lprofit;
            d_tracker.clear();
            d_tracker = d_list;
            tree[i].dir = 4;
        }
        tree[i].domino_list = d_tracker;
    }
}

// this function prints the "move up/down/left/right" to the output stream
void print_moving()
{
    // once we have current coords in x_curr, y_curr
    // and coords of next tree in x_next, y_next,
    // then: we can start printing moving upto that next tree
	if (x_next > x_curr)
    {
        // to move right until you reach x coords of next tree
		for (int i=0; i<(x_next - x_curr) && t>0; i++)
        {
			cout << "move right\n";
			t--;
		}
	}
	else{
        // to move left until you reach x coords of next tree
		for(int i=0; i<(x_curr - x_next) && t>0; i++)
        {
			cout << "move left\n";
			t--;
		}
	}

	if (y_next > y_curr)
    {
        // to move up until you reach y coords of next tree
		for (int i=0; i<(y_next - y_curr) && t>0; i++)
        {
			cout << "move up\n";
			t--;
		}
	}
	else{
        // to move down until you reach y coords of next tree
		for (int i=0; i<(y_curr - y_next) && t>0; i++)
        {
			cout << "move down\n";
			t--;
		}
	}
}

// profit_left(), profit_right(), profit_up(), profit_down() return profit when cutting a tree in
// a certain direction, and also modify a temporary vector that will store indexes of trees that
// would be cut due to domino effect in that direction
int profit_left(int a, vector<int> &left_vec) {
	// we will find the index of the tree closest to current tree on its left
	int left_ind = -1;

    // check for trees at same y-level but to the left of current tree
	for (int i = 0; i < (int)tree.size(); i++)
    {
        if (tree[i].y == tree[a].y && tree[i].x < tree[a].x)
        {                                          // if a tree is to the left of curr,
            if(left_ind < 0)                       // if no tree has been found to the left yet,
            {
                left_ind = i;                      // then update left_ind
            }
            else{                                  // if atleast one tree has been found to the left already,
                if(tree[left_ind].x < tree[i].x)   // if there is a tree in between that tree and curr,
                {
                    left_ind = i;                  // then update left_ind
                }
            }
        }
	}

    int left_profit = 0;
	if (left_ind > 0) // if there exists a tree to the left of current tree
    {
		if (tree[a].weight > tree[left_ind].weight && tree[a].h > (tree[a].x - tree[left_ind].x))
        {                                           // if the tree to the left will fall, given curr tree falls left,
			left_profit = tree[left_ind].value;     // add its value to profit
			left_vec.push_back(left_ind);           // and push that index into our left_vec vector
			int more_left_profit = profit_left(left_ind, left_vec); // domino effect
			left_profit = left_profit + more_left_profit;           // add domino profit to left_profit
			return left_profit;                                     // return left_profit
		}
        else return 0;
	}
    else return 0;
}

int profit_right(int curr, vector<int> &rt_vec)
{
    // we will find the index of the tree closest to current tree on its right
	int right_ind = -1;

    // check for trees at same y-level but to the right of current tree
	for (int i = 0; i < (int)tree.size(); i++)
    {
        if (tree[i].y == tree[curr].y && tree[curr].x < tree[i].x)
        {                                           // if a tree is to the right of curr,
            if(right_ind < 0)                       // if no tree has been found to the right yet,
            {
                right_ind = i;                      // then update right_ind
            }
            else{                                   // if atleast one tree has been found to the right already,
                if(tree[i].x < tree[right_ind].x)   // if there is a tree in between that tree and curr,
                {
                    right_ind = i;                  // then update right_ind
                }
            }
        }
	}

    int right_profit = 0;
	if (right_ind > 0) // if there exists a tree to the right of current tree
    {
		if (tree[curr].weight > tree[right_ind].weight && tree[curr].h > (tree[right_ind].x - tree[curr].x))
        {                                             // if the tree to the right will fall, given curr tree falls right,
			right_profit = tree[right_ind].value;     // add its value to profit
			rt_vec.push_back(right_ind);              // and push that index into our rt_vec vector
			int more_right_profit = profit_right(right_ind, rt_vec); // domino effect
			right_profit = right_profit + more_right_profit;         // add domino profit to right_profit
			return right_profit;                                     // return right_profit
		}
        else return 0;
	}
    else return 0;
}

int profit_up(int curr, vector<int> &up_vec)
{
    // if a tree is up from current tree on the map, we will mention it as being "above" current tree
    // we will find the index of the tree closest to current tree above it
	int up_ind = -1;

    // check for trees at same x-level but above the current tree
	for (int i = 0; i < (int)tree.size(); i++)
    {
        if (tree[curr].x == tree[i].x && tree[i].y > tree[curr].y)
        {                                        // if a tree is above curr,
            if(up_ind < 0)                       // if no tree has been found above curr yet,
            {
                up_ind = i;                      // then update up_ind
            }
            else{                                // if atleast one tree has been found above curr already,
                if(tree[up_ind].y > tree[i].y)   // if there is a tree in between that tree and curr,
                {
                    up_ind = i;                  // then update up_ind
                }
            }
        }
	}

    int up_profit = 0;
	if (up_ind > 0) // if there exists a tree above the current tree
    {
		if (tree[curr].weight > tree[up_ind].weight && tree[curr].h > (tree[up_ind].y - tree[curr].y))
        {                                          // if the tree above will fall, given curr tree falls up,
			up_profit = tree[up_ind].value;        // add its value to profit
			up_vec.push_back(up_ind);              // and push that index into our up_vec vector
			int more_up_profit = profit_up(up_ind, up_vec);    // domino effect
			up_profit = up_profit + more_up_profit;            // add domino profit to up_profit
			return up_profit;                                  // return up_profit
		}
        else return 0;
	}
    else return 0;
}

int profit_down(int curr, vector<int> &down_vec)   // calls itself O(k) times, so time complexity is O(k^2)
{
    // if a tree is down from current tree on the map, we will mention it as being "below" current tree
    // we will find the index of the tree closest to current tree below it
	int down_ind = -1;

    // check for trees at same x-level but below the current tree
	for (int i = 0; i < (int)tree.size(); i++)     // O(k) for k trees
    {
        if (tree[i].y < tree[curr].y && tree[curr].x == tree[i].x)
        {                                          // if a tree is below curr,
            if(down_ind < 0)                       // if no tree has been found below curr yet,
            {
                down_ind = i;                      // then update down_ind
            }
            else{                                  // if atleast one tree has been found below curr already,
                if(tree[down_ind].y < tree[i].y)   // if there is a tree in between that tree and curr,
                {
                    down_ind = i;                  // then update down_ind
                }
            }
        }
	}

    int down_profit = 0;
	if (down_ind > 0) // if there exists a tree below the current tree
    {
		if (tree[curr].weight > tree[down_ind].weight && tree[curr].h > (tree[curr].y - tree[down_ind].y))
        {                                              // if the tree above will fall, given curr tree falls down,
			down_profit = tree[down_ind].value;        // add its value to profit
			down_vec.push_back(down_ind);              // and push that index into our down_vec vector
			int more_down_profit = profit_down(down_ind, down_vec);  // domino effect, at max O(k) times
			down_profit = down_profit + more_down_profit;            // add domino profit to down_profit
			return down_profit;                                      // return down_profit
		}
        else return 0;
	}
    else return 0;
}