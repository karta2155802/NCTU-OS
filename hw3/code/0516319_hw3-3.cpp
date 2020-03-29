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
    int arr[2][p_num],burst[p_num],q;
    for(int i=0;i<2;i++){
        for(int j=0;j<p_num;j++){
            fin>>arr[i][j];
        }
    }
    for(int i=0;i<p_num;i++){
        burst[i]=arr[1][i];
    }
    fin>>q;
    fin.close();

    int output[2][p_num];
    int t=0,curr=-1,wt=0,tt=0,remain=p_num;
    int que[101],que_tail=0,arrived[p_num]={0};
    int r=q;

    while(remain>0){
        for(int i=0;i<p_num;i++){
            if(t>=arr[0][i] && arrived[i]==0){
                que[que_tail]=i;
                que_tail++;
                arrived[i]=1;
            }
        }
        /*cout<<t<<" "<<que_tail<<" ";
        for(int a=0;a<que_tail;a++){
            cout<<que[a];
        }
        cout<<endl;*/

        if(que_tail>0){
            curr=que[0];
            arr[1][curr]--;
            r--;
            if(arr[1][curr]==0){
                t++;
                output[1][curr]=t-arr[0][curr];
                output[0][curr]=output[1][curr]-burst[curr];
                remain--;
                r=q;
                for(int k=0;k<que_tail-1;k++){
                    que[k]=que[k+1];
                }
                que_tail--;
            }
            else if(r==0){
                t++;
                for(int i=0;i<p_num;i++){
                    if(t>=arr[0][i] && arrived[i]==0){
                        que[que_tail]=i;
                        que_tail++;
                        arrived[i]=1;
                    }
                }
                r=q;
                for(int k=0;k<que_tail-1;k++){
                    que[k]=que[k+1];
                }
                que[que_tail-1]=curr;
            }
            else t++;
        }
        else t++;
        //cout<<t<<" "<<curr<<" "<<arr[1][curr]<<" "<<r<<endl;
    }

    fstream fout;
    fout.open("ans3.txt",ios::out);
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
