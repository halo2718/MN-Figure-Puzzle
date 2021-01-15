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
	bmp_header fileheader;  //���ļ�ͷ 
	BMPinfoheader infoheader;  //����Ϣͷ 
	Displaystart();
	cout<<"���������������������Ѷȣ���������С�ڵ���180����\n" ;
	cin>>M>>N;
	cout<<"**************************************************************\n";
	cout<<"������һ��24λbmpͼƬ��·����"<<endl;
	cin>>path;	//����·�� 
	cout<<"**************************************************************\n";
	cout<<"������һ��·�����ں������������ɵ�bmpͼƬ��"<<endl;
	cin>>name;	//֮�󱣴��ͼƬ�ľ���·�� 
	cout<<"**************************************************************\n";
	FILE*pfin=fopen(path,"rb");  //��bmp 
	fread(&fileheader,sizeof(bmp_header),1,pfin);   
	fread(&infoheader,sizeof(BMPinfoheader),1,pfin);
	if(fileheader.btype!=19778)  
	{
		cout<<"ͼƬ��ʽ���Ϸ����޷�����ʵ��ͼƬ���ܡ�"<<endl;  //�ж��ļ��Ƿ�Ϊbmp
		cout<<"**************************************************************\n";
		forbidPicture=false; //����ͼƬ���� 
	} 
	else 
	{
		cout<<"ͼƬ��ʽ��ȷ��" <<endl; 
		cout<<"**************************************************************\n";
	}
	int sizeofpic=infoheader.width*infoheader.height;  //������ 
    fread(image,sizeof(RGB_space),sizeofpic,pfin);  //��������Ϣ
	fclose(pfin); 
	calculateFlag(M,N);
	puzzle_info** puzzle=new puzzle_info* [M];  //����puzzle 
	for(int i=0;i<M;i++)  puzzle[i]=new puzzle_info [N];
	puzzle_info** storage=new puzzle_info* [M];  //����һ�������̬������ 
	for(int i=0;i<M;i++)  storage[i]=new puzzle_info [N];
	puzzle_info** target=new puzzle_info* [M];  //����һ������֮����������� 
	for(int i=0;i<M;i++)  target[i]=new puzzle_info [N];
	getRandomTable(puzzle,M,N);
	x0=x,y0=y;
	cout<<"��������\n" ;
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
		cout<<"�����ַ�'C'�����������������ַ�'E'������Ϸ��"<<endl;
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
			cout<<"�����ɵ�����\n";
			displayTable(puzzle,M,N); 
			copy_table(storage,puzzle,M,N); 
		}
		if(any=='E')  
		{
			cout<<"��Ϸ��������ӭ������"<<endl;
			fclose(pfin);
			return 0;
		}
	}
	while(true)
	{
		if(!is_complete(puzzle,M,N)) cout<<"��״̬δ��ɣ�\n";
		cout<<"**************************************************************\n";
		cout<<"���������������������������'V'����\n" ;
		cin>>op;
		cout<<"**************************************************************\n";
		switch(op)
		{
			case 'W':move_W(puzzle,x,y);if(x!=0) cout<<"�����ƶ���"<<endl;if(x!=0)x-=1;break;
			case 'A':move_A(puzzle,x,y);if(y!=0) cout<<"�����ƶ���"<<endl;if(y!=0)y-=1;break;
			case 'S':move_S(puzzle,x,y);if(x!=M-1) cout<<"�����ƶ���"<<endl;if(x!=M-1)x+=1;break;
			case 'D':move_D(puzzle,x,y);if(y!=N-1) cout<<"�����ƶ���"<<endl;if(y!=N-1)y+=1;break;
			case 'R':
			{
				copy_table(puzzle,storage,M,N);
				for(int i=0;i<M;i++)
				   for(int j=0;j<N;j++)
				   {
				   	    if(puzzle[i][j].number==0)    x=i,y=j;
				   }
				x0=x,y0=y;
				cout<<"���¿�ʼ!\n";
				starttime=clock();
				stepcnt=0;break;
			}
			case 'N':getRandomTable(puzzle,M,N);cout<<"����һ�֣�"<<endl;is_completable(puzzle,M,N);starttime=clock();stepcnt=0; break;
			case 'I':
			{
				cout<<"�������½��浵����:"<<endl; 
				cin>>code;
				puzzle_info** image=new puzzle_info* [M];
				for(int i=0;i<M;i++)  image[i]=new puzzle_info [N];
				copy_table(image,puzzle,M,N);
				savetable[code]=image;
				cout<<"�Ѵ���浵��" <<endl;
				break;
			}
			case 'O':
			{
				cout<<"�������ȡ�浵���ƣ��������Ѿ��浵���ģ���:"<<endl;
				cin>>key;
				if(savetable[key]==NULL)
				{
					cout<<"��Ч���ƣ�" <<endl;
					break; 
				}
				puzzle=savetable[key];
				for(int i=0;i<M;i++) 
					for(int j=0;j<N;j++)
						if(puzzle[i][j].number==0)  x=i,y=j;
				cout<<"�Ѷ����浵��" <<endl;
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
					cout<<"ͼƬ�Ѿ�����ɹ������ͼƬ�鿴״̬��" <<endl;
				}
				else  cout<<"�ù�������ͼƬ��ʽ���Ϸ��ѱ����ã�"<<endl;
				break;
			}
			case 'F':
			{ 
				bfs(target,puzzle,M,N);
				cout<<"�ڵ��������:"<<cnt<<endl;
				cnt=1;
				AImark=true;break;
			} 
			case 'T':
			{
				cout<<"������Ҫ�������������֣�\n";
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
				if(is_completable(puzzle,M,N))  cout<<"TP�ɹ����������ͣ�"<<endl;
				else cout<<"���ź������ÿɽ����Ϸ��ò��ɽ���23333" <<endl;
				break;
			}
			case 'U':
			{
				if(M>4||N>4)  cout<<"���뱬�ڴ���TAT"<<endl;
				else specialbfs(target,puzzle,M,N);
				cout<<"�ڵ����������" <<endl;
				cnt=1;
				AImark=true;
				break;
			}
			case 'L':
			{
				cout<<"��Ϸ���а�:\n";
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
			cout<<"�����˳���Ϸ��"<<endl;
			break; 
		}
		displayTable(puzzle,M,N);
		if(is_complete(puzzle,M,N)) 
		{
			endtime=clock();
			cout<<"��ϲ���ɹ���ԭ�ˣ�\n";
			if(!AImark)
			{
				if((endtime-starttime)/CLOCKS_PER_SEC<record[(M-1)*10+N-1])
				{
					cout<<"��ϲ���Ƽ�¼�ˣ�\n"<<"New Record of "<<M<<"*";
					cout<<N<<": "<<(endtime-starttime)/CLOCKS_PER_SEC<<"s"<<endl;
					cout<<"�ܲ���Ϊ��"<<stepcnt<<endl;
					cout<<"���������Ĵ�����"<<endl; 
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
					cout<<"�´���ȡ�Ƹ���¼��"<<endl; 
				}
			}
			else
			{
				cout<<"�´���ȡ�Լ����Ŷ��"<<endl;
			}
			cout<<"��Ϸ��������ӭ������" <<endl;
			break;
		}		
	}
	return 0;
}
