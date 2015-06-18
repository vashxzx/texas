#include"texasv30g.h"


char *deck="23456789TJQKA";//һ���Ƶ�ֵ�ͻ�ɫ
char *suit="CDHS";
int deal[7];
int dealtmp[7]={0}; 
char card[50] = {'\0'}; //���ݸ��ҵ���

extern int my_blind;
extern int fold_num;//�ۼ�ͳ��fold�Ķ�����
extern int check_num;//�ۼ�check������
extern int raise_num;
extern int all_num;
extern int inquire_num_players;
extern int game_round_num;

int bet;
int pot;
int blind;


 int pork_max(int *deal,int n,int m)//Ϊ�˼��㹫����ֵ����
 {
     int max=deal[n]>>2;
     int j;
     for(j=n+1;j<=m;++j)
         if((deal[j]>>2)>max)
             max=(deal[j]>>2);
     return max;
 }
 int suit_num(int *deal,int n,int m )//�ж�m-n+1���ƵĻ�ɫ
 {
     int five=0;
     int four=0;
     int three=0;
     int two=0;
     int count_suit_sub[4]={0};
     int j;
     for(j=n;j<=m;++j)
         ++count_suit_sub[deal[j]&3];
     for(j=0;j<4;++j)
     {
         if(5==count_suit_sub[j])
             five=1;
         else if(4==count_suit_sub[j])
             four=1;
         else if(3==count_suit_sub[j])
             three=1;
         else if(2==count_suit_sub[j])
             two=1;
     }
     if(five)
         return 5;
     else if(four)
         return 4;
     else if(three)
         return 3;
     else if(two)
         return 2;
     else
     return 1;//���Ź����Ʋ������1�����
 }

  int pork_same_num(int *deal,int n,int m)//�ж�m-n+1���Ƶ�����
 {

     int count_sub[13]={0};
     int four=0;
     int three=0;
     int two=0;
     int straight=0;
     int j;
     int i;
     for(j=n;j<=m;++j)
         ++count_sub[deal[j]>>2];
     if(m-n+1>=5)
     {
         for(j=0;j<=8;++j)
         //A2345Ҳ��˳��
         if((count_sub[j]==1&&count_sub[j+1]==1&&count_sub[j+2]==1&&count_sub[j+3]==1&&count_sub[j+4]==1)||(count_sub[0]==1&&count_sub[1]==1&&count_sub[2]==1&&count_sub[3]==1&&count_sub[12]==1))
                straight=1;//˳��
      }
     for(i=0;i<=12;++i)
         {
             if(4==count_sub[i])
                 four=1;
             else if(3==count_sub[i])
                 three =1;
             else if(2==count_sub[i])
                 ++two;
        }
     if(four)
         return 6;
     else if(three&&two)
         return 5;
     else if(straight)
         return 4;
     else if(three)
         return 3;
     else if(2==two)
         return 2;
     else if(two)
         return 1;
     else
         return 0;
 }

  int judgemypork(int pork_num)
  {
      
      int pairs_num=0;//������
      int three_num=0;//������2.11
      int four_num=0;//������0.024
      int flush_num=0;//ͬ����0.197
      int straight_num=0;//˳����0.39
      int count[13]={0};//������ÿ�ֵ������ظ�����

      int count_suit[4]={0}; //������ÿ�ֻ�ɫ���ظ�����

	  
      //***************************************************���Ƿָ���

  if(2==pork_num)//��ǰ��������
  {
    int j,i;
    int *p_deal=deal;
	int round_rate=game_round_num/5;

   for(i=0;i<2 ;++i)
   deal[i]=((strchr(deck,card[2*i+1])-deck)<<2)+(strchr(suit,card[2*i])-suit);



   for(j=0;j<2;j++)
    ++count[p_deal[j]>>2];//���ü���������¼ÿ�����ظ��ĸ���

   for(j=12;j>=0;--j) //��¼�˶��ƶ�Ӧcount���±� j
     if(2==count[j])
     {
          pairs_num=1;
          break;
     }

		 if(pairs_num&&j>=9||((deal[0]>>2)==12&&(deal[1]>>2)==11)||((deal[1]>>2)==12&&(deal[0]>>2)==11))//�������Ƕ�������ֵ����JJ QQ KK AA AK (1)
        {
		 if(((deal[0]>>2)==(deal[1]>>2))&&(deal[0]>=44))//KK AA
			 return raise2;
		 else if(bet<=7*blind)
			 return raise;
		 else if(bet<=(14*blind+19))
			 return call;
		 else
			 return fold;

        }
	 
		 else if(pairs_num) //66 77 88 99 TT (2,3 4 5) �ĳ�1-10
     {
		 
		  if(j>=6)//���Ӵ���6
		  {
			  if(bet<=3*blind+19)
				  return raise;
			  else if(bet<=7*blind+19)
				  return call;
			  else
				  return fold;
		  }
		 else if(bet<=6*blind+19)
			 return check;
         else
             return fold;		 
     }

		else if((deal[0]>>2)>=8&&(deal[1]>>2)>=8)// AQ AJ AT KQ KJ KT QJ QT JT(3 4 5)
	 {
		 if(bet<=3*blind+19)
			 return raise;
		 else if(bet<=7*blind+19)
			 return call;
         else
             return fold;
	 }

     	
		else if(((deal[0]&3)==(deal[1]&3))&&((deal[0]>=28)&&(deal[1]>=28)))//9Ts 9Js 9Qs TJs TQs TKs TAs JQs JKs JAs QKs QAs  
			 {
				 //if(((deal[0]>>2)==7&&(deal[1]>>2)==11)||((deal[1]>>2)==7&&(deal[0]>>2)==11))//9Ks��Ҫ
					// return fold;
				// else
				 if(bet<=4*blind+19)
					return raise;
				 else if(bet<=6*blind+19)
					 return call;
				 else
					 return fold;

			 }

		else if(((deal[0]&3)==(deal[1]&3))&&(deal[0]>=48||deal[1]>=48))//As+9s--As+2s 
				 {
					if(bet<=4*blind+19)
						return call;
					else
						return fold;
				}

		else if((deal[0]&3)==(deal[1]&3))		//ͬ��ɫ	 
			 {
				 if((deal[0]>>2)>=6&&(deal[1]>>2)>=6)
					{
						if(bet<=3*blind+19)
							return raise;
						else if(bet<=7*blind+19)
							return check;
						else
							return fold;
					}

				else
				 {
					 if(round_rate&1)
					 {
						if(bet<=4*blind+19)
						 return call;
						else
						 return fold;
					 }
					 else
						 return fold;
				 }
				 				
			 }
			

			 else
			 {
				 if(my_blind&&!raise_num&&!all_num)
				 {
					 if(round_rate&1)
						 return call;
					 else
						 return fold;
				 }
				 else if(my_blind)
				{
					if(round_rate&1)
					{
						if(bet<=4*blind+19)
							return call;
						else
							return fold;
					}
					else
						return fold;

				 }				
				 else
					 return fold;
			 }			 				 
   
   }
    //**********************************************************************���Ƿָ���

   if(5==pork_num)
   {
       int j,i;
    int *p_deal=deal;
	int round_rate=game_round_num/10;

    for(i=0;i<5;++i)
         deal[i]=((strchr(deck,card[2*i+1])-deck)<<2)+(strchr(suit,card[2*i])-suit);


       for(j=0;j<5;++j)//�����ƻ�ɫ����
           ++count_suit[p_deal[j]&3];

       for(j=0;j<5;j++)//�����Ƶ�������
         ++count[p_deal[j]>>2];

       for(j=0;j<4;++j)
       {
        if(5==count_suit[j])
             flush_num=1;//ͬ��
       }

       for(j=0;j<=8;++j)
           //A2345Ҳ��˳��
           if((count[j]==1&&count[j+1]==1&&count[j+2]==1&&count[j+3]==1&&count[j+4]==1)||(count[0]==1&&count[1]==1&&count[2]==1&&count[3]==1&&count[12]==1))
                  straight_num=1;//˳��

       for(j=12;j>=0;--j)
       {
         if(count[j]==2)
               ++pairs_num;
         else if(3==count[j])
             three_num=1;
         else if(4==count[j])
             four_num=1;

       }
         if(flush_num&&straight_num)//ͬ��˳
                   return raise2;

          else if(four_num)//����ʱֻ��һ���ܹ�������
               return raise2 ;

          else if(three_num&&pairs_num==1)
           {
               
                   return raise2;
               
           }

           else if(flush_num)
                   return raise2;		  
           
           else  if(straight_num)
                   return raise2;

           else if(three_num)//��������ͦ�ߵģ���Ҫ���������
           {
               if((deal[0]>>2)==(deal[1]>>2))
               {
                   if(bet<=8*blind)
                   return raise2 ;
                   else
                   return call;
               }
               else if((deal[2]>>2==deal[3]>>2)&&(deal[2]>>2==deal[4]>>2))//�����ڹ�����
               {
				   if(raise_num||all_num)
                  {
					  if(((deal[0]>>2)==11)&&((deal[1]>>2)==12)||((deal[0]>>2)==12)&&((deal[1]>>2)==11))//AK
					  {
						  if(bet<=(14*blind+19))
							return call;
						  else
							return fold;
					  }

					else if(bet<=10*blind)
						return call;
					else                 
						return fold;
				   }
				   else
					   return call;
               }
               else//������һ���ڹ�����,�Է�����  ��ͬ��  ��˳�� ��«
               {
				   if(raise_num||all_num)
                  {
					  if(((deal[0]>>2)==11)&&((deal[1]>>2)==12)||((deal[0]>>2)==12)&&((deal[1]>>2)==11))
					  {
						  if(bet<=(14*blind+19))
							return call;
						  else
							return fold;
					  }
					  else if(bet<=12*blind)
						return call;
					  else
						return fold;
				   }
				   else
					   return raise2;
               }

           }

             else if(pairs_num==2)//����  0.95
           {
			   
				if(bet>=8*blind)
				 return  check;
			   else
				 return  raise2;


           }

		   //*****************************����
			else if(pairs_num==1&&(deal[0]>>2)==(deal[1]>>2)&&(deal[0]>=36))//JJ QQ KK AA
			 {
				   if(raise_num||all_num)
				   {
					   if(bet>=6*blind+19)
					      return check;
					   else
						  return raise;
				   }					   
				   else
					   return raise;

			 }

            else if(pairs_num==1&&((deal[0]>>2)==(deal[1]>>2)))//С�����ڵ�����
           {
			   int max_number=pork_max(deal,2,4);

			   if(raise_num||all_num)
			  { 
				 if(bet<=10*blind)
				 {
				   if(deal[0]>max_number)
						return call;
				   else
					   return check;
				 }
			    else
                   return fold;
			  }

			   else
				{
					 if(deal[0]>max_number)
						return raise;
					 else
					   return check;
					   
			    }

           }
			
           else if(pairs_num==1)
           { 
			    int pork_same_number=pork_same_num(deal,2,4);
				int suit_number=suit_num(deal,0,4);

			   if(pork_same_number)//�ڹ�������
			   {
				   if(raise_num||all_num)
				   {
					    if(((deal[0]>>2)==11)&&((deal[1]>>2)==12)||((deal[0]>>2)==12)&&((deal[1]>>2)==11))//AK
					    {
						  if(bet<=(14*blind+19))
							return call;
						  else
							return fold;
					    }

						else if(bet<=8*blind+19)				   																   
						{
							if(suit_number==4)							
								return call;

							else 
								 return check;
					    }
					   else
						   return fold;				   
				   }

				   else
					   return call;

			   }

			 


			   else if(!pork_same_number)//�����빫������ɶ���
			   { 
				   if(raise_num||all_num)
				   {
					   if(((deal[0]>>2)==11)&&((deal[1]>>2)==12)||((deal[0]>>2)==12)&&((deal[1]>>2)==11))//AK
					  {
						  if(bet<=(14*blind+19))
							return call;
						  else
							return fold;
					  }

					   else if(bet<=10*blind)
							return call;

					else													
						 return fold;
					   

				   }
				   else
					   return check;
			   }
				
			}

			//*************************************************����
           else
			{
				if(raise_num||all_num)
				{
					 if(((deal[0]>>2)==11)&&((deal[1]>>2)==12)||((deal[0]>>2)==12)&&((deal[1]>>2)==11))//AK
					  {
						  if(bet<=6*blind+19)
							 return raise;
					      else if(bet<=(14*blind+19))
							return call;
						  else
							return fold;
					  }

					 else if(bet<=9*blind+19)
					{
						int suit_number=suit_num(deal,0,4);
						int max_number=pork_max(deal,2,4);

					  if((deal[0]>>2)>max_number||(deal[1]>>2)>max_number)
						return call;
					
					  else if(suit_number==4)
						return check;

					  else if(bet<=8*blind+19)
						return check;

					  else
						return fold;
					}  
				  else 
					return fold;
			  }

				else//�޼�ע�Ķ���
				{
					int max_number=pork_max(deal,2,4);
					int suit_number=suit_num(deal,0,4);

					if(((deal[0]>>2)==11)&&((deal[1]>>2)==12)||((deal[0]>>2)==12)&&((deal[1]>>2)==11))//AK
					  {
						  if(bet<=(14*blind+19))
							return call;
						  else
							return fold;
					  }
					else if((deal[0]>>2)>max_number||(deal[1]>>2)>max_number)
					{
						if(round_rate&1)
							return raise;
						else
							return call;

					}
					else if(suit_number==4)
						return call;
					else
					{
						if(round_rate&1)
							return call;
						else
							return fold;
					}
					
					
				}
		   }                  	      
		 
   }
   //*************************************************************************�Ҿ��Ƿָ��ߣ��������Ƶľ���
  if(6==pork_num)
   {
        int j,i;
		int *p_deal=deal;
		int round_rate=game_round_num/10;

       for(i=0;i<6 ;++i)
         deal[i]=((strchr(deck,card[2*i+1])-deck)<<2)+(strchr(suit,card[2*i])-suit);


       for(j=0;j<6;++j)//6���ƻ�ɫ����
           ++count_suit[p_deal[j]&3];

       for(j=0;j<6;j++)//6���Ƶ�������
         ++count[p_deal[j]>>2];

       for(j=0;j<4;++j)
       {
        if(5<=count_suit[j])
             flush_num=1;//ͬ��
       }

       for(j=0;j<=8;++j)
           //A2345Ҳ��˳��
           if((count[j]&&count[j+1]&&count[j+2]&&count[j+3]&&count[j+4]||count[0]==1&&count[1]==1)&&(count[2]==1&&count[3]==1&&count[12]==1))
                  straight_num=1;//˳��

       for(j=12;j>=0;--j)
       {
         if(2==count[j])
             ++pairs_num;
         else if(3==count[j])
             ++three_num;
         else if(4==count[j])
             four_num=1;

       }
         if(flush_num&&straight_num)//ͬ��˳
                return raise2;

          else if(four_num)//�������ӣ����������
          {
              int pork_same_number=pork_same_num(deal,2,5);//����������

              if(pork_same_number)//���ƺ͹����Ƹ����ţ��Է�Ҳ��������������������33���Է�44������Ϊ3344
               return raise;

              else if(3==pork_same_number )//����һ�����ڹ������У������˾�������������������ڷ��������Ƶ�ʱ������ܳ�Ϊͬ��˳���������ʼ���Ϊ0
                return raise2;

              else if(6==pork_same_number &&((deal[0]>>2)==12||(deal[1]>>2)==12))//���������ڹ�������������A���˾��ʤ��
                 return raise2;
              else//��Ҷ�������������������û��A�����ư�
                  return fold;
         }

          else if(three_num&&pairs_num==1||three_num==2)//�к�«ʱ��ҲҪ�����
           {
                int pork_same_number=pork_same_num(deal,2,5);//�������е�����ͬ���Ƶ���Ŀ
                if(three_num==1)
                {
                   if((deal[0]>>2)==(deal[1]>>2))//�������
					{
						if(3==pork_same_number)//������������,�Է��д��«�����������൱����22 3334
							                    
								return call;                       
							
						else//22 2334
							                     
								return call;							
					}
					else//23 3324�Է����ڳ���ͬ��˳�������бȺ�«�����
                    return raise2;
               }
                else
					{       
						if(raise_num)            
							return call;                   
						else
							return raise;
					 }
          }

		  //****************************ͬ��
           else if(flush_num)
           {
			  
               int pork_same_number=pork_same_num(deal,2,5);//�������е�����ͬ���Ƶ���Ŀ

               if((deal[0]&3)==(deal[1]&3))
               {
                   if(!pork_same_number )//������û�е�����ͬ����
                    return raise;

                   else if(bet<=8*blind)
					   return raise;
				   else
                    return call;
               }
               else//������Ϊ3c5c7c8c�����ֿ���Ϊͬ��˳
                   return raise;
          }

		  //***************************˳��
          else if(straight_num)
                  {
					  int pork_same_number=pork_same_num(deal,2,5);//�е���ˣ���Ϊ������23 4567 ���ֿ����Ǵ�˳�ӣ����ҵ�23 4566ʱ�����Ǻ�«�������������ˣ�����
					  if(raise_num||all_num)
						  return call;
					  else
						  return raise;
					}


		   //***************************�������������
          else if(three_num)
           {
			   int pork_same_number=pork_same_num(deal,2,5);

               if((deal [0]>>2)==(deal[1]>>2))//2��ͬ�����ĵ��ƺ�һ�Ź����������22 2456 0.77
               {
				   if(raise_num||all_num)
					  {
						   if((deal[0]>>2)>=9)//JJ QQ KK AA
							   return raise;
					  

						else if(bet<=10*blind+19)
						  return call;

						else
						  return fold;
					  }
					  else
						  return raise;
                 
               }

			    else if(3==pork_same_number)//�����ڹ�����,�Է�������  ��«  ����
               {
				   if(raise_num||all_num)
				   {
						int max_number=pork_max(deal,2,5);

						if(((deal[0]>>2)==11)&&((deal[1]>>2)==12)||((deal[0]>>2)==12)&&((deal[1]>>2)==11))//AK
					  {
						  if(bet<=(14*blind+19))
							return call;
						  else
							return fold;
					  }
				   else if(bet<=10*blind+19)
						{
							if((deal[0]>>2)>=max_number||(deal[1]>>2)>=max_number)
								return call;
							else
								return check;
						}
                        else                 
                            return fold;

				   }
				   else
					   return call;
               }
               else                       //23 2247 0.9         ���ֿ�����  ˳��  ͬ��  ��«          
				{
					if(raise_num||all_num)
					{
						if(((deal[0]>>2)==11)&&((deal[1]>>2)==12)||((deal[0]>>2)==12)&&((deal[1]>>2)==11))//AK
					  {
						  if(bet<=(14*blind+19))
							return call;
						  else
							return fold;
					  }
						else if(bet<=10*blind+19)
							return call;
						else
							return fold;
					}
					else
						return raise;

				}
           }

		  //**************************������,�������п��ܳ�������
             else if(pairs_num>=2)
           {
              int pork_same_number=pork_same_num(deal,2,5);//�������е�����ͬ���Ƶ���Ŀ

              if((deal [0]>>2)==(deal[1]>>2))//�������
				{
                  if(2==pork_same_number)//22 3344 ���ֺܿ�����  ��«  ����  ����
				  {
					  if(raise_num||all_num)
					  {
						  if((deal[0]>>2)>=9)//JJ QQ KK AA
							   return call;
					  
						  else if(bet<=10*blind+19)
						  return call;

						else
						  return fold;
					  }
					  else
						  return call;

				  }

                  else //22 3345 ���ֿ�����   ����  ˳��  ͬ��   ��«  �������������ܺ�«����������0.67
				  {
					   if(raise_num||all_num)
					  {
						 if((deal[0]>>2)>=9)//JJ QQ KK AA
							   return call;

						else if(bet<=10*blind+19)
						  return call;

						else
						  return fold;
					  }
					  else
						  return raise;
					  
					  
				  }				                     
				}

               else//���Ʋ���
               {
                    if(pork_same_number)//23 2344  23 3466���ֿ����� ����  ˳��  ͬ��  ��«  �������������ܺ�«����������0.75                        
					{
						if(raise_num||all_num)
					  {
						  if(((deal[0]>>2)==11)&&((deal[1]>>2)==12)||((deal[0]>>2)==12)&&((deal[1]>>2)==11))//AK
						{
						  if(bet<=(14*blind+19))
							return call;
						  else
							return fold;
						}
						else if(bet<=10*blind+19)
						  return call;

						else
						  return fold;
					  }

					  else
						  return raise;
						
					}

                    else//23 2345���ֿ�����  ����  ˳��  ͬ��0.68
                    {                      
						if(raise_num||all_num)
					  {
						  if(((deal[0]>>2)==11)&&((deal[1]>>2)==12)||((deal[0]>>2)==12)&&((deal[1]>>2)==11))//AK
						{
						  if(bet<=(14*blind+19))
							return call;
						  else
							return fold;
						}

						else if(bet<=10*blind+19)
						  return call;

						else
						  return fold;
					  }
					  else
						  return raise;                 
                    }
              }
           }

		  //***************************************************һ��
            else if(pairs_num==1)
           {
			   int max_number=pork_max(deal,2,5);
               int pork_same_number=pork_same_num(deal,2,5);

                if(pork_same_number)//�����ڹ�������  �Է������� ���� ���� ˳�� ͬ����
                   {
					   if(raise_num||all_num)
					   {
						   
						   int suit_number=suit_num(deal,0,5);

						   if(((deal[0]>>2)==11)&&((deal[1]>>2)==12)||((deal[0]>>2)==12)&&((deal[1]>>2)==11))//AK 
						{
						  if(bet<=(14*blind+19))
							return call;
						  else
							return fold;
						}

						   else if(suit_number==4)
							{
								if(bet<=10*blind+19)					     
									return call;
								else
									return fold;
							}

						   else if(bet<=10*blind+19)
								return check;

						   else 
								return fold;
							
					     }

						 else 
						   return call;
					   
					  
				   }
				else if(pairs_num==1&&(deal[0]>>2)==(deal[1]>>2)&&(deal[0]>=36))//�����
			   {
				   if(raise_num||all_num)
					   return check;
				   else
					   return call;

			   }

                else //����������    �Է������� ���� ���� ˳�� ͬ����
                {
					if(raise_num||all_num)
					{
						int suit_number=suit_num(deal,0,5);

						if(((deal[0]>>2)==11)&&((deal[1]>>2)==12)||((deal[0]>>2)==12)&&((deal[1]>>2)==11))//AK 
					  {
						  if(bet<=(14*blind+19))
							return call;
						  else
							return fold;
					  }
						else if(suit_number==4)
						{
							if(bet<=10*blind+19)
							   return call;
							else
								return fold;
						}
						else if(((deal[0]>>2)>=max_number)||((deal[0]>>2)>=max_number))
						 {
							if(bet<=10*blind+19)
							   return call;
						  else
						       return fold;
						 }

						else 
						  {						
							if(bet<=10*blind)
								return check;
							else
								return fold;                  
						  }
					}

					else
						return raise;
                }
           }

		   //*************************************����
           else
		   {
			   if(raise_num||all_num)
				{
					if(((deal[0]>>2)==11)&&((deal[1]>>2)==12)||((deal[0]>>2)==12)&&((deal[1]>>2)==11))//AK
					  {
						  if(bet<=(14*blind+19))
							return call;
						  else
							return fold;
					  }
					else if(bet<=10*blind+19)
					{
						int suit_number=suit_num(deal,0,5);
						int max_number=pork_max(deal,2,5);

						if((deal[0]>>2)>max_number||(deal[1]>>2)>max_number)
							return call;

						else if(suit_number==4)
							return check;

						else if(bet<=10*blind+18)
							return check;

						else 
							return fold;
					}

					else
						return fold;
			   }

               else 
				{
						int suit_number=suit_num(deal,0,5);
						int max_number=pork_max(deal,2,5);
						if(((deal[0]>>2)==11)&&((deal[1]>>2)==12)||((deal[0]>>2)==12)&&((deal[1]>>2)==11))//AK
					  {
						  if(bet<=(14*blind+19))
							return call;
						  else
							return fold;
					  }

						else if((deal[0]>>2)>max_number||(deal[1]>>2)>max_number)
							return raise;

						else if(4==suit_number)
							return call;

						else 
							return call;						
							
			   }
			}
			
   }
   
    //**************************************************�ָ��ߣ������Ƕ������Ƶľ���
   if(7==pork_num)
   {
        int j,i;
		int *p_deal=deal;
       for(i=0;i<7;++i)
         deal[i]=((strchr(deck,card[2*i+1])-deck)<<2)+(strchr(suit,card[2*i])-suit);


       for(j=0;j<7;++j)//7���ƻ�ɫ����
           ++count_suit[p_deal[j]&3];

       for(j=0;j<7;j++)//7���Ƶ�������
         ++count[p_deal[j]>>2];

       for(j=0;j<4;++j)
       {
        if(5<=count_suit[j])
             flush_num=1;//ͬ��
       }

       for(j=0;j<=8;++j)
           if(count[j]&&count[j+1]&&count[j+2]&&count[j+3]&&count[j+4]||count[0]==1&&count[1]==1&&count[2]==1&&count[3]==1&&count[12]==1)//A2345Ҳ��˳��
                  straight_num=1;//˳��


       for(j=12;j>=0;--j)
       {
         if(count[j]==2)
               ++pairs_num;
         else if(3==count[j])
             ++three_num;
         else if(4==count[j])
             four_num=1;

       }
           if(flush_num&&straight_num)//ͬ��˳����Ҷ���ͬ��˳���Է��Ǹ���ͬ��˳���ʼ���Ϊ0��~
                return raise2;


          else if(four_num)//�������ӣ����������
          {
              int pork_same_number=pork_same_num(deal,2,6);//�������п�����ɵ�����

              if(pork_same_number)//���ƺ͹����Ƹ����ţ��Է�Ҳ��������������������33���Է�44������Ϊ33445���Է��������ͬ��˳��33456 +78
                    return raise;

              else if(3==pork_same_number )//����һ�����ڹ�������
                  {
                    if((deal[0]>>2)==(deal[1]>>2))//������ȣ�˵����������һ�Ժ͵�����ȵĵ���Ϊ33������Ϊ44433���Է��������4444��������
                        return raise;

                    else//���Ʋ�ͬ����Ϊ34 33355���Է�����Ϊ5555���Է��������ͬ��˳
                        return raise;
                    }
              else if(6==pork_same_number &&((deal[0]>>2)==12||(deal[1]>>2)==12))//���������ڹ�������������A���˾ֱ�ʤ���Է��޷����ͬ��˳
                  return raise2;

              else//��Ҷ�������������������û��A�����ư�
			  {
				  if(raise_num||all_num)
						return fold;
				  else
					  return call;
			  }
         }

          else if((three_num&&pairs_num)||(three_num==2))//��«���ӣ����������
           {
               int pork_same_number=pork_same_num(deal,2,6);//�����ƿ�����ɵ�����
               if(three_num==1)
			   {
				//�����ƺ�«���Է��ܿ����к�«���������Ǿ���������ͬ��˳
					if(5==pork_same_number)
					{
						 if(((deal[0]>>2)==11)&&((deal[1]>>2)==12)||((deal[0]>>2)==12)&&((deal[1]>>2)==11))//AK 
						{
						  if(bet<=(14*blind+19))
							return call;
						  else
							return fold;
						}
							 
						 else
							return fold;
					 }
               //���������������Է������к�«,��������ͬ��˳
					 else if(3==pork_same_number)
						{                   
							 return call;
						 }
               //�����������ԶԷ������к�«����������ͬ��˳������deal[0],��deal[1]�Ա�ҲӦ�üӽ�ȥ
					else if(2==pork_same_number)
					{                    
                       return call;
					 }
               //��������һ�ԣ��Է������к�«����������ͬ��˳������deal[0],��deal[1]�Ա�ҲӦ�üӽ�ȥ
					 else
						 return raise2;
               }
               else
               {
                   
                  return call;
                  
               }
           }

           else if(flush_num)//ͬ��
           {
               int suit_number=suit_num(deal,2,6);
               //ͬ�������ڹ������У���Ҷ���ͬ���������Ƶ���������ͬ������Է����ֻ���Ǹ����ͬ��ͬ��˳
               if(5==suit_number)
               {
                   //���ƻ�ɫ����һ���빫������ͬ�����Ƶ���ΪA,k
                   if(((deal[0]&3)==(deal[3]&3)&&(deal[0]>>2)>=11)||(((deal[1]&3)==(deal[3]&3))&&((deal[1]>>2)>=11)))
                       return raise2;
                   else
                        return fold;

               }
               //�����л�ɫ��ͬ��˵�����������Ż�ɫ��ͬ
               else if((p_deal[0]&3)!=(p_deal[1]&3))
               {
                  int pork_same_number=pork_same_num(deal,2,6);//�����ƿ�����ɵ�����
                  if(!pork_same_number)//��������Ϊ���ƣ�����ǶԷ���ͬ��˳���������Ʋ�һ���ſ������ͬ��˳�����Զ�һ�Σ���ֻ������ͬ����˳���϶�Ҫ���ģ�
                      return raise2;

                  else//��������һ�ԣ��Է�������ͬ��˳���ߺ�«                  
                      return call;
                  
               }
               //�����л�ɫ��ͬ
               else if((p_deal[0]&3)==(p_deal[1]&3))
               {
                    int pork_same_number=pork_same_num(deal,2,6);//�����ƿ�����ɵ�����
                    if(!pork_same_number)//��������Ϊ����
                        return raise2;

                    else if(1==pork_same_number)//����������һ��
                        return raise;

                    else if(2==pork_same_number)//����������2��,�Է��к�«�������ʷǳ���
                    {
						if(bet<=15*blind)
							return raise2;

						else                       
                            return call;//**********
                        
                    }

                    else if(3==pork_same_number)//����������3�����Է��к�«�������ʷǳ���
					{
						if(bet<=15*blind)
							return raise;

						else
                            return call;//****************
					}
               }
           }

           else  if(straight_num)//˳��
           {
               int pork_same_number=pork_same_num(deal,2,6);//�����ƿ�����ɵ�����
               int suit_number=suit_num(deal,2,6);//�����ƻ�ɫ��
               int max_number=pork_max(deal,2,6);//�����Ƶ������ֵ

               if(4==pork_same_number)//˳���ڹ������У��Է������и����˳��ͬ������ͬ��˳�����ֲ�������ɺ�«������
               {
                   if(2==suit_number)//��������ֻ�����Ż�ɫ��ͬ���ƣ����������ͬ��˳
                   {
					   if(((deal[0]>>2)-max_number==1)||((deal[1]>>2)-max_number==1))
                           return raise2;
                       else
					   {
						   if(bet<=14*blind+19)
							   return check;
						   else
								return fold;
					   }
				   }
                   else //��������ֻ��3�����ϻ�ɫ��ͬ����
                       {
						    if(bet<=14*blind+19)
							   return call;
						   else
								return fold;
						   
						}//�������������������ƽϺ�
               }
               else if(!pork_same_number)//��������ɸ���
                 {
					int suit_number=suit_num(deal,2,6);
					if(raise_num||all_num)
				   {
					   
	                   if(suit_number>=3)//���ֿ���ͬ��
			              return check;                  
					   else 
				          return call;          
				   }

				   else
					   return raise2;
					

			   }
               else if(1==pork_same_number)//��������һ�ԣ��Կ�����ͬ������«��������ͬ��˳23446
                   
				   {
					   if(raise_num||all_num)
					   {
						   if(bet<=14*blind+19)
								return call;//�������0.97
						   else
							   return fold;
					   }
					  
					   else
						   return raise2;
					}

               else if(2==pork_same_number||3==pork_same_number)
                  {
					   if(raise_num)
					   {
						   if(bet<=14*blind+19)
								return call;//�������0.97
						   else
							   return fold;
					   }
					   else if(all_num)
					   {
						   if(bet<=14*blind+19)
							   return call;
						   else
								return fold;
					   }
					   else
						   return raise2;//23434;23444 0.6 0.8
			   }
           }

		   //**************************************����
           else if(three_num)
           {
               int pork_same_number=pork_same_num(deal,2,6);
               if(3==pork_same_number)//���������ڹ�������   �Է�����Ϊ ˳�� ͬ�� ��« ����                
				 {
					 if(((deal[0]>>2)==11)&&((deal[1]>>2)==12)||((deal[0]>>2)==12)&&((deal[1]>>2)==11))//AK
					  {
						  if(bet<=(14*blind+19))
							return call;
						  else
							return fold;
					  }
					 else if(raise_num||all_num)
						return fold;
					else 
						 return call;
			     }

               else if(1==pork_same_number)//������Ϊһ��,�Է� ˳��  ͬ��  ��« 23 22456
               {
                      
                   //int suit_number=suit_num(deal,2,6);
				   if(raise_num||all_num)
				   {
					   if(((deal[0]>>2)==11)&&((deal[1]>>2)==12)||((deal[0]>>2)==12)&&((deal[1]>>2)==11))//AK
					  {
						  if(bet<=(14*blind+19))
							return call;
						  else
							return fold;
					  }

					   else if(bet<=12*blind+19)	                  
			              return call;

					   else 
				          return fold;          
				   }

				   else
					   return raise;
                      
               }
               else if(0==pork_same_number)
			   {
				    int suit_number=suit_num(deal,2,6);
					 if(pairs_num==1&&(deal[0]>>2)==(deal[1]>>2)&&(deal[0]>=36))//�����
					{
					 if(raise_num||all_num)
					   return call;
					 else
					   return raise;

					}
					if(raise_num||all_num)
				   {
					   if(bet<=14*blind+12)
							return call;                  
						else
							return fold;					   	                   
				   }

				   else
					   return raise;
                   
			   }

           }

		   //********************************************����
             else if(pairs_num>=2)
           {
               int pork_same_number=pork_same_num(deal,2,6);

               if(2==pork_same_number)//������������  �Է� ����Ϊ���� ˳�� ͬ�� ��« ������
			   {
				   if(pairs_num==1&&(deal[0]>>2)==(deal[1]>>2)&&(deal[0]>=36))//�����
					{
					 if(raise_num||all_num)
					   return call;
					 else
					   return raise;

					}
				   else if(raise_num||all_num)
				   {
					   if(bet<=12*blind+19)
						return call;
					   else
						return fold;
				   }
				   else
					   return call;
			   }

               else if(pork_same_number)//������Ϊһ��  23 23446  23 34457   �Է� ����Ϊ���� ˳�� ͬ�� ��« ������
               {

				   if(raise_num||all_num)
				   {
					   if(((deal[0]>>2)==11)&&((deal[1]>>2)==12)||((deal[0]>>2)==12)&&((deal[1]>>2)==11))//AK
					  {
						  if(bet<=(14*blind+19))
							return call;
						  else
							return fold;
					  }
					   else  if(bet<=12*blind+19)
						return call;
					   else
						return fold;
				   }
				   else
					   return call;
                        
                   
               }

               else if(!pork_same_number)//�������Ǹ��� 23 23457  ����Ϊ���� ˳�� ͬ��
               {
				   int suit_number=suit_num(deal,2,6);

				   if(suit_number>=3)//���ֿ�����ͬ��
				   {
					   if(raise_num||all_num)						
						{
							if(((deal[0]>>2)==11)&&((deal[1]>>2)==12)||((deal[0]>>2)==12)&&((deal[1]>>2)==11))//AK
						   {
						     if(bet<=(14*blind+19))
							   return call;
						     else
							   return fold;
						    }
							else if(bet<=12*blind+19)
								return call;
							else
								return fold;
						 }	

					   else
							return call;
				   }

				   else if(raise_num||all_num)//���ֿ�����˳��  ����
					   {
						   if(((deal[0]>>2)==11)&&((deal[1]>>2)==12)||((deal[0]>>2)==12)&&((deal[1]>>2)==11))//AK
					      {
						    if(bet<=(14*blind+19))
							  return call;
						    else
							  return fold;
					      }
						   else  if(bet<12*blind+19)
							   return call;
						   else
							   return fold;
						}

				   else                                   
                      return raise;
               }
           }

		   //******************************************һ��
            else if(pairs_num==1)
			{
				int max_number=pork_max(deal,2,6);//�����Ƶ������ֵ
				int pork_same_number=pork_same_num(deal,2,6);
				int suit_number=suit_num(deal,2,6);

				
				 if(pork_same_number)
				{
					if(raise_num||all_num)
					{
						if(((deal[0]>>2)==11)&&((deal[1]>>2)==12)||((deal[0]>>2)==12)&&((deal[1]>>2)==11))//AK
						{
						  if(bet<=(14*blind+19))
							return call;
						  else
							return fold;
						 }
							return fold;
					}
					else
				        return call;
				}
				else if(((deal[0]>>2)>=max_number)||((deal[1]>>2)>=max_number))
				{
					if(raise_num||all_num)
					{
						if(pairs_num==1&&(deal[0]>>2)==(deal[1]>>2)&&(deal[0]>=36))//�����
					
					         return call;
					
						else if(((deal[0]>>2)==11)&&((deal[1]>>2)==12)||((deal[0]>>2)==12)&&((deal[1]>>2)==11))//AK
					   {
						  if(bet<=(14*blind+19))
							return call;
						  else
							return fold;
					    }

						else if(bet<=12*blind+19)
							return call;
						else
							return fold;
					}
					else
						return raise;
				}

				else
					{
						if(raise_num||all_num)	
						{
							if(pairs_num==1&&(deal[0]>>2)==(deal[1]>>2)&&(deal[0]>=36))//�����
							{
							 if(raise_num||all_num)
								 return call;
							 else
							  return raise;

							}
							else if(((deal[0]>>2)==11)&&((deal[1]>>2)==12)||((deal[0]>>2)==12)&&((deal[1]>>2)==11))//AK
							{
							 if(bet<=(14*blind+19))
								return call;
							else
								return fold;
							}
							else if(bet<=12*blind+19)
								return call;
							else
								return fold;
						}
						else
							return call;
					}

			}

           else//��Ȼ�����ⲽ��˵�������Ҷ�ûʲ���ƣ����Է�check�����
		   {
			   int max_number=pork_max(deal,2,6);

			   if(raise_num||all_num)				   
				{ 
					 if(((deal[0]>>2)==11)&&((deal[1]>>2)==12)||((deal[0]>>2)==12)&&((deal[1]>>2)==11))//AK
					 {
						if(bet<=(14*blind+19))
							return call;
						else
							return fold;
					}

					else if(bet<=12*blind+19)
					{
					 if((deal[0]>>2)>=max_number||(deal[1]>>2)>=max_number)
						return call;
					 else
						return check;
					}
					else
						return fold;
				}
			   else
				 {
					 if((deal[0]>>2)>=max_number||(deal[1]>>2)>=max_number)
						return raise;

					 else
						return call;
					 
				}
			}
		}
		
 }//����ĩβ
 