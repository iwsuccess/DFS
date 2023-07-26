#include <sys/io.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <dirent.h>
#include "bits/stdc++.h"
#include <sys/stat.h>
using namespace std;
char sign='/';//linux��������'\/'
string sig="/";
struct FileTree{
	string file_name;
	string file_path;
	long long  size=0;
    int file_type;     // 1: folder; 0: file
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
    FileTree(const FileTree& f)
    :file_name(f.file_name)
    ,file_path(f.file_path)
    ,size(f.size)
    ,file_type(f.file_type)
    {
    }
};
class GetFiles{
private:
	int all;
	int mx;
	int change;
	int parent[1000000];//
    int op;
	string output[102400];
	FileTree tr[1000000];
	vector<int> Res;//��ɾ���Ľڵ��Ǵ��ڿ���״̬�������ظ����� 
	map<string,int> mp;//����·����Tree�ڵ��Ӧ��
    void getFiles_linux(string path, std::vector<std::string>& files,int now){
//        struct dirent {
//            ino_t          d_ino;       /* inode number */
//            off_t          d_off;       /* offset to the next dirent */
//            unsigned short d_reclen;    /* length of this record */
//            unsigned char  d_type;      /* type of file; not supported
//                                              by all file system types */
//            char           d_name[256]; /* filename */
//        };

        DIR *dir;
        struct dirent *ptr;
        struct stat statbuf;
        if ((dir = opendir(path.c_str())) == NULL){
            //printf("error: ");
            //cout<<path<<endl;
            //perror("Open dir error...");
            return;
        }



        while ((ptr = readdir(dir)) != NULL)
        {
            if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)    ///current dir OR parrent dir
                continue;

            std::string strFile;
            strFile = path;
            if(path.back()!='/') strFile += "/";
            strFile += ptr->d_name;

            if(ptr->d_type==DT_REG||ptr->d_type==DT_DIR) {
                //cout<<path<<endl;
                tr[now].next.push_back(++all);
                tr[all].file_name = ptr->d_name;
                tr[all].file_path = strFile;
                mp[tr[all].file_path] = all;
                parent[all] = now;
                tr[all].size = FileSize(tr[all].file_path.c_str());
                if (ptr->d_type == 4){
                    files.push_back(strFile);
                    getFiles_linux(strFile,files,all);
                }
                else{
                    continue;
                }
            }


        }
        closedir(dir);
        return;
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
		else output[x][y]='-';
	}
	int dfs(int now,int x,int y,int dep){//�ݹ黭���ļ�Ŀ¼��
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
        path.erase(path.begin(), path.begin()+len);

        if(dep==0) return;
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
        int now= find_node(path);
        if(now==-1) return -1;
        for(int i=0;i<=mx;i++){
            output[i].clear();
        }
        change=mx=0;

		if(op) dfs(now,1,0,0);
		for(int i=0;i<=mx;i++){
			cout<<output[i]<<endl;
		}
        return 1;
	}
    vector<FileTree> Get_Metadata(string path){//current dictory
        int now= find_node(path);
        if(now==-1) return{};
        vector<FileTree> res;
        for(auto i:tr[now].next){
            res.push_back(tr[i]);
        }
        return res;
    }
    FileTree Single_Metadata(string path){
        FileTree node;
        if(find_node(path)==-1) return node;
        return tr[find_node(path)];
    }
    FileTree Single_Metadata(int t){
        FileTree node;
        if(t<0) return node;
        return tr[t];
    }
    vector<FileTree> Batch_Metadata(vector<int> v){
        vector<FileTree> g;
        for(auto i:v) g.push_back(tr[i]);
        return g;
    }
    void Print_Metadata(vector<int> v){
        for(auto i:v){
            cout<<"filename:  "<<tr[i].file_name<<"\n";
            cout<<"filepath:  "<<tr[i].file_path<<"\n";
            cout<<"file_size: "<<tr[i].size<<"\n";
            cout<<"file_type: "<<tr[i].file_type<<"\n\n";
        }
    }
	bool Get_FileMetadata(string filepath,FileTree node){//��ȡ�ļ���Ϣ
		if(!mp.count(filepath)) return false;
		node=tr[mp[filepath]];
		return true;
	}
    GetFiles()=default;
	GetFiles(string filePath){//��ʼ��
		all=mx=change=op=0;
		vector<string> files;

        int now=0;
        string path=filePath;
        if(path.back()=='/'&&path.size()>1) path.pop_back();
        tr[now].file_name=tr[now].file_path=path;
        tr[now].size=0;tr[now].file_type=1;
        mp[path]=now;
        parent[now]=-1;
        string s=path,w;
        while(s.back()!='/') w+=s.back(),s.pop_back();
        if(!w.size()) w+='/';
        reverse(w.begin(),w.end());
        tr[now].file_name=w;
        tr[now].size=FileSize(tr[now].file_path.c_str());
		//getFiles_linux(filePath, files,0);

		//count_size(0);
	}
	bool Exit_Path(string filepath){
		return mp.count(filepath);
	}
    int find_node(string path){
        if(mp.count(path)) return mp[path];
        else return -1;
    }
	int Add_Path(string filepath,FileTree node){//���ļ�����·��
		string path=filepath;
        if(Exit_Path(filepath)) return 0;   //already exits
		while(path.size()&&path.back()!=sign) {
			path.pop_back();//�������linux�����£�Ҫ�ĳ� "\/";
		}
		if(path.back()==sign&&path.size()>1) path.pop_back();
		if(!Exit_Path(path))
        {
            std::cout<<"不存在改路径"<<path<<std::endl;
            return -1;
        } 
        if(tr[find_node(path)].file_type==0) 
        {
            std::cout<<"不是目录"<<path<<std::endl;
             return false;//not folder
        }


		int nod=mp[path];
		tr[nod].next.push_back(++all);
		tr[all]=node;
		parent[all]=nod;
		mp[filepath]=all;
		change=1;
        return 1;
	}
	bool Del_Path(string filepath){//ɾ��·��
        if(filepath.back()==sign&&filepath.size()>1) filepath.pop_back();
		if(!Exit_Path(filepath)) return false;
		int now=mp[filepath];
		int fa=parent[now];
        del_rev(now,fa);
		del_file(mp[filepath]);
		change=1;
	}
    bool Rename_Path(string old_path,string filepath){
        string path=filepath;
        if(Exit_Path(filepath)) return false;   //already exits
        while(path.size()&&path.back()!=sign) {
            path.pop_back();//�������linux�����£�Ҫ�ĳ� "\/";
        }
        if(path.back()==sign&&path.size()>1) path.pop_back();
        if(!Exit_Path(path)) return false;
        if(tr[find_node(path)].file_type==0) return false;//not folder

        int now= find_node(old_path);
        if(now==-1) return false;

        int len=tr[now].file_path.size();
        int val=mp[tr[now].file_path];
        mp.erase(tr[now].file_path);
        tr[now].file_path=filepath;
        mp[tr[now].file_path]=val;
        string w;
        while(filepath.back()!='/') w+=filepath.back(), filepath.pop_back();
        std::reverse(w.begin(), w.end());
        if(!w.size()) return -1;
        tr[now].file_name=w;
        int fa=parent[now];
        del_rev(now,fa);

        tr[find_node(path)].next.push_back(now);
        rename_subfile(now,tr[now].file_path,len,0);
    }
};
//GetFiles *d=new GetFiles(string filePath)   creat a new empty directory,root directory is filePath
//d->print_Files(string filePath)             print filePath's subdirectories, if Directory tree has no such path, return -1;
//d->Add_Path(string filePath, FileTree node) add filepath into directory tree,node contain it's basic information, if can't add ,return -1;
//d->Del_Path(string filePath)                delete filpath from directory, if Directory tree has no such path, return -1;
//d->FileSize(char *path)                     return filesize;
//d->test()                                   print all the file's information
// int main() {
//     GetFiles *d = new GetFiles("/");
//     d->Print_Files("/");
//     //创建目录A
//     d->Add_Path("/A", FileTree("A", "/A", 2, 1));

