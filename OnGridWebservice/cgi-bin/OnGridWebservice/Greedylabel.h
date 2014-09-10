#ifndef _GREEDYLABEL_H
#define _GREEDYLABEL_H

#include "Graph.h"
#include "Utilityfunction.h"
#include <cstring>
#include <hash_map.h>

//#define DEBUG
#define MAXVID 1000000000//Assume int has 4 bypes, and the number of CUI terms is no more than 2^24=268,435,456 .
#define UPPER 0
#define LOWER 1
#define BOTH  2


typedef pair<int,int> intpair;

struct intpaircomp{
	bool operator() (const intpair& p1, const intpair&p2) const{
		if(p1.second>p2.second)
			return true;
		else
		{
			if(p1.second<p2.second)
				return false;
			else
			{
				if(p1.first<p2.first)
					return true;
				else 
					return false;
			}
		}
	}
};




class Greedylabel {
	public:
		Graph& g;
		vector<vector<Label> > l_in, l_out, l_in_out_same;
		vector<vector<LabelDiff> > l_in_out_diff;
		multimap<int, LabelSpecial> l_special;
		
		Greedylabel(Graph& graph);
		~Greedylabel();
		void init(char* in_label, char* out_label, char* in_out_same_label, char* in_out_diff_label, char* special_label);
		Match FindShortestDist(int src, int trg);
		void find_oneshortestpath(int src,int trg, vector<int>& path);
		void find_allpath(int src, int trg, vector<vector<int> >& pathcollection);
		int find_allpathnum(int src, int trg);
		int TotalTransitivePathNum_Contribution(const int& src, const int& trg, double& pContribution, const double& EXPBASE, int& score_in) const; //EXPBASE:: The larger the less contribution a long path will make. 
		void find_allshortestpath(int src, int trg, vector<vector<int> >& pathcollection);
		//void displaylabels();*/

	private:
		void loadLabel(vector<vector<Label> >& VertexLabelList, char* FileName);
		void loadLabel(vector<vector<LabelDiff> >& VertexLabelList, char* FileName);
		void loadLabel(multimap<int, LabelSpecial>& VertexLabelList, char* FileName);
		
		inline void find_onepath(int src, int trg, Match& _match, vector<int>& path, bool require_simplepath)
		{
			
			if(_match.distance>=MAX_VAL ||_match.distance==0)
				return;
			else if(_match.distance==1)
			{
				path.push_back(src);
				path.push_back(trg);
				return;
			}

			//Build Path from src to _match.common_dest
			
			int current_node=src;
			
			if(current_node!=_match.common_dest){//save the calculation of the first jump
				path.push_back(current_node);
				current_node=g.graph[src].portList[_match.out_offset];//The retrieved node must also be in outlist
			}
			
			while(current_node!=_match.common_dest)
			{
				path.push_back(current_node);
				
				int offset_out=BinarySearchOffset(l_out[current_node],_match.common_dest);
				if(offset_out!=-1){
					if(offset_out==65535)
						offset_out=offset_decoder(current_node, _match.common_dest, false);				
					current_node=g.graph[current_node].portList[offset_out];
					continue;
				}
				
				int offset_same=BinarySearchOffset(l_in_out_same[current_node],_match.common_dest);
				if(offset_same!=-1){
					if(offset_same==65535)
						offset_same=offset_decoder(current_node, _match.common_dest, false);					
					current_node=g.graph[current_node].portList[offset_same];
					continue;
				}
				
				int offset_diff=BinarySearchOffset(l_in_out_diff[current_node],_match.common_dest,false);
				if(offset_diff!=-1){
					if(offset_diff==65535)
						offset_diff=offset_decoder(current_node, _match.common_dest, false);				
					current_node=g.graph[current_node].portList[offset_diff];
					continue;
				}					
				
				cout<<"Error: Did not find the dest in the out-related list";
				exit(-1);
			}
			
			if(current_node==_match.common_dest)
				path.push_back(current_node);//Add the common dest here. 
			else{
				cout<<"Error in while loop of find_onepath"<<endl;
				exit(-1);
			}
				

			//Build Path from _match.common_dest to trg
			vector<int> temp_path;
			current_node=trg;

			if(current_node!=_match.common_dest)//save the calculation of the first reverse jump
			{
				temp_path.push_back(current_node);
				current_node=g.graph[trg].portList[_match.in_offset];//The retrieved node must also be in inlist
			}
			
			while(current_node!=_match.common_dest)
			{
				temp_path.push_back(current_node);//make sure not to add the common_dest again.

				int offset_in=BinarySearchOffset(l_in[current_node],_match.common_dest);
				if(offset_in!=-1){
					if(offset_in==65535)
						offset_in=offset_decoder(current_node, _match.common_dest, true);
					current_node=g.graph[current_node].portList[offset_in];
					continue;
				}				

				int offset_same=BinarySearchOffset(l_in_out_same[current_node],_match.common_dest);
				if(offset_same!=-1){
					if(offset_same==65535)
						offset_same=offset_decoder(current_node, _match.common_dest, true);				
					current_node=g.graph[current_node].portList[offset_same];
					continue;
				}

				int offset_diff=BinarySearchOffset(l_in_out_diff[current_node],_match.common_dest, true);
				if(offset_diff!=-1){
					if(offset_diff==65535)
						offset_diff=offset_decoder(current_node, _match.common_dest, true);					
					current_node=g.graph[current_node].portList[offset_diff];
					continue;
				}
				
				cout<<"Error: Did not find the dest in the in-related list";
				exit(-1);				
			}
			
			if(require_simplepath)
			{//simplepath detection. path or temp_path itself will never contain repeated vertices according to kDLS
			 //But they may contain common vertices that make path+temp_path not a simple path.
				for(vector<int>::iterator iter_p=path.begin(); iter_p!=path.end(); iter_p++)
					for (vector<int>::iterator iter_tp=temp_path.begin(); iter_tp!=temp_path.end(); iter_tp++)
					{
						if (*iter_p==*iter_tp)//This indicates path+temp_path is not a simple path.
						{
							path.clear();
							return;
						}
					}
			}
			
			
			for(vector<int>::reverse_iterator rit=temp_path.rbegin();rit<temp_path.rend();++rit)
			{
				path.push_back(*rit);
			}

		};			
		
