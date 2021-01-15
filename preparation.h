#include<iostream>
#include<cstring>
#include<string>
#include<cstdlib>
#include<ctime>
#include<map>
#include<queue>
#include<vector>
#include"thingsaboutbmp.h"
using namespace std;
int M=0,N=0,x=0,y=0,end_sign=0,end_mark=0,cnt=1,comparison[25000],reference[50000],xt1,xt2,yt1,yt2,n1,n2,x0,y0;
char op;
map<string,puzzle_info**>savetable;
map<string,bool>check;
string flag;

struct node  //搜索的节点 
{
	int function;
	char order[2500];
	int state[10][10];
	string feature; 
	int step;
	int xp;
	int yp;
};

node start;
node end;

bool operator>(node a, node b)  //重载运算符 
{
	if(a.function>=b.function)  return true;
    return false; 
}

priority_queue<node,vector<node>,greater<node> >que;  //建立优先级队列 
priority_queue<node,vector<node>,greater<node> >quee;

template<typename T>void give_initnum(T *arr,int size)  //初始化 
{
	for(int i=0;i<size;i++)  arr[i]=-2;
}

void DisplayV()
{
	cout<<"************************操作说明******************************\n";
	cout<<"W、A、S、D：-1块分别向上下左右移动" <<endl;
	cout<<"T：任意交换两个方块"<<endl;
	cout<<"G：保存当前状态的图像" <<endl;
	cout<<"F：自动复原非最短路(支持M和N均不超过10)"<<endl;
	cout<<"U：自动复原最短路(支持M<=4且N<=4)"<<endl;
	cout<<"R：重玩游戏"<<endl;
	cout<<"V：操作说明"<<endl;
	cout<<"N：新建一个数表"<<endl;
	cout<<"I：存档"<<endl;
	cout<<"O：读档"<<endl;
	cout<<"L：查看排行榜"<<endl;
	cout<<"P：退出游戏"<<endl;
	cout<<"**************************************************************\n";
}

void Displaystart()
{
	cout<<"************************拼图移动游戏******************************\n";
	cout<<"作者：吕天    完成日期：2018/12/31   版本号：v1.0"<<endl;
	cout<<"******************************************************************\n";
	DisplayV();
}

void getRandomTable(puzzle_info** type, int dim_1, int dim_2)  //获得随机数表 
{
	srand(time(NULL));
	memset(reference,0,sizeof(reference));
	for(int i=0;i<dim_1;i++)
	{
		for(int j=0;j<dim_2;j++)
		{
			int r=rand()%(dim_1*dim_2);
			while(reference[r]==1)  r=rand()%(dim_1*dim_2);
			type[i][j].number=r;
			reference[r]=1;
			if(r==0)
			{
				x=i;  //记录空白块的位置 
				y=j;
			}
		}
	}
}

void displayTable(puzzle_info** type, int dim_1, int dim_2)
{
	for(int i=0;i<dim_1;i++)
	{
		for(int j=0;j<dim_2;j++)
		{
			cout<<type[i][j].number-1<<"\t"; 
		}
		cout<<endl;
	}
}

bool is_complete(puzzle_info** type, int dim_1, int dim_2)
{
	for(int i=0;i<dim_1;i++)
	{
		for(int j=0;j<dim_2;j++)
		{
			if(i==M-1&&j==N-1)  if(type[i][j].number==0) return true;
			if(type[i][j].number!=1+j+dim_2*i)  return false;
		}
	}
}

void move_W(puzzle_info** type, int posit_x, int posit_y)
{
	if(posit_x==0)  //特判 
	{
		cout<<"无效操作！"<<endl;
		return ;
	}
	swap(type[posit_x-1][posit_y],type[posit_x][posit_y]);
}

void move_A(puzzle_info** type, int posit_x, int posit_y)
{
	if(posit_y==0)
	{
		cout<<"无效操作！"<<endl;
		return ;
	}
	swap(type[posit_x][posit_y-1],type[posit_x][posit_y]);
}

void move_S(puzzle_info** type, int posit_x, int posit_y)
{
	if(posit_x==M-1)
	{
		cout<<"无效操作！"<<endl;
		return ;
	}
	swap(type[posit_x+1][posit_y],type[posit_x][posit_y]);
}

