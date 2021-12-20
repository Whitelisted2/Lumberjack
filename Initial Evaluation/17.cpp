#include<iostream>
#include<vector>
#include<cstdlib>
using namespace std;

// THIS PROGRAM JUST CUTS THE TREE NEAREST TO ORIGIN (IF WITHIN t)
// we will take advantage of domino effect in later versions of this project code

struct tree_gen
{
	int x, y, h, d, c, p; // given parameters of a given tree
	int value, weight;    // derived parameters using h, d, c, p
	int profit;           // other variables w.r.t. a tree, dir stores values of best direction for max profit             
};
vector <tree_gen> tree;
int t, n, k;
int TOTAL_profit = 0;         // current goal: >0

int main()
{
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

        tree[i].profit = tree[i].value; // initialize a tree's profit to be its value
    }
    
    // we want to start with tree closest to origin (enough for just profit>0 case)
    int min_time;        // time to get to the tree having index 0, from origin
    min_time = abs(tree[0].x - 0) + abs(tree[0].y - 0) + tree[0].d;
    int start_i = 0;
    for(i=1; i<k; i++)   // traverse through all trees, check if any other tree is closer to origin
    {
        if(abs(tree[i].x - 0) + abs(tree[i].y - 0) + tree[i].d < min_time)
        {
            start_i = i; // start_i will come in use in later versions
        }
    }

    // to get there, move right by x and move up by y
    int j;
    for(j=0; j<tree[start_i].x && t>0 ; j++)
    {
        cout << "move right\n";
        t--;
    }

    for(j=0; j<tree[start_i].y && t>0 ; j++)
    {
        cout << "move up\n";
        t--;
    }

    if(t>=tree[start_i].d){
        cout<<"cut up\n";
        // there should be enough time to cut the tree, which there will, since we checked earlier!
        t = t - tree[start_i].d;
    }

    // we just wanted TOTAL_profit>0
    // if starter tree is close enough to origin, add its value to total immediately.
    if(tree[start_i].x + tree[start_i].y + tree[start_i].d <= t)
    {
        TOTAL_profit += tree[start_i].value;
    }

    return 0;
}
