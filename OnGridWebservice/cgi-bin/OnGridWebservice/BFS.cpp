#include "BFS.h"

void BFSsearch::FindShortestPath(int begin, int end, int maxDepth, Graph& g, vector<int>& path)
{
	if(begin==end)
		return;
		
	queue<int> q;
	q.push(begin);

	vector<node_info> nodes=vector<node_info>(g.num_vertices());		
	nodes[begin].visited=true;
	nodes[begin].level=0;

	while(!q.empty())
	{
		int now=q.front();
		q.pop();
		
		if(nodes[now].level<maxDepth)
		{
			for(vector<int>::iterator iter=g.graph[now].outList.begin(); iter<g.graph[now].outList.end();iter++)
			{
				if(!nodes[*iter].visited)
				{
					nodes[*iter].level=nodes[now].level+1;
					nodes[*iter].parent=now;
					if(*iter==end)
					{
						vector<int> inv_p;
						int current=*iter;
						while(true)
						{
							inv_p.push_back(current);
							if(current!=begin)
								current=nodes[current].parent;
							else
								break;
						}
						vector<int>::reverse_iterator rit;
						for(rit=inv_p.rbegin(); rit<inv_p.rend();++rit)
						{
							path.push_back(*rit);
						}
						
						return;
						
					}
					q.push(*iter);
					nodes[*iter].visited=true;
				}
			}
		}
	}
						
}

int BFSsearch::FindShortestDist(const int& begin, const int& end, const int& maxDepth, const Graph& g, int& score_in)
{
	if(begin==end)
		return 0;
	
	queue<int> q;
	q.push(begin);


	vector<node_info> nodes=vector<node_info>(g.num_vertices());
	nodes[begin].visited=true;
	nodes[begin].level=0;
	nodes[begin].score_in = g.get_rank(begin);

	while(!q.empty())
	{
		int now=q.front();
		q.pop();
		
		if(nodes[now].level<maxDepth)
		{
			for(vector<int>::const_iterator iter=g.graph[now].outList.begin(); iter<g.graph[now].outList.end();iter++)
			{
				if(!nodes[*iter].visited)
				{
					nodes[*iter].level=nodes[now].level+1;
					nodes[*iter].score_in = nodes[now].score_in * g.get_rank(*iter);
					
					if(*iter==end) {
						score_in = nodes[*iter].score_in;
						return nodes[*iter].level;
					}

					q.push(*iter);
					nodes[*iter].visited=true;
				}
			}
		}
	}
	
	return MAX_VAL;
}


int BFSsearch::FindShortestDist(int begin, int end, int maxDepth, Graph& g)
{
	if(begin==end)
		return 0;
	
	queue<int> q;
	q.push(begin);


	vector<node_info> nodes=vector<node_info>(g.num_vertices());		
	nodes[begin].visited=true;
	nodes[begin].level=0;
	while(!q.empty())
	{
		int now=q.front();
		q.pop();
		
		if(nodes[now].level<maxDepth)
		{
			for(vector<int>::iterator iter=g.graph[now].outList.begin(); iter<g.graph[now].outList.end();iter++)
			{
				if(!nodes[*iter].visited)
				{
					nodes[*iter].level=nodes[now].level+1;
					if(*iter==end)
						return nodes[*iter].level;
					q.push(*iter);
					nodes[*iter].visited=true;
				}
			}
		}
	}
	
	return MAX_VAL;
}


void BFSsearch::FindMultiplePath(int begin, int end, int maxDepth, Graph& g, vector<vector<int> >& pathcollection)
{
	if (begin==end)
		return;
		
	queue<int> q;
	q.push(begin);

	vector<node_info> nodes=vector<node_info>(g.num_vertices());			
	nodes[begin].visited=true;
	nodes[begin].level=0;

	while(!q.empty())
	{
		int now=q.front();
		q.pop();
		
		if(nodes[now].level<maxDepth)
		{
			for(vector<int>::iterator iter=g.graph[now].outList.begin(); iter<g.graph[now].outList.end();iter++)
			{
				if(!nodes[*iter].visited)
				{
					nodes[*iter].level=nodes[now].level+1;
					nodes[*iter].parent=now;
					if(*iter==end){//target node should not be marked as visited.
						vector<int> p;
						vector<int> inv_p;
						int current=*iter;
						while(true){
							inv_p.push_back(current);
							if(current!=begin)
								current=nodes[current].parent;
							else
								break;
						}
						vector<int>::reverse_iterator rit;
						for(rit=inv_p.rbegin(); rit<inv_p.rend();++rit){
							p.push_back(*rit);
						}
						pathcollection.push_back(p);
					}else{
						q.push(*iter);
						nodes[*iter].visited=true;//target node should not be marked as visited.
					}
				}
			}
		}
	}
}


int BFSsearch::FindMultiplePathNum(int begin, int end, int maxDepth, Graph& g)
{
	if (begin==end)
		return 0;
		
	int pathnum=0;
	queue<int> q;
	q.push(begin);

	vector<node_info> nodes=vector<node_info>(g.num_vertices());		
	nodes[begin].visited=true;
	nodes[begin].level=0;

	while(!q.empty())
	{
		int now=q.front();
		q.pop();
		if(nodes[now].level<maxDepth)
		{
			for(vector<int>::iterator iter=g.graph[now].outList.begin(); iter<g.graph[now].outList.end();iter++)
			{
				if(!nodes[*iter].visited)
				{
					nodes[*iter].level=nodes[now].level+1;
					nodes[*iter].parent=now;
					if(*iter==end){//target node should not be marked as visited.
						pathnum++;
					}else{
						q.push(*iter);
						nodes[*iter].visited=true;//target node should not be marked as visited.
					}
				}
			}
		}
	}
	
	return pathnum;
}