void move_D(puzzle_info** type, int posit_x, int posit_y)
{
	if(posit_y==N-1)
	{
		cout<<"无效操作！"<<endl;
		return ;
	}
	swap(type[posit_x][posit_y+1],type[posit_x][posit_y]);
}

void copy_table(puzzle_info** targ, puzzle_info** deri, int dim_1, int dim_2)
{
	for(int i=0;i<dim_1;i++)
	{
		for(int j=0;j<dim_2;j++)
		{
			targ[i][j]=deri[i][j];
		}
	}
}

void saveThrougMap(string name, puzzle_info** table)
{
	savetable.insert(pair<string,puzzle_info**>(name,table));  //建立映射 
}

bool is_completable(puzzle_info** type, int dim_1, int dim_2)
{
	int inversion_num=0;
	for(int i=0;i<dim_1;i++)  //整理成一个序列 
	{
		for(int j=0;j<dim_2;j++)
		{
			comparison[j+i*dim_2]=type[i][j].number;
	    }
	}
	for(int i=1;i<dim_1*dim_2;i++)  //计算逆序数 
	{
		for(int j=i-1;j>=0;j--)
		{
			if(comparison[i]!=0&&comparison[i]<comparison[j])  inversion_num++;
		}
	}
	if(dim_1==1||dim_2==1)  //M或N等于1的特判 
	{
		if(inversion_num==0)
		{
			cout<<"该状态可解！"<<endl;
			return true;
		}
		else cout<<"该状态不可解！"<<endl;
		return false;
	}
	else
	{
		if(dim_2%2==0)  //列为偶数 
		{
			if((inversion_num+dim_1-1-x)%2==0)	
			{
				cout<<"该状态可解！"<<endl;
				return true;
			}
			else cout<<"该状态不可解！"<<endl;
			return false;
		}
		else  //列为奇数 
		{
			if(inversion_num%2==0)	
			{
				cout<<"该状态可解！"<<endl;
				return true;
			}
			else cout<<"该状态不可解！"<<endl;
			return false;
		}
	}
}

