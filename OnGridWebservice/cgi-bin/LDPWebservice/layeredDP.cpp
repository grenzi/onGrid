#include "layeredDP.h"
#include "Dictionary.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <algorithm> 

using namespace std;

layeredDP::layeredDP(){}

/*
float layeredDP::getMav(vector<string>req,vector<string>entryName){
	float mav;
	unsigned int rs = req.size();
	unsigned int es = entryName.size();
	vector< vector<float> > s(rs, vector<float>(es,0));
	for(unsigned int i=0;i<rs;i++){
		for(unsigned int j=0;j<es;j++){
			float x;
			vector<char> r = convert(req.at(i));
			vector<char> e = convert(entryName.at(j));
			vector< vector<float> > c = LCSLength(r,e);
			int rrs = r.size();
			int ees = e.size();
			float value = c.at(rrs-1).at(ees-1);
			x = value/((rrs-1+ees-1)/2);
			s.at(i).at(j) = x;
		}
	}
	for(unsigned int i=1;i<rs;i++){
		for(unsigned int j=1;j<es;j++){
			if((s.at(i-1).at(j-1)+s.at(i).at(j))>max(s.at(i).at(j-1),s.at(i-1).at(j))){
				s.at(i).at(j)=s.at(i-1).at(j-1)+s.at(i).at(j);
			}else{
				s.at(i).at(j)=max(s.at(i).at(j-1),s.at(i-1).at(j));
			}
		}
	}
	mav = s.at(rs-1).at(es-1);
	return mav;
}
*/

float layeredDP::arb_order_score(const vector<string>& req_vect,const vector< pair<string,int> >& name, vector< vector<float> >&scores){
	/*
	float score = 0;
	vector< pair<string,int> > temp_name = name;
	unsigned int req_size = req_vect.size();
	unsigned int name_size = name.size();
	for(unsigned int i=0;i<req_size;i++){
		float max_word_score = 0;
		int max_word_index = 0;
		for(unsigned int j=0;j<temp_name.size();j++){
			float temp_score = scores.at(i).at(temp_name.at(j).second);
			if(temp_score>max_word_score){
				max_word_score = temp_score;
				max_word_index = j;
			}
			//cout<<req_vect.at(i)<<", "<<temp_name.at(j).first<<", "<<temp_score<<endl;
		}
		score = score+max_word_score;
		temp_name.erase(temp_name.begin()+max_word_index);
		if(temp_name.size()==0){
			break;
		}
	}
	
	score = score*2/(float)(req_size-1+name_size-1);
	
	//cout<<temp_name.size()<<", "<<name.size()<<endl;
	//cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
	return score;
	*/
	/*
	float max_word_similarity = 0.0;
	float arb_order_score = 0.0;
	int query_index=0;//index in query name for the word having maximum similarity with word in metathesaurus name
	int meta_index=0;//index in metathesaurus name for word having maximum similarity with word in query name

	size_t row_size = req_vect.size();
	size_t column_size = name.size();
	vector< vector<float> > word_similarity_matrix(row_size,vector<float>(column_size,0));
	for(size_t i=0;i<row_size;i++){
		for(size_t j=0;j<column_size;j++){
			word_similarity_matrix[i][j] = scores[i][name[j].second];
			if(word_similarity_matrix[i][j]>max_word_similarity){
				max_word_similarity = word_similarity_matrix[i][j];
				query_index = i;
				meta_index = j;
			}
		}
	}

	arb_order_score = arb_order_score+max_word_similarity;
	max_word_similarity = 0.0;
	erase_row_column(word_similarity_matrix,query_index,meta_index);//from the word_similarity_matrix, erase the row and coulumn which has the maximum score value
	row_size--;
	column_size--;
	query_index = 0;
	meta_index = 0;

	
	while(row_size>0&&column_size>0){//to make sure there are still both rows and lines in the matrix, otherwise stop loop and get the final score
		for(size_t i=0;i<row_size;i++){
			for(size_t j=0;j<column_size;j++){
				if(word_similarity_matrix[i][j]>max_word_similarity){
					max_word_similarity = word_similarity_matrix[i][j];
					query_index = i;
					meta_index = j;
				}
			}
		}

		arb_order_score = arb_order_score+max_word_similarity;
		max_word_similarity = 0.0;
		erase_row_column(word_similarity_matrix,query_index,meta_index);//from the word_similarity_matrix, erase the row and coulumn which has the maximum score value
		row_size--;
		column_size--;
		query_index = 0;
		meta_index = 0;
	}
	
	arb_order_score = arb_order_score*2.0/(float)(req_vect.size()+name.size());
	
	return arb_order_score;
	*/
	
	//Using maximal matching
	vector<bool> ifused_query(req_vect.size(),false);//if a word in query term is used for matching, its corresponding bool value is true, and it won't be
													//used for other matches later
	vector<bool> ifused_meta(name.size(),false);
	vector<scoreVert> score_vertexes;//scoreVert is defined as a structure in layeredDP.h 
	for(size_t i=0;i<req_vect.size();i++){
		for(size_t j=0;j<name.size();j++){
			scoreVert sv;
			//indicating the vertexes and edge weights in a graph
			//vertexes are the indexes in the requested querey term and in the name from metathesaurus
			sv.score = scores[i][name[j].second];
			sv.vertex_query = i;
			sv.vertex_meta = j;
			
			score_vertexes.push_back(sv);
		}
	}
	sort(score_vertexes.begin(),score_vertexes.end(),score_vertexes_sortfunction);//sort the vector into a decreasing order
	size_t query_size = req_vect.size();
	size_t meta_size = name.size();
	float arb_order_score = 0.0;
	for(size_t i=0;i<score_vertexes.size();i++){
		if((query_size==0)||(meta_size==0)){
			break;
		}//if all words in query term or in name from metathesaurus have been used, break
		if((!ifused_query[score_vertexes[i].vertex_query])&&(!ifused_meta[score_vertexes[i].vertex_meta])){
			arb_order_score += score_vertexes[i].score;
			ifused_query[score_vertexes[i].vertex_query] = true;
			ifused_meta[score_vertexes[i].vertex_meta] = true;
			query_size--;
			meta_size--;
		}//if one of the two vertexes is used, continue to the next
	}
	arb_order_score = arb_order_score*2.0/((float)(req_vect.size()+name.size()));
	return arb_order_score;
}

