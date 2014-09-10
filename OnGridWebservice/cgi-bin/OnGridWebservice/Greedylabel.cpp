
#include "Greedylabel.h"
//#include <sys/time.h>

Greedylabel::Greedylabel(Graph& graph) : g(graph) {
}

Greedylabel::~Greedylabel() {
}

void Greedylabel::init(char* in_label, char* out_label, char* in_out_same_label, char* in_out_diff_label, char* special_label)
{
	l_in = vector<vector<Label> >(g.num_vertices());
	l_out = vector<vector<Label> >(g.num_vertices());
	l_in_out_same = vector<vector<Label> >(g.num_vertices());
	l_in_out_diff = vector<vector<LabelDiff> >(g.num_vertices());
	loadLabel(l_in, in_label);
	loadLabel(l_out, out_label);
	loadLabel(l_in_out_same, in_out_same_label);
	loadLabel(l_in_out_diff, in_out_diff_label);
	loadLabel(l_special, special_label);
}

void Greedylabel::loadLabel(vector<vector<Label> >& VertexLabelList, char* FileName){

	int subi;
	string sub, buf;	
	Label label;
	size_t old_found;
	size_t new_found;
	
	ifstream ifstr_(FileName);
	if(!ifstr_)
	{
		cout<<"error in locating a label file!"<<endl;
	}
	int i=0;
	
	while (getline(ifstr_, buf)) {
		if(i%1000==0)
		{
			cout<<"label loaded: "<<i<<endl;
		}
		
		old_found=0;
		new_found=buf.find("	");
		while (new_found!=string::npos) {
			
			sub = buf.substr(old_found, new_found-old_found);
			istringstream(sub) >> subi;
			label.dest1=subi;
			old_found=new_found+1;
			new_found=buf.find("	",new_found+1);
			
			sub = buf.substr(old_found, new_found-old_found);
			istringstream(sub) >> subi;
			label.dest2=subi;
			old_found=new_found+1;
			new_found=buf.find("	",new_found+1);
			
			sub = buf.substr(old_found, new_found-old_found);
			istringstream(sub) >> subi;
			label.dest3=subi;
			old_found=new_found+1;
			new_found=buf.find("	",new_found+1);
			
			sub = buf.substr(old_found, new_found-old_found);
			istringstream(sub) >> subi;
			label.port1=subi;
			old_found=new_found+1;
			new_found=buf.find("	",new_found+1);
			
			sub = buf.substr(old_found, new_found-old_found);
			istringstream(sub) >> subi;
			label.port2=subi;
			old_found=new_found+1;
			new_found=buf.find("	",new_found+1);
			
			sub = buf.substr(old_found, new_found-old_found);
			istringstream(sub) >> subi;
			label.distance=subi;
			old_found=new_found+1;
			new_found=buf.find("	",new_found+1);
		
			sub = buf.substr(old_found, new_found-old_found);
			istringstream(sub) >> subi;
			label.score_in=subi;
			old_found=new_found+1;
			new_found=buf.find("	",new_found+1);

			sub = buf.substr(old_found, new_found-old_found);
			istringstream(sub) >> subi;
			label.score_out=subi;
			old_found=new_found+1;
			new_found=buf.find("	",new_found+1);
			
			VertexLabelList[i].push_back(label);
		}
		
		i++;
	}
	cout<<"Number of labels loaded:"<<i<<endl;
	ifstr_.close();
}

