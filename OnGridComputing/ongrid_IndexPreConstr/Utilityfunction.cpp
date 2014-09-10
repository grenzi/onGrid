
//Reuse vertex structure as follows
//struct Vertex {
//	bool visited;
//	bool fat;	// fat node
//	int topo_id;	// topological order
//	int path_id;	// path id
//	int dfs_order; 				---> The decentralizing vertex number
//	int pre_order; 				---> The Incoming vertex number, one of the neighbor	
//	int post_order; 			---> The outgoing vertex number, one of the neighbor
//	int first_visit; // for test
//};


#include "Utilityfunction.h"



void BFSLabel(Graph& g, int v, int length_limit, labelEstPara& label_est, vector<int>& l_in_labelsize, vector<int>& l_out_labelsize, vector<int>& l_in_out_same_labelsize, vector<int>& l_in_out_different_labelsize)
{
	//center point go to other points
	set<int> visited;
	set<int> involved;
	queue<vertex_inqueue> q;
	vertex_inqueue vinq;
	vertex_inqueue uinq;
	vertex_inqueue winq;
	Label label; 
	vinq.num=v;
	vinq.level=0;
	q.push(vinq);
	visited.insert(v);
	while(!q.empty())
	{
		uinq=q.front();
		q.pop();
		
		//if(uinq.level!=0)
		//{
		//	/*label.dest=v;
		//	label.dist=uinq.level;
		//	label.hop=uinq.hop;
		//	l_in[uinq.num].push_back(label);*/
		//	label_size++;
		////	cout<<l_in_labelsize[uinq.num]<<endl;
		//}
		
		
		if(uinq.level<=(length_limit-1))
		{
			vector<int>::iterator iter;
			for(iter=g.graph[uinq.num].outList.begin(); iter!=	g.graph[uinq.num].outList.end(); iter++)
			{
				if(!(visited.find(*iter)!=visited.end()))
				{
					visited.insert(*iter);
					involved.insert(*iter);
					g[*iter].dfs_order=v;
					g[*iter].pre_order=uinq.num;
					winq.num=*iter;
					winq.level=uinq.level+1;
					winq.hop=uinq.num;
					q.push(winq);
				}
			}
		}
	}
	
	//all other points go to center
	visited.clear();
	q.push(vinq);
	visited.insert(v);
	while(!q.empty())
	{
		uinq=q.front();
		q.pop();
		//if(uinq.level!=0)
		//{
		//	//l_out[uinq.num].insert(pair<int, int>(v, uinq.level) );
		//	/*label.dest=v;
		//	label.dist=uinq.level;
		//	label.hop=uinq.hop;
		//	l_out[uinq.num].push_back(label);*/	
		////	cout<<l_out_labelsize[uinq.num]<<endl;
		//	label_size++;
		//}		
		if(uinq.level<=(length_limit-1))
		{
			vector<int>::iterator iter;
			for(iter=g.graph[uinq.num].inList.begin(); iter!=g.graph[uinq.num].inList.end(); iter++)
			{
				if(!(visited.find(*iter)!=visited.end()))
				{
					visited.insert(*iter);
					involved.insert(*iter);
					g[*iter].dfs_order=v;
					g[*iter].post_order=uinq.num;					
					winq.num=*iter;
					winq.level=uinq.level+1;
					winq.hop=uinq.num;
					q.push(winq);
				}
			}
		}
	}
	
	for(set<int>::iterator siter=involved.begin(); siter!=involved.end(); siter++)
	{
		if (g[*siter].pre_order==-1 && g[*siter].post_order==-1)
			cout<<"Internal logic error at BFSLabel";
		else if (g[*siter].post_order==-1)
		{
			label_est.only_in++;
			l_in_labelsize[*siter]++;
		}
		else if (g[*siter].pre_order==-1)
		{
			label_est.only_out++;
			l_out_labelsize[*siter]++;
		}
		else if (g[*siter].pre_order==g[*siter].post_order)
		{
			label_est.inout_same++;
			l_in_out_same_labelsize[*siter]++;
		}
		else
		{
			label_est.inout_different++;
			l_in_out_different_labelsize[*siter]++;
		}
		
		g[*siter].dfs_order=-1;
		g[*siter].pre_order=-1;
		g[*siter].post_order=-1;
		
	}
	
}

