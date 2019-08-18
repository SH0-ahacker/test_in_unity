#include <stdio.h>
#include <stdbool.h>

void declare_of_regions(int *i);
void declare_of_programs(int *i);
void sort_array(int *arr,int n);

int main()
{

	int num_regions=0;//holds number of regions
	int num_pro=0;//holds number of programs
	
	declare_of_regions(&num_regions);
	declare_of_programs(&num_pro);

	int main_mem_arr[4][(num_pro*10)];//use to store mem size with their mem region no
	int mem_buffer[4][num_regions];//use to store regions data used in main coding
	int output_arr[4][num_pro];//final output data store here
	int turnaround_time=0;

	int max_region_size=0;

	//*********** Take Region Sizes ************
	int arr_size[num_regions];// declaration of all regions
	
	int ss;
	int y=1;
	while(y<=num_regions)
	{
		printf("Enter Region %d Size = ",y);		
		scanf("%d",&ss);
		if(ss<=0)
		{
			printf("Enter Valid Size\n");
		}
		else
		{
			arr_size[(y-1)]=ss;
			if(max_region_size < ss)
			{			
				max_region_size = ss;
			}
			y++;
		}
	}
	//*****************  End  *****************
	
	//********** Take Programs Info ***********
	int arr_data[num_pro][20];//program data store here
	int p=1;

	while(p<=num_pro)
	{
		int pairs_c=0;
		printf("Program %d :-",p);
		printf("No Pairs = ");
		scanf("%d",&pairs_c);

		if(pairs_c > 0)
		{
			arr_data[(p-1)][0] = pairs_c;
			int u=1;
			int z,mem_s,mem_t;
			for(z=1;z<=pairs_c;)
			{	
				scanf("%d %d",&mem_s,&mem_t);
				if(mem_s>max_region_size)
				{
					printf("Invalid Memory Size(Max = %d)",max_region_size);
				}
				else
				{
					arr_data[(p-1)][u]=mem_s;
					arr_data[(p-1)][(u+1)]=mem_t;
					z++;
					u=u+2;
				}
			}
			p++;
		}
		else
		{
			printf("Enter Valid Pair");
		}
	}
	
	//*****************  End  *****************
	
	printf("\n************ | DATA |  ***************\n");
	printf("%d   %d\n",num_regions,num_pro);
	int l,xx;
	for(l=0;l<num_regions;l++)
	{
		mem_buffer[0][l] = 0;//set in-use
		mem_buffer[1][l] = l;//store region no
		mem_buffer[2][l] = arr_size[l];//store region size
		mem_buffer[3][l] = 0;//store used time
		printf("%d   ",arr_size[l]);
	}
	printf("\n");
	p=0;//main_mem x-index count
	for(l=0;l<num_pro;l++)
	{
		output_arr[0][l]=-1;//initialize output arr program no
		int pp=arr_data[l][0];
		printf("%d   ",pp);
		for(xx=1;xx<(pp*2);)
		{
			main_mem_arr[0][p] = 0;//set in-use
			main_mem_arr[1][p] = l;//store program no
			main_mem_arr[2][p] = arr_data[l][xx];//store program size
			main_mem_arr[3][p] = arr_data[l][(xx+1)];//store program time			
			printf("%d   %d   ",arr_data[l][xx],arr_data[l][(xx+1)]);
			xx=xx+2;
			p++;
		}
		printf("\n");
	}
		
	printf("************ | DATA |  ***************\n\n");

	//----[--  Main Coding  --]-------------------------

	for(y=1;y<p;y++)//Sort Memory Regions
	{
	     int i=0;	     
     	     while(main_mem_arr[3][y] > main_mem_arr[3][i])
             {
	         i++;
             }
	     
	     int key_a=main_mem_arr[0][y];//store key availability
	     int key_r=main_mem_arr[1][y];//store key Program/Region no
	     int key_s=main_mem_arr[2][y];//store key size
	     int key_t=main_mem_arr[3][y];//store key time
	     
	     int z;
	     for(z=0;z<=(y-i);z++)
	     {
		    main_mem_arr[0][(y-z)] = main_mem_arr[0][(y-z-1)];//exchange region av
		    main_mem_arr[1][(y-z)] = main_mem_arr[1][(y-z-1)];//exchange Program/Region no
		    main_mem_arr[2][(y-z)] = main_mem_arr[2][(y-z-1)];//exchange size
		    main_mem_arr[3][(y-z)] = main_mem_arr[3][(y-z-1)];//exchange time
	     }
	    
	     main_mem_arr[0][i] = key_a;//exchange key av
	     main_mem_arr[1][i] = key_r;//exchange key Program/Region no
	     main_mem_arr[2][i] = key_s;//exchange key size
	     main_mem_arr[3][i] = key_t;//exchange key time	   
	}

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>	
	
	int pro_count=0;
	while(pro_count < num_pro)
	{
		int y1;	
		for(y1=0;y1<p;y1++)//loop through main_mem_arr
		{
			int y5;
			if(y1%num_regions == 0)
			{
				if(y1 != 0)//skip the initial loop
					sort_array((int *)mem_buffer,num_regions);
				for(y5=0;y5<num_regions;y5++)
				{
					mem_buffer[0][y5] = 0;
				}	
			}
			if(main_mem_arr[0][y1] == 0)//check used or not
			{
				int y2;
				bool skip=false;
				for(y2=0;y2<num_pro;y2++)//use to check already used or not
				{
					
					if(output_arr[0][y2] == main_mem_arr[1][y1])//check current main mem arr program used or not
					{
						skip=true;
						break;					
					}
				}
				if(skip == true)
					continue;
				else
				{
					int y3,diff=0,mem_no;
					bool found=false;
					for(y3=0;y3<num_regions;y3++)//loop through mem_buffer
					{
						if((mem_buffer[0][y3] == 0) && (mem_buffer[2][y3] >= main_mem_arr[2][y1]))//check memory region not in used and memory size is sufficient
						{	
							if(found == false)
							{
								diff = mem_buffer[2][y3] - main_mem_arr[2][y1];
								mem_no = y3;//pass mem_duffer position
								found=true;	
							}
							else if(diff >= (mem_buffer[2][y3] - main_mem_arr[2][y1]))//check for the closest difference
							{
								diff = mem_buffer[2][y3] - main_mem_arr[2][y1];
								mem_no = y3;//pass mem_duffer position						
							}					
						}						

					}					
					main_mem_arr[0][y1] = 1;//set main mem arr as used (1 = used, 0 = not used)
					mem_buffer[0][mem_no] = 1;//set mem buffer as used (1 = used, 0 = not used)
					output_arr[0][pro_count] = main_mem_arr[1][y1];//pass program no
					output_arr[1][pro_count] = mem_buffer[1][mem_no];//pass region no
					output_arr[2][pro_count] = mem_buffer[3][mem_no];//pass start time
					output_arr[3][pro_count] = main_mem_arr[3][y1];//pass end time
					pro_count++;//update program count
					turnaround_time+=(mem_buffer[3][mem_no] + main_mem_arr[3][y1]);//update turnaround time
					mem_buffer[3][mem_no] += main_mem_arr[3][y1];//update time
				}
			}
		}
	}
	

	//----[--  Main Coding End --]-------------------------

	for(y=1;y<num_pro;y++)//Sort Output_array according to program number
	{
	     int i=0;	     
     	     while(output_arr[0][y] > output_arr[0][i])
             {
	         i++;
             }
	     
	     int key_a=output_arr[0][y];//store key program no
	     int key_r=output_arr[1][y];//store key Region no
	     int key_s=output_arr[2][y];//store key start
	     int key_t=output_arr[3][y];//store key end
	     
	     int z;
	     for(z=0;z<=(y-i);z++)
	     {
		    output_arr[0][(y-z)] = output_arr[0][(y-z-1)];//exchange program no
		    output_arr[1][(y-z)] = output_arr[1][(y-z-1)];//exchange Region no
		    output_arr[2][(y-z)] = output_arr[2][(y-z-1)];//exchange start
		    output_arr[3][(y-z)] = output_arr[3][(y-z-1)];//exchange end
	     }
	    
	     output_arr[0][i] = key_a;//exchange key program no
	     output_arr[1][i] = key_r;//exchange key Region no
	     output_arr[2][i] = key_s;//exchange key start
	     output_arr[3][i] = key_t;//exchange key end	   
	}


	double t_t = (double)turnaround_time/num_pro;
	printf("Average Turnaround Time => %.2f\n\n",t_t);
	for(y=0;y<num_pro;y++)
	{
		printf("Program %d runs in Region %d from %d to %d \n",(output_arr[0][y]+1),(output_arr[1][y]+1),output_arr[2][y],(output_arr[2][y] + output_arr[3][y]));
	}
	//--------------------------------------------------
	printf("\n");		

	return 0;
}