void Greedylabel::loadLabel(vector<vector<LabelDiff> >& VertexLabelList, char* FileName){

	int subi;
	string sub, buf;	
	LabelDiff label_diff;
	size_t old_found;
	size_t new_found;
	
	ifstream ifstr_(FileName);
	if(!ifstr_)
	{
		cout<<"error in locating a label (diff) file!"<<endl;
	}
	int i=0;
	
	while (getline(ifstr_, buf)) {
		if(i%1000==0)
		{
			cout<<"(diff) label loaded: "<<i<<endl;
		}
		
		old_found=0;
		new_found=buf.find("	");
		while (new_found!=string::npos) {
			
			sub = buf.substr(old_found, new_found-old_found);
			istringstream(sub) >> subi;
			label_diff.dest1=subi;
			old_found=new_found+1;
			new_found=buf.find("	",new_found+1);
			
			sub = buf.substr(old_found, new_found-old_found);
			istringstream(sub) >> subi;
			label_diff.dest2=subi;
			old_found=new_found+1;
			new_found=buf.find("	",new_found+1);
			
			sub = buf.substr(old_found, new_found-old_found);
			istringstream(sub) >> subi;
			label_diff.dest3=subi;
			old_found=new_found+1;
			new_found=buf.find("	",new_found+1);
			
			sub = buf.substr(old_found, new_found-old_found);
			istringstream(sub) >> subi;
			label_diff.port1_in=subi;
			old_found=new_found+1;
			new_found=buf.find("	",new_found+1);
			
			sub = buf.substr(old_found, new_found-old_found);
			istringstream(sub) >> subi;
			label_diff.port2_in=subi;
			old_found=new_found+1;
			new_found=buf.find("	",new_found+1);

			sub = buf.substr(old_found, new_found-old_found);
			istringstream(sub) >> subi;
			label_diff.port1_out=subi;
			old_found=new_found+1;
			new_found=buf.find("	",new_found+1);
			
			sub = buf.substr(old_found, new_found-old_found);
			istringstream(sub) >> subi;
			label_diff.port2_out=subi;
			old_found=new_found+1;
			new_found=buf.find("	",new_found+1);			
			
			sub = buf.substr(old_found, new_found-old_found);
			istringstream(sub) >> subi;
			label_diff.distance=subi;
			old_found=new_found+1;
			new_found=buf.find("	",new_found+1);

			sub = buf.substr(old_found, new_found-old_found);
			istringstream(sub) >> subi;
			label_diff.score_in=subi;
			old_found=new_found+1;
			new_found=buf.find("	",new_found+1);

			sub = buf.substr(old_found, new_found-old_found);
			istringstream(sub) >> subi;
			label_diff.score_out=subi;
			old_found=new_found+1;
			new_found=buf.find("	",new_found+1);
		
			VertexLabelList[i].push_back(label_diff);
		}
		
		i++;
	}
	cout<<"Number of (diff) labels loaded:"<<i<<endl;
	ifstr_.close();
}

void Greedylabel::loadLabel(multimap<int, LabelSpecial>& VertexLabelList, char* FileName){

	int subi;
	string sub, buf;	
	LabelSpecial label_special;
	int vertexID;
	size_t old_found;
	size_t new_found;
	
	ifstream ifstr_(FileName);
	if(!ifstr_)
	{
		cout<<"error in locating a label (special) file!"<<endl;
	}
	
	while (getline(ifstr_, buf)) {
		
		old_found=0;
		new_found=buf.find("	");
		while (new_found!=string::npos) {
			
			sub = buf.substr(old_found, new_found-old_found);
			istringstream(sub) >> subi;
			vertexID=subi;
			old_found=new_found+1;
			new_found=buf.find("	",new_found+1);
			
			sub = buf.substr(old_found, new_found-old_found);
			istringstream(sub) >> subi;
			label_special.direction=subi;
			old_found=new_found+1;
			new_found=buf.find("	",new_found+1);
			
			sub = buf.substr(old_found, new_found-old_found);
			istringstream(sub) >> subi;
			label_special.dest=subi;
			old_found=new_found+1;
			new_found=buf.find("	",new_found+1);
			
			sub = buf.substr(old_found, new_found-old_found);
			istringstream(sub) >> subi;
			label_special.port=subi;
			old_found=new_found+1;
			new_found=buf.find("	",new_found+1);
			
			sub = buf.substr(old_found, new_found-old_found);
			istringstream(sub) >> subi;
			label_special.distance=subi;
			old_found=new_found+1;
			new_found=buf.find("	",new_found+1);
			
			sub = buf.substr(old_found, new_found-old_found);
			istringstream(sub) >> subi;
			label_special.ex_distance=subi;
			old_found=new_found+1;
			new_found=buf.find("	",new_found+1);

			sub = buf.substr(old_found, new_found-old_found);
			istringstream(sub) >> subi;
			label_special.score_in=subi;
			old_found=new_found+1;
			new_found=buf.find("	",new_found+1);

			sub = buf.substr(old_found, new_found-old_found);
			istringstream(sub) >> subi;
			label_special.score_out=subi;
			old_found=new_found+1;
			new_found=buf.find("	",new_found+1);
		
			VertexLabelList.insert(pair<int, LabelSpecial>(vertexID,label_special));
		}
	}
	cout<<"Number of (special) labels loaded:"<<VertexLabelList.size()<<endl;
	ifstr_.close();
}