void bfs(puzzle_info** a, puzzle_info** b, int dim_1, int dim_2)
{
	cout<<"正在搜索解决方案......"<<endl; 
	int x1=x,y1=y,finish=0,final=0;
	copy_table(a,b,dim_1,dim_2);
	for(int i=0;i<dim_1;i++)  //计算初始节点的信息 
	{
		for(int j=0;j<dim_2;j++)
		{ 
			if(a[i][j].number!=i*dim_1+j+1&&a[i][j].number!=0) 
			start.function++;
			start.feature+='!'+a[i][j].number;
		} 
	}
	displayTable(a,dim_1,dim_2);
	memset(start.order,0,sizeof(start.order));
	start.step=-1;
	start.function=0;
	start.xp=x;
	start.yp=y; 
	for(int i=0;i<dim_1;i++)  //记录数表的值 
	{
		for(int j=0;j<dim_2;j++)
		{
			start.state[i][j]=a[i][j].number;
		}
	}
	que.push(start);  //压入队列 
	check[start.feature]=true;  //建立映射 
	while(!finish)
	{
		node tmp=que.top();  //取出节点 
		que.pop();  //移除队列中节点 
		int tmpx=tmp.xp, tmpy=tmp.yp;
		for(int i=0;i<dim_1;i++)
		{
			for(int j=0;j<dim_2;j++)
			{
				a[i][j].number=tmp.state[i][j];
			}
		}
		for(int i=0;i<4;i++)
		{
			int f=0;
			if(i==0&&tmpx!=0&&end_mark==0)  //以W方向展开的节点 
			{
				string kp;
				move_W(a,tmpx,tmpy);
				for(int j=0;j<dim_1;j++)
				{
					for(int k=0;k<dim_2;k++)
					{
						if(a[j][k].number!=0)
						f+=abs((a[j][k].number-1)/dim_2-j)+abs((a[j][k].number-1)%dim_2-k);
						kp+=a[j][k].number+'!';
					}
				}
				if(kp==flag)  //已经达到目标状态 
				{
					strcpy(end.order,tmp.order);
					end.order[tmp.step+1]='W';
					end.step=tmp.step+1;
					end_mark=1;
				}
				if(check.count(kp)==0)  //判重 
				{
					cnt++;
					node tmpp;
					for(int j=0;j<dim_1;j++)
					{
						for(int k=0;k<dim_2;k++)
						{
							tmpp.state[j][k]=a[j][k].number;
						}
					}
					tmpp.feature=kp;
					tmpp.step=tmp.step+1;
					tmpp.xp=tmpx-1;
					tmpp.yp=tmpy;
					tmpp.function=f;
					strcpy(tmpp.order,tmp.order); //拷贝步骤 
					tmpp.order[tmpp.step]='W'; //记录步骤 
					que.push(tmpp);
					check[kp]=true;  //建立映射 
				}
				move_S(a,tmpx-1,tmpy);  //回溯 
			}
			if(i==1&&tmpy!=0&&end_mark==0)
			{
				string kp;
				move_A(a,tmpx,tmpy);
				for(int j=0;j<dim_1;j++)
				{
					for(int k=0;k<dim_2;k++)
					{
						if(a[j][k].number!=0)
						f+=abs((a[j][k].number-1)/dim_2-j)+abs((a[j][k].number-1)%dim_2-k);
						kp+=a[j][k].number+'!';
					}
				}
				if(kp==flag) 
				{
					strcpy(end.order,tmp.order);
					end.order[tmp.step+1]='A';
					end.step=tmp.step+1;
					end_mark=1;
				}
				if(check.count(kp)==0)
				{
					cnt++;
					node tmpp;
					tmpp.feature=kp;
					for(int j=0;j<dim_1;j++)
					{
						for(int k=0;k<dim_2;k++)
						{
							tmpp.state[j][k]=a[j][k].number;
						}
					}
					tmpp.step=tmp.step+1;
					tmpp.xp=tmpx;
					tmpp.yp=tmpy-1;
					tmpp.function=f;
					strcpy(tmpp.order,tmp.order);
					tmpp.order[tmpp.step]='A';
					que.push(tmpp);
					check[kp]=true;
				}
				move_D(a,tmpx,tmpy-1);
			}
			if(i==2&&tmpx!=dim_1-1&&end_mark==0)
			{
				string kp;
				move_S(a,tmpx,tmpy);
				for(int j=0;j<dim_1;j++)
				{
					for(int k=0;k<dim_2;k++)
					{
						if(a[j][k].number!=0)
						f+=abs((a[j][k].number-1)/dim_2-j)+abs((a[j][k].number-1)%dim_2-k);
						kp+=a[j][k].number+'!';
					}
				}
				if(kp==flag) 
				{
					strcpy(end.order,tmp.order);
					end.order[tmp.step+1]='S';
					end.step=tmp.step+1;
					end_mark=1;
				}
				
				if(check.count(kp)==0)
				{
					cnt++;
					node tmpp;
					tmpp.feature=kp;
					for(int j=0;j<dim_1;j++)
					{
						for(int k=0;k<dim_2;k++)
						{
							tmpp.state[j][k]=a[j][k].number;
						}
					}
					tmpp.step=tmp.step+1;
					tmpp.xp=tmpx+1;
					tmpp.yp=tmpy;
					tmpp.function=f;
					strcpy(tmpp.order,tmp.order);
					tmpp.order[tmpp.step]='S';
					que.push(tmpp);
					check[kp]=true;
				}
				move_W(a,tmpx+1,tmpy);
			}
			if(i==3&&tmpy!=dim_2-1&&end_mark==0)
			{
				string kp;
				move_D(a,tmpx,tmpy);
				for(int j=0;j<dim_1;j++)
				{
					for(int k=0;k<dim_2;k++)
					{
						if(a[j][k].number!=0)
						f+=abs((a[j][k].number-1)/dim_2-j)+abs((a[j][k].number-1)%dim_2-k);
						kp+=a[j][k].number+'!';
					}
				}
				if(kp==flag) 
				{
					strcpy(end.order,tmp.order);
					end.order[tmp.step+1]='D';
					end.step=tmp.step+1;
					end_mark=1;
				}
				if(check.count(kp)==0)
				{
					cnt++; 
					node tmpp;
					tmpp.feature=kp;
					for(int j=0;j<dim_1;j++)
					{
						for(int k=0;k<dim_2;k++)
						{
							tmpp.state[j][k]=a[j][k].number;
						}
					}
					tmpp.step=tmp.step+1;
					tmpp.xp=tmpx;
					tmpp.yp=tmpy+1;
					tmpp.function=f;
					strcpy(tmpp.order,tmp.order);
					tmpp.order[tmpp.step]='D';
					que.push(tmpp);
					check[kp]=true;
				}
				move_A(a,tmpx,tmpy+1);
			}
//			cout<<f<<endl; 
			if(end_mark==1)
			{
				cout<<end.step<<endl;
				copy_table(a,b,dim_1,dim_2);
				int pri=0;
				x1=x,y1=y;
				while(pri<=end.step)
				{
					cout<<"第"<<pri+1<<"步："; 
					if(end.order[pri]=='W')  
					{
						move_W(a,x1,y1);
						cout<<"向上移动"<<endl;
						x1-=1;
						displayTable(a,M,N);
					}
					else if(end.order[pri]=='A') 
					{
						move_A(a,x1,y1);
						cout<<"向左移动"<<endl;
						y1-=1;
						displayTable(a,M,N);
					} 
					else if(end.order[pri]=='S')
					{
						move_S(a,x1,y1);
						cout<<"向下移动"<<endl;
						x1+=1;
						displayTable(a,M,N);
					}  
					else
					{
						move_D(a,x1,y1);
						cout<<"向右移动"<<endl;
						y1+=1;
						displayTable(a,M,N);
				    } 
				    pri++;
				    cout<<"**************************************************************\n";
				}
				pri=0;
				cout<<"解决方案为：" <<endl; 
				while(pri<=end.step)
				{
					cout<<end.order[pri]<<" ";
					pri++;
				}
				cout<<endl;
				cout<<"总步数为" <<end.step+1<<endl;
				end_mark=0;
				check.clear(); 
				while(!que.empty())  que.pop();
				return;
			}
		}
	}
}