bool layeredDP::score_vertexes_sortfunction(const scoreVert& sv1, const scoreVert& sv2){
	return (sv1.score>sv2.score);
}
/*
void layeredDP::erase_row_column(vector< vector<float> >& word_similarity_matrix,const int& query_index,const int& meta_index){
	word_similarity_matrix.erase(word_similarity_matrix.begin()+query_index);//erase row
	for(size_t i=0;i<word_similarity_matrix.size();i++){
		word_similarity_matrix[i].erase(word_similarity_matrix[i].begin()+meta_index);//erase column
	}
}
*/
float layeredDP::ConceptSimilarityScore(const vector<string>& req, vector< pair<string,int> >& name, vector< vector<float> >& scores, vector< vector<float> >& s, const int& leng){
	unsigned int req_size = req.size();
	unsigned int name_size = name.size();
	float match_value;
	
	for(unsigned int i=0;i<s.size();i++){
		s.at(i).resize(name_size+1,0);
	}
	/*
	for(unsigned int i=0;i<req_size;i++){
		for(unsigned int j=leng;j<name_size;j++){
			s.at(i).at(j)=scores.at(i).at(name.at(j).second);
		}
	}
	
	for(unsigned int i=leng;i<name_size;i++){
		s.at(1).at(i) = scores.at(1).at(name.at(i).second);
	}
	
	if(leng==1){
		for(unsigned int i=1;i<req_size;i++){
			s.at(i).at(1) = scores.at(i).at(name.at(0).second);
		}
	}
	cout<<leng<<"leng!"<<endl;
	*/
	for(unsigned int i=1;i<req_size+1;i++){
		for(unsigned int j=leng+1;j<name_size+1;j++){
			float temp_score = scores.at(i-1).at(name.at(j-1).second);
			if((s.at(i-1).at(j-1)+temp_score)>max(s.at(i).at(j-1),s.at(i-1).at(j))){
				s.at(i).at(j)=s.at(i-1).at(j-1)+temp_score;
			}else{
				s.at(i).at(j)=max(s.at(i).at(j-1),s.at(i-1).at(j));
			}
		}
	}
	match_value = s.at(req_size).at(name_size)*2/(float)(req_size+name_size);
	return match_value;
}