//     d->Print_Files("/");
//     cout << "---------" << endl;
//     d->Add_Path("/A/C", FileTree("C", "/A/C", 2, 0));
//     d->Print_Files("/A");
//     cout << "---------" << endl;

//     d->Add_Path("/B", FileTree("B", "/B", 2, 1));
//     d->Add_Path("/B", FileTree("B", "/B", 2, 0));
//     d->Print_Files("/");
//     cout << "---------" << endl;


//     //cout<<"test----------------"<<endl;

//     FileTree f1;
//     d->Get_FileMetadata("/B",f1);
//     cout<<f1.file_name<<" "<<f1.file_path<<" " <<f1.size<<endl;
//     cout<<"test----------------"<<endl;
// //    d->Rename_Path("/B","BB");
// //    FileTree f;
// //    d->Get_FileMetadata("/BB",f);
// //    cout<<f.file_name<<" "<<f.file_path<<" " <<f.size<<endl;
//     cout<<"test----------------"<<endl;

//     d->Rename_Path("/A","/B/C");
//     d->Print_Files("/B");
//     d->Print_Files("/");
//     cout<<"test----------------"<<endl;
//     d->Print_Files("/B/C");
//     //vector<int >v=d->Get_Metadata("/");
//    // d->Print_Metadata(v);

// }