Match Greedylabel::FindShortestDist(int src, int trg) {
	int pow16=int(pow(2.0,16.0));
	int pow8=int(pow(2.0,8.0));

	Match sdist_match;
	sdist_match.distance=MAX_VAL;
	if(src==trg)
	{
		sdist_match.distance=0;
		sdist_match.common_dest=src;
		return sdist_match;
	}
	
	int temp_distance=MAX_VAL;

	int outgoing_Dest;
	int incoming_Dest;
	
	vector<Label> L_outgoing;//outgoing
	MergeLabelVectorAscend(src, false, l_out[src],  l_in_out_same[src], l_in_out_diff[src], L_outgoing);
	
	vector<Label> L_incoming;//incoming
	MergeLabelVectorAscend(trg, true, l_in[trg], l_in_out_same[trg], l_in_out_diff[trg], L_incoming);
	
	//L_outgoing, L_incoming should contain at least one elememt after calling MergeLabelVectorAscend.
	vector<Label>::iterator iter_outgoing=L_outgoing.begin();
	vector<Label>::iterator iter_incoming=L_incoming.begin();
	
	while(true)
	{		            
		if (iter_outgoing==L_outgoing.end() || iter_incoming==L_incoming.end())
			break;
		
		outgoing_Dest=int(iter_outgoing->dest1)*pow16+int(iter_outgoing->dest2)*pow8+int(iter_outgoing->dest3); //To one vertex
		incoming_Dest=int(iter_incoming->dest1)*pow16+int(iter_incoming->dest2)*pow8+int(iter_incoming->dest3); //From one vertex			
		
		if(outgoing_Dest<incoming_Dest)
			iter_outgoing++;
		else if(outgoing_Dest>incoming_Dest)
			iter_incoming++;
		else
		{
			temp_distance=int(iter_outgoing->distance)+int(iter_incoming->distance);
			if(temp_distance<sdist_match.distance)
			{
				sdist_match.distance=temp_distance;
				sdist_match.out_offset=int(iter_outgoing->port1)*pow8+int(iter_outgoing->port2);
				sdist_match.in_offset=int(iter_incoming->port1)*pow8+int(iter_incoming->port2);
				sdist_match.common_dest=outgoing_Dest;
			}

			iter_outgoing++;
			iter_incoming++;
		}	
	}
	
	return sdist_match;
}

 
void Greedylabel::find_oneshortestpath(int src, int trg, vector<int>& path) {
	
	Match sdist_match=FindShortestDist(src, trg);
	find_onepath(src, trg, sdist_match, path, false);//A  shortest path must be simple path so there is no need to require at extra verification.
}

void Greedylabel::find_allpath(int src, int trg, vector<vector<int> >& pathcollection)//find simple kDLS paths. In the end no two paths are the same in pathcollection. 
{
	int pow16=int(pow(2.0,16.0));
	int pow8=int(pow(2.0,8.0));
	vector<Match> all_match;

	if(src==trg)
		return;
	
	int outgoing_Dest;
	int incoming_Dest;
	
	vector<Label> L_outgoing;//outgoing
	MergeLabelVectorAscend(src, false, l_out[src],  l_in_out_same[src], l_in_out_diff[src], L_outgoing);
	
	vector<Label> L_incoming;//incoming
	MergeLabelVectorAscend(trg, true, l_in[trg], l_in_out_same[trg], l_in_out_diff[trg], L_incoming);
	
	//L_outgoing, L_incoming should contain at least one elememt after calling MergeLabelVectorAscend.
	vector<Label>::iterator iter_outgoing=L_outgoing.begin();
	vector<Label>::iterator iter_incoming=L_incoming.begin();
	
	while(true)
	{		            
		if (iter_outgoing==L_outgoing.end() || iter_incoming==L_incoming.end())
			break;
		
		outgoing_Dest=int(iter_outgoing->dest1)*pow16+int(iter_outgoing->dest2)*pow8+int(iter_outgoing->dest3); //To one vertex
		incoming_Dest=int(iter_incoming->dest1)*pow16+int(iter_incoming->dest2)*pow8+int(iter_incoming->dest3); //From one vertex			
		
		if(outgoing_Dest<incoming_Dest)
			iter_outgoing++;
		else if(outgoing_Dest>incoming_Dest)
			iter_incoming++;
		else
		{
			int temp_distance=int(iter_outgoing->distance)+int(iter_incoming->distance);
			Match temp_match;
			temp_match.distance=temp_distance;
			temp_match.out_offset=int(iter_outgoing->port1)*pow8+int(iter_outgoing->port2);
			temp_match.in_offset=int(iter_incoming->port1)*pow8+int(iter_incoming->port2);
			temp_match.common_dest=outgoing_Dest;
			all_match.push_back(temp_match);
			iter_outgoing++;
			iter_incoming++;
		}	
	}
	
	for(vector<Match>::iterator iter_match=all_match.begin(); iter_match!=all_match.end(); iter_match++)
	{
		vector<int> path;
		find_onepath(src, trg, *iter_match, path, true);//only simple kDLS paths.
		if (path.size()>0)
			pathcollection.push_back(path);
	}
}