void calculateFlag(int a,int b)  //计算目标状态的特征字符串 
{
	for(int i=1;i<=a*b-1;i++)  
	{
		flag+=char(i+'!');
	}
	flag+='!';
}

void getbitmap(puzzle_info** a, BMPinfoheader infoheader)  //得到打乱的图片 
{
	for(int i=0;i<M;i++)
	{
		for(int j=0;j<N;j++)
		{
			if(a[i][j].number!=0)
			{
				int ccnt=infoheader.height/M;  //确定每一块的高度 
				int l=infoheader.width/N;  //确定每一块的宽度 
				int kk=infoheader.width*(M-1-i)*ccnt/2+j*l/2+1;  //确定应该在的位置的左下角的y坐标 
				int ll=kk+l/2-2;  //确定应该在的位置的右下角的y坐标 
				int d=infoheader.width*(M-1-(a[i][j].number-1)/N)*ccnt/2+((a[i][j].number-1)%N)*l/2-kk;  //求出数字当前位置与应在位置的左下角坐标的差值 
				while(ccnt>3)
				{
					for(int tmp=kk;tmp<ll;tmp++)
					{
						imagecpy[tmp]=image[tmp+d];  //更新图片信息 
					}
					kk+=infoheader.width/2;  //上一层 
					ll+=infoheader.width/2;  //上一层 
					ccnt--;
				}
			}
			if(a[i][j].number==0)  //对于空白块的特判 
			{
				int ccnt=infoheader.height/M;
				int l=infoheader.width/N;
				int kk=infoheader.width*(M-1-i)*ccnt/2+j*l/2;
				int ll=kk+l/2-2;
				while(ccnt>3)
				{
					for(int tmp=kk;tmp<ll;tmp++)
					{
						imagecpy[tmp].R=imagecpy[tmp].G=imagecpy[tmp].B=0;  //全部涂黑 
					}
					kk+=infoheader.width/2;
					ll+=infoheader.width/2;
					ccnt--;
				}
			} 
		}
	}
}

