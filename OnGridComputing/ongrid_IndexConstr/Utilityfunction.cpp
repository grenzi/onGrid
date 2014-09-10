
//Reuse vertex structure as follows
//struct Vertex {
//	bool visited;
//	bool fat;	// fat node
//	int topo_id;				---> The incoming distance
//	int path_id;				---> The outgoing distance
//	int dfs_order; 				---> The decentralizing vertex number
//	int pre_order; 				---> The Incoming vertex number, one of the neighbor	
//	int post_order; 			---> The outgoing vertex number, one of the neighbor
//	int first_visit; // for test
//};

#include "Utilityfunction.h"

void BFSLabel(Graph& g, int v, int length_limit,  labelEstPara& label_est, vector<int>& l_in_labelsize, vector<int>& l_out_labelsize, vector<int>& l_in_out_same_labelsize, vector<int>& l_in_out_diff_labelsize, vector<vector<Label> >& l_in, vector<vector<Label> >& l_out, vector<vector<Label> >& l_in_out_same, vector<vector<LabelDiff> >& l_in_out_diff, multimap<int, LabelSpecial>& l_special)
{
	//center point go to other points
	set<int> visited;
	set<int> involved;
	queue<vertex_inqueue> q;
	vertex_inqueue vinq;
	vertex_inqueue uinq;
	vertex_inqueue winq;
	vinq.num=v;
	vinq.level=0;
	vinq.score_in = g[v].rank;
	vinq.score_out = g[v].rank;
	q.push(vinq);
	visited.insert(v);
	int pow16=int(pow(2.0,16.0));
	int pow8=int(pow(2.0,8.0));	
	int pow4=int(pow(2.0,4.0));
	
	while(!q.empty())
	{
		uinq=q.front();
		q.pop();
		
		if(uinq.level<=(length_limit-1))
		{
			vector<int>::iterator iter;
			for(iter=g.graph[uinq.num].outList.begin(); iter!=	g.graph[uinq.num].outList.end(); iter++)
			{
				if(visited.find(*iter)==visited.end())
				{
					visited.insert(*iter);
					involved.insert(*iter);
					g[*iter].dfs_order=v;

					vector<int>::iterator iter_in=find(g.graph[*iter].portList.begin(), g.graph[*iter].portList.end(), uinq.num);
					if(iter_in==g.graph[*iter].portList.end())
						cout<<"Error in offset locating"<<endl;
					g[*iter].pre_order=distance(g.graph[*iter].portList.begin(),iter_in);
					if (g[*iter].pre_order<0 || g[*iter].pre_order>=65535)
						cout<<"Extreme offset detected: "<<g[*iter].pre_order<<endl;

					g[*iter].topo_id=uinq.level+1;
					g[*iter].score_in = uinq.score_in * g[*iter].rank;
					winq.num=*iter;
					winq.level=uinq.level+1;
					winq.score_in = uinq.score_in * g[*iter].rank;
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
	
		if(uinq.level<=(length_limit-1))
		{
			vector<int>::iterator iter;
			for(iter=g.graph[uinq.num].inList.begin(); iter!=g.graph[uinq.num].inList.end(); iter++)
			{
				if(visited.find(*iter)==visited.end())
				{
					visited.insert(*iter);
					involved.insert(*iter);
					g[*iter].dfs_order=v;

					vector<int>::iterator iter_out=find(g.graph[*iter].portList.begin(), g.graph[*iter].portList.end(), uinq.num);
					if(iter_out==g.graph[*iter].portList.end())
						cout<<"Error in offset locating"<<endl;
					g[*iter].post_order=distance(g.graph[*iter].portList.begin(),iter_out);					
					if (g[*iter].post_order<0 || g[*iter].post_order>=65535)
						cout<<"Extreme offset detected: "<<g[*iter].post_order<<endl;
					
					g[*iter].path_id=uinq.level+1;		
					g[*iter].score_out = uinq.score_out * g[*iter].rank;			
					winq.num=*iter;
					winq.level=uinq.level+1;
					winq.score_out = uinq.score_out * g[*iter].rank;
					q.push(winq);
				}
			}
		}
	}
	
	for(set<int>::iterator siter=involved.begin(); siter!=involved.end(); siter++)
	{
		if (g[*siter].pre_order==-1 && g[*siter].post_order==-1)
			cout<<"Internal logic error at BFSLabel"<<endl;
		else if (g[*siter].post_order==-1)
		{
			Label label;
			label_est.only_in++;
			
			label.dest1=g[*siter].dfs_order/pow16;
			label.dest2=(g[*siter].dfs_order-label.dest1*pow16)/pow8;
			label.dest3=g[*siter].dfs_order-int(label.dest1)*pow16-int(label.dest2)*pow8;

			if (g[*siter].pre_order<65535)
			{
				label.port1=g[*siter].pre_order/pow8;
				label.port2=g[*siter].pre_order-int(label.port1)*pow8;
			}else{
				label.port1=255;
				label.port2=255;
				LabelSpecial label_special;
				label_special.direction=1;
				label_special.dest=g[*siter].dfs_order;
				label_special.port=g[*siter].pre_order;
				label_special.distance=g[*siter].topo_id;
				//set score
				label_special.score_in = g[*siter].score_in;
				label_special.score_out = g[*siter].score_out;	
				l_special.insert(pair<int, LabelSpecial>(*siter,label_special));
			}
			
			label.distance=g[*siter].topo_id;

			//set score
			label.score_in = g[*siter].score_in;
			label.score_out = g[*siter].score_out;

			l_in[*siter][l_in_labelsize[*siter]]=label;
			l_in_labelsize[*siter]++;
		}
		else if (g[*siter].pre_order==-1)
		{
			Label label;
			
			label_est.only_out++;

			label.dest1=g[*siter].dfs_order/pow16;
			label.dest2=(g[*siter].dfs_order-label.dest1*pow16)/pow8;
			label.dest3=g[*siter].dfs_order-int(label.dest1)*pow16-int(label.dest2)*pow8;

			if (g[*siter].post_order<65535)
			{
				label.port1=g[*siter].post_order/pow8;
				label.port2=g[*siter].post_order-int(label.port1)*pow8;
			}else{				
				label.port1=255;
				label.port2=255;
				LabelSpecial label_special;
				label_special.direction=2;
				label_special.dest=g[*siter].dfs_order;
				label_special.port=g[*siter].post_order;
				label_special.distance=g[*siter].path_id;
				//set score
				label_special.score_in = g[*siter].score_in;
				label_special.score_out = g[*siter].score_out;
				l_special.insert(pair<int, LabelSpecial>(*siter,label_special));				
			}
			
			label.distance=g[*siter].path_id;

			//set score
			label.score_in = g[*siter].score_in;
			label.score_out = g[*siter].score_out;			
			
			l_out[*siter][l_out_labelsize[*siter]]=label;
			l_out_labelsize[*siter]++;
		}
		else if (g[*siter].pre_order==g[*siter].post_order)
		{
			Label label;
			
			label_est.inout_same++;

			label.dest1=g[*siter].dfs_order/pow16;
			label.dest2=(g[*siter].dfs_order-label.dest1*pow16)/pow8;
			label.dest3=g[*siter].dfs_order-int(label.dest1)*pow16-int(label.dest2)*pow8;	

			if (g[*siter].pre_order<65535)
			{
				label.port1=g[*siter].pre_order/pow8;
				label.port2=g[*siter].pre_order-int(label.port1)*pow8;
			}else{
				label.port1=255;
				label.port1=255;
				LabelSpecial label_special;
				label_special.direction=3;
				label_special.dest=g[*siter].dfs_order;
				label_special.port=g[*siter].pre_order;
				label_special.distance=g[*siter].topo_id;
				label_special.ex_distance=g[*siter].path_id;
				//set score
				label_special.score_in = g[*siter].score_in;
				label_special.score_out = g[*siter].score_out;
				l_special.insert(pair<int, LabelSpecial>(*siter,label_special));					
			}			

			if (g[*siter].topo_id>15 || g[*siter].path_id>15)
				cout<<"Distance overflow at BFSLabel"<<endl;
			else
				label.distance=g[*siter].topo_id*pow4+g[*siter].path_id;

			//set score
			label.score_in = g[*siter].score_in;
			label.score_out = g[*siter].score_out;
			
			l_in_out_same[*siter][l_in_out_same_labelsize[*siter]]=label;
			l_in_out_same_labelsize[*siter]++;
		}
		else//means in and out through different ports.
		{
			LabelDiff labeldiff;	
			label_est.inout_different++;
			
			labeldiff.dest1=g[*siter].dfs_order/pow16;
			labeldiff.dest2=(g[*siter].dfs_order-labeldiff.dest1*pow16)/pow8;
			labeldiff.dest3=g[*siter].dfs_order-int(labeldiff.dest1)*pow16-int(labeldiff.dest2)*pow8;

			if (g[*siter].pre_order<65535)
			{
				labeldiff.port1_in=g[*siter].pre_order/pow8;
				labeldiff.port2_in=g[*siter].pre_order-int(labeldiff.port1_in)*pow8;
			}else{
				labeldiff.port1_in=255;
				labeldiff.port2_in=255;
				LabelSpecial label_special;
				label_special.direction=1;
				label_special.dest=g[*siter].dfs_order;
				label_special.port=g[*siter].pre_order;
				label_special.distance=g[*siter].topo_id;
				//set score
				label_special.score_in = g[*siter].score_in;
				label_special.score_out = g[*siter].score_out;
				l_special.insert(pair<int, LabelSpecial>(*siter,label_special));			
			}			
			
			if (g[*siter].post_order<65535)
			{
				labeldiff.port1_out=g[*siter].post_order/pow8;
				labeldiff.port2_out=g[*siter].post_order-int(labeldiff.port1_out)*pow8;
			}else{
				labeldiff.port1_out=255;
				labeldiff.port2_out=255;
				LabelSpecial label_special;
				label_special.direction=2;
				label_special.dest=g[*siter].dfs_order;
				label_special.port=g[*siter].post_order;
				label_special.distance=g[*siter].path_id;
				//set score
				label_special.score_in = g[*siter].score_in;
				label_special.score_out = g[*siter].score_out;
				l_special.insert(pair<int, LabelSpecial>(*siter,label_special));					
			}

			if (g[*siter].topo_id>15 || g[*siter].path_id>15)
				cout<<"Distance overflow at BFSLabel"<<endl;
			else
				labeldiff.distance=g[*siter].topo_id*pow4+g[*siter].path_id;	

			//set score
			labeldiff.score_in = g[*siter].score_in;
			labeldiff.score_out = g[*siter].score_out;	
			
			l_in_out_diff[*siter][l_in_out_diff_labelsize[*siter]]=labeldiff;
			l_in_out_diff_labelsize[*siter]++;
		}
		
		g[*siter].topo_id=-1;
		g[*siter].path_id=-1;		
		g[*siter].dfs_order=-1;
		g[*siter].pre_order=-1;
		g[*siter].post_order=-1;
		g[*siter].score_in = -1;
		g[*siter].score_out = -1;
		
	}
	
}