int Greedylabel::find_allpathnum(int src, int trg)//Find the number of all kDLS paths (including non-simple paths).
{
	int pathnum=0;
	int pow16=int(pow(2.0,16.0));
	int pow8=int(pow(2.0,8.0));

	if(src==trg)
		return pathnum;

	int outgoing_Dest;
	int incoming_Dest;
	
	vector<Label> L_outgoing;//outgoing
	MergeLabelVectorAscend(src, false, l_out[src],  l_in_out_same[src], l_in_out_diff[src], L_outgoing);
	
	vector<Label> L_incoming;//incoming
	MergeLabelVectorAscend(trg, true, l_in[trg], l_in_out_same[trg], l_in_out_diff[trg], L_incoming);
	
	//L_outgoing, L_incoming should contain at least one elememt after calling MergeLabelVectorAscend.
	vector<Label>::iterator iter_outgoing=L_outgoing.begin();
	vector<Label>::iterator iter_incoming=L_incoming.begin();
	
	while(true)
	{		            
		if (iter_outgoing==L_outgoing.end() || iter_incoming==L_incoming.end())
			break;
		
		outgoing_Dest=int(iter_outgoing->dest1)*pow16+int(iter_outgoing->dest2)*pow8+int(iter_outgoing->dest3); //To one vertex
		incoming_Dest=int(iter_incoming->dest1)*pow16+int(iter_incoming->dest2)*pow8+int(iter_incoming->dest3); //From one vertex			
		
		if(outgoing_Dest<incoming_Dest)
			iter_outgoing++;
		else if(outgoing_Dest>incoming_Dest)
			iter_incoming++;
		else
		{
			pathnum++;
			iter_outgoing++;
			iter_incoming++;
		}	
	}
	return pathnum;
}

int Greedylabel::TotalTransitivePathNum_Contribution(const int& src, const int& trg, double& pContribution, const double& EXPBASE, int& score_in) const//special calculation for KDD: direct edges are not counted. non-simple paths are included.
{
	int pathnum=0;
	int pow16=int(pow(2.0,16.0));
	int pow8=int(pow(2.0,8.0));
	int total_dist=0;
	
	if(src==trg)
		return pathnum;

	int outgoing_Dest;
	int incoming_Dest;
	
	vector<Label> L_outgoing;//outgoing
	MergeLabelVectorAscend(src, false, l_out[src],  l_in_out_same[src], l_in_out_diff[src], L_outgoing);
	
	vector<Label> L_incoming;//incoming
	MergeLabelVectorAscend(trg, true, l_in[trg], l_in_out_same[trg], l_in_out_diff[trg], L_incoming);
	
	//L_outgoing, L_incoming should contain at least one elememt after calling MergeLabelVectorAscend.
	vector<Label>::iterator iter_outgoing=L_outgoing.begin();
	vector<Label>::iterator iter_incoming=L_incoming.begin();
	
	while(true)
	{		            
		if (iter_outgoing==L_outgoing.end() || iter_incoming==L_incoming.end())
			break;
		
		outgoing_Dest=int(iter_outgoing->dest1)*pow16+int(iter_outgoing->dest2)*pow8+int(iter_outgoing->dest3); //To one vertex
		incoming_Dest=int(iter_incoming->dest1)*pow16+int(iter_incoming->dest2)*pow8+int(iter_incoming->dest3); //From one vertex			
		
		if(outgoing_Dest<incoming_Dest)
			iter_outgoing++;
		else if(outgoing_Dest>incoming_Dest)
			iter_incoming++;
		else
		{
			int temp_distance=int(iter_outgoing->distance)+int(iter_incoming->distance);
			if(temp_distance>1) {
				int score = iter_outgoing->score_out * iter_incoming->score_in / g.get_rank(outgoing_Dest);
				score_in = score;
				pContribution = pContribution + 1.0/double(score) * (1.0/double(pow(EXPBASE,double(temp_distance-1))));
				pathnum++;
			}
			iter_outgoing++;
			iter_incoming++;
		}	
	}

	return pathnum;
}

