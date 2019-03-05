#include <iostream>
#include <math.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <iomanip>
#define maxx 10000000
#define k 3
using namespace std;
class relation
{
public:
    int attribute; //维度
    int total_tuple;
    vector< vector<double> > relationship; //全部数据
    vector< vector<double> > tables[k];   //聚类后的结果
    vector< double > center[k];           //K个中心点
public:
    relation(int x,int y)
    {
        attribute=x;
        total_tuple=y;
    }
    void read_file();        //读取数据函数
    void out_file();
    double mindist(const vector<double> & tu1, const vector<double> & tu2);//闵式距离
    double oudist(const vector<double> & tu1, const vector<double> & tu2);//闵式距离
    void print(vector<double>  temp);
    void ini_center();
    int reset_center();//if change return 1,else return 0
    void kmeans();
};

double relation::mindist(const vector<double> & tu1, const vector<double> & tu2)
{
    int i=0;
    double distant=0.0;
    double temp;
    for(i=1;i<=attribute;i++)
    {
        temp=fabs(tu1[i]-tu2[i]);
        double temp2=attribute-1.0;
        distant=distant+pow(temp,temp2);
    }
    double temp3=1.0/attribute;
    distant=pow(distant,temp3);
    return distant;
}
double relation::oudist(const vector<double> & tu1, const vector<double> & tu2)
{
    int i=0;
    double distant=0.0;
    double temp;
    for(i=1;i<=attribute;i++)
    {
        temp=fabs(tu1[i]-tu2[i]);
        distant=distant+temp*temp;
    }
    distant=sqrt(distant);
    return distant;
}
void relation::print(vector<double>  temp)
{
    int i;
    cout<<"datas of the "<<temp[0]<<" point is ";
    for(i=1;i<=attribute;i++)
    {
        cout<<temp[i];
        cout<<"  ";
    }
    cout<<endl;
}
//Initialization of vectors
void relation::ini_center()
{
    int i,m;
    for(i=0;i<k;i++)
    {
        int j;
        for(j=0;j<=attribute;j++)
        {
            center[i].push_back(relationship[i][j]);
        }
        tables[i].push_back(center[i]);
    }
    for(i=k;i<relationship.size();i++)
    {
        int j;
        double minvalue=maxx+1.0;
        double temppp=0.0;
        int minkey=0;
        for(m=0;m<k;m++)
        {
            temppp=mindist(center[m],relationship[i]);
            if(temppp<=minvalue)
            {
                minvalue=temppp;
                minkey=m;
            }
        }
        tables[minkey].push_back(relationship[i]);
    }
    cout<<"ini"<<endl;
    cout<<"---------------------------------"<<endl;
        for(i=0;i<k;i++)
        {
            cout<<"the center is ";
            print(center[i]);
            for(m=0;m<tables[i].size();m++)
            {
                cout<<"  ";
                 print(tables[i][m]);
            }
        }
        cout<<"---------------------------------"<<endl;
}
//reset center and tables
int relation::reset_center()
{
    int i=0;
    int change=0;
    for(i=0;i<k;i++)
    {
        cout<<"reset center begin:"<<endl;
        print(center[i]);
        vector<double> theory;
        theory.push_back(i);
        int m=0;
        cout<<"table"<<i<<" : "<<endl;
        for(m=1;m<=attribute;m++)
        {
            int j=0;
            double temp=0.0;
            for(j=0;j<tables[i].size();j++)
            {
              temp=temp+tables[i][j][m];
            }
            temp=temp/tables[i].size();
            theory.push_back(temp);
        }
        double minvalue=maxx+1.0;
        double temppp=0.0;
        int minkey=0;
        for(m=0;m<tables[i].size();m++)
        {
            temppp=mindist(theory,tables[i][m]);
            if(temppp<=minvalue)
            {
                minvalue=temppp;
                minkey=m;
            }
        }
        if(center[i][0]!=tables[i][minkey][0])
        {
            change=1;
            cout<<"there is change center "<<i<<"  ";
            center[i]=tables[i][minkey];
            print(center[i]);
        }
    }
    return change;
}
//read the files
void relation::read_file()
{
    int flag=0;
    ifstream infile;
    while(flag==0)
    {
        cout<<"please input read_file name:"<<endl;
        string name;
        cin>>name;
        //name="D:\\test2.txt";
        cout<<name.data();
        infile.open(name.data());
        if(!infile)
            cout<<"cannot open this file"<<endl;
        else flag=1;
    }
    if(flag==1)
    {
        int j=1;
        for(j=1;j<=total_tuple;j++)
        {
            int i=0;
            vector<double> temp;
            temp.push_back(j);
            for(i=1;i<=attribute;i++)
            {
               double x;
               infile>>x;
               temp.push_back(x);
            }
            relationship.push_back(temp);
        }
    }
    cout<<"have read "<<total_tuple<<" datas";
    cout<<endl;
    infile.close();
}
//output the result to files
void relation::out_file()
{
    int flag=0;
    string name;
    name="D:\\result.txt";
    ofstream outfile;
    outfile.open(name.data(),ios::out);
    int i;
    outfile<<attribute<<"  ";
    outfile<<relationship.size()<<endl;
    for(i=0;i<k;i++)
    {
        outfile<<"center "<<i<<" : ";
        outfile<<center[i][0]<<" point is ";
        int j;
        for(j=1;j<=attribute;j++)
        {
        outfile<<center[i][j];
        outfile<<"  ";
        }
        outfile<<endl;
        outfile<<"all points in table "<<i<<" : "<<endl;
        for(j=0;j<tables[i].size();j++)
        {
            outfile<<tables[i][j][0];
            outfile<<" point is ";
            int m;
            for(m=1;m<=attribute;m++)
            {
                outfile<<tables[i][j][m];
                outfile<<"  ";
            }
            outfile<<endl;

        }
        outfile<<endl<<endl<<endl;
    }
}

void relation::kmeans()
{
    ini_center();
    int change=1;
    int countt=0;
    while(change==1)
    {
        change=0;
        int i,j,m;
        cout<<"kmeans  begin !!!!"<<endl;
        change=reset_center();
        for(i=0;i<k;i++)
        {
            tables[i].clear();
        }
        for(i=0;i<relationship.size();i++)
        {
        int j,m;
        double minvalue=maxx+1.0;
        double temppp=0;
        int minkey=0;
        for(m=0;m<k;m++)
        {
            temppp=mindist(center[m],relationship[i]);
            if(temppp<=minvalue)
            {
                minvalue=temppp;
                minkey=m;
            }
        }
        tables[minkey].push_back(relationship[i]);
        }
        countt++;
        cout<<"showing  NO."<<countt<<":"<<endl;
        cout<<"---------------------------------"<<endl;
        for(i=0;i<k;i++)
        {
            cout<<"the center is "<<center[i][0]<<endl;
            for(m=0;m<tables[i].size();m++)
            {
                 print(tables[i][m]);
            }
        }
        cout<<"---------------------------------"<<endl;
        cout<<"showing  NO."<<countt<<":"<<endl;
    }
   if(change==0)
   {
       cout<<"kmeans  is  over !!!!"<<endl;
   }
}
int main()
{
    int total_attribute;
    cout<<"please input total attributes;"<<endl;
    cin>>total_attribute;
    int total_tuple;
    cout<<"please input total tuples;"<<endl;
    cin>>total_tuple;
    relation rela(total_attribute,total_tuple);
    rela.read_file();
    rela.kmeans();
   rela.out_file();
    return 0;
}