void specialbfs(puzzle_info** a, puzzle_info** b, int dim_1, int dim_2)  //最短路的解 
{
	cout<<"正在搜索最短解决方案......"<<endl; 
	int x1=x,y1=y,finish=0,final=0;
	copy_table(a,b,dim_1,dim_2);
	for(int i=0;i<dim_1;i++)
	{
		for(int j=0;j<dim_2;j++)
		{ 
			if(a[i][j].number!=i*dim_1+j+1&&a[i][j].number!=0) 
			start.function++;
			start.feature+='!'+a[i][j].number;
		} 
	}
	displayTable(a,dim_1,dim_2);
	memset(start.order,0,sizeof(start.order));
	start.step=-1;
	start.function=0;
	start.xp=x;
	start.yp=y; 
	for(int i=0;i<dim_1;i++)
	{
		for(int j=0;j<dim_2;j++)
		{
			start.state[i][j]=a[i][j].number;
		}
	}
	quee.push(start); 
	check[start.feature]=true;
	while(!finish)
	{
		node tmp=quee.top();
		quee.pop();
		int tmpx=tmp.xp, tmpy=tmp.yp;
		for(int i=0;i<dim_1;i++)
		{
			for(int j=0;j<dim_2;j++)
			{
				a[i][j].number=tmp.state[i][j];
			}
		}
		for(int i=0;i<4;i++)
		{
			int f=0;
			if(i==0&&tmpx!=0&&end_mark==0)
			{
				string kp;
				move_W(a,tmpx,tmpy);
				for(int j=0;j<dim_1;j++)
				{
					for(int k=0;k<dim_2;k++)
					{
						if(a[j][k].number!=0)
						f+=abs((a[j][k].number-1)/dim_2-j)+abs((a[j][k].number-1)%dim_2-k);
						kp+=a[j][k].number+'!';
					}
				}
				if(kp==flag) 
				{
					strcpy(end.order,tmp.order);
					end.order[tmp.step+1]='W';
					end.step=tmp.step+1;
					end_mark=1;
				}
				if(check.count(kp)==0)
				{
					cnt++;
					node tmpp;
					for(int j=0;j<dim_1;j++)
					{
						for(int k=0;k<dim_2;k++)
						{
							tmpp.state[j][k]=a[j][k].number;
						}
					}
					tmpp.feature=kp;
					tmpp.step=tmp.step+1;
					tmpp.xp=tmpx-1;
					tmpp.yp=tmpy;
					tmpp.function=f+tmpp.step;  //加上了step 
					strcpy(tmpp.order,tmp.order);
					tmpp.order[tmpp.step]='W';
					quee.push(tmpp);
					check[kp]=true;
				}
				move_S(a,tmpx-1,tmpy);
			}
			if(i==1&&tmpy!=0&&end_mark==0)
			{
				string kp;
				move_A(a,tmpx,tmpy);
				for(int j=0;j<dim_1;j++)
				{
					for(int k=0;k<dim_2;k++)
					{
						if(a[j][k].number!=0)
						f+=abs((a[j][k].number-1)/dim_2-j)+abs((a[j][k].number-1)%dim_2-k);
						kp+=a[j][k].number+'!';
					}
				}
				if(kp==flag) 
				{
					strcpy(end.order,tmp.order);
					end.order[tmp.step+1]='A';
					end.step=tmp.step+1;
					end_mark=1;
				}
				if(check.count(kp)==0)
				{
					cnt++;
					node tmpp;
					tmpp.feature=kp;
					for(int j=0;j<dim_1;j++)
					{
						for(int k=0;k<dim_2;k++)
						{
							tmpp.state[j][k]=a[j][k].number;
						}
					}
					tmpp.step=tmp.step+1;
					tmpp.xp=tmpx;
					tmpp.yp=tmpy-1;
					tmpp.function=f+tmpp.step;
					strcpy(tmpp.order,tmp.order);
					tmpp.order[tmpp.step]='A';
					quee.push(tmpp);
					check[kp]=true;
				}
				move_D(a,tmpx,tmpy-1);
			}
			if(i==2&&tmpx!=dim_1-1&&end_mark==0)
			{
				string kp;
				move_S(a,tmpx,tmpy);
				for(int j=0;j<dim_1;j++)
				{
					for(int k=0;k<dim_2;k++)
					{
						if(a[j][k].number!=0)
						f+=abs((a[j][k].number-1)/dim_2-j)+abs((a[j][k].number-1)%dim_2-k);
						kp+=a[j][k].number+'!';
					}
				}
				if(kp==flag) 
				{
					strcpy(end.order,tmp.order);
					end.order[tmp.step+1]='S';
					end.step=tmp.step+1;
					end_mark=1;
				}
				
				if(check.count(kp)==0)
				{
					cnt++;
					node tmpp;
					tmpp.feature=kp;
					for(int j=0;j<dim_1;j++)
					{
						for(int k=0;k<dim_2;k++)
						{
							tmpp.state[j][k]=a[j][k].number;
						}
					}
					tmpp.step=tmp.step+1;
					tmpp.xp=tmpx+1;
					tmpp.yp=tmpy;
					tmpp.function=f+tmpp.step;
					strcpy(tmpp.order,tmp.order);
					tmpp.order[tmpp.step]='S';
					quee.push(tmpp);
					check[kp]=true;
				}
				move_W(a,tmpx+1,tmpy);
			}
			if(i==3&&tmpy!=dim_2-1&&end_mark==0)
			{
				string kp;
				move_D(a,tmpx,tmpy);
				for(int j=0;j<dim_1;j++)
				{
					for(int k=0;k<dim_2;k++)
					{
						if(a[j][k].number!=0)
						f+=abs((a[j][k].number-1)/dim_2-j)+abs((a[j][k].number-1)%dim_2-k);
						kp+=a[j][k].number+'!';
					}
				}
				if(kp==flag) 
				{
					strcpy(end.order,tmp.order);
					end.order[tmp.step+1]='D';
					end.step=tmp.step+1;
					end_mark=1;
				}
				if(check.count(kp)==0)
				{
					cnt++; 
					node tmpp;
					tmpp.feature=kp;
					for(int j=0;j<dim_1;j++)
					{
						for(int k=0;k<dim_2;k++)
						{
							tmpp.state[j][k]=a[j][k].number;
						}
					}
					tmpp.step=tmp.step+1;
					tmpp.xp=tmpx;
					tmpp.yp=tmpy+1;
					tmpp.function=f+tmpp.step;
					strcpy(tmpp.order,tmp.order);
					tmpp.order[tmpp.step]='D';
					quee.push(tmpp);
					check[kp]=true;
				}
				move_A(a,tmpx,tmpy+1);
			}
//			cout<<f<<endl; 
			if(end_mark==1)
			{
				cout<<end.step<<endl;
				copy_table(a,b,dim_1,dim_2);
				int pri=0;
				x1=x,y1=y;
				while(pri<=end.step)
				{
					cout<<"第"<<pri+1<<"步："; 
					if(end.order[pri]=='W')  
					{
						move_W(a,x1,y1);
						cout<<"向上移动"<<endl;
						x1-=1;
						displayTable(a,M,N);
					}
					else if(end.order[pri]=='A') 
					{
						move_A(a,x1,y1);
						cout<<"向左移动"<<endl;
						y1-=1;
						displayTable(a,M,N);
					} 
					else if(end.order[pri]=='S')
					{
						move_S(a,x1,y1);
						cout<<"向下移动"<<endl;
						x1+=1;
						displayTable(a,M,N);
					}  
					else
					{
						move_D(a,x1,y1);
						cout<<"向右移动"<<endl;
						y1+=1;
						displayTable(a,M,N);
				    } 
				    pri++;
				    cout<<"**************************************************************\n";
				}
				pri=0;
				cout<<"解决方案为：\n" <<endl; 
				while(pri<=end.step)
				{
					cout<<end.order[pri]<<" ";
					pri++;
				}
				cout<<endl;
				cout<<"总步数为" <<end.step+1<<endl;
				end_mark=0;
				check.clear(); 
				while(!quee.empty())  quee.pop();
				return;
			}
		}
	}
}
