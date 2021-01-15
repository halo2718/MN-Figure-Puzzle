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

struct node  //�����Ľڵ� 
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

bool operator>(node a, node b)  //��������� 
{
	if(a.function>=b.function)  return true;
    return false; 
}

priority_queue<node,vector<node>,greater<node> >que;  //�������ȼ����� 
priority_queue<node,vector<node>,greater<node> >quee;

template<typename T>void give_initnum(T *arr,int size)  //��ʼ�� 
{
	for(int i=0;i<size;i++)  arr[i]=-2;
}

void DisplayV()
{
	cout<<"************************����˵��******************************\n";
	cout<<"W��A��S��D��-1��ֱ������������ƶ�" <<endl;
	cout<<"T�����⽻����������"<<endl;
	cout<<"G�����浱ǰ״̬��ͼ��" <<endl;
	cout<<"F���Զ���ԭ�����·(֧��M��N��������10)"<<endl;
	cout<<"U���Զ���ԭ���·(֧��M<=4��N<=4)"<<endl;
	cout<<"R��������Ϸ"<<endl;
	cout<<"V������˵��"<<endl;
	cout<<"N���½�һ������"<<endl;
	cout<<"I���浵"<<endl;
	cout<<"O������"<<endl;
	cout<<"L���鿴���а�"<<endl;
	cout<<"P���˳���Ϸ"<<endl;
	cout<<"**************************************************************\n";
}

void Displaystart()
{
	cout<<"************************ƴͼ�ƶ���Ϸ******************************\n";
	cout<<"���ߣ�����    ������ڣ�2018/12/31   �汾�ţ�v1.0"<<endl;
	cout<<"******************************************************************\n";
	DisplayV();
}

void getRandomTable(puzzle_info** type, int dim_1, int dim_2)  //���������� 
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
				x=i;  //��¼�հ׿��λ�� 
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
	if(posit_x==0)  //���� 
	{
		cout<<"��Ч������"<<endl;
		return ;
	}
	swap(type[posit_x-1][posit_y],type[posit_x][posit_y]);
}

void move_A(puzzle_info** type, int posit_x, int posit_y)
{
	if(posit_y==0)
	{
		cout<<"��Ч������"<<endl;
		return ;
	}
	swap(type[posit_x][posit_y-1],type[posit_x][posit_y]);
}

void move_S(puzzle_info** type, int posit_x, int posit_y)
{
	if(posit_x==M-1)
	{
		cout<<"��Ч������"<<endl;
		return ;
	}
	swap(type[posit_x+1][posit_y],type[posit_x][posit_y]);
}

void move_D(puzzle_info** type, int posit_x, int posit_y)
{
	if(posit_y==N-1)
	{
		cout<<"��Ч������"<<endl;
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
	savetable.insert(pair<string,puzzle_info**>(name,table));  //����ӳ�� 
}

bool is_completable(puzzle_info** type, int dim_1, int dim_2)
{
	int inversion_num=0;
	for(int i=0;i<dim_1;i++)  //�����һ������ 
	{
		for(int j=0;j<dim_2;j++)
		{
			comparison[j+i*dim_2]=type[i][j].number;
	    }
	}
	for(int i=1;i<dim_1*dim_2;i++)  //���������� 
	{
		for(int j=i-1;j>=0;j--)
		{
			if(comparison[i]!=0&&comparison[i]<comparison[j])  inversion_num++;
		}
	}
	if(dim_1==1||dim_2==1)  //M��N����1������ 
	{
		if(inversion_num==0)
		{
			cout<<"��״̬�ɽ⣡"<<endl;
			return true;
		}
		else cout<<"��״̬���ɽ⣡"<<endl;
		return false;
	}
	else
	{
		if(dim_2%2==0)  //��Ϊż�� 
		{
			if((inversion_num+dim_1-1-x)%2==0)	
			{
				cout<<"��״̬�ɽ⣡"<<endl;
				return true;
			}
			else cout<<"��״̬���ɽ⣡"<<endl;
			return false;
		}
		else  //��Ϊ���� 
		{
			if(inversion_num%2==0)	
			{
				cout<<"��״̬�ɽ⣡"<<endl;
				return true;
			}
			else cout<<"��״̬���ɽ⣡"<<endl;
			return false;
		}
	}
}

void bfs(puzzle_info** a, puzzle_info** b, int dim_1, int dim_2)
{
	cout<<"���������������......"<<endl; 
	int x1=x,y1=y,finish=0,final=0;
	copy_table(a,b,dim_1,dim_2);
	for(int i=0;i<dim_1;i++)  //�����ʼ�ڵ����Ϣ 
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
	for(int i=0;i<dim_1;i++)  //��¼�����ֵ 
	{
		for(int j=0;j<dim_2;j++)
		{
			start.state[i][j]=a[i][j].number;
		}
	}
	que.push(start);  //ѹ����� 
	check[start.feature]=true;  //����ӳ�� 
	while(!finish)
	{
		node tmp=que.top();  //ȡ���ڵ� 
		que.pop();  //�Ƴ������нڵ� 
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
			if(i==0&&tmpx!=0&&end_mark==0)  //��W����չ���Ľڵ� 
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
				if(kp==flag)  //�Ѿ��ﵽĿ��״̬ 
				{
					strcpy(end.order,tmp.order);
					end.order[tmp.step+1]='W';
					end.step=tmp.step+1;
					end_mark=1;
				}
				if(check.count(kp)==0)  //���� 
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
					strcpy(tmpp.order,tmp.order); //�������� 
					tmpp.order[tmpp.step]='W'; //��¼���� 
					que.push(tmpp);
					check[kp]=true;  //����ӳ�� 
				}
				move_S(a,tmpx-1,tmpy);  //���� 
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
					cout<<"��"<<pri+1<<"����"; 
					if(end.order[pri]=='W')  
					{
						move_W(a,x1,y1);
						cout<<"�����ƶ�"<<endl;
						x1-=1;
						displayTable(a,M,N);
					}
					else if(end.order[pri]=='A') 
					{
						move_A(a,x1,y1);
						cout<<"�����ƶ�"<<endl;
						y1-=1;
						displayTable(a,M,N);
					} 
					else if(end.order[pri]=='S')
					{
						move_S(a,x1,y1);
						cout<<"�����ƶ�"<<endl;
						x1+=1;
						displayTable(a,M,N);
					}  
					else
					{
						move_D(a,x1,y1);
						cout<<"�����ƶ�"<<endl;
						y1+=1;
						displayTable(a,M,N);
				    } 
				    pri++;
				    cout<<"**************************************************************\n";
				}
				pri=0;
				cout<<"�������Ϊ��" <<endl; 
				while(pri<=end.step)
				{
					cout<<end.order[pri]<<" ";
					pri++;
				}
				cout<<endl;
				cout<<"�ܲ���Ϊ" <<end.step+1<<endl;
				end_mark=0;
				check.clear(); 
				while(!que.empty())  que.pop();
				return;
			}
		}
	}
}