void Greedylabel::find_allshortestpath(int src, int trg, vector<vector<int> >& pathcollection)
{//Note by YX: I did not call find FindShortestDist to reduce repeated calculation.

	//find shortest distance.
	int pow16=int(pow(2.0,16.0));
	int pow8=int(pow(2.0,8.0));
	
	int distance=MAX_VAL;
	if(src==trg)
		return;
	
	int temp_distance=MAX_VAL;

	int outgoing_Dest;
	int incoming_Dest;
	
	vector<Label> L_outgoing;//outgoing
	MergeLabelVectorAscend(src, false, l_out[src],  l_in_out_same[src], l_in_out_diff[src], L_outgoing);
	
	vector<Label> L_incoming;//incoming
	MergeLabelVectorAscend(trg, true, l_in[trg], l_in_out_same[trg], l_in_out_diff[trg], L_incoming);
	
	//L_outgoing, L_incoming should contain at least one elememt after calling MergeLabelVectorAscend.
	vector<Label>::iterator iter_outgoing=L_outgoing.begin();
	vector<Label>::iterator iter_incoming=L_incoming.begin();
	
	while(true)
	{		            
		if (iter_outgoing==L_outgoing.end() || iter_incoming==L_incoming.end())
			break;
		
		outgoing_Dest=int(iter_outgoing->dest1)*pow16+int(iter_outgoing->dest2)*pow8+int(iter_outgoing->dest3); //To one vertex
		incoming_Dest=int(iter_incoming->dest1)*pow16+int(iter_incoming->dest2)*pow8+int(iter_incoming->dest3); //From one vertex			
		
		if(outgoing_Dest<incoming_Dest)
			iter_outgoing++;
		else if(outgoing_Dest>incoming_Dest)
			iter_incoming++;
		else
		{
			temp_distance=int(iter_outgoing->distance)+int(iter_incoming->distance);
			if(temp_distance<distance)
				distance=temp_distance;
	
			iter_outgoing++;
			iter_incoming++;
		}	
	}
	

	//start build shortest paths.
	if (distance==MAX_VAL)
		return;
		
	iter_outgoing=L_outgoing.begin();
	iter_incoming=L_incoming.begin();

	vector<Match> all_shortestmatch;
	
	while(true)
	{		            
		if (iter_outgoing==L_outgoing.end() || iter_incoming==L_incoming.end())
			break;
		
		outgoing_Dest=int(iter_outgoing->dest1)*pow16+int(iter_outgoing->dest2)*pow8+int(iter_outgoing->dest3); //To one vertex
		incoming_Dest=int(iter_incoming->dest1)*pow16+int(iter_incoming->dest2)*pow8+int(iter_incoming->dest3); //From one vertex			
		
		if(outgoing_Dest<incoming_Dest)
			iter_outgoing++;
		else if(outgoing_Dest>incoming_Dest)
			iter_incoming++;
		else
		{
			int temp_distance=int(iter_outgoing->distance)+int(iter_incoming->distance);
			if (temp_distance==distance)
			{
				Match temp_match;
				temp_match.distance=temp_distance;
				temp_match.out_offset=int(iter_outgoing->port1)*pow8+int(iter_outgoing->port2);
				temp_match.in_offset=int(iter_incoming->port1)*pow8+int(iter_incoming->port2);
				temp_match.common_dest=outgoing_Dest;
				all_shortestmatch.push_back(temp_match);
			}
			iter_outgoing++;
			iter_incoming++;			
		}	
	}	
	
	for(vector<Match>::iterator iter_match=all_shortestmatch.begin(); iter_match!=all_shortestmatch.end(); iter_match++)
	{
		vector<int> path;
		find_onepath(src, trg, *iter_match, path, false);//shortest path must be simple path so there is no need to require at extra verification. 
		pathcollection.push_back(path);
	}

}