vector<char> layeredDP::convert(const string& x){
	vector<char> v(x.begin(), x.end());
	//v.insert(v.begin(),' ');
	return v;
}

float layeredDP::WordSimilarity_caseSensitive(const vector<char>& x_vect, const vector<char>& y_vect, vector <vector <float> >& c, const int& leng){
	unsigned int x_size = x_vect.size();
	unsigned int y_size = y_vect.size();

	for(unsigned int i=0;i<c.size();i++){
		c[i].resize(y_size+1,0);
	}
	
	for(unsigned int i=1;i<x_size+1;i++){
		for(unsigned int j=leng+1;j<y_size+1;j++){
			if(x_vect.at(i-1)==y_vect.at(j-1)){
				c.at(i).at(j) = c.at(i-1).at(j-1)+1;
			}else{
				c.at(i).at(j) = max(c.at(i-1).at(j),c.at(i).at(j-1));
			}
		}
	}

	float word_score = c.at(x_size).at(y_size)/((float)(x_size+y_size)/2);
	return word_score;
}


float layeredDP::WordSimilarity_caseInsensitive(const vector<char>& x_vect, const vector<char>& y_vect, vector <vector <float> >& c, const int& leng){
	unsigned int x_size = x_vect.size();
	unsigned int y_size = y_vect.size();

	for(unsigned int i=0;i<c.size();i++){
		c[i].resize(y_size+1,0);
	}
	
	for(unsigned int i=1;i<x_size+1;i++){
		for(unsigned int j=leng+1;j<y_size+1;j++){
			if(x_vect.at(i-1)==y_vect.at(j-1) || (is_letter(x_vect[i-1]) && is_letter(y_vect[j-1]) && abs(x_vect[i-1] - y_vect[j-1]) == 32)){
				c.at(i).at(j) = c.at(i-1).at(j-1)+1;
			}else{
				c.at(i).at(j) = max(c.at(i-1).at(j),c.at(i).at(j-1));
			}
		}
	}

	float word_score = c.at(x_size).at(y_size)/((float)(x_size+y_size)/2);
	return word_score;
}

bool layeredDP::is_letter(const char &x){
	return ((x <= 122 && x >= 97)||(x <= 90 && x >= 65));
}


/*
float layeredDP::continuous_match_WordSimilarity(vector<char> x,vector<char> y){
	unsigned int cx = x.size();
	unsigned int cy = y.size();
	vector< vector<float> > c(cx, vector<float>(cy,0.0));
	for(unsigned int i=1;i<cx;i++){
		for(unsigned int j=1;j<cy;j++){
			if(x.at(i)==y.at(j)){
				c.at(i).at(j) = c.at(i-1).at(j-1)+1.0;
			}else{
				c.at(i).at(j) = max(c.at(i-1).at(j),c.at(i).at(j-1));
			}
		}
	}
	float score = c.at(cx-1).at(cy-1)*2.0/((float)(cx-1+cy-1));
	return score;
}

*/

/*
vector<string> layeredDP::getLCS(vector< vector<int> >c, vector<char> x, vector<char> y,int i,int j){
	vector<string> R;
	if((i==0)||(j==0)){
		R.push_back("");
		return R;
	}else if(x.at(i)==y.at(j)){
		R = getLCS(c,x,y,i-1,j-1);
		char xi = x.at(i);
		for(unsigned int p=0;p<R.size();p++){
			R.at(p).append(1,xi);
		}
	}
	else{
		if(c.at(i).at(j-1)>c.at(i-1).at(j)){
			R=getLCS(c,x,y,i,j-1);
		}else if(c.at(i-1).at(j)>c.at(i).at(j-1)){
			R=getLCS(c,x,y,i-1,j);
		}else{
			vector<string> R1 = getLCS(c,x,y,i,j-1);
			vector<string> R2 = getLCS(c,x,y,i-1,j);
			for(unsigned int p=0;p<R1.size();p++){
				R.push_back(R1.at(p));
			}
			for(unsigned int p=0;p<R2.size();p++){
				R.push_back(R2.at(p));
			}
		}
	}
	return R;
}*/