void calculateFlag(int a,int b)  //����Ŀ��״̬�������ַ��� 
{
	for(int i=1;i<=a*b-1;i++)  
	{
		flag+=char(i+'!');
	}
	flag+='!';
}

void getbitmap(puzzle_info** a, BMPinfoheader infoheader)  //�õ����ҵ�ͼƬ 
{
	for(int i=0;i<M;i++)
	{
		for(int j=0;j<N;j++)
		{
			if(a[i][j].number!=0)
			{
				int ccnt=infoheader.height/M;  //ȷ��ÿһ��ĸ߶� 
				int l=infoheader.width/N;  //ȷ��ÿһ��Ŀ�� 
				int kk=infoheader.width*(M-1-i)*ccnt/2+j*l/2+1;  //ȷ��Ӧ���ڵ�λ�õ����½ǵ�y���� 
				int ll=kk+l/2-2;  //ȷ��Ӧ���ڵ�λ�õ����½ǵ�y���� 
				int d=infoheader.width*(M-1-(a[i][j].number-1)/N)*ccnt/2+((a[i][j].number-1)%N)*l/2-kk;  //������ֵ�ǰλ����Ӧ��λ�õ����½�����Ĳ�ֵ 
				while(ccnt>3)
				{
					for(int tmp=kk;tmp<ll;tmp++)
					{
						imagecpy[tmp]=image[tmp+d];  //����ͼƬ��Ϣ 
					}
					kk+=infoheader.width/2;  //��һ�� 
					ll+=infoheader.width/2;  //��һ�� 
					ccnt--;
				}
			}
			if(a[i][j].number==0)  //���ڿհ׿������ 
			{
				int ccnt=infoheader.height/M;
				int l=infoheader.width/N;
				int kk=infoheader.width*(M-1-i)*ccnt/2+j*l/2;
				int ll=kk+l/2-2;
				while(ccnt>3)
				{
					for(int tmp=kk;tmp<ll;tmp++)
					{
						imagecpy[tmp].R=imagecpy[tmp].G=imagecpy[tmp].B=0;  //ȫ��Ϳ�� 
					}
					kk+=infoheader.width/2;
					ll+=infoheader.width/2;
					ccnt--;
				}
			} 
		}
	}
}

void specialbfs(puzzle_info** a, puzzle_info** b, int dim_1, int dim_2)  //���·�Ľ� 
{
	cout<<"����������̽������......"<<endl; 
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
					tmpp.function=f+tmpp.step;  //������step 
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
					cout<<"��"<<pri+1<<"����"; 
					if(end.order[pri]=='W')  
					{
						move_W(a,x1,y1);
						cout<<"�����ƶ�"<<endl;
						x1-=1;
						displayTable(a,M,N);
					}
					else if(end.order[pri]=='A') 
					{
						move_A(a,x1,y1);
						cout<<"�����ƶ�"<<endl;
						y1-=1;
						displayTable(a,M,N);
					} 
					else if(end.order[pri]=='S')
					{
						move_S(a,x1,y1);
						cout<<"�����ƶ�"<<endl;
						x1+=1;
						displayTable(a,M,N);
					}  
					else
					{
						move_D(a,x1,y1);
						cout<<"�����ƶ�"<<endl;
						y1+=1;
						displayTable(a,M,N);
				    } 
				    pri++;
				    cout<<"**************************************************************\n";
				}
				pri=0;
				cout<<"�������Ϊ��\n" <<endl; 
				while(pri<=end.step)
				{
					cout<<end.order[pri]<<" ";
					pri++;
				}
				cout<<endl;
				cout<<"�ܲ���Ϊ" <<end.step+1<<endl;
				end_mark=0;
				check.clear(); 
				while(!quee.empty())  quee.pop();
				return;
			}
		}
	}
}
