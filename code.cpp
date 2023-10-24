#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <cstdlib>


// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory
using namespace std;

// Our graph consists of a list of nodes where each node is represented as follows:
class Graph_Node{

private:
	string Node_Name;  // Variable name 
	vector<int> Children; // Children of a particular node - these are index of nodes in graph.
	vector<string> Parents; // Parents of a particular node- note these are names of parents
	int nvalues;  // Number of categories a variable represented by this node can take
	vector<string> values; // Categories of possible values
	vector<float> CPT; // conditional probability table as a 1-d array . Look for BIF format to understand its meaning

public:
	// Constructor- a node is initialised with its name and its categories
    Graph_Node(string name,int n,vector<string> vals)
	{
		Node_Name=name;
	
		nvalues=n;
		values=vals;
		

	}
	string get_name()
	{
		return Node_Name;
	}
	vector<int> get_children()
	{
		return Children;
	}
	vector<string> get_Parents()
	{
		return Parents;
	}
	vector<float> get_CPT()
	{
		return CPT;
	}
	int get_nvalues()
	{
		return nvalues;
	}
	vector<string> get_values()
	{
		return values;
	}
	void set_CPT(vector<float> new_CPT)
	{
		CPT.clear();
		CPT=new_CPT;
	}
    void set_Parents(vector<string> Parent_Nodes)
    {
        Parents.clear();
        Parents=Parent_Nodes;
    }
    // add another node in a graph as a child of this node
    int add_child(int new_child_index )
    {
        for(int i=0;i<Children.size();i++)
        {
            if(Children[i]==new_child_index)
                return 0;
        }
        Children.push_back(new_child_index);
        return 1;
    }



};


 // The whole network represted as a list of nodes
class network{

	list <Graph_Node> Pres_Graph;

public:
	int addNode(Graph_Node node)
	{
		Pres_Graph.push_back(node);
		return 0;
	}
    
    
	int netSize()
	{
		return Pres_Graph.size();
	}
    // get the index of node with a given name
    int get_index(string val_name)
    {
        list<Graph_Node>::iterator listIt;
        int count=0;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(listIt->get_name().compare(val_name)==0)
                return count;
            count++;
        }
        return -1;
    }
// get the node at nth index
    list<Graph_Node>::iterator get_nth_node(int n)
    {
       list<Graph_Node>::iterator listIt;
        int count=0;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(count==n)
                return listIt;
            count++;
        }
        return listIt; 
    }
    //get the iterator of a node with a given name
    list<Graph_Node>::iterator search_node(string val_name)
    {
        list<Graph_Node>::iterator listIt;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(listIt->get_name().compare(val_name)==0)
                return listIt;
        }
    
            cout<<"node not found\n";
        return listIt;
    }
	

};

network read_network()
{
	network Alarm;
	string line;
	int find=0;
  	ifstream myfile("alarm.bif");
  	string temp;
  	string name;
  	vector<string> values;
  	
    if (myfile.is_open())
    {
    	while (! myfile.eof() )
    	{
    		stringstream ss;
      		getline (myfile,line);
      		
      		
      		ss.str(line);
     		ss>>temp;
     		
     		
     		if(temp.compare("variable")==0)
     		{
                    
     				ss>>name;
     				getline (myfile,line);
                   
     				stringstream ss2;
     				ss2.str(line);
     				for(int i=0;i<4;i++)
     				{
     					
     					ss2>>temp;
     					
     					
     				}
     				values.clear();
     				while(temp.compare("};")!=0)
     				{
     					values.push_back(temp);
     					
     					ss2>>temp;
    				}
     				Graph_Node new_node(name,values.size(),values);
     				int pos=Alarm.addNode(new_node);

     				
     		}
     		else if(temp.compare("probability")==0)
     		{
                    
     				ss>>temp;
     				ss>>temp;
     				
                    list<Graph_Node>::iterator listIt;
                    list<Graph_Node>::iterator listIt1;
     				listIt=Alarm.search_node(temp);
                    int index=Alarm.get_index(temp);
                    ss>>temp;
                    values.clear();
     				while(temp.compare(")")!=0)
     				{
                        listIt1=Alarm.search_node(temp);
                        listIt1->add_child(index);
     					values.push_back(temp);
     					
     					ss>>temp;

    				}
                    listIt->set_Parents(values);
    				getline (myfile,line);
     				stringstream ss2;
                    
     				ss2.str(line);
     				ss2>> temp;
                    
     				ss2>> temp;
                    
     				vector<float> curr_CPT;
                    string::size_type sz;
     				while(temp.compare(";")!=0)
     				{
                        
     					curr_CPT.push_back(atof(temp.c_str()));
     					
     					ss2>>temp;
                       
                        

    				}
                    
                    listIt->set_CPT(curr_CPT);


     		}
            else
            {
                
            }
     		
     		

    		
    		
    	}
    	
    	if(find==1)
    	myfile.close();
  	}
  	
  	return Alarm;
}



