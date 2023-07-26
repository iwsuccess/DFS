#include <sys/io.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <dirent.h>
#include "bits/stdc++.h"
#include <sys/stat.h>
#include "mySemaphore.cpp"
using namespace std;
char sign='/';//linux��������'\/'
string sig="/";
struct FileTree{
	string file_name;
	string file_path;
	long long  size=0;
    int file_type;     // 1: folder; 0: file
    int parent;
	unordered_map<int,int64_t> chunk_handles;
	vector<int> next;
	int x,y;
	FileTree(){}
	FileTree(string f_n,string f_p,long long _size,int f_t){
		file_name=f_n;
		file_path=f_p;
		size     =_size;
        file_type=f_t;
	}
};
class GetFiles{
private:
	int all;
	int mx;
	int change;
	string output[102400];
	vector<FileTree> tr;
	vector<int> Res;//��ɾ���Ľڵ��Ǵ��ڿ���״̬�������ظ����� 
	map<string,int> mp;//����·����Tree�ڵ��Ӧ��
    map<string,MySemaphore *> W;
    map<string,MySemaphore *> RW;
    map<string,MySemaphore *> Mutex;
    map<string,int> Count;
    MySemaphore *Cal=new MySemaphore();
    //MySemaphore *W=new MySemaphore(),*RW=new MySemaphore(),*Mutex=new MySemaphore();

