#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;
int han(float ave){
    int i;
    for(i=0;i<=4;i++){
        if(ave*10 == (int)ave*10){
            ave=ave*10;
            break;
        }
        else {
            ave=ave*10;
        }
    }
    return i;
}
int main(int argc, char *argv[])
{
    int p_num;
    ifstream fin(argv[1]);
    fin>>p_num;
    int arr[3][101],burst[p_num];
    int q1,q2;
    for(int i=0;i<2;i++){
        for(int j=0;j<p_num;j++){
            fin>>arr[i][j];
        }
    }
    for(int i=0;i<p_num;i++){
        burst[i]=arr[1][i];
        arr[2][i]=-1;
    }
    fin>>q1>>q2;
    fin.close();

    int output[2][p_num];
    int t=0,curr,wt=0,tt=0,remain=p_num;
    int que1[101],que2[101],q1_tail=0,q2_tail=0,arrived[p_num]={0};
    int r1=q1,r2=q2,q3_tail=0;
    arr[1][100]=101;

    while(remain>0){
        for(int i=0;i<p_num;i++){
            if(t>=arr[0][i] && arrived[i]==0){
                que1[q1_tail]=i;
                q1_tail++;
                arrived[i]=1;
                arr[2][i]=1;
            }
        }

        if(q1_tail>0){
            curr=que1[0];
            arr[1][curr]--;
            r1--;
            if(arr[1][curr]==0){
                arr[2][curr]=-1;
                t++;
                output[1][curr]=t-arr[0][curr];
                output[0][curr]=output[1][curr]-burst[curr];
                remain--;
                r1=q1;
                for(int k=0;k<q1_tail-1;k++){
                    que1[k]=que1[k+1];
                }
                q1_tail--;
            }
            else if(r1==0){
                arr[2][curr]=2;
                t++;
                for(int i=0;i<p_num;i++){
                    if(t>=arr[0][i] && arrived[i]==0){
                        que1[q1_tail]=i;
                        q1_tail++;
                        arrived[i]=1;
                    }
                }
                r1=q1;
                for(int k=0;k<q1_tail-1;k++){
                    que1[k]=que1[k+1];
                }
                q1_tail--;
                que2[q2_tail]=curr;
                q2_tail++;
            }
            else t++;
        }
        else if(q2_tail>0){
            curr=que2[0];
            arr[1][curr]--;
            r2--;
            if(arr[1][curr]==0){
                arr[2][curr]=-1;
                t++;
                output[1][curr]=t-arr[0][curr];
                output[0][curr]=output[1][curr]-burst[curr];
                remain--;
                r2=q2;
                for(int k=0;k<q2_tail-1;k++){
                    que2[k]=que2[k+1];
                }
                q2_tail--;
            }
            else if(r2==0){
                arr[2][curr]=3;
                t++;
                for(int i=0;i<p_num;i++){
                    if(t>=arr[0][i] && arrived[i]==0){
                        que2[q2_tail]=i;
                        q2_tail++;
                        arrived[i]=1;
                    }
                }
                r2=q2;
                for(int k=0;k<q2_tail-1;k++){
                    que2[k]=que2[k+1];
                }
                q2_tail--;
                q3_tail++;
            }
            else t++;
        }
        else if(q3_tail>0){
            curr=100;
            for(int i=0;i<p_num;i++){
                if(arr[2][i]==3 && arr[1][i]<arr[1][curr] && arr[1][i]>0){
                        curr=i;
                }
            }
            if(curr!=100){
                t++;
                arr[1][curr]--;
                if(arr[1][curr]==0){
                    output[1][curr]=t-arr[0][curr];
                    output[0][curr]=output[1][curr]-burst[curr];
                    remain--;
                    q3_tail--;
                    arr[2][curr]=-1;
                }
            }
            else t++;
        }
        else t++;
        //cout<<t<<" "<<curr<<" "<<arr[1][curr]<<" "<<r<<endl;
    }





    fstream fout;
    fout.open("ans4.txt",ios::out);
    for(int i=0;i<p_num;i++)
    {
        fout<<output[0][i]<<" ";
        wt=wt+output[0][i];
        fout<<output[1][i]<<endl;
        tt=tt+output[1][i];
    }

    float sum1=(float)wt/p_num, sum2=(float)tt/p_num;

    fout<<fixed<<setprecision(han(sum1))<<sum1<<endl;
    fout<<setprecision(han(sum2))<<sum2<<endl;
    fout.close();
}