class Solve{

public:
vector <vector<string>> datavec_init;
vector <vector<string>> datavec;
network Alarm;


void readRecord(){
	ifstream datafile("records.dat");
	
	if (datafile.is_open())
    {
    	while (! datafile.eof() )
    	{
    		stringstream ss;
			string data,component;
      		getline (datafile,data);
      		ss.str(data);
     		ss>>component;
     		
     		vector <string> temp;
			int missing_index;
			int i=0;
     		while(component.compare("\n")!=0)
     		{
				if(component.compare("\"?\"")==0){
					missing_index=i;
				}
				temp.push_back(component);
				ss>>component;	
				i++;
			}
			temp.push_back(to_string(missing_index));
			temp.push_back(to_string(1));
			datavec.push_back(temp);
			ss.clear();
		}
	// Print read data
	datavec_init=datavec;
	for(int i=0;i<datavec.size();i++){
		for(auto j: datavec[i]){
			cout<<j;
		}
	}
	}
}

void initializeNetwork(){
	for(int i=0;i<Alarm.netSize();i++){
			auto this_node=Alarm.get_nth_node(i);
			auto this_node_values=this_node->get_values();
			vector <vector<string>> child_values;
			for(auto k: this_node->get_children()){
				auto child_node=Alarm.get_nth_node(k);
				child_values.push_back(child_node->get_values());
			}

			for(auto )
			
	}
}

void updateNetwork(){
	
}


int find_index(vector<string> vec, string sought){
 for(int i=0;i<vec.size();i++){
	if(vec[i]==sought){
		return i;
	}
 }
 return 0;
}

int get_offset(int index, vector<string> data){
	auto node_iter=Alarm.get_nth_node(index);
	auto node_val=find_index(node_iter->get_values(),data[index]);
	vector<string> Par_of_node=node_iter->get_Parents();
	vector <list<Graph_Node>::iterator> Par_iter;
	vector<int> par_val;
	for(auto i:Par_of_node){
		Par_iter.push_back(Alarm.search_node(i));
		par_val.push_back(find_index(Alarm.search_node(i)->get_values(),data[index]));
	}
	
	int offset=0;
	for(int i=0;i<Par_iter.size();i++){
		int temp=par_val[i];
		for(int j=i+1;j<Par_iter.size();j++)
			temp*=Par_iter[j]->get_nvalues();
		offset+=temp;
	}
	float temp=node_val;
	for(int i=0;i<Par_iter.size();i++){
		temp*=Par_iter[i]->get_nvalues();
	}
	offset+=temp;
	return offset;
}




void updateRecords(){
	auto updated_data=datavec;
	for(int i=0;i<datavec.size();i++){
		auto missing_node_iter=Alarm.get_nth_node(stoi(datavec[i][datavec.size()-2]));
		vector <list<Graph_Node>::iterator> child_nodes_iter;
		vector<int> child_class;
		for(auto k: missing_node_iter->get_children()){
			child_nodes_iter.push_back(Alarm.get_nth_node(k));
		}
		for(int j=0;j<missing_node_iter->get_values().size();j++){
			vector<string> temp_data=datavec[i];
			temp_data[stoi(datavec[i][datavec.size()-2])]=missing_node_iter->get_values()[j];
			float weight=missing_node_iter->get_CPT()[get_offset(stoi(datavec[i][datavec.size()-2]),temp_data)];
			for(int k=0;k<child_nodes_iter.size();k++){
				weight*=child_nodes_iter[k]->get_CPT()[get_offset(missing_node_iter->get_children()[k],temp_data)];
			}
			temp_data[temp_data.size()-1]=to_string(weight);
			updated_data.push_back(temp_data);
		}
		updated_data.erase(updated_data.begin()+i);
	}

}

Solve(){
	Alarm=read_network();
	datavec_init.resize(0);
	datavec.resize(0);
	initializeNetwork();
	while(true){
		updateRecords();
		updateNetwork();
	}
}

// void initializeNetwork(){

// }
// void updateNetwork(){

// }
// void updateRecords(){
	
// }
};

int main()
{
	
    
// Example: to do something


	cout<<"Perfect! Hurrah! \n";
	
}