    void Read_P(const string &path){
        if(!W.count(path)) W[path]=new MySemaphore();
        W[path]->P();
        if(!Mutex.count(path)) Mutex[path]=new MySemaphore();
        Mutex[path]->P();
        if(!Count.count(path)) Count[path]=0;
        if(Count[path]==0){
            if(!RW.count(path)) RW[path]=new MySemaphore();
            RW[path]->P();
        }
        Count[path]++;
        W[path]->V();
        Mutex[path]->V();
    }
    void Read_V(const string& path){
        Mutex[path]->P();
        Count[path]--;
        if(Count[path]==0){
            RW[path]->V();
        }
        Mutex[path]->V();
    }
    void Write_P(const string &path){
        if(!W.count(path)) W[path]=new MySemaphore();
        W[path]->P();
        if(!RW.count(path)) RW[path]=new MySemaphore();
        RW[path]->P();
    }
    void Write_V(const string path){
        RW[path]->V();
        W[path]->V();
    }
	long long count_size(int now){//������ļ��д�С����bug��
		for(auto nxt:tr[now].next){
			long long num;
            num=count_size(nxt);
			tr[now].size+=num;
		}
		return tr[now].size;
	}
	void write(int x,int y,const string& file_name){
		while(output[x].size()<=y+1){
				output[x]+=' ';
			}
		output[x]+=file_name;
	}
	void add_line(int bx,int ex,int y){
		for(int i=bx;i<=ex;i++){
			while(output[i].size()<=y+1){
				output[i]+=' ';
			}
			output[i][y]='|';
		}
	}
	void add_other(int x,int y,int op){
		if(op==0) output[x][y]='-';
		else output[x][y]=' ';
	}
	int dfs(int now,int x,int y,int dep){
		mx=max(mx,x);
		tr[now].x=x;tr[now].y=y;
		write(x,y,tr[now].file_name);
        if(dep==1) return mx;
		int las=0;
		for(int i=0;i<tr[now].next.size();i++){
			int nxt=tr[now].next[i];

			if(tr[nxt].next.size()==0){
				if(i==tr[now].next.size()-1) add_line(x+1,mx+1,y+4);
                mx=dfs(nxt,mx+1,y+4,dep+1);
				las=0;
			}
			else{
				if(i==tr[now].next.size()-1) add_line(x+1,mx+1,y+4);
				mx=dfs(nxt,mx+1,y+4,dep+1);
				las=1;
			}
		}
		for(int i=0;i<tr[now].next.size();i++){
			int nxt=tr[now].next[i];
			add_other(tr[nxt].x,y+5,(tr[nxt].next.size()!=0));
		}
		return mx;
	}
	void del_file(int now){
		for(int i=0;i<tr[now].next.size();i++){
			int nxt=tr[now].next[i];
			del_file(nxt);
		}
		mp.erase(tr[now].file_path);
		Res.push_back(now);
		tr[now].next.clear();
		tr[now].file_path="";
	}
    void del_rev(int now,int fa){
        for(int i=0;i<tr[fa].next.size();i++){
            int num=tr[fa].next[i];
            if(num==now){
                tr[fa].next.erase(tr[fa].next.begin()+i);
                //swap(tr[fa].next[i],tr[fa].next.back());
                //tr[fa].next.pop_back();
                break;
            }
        }
    }
    void rename_subfile(int now,string s,int len,int dep){
        for(int i=0;i<tr[now].next.size();i++){
            int nxt=tr[now].next[i];
            rename_subfile(nxt,s,len,dep+1);
        }
        string path=tr[now].file_path;
        if(dep==0) return;
        path.erase(path.begin(), path.begin()+len);


        int val=mp[tr[now].file_path];
        mp.erase(tr[now].file_path);
        tr[now].file_path=s+path;
        mp[tr[now].file_path]=val;
    }
public:
    long long FileSize(const char* fname){
        struct stat statbuf;
        if(stat(fname,&statbuf)==0)
            return statbuf.st_size;
        return -1;
    }
	void test(){
		for(int i=0;i<=all;i++){
			if(tr[i].file_path=="") continue;
			cout<<"filename:  "<<tr[i].file_name<<"\n";
			cout<<"filepath:  "<<tr[i].file_path<<"\n";
			cout<<"file_size: "<<tr[i].size<<"\n";
            cout<<"file_type: "<<tr[i].file_type<<"\n";
		}
	}
	int Print_Files(string path){//����ļ�Ŀ¼��
        Read_P(path);
        int now= find_node(path);
        if(now==-1) return -1;
        for(int i=0;i<=mx;i++){
            output[i].clear();
        }

        dfs(now,1,0,0);
		for(int i=0;i<=mx;i++){
			cout<<output[i]<<endl;
		}

        Read_V(path);
        return 1;
	}
    vector<FileTree> Get_Metadata(string path){//current dictory
        Read_P(path);

        int now= find_node(path);
        if(now==-1) return{};
        vector<FileTree> res;
        for(auto i:tr[now].next){
            res.push_back(tr[i]);
        }

        Read_V(path);
        return res;
    }
    void Print_Metadata(vector<int> v){
        Read_P("/");
        for(auto i:v){
            cout<<"filename:  "<<tr[i].file_name<<"\n";
            cout<<"filepath:  "<<tr[i].file_path<<"\n";
            cout<<"file_size: "<<tr[i].size<<"\n";
            cout<<"file_type: "<<tr[i].file_type<<"\n"<<"\n\n";
        }
        Read_V("/");
    }
	bool Get_FileMetadata(string filepath,FileTree &node){//��ȡ�ļ���Ϣ
        Read_P(filepath);
		if(!mp.count(filepath)) {Read_V(filepath);return false;}
		node=tr[mp[filepath]];
        Read_V(filepath);
		return true;
	}
    void add_node(string file_name,string file_path,int size,int file_type,int parent){
        Cal->P();
        int id=++all;
        Cal->V();

        FileTree node=FileTree(file_name,file_path,size,file_type);
        node.parent=parent;
        mp[file_path]=id;
        tr.push_back(node);

        if(parent==-1) return;
        tr[parent].next.push_back(id);
    }
	GetFiles(string filePath){//��ʼ��
		mx=0;
        all=-1;

        string path=filePath;
        if(path.back()=='/'&&path.size()>1) path.pop_back();
        string s=path,w;
        while(s.back()!='/') w+=s.back(),s.pop_back();
        if(!w.size()) w+='/';
        reverse(w.begin(),w.end());

        add_node(w,filePath,0,1,-1);
//        tr[now].file_name=tr[now].file_path=path;
//        tr[now].size=0;tr[now].file_type=1;
//        mp[path]=now;
//        tr[now].parent=-1;
//        tr[now].file_name=w;

	}
	bool Exit_Path(string filepath){
		return mp.count(filepath);
	}
    int find_node(string path){
        if(mp.count(path)) return mp[path];
        else return -1;
    }
    void update_size(int now,int size){
        if(tr[now].parent==-1) return;
        tr[tr[now].parent].size+=size;
        update_size(tr[now].parent,size);
    }
	int  Add_Path(string filepath,FileTree node){
        Write_P(filepath);

		string path=filepath;
        int type=node.file_type;
        if(Exit_Path(filepath)) {Write_V(filepath);return 0;}   //file exits
		while(path.size()&&path.back()!=sign) {
			path.pop_back();//�������linux�����£�Ҫ�ĳ� "\/";
		}
		if(path.back()==sign&&path.size()>1) path.pop_back();
        if(!Exit_Path(path)) {Write_V(filepath);return -1;}//not route
		if(Exit_Path(path)&&tr[find_node(path)].file_type!=1) {Write_V(filepath);return -1;} //not folder

		int nod=mp[path];
        add_node(node.file_name,node.file_path,node.size,node.file_type,nod);

//		tr[nod].next.push_back(++all);
//		tr[all]=node;
//		tr[all].parent=nod;
//		mp[filepath]=all;
//		change=1;
        update_size(all,node.size);
        Write_V(filepath);
        return 1;
	}
	bool Del_Path(string filepath){//
        Write_P(filepath);

        if(filepath.back()==sign&&filepath.size()>1) filepath.pop_back();
		if(!Exit_Path(filepath)) {Write_V(filepath);return false;}//can not found
		int now=mp[{filepath}];
		int fa=tr[now].parent,size=tr[now].size;
        update_size(now,-size);
        del_rev(now,fa);
		del_file(mp[{filepath}]);

        Write_V(filepath);
        return true;//delete success
	}
    bool Rename_Path(string old_path,string filepath){
        string a=old_path,b=filepath;
        Write_P(a);
        Write_P(b);

        string path=filepath;
        if(old_path==filepath) {Write_V(a);Write_V(b);return true;}//do not change
        if(Exit_Path(filepath)) {Write_V(a);Write_V(b);return false;}   //already exits
        while(path.size()&&path.back()!=sign) {
            path.pop_back();
        }
        if(path.back()==sign&&path.size()>1) path.pop_back();
        if(!Exit_Path(path)) {Write_V(a);Write_V(b);return false;}//not route
        if(Exit_Path(path)&&tr[find_node(path)].file_type!=1) {Write_V(a);Write_V(b);return false;} //not folder

        int now= find_node(old_path);
        if(now==-1) {Write_V(a);Write_V(b);return false;}//add failed

        int len=tr[now].file_path.size();
        int val=mp[tr[now].file_path];

        update_size(now,-tr[now].size);
        mp.erase(tr[now].file_path);
        tr[now].file_path=filepath;
        mp[{tr[now].file_path}]=val;
        string w;
        while(filepath.back()!='/') w+=filepath.back(), filepath.pop_back();
        std::reverse(w.begin(), w.end());
        if(!w.size()) {Write_V(a);Write_V(b);return false;}//add failed
        tr[now].file_name=w;
        int fa=tr[now].parent;
        del_rev(now,fa);

        tr[find_node(path)].next.push_back(now);
        tr[now].parent=find_node(path);
        update_size(now,tr[now].size);
        rename_subfile(now,tr[now].file_path,len,0);

        Write_V(a);
        Write_V(b);
        return true;//add success
    }
};
//path_type; 1:folder 0:file
//GetFiles *d=new GetFiles(string filePath)   creat a new empty directory,root directory is filePath
//d->print_Files(string filePath)             print filePath's subdirectories, if Directory tree has no such path, return -1;
//d->Add_Path(string filePath, FileTree node) add filepath into directory tree,node contain it's basic information, if can't add ,return -1;
//d->Del_Path(string filePath,int type)       delete filpath from directory, if Directory tree has no such path, return -1;
//d->FileSize(char *path)                     return filesize;
//d->test()                                   print all the file's information
//vector<FileTree> Get_Metadata(string path,int type)
//int main() {
//    FileTree it;
//    GetFiles *d = new GetFiles("/");
//    d->Print_Files("/");
//    //创建目录A
//    d->Add_Path("/tmp_dir",FileTree("tmp_dir","/tmp_dir",0,1));
//    d->Print_Files("/");
//    d->Rename_Path("/tmp_dir","/A");
//    //d->Rename_Path("/dir","/A/B/C");
//    d->Print_Files("/");
//
//
//}