void declare_of_regions(int *i)
{
	printf("Enter Number of Regions = ");
        scanf("%d",&*i);
        if(*i <= 0 || *i > 10)
        {
                printf("Invalid No of Regions (min = 1,max = 10)\n");
		declare_of_regions(&*i);
        }
}

void declare_of_programs(int *i)
{
        printf("Enter Number of Programs = ");
        scanf("%d",&*i);
        if(*i <= 0 || *i > 50)
        {
                printf("Invalid No of Programs (min = 1,max = 50)\n");
                declare_of_programs(&*i);
        }
}

//+++++++++++++++++++  [ INSERTION SORT ]  ++++++++++++++++++++++++++++++++
void sort_array(int *arr,int n)
{
	int y;
	for(y=1;y<n;y++)//Sort Memory Regions
	{
	     int i=0;	     
     	     while(*((arr+3*n) + y) > *((arr+3*n) + i))
             {
	         i++;
             }
	     
	     int key_a=*((arr+0*n) + y);//store key availability
	     int key_r=*((arr+1*n) + y);//store key Program/Region no
	     int key_s=*((arr+2*n) + y);//store key size
	     int key_t=*((arr+3*n) + y);//store key time
	     
	     int z;
	     for(z=0;z<=(y-i);z++)
	     {
		    *((arr+0*n) + (y-z)) = *((arr+0*n) + (y-z-1));//exchange region av
		    *((arr+1*n) + (y-z)) = *((arr+1*n) + (y-z-1));//exchange Program/Region no
		    *((arr+2*n) + (y-z)) = *((arr+2*n) + (y-z-1));//exchange size
		    *((arr+3*n) + (y-z)) = *((arr+3*n) + (y-z-1));//exchange time
	     }
	    
	     *((arr+0*n) + i) = key_a;//exchange key av
	     *((arr+1*n) + i) = key_r;//exchange key Program/Region no
	     *((arr+2*n) + i) = key_s;//exchange key size
	     *((arr+3*n) + i) = key_t;//exchange key time	   
	}
}
//+++++++++++++++++++  [ INSERTION SORT END ]  ++++++++++++++++++++++++++++
