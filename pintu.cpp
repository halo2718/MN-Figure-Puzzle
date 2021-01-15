#include"preparation.h"
string key,code,bre;
char path[500],name[500],ty,recordbreaker[101][25];
long long record[101], starttime, endtime, recordstep[101],stepcnt=0;
bool forbidPicture=true,AImark=false;
/*
5 5
C:\\Users\\ltlt915\\Desktop\\pintu\\true\\erxiaomen.bmp
C:\\Users\\ltlt915\\Desktop\\pintu\\true\\erxiaomenpuzzle.bmp
*/

int main()
{
	srand(time(NULL));
	bmp_header fileheader;  //存文件头 
	BMPinfoheader infoheader;  //存信息头 
	Displaystart();
	cout<<"请输入两个数字来设置难度（两个数字小于等于180）：\n" ;
	cin>>M>>N;
	cout<<"**************************************************************\n";
	cout<<"请输入一个24位bmp图片的路径："<<endl;
	cin>>path;	//绝对路径 
	cout<<"**************************************************************\n";
	cout<<"请输入一个路径用于后续保存新生成的bmp图片："<<endl;
	cin>>name;	//之后保存的图片的绝对路径 
	cout<<"**************************************************************\n";
	FILE*pfin=fopen(path,"rb");  //打开bmp 
	fread(&fileheader,sizeof(bmp_header),1,pfin);   
	fread(&infoheader,sizeof(BMPinfoheader),1,pfin);
	if(fileheader.btype!=19778)  
	{
		cout<<"图片格式不合法，无法正常实现图片功能。"<<endl;  //判断文件是否为bmp
		cout<<"**************************************************************\n";
		forbidPicture=false; //禁用图片功能 
	} 
	else 
	{
		cout<<"图片格式正确！" <<endl; 
		cout<<"**************************************************************\n";
	}
	int sizeofpic=infoheader.width*infoheader.height;  //总像素 
    fread(image,sizeof(RGB_space),sizeofpic,pfin);  //读像素信息
	fclose(pfin); 
	calculateFlag(M,N);
	puzzle_info** puzzle=new puzzle_info* [M];  //建立puzzle 
	for(int i=0;i<M;i++)  puzzle[i]=new puzzle_info [N];
	puzzle_info** storage=new puzzle_info* [M];  //建立一个储存初态的数组 
	for(int i=0;i<M;i++)  storage[i]=new puzzle_info [N];
	puzzle_info** target=new puzzle_info* [M];  //建立一个方便之后操作的数组 
	for(int i=0;i<M;i++)  target[i]=new puzzle_info [N];
	getRandomTable(puzzle,M,N);
	x0=x,y0=y;
	cout<<"生成数表！\n" ;
	displayTable(puzzle,M,N); 
	copy_table(storage,puzzle,M,N);
	FILE*pfinn=fopen("rank.txt","rb");
	int g=0;
	while(fscanf(pfinn,"%lld%lld%s",&record[g],&recordstep[g],recordbreaker[g])!=EOF) g++;
	fclose(pfinn);
	starttime=clock();
	while(!is_completable(puzzle,M,N))
	{
		cout<<"**************************************************************\n";
		char any;
		cout<<"输入字符'C'重新生成数阵，输入字符'E'结束游戏："<<endl;
		cin>>any;
		cout<<"**************************************************************\n";
		if(any=='C') 
		{
			getRandomTable(puzzle,M,N);
			for(int i=0;i<M;i++)
				   for(int j=0;j<N;j++)
				   {
				   	    if(puzzle[i][j].number==0)    x=i,y=j;
				   }
			x0=x,y0=y;
			cout<<"新生成的数表：\n";
			displayTable(puzzle,M,N); 
			copy_table(storage,puzzle,M,N); 
		}
		if(any=='E')  
		{
			cout<<"游戏结束，欢迎再来！"<<endl;
			fclose(pfin);
			return 0;
		}
	}
	while(true)
	{
		if(!is_complete(puzzle,M,N)) cout<<"该状态未完成！\n";
		cout<<"**************************************************************\n";
		cout<<"请输入操作符（如有疑问请输入'V'）：\n" ;
		cin>>op;
		cout<<"**************************************************************\n";
		switch(op)
		{
			case 'W':move_W(puzzle,x,y);if(x!=0) cout<<"向上移动！"<<endl;if(x!=0)x-=1;break;
			case 'A':move_A(puzzle,x,y);if(y!=0) cout<<"向左移动！"<<endl;if(y!=0)y-=1;break;
			case 'S':move_S(puzzle,x,y);if(x!=M-1) cout<<"向下移动！"<<endl;if(x!=M-1)x+=1;break;
			case 'D':move_D(puzzle,x,y);if(y!=N-1) cout<<"向右移动！"<<endl;if(y!=N-1)y+=1;break;
			case 'R':
			{
				copy_table(puzzle,storage,M,N);
				for(int i=0;i<M;i++)
				   for(int j=0;j<N;j++)
				   {
				   	    if(puzzle[i][j].number==0)    x=i,y=j;
				   }
				x0=x,y0=y;
				cout<<"重新开始!\n";
				starttime=clock();
				stepcnt=0;break;
			}
			case 'N':getRandomTable(puzzle,M,N);cout<<"再来一局！"<<endl;is_completable(puzzle,M,N);starttime=clock();stepcnt=0; break;
			case 'I':
			{
				cout<<"请输入新建存档名称:"<<endl; 
				cin>>code;
				puzzle_info** image=new puzzle_info* [M];
				for(int i=0;i<M;i++)  image[i]=new puzzle_info [N];
				copy_table(image,puzzle,M,N);
				savetable[code]=image;
				cout<<"已存入存档！" <<endl;
				break;
			}
			case 'O':
			{
				cout<<"请输入读取存档名称（必须是已经存档过的！）:"<<endl;
				cin>>key;
				if(savetable[key]==NULL)
				{
					cout<<"无效名称！" <<endl;
					break; 
				}
				puzzle=savetable[key];
				for(int i=0;i<M;i++) 
					for(int j=0;j<N;j++)
						if(puzzle[i][j].number==0)  x=i,y=j;
				cout<<"已读出存档！" <<endl;
				break;
			}
			case 'P':end_sign=1;break;
			case 'G':
			{
				if(forbidPicture) 
				{
					FILE*pfout=fopen(name,"wb");
					getbitmap(puzzle,infoheader);
					fwrite(&fileheader,sizeof(bmp_header),1,pfout);
					fwrite(&infoheader,sizeof(BMPinfoheader),1,pfout);
					fwrite(imagecpy,sizeof(RGB_space),sizeofpic/2,pfout);
					fclose(pfout);	
					cout<<"图片已经保存成功，请打开图片查看状态！" <<endl;
				}
				else  cout<<"该功能由于图片格式不合法已被禁用！"<<endl;
				break;
			}
			case 'F':
			{ 
				bfs(target,puzzle,M,N);
				cout<<"节点访问总数:"<<cnt<<endl;
				cnt=1;
				AImark=true;break;
			} 
			case 'T':
			{
				cout<<"请输入要交换的两个数字：\n";
				cin>>n1>>n2;
				for(int i=0;i<M;i++)
				   for(int j=0;j<N;j++)
				   {
				   	    if(puzzle[i][j].number==n1+1)    xt1=i,yt1=j;
				   	    if(puzzle[i][j].number==n2+1)    xt2=i,yt2=j; 
				   }
				swap(puzzle[xt1][yt1],puzzle[xt2][yt2]);
				for(int i=0;i<M;i++) 
					for(int j=0;j<N;j++)
						if(puzzle[i][j].number==0)  x=i,y=j;
				if(is_completable(puzzle,M,N))  cout<<"TP成功！继续加油！"<<endl;
				else cout<<"很遗憾，你让可解的游戏变得不可解了23333" <<endl;
				break;
			}
			case 'U':
			{
				if(M>4||N>4)  cout<<"你想爆内存吗？TAT"<<endl;
				else specialbfs(target,puzzle,M,N);
				cout<<"节点访问总数：" <<endl;
				cnt=1;
				AImark=true;
				break;
			}
			case 'L':
			{
				cout<<"游戏排行榜:\n";
				for(int i=0;i<100;i++)
				{
					cout<<"time of "<<i/10+1<<"*"<<i%10+1<<": "<<record[i]<<"s  ";
	                printf("step: %lld  ",recordstep[i]);
					printf("keeper: %s\n",recordbreaker[i]);
				}
				break;
			}
			case 'V':
			{
				DisplayV();
				break; 
			}
		}
		cout<<"**************************************************************\n";
		stepcnt++;
		if(end_sign==1)
		{
			cout<<"您已退出游戏！"<<endl;
			break; 
		}
		displayTable(puzzle,M,N);
		if(is_complete(puzzle,M,N)) 
		{
			endtime=clock();
			cout<<"恭喜您成功复原了！\n";
			if(!AImark)
			{
				if((endtime-starttime)/CLOCKS_PER_SEC<record[(M-1)*10+N-1])
				{
					cout<<"恭喜您破纪录了！\n"<<"New Record of "<<M<<"*";
					cout<<N<<": "<<(endtime-starttime)/CLOCKS_PER_SEC<<"s"<<endl;
					cout<<"总步数为："<<stepcnt<<endl;
					cout<<"请输入您的大名："<<endl; 
					cin>>bre;
					record[(M-1)*10+N-1]=(endtime-starttime)/CLOCKS_PER_SEC;
					for(int i=0;i<25;i++)  recordbreaker[(M-1)*10+N-1][i]=0;
					for(int i=0;i<bre.length();i++)
					recordbreaker[(M-1)*10+N-1][i]=bre[i];
					recordstep[(M-1)*10+N-1]=stepcnt;
					FILE*foutt=fopen("rank.txt","wb");
					for(int i=0;i<100;i++) fprintf(foutt,"%lld  %lld  %s\r\n",record[i],recordstep[i],recordbreaker[i]);
					fclose(foutt);
				}
				else
				{
					cout<<"下次争取破个记录！"<<endl; 
				}
			}
			else
			{
				cout<<"下次争取自己完成哦！"<<endl;
			}
			cout<<"游戏结束，欢迎再来！" <<endl;
			break;
		}		
	}
	return 0;
}
