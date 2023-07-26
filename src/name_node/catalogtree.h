#include <sys/io.h>
#include <iostream>
#include <vector>
#include<string>
#include <unordered_map>
#include <map>
using namespace std;
// char sign='\\';//linux��������'\/' 
// string sig="\\";

struct Node
{
    std::string file_name;
	std::string file_path;
	long long  size=0;
    bool iscatalogue;//是否为目录
	Node(const Node& n)
	:file_name(n.file_name)
	,file_path(n.file_path)
	,size(n.size)
	,iscatalogue(n.iscatalogue)
	,chunk_handles(n.chunk_handles)
	{
		
	}
	Node()
	{}
	std::unordered_map<int,int64_t> chunk_handles;//chunk index to chunkhandle
};
std::unordered_map<std::string,Node*> mapNode;//路径到Node的映射

class FileTree{
    std::unordered_map<std::string,FileTree*> m;//目录名与结点
    //std::string path;//目录路径
    std::vector<Node*> v; //当前目录结点
	public:
	//规定文件名字不带/ 
	//规定文件路径以/开头
	FileTree* Findcatalogue(std::string filepath,string& filename)
	{
		FileTree* ret=this;
		std::string path="";
		int index;
		while((index=filepath.find("/",1))!=string::npos)
		{
			
			//切割
			path=string(filepath.begin()+1,filepath.begin()+index);
			cout<<path<<endl;
			if(m.find(path)==m.end())
			{
				return nullptr;
			}
			ret=m[path];
			filepath=string(filepath.begin()+index,filepath.end());
		}
		path=string(filepath.begin()+1,filepath.end());
		cout<<path<<endl;
		filename=path;
		//path name
		return ret;
	}
    bool insert(Node node)//插入当前结点到该路径
    {//已经到达了目录树
		Node* n=new Node(node);
		if(mapNode.find(node.file_path)!=mapNode.end())
		{
			cout<<"已经存在该文件了"<<endl;
			return false;
		}
		string filename="";
		FileTree* cur=Findcatalogue(node.file_path,filename);
		if(cur==nullptr)
		{
			cout<<"没有该目录插入失败"<<endl;
		}

		if(node.iscatalogue)
		{
			
			//是目录
			FileTree* f=new FileTree();
			cur->m[node.file_name]=f;
		}
		mapNode[node.file_path]=n;
		cur->v.push_back(n);

        //目录特殊处理
        //加锁并添加到map中
    }
    bool erase(string filepath)//删除文件的名字
    {
		if(mapNode.find(filepath)==mapNode.end())
		{
			cout<<"删除失败不存在"<<endl;
			return false;
		}
		string filename="";
		FileTree* cur=Findcatalogue(filepath,filename);
		if(m.find(filename)!=m.end())
		{
			//目录
			cur->m.erase(filename);
		}
		else
		{
			//非目录
			mapNode.erase(filepath);
		}
		//删除
		auto it=cur->v.begin();
		while(it!=cur->v.end())
		{
			if((*it)->file_path==filepath)
			{
				cur->v.erase(it);
				return true;;
			}
			it++;
		}
		if(it==cur->v.end())
		{
			std::cout<<"vector没找到文件"<<std::endl;
			return false;
		}

    }
    bool rename(std::string srcpath,std::string destpath)
    {
		if(mapNode.find(srcpath)==mapNode.end())
		{
			cout<<"原文件不存在"<<endl;
			return false;
		}
		if(mapNode.find(destpath)!=mapNode.end())
		{
			cout<<"已经存在目标文件"<<endl;
			return false;
		}
		
		string name="";
		FileTree* cur=Findcatalogue(srcpath,name);
		
		string destname=string(destpath.begin()+destpath.rfind("/")+1,destpath.end());
        //修改map
       Node tmp=*mapNode[srcpath];
	   tmp.file_path=destpath;
	   tmp.file_name=destname;
	   mapNode.erase(srcpath);
		
	   cout<<"name"<<name<<endl;
	   cout<<"destname"<<destname<<endl;
	 	auto it=cur->v.begin();
		while(it!=cur->v.end())
		{
			if((*it)->file_path==srcpath)
			{
				cout<<"yes"<<endl;
				cur->v.erase(it);
				break;
			}
			it++;
		}

		insert(tmp);
		
    }
    void showlist(string path)//最后要加/
    {
		string filename="";
        //打印当前路径的原数据信息
		FileTree* cur=Findcatalogue(path,filename);

		for(auto& x:cur->v)
		{
		
			cout<<x->file_name<<" size:"<<x->size<<std::endl;
			
		}

    }
	void Stat(string filepath)
	{
		string filename="";
		FileTree* cur=Findcatalogue(filepath,filename);
		for(auto& x:cur->v)
		{
			if(x->file_path==filepath)
			{
				cout<<x->file_name<<" size:"<<x->size<<std::endl;
				return;
			}
		}
		cout<<"Not Found"<<endl;
	}  

};


int main(){
	// FileTree f;
	// string ret;
	// f.Findcatalogue("/",ret);
	// cout<<"------"<<endl;
	// f.Findcatalogue("/A",ret);
	// cout<<"------"<<endl;
	// f.Findcatalogue("/A/B",ret);
	// cout<<"------"<<endl;
	// f.Findcatalogue("/ABB",ret);
	// cout<<"------"<<endl;

	cout<<"hello"<<endl;
	FileTree f;
	f.showlist("/");
	cout<<"------------------"<<endl;
	Node n;
	n.file_name="A";
	n.file_path="/A";
	n.size=1;
	n.iscatalogue=false;
	f.insert(n);
	f.showlist("/");
	cout<<"------"<<endl;
	Node n1;
	n1.file_name="B";
	n1.file_path="/B";
	n1.size=1;
	n1.iscatalogue=true;
	f.insert(n1);
	f.showlist("/");
	cout<<"-------------"<<endl;
	Node n2;
	n2.file_name="B";
	n2.file_path="/B/B";
	n2.size=3;
	n1.iscatalogue=false;
	f.insert(n2);
	f.showlist("/B/");
	cout<<"-------------"<<endl;
	f.showlist("/");
	f.rename("/A","/C");
	f.showlist("/");
	

	// string file_path="/A";
	// auto node=mapNode[file_path];
	// cout<<node->file_name<<" size"<<node->size<<" "<<node->file_path<<endl;
	// f.Stat("/AB");
	// f.erase("A","/A");

	// f.Stat("/A");
	// f.showlist();
	// n.file_name="A";
	// n.file_path="/A/A";
	// n.size=1;
	// n.iscatalogue=false;
	// f.insert(n);
	// f.showlist();

	return 0;
}