		inline int offset_decoder(int vertexID, int destination, bool incoming)
		{
			pair<multimap<int, LabelSpecial>::iterator,multimap<int, LabelSpecial>::iterator> ret=l_special.equal_range(vertexID);
			for(multimap<int, LabelSpecial>::iterator it=ret.first; it!=ret.second; it++){
				if(incoming){
					if ((it->second.direction==1 || it->second.direction==3) && (it->second.dest==destination))
						return it->second.port;
				}
				else{
					if ((it->second.direction==2 || it->second.direction==3) && (it->second.dest==destination))
						return it->second.port;
				}
			}
				cout<<"Error: offset_decoder fails";
				exit(-1);			
		};
		
		//Important Assumption 
		//(1) labelVector's members are in ascending order of dest, 
		//(2) no two members have the same dest
		//(3) no member's dest is equal to vertexID
		inline void MergeLabelVectorAscend(const int& vertexID, bool incoming, const vector<Label>& labelVector, const vector<Label>& labelSameVector, const vector<LabelDiff>& labelDiffVector, vector<Label>& VertexLabelVector) const
		{
			int pow16=int(pow(2.0,16.0));
			int pow8=int(pow(2.0,8.0));
			int pow4=int(pow(2.0,4.0));

			Label labelvid;
			labelvid.dest1=vertexID/pow16;
			labelvid.dest2=(vertexID-int(labelvid.dest1)*pow16)/pow8;
			labelvid.dest3=vertexID-int(labelvid.dest1)*pow16-int(labelvid.dest2)*pow8;
			labelvid.port1=255;//note the 255 special port also used here
			labelvid.port2=255;
			labelvid.distance=0;
			labelvid.score_in = g[vertexID].rank;
			labelvid.score_out = g[vertexID].rank;
			
			VertexLabelVector.resize(labelVector.size()+labelSameVector.size()+labelDiffVector.size()+1);
			
			vector<Label>::const_iterator iter_lV=labelVector.begin();
			vector<Label>::const_iterator iter_lSV=labelSameVector.begin();
			vector<LabelDiff>::const_iterator iter_lDV=labelDiffVector.begin();
			vector<Label>::iterator iter_VLV=VertexLabelVector.begin();
			
			int dest_lV=MAXVID;
			int dest_lSV=MAXVID;
			int dest_lDV=MAXVID;
			
			while (!(iter_lV==labelVector.end() && iter_lSV==labelSameVector.end() && iter_lDV==labelDiffVector.end()))
			{
				if (iter_lV!=labelVector.end())
					dest_lV=int(iter_lV->dest1)*pow16+int(iter_lV->dest2)*pow8+int(iter_lV->dest3);
				else
					dest_lV=MAXVID;
				
				if (iter_lSV!=labelSameVector.end())
					dest_lSV=int(iter_lSV->dest1)*pow16+int(iter_lSV->dest2)*pow8+int(iter_lSV->dest3);
				else
					dest_lSV=MAXVID;
				
				if (iter_lDV!=labelDiffVector.end())
					dest_lDV=int(iter_lDV->dest1)*pow16+int(iter_lDV->dest2)*pow8+int(iter_lDV->dest3);
				else
					dest_lDV=MAXVID;

				if (dest_lDV<dest_lV && dest_lDV<dest_lSV)//dest_lDV must be the smallest
				{
					if(vertexID<dest_lDV)
					{
						*iter_VLV=labelvid;
						iter_VLV++;
						break;
					}
					else
					{
						Label labeltransit;
						labeltransit.dest1=iter_lDV->dest1;
						labeltransit.dest2=iter_lDV->dest2;
						labeltransit.dest3=iter_lDV->dest3;
						if (incoming)
						{
							labeltransit.port1=iter_lDV->port1_in;
							labeltransit.port2=iter_lDV->port2_in;
							labeltransit.distance=int(iter_lDV->distance)/pow4;
						}else
						{
							labeltransit.port1=iter_lDV->port1_out;
							labeltransit.port2=iter_lDV->port2_out;	
							labeltransit.distance=int(iter_lDV->distance)-(int(iter_lDV->distance)/pow4)*pow4;							
						}
						labeltransit.score_in = iter_lDV->score_in;
						labeltransit.score_out = iter_lDV->score_out;
						*iter_VLV=labeltransit;
						iter_VLV++;
						iter_lDV++;						
					}
					
				}
				else if (dest_lV<dest_lSV)//dest_lV must be the smallest
				{
					if(vertexID<dest_lV)
					{
						*iter_VLV=labelvid;
						iter_VLV++;
						break;
					}
					else
					{
						*iter_VLV=*iter_lV;
						iter_VLV++;
						iter_lV++;
					}				
				}
				else if (dest_lSV<dest_lV)//dest+lSV must be the smallest
				{
					if(vertexID<dest_lSV)
					{
						*iter_VLV=labelvid;
						iter_VLV++;
						break;
					}
					else
					{
						*iter_VLV=*iter_lSV;
						if(incoming)
							iter_VLV->distance=int(iter_VLV->distance)/pow4;
						else
							iter_VLV->distance=int(iter_VLV->distance)-(int(iter_VLV->distance)/pow4)*pow4;
						iter_VLV++;
						iter_lSV++;
					}					
				}
				else if (!(dest_lV==MAXVID && dest_lSV==MAXVID && dest_lDV==MAXVID))
					cout<<"Error: Assumption for MergeLabelVectorAscend is invalid."<<endl;
			}

			if (iter_lV==labelVector.end() && iter_lSV==labelSameVector.end() && iter_lDV==labelDiffVector.end())
				*iter_VLV=labelvid;
			//else implies !(iter_lV==labelVector.end() && iter_lSV==labelSameVector.end() && iter_lDV==labelDiffVector.end()) and the following will not be run
			//vertexID has been inserted.
			
			
			while (!(iter_lV==labelVector.end() && iter_lSV==labelSameVector.end() && iter_lDV==labelDiffVector.end()))
			{
				if (iter_lV!=labelVector.end())
					dest_lV=int(iter_lV->dest1)*pow16+int(iter_lV->dest2)*pow8+int(iter_lV->dest3);
				else
					dest_lV=MAXVID;
				
				if (iter_lSV!=labelSameVector.end())
					dest_lSV=int(iter_lSV->dest1)*pow16+int(iter_lSV->dest2)*pow8+int(iter_lSV->dest3);
				else
					dest_lSV=MAXVID;
				
				if (iter_lDV!=labelDiffVector.end())
					dest_lDV=int(iter_lDV->dest1)*pow16+int(iter_lDV->dest2)*pow8+int(iter_lDV->dest3);
				else
					dest_lDV=MAXVID;

				if (dest_lDV<dest_lV && dest_lDV<dest_lSV)//dest_lDV must be the smallest
				{
					Label labeltransit;
					labeltransit.dest1=iter_lDV->dest1;
					labeltransit.dest2=iter_lDV->dest2;
					labeltransit.dest3=iter_lDV->dest3;
					if (incoming)
					{
						labeltransit.port1=iter_lDV->port1_in;
						labeltransit.port2=iter_lDV->port2_in;
						labeltransit.distance=int(iter_lDV->distance)/pow4;
					}else
					{
						labeltransit.port1=iter_lDV->port1_out;
						labeltransit.port2=iter_lDV->port2_out;
						labeltransit.distance=int(iter_lDV->distance)-(int(iter_lDV->distance)/pow4)*pow4;							
					}
					labeltransit.score_in = iter_lDV->score_in;
					labeltransit.score_out = iter_lDV->score_out;
					*iter_VLV=labeltransit;
					iter_VLV++;
					iter_lDV++;						
				}else if (dest_lV<dest_lSV)//dest_lV must be the smallest
				{
					*iter_VLV=*iter_lV;
					iter_VLV++;
					iter_lV++;
				}
				else if (dest_lSV<dest_lV)//dest+lSV must be the smallest
				{
					*iter_VLV=*iter_lSV;
					if(incoming)
						iter_VLV->distance=int(iter_VLV->distance)/pow4;
					else
						iter_VLV->distance=int(iter_VLV->distance)-(int(iter_VLV->distance)/pow4)*pow4;
					iter_VLV++;
					iter_lSV++;
				}
				else if (!(dest_lV==MAXVID && dest_lSV==MAXVID && dest_lDV==MAXVID))
					cout<<"Error: Assumption for MergeLabelVectorAscend is invalid."<<endl;
			}			
		};
		
};
#endif

